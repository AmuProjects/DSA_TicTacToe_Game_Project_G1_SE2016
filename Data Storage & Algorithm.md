
## 🧠 Data Storage & Algorithm Selection in Tic Tac Toe

This section describes how data is stored and manipulated in our Tic Tac Toe game, the algorithms used to determine game logic, and how we apply computer science concepts like time/space complexity and core data structures and algorithms.

---

### 📦 Data Storage

#### 🗂️ Board Representation

```cpp
char board[3][3];
```

* **Type**: 2D array of `char`
* **Size**: Fixed (3x3)
* **Purpose**: Represents game state — each cell holds `'X'`, `'O'`, or `' '` (empty)

#### 🔍 Time & Space Complexity

| Operation           | Time Complexity | Space Complexity |
| ------------------- | --------------- | ---------------- |
| Accessing a cell    | O(1)            | O(1)             |
| Updating a cell     | O(1)            | O(1)             |
| Checking full board | O(n²) → O(9)    | O(1)             |

> Note: Since `n = 3`, we treat most operations as **constant time** for practical purposes.

---

### 🧮 Algorithms Used

#### 1. **Game Loop (Main Driver)**

* Iterates until a win or draw is detected.
* Alternates player turns, processes input, updates board, checks state.

```cpp
while (!isGameOver) {
   // Get input → Validate → Update board → Check game state
}
```

* **Time Complexity per iteration**: O(1)
* **Total Time Complexity (worst case)**: O(9)

---

#### 2. **Win Detection Algorithm**

```cpp
bool checkWin(char player);
```

* Checks:

  * 3 Rows
  * 3 Columns
  * 2 Diagonals

* **Total Checks**: 8

* **Time Complexity**: O(1)

* **Space Complexity**: O(1)

##### DSA Concept Used:

* **Pattern Checking**: Similar to fixed-size pattern recognition
* **Control Structures**: Conditional branches and loops

---

#### 3. **Draw Detection**

```cpp
bool isDraw();
```

* Traverses entire board to ensure all cells are filled.
* **Time Complexity**: O(n²) → O(9)
* **Space Complexity**: O(1)

---

### 🤖 AI Strategies

We implemented Priority-based Heuristic Algorithm (or Rule-based AI for Tic-Tac-Toe).:

*** Algorithm & Key Characteristics: ***
** Immediate Win Check (Greedy Strategy) **

* First, the AI checks if it can win immediately by placing an 'O' in any empty cell (using wouldWin()).

* This is a greedy choice—prioritizing the winning move if it exists.

*** Block Opponent's Win (Defensive Strategy) ***

* If the opponent ('X') can win in the next move, the AI blocks them by placing an 'O' in that position.

* This mimics the minimax concept of preventing the opponent's advantage, but without full tree traversal.

*** Center Control (Heuristic) ***

* The center is the most strategic position in Tic-Tac-Toe, so the AI takes it if available.

* This is a static evaluation heuristic (common in board games).

*** Corner Preference (Heuristic) ***

* Corners offer more winning opportunities, so the AI prioritizes them next.

* Another heuristic-based choice (empirically strong in Tic-Tac-Toe).

*** Edge Fallback (Default) ***

* If none of the above apply, the AI takes any remaining edge.

##### DSA Concepts Used:

* **Backtracking**
* **Recursion**
* **Game Trees**
* **Dynamic Evaluation**

> ⚡ Optimization: **Alpha-Beta Pruning** can reduce the time complexity significantly by skipping unnecessary branches.

---

### 🛡️ Input Validation

* Ensures:

  * Inputs are within bounds: (0 ≤ row, col ≤ 2)
  * Chosen cell is empty

```cpp
bool isValidMove(int row, int col);
```

* **Time Complexity**: O(1)
* **Space Complexity**: O(1)

---

### 🧠 Data Structures Used

| Data Structure                        | Use Case                     | Reason for Use                  |
| ------------------------------------- | ---------------------------- | ------------------------------- |
| `char[][]`                            | Board representation         | Simple, fast, memory-efficient  |
| `bool`, `int`                         | State flags, scores, indexes | Control flow and logic handling |

---

### 🧩 Summary Table

| Feature          | Algorithm / Structure | Time Complexity | Space Complexity |
| ---------------- | --------------------- | --------------- | ---------------- |
| Move Input       | Procedural            | O(1)            | O(1)             |
| Win Detection    | 8 Fixed Checks        | O(1)            | O(1)             |
| Draw Detection   | Full Board Traversal  | O(n²) → O(9)    | O(1)             |
| Random AI        | Move List + RNG       | O(n²)           | O(n²)            |
| Minimax AI       | Recursive Tree Search | O(9!)           | O(h)             |
| Input Validation | Conditionals          | O(1)            | O(1)             |

---

### 🧠 Bonus DSA Concepts Covered

* **Backtracking**: In the Minimax algorithm, AI tries different paths recursively.
* **State Space Search Tree**: The board evolves through a decision tree of moves.
* **Greedy vs. Exhaustive Search**: Random AI is greedy; Minimax is exhaustive.
* **Control Structures & Validation**: Core to managing turn-based game logic.

---

### 📘 Final Notes

This project, while simple on the surface, demonstrates foundational principles in:

* Clean data organization
* Efficient algorithm design
* Practical use of classic DSA topics
* Recursive AI logic using decision trees

It's a great introduction to how **Data Structures and Algorithms** power real games—even basic ones like Tic Tac Toe.

---
