#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 1000 


void merge(int arr[], int low, int mid, int high) {
    int i = low, j = mid + 1, k = 0;
    int temp[MAX_SIZE];

    while (i <= mid && j <= high) {
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= high) temp[k++] = arr[j++];

    for (i = low, k = 0; i <= high; i++, k++) {
        arr[i] = temp[k];
    }
}


void mergeSort(int arr[], int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;
        mergeSort(arr, low, mid);     // Divide Left
        mergeSort(arr, mid + 1, high); // Divide Right
        merge(arr, low, mid, high);    // Combine
    }
}


int binarySearch(int arr[], int low, int high, int key) {
    if (low <= high) {
        int mid = (low + high) / 2;
        if (arr[mid] == key) return mid;
        if (key < arr[mid]) return binarySearch(arr, low, mid - 1, key);
        return binarySearch(arr, mid + 1, high, key);
    }
    return -1;
}

int main() {
    int arr[MAX_SIZE];
    int n = 0;
    int key, result;
    FILE *fp;
    char filename[] = "numbers.csv";

    printf("--- Merge Sort & Binary Search on File Data ---\n");

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: Could not open '%s'.\n", filename);
        printf("Please run the python script 'generate_column.py' first.\n");
        return 1;
    }

    printf("Reading data from %s...\n", filename);
    while (n < MAX_SIZE && fscanf(fp, "%d", &arr[n]) == 1) {
        n++;
    }
    fclose(fp);
    
    printf("Successfully loaded %d integers.\n", n);

    mergeSort(arr, 0, n - 1);

    printf("\nSorted Data (First 10): ");
    for (int i = 0; i < (n < 10 ? n : 10); i++) printf("%d ", arr[i]);
    printf("\nSorted Data (Last 10):  ");
    for (int i = (n > 10 ? n - 10 : 0); i < n; i++) printf("%d ", arr[i]);
    printf("\n");
    printf("\nEnter element to search: ");
    scanf("%d", &key);

    result = binarySearch(arr, 0, n - 1, key);

    if (result != -1) {
        printf("Success: Element %d found at index %d.\n", key, result);
    } else {
        printf("Result: Element %d not found in the file.\n", key);
    }

    return 0;
}
