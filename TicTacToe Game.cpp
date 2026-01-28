/**
  @Title TicTacToe Game - Enhanced Educational Edition
  @Intro Implementation of a Tic-Tac-Toe game with both single and two-player modes
  @authors
            -NAME                         -ID
        1.Misiker Genene                 NSR/1450/16
        2.Biruk Getahun                  NSR/204/16
        3.Tsion Samuel                   NSR/989/16
        4.Heran Mohammed                 NSR/1667/16
        5.Hilina Kitachew                NSR/500/16
        6.Sadam Robel                    NCSR/1478/16

  @date [January 28, 2026]

  ENHANCED FEATURES:
    - Three AI difficulty levels (Easy, Medium, Hard with Minimax)
    - Score tracking and statistics
    - Move history with undo functionality
    - Cross-platform support (Windows, Linux, macOS)
    - Enhanced error handling
    - Educational code comments and complexity analysis
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>
#include <climits>
#include <fstream>
#include <sstream>

// Platform detection and headers
#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_WINDOWS
    #include <windows.h>
#else
    #define PLATFORM_UNIX
#endif

using namespace std;

// ==================== CROSS-PLATFORM COLOR SYSTEM ====================

/**
 * @brief Color enumeration for cross-platform color support
 */
enum class Color {
    RED, BLUE, GREEN, YELLOW, MAGENTA, CYAN, WHITE, RESET
};

/**
 * @brief Sets console text color in a cross-platform way
 * Windows: Uses SetConsoleTextAttribute
 * Unix: Uses ANSI escape codes
 */
void setColor(Color color) {
#ifdef PLATFORM_WINDOWS
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int colorCode;
    switch(color) {
        case Color::RED:     colorCode = 12; break;
        case Color::BLUE:    colorCode = 9;  break;
        case Color::GREEN:   colorCode = 10; break;
        case Color::YELLOW:  colorCode = 14; break;
        case Color::MAGENTA: colorCode = 13; break;
        case Color::CYAN:    colorCode = 11; break;
        case Color::WHITE:   colorCode = 15; break;
        case Color::RESET:   colorCode = 7;  break;
        default:             colorCode = 7;  break;
    }
    SetConsoleTextAttribute(hConsole, colorCode);
#else
    const char* ansiCode;
    switch(color) {
        case Color::RED:     ansiCode = "\033[31m"; break;
        case Color::BLUE:    ansiCode = "\033[34m"; break;
        case Color::GREEN:   ansiCode = "\033[32m"; break;
        case Color::YELLOW:  ansiCode = "\033[33m"; break;
        case Color::MAGENTA: ansiCode = "\033[35m"; break;
        case Color::CYAN:    ansiCode = "\033[36m"; break;
        case Color::WHITE:   ansiCode = "\033[37m"; break;
        case Color::RESET:   ansiCode = "\033[0m";  break;
        default:             ansiCode = "\033[0m";  break;
    }
    cout << ansiCode;
#endif
}

// ==================== DATA STRUCTURES ====================

/**
 * @brief Represents a single move in the game
 */
struct Move {
    int row, col;
    char token;
    Move(int r, int c, char t) : row(r), col(c), token(t) {}
};

/**
 * @brief Game statistics tracker
 */
struct GameStats {
    int player1Wins = 0;
    int player2Wins = 0;
    int draws = 0;
    int totalGames = 0;
    
    void recordWin(bool player1Won) {
        if (player1Won) player1Wins++;
        else player2Wins++;
        totalGames++;
    }
    
    void recordDraw() {
        draws++;
        totalGames++;
    }
    
    void reset() {
        player1Wins = player2Wins = draws = totalGames = 0;
    }
};

/**
 * @brief AI difficulty levels
 */
enum class Difficulty {
    EASY,    // Random moves
    MEDIUM,  // Strategic play
    HARD     // Minimax algorithm (unbeatable)
};

// ==================== GLOBAL VARIABLES ====================

char space[3][3] = {{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}};
char token = 'X';
bool gameTie = false;
string name1, name2;
int width = 60;
vector<Move> moveHistory;
GameStats stats;
Difficulty aiDifficulty = Difficulty::MEDIUM;

