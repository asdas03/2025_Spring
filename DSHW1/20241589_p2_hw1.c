#include <stdio.h>
#define MAX_TERM 100

int p1[2][100];
int p2[2][100];
int res[199] = {0};
int p1size = 0;
int p2size = 0;

void readpoly(){
    int a=1;
    int b=1;
    int i=0;
    printf("Input Polynomal:\n");
    scanf("%d",&p1size);
    for(int i=0; i<p1size; i++){
        scanf("%d %d",&a,&b);
        p1[0][i] = a; //coef
        p1[1][i] = b; //exp
    }
    printf("Input Polynomal:\n");
    scanf("%d",&p2size);
    b = 1; i = 0;
    for(int i=0; i<p2size; i++){
        scanf("%d %d",&a,&b);
        p2[0][i] = a;
        p2[1][i] = b;
    }
}

void pmult(){
    for(int i=0; i<p1size; i++){
        for(int j=0; j<p2size; j++){
            res[p1[1][i] + p2[1][j]] += p1[0][i]*p2[0][j];
        }
    }
}

void printpoly(){
    int i = 198;
    while(res[i] == 0) i--;
    printf("%dx^%d",res[i],i);
    for(int j=i-1;j>0; j--){
        if(res[j]>0) printf("+%dx^%d",res[j],j);
        else if(res[j]<0) printf("%dx^%d",res[j],j);
    }
    if(res[0]>0) printf("+%d\n",res[0]);
    else if(res[0]<0) printf("%d\n",res[0]);
}

int main(){
    readpoly();
    if(p1size == 0 || p2size ==0){
        printf("0\n");
        return 0;
    }
    pmult();
    printpoly();
    return 0;
}