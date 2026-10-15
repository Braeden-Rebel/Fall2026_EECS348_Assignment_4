/*
Name: EECS 348 Assignment 4
Description: Program that prints solutions to provided sudoku puzzles in text files
Inputs: Any number of file paths to sudoku puzzles
Outputs: All solutions in output stream or any errors with creating solutions
Collaborators: None
Sources: Gemini, ChatGPT
Author: Braeden Rebel
Creation Date: 10/14/2026
*/

// Include packages (Gemini)
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Use input output namesapce (Braeden)
using namespace std;

// SudokuBoard class that represents a valid sudoku board (Braeden)
class SudokuBoard {
private:
    // Internal 2D array to represent 9x9 sudoku board (Gemini)
    int board[9][9];

    // Returns if the row index and column index are on the board (Braeden)
    bool doesPositionExist(int rowIndex, int columnIndex)
    {
        // Returns if the row index is within [0, 9) and column index is within [0, 9) since that represents if a position is valid or not (Braeden)
        return 0 <= rowIndex && rowIndex < 9 && 0 <= columnIndex && columnIndex < 9;
    }

    // Returns if a digit is allowed in a cell or not (Braeden)
    bool isDigitAllowed(int digit)
    {
        // Return if the digit is within [1, 9] as that represents a valid digit for a cell (Braeden)
        return 1 <= digit && digit <= 9;
    }

    // Returns if a digit is valid at the row indicated at the index (Braeden)
    bool isDigitValidAtRow(int rowIndex, int digit)
    {
        // For every column (Gemini)
        for (int columnIndex = 0; columnIndex < 9; ++columnIndex) {
            // Check if the column within the row at row index already has the digit (Gemini)
            if (board[rowIndex][columnIndex] == digit) {
                // If the digit already in the row, then the digit is invalid at the row (Gemini)
                return false;
            }
        }
        // If all columns were checked and no duplicate digits found, then the digit is valid (Braeden)
        return true;
    }

    // Returns if a digit is valid at the column indicated by the index (Braeden)
    bool isDigitValidAtColumn(int columnIndex, int digit)
    {
        // For every row (Gemini)
        for (int rowIndex = 0; rowIndex < 9; ++rowIndex) {
            // Check if the row within the column at the index already has the digit (Gemini)
            if (board[rowIndex][columnIndex] == digit) {
                // If the digit already in the column, then the digit is invalid at the column (Gemini)
                return false;
            }
        }
        // If all rows were checked and no duplicate digits found, then the digit is valid (Braeden)
        return true;
    }

    // Returns if a digit is valid a the block that has the row index and column index within (Braeden)
    bool isDigitValidAtBlock(int rowIndex, int columnIndex, int digit)
    {
        // Get start row of the block (Gemini)
        int startRow = rowIndex - rowIndex % 3;
        // Get starting column of the block (Gemini)
        int startCol = columnIndex - columnIndex % 3;
        // For local row indices (Gemini)
        for (int localRowIndex = 0; localRowIndex < 3; ++localRowIndex) {
            // For local column indices (Gemini)
            for (int localColumnIndex = 0; localColumnIndex < 3; ++localColumnIndex) {
                // Check if digit already in a particular cell within the block (Gemini)
                if (board[startRow + localRowIndex][startCol + localColumnIndex] == digit) {
                    // If the digit exists in the block, then the digit is invalid (Gemini)
                    return false;
                }
            }
        }
        // If all cells were checked and no duplicate digits found, then the digit is valid (Braeden)
        return true;
    }

    // Returns if a digit at a position (Indicated by row index and column index) is valid (Braeden)
    bool isDigitValidAtPosition(int rowIndex, int columnIndex, int digit) {
        // Return if the digit is valid at the row, column, and block since that means the digit is valid (Braeden)
        return isDigitValidAtRow(rowIndex, digit) && isDigitValidAtColumn(columnIndex, digit) && isDigitValidAtBlock(rowIndex, columnIndex, digit);
    }

