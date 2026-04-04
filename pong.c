#include <stdio.h>

int main() {
    const int WIDTH = 80;
    const int HEIGHT = 25;
    const int PADDLE_HEIGHT = 3;
    const int WIN_SCORE = 21;

    int left_y = (HEIGHT - PADDLE_HEIGHT) / 2;
    int right_y = (HEIGHT - PADDLE_HEIGHT) / 2;
    int ball_x = WIDTH / 2;
    int ball_y = HEIGHT / 2;
    int ball_dx = -1;
    int ball_dy = 0;
    int score_left = 0;
    int score_right = 0;

    while (score_left < WIN_SCORE && score_right < WIN_SCORE) {
        printf("\033[2J\033[H");

        for (int x = 0; x < WIDTH; x++) {
            printf("%c", (x == 0 || x == WIDTH - 1) ? '|' : '-');
        }
        printf("\n");

        for (int y = 0; y < HEIGHT; y++) {
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

        for (int x = 0; x < WIDTH; x++) {
            printf("%c", (x == 0 || x == WIDTH - 1) ? '|' : '-');
        }
        printf("\n");

        printf("Счёт: Игрок 1 — %d | Игрок 2 — %d\n", score_left, score_right);

        printf("Игрок 1 (A/Z), Игрок 2 (K/M): ");
        char input = getchar();
        while (getchar() != '\n');

        if ((input == 'A' || input == 'a') && left_y > 0) {
            left_y--;
        }
        if ((input == 'Z' || input == 'z') && left_y + PADDLE_HEIGHT < HEIGHT) {
            left_y++;
        }

        if ((input == 'K' || input == 'k') && right_y > 0) {
            right_y--;
        }
        if ((input == 'M' || input == 'm') && right_y + PADDLE_HEIGHT < HEIGHT) {
            right_y++;
        }

        int new_ball_x = ball_x + ball_dx;
        int new_ball_y = ball_y + ball_dy;

        if (new_ball_x < 1) {
            score_right++;
            left_y = (HEIGHT - PADDLE_HEIGHT) / 2;
            right_y = (HEIGHT - PADDLE_HEIGHT) / 2;
            ball_x = WIDTH / 2;
            ball_y = HEIGHT / 2;
            ball_dx = -1;
            ball_dy = 0;
            continue;
        }

        if (new_ball_x > WIDTH - 2) {
            score_left++;
            left_y = (HEIGHT - PADDLE_HEIGHT) / 2;
            right_y = (HEIGHT - PADDLE_HEIGHT) / 2;
            ball_x = WIDTH / 2;
            ball_y = HEIGHT / 2;
            ball_dx = -1;
            ball_dy = 0;
            continue;
        }

        if (new_ball_x == 1 && ball_y >= left_y && ball_y < left_y + PADDLE_HEIGHT) {
            ball_dx = 1;

            if (ball_y < left_y + 1)
                ball_dy = -1;
            else if (ball_y > left_y + 1)
                ball_dy = 1;
            else
                ball_dy = 0;

            new_ball_x += ball_dx;
            new_ball_y += ball_dy;
        }

        if (new_ball_x == WIDTH - 2 && ball_y >= right_y && ball_y < right_y + PADDLE_HEIGHT) {
            ball_dx = -1;

            if (ball_y < right_y + 1)
                ball_dy = -1;
            else if (ball_y > right_y + 1)
                ball_dy = 1;
            else
                ball_dy = 0;

            new_ball_x += ball_dx;
            new_ball_y += ball_dy;
        }

        if (new_ball_y <= 0 || new_ball_y >= HEIGHT - 1) {
            ball_dy = -ball_dy;

            new_ball_x += ball_dx;
            new_ball_y += ball_dy;
        }

        ball_x = new_ball_x;
        ball_y = new_ball_y;

        for (volatile int i = 0; i < 500000; i++);
    }

    printf("\nИгра окончена!\n");
    if (score_left >= WIN_SCORE)
        printf("Победил Игрок 1! Счёт: %d\n", score_left);
    else
        printf("Победил Игрок 2! Счёт: %d\n", score_right);

    return 0;
}