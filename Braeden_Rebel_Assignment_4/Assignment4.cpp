/**
 * Name of Program: EECS 348 Assignment 4 - Sudoku Solver
 * Brief Description: An object-oriented C++ program that reads partially completed 
 *                    Sudoku puzzles from text files, implements a recursive 
 *                    depth-first search with backtracking using classes and objects, 
 *                    and prints all valid solutions or "No solution found".
 * Inputs: puzzle1.txt, puzzle2.txt, puzzle3.txt, puzzle4.txt, puzzle5.txt
 * Output: Puzzle file name, original puzzle grid, and solved puzzle grid(s) or "No solution found"
 * All Collaborators: None
 * Other Sources for Code: C++ Standard Library documentation, EECS 348 Assignment 4 Instructions[cite: 1, 2, 3, 4]
 * Author: Student
 * Creation Date: October 13, 2026
 */

#include <iostream>  // Authored by student: Included for standard input/output stream operations
#include <fstream>   // Authored by student: Included for file input/output handling
#include <vector>    // Authored by student: Included for potential collection management
#include <string>    // Authored by student: Included for string manipulation (filenames)

/**
 * Class SudokuSolver
 * Encapsulates the 9x9 Sudoku grid, puzzle loading, validation rules, 
 * and recursive backtracking solver using object-oriented principles.
 */
class SudokuSolver {
private:
    int board[9][9];       // Authored by student: 2D array representing the 9x9 Sudoku board
    std::string filename;  // Authored by student: Stores the name of the current puzzle file

    /**
     * Checks whether placing a given number at board[row][col] is valid 
     * according to Sudoku row, column, and 3x3 block constraints.
     */
    bool isValid(int row, int col, int num) {
        // Authored by student: Check if the number already exists in the current row
        for (int c = 0; c < 9; ++c) {
            if (board[row][c] == num) {
                return false;
            }
        }

        // Authored by student: Check if the number already exists in the current column
        for (int r = 0; r < 9; ++r) {
            if (board[r][col] == num) {
                return false;
            }
        }

        // Authored by student: Check if the number already exists in the 3x3 sub-grid (block)
        int startRow = row - row % 3;
        int startCol = col - col % 3;
        for (int r = 0; r < 3; ++r) {
            for (int c = 0; c < 3; ++c) {
                if (board[startRow + r][startCol + c] == num) {
                    return false;
                }
            }
        }

        return true; // Authored by student: Placement is valid
    }

    /**
     * Recursive depth-first search function with backtracking to solve the Sudoku puzzle.
     * Finds and prints all possible solutions if multiple exist.
     */
    bool solveRecursive(int row, int col, int& solutionCount) {
        // Authored by student: Base case - if row reaches 9, the entire board is successfully filled
        if (row == 9) {
            solutionCount++;
            std::cout << "Solution " << solutionCount << ":\n";
            printBoard();
            return true; 
        }

        // Authored by student: Determine next cell coordinates (move across columns, then down rows)
        int nextRow = (col == 8) ? row + 1 : row;
        int nextCol = (col == 8) ? 0 : col + 1;

        // Authored by student: If the current cell is already filled, skip to the next cell
        if (board[row][col] != 0) {
            return solveRecursive(nextRow, nextCol, solutionCount);
        }

        bool foundAny = false;
        // Authored by student: Try candidate digits from 1 through 9
        for (int num = 1; num <= 9; ++num) {
            if (isValid(row, col, num)) {
                board[row][col] = num; // Authored by student: Temporarily place candidate number
                
                // Authored by student: Recursively attempt to solve for the next cell
                if (solveRecursive(nextRow, nextCol, solutionCount)) {
                    foundAny = true;
                }
                
                board[row][col] = 0;   // Authored by student: Backtrack by resetting cell to empty (0)
            }
        }

        return foundAny;
    }

public:
    /**
     * Constructor to initialize the SudokuSolver object with a specific puzzle filename.
     */
    SudokuSolver(const std::string& fname) : filename(fname) {
        // Authored by student: Initialize board cells to 0 (empty)
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                board[i][j] = 0;
            }
        }
    }

    /**
     * Loads the puzzle from the text file, interpreting underscores ('_') as empty cells (0).
     */
    bool loadPuzzle() {
        std::ifstream file(filename);
        // Authored by student: Check if file successfully opened
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << "\n";
            return false;
        }

        // Authored by student: Read 9x9 grid elements from file
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                char ch;
                if (!(file >> ch)) {
                    std::cerr << "Error: Malformed puzzle file " << filename << "\n";
                    return false;
                }
                if (ch == '_') {
                    board[i][j] = 0; // Authored by student: Represent empty cell as 0
                } else if (ch >= '1' && ch <= '9') {
                    board[i][j] = ch - '0'; // Authored by student: Convert char digit to integer
                } else {
                    board[i][j] = 0;
                }
            }
        }
        file.close();
        return true;
    }

    /**
     * Prints the current state of the Sudoku board, formatting empty cells as underscores ('_').
     */
    void printBoard() const {
        // Authored by student: Iterate through each cell and print formatted output
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (board[i][j] == 0) {
                    std::cout << "_ ";
                } else {
                    std::cout << board[i][j] << " ";
                }
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    /**
     * Initiates the solving process, printing the file name, original puzzle, 
     * and solution results or "No solution found".
     */
    void solve() {
        std::cout << "Puzzle File Name: " << filename << "\n";
        std::cout << "Original Puzzle:\n";
        printBoard();

        int solutionCount = 0;
        std::cout << "Solving...\n";
        bool hasSolution = solveRecursive(0, 0, solutionCount);

        // Authored by student: Output message if no valid solution was found
        if (!hasSolution || solutionCount == 0) {
            std::cout << "No solution found\n";
        }
        std::cout << "----------------------------------------\n\n";
    }
};

/**
 * Main function to instantiate objects for all five puzzle files and execute their solutions.
 */
int main() {
    // Authored by student: Array containing the required puzzle filenames
    std::string puzzleFiles[] = {"puzzle1.txt", "puzzle2.txt", "puzzle3.txt", "puzzle4.txt", "puzzle5.txt"};

    // Authored by student: Iterate through each puzzle file, load it, and solve it using objects
    for (const std::string& file : puzzleFiles) {
        SudokuSolver solver(file);
        if (solver.loadPuzzle()) {
            solver.solve();
        }
    }

    return 0;
}