#include <iostream>

bool IsRowSafe(int grid[9][9], int row, int num);
bool IsColSafe(int grid[9][9], int col, int num);
bool IsBoxSafe(int grid[9][9], int boxRow, int boxCol, int num);
bool IsSafe(int grid[9][9], int row, int col, int num);
bool FindBlankSpace(int grid[9][9], int& row, int& col);
void DrawGrid(int grid[9][9]);
void AssignNumber(int grid[9][9], int row, int col);
bool Solve(int grid[9][9]);



bool IsSafe(int grid[9][9], int row, int col, int num) {
    if (!IsRowSafe(grid, row, num) 
        || !IsColSafe(grid, col, num)
        || !IsBoxSafe(grid, row, col, num)) {
            return false;
        }
    return true;

}

bool IsRowSafe(int grid[9][9], int row, int num) {
    for (int i = 0; i < 9; i++) {
        if (num == grid[row][i]) {
            return false;
        }
    }
    return true;
}

bool IsColSafe(int grid[9][9], int col, int num) {
    for (int i = 0; i < 9; i++) {
        if (num == grid[i][col]) {
            return false;
        }
    }
    return true;
}

bool IsBoxSafe(int grid[9][9], int row, int col, int num) {
    int boxRow, boxCol;

    boxRow = (row / 3) * 3;
    boxCol = (col / 3) * 3;

    for (int i = boxRow; i < (boxRow+3); i++) {
        for (int j = boxCol; j < (boxCol+3); j++) {
            if (num == grid[i][j]) {
                return false;
            }
        }
    }
    return true;

}

void DrawGrid(int grid[9][9]) {
    int boxRow = 0;
    int boxCol = 0;

    for (int i = 0; i < 9; i++) {
        std::cout << "|";
        for (int j = 0; j < 9; j++) {
            std::cout << grid[i][j];
            if ((j+1)%3 == 0) {
                std::cout << "|";
            }
        }
        std::cout << "\n";
        if ((i+1)%3 == 0) {
            std::cout << "--------------\n";
        }   
    }
}

bool FindBlankSpace(int grid[9][9], int& row, int& col) {
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            if (grid[row][col] == 0) {
                return true;
            }
        }
    }
    return false;
}

bool Solve(int grid[9][9]) {
    int row, col;
    int num;

    if (!FindBlankSpace(grid, row, col)) {
        std::cout << "The Sudoku has been solved!!!\n";
        std::cout << "The solution is: \n";
        DrawGrid(grid);
        return true;
    }

    for (num = 1; num <= 9; num++) {
        if (IsSafe(grid, row, col, num)) {
            grid[row][col] = num;

            if (Solve(grid)) {
                return true;
            }
            grid[row][col] = 0;
        }
    }
    return false;
}

int main() {
    int grid[9][9] = {{0, 0, 0, 0, 0, 8, 0, 9, 1},
                      {0, 0, 0, 1, 0, 0, 0, 0, 0},
                      {7, 0, 0, 9, 0, 0, 0, 6, 0},
                      {0, 6, 0, 0, 0, 0, 7, 0, 0},
                      {2, 0, 0, 0, 0, 0, 0, 0, 8},
                      {0, 0, 0, 0, 3, 2, 4, 0, 0},
                      {0, 0, 8, 0, 5, 0, 0, 0, 0},
                      {6, 1, 9, 0, 0, 0, 0, 0, 0},
                      {0, 0, 7, 4, 8, 0, 0, 0, 0}};  
    DrawGrid(grid);

    if (Solve(grid) == true) {
        std::cout << "The program is done!";
    }
    else {
        std::cout << "No solution found.";
    }
    return 0;
}