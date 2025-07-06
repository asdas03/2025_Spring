#define MAX_SIZE 50
#include <stdio.h>
#include <stdlib.h>

/*This code is representation of sparse matrix with Linked List
each row and col are circularly linked list*/

typedef enum{head, entry} tagfield;
typedef struct matrix_node *matrix_pointer;
typedef struct entry_node{
    int row;
    int col;
    int value;
} entry_node;

typedef struct matrix_node{
    matrix_pointer down; /*column side link*/
    matrix_pointer right; /*row side link*/
    tagfield tag; /*This let us know does node is head or entry*/
    union{
        matrix_pointer next; /*For head, points next hdnode*/
        entry_node entry; /*For entry, information of entry*/
    } u;
} matrix_node;

matrix_pointer hdnode[MAX_SIZE];
matrix_pointer new_hdnode[MAX_SIZE];

matrix_pointer new_node(){
    return (matrix_pointer)malloc(sizeof(matrix_node));
}

matrix_pointer mread(){
    int num_rows, num_cols, num_terms, num_heads, i;
    int row, col, value, current_row;
    matrix_pointer temp, last, node;
    FILE *fp;
    fp = fopen("input.txt","r");
    if(fp == NULL){
        fprintf(stderr,"Can't Find input.txt!\n");
        exit(1);
    }
    fscanf(fp,"%d %d %d",&num_rows, &num_cols, &num_terms);
    num_heads = (num_cols > num_rows)? num_cols : num_rows;
    node = new_node(); node->tag=entry;
    node->u.entry.row = num_rows;
    node->u.entry.col = num_cols;
    node->u.entry.value = num_terms;
    if(!num_heads) node->right = node;
    else{ 
        for(i=0; i<num_heads; i++){
            temp = new_node();
            hdnode[i] = temp; hdnode[i]->tag = head;
            hdnode[i]->right = temp; hdnode[i]->u.next = temp;
        }
        current_row = 0;
        last = hdnode[0]; 
        for(i=0; i<num_terms; i++){
            fscanf(fp,"%d %d %d",&row, &col, &value);
            if(row > current_row){
                last->right = hdnode[current_row];
                current_row = row;
                last = hdnode[row];
            }
            temp = new_node(); temp->tag = entry;
            temp->u.entry.row = row; temp->u.entry.col = col;
            temp->u.entry.value = value;
            last->right = temp; 
            last = temp;
            hdnode[col]->u.next->down = temp; 
            hdnode[col]->u.next = temp;
        }
        last->right = hdnode[current_row]; 
        for(i=0; i<num_cols; i++) hdnode[i]->u.next->down = hdnode[i];  
        for(i=0; i<num_heads-1; i++) hdnode[i]->u.next = hdnode[i+1];  
        hdnode[num_heads-1]->u.next = node; 
        node->right = hdnode[0];  
    }
    fclose(fp);
    return node;
}

matrix_pointer mtranspose(matrix_pointer node) {
    int num_rows = node->u.entry.row;
    int num_cols = node->u.entry.col;
    int num_values = node->u.entry.value;
    int num_heads = (num_rows > num_cols) ? num_rows : num_cols;
    matrix_pointer new_node_ptr = new_node();
    new_node_ptr->tag = entry;
    new_node_ptr->u.entry.row = num_cols;
    new_node_ptr->u.entry.col = num_rows;
    new_node_ptr->u.entry.value = num_values;
    if(num_heads == 0){
        new_node_ptr->right = new_node_ptr;
        return new_node_ptr;
    }
    for(int i=0; i<num_heads; i++){
        matrix_pointer temp = new_node();
        new_hdnode[i] = temp;
        temp->tag = head;
        temp->right = temp;
        temp->u.next = temp;
    }
    matrix_pointer row_head = node->right;
    for(int i=0; i<num_rows; i++){
        matrix_pointer current = row_head->right;
        while(current != row_head){
            int r = current->u.entry.row;
            int c = current->u.entry.col;
            int v = current->u.entry.value;
            matrix_pointer temp = new_node();
            temp->tag = entry;
            temp->u.entry.row = c;
            temp->u.entry.col = r;
            temp->u.entry.value = v;
            matrix_pointer last = new_hdnode[c];
            while(last->right != new_hdnode[c]){
                last = last->right;
            }
            last->right = temp;
            temp->right = new_hdnode[c];
            new_hdnode[r]->u.next->down = temp;
            new_hdnode[r]->u.next = temp;
            current = current->right;
        }
        row_head = row_head->u.next;
    }

    for(int i=0; i<num_cols; i++){
        new_hdnode[i]->u.next->down = new_hdnode[i];
    }

    for(int i=0; i<num_heads-1; i++) {
        new_hdnode[i]->u.next = new_hdnode[i+1];
    }
    new_hdnode[num_heads-1]->u.next = new_node_ptr;
    new_node_ptr->right = new_hdnode[0];

    return new_node_ptr;
}


void mwrite(matrix_pointer node){
    int i;
    matrix_pointer temp, head = node->right;
    FILE *fp;
    fp = fopen("output.txt","w");
    fprintf(fp,"%d %d %d\n", node->u.entry.row, node->u.entry.col, node->u.entry.value);
    for(i=0; i<node->u.entry.row; i++){
        for(temp=head->right; temp != head; temp = temp->right)
            fprintf(fp,"%d %d %d\n",
                temp->u.entry.row, temp->u.entry.col, temp->u.entry.value);
        head = head->u.next;
    }
}

int main(){
    matrix_pointer mat, t_mat;
    mat = mread();
    t_mat = mtranspose(mat);
    mwrite(t_mat);
    return 0;
}