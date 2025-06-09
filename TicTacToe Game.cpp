/**
  @Title TicTacToe Game
  @Intro Implementation of a Tic-Tac-Toe game with both single and two-player modes
  @authors
            -NAME                         -ID
        1.Misiker Genene                 NSR/1450/16
        2.Biruk Getahun                  NSR/204/16
        3.Tsion Samuel                   NSR//16
        4.Heran Mohammed                 NSR//16
        5.Hilina Kitachew                NSR//16
        6.Sadam Robel                    NSR//16

  @date [Current Date]

 * This program implements a Tic-Tac-Toe game with the following features:
       - Two-player mode
       - Single-player mode against AI
       - Input validation
       - Clear console display
       - Game state tracking
 */

#include <iostream>
#include <cstdlib> // For system("clear") or system("cls")
#include <ctime>   // For random AI moves
#include <iomanip>
#include <algorithm> // for transform()
#include <cctype>    // for toupper()
#include <windows.h> // For Windows color functions

// Color definitions
#define RED "\033[31m"
#define BLUE "\033[34m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define RESET "\033[0m"

using namespace std;

// Windows color functions
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Color constants for Windows
const int RED_WIN = 12;      // Light Red
const int BLUE_WIN = 9;      // Light Blue
const int GREEN_WIN = 10;    // Light Green
const int YELLOW_WIN = 14;   // Yellow
const int CYAN_WIN = 11;     // Light Cyan
const int MAGENTA_WIN = 13;  // Light Magenta
const int WHITE = 15;        // White

char space[3][3] = {{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}};
char token = 'X';  // Current player's token
bool gameTie = false;  // Game state tracking
string name1, name2;  // Player names
int width = 60;  // Display formatting constant

void clearConsole() {
#ifdef _WIN32
    system("cls"); // For windows system
#else
    system("clear"); // For Unix-like systems (Linux , macOS ...)
#endif
}

void toeBox() {
    int x = 39;
    setColor(GREEN_WIN);
    cout << setw(width) <<" _______________________ \n";
    cout << setw(width) <<"|                       |\n";
    cout << setw(width) <<"|      TIC TAC TOE      |\n";
    cout << setw(width) <<"|_______________________|\n";
    cout << setw(width) <<"|***********************|\n";
    cout << setw(width) <<"|*|     |      |      |*|\n";

    // First row
    cout << setw(x) << "|*|  ";
    if (space[0][0] == 'X') { setColor(RED_WIN); cout << space[0][0]; setColor(GREEN_WIN); }
    else if (space[0][0] == 'O') { setColor(BLUE_WIN); cout << space[0][0]; setColor(GREEN_WIN); }
    else cout << space[0][0];
    cout << "  |  ";
    if (space[0][1] == 'X') { setColor(RED_WIN); cout << space[0][1]; setColor(GREEN_WIN); }
    else if (space[0][1] == 'O') { setColor(BLUE_WIN); cout << space[0][1]; setColor(GREEN_WIN); }
    else cout << space[0][1];
    cout << "   |  ";
    if (space[0][2] == 'X') { setColor(RED_WIN); cout << space[0][2]; setColor(GREEN_WIN); }
    else if (space[0][2] == 'O') { setColor(BLUE_WIN); cout << space[0][2]; setColor(GREEN_WIN); }
    else cout << space[0][2];
    cout << "   |*|\n";

    cout << setw(width) <<"|*|_____|______|______|*|\n";
    cout << setw(width) <<"|*|     |      |      |*|\n";

    // Second row
    cout << setw(x) << "|*|  ";
    if (space[1][0] == 'X') { setColor(RED_WIN); cout << space[1][0]; setColor(YELLOW_WIN); }
    else if (space[1][0] == 'O') { setColor(BLUE_WIN); cout << space[1][0]; setColor(YELLOW_WIN); }
    else cout << space[1][0];
    cout << "  |  ";
    if (space[1][1] == 'X') { setColor(RED_WIN); cout << space[1][1]; setColor(YELLOW_WIN); }
    else if (space[1][1] == 'O') { setColor(BLUE_WIN); cout << space[1][1]; setColor(YELLOW_WIN); }
    else cout << space[1][1];
    cout << "   |  ";
    if (space[1][2] == 'X') { setColor(RED_WIN); cout << space[1][2]; setColor(YELLOW_WIN); }
    else if (space[1][2] == 'O') { setColor(BLUE_WIN); cout << space[1][2]; setColor(YELLOW_WIN); }
    else cout << space[1][2];
    cout << "   |*|\n";

    cout << setw(width) <<"|*|_____|______|______|*|\n";
    cout << setw(width) <<"|*|     |      |      |*|\n";

    // Third row
    cout << setw(x) << "|*|  ";
    if (space[2][0] == 'X') { setColor(RED_WIN); cout << space[2][0]; setColor(RED_WIN); }
    else if (space[2][0] == 'O') { setColor(BLUE_WIN); cout << space[2][0]; setColor(RED_WIN); }
    else cout << space[2][0];
    cout << "  |  ";
    if (space[2][1] == 'X') { setColor(RED_WIN); cout << space[2][1]; setColor(RED_WIN); }
    else if (space[2][1] == 'O') { setColor(BLUE_WIN); cout << space[2][1]; setColor(RED_WIN); }
    else cout << space[2][1];
    cout << "   |  ";
    if (space[2][2] == 'X') { setColor(RED_WIN); cout << space[2][2]; setColor(RED_WIN); }
    else if (space[2][2] == 'O') { setColor(BLUE_WIN); cout << space[2][2]; setColor(RED_WIN); }
    else cout << space[2][2];
    cout << "   |*|\n";

    cout << setw(width) <<"|*|     |      |      |*|\n";
    cout << setw(width) <<" *********************** \n";
    setColor(WHITE);
}