// ==================== UTILITY FUNCTIONS ====================

/**
 * @brief Clears the console screen based on the operating system
 * Complexity: O(1)
 */
void clearConsole() {
#ifdef PLATFORM_WINDOWS
    system("cls");
#else
    int result = system("clear");
    if (result != 0) {
        cout << "\033[2J\033[1;1H";  // ANSI fallback
    }
#endif
}

/**
 * @brief Converts string to title case
 * Complexity: O(n) where n is string length
 */
string toTitleCase(const string& str) {
    string result = str;
    bool newWord = true;
    for (char& c : result) {
        if (newWord && isalpha(c)) {
            c = toupper(c);
            newWord = false;
        } else if (isalpha(c)) {
            c = tolower(c);
        }
        if (isspace(c)) newWord = true;
    }
    return result;
}

// ==================== DISPLAY FUNCTIONS ====================

/**
 * @brief Displays the current state of the game board
 * Uses formatted output with colors for X and O
 * Complexity: O(1) - constant 3x3 board
 */
void toeBox() {
    int x = 39;
    setColor(Color::GREEN);
    cout << setw(width) << " _______________________ \n";
    cout << setw(width) << "|                       |\n";
    cout << setw(width) << "|      TIC TAC TOE      |\n";
    cout << setw(width) << "|_______________________|\n";
    cout << setw(width) << "|***********************|\n";
    cout << setw(width) << "|*|     |      |      |*|\n";

    // First row
    cout << setw(x) << "|*|  ";
    if (space[0][0] == 'X') { setColor(Color::RED); cout << space[0][0]; setColor(Color::GREEN); }
    else if (space[0][0] == 'O') { setColor(Color::BLUE); cout << space[0][0]; setColor(Color::GREEN); }
    else cout << space[0][0];
    cout << "  |  ";
    if (space[0][1] == 'X') { setColor(Color::RED); cout << space[0][1]; setColor(Color::GREEN); }
    else if (space[0][1] == 'O') { setColor(Color::BLUE); cout << space[0][1]; setColor(Color::GREEN); }
    else cout << space[0][1];
    cout << "   |  ";
    if (space[0][2] == 'X') { setColor(Color::RED); cout << space[0][2]; setColor(Color::GREEN); }
    else if (space[0][2] == 'O') { setColor(Color::BLUE); cout << space[0][2]; setColor(Color::GREEN); }
    else cout << space[0][2];
    cout << "   |*|\n";

    cout << setw(width) << "|*|_____|______|______|*|\n";
    cout << setw(width) << "|*|     |      |      |*|\n";

    // Second row
    cout << setw(x) << "|*|  ";
    if (space[1][0] == 'X') { setColor(Color::RED); cout << space[1][0]; setColor(Color::YELLOW); }
    else if (space[1][0] == 'O') { setColor(Color::BLUE); cout << space[1][0]; setColor(Color::YELLOW); }
    else cout << space[1][0];
    cout << "  |  ";
    if (space[1][1] == 'X') { setColor(Color::RED); cout << space[1][1]; setColor(Color::YELLOW); }
    else if (space[1][1] == 'O') { setColor(Color::BLUE); cout << space[1][1]; setColor(Color::YELLOW); }
    else cout << space[1][1];
    cout << "   |  ";
    if (space[1][2] == 'X') { setColor(Color::RED); cout << space[1][2]; setColor(Color::YELLOW); }
    else if (space[1][2] == 'O') { setColor(Color::BLUE); cout << space[1][2]; setColor(Color::YELLOW); }
    else cout << space[1][2];
    cout << "   |*|\n";

    cout << setw(width) << "|*|_____|______|______|*|\n";
    cout << setw(width) << "|*|     |      |      |*|\n";

    // Third row
    cout << setw(x) << "|*|  ";
    if (space[2][0] == 'X') { setColor(Color::RED); cout << space[2][0]; setColor(Color::RED); }
    else if (space[2][0] == 'O') { setColor(Color::BLUE); cout << space[2][0]; setColor(Color::RED); }
    else cout << space[2][0];
    cout << "  |  ";
    if (space[2][1] == 'X') { setColor(Color::RED); cout << space[2][1]; setColor(Color::RED); }
    else if (space[2][1] == 'O') { setColor(Color::BLUE); cout << space[2][1]; setColor(Color::RED); }
    else cout << space[2][1];
    cout << "   |  ";
    if (space[2][2] == 'X') { setColor(Color::RED); cout << space[2][2]; setColor(Color::RED); }
    else if (space[2][2] == 'O') { setColor(Color::BLUE); cout << space[2][2]; setColor(Color::RED); }
    else cout << space[2][2];
    cout << "   |*|\n";

    cout << setw(width) << "|*|     |      |      |*|\n";
    cout << setw(width) << " *********************** \n";
    setColor(Color::WHITE);
}

