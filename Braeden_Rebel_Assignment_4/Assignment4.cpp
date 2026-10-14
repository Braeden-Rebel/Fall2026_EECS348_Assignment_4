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

    /*
    Returns the string representation of the board (Braeden)
    */
    string toString() const {
        // Create return string (Braeden)
        string boardString;
        // For every row (ChatGPT)
        for (int i = 0; i < 9; ++i) {
            // If the index is zero then start as a new line (Braeden)
            if (i != 0) boardString += "\n";
            // For every column (ChatGPT)
            for (int j = 0; j < 9; ++j) {
                // If the value of the cell at i and j is 0 (ChatGPT)
                if (board[i][j] == 0) {
                    // Then add an underscore to the string (ChatGPT)
                    boardString += "_ ";
                } else {
                    // Otherwise add the digit present at the cell to the string (ChatGPT)
                    boardString += to_string(board[i][j]) += " ";
                }
            }
        }
        // Return the final string as it now represents the board (Braeden)
        return boardString;
    }

    vector<string> getStringSolutionList() {
        vector<string> solutionList;
        bool hasSolution = getStringSolutionListFromPosition(0, 0, solutionList);
        return solutionList;
    }
};

/*
Class that deals with how to translate file paths into sudoku puzzles (Braeden)
*/
class SudokuInputReader
{
    private:
        // A vector that has the list of filepaths the input reader was started with (Braeden)
        vector<string> filePaths;

        // Processes a single line and puts it into the given board according to the given row index (Braeden)
        bool processLineIntoBoard(SudokuBoard &board, string line, int rowIndex)
        {
            // If the line's length is not 17, then it does not match the format shown in the example puzzles (Braeden)
            if (line.length() != 17)
            {
                // Add an error message about the bad row format to error stream (Braeden)
                cerr << "Incorrect row length at line " << rowIndex << endl;
                // Return that the line did not process without error (Braeden)
                return false;
            }
            // If the line's length is 17 then it does follow the format (Braeden)
            else
            {
                // For all indices in the line [incrementing by two to ignore spcaes] (Braeden)
                for(int i = 0; i < 17; i+=2)
                {
                    // Get specific character at the index (Braeden)
                    char character = line[i];
                    
                    // If the charcater is an _ (Braeden)
                    if (character == '_')
                    {
                        continue;
                    }
                    // Else if the character is a valid digit (Braeden)
                    else if ('9' >= character >= '0')
                    {
                        // Get the columnIndex by dividning the character index by two (Braeden)
                        int columnIndex = i / 2;
                        // Get the digit by subtracting the character by zero (Braeden)
                        int digit = character - '0';
                        // Try to assign the position indicated by the columnIndex and rowIndex to the digit (Braeden)
                        // Digit has to be allowed, and position indices are correct, so if assignment fails then board is invalid (Braeden)
                        if (!board.TryAssignPositionToValue(rowIndex, columnIndex, digit))
                        {
                            // Board is invalid so add  to error stream that the board in the file is invalid (Braeden)
                            cerr << "Board in file is invalid" << endl;
                            // Return that the line was not processed successfully (Braeden)
                            return false;
                        }
                    }
                    // Else, the digit is invalid (Braeden)
                    else
                    {
                        // Add to error stream that there was an unrecognized character at the line (Braeden)
                        cerr << "Unrecognized character " << character << " at line " << rowIndex << endl;
                        // Return that the line was not processed successfully (Braeden)
                        return false;
                    }
                }
            }
            // Since processing is complete then the line was processed successfully, return true to indicate this (Braeden)
            return true;
        }

