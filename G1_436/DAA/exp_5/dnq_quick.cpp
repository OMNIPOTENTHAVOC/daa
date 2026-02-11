#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void printArray(int arr[], int n) {

    if (n > 500) {
        printf("Array too large.\n");
        return;
    }
    for (int i = 0; i < n; i++) printf("%d, ", arr[i]);
    printf("\n");
}

int partition(int arr[], int low, int high) {
    int pivot = arr[low]; //selecting first element as pivot
    int i = low + 1;
    int j = high;

    while (i <= j) {
        //element greater than pivot
        while (i <= high && arr[i] <= pivot) i++;
        
        //element smaller than pivot
        while (j > low && arr[j] > pivot) j--;

        if (i < j) {
            swap(&arr[i], &arr[j]);
        }
    }
    //swap pivot to correct posn
    swap(&arr[low], &arr[j]);
    return j;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void merge(int arr[], int low, int mid, int high) {
    int i = low, j = mid + 1, k = 0;
    int *temp = (int*)malloc((high - low + 1) * sizeof(int));

    while (i <= mid && j <= high) {
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= high) temp[k++] = arr[j++];

    for (i = low, k = 0; i <= high; i++, k++) {
        arr[i] = temp[k];
    }
    free(temp);
}

void mergeSort(int arr[], int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;
        mergeSort(arr, low, mid);
        mergeSort(arr, mid + 1, high);
        merge(arr, low, mid, high);
    }
}

int main() {
    int n, choice;
    printf("Quick Sort Comparison\n");
    printf("Enter number of elements: ");
    scanf("%d", &n);

    //allocate memory for two arrays (one for Quick, one for Merge)
    //so we can compare them on the exact same data.
    int *arrQuick = (int*)malloc(n * sizeof(int));
    int *arrMerge = (int*)malloc(n * sizeof(int));

    printf("1. Manually enter elements\n");
    printf("2. Generate random\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter %d integers:\n", n);
        for (int i = 0; i < n; i++) {
            scanf("%d", &arrQuick[i]);
            arrMerge[i] = arrQuick[i]; //copy data
        }
    } else {
        srand(time(0));
        for (int i = 0; i < n; i++) {
            arrQuick[i] = rand() % 10000;
            arrMerge[i] = arrQuick[i]; //copy data
        }
        printf("Random data generated.\n");
    }

    printf("\nUnsorted Array: ");
    printArray(arrQuick, n);

    clock_t start = clock();
    quickSort(arrQuick, 0, n - 1);
    clock_t end = clock();
    double time_quick = ((double)(end - start)) / CLOCKS_PER_SEC;

    start = clock();
    mergeSort(arrMerge, 0, n - 1);
    end = clock();
    double time_merge = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\n\n");

    printf("Sorted Array:   ");
    printArray(arrQuick, n);

    //results
    printf("\nPerformance Comparison\n");
    printf("Input Size (n): %d\n", n);
    printf("Quick Sort Time: %f seconds\n", time_quick);
    printf("Merge Sort Time: %f seconds\n", time_merge);

    if (time_quick < time_merge) printf("Quick Sort was faster.\n");
    else printf("Merge Sort was faster.\n");

    free(arrQuick);
    free(arrMerge);
    return 0;
}