/**
 * @brief Displays game statistics
 */
void showStatistics() {
    clearConsole();
    setColor(Color::CYAN);
    cout << "\n        ═══════════════════════════════════\n";
    cout << "                GAME STATISTICS\n";
    cout << "        ═══════════════════════════════════\n\n";
    setColor(Color::WHITE);
    cout << "        Total Games: " << stats.totalGames << "\n\n";
    setColor(Color::GREEN);
    cout << "        " << name1 << " Wins: " << stats.player1Wins << "\n";
    if (name2 != "AI") {
        cout << "        " << name2 << " Wins: " << stats.player2Wins << "\n";
    } else {
        setColor(Color::BLUE);
        cout << "        AI Wins: " << stats.player2Wins << "\n";
    }
    setColor(Color::YELLOW);
    cout << "        Draws: " << stats.draws << "\n\n";
    setColor(Color::CYAN);
    cout << "        ═══════════════════════════════════\n";
    setColor(Color::WHITE);
    cout << "\n        Press Enter to continue...";
    cin.get();
}

/**
 * @brief Displays move history
 */
void showMoveHistory() {
    clearConsole();
    setColor(Color::MAGENTA);
    cout << "\n        === MOVE HISTORY ===\n\n";
    for (size_t i = 0; i < moveHistory.size(); i++) {
        const Move& move = moveHistory[i];
        string playerName = (move.token == 'X') ? name1 : name2;
        int position = move.row * 3 + move.col + 1;
        cout << "        " << (i + 1) << ". " << playerName << " (" << move.token 
             << ") chose position " << position << "\n";
    }
    setColor(Color::WHITE);
    cout << "\n        Press Enter to continue...";
    cin.get();
}

// ==================== GAME LOGIC ====================

/**
 * @brief Checks for winning conditions or a tie
 * Algorithm complexity: O(1) - constant 3x3 board
 * Checks: 3 rows + 3 columns + 2 diagonals + board full check
 */
bool checkWinner() {
    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        if ((space[i][0] == space[i][1] && space[i][1] == space[i][2]) ||
            (space[0][i] == space[1][i] && space[1][i] == space[2][i])) {
            return true;
        }
    }
    
    // Check diagonals
    if ((space[0][0] == space[1][1] && space[1][1] == space[2][2]) ||
        (space[0][2] == space[1][1] && space[1][1] == space[2][0])) {
        return true;
    }

    // Check for draw
    bool isDraw = true;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (space[i][j] != 'X' && space[i][j] != 'O') {
                isDraw = false;
                return false;
            }
        }
    }

    if (isDraw) {
        gameTie = true;
        return true;
    }

    return false;
}

/**
 * @brief Records a move in history and makes it on the board
 */
bool makeMove(int row, int col) {
    if (space[row][col] != 'X' && space[row][col] != 'O') {
        moveHistory.push_back(Move(row, col, token));
        space[row][col] = token;
        return true;
    }
    return false;
}

/**
 * @brief Undoes the last 2 moves (returns to same player's turn)
 */
