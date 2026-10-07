#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

// ANSI-colors
namespace Color
{
    const string RESET   = "\033[0m";
    const string BOLD    = "\033[1m";
    const string RED     = "\033[31m";
    const string GREEN   = "\033[32m";
    const string YELLOW  = "\033[33m";
    const string BLUE    = "\033[34m";
    const string MAGENTA = "\033[35m";
    const string CYAN    = "\033[36m";
    const string WHITE   = "\033[37m";
    const string GRAY    = "\033[90m";

    const string BRIGHT_RED    = "\033[91m";
    const string BRIGHT_GREEN  = "\033[92m";
    const string BRIGHT_YELLOW = "\033[93m";
    const string BRIGHT_CYAN   = "\033[96m";
}

// Класс игры
class TicTacToe
{
    public:
        // Режим игры
        enum class Mode
        {
            PVP, // Игрок против игрока
            PVE  // Игрок против компьютера
        };

        // Результат партии
        enum class Result
        {
            X_WINS,
            O_WINS,
            DRAW,
            IN_PROGRESS
        };

    private:
        static const int SIZE = 3;
        static const char PLAYER_X = 'X';
        static const char PLAYER_O = 'O';
        static const char EMPTY = ' ';

        char board[SIZE][SIZE];
        char humanSymbol; // За кого играет человек в режиме PVE
        char aiSymbol; // За кого играет компьютер
        char currentPlayer;
        Mode mode;

    public:
        // Конструктор
        TicTacToe(Mode m = Mode::PVP) 
            : mode(m), currentPlayer(PLAYER_X)
        {
            humanSymbol = PLAYER_X;
            aiSymbol = PLAYER_O;
            initBoard();
        }

        // Установить символ человека в режиме PVE ('X' or 'O')
        void setHumanSymbol(char symbol)
        {
            if (symbol == PLAYER_X || symbol == PLAYER_O)
            {
                humanSymbol = symbol;
                aiSymbol = (symbol == PLAYER_X) ? PLAYER_O : PLAYER_X;
                currentPlayer = PLAYER_X; // Х всегда ходит первым
            }
        }

        // Запуск игры
        void run()
        {
            printIntro();

            while (true)
            {
                printBoard();

                // Чей ход
                if (mode == Mode::PVE && currentPlayer == aiSymbol)
                {
                    cout << Color::BRIGHT_CYAN << "Компьютер (" << currentPlayer << ") думает ...\n" << Color::RESET;
                    makeAIMove();
                }
                else{
                    makeHumanMove();
                }

                // Проверка результата
                Result res = checkResult();
                if (res != Result::IN_PROGRESS)
                {
                    printBoard();
                    printResult(res);
                    break;
                }

                switchPlayer();
            }
        }
    
