#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Swaps two integers.
 */
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief Standard Partition function.
 * Uses the first element (arr[low]) as the pivot.
 */
int partition(int arr[], int low, int high) {
    int pivot = arr[low];
    int i = low + 1;
    int j = high;

    while (i <= j) {
        // Find an element greater than the pivot
        while (i <= high && arr[i] <= pivot) {
            i++;
        }
        
        // Find an element smaller than the pivot
        while (j > low && arr[j] > pivot) {
            j--;
        }

        // Swap if they haven't crossed
        if (i < j) {
            swap(&arr[i], &arr[j]);
        }
    }
    
    // Swap the pivot to its correct final position
    swap(&arr[low], &arr[j]);
    return j;
}

/**
 * @brief Randomized Partition function.
 * Picks a random element and swaps it with the first element before partitioning.
 */
int randomizedPartition(int arr[], int low, int high) {
    // Generate a random index between 'low' and 'high' (inclusive)
    int randomIndex = low + rand() % (high - low + 1);
    
    // Swap the randomly chosen element with the first element
    swap(&arr[low], &arr[randomIndex]);
    
    // Call the standard partition
    return partition(arr, low, high);
}

/**
 * @brief The main Randomized Quick Sort function.
 */
void randomizedQuickSort(int arr[], int low, int high) {
    if (low < high) {
        // Find the pivot index using the randomized approach
        int pi = randomizedPartition(arr, low, high);
        
        // Recursively sort elements before and after partition
        randomizedQuickSort(arr, low, pi - 1);
        randomizedQuickSort(arr, pi + 1, high);
    }
}

/**
 * @brief Utility function to print an array.
 */
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int n, choice;

    // Seed the random number generator
    srand(time(NULL));

    printf("--- Randomized Quick Sort ---\n");
    printf("Enter number of elements: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid input size.\n");
        return 1;
    }

    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    printf("1. Enter elements manually\n");
    printf("2. Generate random elements\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter %d integers:\n", n);
        for (int i = 0; i < n; i++) {
            scanf("%d", &arr[i]);
        }
    } else {
        printf("Generating %d random integers...\n", n);
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % 1000; // Random numbers between 0 and 999
        }
    }

    printf("\nUnsorted Array:\n");
    if (n <= 50) printArray(arr, n);
    else printf("[Array is too large to display entirely]\n");

    // Perform Randomized Quick Sort
    randomizedQuickSort(arr, 0, n - 1);

    printf("\nSorted Array:\n");
    if (n <= 50) printArray(arr, n);
    else printf("[Array successfully sorted. Output hidden due to size]\n");

    free(arr);
    return 0;
}
