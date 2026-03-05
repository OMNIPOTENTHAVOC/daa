#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Structure to represent an item
typedef struct {
    int id;
    float profit;
    float weight;
    float ratio;    // Profit-to-weight ratio (value for money)
    float fraction; // How much of this item was put in the bag (0.0 to 1.0)
} Item;

// Comparator function for qsort to sort items by ratio in descending order
int compareItems(const void *a, const void *b) {
    Item *itemA = (Item *)a;
    Item *itemB = (Item *)b;
    
    if (itemA->ratio < itemB->ratio) return 1;
    else if (itemA->ratio > itemB->ratio) return -1;
    else return 0;
}

// Function to generate random profits and weights for items
void generateRandomItems(Item items[], int n) {
    for (int i = 0; i < n; i++) {
        items[i].id = i + 1;
        // Generate a random profit between 10 and 100
        items[i].profit = (rand() % 91) + 10;
        // Generate a random weight between 5 and 50
        items[i].weight = (rand() % 46) + 5;
        
        items[i].ratio = items[i].profit / items[i].weight;
        items[i].fraction = 0.0; // Initially, nothing is in the bag
    }
}

// The Greedy Fractional Knapsack Algorithm
void fractionalKnapsack(Item items[], int n, float capacity) {
    float totalProfit = 0.0;
    float remainingCapacity = capacity;

    // Step 1: Sort the items based on their profit/weight ratio using standard qsort
    qsort(items, n, sizeof(Item), compareItems);

    // Step 2: Greedily pick items
    for (int i = 0; i < n; i++) {
        // If the bag is full, stop looking at items
        if (remainingCapacity == 0) {
            break;
        }

        // If the whole item fits, take it entirely
        if (items[i].weight <= remainingCapacity) {
            items[i].fraction = 1.0;
            totalProfit += items[i].profit;
            remainingCapacity -= items[i].weight;
        } 
        // If it doesn't fit, break it and take a fraction
        else {
            items[i].fraction = remainingCapacity / items[i].weight;
            totalProfit += items[i].profit * items[i].fraction;
            remainingCapacity = 0; // The bag is now exactly full
        }
    }

    // --- Print the final results ---
    printf("\n================ KNAPSACK SOLUTION ================\n");
    printf("Maximum Total Profit: %.2f\n\n", totalProfit);
    printf("Items Placed in the Bag:\n");
    printf("ID\tWeight\tProfit\tRatio\tFraction Taken\n");
    printf("---------------------------------------------------\n");
    
    for (int i = 0; i < n; i++) {
        // Only print the items we actually took (fraction > 0)
        if (items[i].fraction > 0) {
            printf("%d\t%.2f\t%.2f\t%.2f\t%.2f\n",
                   items[i].id, items[i].weight, items[i].profit,
                   items[i].ratio, items[i].fraction);
        }
    }
    printf("===================================================\n");
}

int main() {
    // Seed the random number generator so we get different numbers every time we run
    srand(time(NULL)); 
    
    int n, choice;
    float capacity;

    printf("Enter the number of available items: ");
    scanf("%d", &n);

    printf("Enter the maximum capacity of the knapsack: ");
    scanf("%f", &capacity);

    // Dynamically allocate memory for the items
    Item *items = (Item *)malloc(n * sizeof(Item));

    printf("\nHow would you like to input the item data?\n");
    printf("1. Enter manually\n");
    printf("2. Generate randomly\n");
    printf("Choice (1 or 2): ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("\n--- Manual Input ---\n");
        for (int i = 0; i < n; i++) {
            items[i].id = i + 1;
            printf("Enter Profit and Weight for Item %d (separated by space): ", i + 1);
            scanf("%f %f", &items[i].profit, &items[i].weight);
            items[i].ratio = items[i].profit / items[i].weight;
            items[i].fraction = 0.0;
        }
    } else {
        generateRandomItems(items, n);
        
        printf("\n--- Randomly Generated Items ---\n");
        printf("ID\tWeight\tProfit\tRatio\n");
        printf("--------------------------------------\n");
        for(int i = 0; i < n; i++) {
            printf("%d\t%.2f\t%.2f\t%.2f\n", 
                   items[i].id, items[i].weight, items[i].profit, items[i].ratio);
        }
    }

    // Run the algorithm
    fractionalKnapsack(items, n, capacity);

    // Free the allocated memory
    free(items);
    
    return 0;
}
