#include <stdio.h>
#include <stdlib.h>

FILE* fp;
int drow,dcol,asize,bsize,count;
int **A,**B,**D;

int** make2darray(int rows, int cols){
    int **x,i;
    x = (int**)malloc(rows*sizeof(int*));
    for(i=0; i<rows; i++){
        x[i] = (int*)malloc(cols*sizeof(int));
    }
    return x;
}

int** readMatrix(char* fname, char is_a_or_b){
    int row,col;
    int** mat;
    fp = fopen(fname,"r");
    if(fp == NULL){
        printf("Can't Read Input!\n");
        exit(1);
    }
    fscanf(fp,"%d %d",&row,&col);
    int k = 0; int tmp;
    mat = make2darray(row*col,3);
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            fscanf(fp,"%d",&tmp);
            if(tmp != 0){
                mat[k][0] = i;
                mat[k][1] = j;
                mat[k][2] = tmp;
                k++;
            }
        }
    }
    if(is_a_or_b == 'a'){
        drow = row;
        asize = k;
    }
    else if(is_a_or_b == 'b'){
        dcol = col;
        bsize = k;
    }
    fclose(fp);
    return mat;
}
void mulMatrix(){
    D = make2darray(drow,dcol);
    for(int i=0; i<drow; i++){
        for(int j=0; j<dcol; j++){
            D[i][j] = 0;
        }
    }
    for(int i=0; i<asize; i++){
        for(int j=0; j<bsize; j++){
            if(A[i][1] == B[j][0]){
                D[A[i][0]][B[j][1]] += A[i][2]*B[j][2];
            }
        }
    }
    for(int i=0; i<drow; i++){
        for(int j=0; j<dcol; j++){
            if(D[i][j] != 0) count++;
        }
    }
}

void printMatrix(){
    fp = fopen("Output.txt","w");
    if(fp == NULL){
        printf("Can't Read output!\n");
        exit(1);
    }
    fprintf(fp,"%d %d %d\n",drow,dcol,count);
    for(int i=0; i<drow; i++){
        for(int j=0; j<dcol; j++){
            if(D[i][j] != 0) fprintf(fp,"%d %d %d\n",i,j,D[i][j]);
        }
    }
    fclose(fp);
}

int main(){
    A = readMatrix("InputA.txt",'a');
    B = readMatrix("InputB.txt",'b');
    mulMatrix();
    printMatrix();
    for(int i=0; i<asize; i++) free(A[i]);
    free(A);
    for(int i=0; i<bsize; i++) free(B[i]);
    free(B);
    for(int i=0; i<drow; i++) free(D[i]);
    free(D);
    return 0;
}