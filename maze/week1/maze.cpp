#include "maze.hpp"

void unify(int a, int b, int r, int c){
    int from = (a < b)? b : a;
    int to = (a > b)? b : a;
    for(int i=1; i<r; i+=2){
        for(int j=1; j<c; j+=2){
            if(x[i][j] == from) x[i][j] = to;
        }
    }
}

void Generate(int r, int c){
    int i,j;
    int row = r*2+1;
    int col = c*2+1;
    x = (int**)malloc(row*sizeof(int*));
    for(i=0; i<row; i++){
        x[i] = (int*)malloc(col*sizeof(int));
    }
    for(i=0; i<row; i++){
        for(j=0; j<col; j++){
            if(i%2 && j%2) x[i][j] = i/2*r + j/2;
            else if(i%2 && !(j%2)) x[i][j] = '|';
            else if(!(i%2) && j%2 ) x[i][j] = '-';
            else x[i][j] = '+';
        }
    }

    srand(static_cast<unsigned int>(time(NULL)));

    int nums = 0;
    i = 1;
    for(i=1; i<row-2; i+=2){
        for(j=2; j<col-1; j+=2){
            if(x[i][j-1] != x[i][j+1]){
                if(rand()%2){
                    x[i][j] = ' ';
                    unify(x[i][j-1],x[i][j+1],row,col);
                }
            }
        }
        for(j=1; j<col; j+=2){
            if(j == col-1 &&!nums){
                x[i+1][j] = ' ';
                unify(x[i][j],x[i+2][j],row,col);
                nums = 0;
                break;
            }
            else if(x[i][j+2] != x[i][j]){
                if(!nums){
                    x[i+1][j] = ' ';
                    unify(x[i][j],x[i+2][j],row,col);
                }
                nums = 0;
            }
            else{
                if(rand()%2){
                    x[i+1][j] = ' ';
                    unify(x[i][j],x[i+2][j],row,col);
                    nums++;
                }
            }
        }
    }
    
    for(j=1; j<col-2; j+=2){
        if(x[i][j] != x[i][j+2]){
            x[i][j+1] = ' ';
            unify(x[i][j],x[i][j+2],row,col);
        }
    }

    for(i=1; i<row; i+=2){
        for(j=1; j<col; j+=2){
            x[i][j] = ' ';
        }
    }
}



int main(){
    int n,m;
    cin >> n >> m;
    Generate(n,m);
    FILE* fp = fopen("output.maz","w");
    for(int i=0; i<n*2+1; i++){
        for(int j=0; j<m*2+1; j++){
            fprintf(fp,"%c",x[i][j]);
        }
        fprintf(fp,"\n");
    }
    for(int i=0; i<n*2+1; i++){
        free(x[i]);
    }
    free(x);
    return 0;
}