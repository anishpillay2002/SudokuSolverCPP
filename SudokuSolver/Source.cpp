#include<iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility> // pair
#include <stdexcept> // runtime_error
#include <sstream> // stringstream
using namespace std;


// Defining Sudoku Rows and Column lengths as globals
#define N_rows 9
#define N_cols 9

int** read_csv(string filename) {
    int** result = 0;
    result = new int* [N_rows];

    // Create an input filestream
    ifstream inFile(filename);

    // Make sure the file is open
    if (!inFile.is_open()) throw runtime_error("Could not open file");

    // Line variable
    string line;
    // Value variable
    int val;

    // Row Index
    int rowIdx = 0;
    // Read data, line by line
    while (getline(inFile, line))
    {
        // Create a stringstream of the current line
        stringstream ss(line);
        
        // Column Index
        int colIdx = 0;
        result[rowIdx] = new int[N_cols];
        
        // Extract each integer
        while (ss.tellg() != -1){
            if (ss.peek() == ',' || ss.peek() == EOF)
            {
                val = 0;
            }
            else
            {
                ss >> val;
            }
            // Assign the value 
            result[rowIdx][colIdx] = val;
            // If the next token is a comma, ignore it and move on
            if (ss.peek() == ',') ss.ignore();

            colIdx++;
        }
        rowIdx++;
    }

    // Close file
    inFile.close();

    return result;
}




bool isUnassigned(int** InputPuzzle, int row, int col)
{
    // Checking if the cell is unassigned
    if (InputPuzzle[row][col] == 0)
        return true;
    return false;
}

bool PresentInRow(int** InputPuzzle, int col, int num)
{
    // Checking if the number is already present in the row
    for (int i = 0; i < N_cols; i++)
        if (InputPuzzle[i][col] == num)
            return true;
    return false;
}

bool PresentInCol(int** InputPuzzle, int row, int num)
{
    // Checking if the number is already present in the column
    for (int i = 0; i < N_rows; i++)
        if (InputPuzzle[row][i] == num)
            return true;
    return false;
}


void PrintPuzzle(int** InputSudokuPuzzle)
{
    // For Printing the puzzle out at any stage
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            cout << InputSudokuPuzzle[i][j] << " ";
        }
        cout << endl;
    }
}

bool PresentInBlk(int** InputPuzzle, int row,int col, int num)
{
    // Checking if the number is present in the block
    int strtRow = row - row % 3;
    int strtCol = col - col % 3;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (InputPuzzle[i + strtRow][j + strtCol] == num)
                return true;

    return false;
}

bool CanAssign(int** InputPuzzle, int row, int col, int num)
{
    // Checking if the number can be assigned to the cell
    if (PresentInRow(InputPuzzle, col, num) || PresentInCol(InputPuzzle, row, num) || PresentInBlk(InputPuzzle, row, col, num))
        return false;
    else
        return true;
}


bool FindUnassigned(int** InputPuzzle, int &row, int &col)
{
    // FInding unassigned row and column. Notice the row and column variables being passed by reference
    for ( row = 0; row < N_rows; row++)
    {
        for ( col = 0; col < N_cols; col++)
        {
            if (isUnassigned(InputPuzzle, row, col) == true)
            {
                return true;
            }
        }
    }
    return false;
}


bool SolveSudoku(int** InputPuzzle)
{
    // Recursive function to get solve the Puzzle
    int row;
    int col;
    if (!FindUnassigned(InputPuzzle, row, col))
        return true;

    for (int trynum = 1; trynum < 10; trynum++)
    {
        if (CanAssign(InputPuzzle, row, col, trynum) == true)
        {
            InputPuzzle[row][col] = trynum;
            //cout << "Row" << row << "Col" << col << endl;
            if (SolveSudoku(InputPuzzle))
                return true;
            else
                InputPuzzle[row][col] = 0;
        }
    }
    return false;
}

int main() {
    // Filename string
    string infilename;
    cout << "Please enter the input csv filename" << endl;
    // Asking user for input file. Should be a csv
    while (true)
    {
        getline(cin, infilename);
        ifstream inFile(infilename.c_str());
        if (inFile.is_open()) break;
        cout << "Invalid file. Please enter a valid input file name> " << endl;
    }
    
    // Read CSV for Sudoku input
    int** InputSudokuPuzzle = read_csv(infilename.c_str());

    cout << "Input Puzzle:" << endl;
    PrintPuzzle(InputSudokuPuzzle);

    
    if (SolveSudoku(InputSudokuPuzzle))
    {
        // If sudoku is solved, print it
        cout << "Sudoku Solution:" << endl;
        PrintPuzzle(InputSudokuPuzzle);
    }
    else
        cout << "Puzzle could not be solved" << endl;
    return 0;
}