#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Utility function to find the maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}

/**
 * @brief Computes and prints the Longest Common Subsequence of two strings
 */
void findLCS(char* X, char* Y) {
    int m = strlen(X);
    int n = strlen(Y);

    // Step 1: Create the DP table
    // L[i][j] will store the length of the LCS of X[0..i-1] and Y[0..j-1]
    int** L = (int**)malloc((m + 1) * sizeof(int*));
    for(int i = 0; i<=m; i++) {
        L[i] = (int*)malloc((n + 1) * sizeof(int));
    }

    // Step 2: Build the L[m+1][n+1] table in bottom-up fashion
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0 || j == 0) {
                // Base case: If either string is empty, LCS length is 0
                L[i][j] = 0;
            } 
            else if (X[i - 1] == Y[j - 1]) {
                // If characters match, add 1 to the result of the remaining strings
                L[i][j] = L[i - 1][j - 1] + 1;
            } 
            else {
                // If characters do not match, take the maximum from excluding 
                // either the current character of X or the current character of Y
                L[i][j] = max(L[i - 1][j], L[i][j - 1]);
            }
        }
    }

    int lcs_length = L[m][n];
    printf("\nLength of the Longest Common Subsequence: %d\n", lcs_length);

    // Step 3: Trace back through the table to find the actual LCS string
    // Create an array to store the LCS string (+1 for the null terminator)
    char* lcs_str = (char*)malloc((lcs_length + 1) * sizeof(char));
    lcs_str[lcs_length] = '\0'; // Set the null terminator

    int i = m, j = n;
    int index = lcs_length - 1;

    // Start from the bottom-right corner of the table and work backwards
    while (i > 0 && j > 0) {
        // If characters in X and Y match, they are part of the LCS
        if (X[i - 1] == Y[j - 1]) {
            lcs_str[index] = X[i - 1]; // Put current character in result
            i--; 
            j--; 
            index--;     // Move diagonally up-left
        }
        // If not matching, trace back in the direction of the larger value
        else if (L[i - 1][j] > L[i][j - 1]) {
            i--;         // Move up
        } 
        else {
            j--;         // Move left
        }
    }

    // Step 4: Display the actual LCS string
    if (lcs_length > 0) {
        printf("The Longest Common Subsequence is: \"%s\"\n", lcs_str);
    } else {
        printf("There is no common subsequence.\n");
    }

    // Free dynamically allocated memory
    for (int k = 0; k <= m; k++) {
        free(L[k]);
    }
    free(L);
    free(lcs_str);
}

int main() {
    // Buffers to hold the input sequences
    char X[1000];
    char Y[1000];

    printf("--- Longest Common Subsequence (DP) ---\n");
    
    printf("Enter the first sequence (String X): ");
    if (scanf("%s", X) != 1) return 1;

    printf("Enter the second sequence (String Y): ");
    if (scanf("%s", Y) != 1) return 1;

    // Run the DP Algorithm
    findLCS(X, Y);

    return 0;
}