bool undoLastMove() {
    if (moveHistory.size() < 2) {
        setColor(Color::RED);
        cout << setw(35) << "\nNot enough moves to undo!\n";
        cout << setw(35) << "Press Enter to continue...";
        cin.get();
        setColor(Color::WHITE);
        return false;
    }
    
    for (int i = 0; i < 2; i++) {
        Move lastMove = moveHistory.back();
        moveHistory.pop_back();
        int position = lastMove.row * 3 + lastMove.col + 1;
        space[lastMove.row][lastMove.col] = '0' + position;
        token = (token == 'X') ? 'O' : 'X';
    }
    
    setColor(Color::GREEN);
    cout << setw(35) << "\nMove undone!\n";
    cout << setw(35) << "Press Enter to continue...";
    cin.get();
    setColor(Color::WHITE);
    return true;
}

/**
 * @brief Handles the game logic for player moves
 */
void gameLogic() {
    int digit;
    bool validChoice = false;

    while (!validChoice) {
        clearConsole();
        toeBox();

        if (gameTie) return;

        if (token == 'X') {
            cout << setw(35) << name1 << " Enter (1-9) [0=Menu]: ";
        } else {
            cout << setw(35) << name2 << " Enter (1-9) [0=Menu]: ";
        }

        if (!(cin >> digit)) {
            cin.clear();
            cin.ignore(10000, '\n');
            setColor(Color::RED);
            cout << setw(35) << "\nInvalid input!\n";
            cout << setw(35) << "Press Enter to continue...";
            cin.get();
            setColor(Color::WHITE);
            continue;
        }
        
        // In-game menu
        if (digit == 0) {
            clearConsole();
            setColor(Color::CYAN);
            cout << "\n        === IN-GAME MENU ===\n";
            cout << "        1. Resume Game\n";
            cout << "        2. Undo Last Move\n";
            cout << "        3. View Move History\n";
            cout << "        4. View Statistics\n";
            cout << "        Choice: ";
            setColor(Color::WHITE);
            int choice;
            cin >> choice;
            cin.ignore();
            
            switch(choice) {
                case 2:
                    undoLastMove();
                    break;
                case 3:
                    showMoveHistory();
                    break;
                case 4:
                    showStatistics();
                    break;
            }
            continue;
        }

        if (digit < 1 || digit > 9) {
            setColor(Color::RED);
            cout << setw(35) << "\nInvalid choice (1-9)!\n";
            cout << setw(35) << "Press Enter to continue...";
            cin.get();
            setColor(Color::WHITE);
            continue;
        }

        int row = (digit - 1) / 3;
        int column = (digit - 1) % 3;

        if (makeMove(row, column)) {
            token = (token == 'X') ? 'O' : 'X';
            validChoice = true;
        } else {
            setColor(Color::RED);
            cout << setw(35) << "\nSpace occupied!\n";
            cout << setw(35) << "Press Enter to continue...";
            cin.get();
            cin.ignore(10000, '\n');
            setColor(Color::WHITE);
        }
    }
}

/**
 * @brief Resets the game board to its initial state
 * Complexity: O(1) - constant 3x3 board
 */
void resetGame() {
    char defaultSpace = '1';
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            space[i][j] = defaultSpace++;
        }
    }
    token = 'X';
    gameTie = false;
    moveHistory.clear();
}

// ==================== AI IMPLEMENTATION ====================

/**
 * @brief Checks if a move would result in a win
 * Complexity: O(1)
 */
bool wouldWin(int row, int col, char playerToken) {
    char original = space[row][col];
    space[row][col] = playerToken;
    
    bool win = false;
    
    // Check row
    if (space[row][0] == playerToken && space[row][1] == playerToken && space[row][2] == playerToken)
        win = true;
    
    // Check column
    if (space[0][col] == playerToken && space[1][col] == playerToken && space[2][col] == playerToken)
        win = true;
    
    // Check diagonals
    if (row == col && space[0][0] == playerToken && space[1][1] == playerToken && space[2][2] == playerToken)
        win = true;
    if (row + col == 2 && space[0][2] == playerToken && space[1][1] == playerToken && space[2][0] == playerToken)
        win = true;
    
    space[row][col] = original;
    return win;
}

/**
 * @brief Evaluates board state for minimax
 * @return +10 if AI wins, -10 if player wins, 0 otherwise
 */
