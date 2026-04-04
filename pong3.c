#include <stdio.h>

#define WIDTH 80
#define HEIGHT 25
#define PADDLE_HEIGHT 3
#define WIN_SCORE 21

int left_y, right_y;
int ball_x, ball_y;
int ball_dx, ball_dy;
int score_left, score_right;

void init_round() {
    left_y = (HEIGHT - PADDLE_HEIGHT) / 2;
    right_y = (HEIGHT - PADDLE_HEIGHT) / 2;
    ball_x = WIDTH / 2;
    ball_y = HEIGHT / 2;
    ball_dx = -1;
    ball_dy = 0;
}

void init_game() {
    score_left = 0;
    score_right = 0;
    init_round();
}

void clear_screen() { printf("\033[2J\033[H"); }

void draw() {
    clear_screen();

    // Верхняя и нижняя границы
    for (int x = 0; x < WIDTH; x++) {
        printf("%c", (x == 0 || x == WIDTH - 1) ? '|' : '-');
    }
    printf("\n");

    // Игровое поле
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++)  // цикл внутри цикла
        {
            if (x == 0 || x == WIDTH - 1) {
                printf("|");
            } else if (x == 1) {
                if (y >= left_y && y < left_y + PADDLE_HEIGHT)
                    printf("|");
                else
                    printf(" ");
            } else if (x == WIDTH - 2) {
                if (y >= right_y && y < right_y + PADDLE_HEIGHT)
                    printf("|");
                else
                    printf(" ");
            } else if (x == ball_x && y == ball_y) {
                printf("@");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }

    // Нижняя граница
    for (int x = 0; x < WIDTH; x++) {
        printf("%c", (x == 0 || x == WIDTH - 1) ? '|' : '-');
    }
    printf("\n");

    printf("Счёт: Игрок 1 — %d | Игрок 2 — %d\n", score_left, score_right);
}

char get_input() {
    char c = getchar();
    while (getchar() != '\n');
    return c;
}

void update_game() {
    int new_ball_x = ball_x + ball_dx;
    int new_ball_y = ball_y + ball_dy;

    // Гол слева (игрок 2 забил)
    if (new_ball_x < 1) {
        score_right++;
        init_round();
        return;
    }

    // Гол справа (игрок 1 забил)
    if (new_ball_x > WIDTH - 2) {
        score_left++;
        init_round();
        return;
    }

    // Столкновение с левой ракеткой
    if (new_ball_x == 1) {
        if (ball_y >= left_y && ball_y < left_y + PADDLE_HEIGHT) {
            new_ball_x = 2;
            ball_dx = 1;
            if (ball_y < left_y + 1)
                ball_dy = -1;
            else if (ball_y > left_y + 1)
                ball_dy = 1;
            else
                ball_dy = 0;
        }
    }

    // Столкновение с правой ракеткой
    if (new_ball_x == WIDTH - 2) {
        if (ball_y >= right_y && ball_y < right_y + PADDLE_HEIGHT) {
            new_ball_x = WIDTH - 3;
            ball_dx = -1;
            if (ball_y < right_y + 1)
                ball_dy = -1;
            else if (ball_y > right_y + 1)
                ball_dy = 1;
            else
                ball_dy = 0;
        }
    }

    // Отскок от верхней и нижней стенок
    if (new_ball_y <= 0 || new_ball_y >= HEIGHT - 1) {
        ball_dy = -ball_dy;
        new_ball_y = ball_y + ball_dy;
    }

    // Обновляем позицию мяча
    ball_x = new_ball_x;
    ball_y = new_ball_y;
}

int main() {
    init_game();

    while (score_left < WIN_SCORE && score_right < WIN_SCORE) {
        draw();
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

        update_game();
    }

    draw();
    if (score_left >= WIN_SCORE)
        printf("Поздравляем! Игрок 1 победил!\n");
    else
        printf("Поздравляем! Игрок 2 победил!\n");

    return 0;
}