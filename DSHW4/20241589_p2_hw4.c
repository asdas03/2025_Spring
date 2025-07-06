#include <stdio.h>
#include <stdlib.h>

int printed = 0;

typedef struct tree* tree_pointer;
typedef struct tree{
    int data;
    tree_pointer left;
    tree_pointer right;
} tree;

tree_pointer search(tree_pointer root, int key){
    if (!root)  return NULL;
    if (key == root->data)  return root;
    if (key < root->data) return search(root->left, key);
    return search(root->right, key);
}

tree_pointer mod_search(tree_pointer root, int key){
    if(!root || root->data == key) return NULL;
    else if(key < root->data) return (root->left)? mod_search(root->left,key): root;
    else return (root->right)? mod_search(root->right,key) : root;
}

void insert(tree_pointer *node, int num)
{ 
    tree_pointer ptr, temp = mod_search(*node, num);
    if (temp || !(*node))  {
        ptr = (tree_pointer) malloc(sizeof(tree));
        ptr->data = num;
        ptr->left = ptr->right = NULL;
        if (*node)
            if (num < temp->data)
               temp->left = ptr;
            else  temp->right = ptr;
        else  *node = ptr;
    }
}

void write(tree_pointer ptr){
    if(ptr){
        printf("[%d] = %d\n",++printed,ptr->data);
        write(ptr->left);
        write(ptr->right);
    }
}

int main(){
    printf("Binary Search Tree Operations\n");
    int commend = 1,t;
    tree_pointer root = NULL,tmp;
    while(commend){
        printf("\n1. Insert 2. Search 0. Quit: ");
        scanf("%d",&commend);
        printed = 0;
        switch(commend){
            case 0:
                break;
            case 1:
                printf("Enter a number: ");
                scanf("%d",&t);
                insert(&root,t);
                write(root);
                break;
            case 2:
                printf("Search for y: ");
                scanf("%d",&t);
                tmp = search(root,t);
                if(tmp) printf("%d was FOUND in the tree.\n",t);
                else printf("%d is not in the tree.\n",t);
                break;
        }
    }
    return 0;
}