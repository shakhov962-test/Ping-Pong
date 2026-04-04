#include <stdio.h>

int main() {
    // --- КОНСТАНТЫ  ---
    const int WIDTH = 80;
    const int HEIGHT = 25;
    const int PADDLE_HEIGHT = 3;
    const int WIN_SCORE = 21;

    // --- ПЕРЕМЕННЫЕ СОСТОЯНИЯ ИГРЫ ---
    int left_y = (HEIGHT - PADDLE_HEIGHT) / 2;
    int right_y = (HEIGHT - PADDLE_HEIGHT) / 2;
    int ball_x = WIDTH / 2;
    int ball_y = HEIGHT / 2;
    int ball_dx = -1;  // Направление мяча по X
    int ball_dy = 0;   // Направление мяча по Y
    int score_left = 0;
    int score_right = 0;

    // --- ИГРОВОЙ ЦИКЛ ---
    while (score_left < WIN_SCORE && score_right < WIN_SCORE) {
        // --- ОТРИСОВКА ЭКРАНА ---
        printf("\033[2J\033[H");  // Очистка экрана и перевод курсора в начало

        // Верхняя граница
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

        // Нижняя граница
        for (int x = 0; x < WIDTH; x++) {
            printf("%c", (x == 0 || x == WIDTH - 1) ? '|' : '-');
        }
        printf("\n");

        // Счёт
        printf("Счёт: Игрок 1 — %d | Игрок 2 — %d\n", score_left, score_right);

        // --- ВВОД ДАННЫХ ---
        printf("Игрок 1 (A/Z), Игрок 2 (K/M): ");
        char input = getchar();
        while (getchar() != '\n');  // Очистка буфера ввода

        // Движение левой ракетки (Игрок 1)
        if ((input == 'A' || input == 'a') && left_y > 0) {
            left_y--;
        }
        if ((input == 'Z' || input == 'z') && left_y + PADDLE_HEIGHT < HEIGHT) {
            left_y++;
        }

        // Движение правой ракетки (Игрок 2)
        if ((input == 'K' || input == 'k') && right_y > 0) {
            right_y--;
        }
        if ((input == 'M' || input == 'm') && right_y + PADDLE_HEIGHT < HEIGHT) {
            right_y++;
        }

        // --- ЛОГИКА ДВИЖЕНИЯ МЯЧА ---

        // Вычисляем следующую позицию мяча
        int new_ball_x = ball_x + ball_dx;
        int new_ball_y = ball_y + ball_dy;

        // Проверка гола слева
        if (new_ball_x < 1) {
            score_right++;
            // Сброс раунда
            left_y = (HEIGHT - PADDLE_HEIGHT) / 2;
            right_y = (HEIGHT - PADDLE_HEIGHT) / 2;
            ball_x = WIDTH / 2;
            ball_y = HEIGHT / 2;
            ball_dx = -1;
            ball_dy = 0;
            continue;  // Пропускаем остальную логику и начинаем новый цикл
        }

        // Проверка гола справа
        if (new_ball_x > WIDTH - 2) {
            score_left++;
            // Сброс раунда
            left_y = (HEIGHT - PADDLE_HEIGHT) / 2;
            right_y = (HEIGHT - PADDLE_HEIGHT) / 2;
            ball_x = WIDTH / 2;
            ball_y = HEIGHT / 2;
            ball_dx = -1;
            ball_dy = 0;
            continue;  // Пропускаем остальную логику и начинаем новый цикл
        }

        // Отскок от левой ракетки
        if (new_ball_x == 1 && ball_y >= left_y && ball_y < left_y + PADDLE_HEIGHT) {
            ball_dx = 1;  // Меняем направление на право

            // Задаём угол отскока в зависимости от места удара по ракетке
            if (ball_y < left_y + 1)
                ball_dy = -1;
            else if (ball_y > left_y + 1)
                ball_dy = 1;
            else
                ball_dy = 0;

            new_ball_x += ball_dx;  // Сразу сдвигаем мяч на один шаг вправо, чтобы он не застрял в ракетке
            new_ball_y += ball_dy;
        }

        // Отскок от правой ракетки
        if (new_ball_x == WIDTH - 2 && ball_y >= right_y && ball_y < right_y + PADDLE_HEIGHT) {
            ball_dx = -1;  // Меняем направление на лево

            if (ball_y < right_y + 1)
                ball_dy = -1;
            else if (ball_y > right_y + 1)
                ball_dy = 1;
            else
                ball_dy = 0;

            new_ball_x += ball_dx;  // Сразу сдвигаем мяч на один шаг влево
            new_ball_y += ball_dy;
        }

        // Отскок от верхней и нижней стенок
        if (new_ball_y <= 0 || new_ball_y >= HEIGHT - 1) {
            ball_dy = -ball_dy;  // Инвертируем вертикальное направление

            // Вычисляем финальную позицию с учётом отскока от стены,
            // чтобы мяч не "застрял" внутри стенки на следующем шаге.
            new_ball_x += ball_dx;
            new_ball_y += ball_dy;
        }

        // Обновляем координаты мяча на новые вычисленные значения
        ball_x = new_ball_x;
        ball_y = new_ball_y;

        // Задержка для плавности игры
        for (volatile int i = 0; i < 500000; i++);
    }

    // --- КОНЕЦ ИГРЫ ---
    printf("\nИгра окончена!\n");
    if (score_left >= WIN_SCORE)
        printf("Победил Игрок 1! Счёт: %d\n", score_left);
    else
        printf("Победил Игрок 2! Счёт: %d\n", score_right);

    return 0;
}