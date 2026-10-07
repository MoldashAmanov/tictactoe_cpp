#include <iostream>
#include <limits> // Очистка потока ввода

using namespace std;

const int SIZE = 3;

const char PLAYER_X = 'X';
const char PLAYER_0 = '0';
const char EMPTY = ' ';

// Инициализация поля, заполняем все клетки пробелами
void initBoard(char board[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            board[i][j] = EMPTY;
        }
    }
}

// Отрисовка игрового поля в консоли
void printBoard(const char board[SIZE][SIZE])
{
    cout << "\n";
    cout << "     1   2   3\n"; // Нумерация столбов
    cout << "   +---+---+---+\n";

    for (int i = 0; i < SIZE; i++)
    {
        cout << " " << (i + 1) << " |"; // Номер строки
        for (int j = 0; j < SIZE; j++)
        {
            cout << " " << board[i][j] << " |";
        }
        cout << "\n   +---+---+---+\n";
    }
    cout << "\n";
}

// Проверка победил ли пользователь с символом "player"
bool checkWin(const char board[SIZE][SIZE], char player)
{
    // Проверка строк
    for (int i = 0; i < SIZE; i++)
    {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player)
        {
            return true;
        }
    }
    // Проверка столбцов
    for (int j = 0; j < SIZE; j++)
    {
        if (board[0][j] == player && board[1][j] == player && board[2][j] == player)
        {
            return true;
        }
    }
    // Главная диагональ
    if (board[0][0] == player & board[1][1] == player && board[2][2] == player)
    {
        return true;
    }
    // Побочная диагональ
    if (board[0][2] == player && board [1][1] == player && board[2][0] == player)
    {
        return true;
    }
    return false;
}

// Проверка заполнено поле полностью (ничья)
bool isFull(const char board[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] == EMPTY)
            return false;
        }
    }
    return true;
}

// Ввод хода игрока, возвращает true, если ход сделать успешно
bool makeMove(char board[SIZE][SIZE], char player)
{
    int row, col;

    while (true)
    {
        cout << "Player " << player << ", prints row and col (1-3): ";

        // Проверка корректности ввода
        if (!(cin >> row >> col))
        {
            cout << "Error: needs two digits!\n";
            cin.clear(); // Сброс флага ошибки
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера
            continue;
        }

        // Преобразование индексов
        row--;
        col--;

        // Проверка диапазона
        if (row < 0 || row >= SIZE || col < 0 || col >= SIZE)
        {
            cout << "Error: coords need from 1 to 3!\n";
            continue;
        }

        // Проверка, что клетка свободна
        if (board[row][col] != EMPTY)
        {
            cout << "Error: cell isn't free!\n";
            continue;
        }

        // Ход принят
        board[row][col] = player;
        return true;
    }
}

int main()
{
    char board[SIZE][SIZE];
    char currentPlayer = PLAYER_X;
    bool gameOver = false;

    initBoard(board);

    cout << "=== TIC TAC TOE ===\n";
    cout << "Player X goes first.\n";

    while (!gameOver)
    {
        printBoard(board);
        makeMove(board, currentPlayer);

        // Проверка победы текущего игрока
        if (checkWin(board, currentPlayer))
        {
            printBoard(board);
            cout << currentPlayer << " is win!\n";
            gameOver = true;
        }

        // Проверка ничьей
        else if (isFull(board))
        {
            printBoard(board);
            cout << "Nobody!\n";
            gameOver = true;
        }

        // Смена игрока
        else{
            currentPlayer = (currentPlayer == PLAYER_X) ? PLAYER_0 : PLAYER_X;
        }
    }

    cout << "\nThank you for game!\n";
    return 0;
}