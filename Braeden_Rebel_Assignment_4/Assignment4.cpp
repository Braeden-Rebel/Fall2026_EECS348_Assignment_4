#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// SudokuBoard class represents a valid sudoku board (Braeden)
class SudokuBoard {
private:
    // Internal 2D array to represent 9x9 sudoku board (Gemini)
    int board[9][9];

    bool doesPositionExist(int rowIndex, int columnIndex)
    {
        return 0 <= rowIndex && rowIndex < 9 && 0 <= columnIndex && columnIndex < 9;
    }

    bool isDigitAllowed(int digit)
    {
        return 1 <= digit && digit <= 9;
    }

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
    SudokuBoard() {
        // Create board and start all cells at zero (Gemini)
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                board[i][j] = 0;
            }
        }
    }

    /*
    Function that takes in the indices of a positon and a digit and tries to assign that position to the digit given
    Returns true if the assignment works and false otherwise
    */
    bool TryAssignPositionToValue(int rowIndex, int columnIndex, int digit)
    {
        // If the position indices exist ont the board, the digit is allowed, and the digit at that position follows sudoku rules:
        if (doesPositionExist(rowIndex, columnIndex) && isDigitAllowed(digit) && isValid(rowIndex, columnIndex, digit))
        {
            // Assign the position to the digit
            board[rowIndex][columnIndex] = digit;
            // Return true to represent a successful assignment
            return true;
        }
        else
        {
            // Return false to represent a failed assignment
            return false;
        }
    }


    void ClearPosition(int rowIndex, int columnIndex)
    {
        if (doesPositionExist(rowIndex, columnIndex))
        {
            board[rowIndex][columnIndex] = 0;
        }
    }

    void Clear()
    {
        for(int row = 0; row < 9; row++)
        {
            for(int column = 0; column < 9; column++)
            {
                ClearPosition(row, column);
            }
        }
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
        //cout << "Puzzle File Name: " << filename << "\n";
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

        bool processLineIntoBoard(SudokuBoard &board, string line, int rowIndex)
        {
            if (line.length() != 17)
            {
                cerr << "Incorrect row length at line " << rowIndex << endl;
                return false;
            }
            else
            {
                for(int i = 0; i < 17; i+=2)
                {
                    char character = line[i];
                    
                    if (character != '_')
                    {
                        int columnIndex = i / 2;
                        if (!board.TryAssignPositionToValue(rowIndex, columnIndex, character - '0'))
                        {
                            return false;
                        }
                    }
                }
            }
            return true;
        }

        bool processFileIntoBoard(SudokuBoard &board, string filePath)
        {

            ifstream file(filePath);

            if (!file.is_open())
            {
                cerr << "Filepath " << filePath << " could not be openned";
                return false;
            }
            else
            {
                board.Clear();

                int rowIndex = 0;
                string line;
                while(getline(file, line))
                {
                    bool lineWithoutErrors = processLineIntoBoard(board, line, rowIndex);
                    if (!lineWithoutErrors)
                    {
                        return false;
                    }
                    rowIndex++;
                }
                return true;
            }
        }

    public:
        SudokuInputReader(vector<string> fPaths) : filePaths(fPaths)
        {

        };

        void printSolutionsToAll()
        {
            SudokuBoard board;
            for (string filePath : filePaths) {
                cout << "File: " << filePath << endl << endl;
                ifstream file(filePath);
                if(file.is_open())
                {
                    bool successfulFileProcessing = processFileIntoBoard(board, filePath);
                    if (successfulFileProcessing)
                    {
                        board.solve();
                    }
                }
                else
                {
                    cerr << "Error: Filepath \"" << filePath << "\" could not be opened" << endl;
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