//1. Check winner

bool checkWinner() {
    // First check for a winner
    for (int i = 0; i < 3; i++) {
        if ((space[i][0] == space[i][1] && space[i][1] == space[i][2]) 
            (space[0][i] == space[1][i] && space[1][i] == space[2][i])) {
            return true;
        }
    }
    if ((space[0][0] == space[1][1] && space[1][1] == space[2][2]) 
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

    // If we get here, it's a draw
    if (isDraw) {
        gameTie = true;
        return true;
    }

    return false;
}


void gameLogic() {
    int digit;
    bool validChoice = false;

    while (!validChoice) {
        clearConsole();
        toeBox();

        // Check if the game is already a draw
        if (gameTie) {
            return;
        }

        if (token == 'X') {
            cout << setw(35) << name1 << " Please Enter Choice (1-9): ";
        } else {
            cout << setw(35) << name2 << " Please Enter Choice (1-9): ";
        }

        if (!(cin >> digit)) {  // Check for invalid input
            cin.clear();  // Clear error flags
            cin.ignore(10000, '\n');  // Clear input buffer
            setColor(RED_WIN);
            cout << setw(35) << "\nInvalid input. Please enter a number between 1 and 9.\n";
            cout << setw(35) << "Press Enter to continue...";
            cin.get();
            setColor(WHITE);
            continue;
        }

        if (digit < 1 || digit > 9) {
            setColor(RED_WIN);
            cout << setw(35) << "\nInvalid choice. Please enter a number between 1 and 9.\n";
            cout << setw(35) << "Press Enter to continue...";
            cin.get();
            setColor(WHITE);
            continue;
        }

        int row = (digit - 1) / 3;
        int column = (digit - 1) % 3;

        if (space[row][column] != 'X' && space[row][column] != 'O') {
            space[row][column] = token;
            token = (token == 'X') ? 'O' : 'X';
            validChoice = true;
        } else {
            setColor(RED_WIN);
            cout << setw(35) << "\nThis space is already occupied. Try again.\n";
            cout << setw(35) << "Press Enter to continue...";
            cin.get();
            cin.ignore(10000, '\n');  // Clear any remaining input
            setColor(WHITE);
        }
    }
}


//2.reset game and wouldwin


void aiMove() {
    // 1.Try to win
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (space[i][j] != 'X' && space[i][j] != 'O') {
                if (wouldWin(i, j, 'O')) {
                    space[i][j] = 'O';
                    cout << "AI played O at position " << (i * 3 + j + 1) << endl;
                    return;
                }
            }
        }
    }

    // 2.Block opponent's winning move
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (space[i][j] != 'X' && space[i][j] != 'O') {
                if (wouldWin(i, j, 'X')) {
                    space[i][j] = 'O';
                    cout << "AI played O at position " << (i * 3 + j + 1) << endl;
                    return;
                }
            }
        }
    }

    // 3.Take center if available
    if (space[1][1] != 'X' && space[1][1] != 'O') {
        space[1][1] = 'O';
        cout << "AI played O at position 5" << endl;
        return;
    }

    // 4.Take corners if available
    int corners[4][2] = {{0,0}, {0,2}, {2,0}, {2,2}};
    for (int i = 0; i < 4; i++) {
        if (space[corners[i][0]][corners[i][1]] != 'X' && space[corners[i][0]][corners[i][1]] != 'O') {
            space[corners[i][0]][corners[i][1]] = 'O';
            cout << "AI played O at position " << (corners[i][0] * 3 + corners[i][1] + 1) << endl;
            return;
        }
    }

    // 5.Take any available edge
    int edges[4][2] = {{0,1}, {1,0}, {1,2}, {2,1}};
    for (int i = 0; i < 4; i++) {
        if (space[edges[i][0]][edges[i][1]] != 'X' && space[edges[i][0]][edges[i][1]] != 'O') {
            space[edges[i][0]][edges[i][1]] = 'O';
            cout << "AI played O at position " << (edges[i][0] * 3 + edges[i][1] + 1) << endl;
            return;
        }
    }
}

