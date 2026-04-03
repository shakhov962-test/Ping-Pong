#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25
#define PADDLE_HEIGHT 3
#define WIN_SCORE 21

typedef struct {
    int left_y;
    int right_y;
    int ball_x, ball_y;
    int ball_dx, ball_dy;
    int score_left, score_right;
} GameState;

// Инициализация состояния раунда (очки НЕ сбрасываются)
void init_round(GameState *g) {
    g->left_y = (HEIGHT - PADDLE_HEIGHT) / 2;
    g->right_y = (HEIGHT - PADDLE_HEIGHT) / 2;
    g->ball_x = WIDTH / 2;
    g->ball_y = HEIGHT / 2;
    g->ball_dx = -1; // Начинаем движение влево
    g->ball_dy = 0;
}

// Инициализация всей игры (очки сбрасываются)
void init_game(GameState *g) {
    g->score_left = 0;
    g->score_right = 0;
    init_round(g);
}

void clear_screen() {
    printf("\033[2J\033[H");
}

void draw(const GameState *g) {
    clear_screen();

    // Верхняя и нижняя границы
    for (int x = 0; x < WIDTH; x++) {
        printf("%c", (x == 0 || x == WIDTH - 1) ? '|' : '-');
    }
    printf("\n");

    // Игровое поле
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0 || x == WIDTH - 1) {
                printf("|");
            } else if (x == 1) {
                if (y >= g->left_y && y < g->left_y + PADDLE_HEIGHT)
                    printf("#");
                else
                    printf(" ");
            } else if (x == WIDTH - 2) {
                if (y >= g->right_y && y < g->right_y + PADDLE_HEIGHT)
                    printf("#");
                else
                    printf(" ");
            } else if (x == g->ball_x && y == g->ball_y) {
                printf("O");
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

    printf("Счёт: Игрок 1 — %d | Игрок 2 — %d\n", g->score_left, g->score_right);
}

char get_input() {
    char c = getchar();
    while (getchar() != '\n');
    return toupper(c);
}

void update_game(GameState *g) {
    int new_ball_x = g->ball_x + g->ball_dx;
    int new_ball_y = g->ball_y + g->ball_dy;

    // Проверка гола слева (игрок 2 забил)
    if (new_ball_x < 1) {
        g->score_right++;
        init_round(g);
        return;
    }

    // Проверка гола справа (игрок 1 забил)
    if (new_ball_x > WIDTH - 2) {
        g->score_left++;
        init_round(g);
        return;
    }

    // Столкновение с левой ракеткой
    if (new_ball_x == 1) {
        if (g->ball_y >= g->left_y && g->ball_y < g->left_y + PADDLE_HEIGHT) {
            new_ball_x = 2;
            g->ball_dx = 1;
            if (g->ball_y < g->left_y + 1) g->ball_dy = -1;
            else if (g->ball_y > g->left_y + 1) g->ball_dy = 1;
            else g->ball_dy = 0;
        }
    }

    // Столкновение с правой ракеткой
    if (new_ball_x == WIDTH - 2) {
        if (g->ball_y >= g->right_y && g->ball_y < g->right_y + PADDLE_HEIGHT) {
            new_ball_x = WIDTH - 3;
            g->ball_dx = -1;
            if (g->ball_y < g->right_y + 1) g->ball_dy = -1;
            else if (g->ball_y > g->right_y + 1) g->ball_dy = 1;
            else g->ball_dy = 0;
        }
    }

    // Отскок от верхней и нижней стенок
    if (new_ball_y <= 0 || new_ball_y >= HEIGHT - 1) {
        g->ball_dy = -g->ball_dy;
        new_ball_y = g->ball_y + g->ball_dy;
    }

    // Обновляем позицию мяча, если не было гола и не было отскока от ракетки в этом кадре
    // (в этих случаях мы уже вызвали return или изменили координаты выше)
    g->ball_x = new_ball_x;
    g->ball_y = new_ball_y;
}

int main() {
    GameState game;
    init_game(&game);

    while (game.score_left < WIN_SCORE && game.score_right < WIN_SCORE) {
        draw(&game);
        printf("Игрок 1 (A/Z), Игрок 2 (K/M), Space — пропустить: ");
        char input = get_input();

        if (input == 'A' && game.left_y > 0)
            game.left_y--;
        else if (input == 'Z' && game.left_y + PADDLE_HEIGHT < HEIGHT)
            game.left_y++;
        else if (input == 'K' && game.right_y > 0)
            game.right_y--;
        else if (input == 'M' && game.right_y + PADDLE_HEIGHT < HEIGHT)
            game.right_y++;

        update_game(&game);
    }

    draw(&game);
    if (game.score_left >= WIN_SCORE)
        printf("Поздравляем! Игрок 1 победил!\n");
    else
        printf("Поздравляем! Игрок 2 победил!\n");

    return 0;
}
