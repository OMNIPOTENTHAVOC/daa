#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void bubbleSort(int arr[], int n){
	for(int i=0; i<n-1; i++){
		for(int j=0; j<n-i-1; j++){
			if(arr[j]>arr[j+1]){
				int temp = arr[j];
				arr[j]=arr[j+1];
				arr[j+1]=temp;
			}
		}
	}
}

void insertionSort(int arr[], int n){
	for(int i=1; i<n; i++){
		int key= arr[i];
		int j=i-1;
		while(j>=0 && arr[j]>key){
			arr[j+1]=arr[j];
			j--;
		}
		arr[j+1]=key;
	}
}

void printArray(int arr[], int n){
	char choice;
	printf("Display all %d elements? (y/n): ", n);
	scanf("%c", &choice);
	if(choice == 'y' || choice == 'Y'){
		for(int i=0; i<n; i++){
			printf("%d, ", arr[i]);
			if((i+1)%10==0) printf("\n");
		}
	}else{
		printf("First 10: ");
		for(int i=0; i<10; i++) printf("%d, ", arr[i]);
		printf("...Last 10: ");
		for(int i=n-10; i<n; i++) printf("%d, ", arr[i]);
		}
		printf("\n");
	}

int main(){
	int n, choice;
	printf("Enter no. of elements (min 10000): ");
	scanf("%d", &n);
	if(n<1){
		printf("Invalid size\n");
		return 1;
	}
	int *arr= (int *)malloc(n*sizeof(int));
	if(arr==NULL){
		printf("Memory Allocation failed\n");
		return 1;
	}
	printf("1. Manually enter elements\n2. Generate random\nChoice: ");
	scanf("%d", &choice);
	if(choice==1){
		printf("Enter %d elements:\n", n);
		for(int i=0; i<n; i++) scanf("%d", &arr[i]);
	}else{
		srand(time(0));
		for(int i=0; i<n; i++) arr[i]=rand()%10000;
		printf("Random elements generated.\n");
	}
	printf("Choose algorithm\n1. Bubble Sort\n2. Insertion Sort\nChoice: ");
	scanf("%d", &choice);

	printf("Sorting, please wait...");
	clock_t start=clock();
	if(choice==1) bubbleSort(arr, n);
	else if(choice==2) insertionSort(arr, n);
	else{
		printf("Invalid choice\n");
		free(arr);
		return 0;
	}
	clock_t end= clock();

	printf("Sorting Completed\n");
	printArray(arr, n);
	printf("Time taken: %f seconds\n", (double)(end-start)/CLOCKS_PER_SEC);
	free(arr);
	return 0;
}
