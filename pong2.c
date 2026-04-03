#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25
#define PADDLE_HEIGHT 3
#define WIN_SCORE 21

// Структура для хранения состояния игры
typedef struct {
    int left_y;      // Верхняя позиция левой ракетки
    int right_y;     // Верхняя позиция правой ракетки
    int ball_x, ball_y;
    int ball_dx, ball_dy; // Направление мяча: -1, 0, 1
    int score_left, score_right;
} GameState;

// Инициализация состояния игры
void init_game(GameState *g) {
    g->left_y = (HEIGHT - PADDLE_HEIGHT) / 2;
    g->right_y = (HEIGHT - PADDLE_HEIGHT) / 2;
    g->ball_x = WIDTH / 2;
    g->ball_y = HEIGHT / 2;
    g->ball_dx = -1; // Влево
    g->ball_dy = 0;  // Прямо
    g->score_left = 0;
    g->score_right = 0;
}

// Очистка экрана (ANSI escape-код)
void clear_screen() {
    printf("\033[2J\033[H");
}

// Отрисовка игрового поля
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
                // Левая ракетка
                if (y >= g->left_y && y < g->left_y + PADDLE_HEIGHT)
                    printf("#");
                else
                    printf(" ");
            } else if (x == WIDTH - 2) {
                // Правая ракетка
                if (y >= g->right_y && y < g->right_y + PADDLE_HEIGHT)
                    printf("#");
                else
                    printf(" ");
            } else if (x == g->ball_x && y == g->ball_y) {
                printf("O"); // Мяч
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

    // Счёт
    printf("Счёт: Игрок 1 — %d | Игрок 2 — %d\n", g->score_left, g->score_right);
}

// Обработка ввода игрока
char get_input() {
    char c = getchar();
    while (getchar() != '\n'); // Очистка буфера
    return toupper(c);
}

// Обновление состояния игры после хода
void update_game(GameState *g) {
    // Двигаем мяч
    int new_ball_x = g->ball_x + g->ball_dx;
    int new_ball_y = g->ball_y + g->ball_dy;

    // Проверка столкновения с левой ракеткой
    if (new_ball_x == 1) {
        if (g->ball_y >= g->left_y && g->ball_y < g->left_y + PADDLE_HEIGHT) {
            new_ball_x = 2;
            g->ball_dx = 1;
            // Изменяем угол в зависимости от места удара
            if (g->ball_y < g->left_y + 1) g->ball_dy = -1;
            else if (g->ball_y > g->left_y + 1) g->ball_dy = 1;
            else g->ball_dy = 0;
        } else {
            // Гол справа
            g->score_right++;
            init_game(g);
            return;
        }
    }

    // Проверка столкновения с правой ракеткой
    if (new_ball_x == WIDTH - 2) {
        if (g->ball_y >= g->right_y && g->ball_y < g->right_y + PADDLE_HEIGHT) {
            new_ball_x = WIDTH - 3;
            g->ball_dx = -1;
            if (g->ball_y < g->right_y + 1) g->ball_dy = -1;
            else if (g->ball_y > g->right_y + 1) g->ball_dy = 1;
            else g->ball_dy = 0;
        } else {
            // Гол слева
            g->score_left++;
            init_game(g);
            return;
        }
    }

    // Проверка столкновения с верхней/нижней границей
    if (new_ball_y <= 0 || new_ball_y >= HEIGHT - 1) {
        g->ball_dy = -g->ball_dy;
        new_ball_y = g->ball_y + g->ball_dy;
    }

    // Обновляем позицию мяча
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

        // Перемещение ракеток или пропуск хода
        if (input == 'A' && game.left_y > 0)
            game.left_y--;
        else if (input == 'Z' && game.left_y + PADDLE_HEIGHT < HEIGHT)
            game.left_y++;
        else if (input == 'K' && game.right_y > 0)
            game.right_y--;
        else if (input == 'M' && game.right_y + PADDLE_HEIGHT < HEIGHT)
            game.right_y++;
        // Space или любой другой символ — просто пропускаем ход

        update_game(&game);
    }

    draw(&game);
    if (game.score_left >= WIN_SCORE)
        printf("Поздравляем! Игрок 1 победил!\n");
    else
        printf("Поздравляем! Игрок 2 победил!\n");

    return 0;
}