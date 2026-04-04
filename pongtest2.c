#include <stdio.h>

#define WIDTH 80
#define HEIGHT 25
#define PADDLE_HEIGHT 3
#define WIN_SCORE 21

// --- ИНИЦИАЛИЗАЦИЯ ---
void init_round(int* left_y, int* right_y, int* ball_x, int* ball_y, int* ball_dx, int* ball_dy) {
    *left_y = (HEIGHT - PADDLE_HEIGHT) / 2;
    *right_y = (HEIGHT - PADDLE_HEIGHT) / 2;
    *ball_x = WIDTH / 2;
    *ball_y = HEIGHT / 2;
    *ball_dx = -1;
    *ball_dy = 0;
}

void init_game(int* score_left, int* score_right) {
    *score_left = 0;
    *score_right = 0;
}

// --- ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ---
void clear_screen() { printf("\033[2J\033[H"); }

char get_input() {
    char c = getchar();
    while (getchar() != '\n');
    return c;
}

// --- ФУНКЦИИ ОТРИСОВКИ (SMALL DRAW) ---
// Отрисовка горизонтальной границы (верхней или нижней)
void draw_horizontal_border() {
    for (int x = 0; x < WIDTH; x++) {
        printf("%c", (x == 0 || x == WIDTH - 1) ? '|' : '-');
    }
    printf("\n");
}

// Отрисовка одной строки игрового поля
void draw_field_row(int y, int left_y, int right_y, int ball_x, int ball_y) {
    for (int x = 0; x < WIDTH; x++) {
        if (x == 0 || x == WIDTH - 1) {
            printf("|");
        } else if (x == 1) {
            printf("%c", (y >= left_y && y < left_y + PADDLE_HEIGHT) ? '|' : ' ');
        } else if (x == WIDTH - 2) {
            printf("%c", (y >= right_y && y < right_y + PADDLE_HEIGHT) ? '|' : ' ');
        } else if (x == ball_x && y == ball_y) {
            printf("@");
        } else {
            printf(" ");
        }
    }
    printf("\n");
}

void draw(int left_y, int right_y, int ball_x, int ball_y, int score_left, int score_right) {
    clear_screen();
    draw_horizontal_border();

    for (int y = 0; y < HEIGHT; y++) {
        draw_field_row(y, left_y, right_y, ball_x, ball_y);
    }

    draw_horizontal_border();
    printf("Счёт: Игрок 1 — %d | Игрок 2 — %d\n", score_left, score_right);
}

// --- ФУНКЦИИ ЛОГИКИ ИГРЫ (SMALL UPDATE) ---
// Обработка гола (левая сторона)
int handle_left_goal(int* new_ball_x, int* score_right) {
    if (*new_ball_x < 1) {
        (*score_right)++;
        return 1;  // Гол был
    }
    return 0;  // Гола не было
}

// Обработка гола (правая сторона)
int handle_right_goal(int* new_ball_x, int* score_left) {
    if (*new_ball_x > WIDTH - 2) {
        (*score_left)++;
        return 1;  // Гол был
    }
    return 0;  // Гола не было
}

// Обработка отскока от ракеток
void handle_paddle_collision(int new_ball_x, int ball_y, int left_y, int right_y, int* ball_dx,
                             int* ball_dy) {
    // Левая ракетка
    if (new_ball_x == 1) {
        if (ball_y >= left_y && ball_y < left_y + PADDLE_HEIGHT) {
            *ball_dx = 1;
            if (ball_y < left_y + 1)
                *ball_dy = -1;
            else if (ball_y > left_y + 1)
                *ball_dy = 1;
            else
                *ball_dy = 0;
        }
    }
    // Правая ракетка
    else if (new_ball_x == WIDTH - 2) {
        if (ball_y >= right_y && ball_y < right_y + PADDLE_HEIGHT) {
            *ball_dx = -1;
            if (ball_y < right_y + 1)
                *ball_dy = -1;
            else if (ball_y > right_y + 1)
                *ball_dy = 1;
            else
                *ball_dy = 0;
        }
    }
}

// Обработка отскока от стенок
void handle_wall_collision(int new_ball_y, int* ball_dy) {
    if (new_ball_y <= 0 || new_ball_y >= HEIGHT - 1) {
        *ball_dy = -(*ball_dy);
    }
}

void update_game(int* left_y, int* right_y, int* ball_x, int* ball_y, int* ball_dx, int* ball_dy,
                 int* score_left, int* score_right) {
    int new_ball_x = *ball_x + *ball_dx;
    int new_ball_y = *ball_y + *ball_dy;

    // Проверка голов
    if (handle_left_goal(&new_ball_x, score_right)) {
        init_round(left_y, right_y, ball_x, ball_y, ball_dx, ball_dy);
        return;
    }
    if (handle_right_goal(&new_ball_x, score_left)) {
        init_round(left_y, right_y, ball_x, ball_y, ball_dx, ball_dy);
        return;
    }

    // Обработка физики
    handle_paddle_collision(new_ball_x, *ball_y, *left_y, *right_y, ball_dx, ball_dy);
    handle_wall_collision(new_ball_y, ball_dy);

    // Обновляем позицию мяча с учетом отскоков
    new_ball_y = *ball_y + (*ball_dy);

    // Финальное обновление координат
    *ball_x = new_ball_x;
    *ball_y = new_ball_y;
}

int main() {
    int left_y, right_y;
    int ball_x, ball_y;
    int ball_dx, ball_dy;
    int score_left, score_right;

    init_game(&score_left, &score_right);
    init_round(&left_y, &right_y, &ball_x, &ball_y, &ball_dx, &ball_dy);

    while (score_left < WIN_SCORE && score_right < WIN_SCORE) {
        draw(left_y, right_y, ball_x, ball_y, score_left, score_right);
        printf("Игрок 1 (A/Z), Игрок 2 (K/M), Space — пропустить: ");
        char input = get_input();

        if (input == 'A' && left_y > 0)
            left_y--;
        else if (input == 'Z' && left_y + PADDLE_HEIGHT < HEIGHT)
            left_y++;
        else if (input == 'K' && right_y > 0)
            right_y--;
        else if (input == 'M' && right_y + PADDLE_HEIGHT < HEIGHT)
            right_y++;

        update_game(&left_y, &right_y, &ball_x, &ball_y, &ball_dx, &ball_dy, &score_left, &score_right);

        // Задержка для плавности
        for (volatile int i = 0; i < 500000; i++);
    }

    draw(left_y, right_y, ball_x, ball_y, score_left, score_right);

    if (score_left >= WIN_SCORE)
        printf("Поздравляем! Игрок 1 победил!\n");
    else
        printf("Поздравляем! Игрок 2 победил!\n");

    return 0;
}