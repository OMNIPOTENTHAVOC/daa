#include <stdio.h>
#include <stdlib.h>

//AVL TREE (In-order)
typedef struct AVLNode{
    int data;
    struct AVLNode *left, *right;
    int height;
}AVLNode;

int max(int a, int b){return(a>b)?a:b;}
int getHeight(AVLNode* n){return(n==NULL)?0:n->height;}

AVLNode* createNode(int data){
    AVLNode* node= (AVLNode*)malloc(sizeof(AVLNode));
    node->data = data;
    node->left = node->right = NULL;
    node->height=1;
    return node;
}

int getBalance(AVLNode* n){return(n==NULL)?0:getHeight(n->left)-getHeight(n->right);}
AVLNode* rotateRight(AVLNode* y){
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right=y; x->left=T2;
    y->height= max(getHeight(y->left), getHeight(y->right))+1;
    x->height= max(getHeight(x->left), getHeight(x->right))+1;
    return x;
}
AVLNode* rotateLeft(AVLNode* x){
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    x->right=T2; y->left=x;
    y->height= max(getHeight(y->left), getHeight(y->right))+1;
    x->height= max(getHeight(x->left), getHeight(x->right))+1;
    return y;
}
AVLNode* insertAVL(AVLNode* node, int data){
    if(node==NULL) return createNode(data);
    if(data<node->data) node->left= insertAVL(node->left, data);
    else if(data>node->data) node->right= insertAVL(node->right, data);
    else return node;
    node->height= 1+max(getHeight(node->left), getHeight(node->right));
    int balance= getBalance(node);
    if(balance>1 && data<node->left->data) return rotateRight(node);
    if(balance<-1 && data>node->right->data) return rotateLeft(node);
    if(balance>1 && data>node->left->data){
        node->left= rotateLeft(node->left);
        return rotateRight(node);
    }
    if(balance<-1 && data<node->right->data){
        node->right= rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}
void inorder(AVLNode* root){
    if(root!=NULL){
        inorder(root->left);
        printf("%d", root->data);
        inorder(root->right);
    }
}

//Heap (Max)
void swap(int* a, int* b){
    int t= *a;
    *a= *b; *b= t;
}
void heapify(int arr[], int n, int i){
    int largest= i;
    int l= 2*i+1;
    int r= 2*i+2;
    if(l<n && arr[l]>arr[largest]) largest= l;
    if(r<n && arr[r]>arr[largest]) largest= r;
    if(largest != i){
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}
void insertHeap(int arr[], int* n, int value){
    arr[*n]= value;
    int i= *n;
    (*n)++;
    while(i!=0 && arr[(i-2)/2]<arr[i]){
        swap(&arr[i], &arr[(i-1)/2]);
        i= (i-1)/2;
    }
}


//Main
int main(){
    int choice, n, val, i;
    int heapArr[100];
    int heapSize= 0;
    AVLNode* avlRoot= NULL;
    while(1){
        printf("Please select sorting method:-\n");
        printf("1. Heap Sort\n"); printf("2. AVL Tree Sort\n");
        printf("3. Exit\n\n"); printf("Enter choice: ");
        scanf("%d", &choice);
        if(choice==3) break;
        printf("Enter no. of elements: "); scanf("%d", &n);
        printf("Enter %d elements: ", n);

        if(choice==1){
            heapSize= 0;
            for(i=0; i<n; i++){
                scanf("%d", &val); insertHeap(heapArr, &heapSize, val);
            }
            printf("Sorted Heap: ");
            int originalSize= heapSize;
            for(i=originalSize-1; i>n; i--){
                swap(&heapArr[0], &heapArr[i]); heapify(heapArr, i, 0);
            }
            for(i=0; i<originalSize; i++) printf("%d", heapArr[i]);
            printf("\n");
        }
        else if(choice==2){
            avlRoot=NULL;
            for(i=0; i<n; i++){
                scanf("%d", &val);
                avlRoot= insertAVL(avlRoot, val);
            }
            printf("Sorted AVL (In-Order): ");
            inorder(avlRoot); printf("\n");
        }
    }
    return 0;
}