        // Processes a file at a filepath and puts the sudoku board found within the file into the board provided (Braeden)
        bool processFileIntoBoard(SudokuBoard &board, string filePath)
        {
            // Create a new ifstream from the filepath (Braeden)
            ifstream file(filePath);

            // Check if the file didn't open (Braeden)
            if (!file.is_open())
            {
                // If the file didn't open then indicate this in the error stream (Braeden)
                cerr << "Filepath " << filePath << " could not be openned";
                // Return that the file was not processed successfully (Braeden)
                return false;
            }
            else
            {
                // Clear whatever is currently in the board (Braeden)
                board.Clear();

                // Start row index at zero (Braeden)
                int rowIndex = 0;
                // Create a string for each line in the file (Braeden)
                string line;
                // While there are lines to assign to line in the file (Braeden)
                while(getline(file, line))
                {
                    // Try to process the line and save if there were any errors in processing the line (Braeden)
                    bool lineWithoutErrors = processLineIntoBoard(board, line, rowIndex);
                    // If there were errors... (Braeden)
                    if (!lineWithoutErrors)
                    {
                        // Then return that the file was not processed without errors (Braeden)
                        return false;
                    }
                    // Increment the row index by one as the line is not processed (Braeden)
                    rowIndex++;
                }
                // Since the file is out of lines, all lines had to be processed succesfully, so the file is processed successfully, return true to indicate this (Braeden)
                return true;
            }
        }

    public:
        // Constructor for SudokuInputReader class that takes a vector<string> of file paths and assigns it to the filePaths member (Braeden)
        SudokuInputReader(vector<string> fPaths) : filePaths(fPaths)
        {

        };

        // Function that prints all solutions to every filepath that is within the reader (Braeden)
        void printSolutionsToAll()
        {
            // Create a board for processing (Braeden)
            SudokuBoard board;
            // Print starting separator (Braeden)
            cout << "---------------------------" << endl;
            // For every filepath in filePaths... (Braeden)
            for (string filePath : filePaths) {
                // Add name of file to output stream (Braeden)
                cout << "File: " << filePath << endl << endl;
                // Try to process the file into board and store if it did so without error into a bool (Braeden)
                bool successfulFileProcessing = processFileIntoBoard(board, filePath);
                // Check if file was processed succesfully (Braeden)
                if (successfulFileProcessing)
                {
                    // Get list of all solutions from the board with file loaded in (Braeden)
                    vector<string> solutionList = board.getStringSolutionList();

                    // Add original board label to output stream (Braeden)
                    cout << "Original Board:" << endl << endl;

                    // Add original board to output stream (Braeden)
                    cout << board.toString() << endl;

                    // If no solutions were found (Braeden)
                    if (solutionList.empty())
                    {
                        // Add that there were no solutions to output stream (Braeden)
                        cout << endl << "Solutions: None" << endl;
                    }
                    // If solution were found (Braeden)
                    else
                    {
                        // For every solution in the list (Braeden)
                        for(int i = 0; i < solutionList.size(); i++)
                        {
                            // Print solution label with current solution number (Braeden)
                            cout << endl << "Solution " << (i + 1) << ":" << endl;
                            // Print solution in the list at the index (Braeden)
                            cout << solutionList[i] << endl;
                        }
                    }
                }
            }
        }
};

// Main function that connects arguments to input readings and printing (Braeden)
int main(int argc, char *argv[]) {
    // Check if the argument count is above one (Braeden)
    if (argc < 1)
    {
        // Print a message informing user of proper program usage (Braeden)
        cout << "Usage: Assignment4.exe 1..*<filePath>";
        // Return 0 to indicate proper program close (Braeden)
        return 0;
    }
    else
    {
        // Create vector string of all paths (Braeden)
        vector<string> paths;
        // For loop adapted from ChatGPT, push all arguments after the executable name into paths vector (Braeden)
        for (int argument = 1; argument < argc; ++argument) {
            paths.push_back(argv[argument]);
        }
        
        // Create a SudokuInputReader with the paths found in the argument list (Braeden)
        SudokuInputReader sudokuInputReader(paths);

        // Have SudokuInputReader object print all solutions to the puzzles found in the files by the paths (Braeden)
        sudokuInputReader.printSolutionsToAll();

        // Return 0 to indicate that program has successfully terminated (Braeden)
        return 0;
    }
}