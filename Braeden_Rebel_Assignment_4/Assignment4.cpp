#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class SudokuSolver {
private:
    int board[9][9];
    string filename;

    bool isValid(int row, int col, int num) {
        for (int c = 0; c < 9; ++c) {
            if (board[row][c] == num) {
                return false;
            }
        }

        for (int r = 0; r < 9; ++r) {
            if (board[r][col] == num) {
                return false;
            }
        }

        int startRow = row - row % 3;
        int startCol = col - col % 3;
        for (int r = 0; r < 3; ++r) {
            for (int c = 0; c < 3; ++c) {
                if (board[startRow + r][startCol + c] == num) {
                    return false;
                }
            }
        }

        return true;
    }

    bool solveRecursive(int row, int col, int& solutionCount) {
        if (row == 9) {
            solutionCount++;
            cout << "Solution " << solutionCount << ":\n";
            printBoard();
            return true; 
        }

        int nextRow = (col == 8) ? row + 1 : row;
        int nextCol = (col == 8) ? 0 : col + 1;

        if (board[row][col] != 0) {
            return solveRecursive(nextRow, nextCol, solutionCount);
        }

        bool foundAny = false;
        for (int num = 1; num <= 9; ++num) {
            if (isValid(row, col, num)) {
                board[row][col] = num;
                
                if (solveRecursive(nextRow, nextCol, solutionCount)) {
                    foundAny = true;
                }
                
                board[row][col] = 0;
            }
        }

        return foundAny;
    }

public:
    SudokuSolver(const string& fname) : filename(fname) {
        // Authored by student: Initialize board cells to 0 (empty)
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                board[i][j] = 0;
            }
        }
    }

    bool loadPuzzle() {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << "\n";
            return false;
        }

        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                char ch;
                if (!(file >> ch)) {
                    cerr << "Error: Malformed puzzle file " << filename << "\n";
                    return false;
                }
                if (ch == '_') {
                    board[i][j] = 0;
                } else if (ch >= '1' && ch <= '9') {
                    board[i][j] = ch - '0';
                } else {
                    board[i][j] = 0;
                }
            }
        }
        file.close();
        return true;
    }

    void printBoard() const {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (board[i][j] == 0) {
                    cout << "_ ";
                } else {
                    cout << board[i][j] << " ";
                }
            }
            cout << "\n";
        }
        cout << "\n";
    }

    void solve() {
        cout << "Puzzle File Name: " << filename << "\n";
        cout << "Original Puzzle:\n";
        printBoard();

        int solutionCount = 0;
        cout << "Solving...\n";
        bool hasSolution = solveRecursive(0, 0, solutionCount);

        if (!hasSolution || solutionCount == 0) {
            cout << "No solution found\n";
        }
        cout << "----------------------------------------\n\n";
    }
};

/*
Class that can read a list of 
*/
class SudokuInputReader
{
    private:
        vector<string> filePaths;
    public:
        SudokuInputReader(vector<string> fPaths) : filePaths(fPaths)
        {

        };

        void printSolutionsToAll()
        {
            for (string filePath : filePaths) {
                SudokuSolver solver(filePath);
                if (solver.loadPuzzle()) {
                    solver.solve();
                }
            }
        }
};

int main(int argc, char *argv[]) {
    if (argc < 1)
    {
        cout << "Usage: Assignment4.exe 1..*<filePath>";
        return 0;
    }
    else
    {
        vector<string> paths;
        // For loop adapted from ChatGPT
        for (int argument = 1; argument < argc; ++argument) {
            paths.push_back(argv[argument]);
        }
        
        SudokuInputReader sudokuInputReader(paths);

        sudokuInputReader.printSolutionsToAll();

        return 0;
    }
}