//3.play again and string

//4.Single player Function

//5.Two player Function

void showMenu() {
    int choice;
    while (true) {
        clearConsole();
        // Top box
        setColor(YELLOW_WIN);
        cout << "             TIC TAC TOE         " << endl;
        setColor(MAGENTA_WIN);
        cout << "        -----------------------  " << endl;
        // Menu options
        cout << "        ";
        setColor(CYAN_WIN);
        cout << "1. Start New Game";
        setColor(WHITE);
        cout << "       "<< endl;

        cout << "        ";
        setColor(CYAN_WIN);
        cout << "2. Single player";
        setColor(WHITE);
        cout << "       " << endl;

        cout << "        ";
        setColor(CYAN_WIN);
        cout << "3. View Game Rules";
        setColor(WHITE);
        cout << "        " << endl;

        cout << "        ";
        setColor(CYAN_WIN);
        cout << "4. Exit";
        setColor(WHITE);
        cout << "                  " << endl;

        setColor(WHITE);
        setColor(MAGENTA_WIN);
        cout << "        ***********************" << endl;
        setColor(YELLOW_WIN);
        cout << "        Enter your choice: ";
        setColor(WHITE);

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\nInvalid input. Please enter a number between 1 and 4.\n";
            cout << "Press Enter to continue...";
            cin.get();
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
                clearConsole();
                setColor(BLUE_WIN);
                cout << setw(width) << " ____________________________________________________________________________________________\n";
                cout << setw(width) << "|                                                                                            |\n";
                cout << setw(width) << "|                                      TIC TAC TOE RULES:                                    |\n";
                cout << setw(width) << "|____________________________________________________________________________________________|\n";
                cout << setw(width) << "|********************************************************************************************|\n";
                cout << setw(width) << "|1. The game is played on a 3x3 grid.                                                        |\n";
                cout << setw(width) << "|2. Players take turns placing their marks (X or O) in an empty cell.                        |\n";
                cout << setw(width) << "|3. The first player to get 3 marks in a row (horizontally, vertically, or diagonally) wins. |\n";
                cout << setw(width) << "|4. If all 9 cells are filled and no one has won, the game is a tie.                         |\n";
                cout << setw(width) << "|                  Press Enter to return to the menu....                                     |\n";
                cout << setw(width) << "**********************************************************************************************\n";
                cin.get();
                setColor(WHITE);
                break;
            case 4:
                setColor(RED_WIN);
                cout << setw(50) << "Exiting the game. Goodbye!\n";
                setColor(WHITE);
                return;
            default:
                setColor(RED_WIN);
                cout << setw(50) << "\nInvalid choice. Please enter a number between 1 and 4.\n";
                cout << setw(50) << "Press Enter to continue...";
                cin.get();
                setColor(WHITE);
                break;
        }
    }
}

int main() {
    srand(time(0));  // Initialize random seed for AI moves
    showMenu();
    return 0;
}
