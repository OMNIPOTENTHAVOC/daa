#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- Helper Functions for Memory Management ---
int** allocateMatrix(int n) {
    int** mat = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        mat[i] = (int*)calloc(n, sizeof(int)); // calloc initializes to 0
    }
    return mat;
}

void freeMatrix(int** mat, int n) {
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
}

void printMatrix(int** mat, int n) {
    if (n > 10) {
        printf("[Matrix too large to print nicely. Showing top-left 5x5]\n");
        n = 5; 
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%6d ", mat[i][j]);
        }
        printf("\n");
    }
}

// --- Matrix Math Operations ---
void addMatrix(int** A, int** B, int** C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
}

void subMatrix(int** A, int** B, int** C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] - B[i][j];
}

// --- Conventional O(n^3) Multiplication ---
void conventionalMultiply(int** A, int** B, int** C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// --- Strassen's Algorithm Implementation ---
void strassenMultiply(int** A, int** B, int** C, int n) {
    // Base case: 1x1 matrix
    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    int k = n / 2;

    // Allocate memory for submatrices
    int **A11 = allocateMatrix(k), **A12 = allocateMatrix(k), **A21 = allocateMatrix(k), **A22 = allocateMatrix(k);
    int **B11 = allocateMatrix(k), **B12 = allocateMatrix(k), **B21 = allocateMatrix(k), **B22 = allocateMatrix(k);
    int **C11 = allocateMatrix(k), **C12 = allocateMatrix(k), **C21 = allocateMatrix(k), **C22 = allocateMatrix(k);
    int **P1 = allocateMatrix(k), **P2 = allocateMatrix(k), **P3 = allocateMatrix(k), **P4 = allocateMatrix(k);
    int **P5 = allocateMatrix(k), **P6 = allocateMatrix(k), **P7 = allocateMatrix(k);
    int **tempA = allocateMatrix(k), **tempB = allocateMatrix(k);

    // Divide matrices into 4 quadrants
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + k];
            A21[i][j] = A[i + k][j];
            A22[i][j] = A[i + k][j + k];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + k];
            B21[i][j] = B[i + k][j];
            B22[i][j] = B[i + k][j + k];
        }
    }

    // Calculate the 7 products P1 to P7
    subMatrix(B12, B22, tempB, k);
    strassenMultiply(A11, tempB, P1, k); // P1 = A11 * (B12 - B22)

    addMatrix(A11, A12, tempA, k);
    strassenMultiply(tempA, B22, P2, k); // P2 = (A11 + A12) * B22

    addMatrix(A21, A22, tempA, k);
    strassenMultiply(tempA, B11, P3, k); // P3 = (A21 + A22) * B11

    subMatrix(B21, B11, tempB, k);
    strassenMultiply(A22, tempB, P4, k); // P4 = A22 * (B21 - B11)

    addMatrix(A11, A22, tempA, k);
    addMatrix(B11, B22, tempB, k);
    strassenMultiply(tempA, tempB, P5, k); // P5 = (A11 + A22) * (B11 + B22)

    subMatrix(A12, A22, tempA, k);
    addMatrix(B21, B22, tempB, k);
    strassenMultiply(tempA, tempB, P6, k); // P6 = (A12 - A22) * (B21 + B22)

    subMatrix(A11, A21, tempA, k);
    addMatrix(B11, B12, tempB, k);
    strassenMultiply(tempA, tempB, P7, k); // P7 = (A11 - A21) * (B11 + B12)

    // Calculate C quadrants
    // C11 = P5 + P4 - P2 + P6
    addMatrix(P5, P4, tempA, k);
    subMatrix(tempA, P2, tempB, k);
    addMatrix(tempB, P6, C11, k);

    // C12 = P1 + P2
    addMatrix(P1, P2, C12, k);

    // C21 = P3 + P4
    addMatrix(P3, P4, C21, k);

    // C22 = P5 + P1 - P3 - P7
    addMatrix(P5, P1, tempA, k);
    subMatrix(tempA, P3, tempB, k);
    subMatrix(tempB, P7, C22, k);

    // Combine quadrants back into Result Matrix C
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            C[i][j] = C11[i][j];
            C[i][j + k] = C12[i][j];
            C[i + k][j] = C21[i][j];
            C[i + k][j + k] = C22[i][j];
        }
    }

    // Free all allocated memory for this recursive step to prevent leaks
    freeMatrix(A11, k); freeMatrix(A12, k); freeMatrix(A21, k); freeMatrix(A22, k);
    freeMatrix(B11, k); freeMatrix(B12, k); freeMatrix(B21, k); freeMatrix(B22, k);
    freeMatrix(C11, k); freeMatrix(C12, k); freeMatrix(C21, k); freeMatrix(C22, k);
    freeMatrix(P1, k); freeMatrix(P2, k); freeMatrix(P3, k); freeMatrix(P4, k);
    freeMatrix(P5, k); freeMatrix(P6, k); freeMatrix(P7, k);
    freeMatrix(tempA, k); freeMatrix(tempB, k);
}

