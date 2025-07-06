#include <stdio.h>
#include <stdlib.h>
#define MAX_HEAP_SIZE 200

#define MAX_ELEMENTS 200 /*maximum heap size+1 */
#define HEAP_FULL(n) (n == MAX_ELEMENTS-1)
#define HEAP_EMPTY(n) (!n)
typedef struct {
    int key;
} element;
element heap[MAX_ELEMENTS];
int n = 0;

void push(element item, int *n){ 
    int i;
    if (HEAP_FULL(*n)) {
        fprintf(stderr, "The heap is full. \n");
        exit(EXIT_FAILURE);
    }
    i = ++(*n);
    while ((i != 1) && (item.key < heap[i/2].key)) {
        heap[i] = heap[i/2];
        i /= 2;
    }
    heap[i] = item;
}

int delete(int *n){
    int i = 1;
    while(i < *n){
        heap[i].key = (heap[i*2].key > heap[i*2+1].key)? heap[i*2+1].key : heap[i*2].key;
        i = (heap[i*2].key > heap[i*2+1].key)? i*2+1 : i*2;
    }
    (*n)--;
}

int search(int find, int n){
    for(int i=1; i<=n; i++){
        if(find == heap[i].key) return i;
    }
    return 0;
}

int change(element item, int idx, int n){
    int i = idx;
    int isbigger = (heap[i].key < item.key)? 1 : 0;
    if(isbigger){
        while((i*2 <= n) && (item.key > heap[i*2].key)){
            heap[i*2] = heap[i];
            i *= 2;
        }
        heap[i] = item;
        return 1;
    }
    else if(idx){
        while ((i != 1) && (item.key < heap[i/2].key)) {
            heap[i] = heap[i/2];
            i /= 2;
        }
        heap[i] = item;
        return 1;
    }
    else return 0;
}

void removeheap(int idx, int *n){
    for(int i=idx; i<=(*n); i++){
        heap[i].key = heap[i+1].key;
    }
    (*n)--;
}

void write(){
    for(int i=1; i<=n; i++) printf("[%d] = %d\n",i,heap[i].key);
}

int main(){
    printf("MIN Heap Operations\n");
    int y,commend = 1;
    element a;
    while(commend){
        printf("\n1. Insert 2. Delete 3. Search 4. Top Priority 5. Change Priority 6. Remove Priority 0. Quit: ");
        scanf("%d", &commend);
        switch(commend){
            case 0: break;
            case 1:
                printf("Enter a number: ");
                scanf("%d", &a.key);
                printf("n = %d\n",n);
                push(a,&n);
                write();
                break;
            case 2:
                printf("%d was deleted from the heap\n",a.key);
                a.key = delete(&n);
                write();
                break;
            case 3:
                printf("Search for y: ");
                scanf("%d",&y);
                a.key = search(y,n);
                if(a.key) printf("%d was FOUND in position %d\n",y,a.key);
                else printf("%d is not in the heap\n",y);
                break;
            case 4:
                printf("The top priority is: %d.\n",heap[1].key);
                break;
            case 5:
                printf("Change priority of: ");
                scanf("%d",&y);
                y = search(y,n);
                if(y){
                    printf("New Priority: ");
                    scanf("%d",&a.key);
                    change(a, y, n);
                    write();
                }
                else printf("There is no %d in heap.\n",y);
                break;
            case 6:
                printf("Remove Priority: ");
                scanf("%d",&y);
                int num = y;
                y = search(y,n);
                if(y){
                    removeheap(y,&n);
                    write();
                }
                else{
                    printf("%d is not in priority queue\n",num);
                    write();
                }
                break;
        }
    }
}