int evaluate() {
    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        if (space[i][0] == space[i][1] && space[i][1] == space[i][2]) {
            if (space[i][0] == 'O') return 10;
            if (space[i][0] == 'X') return -10;
        }
        if (space[0][i] == space[1][i] && space[1][i] == space[2][i]) {
            if (space[0][i] == 'O') return 10;
            if (space[0][i] == 'X') return -10;
        }
    }
    
    // Check diagonals
    if (space[0][0] == space[1][1] && space[1][1] == space[2][2]) {
        if (space[0][0] == 'O') return 10;
        if (space[0][0] == 'X') return -10;
    }
    if (space[0][2] == space[1][1] && space[1][1] == space[2][0]) {
        if (space[0][2] == 'O') return 10;
        if (space[0][2] == 'X') return -10;
    }
    
    return 0;
}

/**
 * @brief Checks if game is over
 */
bool isGameOver() {
    if (evaluate() != 0) return true;
    
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (space[i][j] != 'X' && space[i][j] != 'O')
                return false;
    
    return true;
}

/**
 * @brief Minimax algorithm with alpha-beta pruning
 * 
 * Time Complexity: O(b^d) where b=branching factor, d=depth
 * With alpha-beta: Best O(b^(d/2)), Average O(b^(3d/4))
 * Space Complexity: O(d) for recursion stack
 * 
 * Algorithm:
 * 1. Base case: If game over, return evaluation
 * 2. Try all possible moves
 * 3. Recursively evaluate each move
 * 4. Return best score for current player
 * 5. Prune branches that can't affect final decision
 */
int minimax(int depth, bool isMaximizing, int alpha, int beta) {
    int score = evaluate();
    
    // Base cases
    if (score == 10) return score - depth;   // AI wins (prefer faster wins)
    if (score == -10) return score + depth;  // Player wins
    if (isGameOver()) return 0;              // Draw
    
    if (isMaximizing) {
        // AI's turn - maximize score
        int best = INT_MIN;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (space[i][j] != 'X' && space[i][j] != 'O') {
                    char original = space[i][j];
                    space[i][j] = 'O';
                    int value = minimax(depth + 1, false, alpha, beta);
                    best = max(best, value);
                    alpha = max(alpha, best);
                    space[i][j] = original;
                    if (beta <= alpha) break;  // Alpha-beta pruning
                }
            }
        }
        return best;
    } else {
        // Player's turn - minimize score
        int best = INT_MAX;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (space[i][j] != 'X' && space[i][j] != 'O') {
                    char original = space[i][j];
                    space[i][j] = 'X';
                    int value = minimax(depth + 1, true, alpha, beta);
                    best = min(best, value);
                    beta = min(beta, best);
                    space[i][j] = original;
                    if (beta <= alpha) break;  // Alpha-beta pruning
                }
            }
        }
        return best;
    }
}

/**
 * @brief Finds best move using minimax (HARD difficulty)
 */
void findBestMoveHard(int& bestRow, int& bestCol) {
    int bestValue = INT_MIN;
    bestRow = -1;
    bestCol = -1;
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (space[i][j] != 'X' && space[i][j] != 'O') {
                char original = space[i][j];
                space[i][j] = 'O';
                int moveValue = minimax(0, false, INT_MIN, INT_MAX);
                space[i][j] = original;
                
                if (moveValue > bestValue) {
                    bestRow = i;
                    bestCol = j;
                    bestValue = moveValue;
                }
            }
        }
    }
}

/**
 * @brief Makes a strategic move (MEDIUM difficulty)
 * Strategy:
 * 1. Win if possible
 * 2. Block opponent's winning move
 * 3. Take center if available
 * 4. Take corners
 * 5. Take edges
 */
