/*
Program : Sudoku Solver
Description :
This problem takes a file input and then prints any possible solutions or that none were found
Input : Takes an input file at runtime
Output : Potential solutions for sudoku puzzle on console
Collaborators : None
Sources : None
Author : Braeden Rebel
Creation Date : 10/7/2026
*/

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class SudokuBoard
{
    private:
        int board[9][9] = {};

        bool IsValidPositionIndex(int index)
        {
            return index >= 0 && index < 9;
        }

        bool IsValidValue(int value)
        {
            return value > 0 && value <= 9;
        }

        bool CanRowHaveValue(int rowIndex, int value)
        {
            if (!IsValidPositionIndex(rowIndex) || !IsValidValue(value))
            {
                return false;
            }
            else
            {
                for(int columnIndex = 0; columnIndex < 9; columnIndex++)
                {
                    if (board[rowIndex][columnIndex] == value) return false;
                }
                return true;
            }
        }
        
        bool CanColumnHaveValue(int columnIndex, int value)
        {
            if (!IsValidPositionIndex(columnIndex) || !IsValidValue(value))
            {
                return false;
            }
            else
            {
                for(int rowIndex = 0; rowIndex < 9; rowIndex++)
                {
                    if (board[rowIndex][columnIndex] == value) return false;
                }
                return true;
            }
        }

        bool CanRegionHaveValue(int rowIndex, int columnIndex, int value)
        {
            if (!IsValidPositionIndex(rowIndex) || !IsValidPositionIndex(columnIndex) || !IsValidValue(value))
            {
                return false;
            }
            else
            {
                int regionY = rowIndex / 3;
                int regionX = columnIndex / 3;
                
                int startRowIndex = regionY * 3;
                int startColumnIndex = regionX * 3;

                for (int regionRowIndex = 0; regionRowIndex < 3; regionRowIndex++)
                {
                    int rowIndex = startRowIndex + regionRowIndex;
                    for (int regionColumnIndex = 0; regionColumnIndex < 3; regionColumnIndex++)
                    {
                        int columnIndex = startColumnIndex + regionColumnIndex;
                        if (board[rowIndex][columnIndex] == value) return false;
                    }
                }
                return true;
            }
        }

        bool CanIndexHaveValue(int rowIndex, int columnIndex, int value)
        {
            return CanRowHaveValue(rowIndex, value) && CanColumnHaveValue(columnIndex, value) && CanRegionHaveValue(rowIndex, columnIndex, value);
        }

        int FindNextEmpty(int index)
        {
            int rowIndex;
            int columnIndex;
            while(index < 81)
            {
                rowIndex = index % 9;
                columnIndex = index / 9;
                if (!IsValidValue(board[rowIndex][columnIndex]))
                {
                    return index;
                }
                else
                {
                    index++;
                }
            }
            return -1;
        }

        int FindSquare(int startIndex = 0)
        {
            int solutions = 0;
            int targetIndex = FindNextEmpty(startIndex);
            if (targetIndex != -1)
            {
                int rowIndex = targetIndex % 9;
                int columnIndex = targetIndex / 9;
                for(int n = 1; n <= 9; n++)
                {
                    bool valid = TryAssignIndexToValue(rowIndex, columnIndex, n);
                    if (valid)
                    {
                        solutions += FindSquare(targetIndex+1);
                    }
                }
                board[rowIndex][columnIndex] = 0;
                return solutions;
            }
            else
            {
                Print();
                cout << endl << endl;
                return 1;
            }
        }

    public:
        void Print()
        {
            for(int rowIndex = 0; rowIndex < 9; rowIndex++)
            {
                for (int columnIndex = 0; columnIndex < 9; columnIndex++)
                {
                    int value = board[rowIndex][columnIndex];
                    
                    if (columnIndex != 0)
                    {
                        cout << " ";
                    }

                    if (IsValidValue(value))
                    {
                        cout << value;
                    }
                    else
                    {
                        cout << "_";
                    }
                }
                if (rowIndex != 8) cout << endl;
            }
        }

        void PrintSolutions()
        {
            int solutionCount = FindSquare();
            cout << "Solutions found: " << solutionCount;
        }

        bool TryAssignIndexToValue(int rowIndex, int columnIndex, int value)
        {
            if (IsValidPositionIndex(rowIndex) && IsValidPositionIndex(columnIndex) && IsValidValue(value) && CanIndexHaveValue(rowIndex, columnIndex, value))
            {
                board[rowIndex][columnIndex] = value;
                return true;
            }
            else
            {
                return false;
            }
        }
};

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cout << "No path found" << endl << "Usage: sudokuSolver <Path to sudoku text>" << endl;
        return 0;
    }
    else
    {
        ifstream file(argv[1]);

        if (!file.is_open())
        {
            cout << "No file found at path " << argv[1] << endl << "Usage: sudokuSolver <Path to sudoku text>" << endl;
            return 0;
        }
        else
        {
            SudokuBoard givenSudokuBoard;

            bool validBoard = true;
            int rowIndex = 0;
            string line;

            while(validBoard && getline(file, line))
            {
                if (line.length() != 17)
                {
                    validBoard = false;
                }
                else
                {
                    for(int i = 0; i < 17; i+=2)
                    {
                        char character = line[i];
                        
                        if (character != '_')
                        {
                            int columnIndex = i / 2;
                            if (!givenSudokuBoard.TryAssignIndexToValue(rowIndex, columnIndex, character - '0'))
                            {
                                validBoard = false;
                            }
                        }
                    }
                    rowIndex++;
                }
            }
            
            givenSudokuBoard.PrintSolutions();
        }
    }
}