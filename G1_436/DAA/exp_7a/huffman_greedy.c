#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 100

// --- Data Structures ---

// A Huffman tree node
struct MinHeapNode {
    char data;                 // One of the input characters
    unsigned freq;             // Frequency of the character
    struct MinHeapNode *left, *right; // Left and right child
};

// A Min Heap: Collection of min-heap (or Huffman tree) nodes
struct MinHeap {
    unsigned size;             // Current size of min heap
    unsigned capacity;         // capacity of min heap
    struct MinHeapNode** array; // Array of minheap node pointers
};

// --- Utility Functions for Node and Heap ---

// A utility function allocate a new min heap node with given character
// and frequency of the character
struct MinHeapNode* newNode(char data, unsigned freq) {
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

// A utility function to create a min heap of given capacity
struct MinHeap* createMinHeap(unsigned capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// A utility function to swap two min heap nodes
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// The standard minHeapify function.
void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// A utility function to check if size of heap is 1 or not
int isSizeOne(struct MinHeap* minHeap) {
    return (minHeap->size == 1);
}

// A standard function to extract minimum value node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

// A utility function to insert a new node to Min Heap
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

// A standard function to build min heap
void buildMinHeap(struct MinHeap* minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

// A utility function to check if this node is leaf
int isLeaf(struct MinHeapNode* root) {
    return !(root->left) && !(root->right);
}

// Creates a min heap of capacity equal to size and inserts all character of data[] in min heap
struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size) {
    struct MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);
    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

// --- Huffman Tree Construction ---

// The main function that builds Huffman tree
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) {
    struct MinHeapNode *left, *right, *top;

    // Step 1: Create a min heap of capacity equal to size.
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    // Iterate while size of heap doesn't become 1
    while (!isSizeOne(minHeap)) {
        // Step 2: Extract the two minimum freq items from min heap
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        // Step 3: Create a new internal node with frequency equal to the
        // sum of the two nodes frequencies. Make the two extracted node as
        // left and right children. Add this node to the min heap
        // '$' is a special value for internal nodes, not used
        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    // Step 4: The remaining node is the root node and the tree is complete.
    return extractMin(minHeap);
}

// --- Display & Calculation Functions ---

// Prints huffman codes from the root of Huffman Tree.
// It uses arr[] to store codes
void printCodes(struct MinHeapNode* root, int arr[], int top, int* total_bits) {
    // Assign 0 to left edge and recur
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1, total_bits);
    }

    // Assign 1 to right edge and recur
    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1, total_bits);
    }

    // If this is a leaf node, then it contains one of the input
    // characters, print the character and its code from arr[]
    if (isLeaf(root)) {
        // Handle special characters for better display formatting
        if (root->data == ' ') {
            printf(" ' ' (sp) | %-9d | ", root->freq);
        } else if (root->data == '\n') {
            printf(" '\\n'(nl) | %-9d | ", root->freq);
        } else if (root->data == '\t') {
            printf(" '\\t'(tb) | %-9d | ", root->freq);
        } else {
            printf(" '%c'      | %-9d | ", root->data, root->freq);
        }
        
        for (int i = 0; i < top; ++i) {
            printf("%d", arr[i]);
        }
        printf("\n");
        
        // Calculate bits for this character (Frequency * Code Length)
        *total_bits += (root->freq * top);
    }
}

// The main function that builds a Huffman Tree and print codes by traversing the built Huffman Tree
void HuffmanCodes(char data[], int freq[], int size) {
    // Construct Huffman Tree
    struct MinHeapNode* root = buildHuffmanTree(data, freq, size);

    // Print Huffman codes using the Huffman tree built above
    int arr[MAX_TREE_HT], top = 0;
    int total_compressed_bits = 0;
    int total_original_bits = 0;

    printf("\n--- Huffman Dictionary ---\n");
    printf(" Char  | Frequency | Huffman Code\n");
    printf("-----------------------------------\n");
    printCodes(root, arr, top, &total_compressed_bits);
    printf("-----------------------------------\n");

    // Calculate Original Bits (assuming 8-bits per character in standard ASCII)
    for (int i = 0; i < size; i++) {
        total_original_bits += (freq[i] * 8);
    }

    printf("\n--- Compression Analysis ---\n");
    printf("Total bits using standard 8-bit ASCII: %d bits\n", total_original_bits);
    printf("Total bits using Huffman Encoding:     %d bits\n", total_compressed_bits);
    
    float savings = (float)(total_original_bits - total_compressed_bits) / total_original_bits * 100;
    printf("Space Saved: %.2f%%\n", savings);
}

// --- Main Driver ---

int main() {
    char text[4096];
    int freq_map[256] = {0}; // To store frequencies of all ASCII characters
    int unique_count = 0;

    printf("--- Huffman Coding Algorithm (Greedy) ---\n");
    printf("Enter a paragraph or string to encode:\n");
    
    if (fgets(text, sizeof(text), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }

    // Remove trailing newline character added by fgets
    text[strcspn(text, "\n")] = '\0';

    int len = strlen(text);
    if (len == 0) {
        printf("Empty input string. Exiting...\n");
        return 0;
    }

    // 1. Calculate frequencies of each character in the input string
    for (int i = 0; i < len; i++) {
        freq_map[(unsigned char)text[i]]++;
    }

    // 2. Count how many unique characters exist
    for (int i = 0; i < 256; i++) {
        if (freq_map[i] > 0) {
            unique_count++;
        }
    }

    // 3. Populate the arrays for the Huffman algorithm
    char* arr = (char*)malloc(unique_count * sizeof(char));
    int* freq = (int*)malloc(unique_count * sizeof(int));

    int index = 0;
    for (int i = 0; i < 256; i++) {
        if (freq_map[i] > 0) {
            arr[index] = (char)i;
            freq[index] = freq_map[i];
            index++;
        }
    }

    printf("\nInput string length: %d characters\n", len);
    printf("Unique characters found: %d\n", unique_count);

    // 4. Generate Huffman Codes
    HuffmanCodes(arr, freq, unique_count);

    free(arr);
    free(freq);
    return 0;
}
