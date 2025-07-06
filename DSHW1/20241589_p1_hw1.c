#include <stdio.h>
#include <stdlib.h>

void transpose(int **a, int **b, int rows, int cols){
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            b[j][i] = a[i][j];
        }
    }
}

int** make2darraay(int rows, int cols){
    int **x,i;
    x = (int**)malloc(rows*sizeof(int*));
    for(i=0; i<rows; i++){
        x[i] = (int*)malloc(cols*sizeof(int));
    }
    return x;
}
int main(){
    //open files
    FILE *inp, *oup;

    inp = fopen("input.txt","r");
    if(inp == NULL){
        printf("Can't Read Input File!\n");
        exit(1);
    }

    oup = fopen("output.txt","w");
    if(oup == NULL){
        printf("Can't Read Output File!\n");
        fclose(inp);
        exit(1);
    }

    //scan row col
    int row, col;
    fscanf(inp,"%d %d",&row,&col);

    //define 2d arrays
    int **mat, **tmat;
    mat = make2darraay(row,col);
    tmat = make2darraay(col,row);

    //scan matrix
    int i,j;
    for(i=0; i<row; i++){
        for(j=0; j<col; j++){
            fscanf(inp,"%d",&mat[i][j]);
        }
    }
    //transpose
    transpose(mat,tmat,row,col);

    //print to output
    fprintf(oup,"%d %d\n",col,row);
    for(i=0; i<col; i++){
        for(j=0; j<row; j++){
            fprintf(oup,"%d ",tmat[i][j]);
        }
        fprintf(oup,"\n");
    }
    fclose(inp); fclose(oup);
    for(int i=0; i<row; i++) free(mat[i]);
    free(mat);
    for(int i=0; i<col; i++) free(tmat[i]);
    free(tmat);
}