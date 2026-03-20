#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Global variable to keep track of the number of solutions found
int solutionCount = 0;

/**
 * @brief Utility function to print the chessboard
 */
void printBoard(int* board, int n) {
    printf("\nSolution %d:\n", solutionCount);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // board[i] stores the column position of the queen in row i
            if (board[i] == j) {
                printf(" Q "); // Print Queen
            } else {
                printf(" . "); // Print empty space
            }
        }
        printf("\n");
    }
}

/**
 * @brief Checks if it is safe to place a queen at (row, col)
 * We only need to check the rows ABOVE the current row because 
 * we haven't placed any queens in the rows below yet.
 */
int isSafe(int* board, int row, int col) {
    for (int i = 0; i < row; i++) {
        // board[i] is the column of the queen in row i.
        // 1. Check if they are in the same column: board[i] == col
        // 2. Check if they are on the same diagonal: 
        //    The absolute difference between rows == absolute difference between cols
        if (board[i] == col || abs(board[i] - col) == abs(i - row)) {
            return 0; // Not safe
        }
    }
    return 1; // Safe
}

/**
 * @brief Recursive function to solve the N-Queens problem using Backtracking
 */
void solveNQueens(int* board, int row, int n) {
    // Base Case: If we have successfully placed queens in all 'n' rows
    if (row == n) {
        solutionCount++;
        printBoard(board, n);
        return;
    }

    // Try placing a queen in all columns one by one for the current row
    for (int col = 0; col < n; col++) {
        // Check if it's safe to place the queen at (row, col)
        if (isSafe(board, row, col)) {
            
            // PLACE: Assign the column to the current row
            board[row] = col;

            // RECURSE: Move to the next row
            solveNQueens(board, row + 1, n);

            // BACKTRACK: 
            // In a 1D array, backtracking is implicit. When the recursion returns, 
            // the 'for' loop advances to the next 'col' and overwrites board[row].
            // If we were using a 2D matrix, we would explicitly write matrix[row][col] = 0 here.
        }
    }
}

int main() {
    int n;

    printf("--- N-Queens Problem (Backtracking) ---\n");
    printf("Enter the number of Queens (N): ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid input.\n");
        return 1;
    }

    // We use a 1D array where index = row, and value = column.
    // Example: board[0] = 2 means Queen 1 is at Row 0, Col 2.
    int* board = (int*)malloc(n * sizeof(int));
    if (board == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Reset the solution counter
    solutionCount = 0;

    // Start the backtracking process from row 0
    solveNQueens(board, 0, n);

    // Display appropriate messages based on the outcome
    if (solutionCount == 0) {
        printf("\nNo solutions exist for N = %d.\n", n);
        printf("Note: N must be 1, or greater than 3.\n");
    } else {
        printf("\nTotal valid solutions found for %d Queens: %d\n", n, solutionCount);
    }

    free(board);
    return 0;
}
