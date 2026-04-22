#include <stdio.h>
#include <limits.h>

// Maximum number of cities. 
// Note: DP with bitmasking limits N to around 20 because 2^20 becomes very large.
#define MAX 15 
#define INF 999999

int n;
int dist[MAX][MAX];
int dp[1 << MAX][MAX]; // DP table: Rows = 2^N (Masks), Cols = N (Cities)

// Utility function to find minimum of two integers
int min(int a, int b) {
    return (a < b) ? a : b;
}

/**
 * @brief Computes the shortest TSP tour cost using DP + Bitmasking.
 * @param mask A bitmask representing visited cities (1 = visited, 0 = unvisited).
 * @param pos The current city we are at.
 * @return The minimum cost to visit all remaining cities and return to start.
 */
int tsp(int mask, int pos) {
    // VISITED_ALL represents the state where all 'n' bits are 1
    int VISITED_ALL = (1 << n) - 1;

    // Base Case: If all cities are visited, return distance from current city to start (City 0)
    if (mask == VISITED_ALL) {
        return dist[pos][0];
    }

    // Memoization: If this subproblem is already calculated, return the cached answer
    if (dp[mask][pos] != -1) {
        return dp[mask][pos];
    }

    int ans = INF;

    // Try going to every other unvisited city
    for (int city = 0; city < n; city++) {
        // Check if the 'city' is NOT visited in the current mask
        if ((mask & (1 << city)) == 0) {
            
            // Mark 'city' as visited by setting its bit to 1
            int newMask = mask | (1 << city);
            
            // Cost = Distance to new city + Cost to complete the rest of the tour
            int newCost = dist[pos][city] + tsp(newMask, city);
            
            ans = min(ans, newCost);
        }
    }

    // Cache the result in the DP table and return
    return dp[mask][pos] = ans;
}

/**
 * @brief Traces back through the DP table to print the optimal path.
 */
void printPath(int mask, int pos) {
    printf("%d -> ", pos + 1); // Print 1-indexed city

    int VISITED_ALL = (1 << n) - 1;
    if (mask == VISITED_ALL) {
        printf("1\n"); // Return to start (1-indexed)
        return;
    }

    // Find the next city that resulted in the optimal DP value
    for (int city = 0; city < n; city++) {
        if ((mask & (1 << city)) == 0) {
            
            int newMask = mask | (1 << city);
            int optimalCostFromHere = dist[pos][city] + dp[newMask][city];
            
            if (dp[mask][pos] == optimalCostFromHere) {
                printPath(newMask, city);
                break;
            }
        }
    }
}

int main() {
    printf("--- Travelling Salesman Problem (DP with Bitmasking) ---\n");
    printf("Enter number of cities (Max %d): ", MAX);
    scanf("%d", &n);

    if (n <= 1 || n > MAX) {
        printf("Invalid number of cities.\n");
        return 1;
    }

    printf("Enter the Adjacency Matrix representing distances:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &dist[i][j]);
        }
    }

    // Initialize DP table with -1
    // 1 << n is 2^n. So we loop up to 2^n.
    for (int i = 0; i < (1 << n); i++) {
        for (int j = 0; j < n; j++) {
            dp[i][j] = -1;
        }
    }

    // Start TSP from City 0, with only City 0 visited (mask = 1)
    // Mask 1 in binary is ...0001, meaning the 0th bit is 1.
    int min_cost = tsp(1, 0);

    printf("\nMinimum Cost for TSP Tour: %d\n", min_cost);
    
    printf("Optimal Path: ");
    printPath(1, 0);

    return 0;
}