// Function to find the next power of 2
int nextPowerOf2(int n) {
    int p = 1;
    while (p < n) {
        p *= 2;
    }
    return p;
}

int main() {
    int n, choice;
    printf("--- Strassen's Matrix Multiplication ---\n");
    printf("Enter the size of the square matrix (N x N): ");
    scanf("%d", &n);

    // Find the nearest power of 2 for padding
    int paddedSize = nextPowerOf2(n);

    int **A = allocateMatrix(paddedSize);
    int **B = allocateMatrix(paddedSize);
    int **C_conv = allocateMatrix(paddedSize);
    int **C_strassen = allocateMatrix(paddedSize);

    printf("1. Manually enter matrices\n");
    printf("2. Generate random matrices (Recommended for N > 5)\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter elements of Matrix A row by row:\n");
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                scanf("%d", &A[i][j]);

        printf("Enter elements of Matrix B row by row:\n");
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                scanf("%d", &B[i][j]);
    } else {
        srand(time(0));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                A[i][j] = rand() % 10; // 0 to 9
                B[i][j] = rand() % 10;
            }
        }
        printf("Matrices generated with random numbers.\n");
    }

    // Print input matrices if size is small
    if (n <= 5) {
        printf("\nMatrix A:\n"); printMatrix(A, n);
        printf("\nMatrix B:\n"); printMatrix(B, n);
    }

    // --- 1. Measure Conventional Matrix Multiplication ---
    clock_t start = clock();
    conventionalMultiply(A, B, C_conv, n); // Can compute unpadded size
    clock_t end = clock();
    double time_conv = ((double)(end - start)) / CLOCKS_PER_SEC;

    // --- 2. Measure Strassen's Matrix Multiplication ---
    start = clock();
    strassenMultiply(A, B, C_strassen, paddedSize); // Must compute padded size
    end = clock();
    double time_strassen = ((double)(end - start)) / CLOCKS_PER_SEC;

    // --- Display Results ---
    printf("\n--- Results ---\n");
    printf("Resultant Matrix (Strassen's):\n");
    printMatrix(C_strassen, n); // Print only 'n' size, ignoring the 0-padding

    printf("\n--- Performance Comparison ---\n");
    printf("Input Size (n x n): %d x %d\n", n, n);
    printf("Padded Size for Strassen: %d x %d\n", paddedSize, paddedSize);
    printf("Conventional Time: %f seconds\n", time_conv);
    printf("Strassen's Time:   %f seconds\n", time_strassen);

    // Free main memory
    freeMatrix(A, paddedSize);
    freeMatrix(B, paddedSize);
    freeMatrix(C_conv, paddedSize);
    freeMatrix(C_strassen, paddedSize);

    return 0;
}
