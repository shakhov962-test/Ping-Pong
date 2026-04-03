#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

// --- Глобальные переменные состояния игры ---
int height, width;
int left_paddle_y, right_paddle_y;
int ball_x, ball_y;
int ball_dx, ball_dy;
int left_score, right_score;
int game_over;

// --- Константы ---
#define WINNING_SCORE 3
#define PADDLE_HEIGHT 3
#define PADDLE_LEFT_X 1
#define PADDLE_RIGHT_X 0 // Будет рассчитываться как width - 2

// --- Инициализация игры ---
void init_game()
{
    getmaxyx(stdscr, height, width);

    // Позиции по центру
    left_paddle_y = height / 2 - PADDLE_HEIGHT / 2;
    right_paddle_y = height / 2 - PADDLE_HEIGHT / 2;
    ball_x = width / 2;
    ball_y = height / 2;

    // Начальное направление мяча: вправо и вверх
    ball_dx = 1;
    ball_dy = -1;

    left_score = 0;
    right_score = 0;
    game_over = 0;
}

// --- Функция отрисовки ---
void draw()
{
    clear();

    // Границы поля
    border(0, 0, 0, 0, 0, 0, 0, 0);

    // Счет
    mvprintw(0, width / 2 - 5, "Score: %d | %d", left_score, right_score);

    // Ракетки (рисуем блок из PADDLE_HEIGHT символов)
    for (int i = 0; i < PADDLE_HEIGHT; i++)
    {
        mvaddch(left_paddle_y + i, PADDLE_LEFT_X, '|');
        mvaddch(right_paddle_y + i, width - PADDLE_LEFT_X - 1, '|');
    }

    // Мяч
    mvaddch(ball_y, ball_x, 'O');

    // Сообщение о конце игры
    if (game_over)
    {
        mvprintw(height / 2, width / 2 - 10, "Game Over! Press 'q' to quit.");
    }

    refresh();
}

// --- Обработка ввода ---
void handle_input()
{
    int key = getch(); // Ждем нажатия клавиши (пошаговый режим)

    if (game_over)
    {
        if (key == 'q')
        {
            endwin();
            exit(0);
        }
        return; // Если игра окончена, не обрабатываем движения
    }

    switch (key)
    {
    // Игрок слева (A/Z)
    case 'a':
        if (left_paddle_y > 1)
            left_paddle_y--;
        break;
    case 'z':
        if (left_paddle_y < height - PADDLE_HEIGHT - 1)
            left_paddle_y++;
        break;

    // Игрок справа (K/M)
    case 'k':
        if (right_paddle_y > 1)
            right_paddle_y--;
        break;
    case 'm':
        if (right_paddle_y < height - PADDLE_HEIGHT - 1)
            right_paddle_y++;
        break;

    case ' ': // Space Bar - пропуск хода. Просто перерисуем экран.
        break;
    case 'q':
        endwin();
        exit(0);
    default:
        break;
    }
}

// --- Логика движения мяча ---
void move_ball()
{
    if (game_over)
        return; // Не двигаем мяч после конца игры

    ball_x += ball_dx;
    ball_y += ball_dy;

    // Отскок от верхней и нижней стенок
    if (ball_y <= 1 || ball_y >= height - 2)
    {
        ball_dy = -ball_dy;
    }

    // Отскок от правой ракетки (игрок K/M)
    if (ball_x == width - PADDLE_LEFT_X - 1)
    {
        for (int i = 0; i < PADDLE_HEIGHT; i++)
        {
            if (ball_y == right_paddle_y + i)
            {
                ball_dx = -ball_dx; // Меняем направление на левое
                return;             // Столкновение произошло, выходим из функции
            }
        }
        // Если цикл завершился без return, значит мяч пролетел мимо ракетки
        left_score++;
        if (left_score >= WINNING_SCORE)
            game_over = 1;
        init_game(); // Сброс позиций для нового раунда
        return;
    }

    // Отскок от левой ракетки (игрок A/Z)
    if (ball_x == PADDLE_LEFT_X + 1)
    {
        for (int i = 0; i < PADDLE_HEIGHT; i++)
        {
            if (ball_y == left_paddle_y + i)
            {
                ball_dx = -ball_dx; // Меняем направление на правое
                return;             // Столкновение произошло, выходим из функции
            }
        }
        // Если цикл завершился без return, значит мяч пролетел мимо ракетки
        right_score++;
        if (right_score >= WINNING_SCORE)
            game_over = 1;
        init_game(); // Сброс позиций для нового раунда
    }
}

// --- Точка входа ---
int main()
{
    initscr();              // Инициализация ncurses
    cbreak();               // Отключаем буферизацию ввода (не нужен Enter)
    noecho();               // Не выводим нажатые клавиши на экран
    keypad(stdscr, TRUE);   // Включаем обработку спец. клавиш (стрелки)
    nodelay(stdscr, FALSE); // Режим блокировки getch() до нажатия клавиши (пошагово)
    curs_set(0);            // Скрываем курсор

    init_game(); // Устанавливаем начальные значения

    while (1)
    {
        handle_input(); // Ждем действия игрока и обновляем позиции ракеток
        move_ball();    // Обновляем позицию мяча и проверяем столкновения/голы

        draw(); // Перерисовываем экран с новыми данными

        usleep(50000); // Задержка для плавности анимации (~50 мс)
                       // В пошаговом режиме она не обязательна для логики,
                       // но делает игру визуально приятнее.
    }

    endwin(); // Корректное завершение ncurses
    return 0;
}