void makeStrategicMove(int& row, int& col) {
    // 1. Try to win
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (space[i][j] != 'X' && space[i][j] != 'O') {
                if (wouldWin(i, j, 'O')) {
                    row = i; col = j;
                    return;
                }
            }
        }
    }
    
    // 2. Block opponent
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (space[i][j] != 'X' && space[i][j] != 'O') {
                if (wouldWin(i, j, 'X')) {
                    row = i; col = j;
                    return;
                }
            }
        }
    }
    
    // 3. Take center
    if (space[1][1] != 'X' && space[1][1] != 'O') {
        row = 1; col = 1;
        return;
    }
    
    // 4. Take corners
    int corners[4][2] = {{0,0}, {0,2}, {2,0}, {2,2}};
    for (int i = 0; i < 4; i++) {
        if (space[corners[i][0]][corners[i][1]] != 'X' && 
            space[corners[i][0]][corners[i][1]] != 'O') {
            row = corners[i][0];
            col = corners[i][1];
            return;
        }
    }
    
    // 5. Take edges
    int edges[4][2] = {{0,1}, {1,0}, {1,2}, {2,1}};
    for (int i = 0; i < 4; i++) {
        if (space[edges[i][0]][edges[i][1]] != 'X' && 
            space[edges[i][0]][edges[i][1]] != 'O') {
            row = edges[i][0];
            col = edges[i][1];
            return;
        }
    }
}

/**
 * @brief Makes a random move (EASY difficulty)
 */
void makeRandomMove(int& row, int& col) {
    vector<pair<int, int>> available;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (space[i][j] != 'X' && space[i][j] != 'O') {
                available.push_back({i, j});
            }
        }
    }
    
    if (!available.empty()) {
        int choice = rand() % available.size();
        row = available[choice].first;
        col = available[choice].second;
    }
}

/**
 * @brief AI move dispatcher based on difficulty
 */
void aiMove() {
    int row, col;
    
    switch (aiDifficulty) {
        case Difficulty::EASY:
            makeRandomMove(row, col);
            cout << "AI (Easy) played O at position " << (row * 3 + col + 1) << endl;
            break;
        case Difficulty::MEDIUM:
            makeStrategicMove(row, col);
            cout << "AI (Medium) played O at position " << (row * 3 + col + 1) << endl;
            break;
        case Difficulty::HARD:
            findBestMoveHard(row, col);
            cout << "AI (Hard/Minimax) played O at position " << (row * 3 + col + 1) << endl;
            break;
    }
    
    makeMove(row, col);
}

// ==================== GAME MODES ====================

bool playAgain() {
    char choice;
    setColor(Color::YELLOW);
    cout << setw(35) << "\nPlay again? (Y/N): ";
    cin >> choice;
    cin.ignore(10000, '\n');
    setColor(Color::WHITE);
    return (choice == 'Y' || choice == 'y');
}

/**
 * @brief Single-player game mode
 */
void singlePlayerGame() {
    // Select difficulty
    clearConsole();
    setColor(Color::CYAN);
    cout << "\n        Select AI Difficulty:\n";
    cout << "        1. Easy   - Random moves\n";
    cout << "        2. Medium - Strategic play\n";
    cout << "        3. Hard   - Unbeatable (Minimax)\n";
    cout << "        Choice: ";
    setColor(Color::WHITE);
    int diff;
    cin >> diff;
    cin.ignore();
    
    switch(diff) {
        case 1: aiDifficulty = Difficulty::EASY; break;
        case 3: aiDifficulty = Difficulty::HARD; break;
        default: aiDifficulty = Difficulty::MEDIUM; break;
    }
    
    do {
        resetGame();
        clearConsole();
        cout << setw(58) << "Enter your name: ";
        getline(cin, name1);
        name1 = toTitleCase(name1);
        name2 = "AI";

        while (!checkWinner()) {
            clearConsole();
            toeBox();
            if (token == 'X') {
                gameLogic();
            } else {
                aiMove();
                token = 'X';
                cout << "Press Enter to continue...";
                cin.get();
            }
        }

        clearConsole();
        toeBox();
        
        if (gameTie) {
            stats.recordDraw();
            setColor(Color::RED);
            cout << setw(58) << "It's a draw!\n";
            setColor(Color::WHITE);
        } else {
            char winnerToken = (token == 'X') ? 'O' : 'X';
            stats.recordWin(winnerToken == 'X');
            setColor(Color::YELLOW);
            cout << setw(70) << "*************************************************\n";
            cout << setw(45) << ((winnerToken == 'X') ? name1 : name2) << " Wins!\n";
            cout << setw(70) << "*************************************************\n";
            setColor(Color::WHITE);

            if (winnerToken == 'X') {
                setColor(Color::GREEN);
                cout << "\n        🏆 YOU BEAT THE AI! 🏆\n";
                cout << "        Congratulations!\n\n";
                setColor(Color::WHITE);
            }
        }
    } while (playAgain());
}