    private:
        // Инициализация
        void initBoard()
        {
            for (int i = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    board[i][j] = EMPTY;
                }
            }
        }

        // Вспомогательные
        bool isEmpty(int r, int c) const
        {
            return board[r][c] == EMPTY;
        }

        bool isFull() const
        {
            for (int i = 0; i < SIZE; i++)
                for (int j = 0; j < SIZE; j++)
                    if (board[i][j] == EMPTY) return false;
            return true;
        }

        void switchPlayer()
        {
            currentPlayer = (currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
        }

        // Цвет символа
        string symbolColor(char c) const
        {
            if (c == PLAYER_X) return Color::BRIGHT_RED;
            if (c == PLAYER_O) return Color::BRIGHT_YELLOW;
            return Color::GRAY;
        }

        void printIntro() const
        {
            cout << Color::BOLD << Color::BRIGHT_CYAN
            << "\n  ╔══════════════════════════════╗\n"
            << "  ║      КРЕСТИКИ-НОЛИКИ         ║\n"
            << "  ╚══════════════════════════════╝\n"
            << Color::RESET;

            if (mode == Mode::PVE)
            {
                cout << Color::GREEN << " Режим: Игрок vs компьютер\n"
                << " Вы играете за " << symbolColor(humanSymbol)
                << humanSymbol << Color::GREEN << "\n" << Color::RESET;
            }
            else 
            {
                cout << Color::GREEN << " Режим: игрок vs игрок\n" << Color::RESET;
            }
            
            cout << " Х ходит первым. Вводите строку и столбец (1-3)\n\n";
        }

        void printBoard() const
        {
            cout << "\n";

            // Заголовок столбцов
            cout << Color::GRAY << "      1   2   3\n" << Color::RESET;
            cout << Color::GRAY << "   ┌───┬───┬───┐\n" << Color::RESET;

            for (int i = 0; i < SIZE; i++)
            {
                cout << Color::GRAY << " " << (i + 1) << " │" << Color::RESET;

                for (int j = 0; j < SIZE; j++)
                {
                    char c = board[i][j];
                    if (c == EMPTY)
                        cout << Color::GRAY << "   " << Color::RESET << Color::GRAY << "|" << Color::RESET;
                    else
                        cout << " " << symbolColor(c) << Color::BOLD << c << Color::RESET << " " << Color::GRAY << "|" << Color::RESET;
                }
                cout << "\n";

                if (i < SIZE - 1)
                {
                    cout << Color::GRAY << "   ├───┼───┼───┤\n" << Color::RESET;
                }
            }
            cout << Color::GRAY << "   └───┴───┴───┘\n" << Color::RESET;
        }

        void printResult(Result res) const
        {
            if (res == Result::DRAW)
            {
                cout << Color::BOLD << Color::MAGENTA << "  *** Ничья! ***\n" << Color::RESET;
                return;
            }

            char winner = (res == Result::X_WINS) ? PLAYER_X : PLAYER_O;
            cout << Color::BOLD << symbolColor(winner);

            if (mode == Mode::PVE)
            {
                if (winner == humanSymbol)
                {
                    cout << "\n  ★★★ ПОБЕДА! Вы обыграли компьютер! ★★★\n";
                }
                else
                {
                    cout << "\n  ☠ Компьютер победил. Попробуйте ещё!\n";
                }
            }
            else
            {
                cout << "\n  *** Победил игрок " << winner << "! ***\n";
            }
            cout << Color::RESET;
        }
        void makeHumanMove()
        {
            int row, col;

            while(true)
            {
                cout << "Игрок " << symbolColor(currentPlayer) << Color::BOLD
                << currentPlayer << Color::RESET << ", ваш ход (строка столбец): ";

                if (!(cin >> row >> col))
                {
                    cout << Color::RED << "Ошибка: введите два числа!\n" << Color::RESET;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                row--, col--;

                if (row < 0 || row >= SIZE || col < 0 || col >= SIZE)
                {
                    cout << Color::RED << "Ошибка: координаты от 1 до 3!\n" << Color::RESET;
                    continue;
                }

                if (!isEmpty(row, col))
                {
                    cout << Color::RED << "Ошибка: клетка занята!\n" << Color::RESET;
                    continue;
                }

                board[row][col] = currentPlayer;
                return;
            }
        }

        // Ход компьютера
        void makeAIMove()
        {
            int bestScore = numeric_limits<int>::min();
            int bestRow = -1, bestCol = -1;

            for (int i = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    if (isEmpty(i, j))
                    {
                        board[i][j] = aiSymbol;
                        int score = minimax(false, 0);
                        board[i][j] = EMPTY;

                        if (score > bestScore)
                        {
                            bestScore = score;
                            bestRow = i;
                            bestCol = j;
                        }
                    }
                }
            }

            if (bestRow != -1)
            {
                board[bestRow][bestCol] = aiSymbol;
                cout << Color::BRIGHT_CYAN << "  → Компьютер поставил " << Color::BOLD
                << aiSymbol << Color::RESET << Color::BRIGHT_CYAN << " в [" << (bestRow + 1)
                << ", " << (bestCol + 1) << "]\n" << Color::RESET;
            }
        }

        // Минимакс: isMaximizing == true - ход ИИ, иначе - ход человека
        int minimax(bool isMaximizing, int depth)
        {
            Result res = checkResult();

            // Терминальные состояния
            if (res == Result::X_WINS || res == Result::O_WINS)
            {
                char winner = (res == Result::X_WINS) ? PLAYER_X : PLAYER_O;
                // Чем быстрее победа, тем больше очков, чем позже поражение, тем меньше штраф
                return (winner == aiSymbol) ? (10 - depth) : (depth - 10);
            }
            if (res == Result::DRAW) return 0;

            if (isMaximizing)
            {
                int best = numeric_limits<int>::min();
                for (int i = 0; i < SIZE; i++)
                {
                    for (int j = 0; j < SIZE; j++)
                    {
                        if (isEmpty(i, j))
                        {
                            board[i][j] = aiSymbol;
                            best = max(best, minimax(false, depth + 1));
                            board[i][j] = EMPTY;
                        }
                    }
                }
                return best;
            }
            else
            {
                int best = numeric_limits<int>::max();
                for (int i = 0; i < SIZE; i++)
                {
                    for (int j = 0; j < SIZE; j++)
                    {
                        if (isEmpty(i, j))
                        {
                            board[i][j] = humanSymbol;
                            best = min(best, minimax(true, depth + 1));
                            board[i][j] = EMPTY;
                        }
                    }
                }
                return best;
            }
        }

        // Проверка результата
        bool hasWon(char player) const
        {
            // Строки
            for (int i = 0; i < SIZE; i++)
            {
                if (board[i][0] == player && board [i][1] == player && board[i][2] == player)
                    return true;
            }
            // Столбцы
            for (int j = 0; j < SIZE; j++)
            {
                if (board[0][j] == player && board[1][j] == player && board[2][j] == player)
                    return true;
            }
            // Главная диагональ
            if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
                return true;
            // Побочная диагональ
            if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
                return true;

            return false;
        }

        Result checkResult() const
        {
            if (hasWon(PLAYER_X)) return Result::X_WINS;
            if (hasWon(PLAYER_O)) return Result::O_WINS;
            if (isFull()) return Result::DRAW;
            return Result::IN_PROGRESS;
        }
};

