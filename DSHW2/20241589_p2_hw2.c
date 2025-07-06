#include <stdio.h>
#include <stdlib.h>

/*Maze Structs and Functions*/
typedef struct element{
    short int row;
    short int col;
    short int dir;
} element;

typedef struct offsets{
    short int vert;
    short int horiz;
} offsets;

typedef struct prevpos{
    int row;
    int col;
} prevpos;

offsets* MakeMove(){
    offsets* m;
    m = (offsets*)malloc(8*sizeof(offsets));
    m[0].vert = -1; m[0].horiz = 0; /*N*/
    m[1].vert = -1; m[1].horiz = 1; /*NE*/
    m[2].vert = 0; m[2].horiz = 1; /*E*/
    m[3].vert = 1; m[3].horiz = 1; /*SE*/
    m[4].vert = 1; m[4].horiz = 0; /*S*/
    m[5].vert = 1; m[5].horiz = -1; /*SW*/
    m[6].vert = 0; m[6].horiz = -1; /*W*/
    m[7].vert = -1; m[7].horiz = -1; /*NW*/
    return m;
}

int** make_mazeormark(int row, int col, int initial){
    int** x; int i,j;
    x = (int**)malloc((row)*sizeof(int*));
    for(i=0; i<row; i++){
        x[i] = (int*)malloc((col)*sizeof(int));
        for(j=0; j<col; j++){
            x[i][j] = initial;
        }
    }
    return x;
}

prevpos** MakePost(int row, int col){
    prevpos** p = (prevpos**)malloc(row*sizeof(prevpos*));
    for(int i=0; i<row; i++){
        p[i] = (prevpos*)malloc(col*sizeof(prevpos));
    }
    return p;
}

void Printpos(prevpos** b, int destrow, int destcol, int max){
    prevpos* l = (prevpos*)malloc(max*sizeof(prevpos));
    int i = 0;
    int row = 0,col = 0;
    int nextrow,nextcol;
    row = b[destrow][destcol].row;
    col = b[destrow][destcol].col;
    while((row != 1) || (col != 1)){
        l[i].row = row; l[i].col = col;
        nextrow = b[row][col].row;
        nextcol = b[row][col].col;
        row = nextrow; col = nextcol;
        i++;
    }
    printf("The path is :\n");
    printf("row col\n");
    printf("%2d%5d\n",row,col);
    for(int j=i-1; j>=0; j--){
        printf("%2d%5d\n",l[j].row,l[j].col);
    }
    printf("%2d%5d",destrow,destcol);
    free(l);
}

/*Queue ADT into Functions*/
element* Qmake(int max){
    element* q;
    q = (element*)malloc((max)*sizeof(element));
    return q;
}

int Qempty(int r, int f){
    return r == f;
}

int Qfull(int r, int max){
    return r == max;
}

void Qadd(element* Q, int* r, int max, element item){
    if(Qfull(*r,max)){
        fprintf(stderr,"The queue is already full!\n");
        exit(1);
    }
    Q[++(*r)] = item;
}

element Qdelete(element* Q, int* f, int r){
    if(Qempty(r,*f)){
        fprintf(stderr,"The queue is already empty!\n");
        exit(1);
    }
    element e = Q[++(*f)];
    return e;
}

/*Main*/
int main(){
    FILE* fp;
    int rw,cl; int max = 100;
    int front = -1, rear = -1;
    fp = fopen("input.txt","r");
    if(fp == NULL){
        fprintf(stderr,"Can't Read File!\n");
        exit(1);
    }
    fscanf(fp,"%d %d",&rw,&cl);
    int** maze = make_mazeormark(rw+2,cl+2,1);
    int** mark = make_mazeormark(rw+2,cl+2,0);
    mark[1][1] = 1;
    int Exit_R = rw; int Exit_C = cl;
    int i=0,j=0;
    for(i=1; i<=rw; i++){
        for(j=1; j<=cl; j++){
            fscanf(fp,"%d",&maze[i][j]);
        }
    }
    int row,col,dir; int found = 0;
    element pos,init,nxt; element *queue;
    init.col = 1, init.row = 1; init.dir = 0;
    queue = Qmake(max);
    Qadd(queue,&rear,max,init);
    offsets* move = MakeMove();
    prevpos** pre = MakePost(rw+2,cl+2);
    while((rear != front) && !found){
        pos = Qdelete(queue,&front,rear);
        row = pos.row; col = pos.col;
        dir = 0;
        while(dir<8 && !found){
            nxt.row = row + move[dir].vert;
            nxt.col = col + move[dir].horiz;
            nxt.dir = dir;
            if(nxt.row == Exit_R && nxt.col == Exit_C){
                pre[nxt.row][nxt.col].row = row;
                pre[nxt.row][nxt.col].col = col;
                found = 1;
            }
            else if(!maze[nxt.row][nxt.col] && !mark[nxt.row][nxt.col]){
                mark[nxt.row][nxt.col] = 1;
                pre[nxt.row][nxt.col].row = row;
                pre[nxt.row][nxt.col].col = col;
                Qadd(queue,&rear,max,nxt);
            }
            dir++;
        }
    }
    if(found){
        Printpos(pre,Exit_R,Exit_C,max);
    }
    else printf("The maze does not have a path\n");
    for(i=0; i<row; i++){
        free(maze[i]);
        free(mark[i]);
        free(pre[i]);
    }
    free(maze); free(mark); free(pre); free(queue); free(move);
    return 0;
}