/**
 * @brief Two-player game mode
 */
void twoPlayerGame() {
    do {
        resetGame();
        clearConsole();
        cout << setw(58) << "Enter first player name: ";
        getline(cin, name1);
        name1 = toTitleCase(name1);

        cout << setw(58) << "Enter second player name: ";
        getline(cin, name2);
        name2 = toTitleCase(name2);

        while (!checkWinner()) {
            clearConsole();
            toeBox();
            gameLogic();
        }

        clearConsole();
        toeBox();
        
        if (gameTie) {
            stats.recordDraw();
            setColor(Color::RED);
            cout << setw(58) << "It's a draw!\n";
            setColor(Color::WHITE);
        } else {
            char winnerToken = (token == 'X') ? 'O' : 'X';
            stats.recordWin(winnerToken == 'X');
            setColor(Color::YELLOW);
            cout << setw(70) << "*************************************************\n";
            cout << setw(45) << ((winnerToken == 'X') ? name1 : name2) << " Wins!\n";
            cout << setw(70) << "*************************************************\n";
            setColor(Color::WHITE);
        }
    } while (playAgain());
}

/**
 * @brief Displays game rules
 */
void showRules() {
    clearConsole();
    setColor(Color::BLUE);
    cout << setw(width) << " ___________________________________________________________________________\n";
    cout << setw(width) << "|                         TIC TAC TOE RULES                                 |\n";
    cout << setw(width) << "|___________________________________________________________________________|\n";
    cout << setw(width) << "|1. The game is played on a 3x3 grid.                                       |\n";
    cout << setw(width) << "|2. Players take turns placing marks (X or O) in an empty cell.            |\n";
    cout << setw(width) << "|3. First player to get 3 marks in a row wins.                              |\n";
    cout << setw(width) << "|4. If all 9 cells are filled with no winner, it's a tie.                   |\n";
    cout << setw(width) << "|5. Press 0 during game for menu (undo, history, stats).                    |\n";
    cout << setw(width) << "|                  Press Enter to return...                                 |\n";
    cout << setw(width) << "*****************************************************************************\n";
    setColor(Color::WHITE);
    cin.get();
}

/**
 * @brief Main menu
 */
void showMenu() {
    int choice;
    while (true) {
        clearConsole();
        setColor(Color::YELLOW);
        cout << "             TIC TAC TOE (Enhanced)     \n";
        setColor(Color::MAGENTA);
        cout << "        -----------------------  \n";
        setColor(Color::CYAN);
        cout << "        1. Two Player Mode\n";
        cout << "        2. Single Player Mode (vs AI)\n";
        cout << "        3. View Statistics\n";
        cout << "        4. View Game Rules\n";
        cout << "        5. Exit\n";
        setColor(Color::MAGENTA);
        cout << "        ***********************\n";
        setColor(Color::YELLOW);
        cout << "        Enter choice: ";
        setColor(Color::WHITE);

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore();

        switch (choice) {
            case 1:
                twoPlayerGame();
                break;
            case 2:
                singlePlayerGame();
                break;
            case 3:
                showStatistics();
                break;
            case 4:
                showRules();
                break;
            case 5:
                setColor(Color::RED);
                cout << setw(50) << "Exiting. Goodbye!\n";
                setColor(Color::WHITE);
                return;
            default:
                setColor(Color::RED);
                cout << "Invalid choice!\n";
                setColor(Color::WHITE);
                cin.get();
                break;
        }
    }
}

/**
 * @brief Main entry point
 */
int main() {
    srand(static_cast<unsigned>(time(0)));
    showMenu();
    return 0;
}