// Меню
void clearScrean()
{
    #ifdef _Win32
        system("cls");
    #else
        system("clear");
    #endif
}

int readIntInRange(const string& promt, int lo, int hi)
{
    int value;
    while(true)
    {
        cout << promt;
        if (!(cin >> value))
        {
            cout << Color::RED << "Ошибка: введите число!\n" << Color::RESET;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (value < lo || value > hi)
        {
            cout << Color::RED << "Ошибка: введите число от " << lo << " до " << hi << "!\n" << Color::RESET;
            continue;
        }
        return value;
    }
}

int main()
{
    while (true)
    {
        clearScrean();
        cout << Color::BOLD << Color::BRIGHT_CYAN
        << "\n  ╔══════════════════════════════╗\n"
        << "  ║      КРЕСТИКИ-НОЛИКИ         ║\n"
        << "  ╚══════════════════════════════╝\n"
        << Color::RESET;

        cout << "\n  Меню:\n" << "    " << Color::GREEN << "1" << Color::RESET
        << " - Играть с другим игроком\n"
        << "    " << Color::GREEN << "2" << Color::RESET
        << " — Играть с компьютером\n"
        << "    " << Color::GREEN << "0" << Color::RESET
        << " — Выход\n\n";

        int choice = readIntInRange("  Ваш выбор: ", 0, 2);
        if (choice == 0)
        {
            cout << Color::BRIGHT_CYAN << "\n  До встречи!\n" << Color::RESET;
            break;
        }

        TicTacToe::Mode mode = (choice == 2) ? TicTacToe::Mode::PVE : TicTacToe::Mode::PVP;
        TicTacToe game(mode);

        if (mode == TicTacToe::Mode::PVE)
        {
            cout << "\n  Выберите свой символ:\n" 
            << "    " << Color::BRIGHT_RED << "1" << Color::RESET << " — X (ходите первым)\n"
            << "    " << Color::BRIGHT_YELLOW << "2" << Color::RESET << " — O (ходит компьютер)\n\n";

            int sym = readIntInRange("  Ваш выбор: ", 1, 2);
            game.setHumanSymbol(sym == 1 ? 'X' : 'O');
        }

        game.run();

        cout << "\n";
        int again = readIntInRange("  Сыграть ещё раз? (1 — да, 0 — в меню): ", 0, 1);
        if (again == 0) continue;
    }

    return 0;
}