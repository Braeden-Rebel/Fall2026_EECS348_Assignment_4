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

    bool isDigitValidAtRow(int rowIndex, int digit)
    {
        for (int columnIndex = 0; columnIndex < 9; ++columnIndex) {
            if (board[rowIndex][columnIndex] == digit) {
                return false;
            }
        }
        return true;
    }

    bool isDigitValidAtColumn(int columnIndex, int digit)
    {
        for (int rowIndex = 0; rowIndex < 9; ++rowIndex) {
            if (board[rowIndex][columnIndex] == digit) {
                return false;
            }
        }
        return true;
    }

    bool isDigitValidAtBlock(int rowIndex, int columnIndex, int digit)
    {
        int startRow = rowIndex - rowIndex % 3;
        int startCol = columnIndex - columnIndex % 3;
        for (int localRowIndex = 0; localRowIndex < 3; ++localRowIndex) {
            for (int localColumnIndex = 0; localColumnIndex < 3; ++localColumnIndex) {
                if (board[startRow + localRowIndex][startCol + localColumnIndex] == digit) {
                    return false;
                }
            }
        }
        return true;
    }

    bool isDigitValidAtPosition(int rowIndex, int columnIndex, int digit) {
        return isDigitValidAtRow(rowIndex, digit) && isDigitValidAtColumn(columnIndex, digit) && isDigitValidAtBlock(rowIndex, columnIndex, digit);
    }

    bool incrementPosition(int& rowIndex, int& columnIndex)
    {
        columnIndex++;
        if(columnIndex == 9)
        {
            rowIndex++;
            columnIndex = 0;
        }
        return (rowIndex == 9);
    }

    bool getIndexOfNextBlankFromPosition(int startRowIndex, int startColumnIndex, int& blankRowIndex, int& blankColumnIndex)
    {
        for(int rowIndex = startRowIndex; rowIndex < 9; rowIndex++)
        {
            for (int columnIndex = startColumnIndex; columnIndex < 9; columnIndex++)
            {
                int digit = board[rowIndex][columnIndex];
                if (!isDigitAllowed(digit))
                {
                    blankRowIndex = rowIndex;
                    blankColumnIndex = columnIndex;
                    return true;
                }
            }
            startColumnIndex = 0;
        }
        return false;
    }

    bool getStringSolutionListFromPosition(int startRowIndex, int startColumnIndex, vector<string>& solutionList) {
        int blankRowIndex;
        int blankColumnIndex;
        bool foundBlank = getIndexOfNextBlankFromPosition(startRowIndex, startColumnIndex, blankRowIndex, blankColumnIndex);

        if (!foundBlank)
        {
            solutionList.push_back(toString());
            return true;
        }
        else
        {
            //cout << "(" << blankRowIndex << ", " << blankColumnIndex << ")" <<endl << endl;
            bool foundAny = false;
            for (int num = 1; num <= 9; ++num) {
                bool couldAssign = TryAssignPositionToValue(blankRowIndex, blankColumnIndex, num);

                if (couldAssign)
                {
                    int nextRowIndex = blankRowIndex;
                    int nextColumnIndex = blankColumnIndex;
                    incrementPosition(nextRowIndex, nextColumnIndex);
                    getStringSolutionListFromPosition(nextRowIndex, nextColumnIndex, solutionList);
                    board[blankRowIndex][blankColumnIndex] = 0;
                }
            }

            return foundAny;
        }
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
        if (doesPositionExist(rowIndex, columnIndex) && isDigitAllowed(digit) && isDigitValidAtPosition(rowIndex, columnIndex, digit))
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

    string toString() const {
        string boardString;
        for (int i = 0; i < 9; ++i) {
            if (i != 0) boardString += "\n";
            for (int j = 0; j < 9; ++j) {
                if (board[i][j] == 0) {
                    boardString += "_ ";
                } else {
                    boardString += to_string(board[i][j]) += " ";
                }
            }
        }
        return boardString;
    }

    vector<string> getStringSolutionList() {
        //cout << "Puzzle File Name: " << filename << "\n";
        vector<string> solutionList;
        bool hasSolution = getStringSolutionListFromPosition(0, 0, solutionList);
        return solutionList;
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
            cout << "---------------------------" << endl;
            for (string filePath : filePaths) {
                cout << "File: " << filePath << endl << endl;
                ifstream file(filePath);
                if(file.is_open())
                {
                    bool successfulFileProcessing = processFileIntoBoard(board, filePath);
                    if (successfulFileProcessing)
                    {
                        vector<string> solutionList = board.getStringSolutionList();

                        cout << "Original Board:" << endl << endl;

                        cout << board.toString() << endl;

                        if (solutionList.empty())
                        {
                            cout << endl << "Solutions: None" << endl;
                        }
                        else
                        {
                            for(int i = 0; i < solutionList.size(); i++)
                            {
                                cout << endl << "Solution " << (i + 1) << ":" << endl;
                                cout << solutionList[i] << endl;
                            }
                        }
                    }
                }
                else
                {
                    cerr << "Error: Filepath \"" << filePath << "\" could not be opened" << endl;
                }
                cout << "---------------------------" << endl;
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