    // Increment the two position indices given by one going left to right before top to bottom, returns if posiiton is valid or not (Braeden)
    bool incrementPosition(int& rowIndex, int& columnIndex)
    {
        // Increase the column index by 1 (Braeden)
        columnIndex++;
        // Check if the column index is now out of range (Braeden)
        if(columnIndex == 9)
        {
            // Increment row index by one and set column index to zero to ensure position is valid (Braeden)
            rowIndex++;
            columnIndex = 0;
        }
        // If the row index is now 9 then the position is now invalid, otherwise position is valid (Braeden)
        return (rowIndex == 9);
    }

    // Gets index of next blank cell from position indicated by starting indicies and assigns blank cell index to the two blank indices integers given. It returns if a blank cell was found or not (Braeden)
    bool getIndexOfNextBlankFromPosition(int startRowIndex, int startColumnIndex, int& blankRowIndex, int& blankColumnIndex)
    {
        // For every row after the starting row index (Braeden)
        for(int rowIndex = startRowIndex; rowIndex < 9; rowIndex++)
        {
            // For every column after the starting column index (Braeden)
            for (int columnIndex = startColumnIndex; columnIndex < 9; columnIndex++)
            {
                // Get the digit at the particular position (Braeden)
                int digit = board[rowIndex][columnIndex];
                // Check if the digit is valid, since blank cells have invalid digits (Braeden)
                if (!isDigitAllowed(digit))
                {
                    // Assign the two blank indices to the current indices as the current indices are at a blank cell (Braeden)
                    blankRowIndex = rowIndex;
                    blankColumnIndex = columnIndex;
                    // Return that a blank cell was found (Braeden)
                    return true;
                }
            }
            // If the first row completes then every row afterwards has to check the entire row, so the starting column index becomes 0 (Braeden)
            startColumnIndex = 0;
        }
        // Return that no blank cell was found (Braeden)
        return false;
    }

    // Function that adds all solutions at or after a given position if all preceeding positions have valid digits (Braeden)
    void getStringSolutionListFromPosition(int startRowIndex, int startColumnIndex, vector<string>& solutionList) {
        // Create position indices for blank (Braeden)
        int blankRowIndex;
        int blankColumnIndex;
        // Get next blank cell at or after the starting positions (Braeden)
        bool foundBlank = getIndexOfNextBlankFromPosition(startRowIndex, startColumnIndex, blankRowIndex, blankColumnIndex);
        // Check if any blank was found (Braeden)
        if (!foundBlank)
        {
            // If no blank and every position before start is valid, then the current board is a solution
            // Add its string to the soltuion list (Braeden)
            solutionList.push_back(toString());
        }
        else
        {
            // Loop through all allowed digits for the blank cell (Braeden)
            for (int digit = 1; digit <= 9; ++digit) {
                // Try to assign the current digit to the blank cell (Braeden)
                bool couldAssign = tryAssignPositionToValue(blankRowIndex, blankColumnIndex, digit);
                // Check if the assignment worked (Braeden)
                if (couldAssign)
                {
                    // If the assignment did work then create position indices for next position and assign them to the blank position (Braeden)
                    int nextRowIndex = blankRowIndex;
                    int nextColumnIndex = blankColumnIndex;
                    // Increment the position indices by one so that they point at the cell after the current blank (Braeden)
                    incrementPosition(nextRowIndex, nextColumnIndex);
                    // Recursively add all string solutions after the current position (Braeden)
                    getStringSolutionListFromPosition(nextRowIndex, nextColumnIndex, solutionList);
                    // Reset the blank cell to a blank state (Braeden)
                    board[blankRowIndex][blankColumnIndex] = 0;
                }
            }
        }
    }

public:
    // Constructor to create a sudoku board (Gemini)
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
    Returns true if the assignment works and false otherwise (Braeden)
    */
    bool tryAssignPositionToValue(int rowIndex, int columnIndex, int digit)
    {
        // If the position indices exist ont the board, the digit is allowed, and the digit at that position follows sudoku rules: (Braeden)
        if (doesPositionExist(rowIndex, columnIndex) && isDigitAllowed(digit) && isDigitValidAtPosition(rowIndex, columnIndex, digit))
        {
            // Assign the position to the digit (Braeden)
            board[rowIndex][columnIndex] = digit;
            // Return true to represent a successful assignment (Braeden)
            return true;
        }
        else
        {
            // Return false to represent a failed assignment (Braeden)
            return false;
        }
    }

    // Clear the cell at a certain position indicated by the indices (Braeden)
    void clearPosition(int rowIndex, int columnIndex)
    {
        // Check if the position indices exist in the board (Braeden)
        if (doesPositionExist(rowIndex, columnIndex))
        {
            // Clear the cell at the position if it exists (Braeden)
            board[rowIndex][columnIndex] = 0;
        }
    }

    // Clear the entire board (Braeden)
    void clear()
    {
        // For every row in the board (Braeden)
        for(int row = 0; row < 9; row++)
        {
            // For every column in each row (Braeden)
            for(int column = 0; column < 9; column++)
            {
                // Clear at that position (Braeden)
                clearPosition(row, column);
            }
        }
    }

    /*
    Returns the string representation of the board (Braeden)
    */
    string toString() const {
        // Create return string (Braeden)
        string boardString;
        // For every row (Gemini)
        for (int i = 0; i < 9; ++i) {
            // If the index is zero then start as a new line (Braeden)
            if (i != 0) boardString += "\n";
            // For every column (Gemini)
            for (int j = 0; j < 9; ++j) {
                // If the value of the cell at i and j is 0 (Gemini)
                if (board[i][j] == 0) {
                    // Then add an underscore to the string (Gemini)
                    boardString += "_ ";
                } else {
                    // Otherwise add the digit present at the cell to the string (Gemini)
                    boardString += to_string(board[i][j]) += " ";
                }
            }
        }
        // Return the final string as it now represents the board (Braeden)
        return boardString;
    }

    // Get all string representations of solutions that exist for the board (Braeden)
    vector<string> getStringSolutionList() {
        // Create solution list (Braeden)
        vector<string> solutionList;
        // Assign all solutions possible from the starting position to get all solutions possible for the board (Braeden)
        getStringSolutionListFromPosition(0, 0, solutionList);
        // Return the compelte solution list (Braeden)
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
            // Create variable to track how many valid characters found (Braeden)
            int validCharacterCount = 0;
            // For every character in the line (Braeden)
            for(int i = 0; i < line.length(); i++)
            {
                char character = line[i];
                // If the character is an _ (Braeden)
                if (character == '_')
                {
                    // Add that there was a valid character (Space) before incrementing
                    validCharacterCount++;
                    continue;
                }
                // Else check if the character is a valid digit (Braeden)
                else if (character <= '9' && character >= '0')
                {
                    // Get the digit by subtracting the character by zero (Braeden)
                    int digit = character - '0';
                    // Try to assign the position indicated by the columnIndex and rowIndex to the digit (Braeden)
                    // Digit has to be allowed, and position indices are correct, so if assignment fails then board is invalid (Braeden)
                    if (!board.tryAssignPositionToValue(rowIndex, validCharacterCount, digit))
                    {
                        // Board is invalid so add  to error stream that the board in the file is invalid (Braeden)
                        cerr << "Board in file is invalid" << endl;
                        // Return that the line was not processed successfully (Braeden)
                        return false;
                    }
                    else
                    {
                        // Increment valid character count as character loaded in (Braeden)
                        validCharacterCount++;
                    }
                }
                // Else, check that the character is not an escape or space before treating as unrecoginized (Braeden)
                else if (character != '\n' && character != '\r' && character != ' ')
                {
                    // Add to error stream that there was an unrecognized character at the line (Braeden)
                    cerr << "Unrecognized character " << character << " at line " << rowIndex << endl;
                    // Return that the line was not processed successfully (Braeden)
                    return false;
                }
            }

            // Check if character count is at proper length (Braeden)
            if (validCharacterCount != 9)
            {
                // Print message informing user of bad character length (Braeden)
                cerr << "Bad line length at line " << rowIndex << endl;
                // Return that there were errors (Braeden)
                return false;
            }
            else
            {
                // Since processing is complete then the line was processed successfully, return true to indicate this (Braeden)
                return true;
            }
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
                cerr << "Filepath " << filePath << " could not be opened" << endl;
                // Return that the file was not processed successfully (Braeden)
                return false;
            }
            else
            {
                // Clear whatever is currently in the board (Braeden)
                board.clear();

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
                    cout << "Original Board:" << endl;

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
                // Add ending separator to output stream (Braeden)
                cout << "---------------------------" << endl;
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