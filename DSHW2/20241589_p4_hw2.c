#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*STACK*/
char** CreateS(int max){
    char** x = (char**)malloc(max*sizeof(char*));
    for(int i=0; i<max; i++){
        x[i] = (char*)malloc(max*sizeof(char));
    }
    return x;
}

int isempty(int top){
    return top == -1;
}

int isfull(int top, int max){
    return top == max-1;
}

void push(char** s, char* item, int* top, int max){
    if(isfull(*top,max)){
        fprintf(stderr,"The stack is full!\n");
        exit(1);
    }
    strcpy(s[++(*top)],item);
}

char* pop(char**s, int* top){
    if(isempty(*top)){
        fprintf(stderr,"The stack is empty!\n");
        exit(1);
    }
    return s[(*top)--];
}

void move(char**s, char* item, int* top, int max){
    if(strcmp(item,"..") == 0){
        if(!isempty(*top)) pop(s,top);
    }
    else if(strcmp(item,".") != 0){
        if(!isfull(*top,max)) push(s,item,top,max);
    }
}

char* substr(char* s, int start, int end){
    char* t = (char*)malloc((end-start+2)*sizeof(char));
    for(int i=start; i<=end; i++){
        t[i-start] = s[i];
    }
    t[end-start+1] = '\0';
    return t;
}

void printpath(char** stk, int top){
    int i = 0;
    for(;i<=top; i++){
        printf("/%s",stk[i]);
    }
    if(top == -1){
        printf("/");
    }
    printf("\n");
}

int main(){
    int top = -1; int max = 30;
    char** stack = CreateS(max);
    char* s = (char*)malloc(max*sizeof(char));
    char* temp;
    scanf("%s",s);
    int len = strlen(s); int i=0, j=0;
    for(i=1; i<len; i++){
        if(s[i] == '/'){
            if((i-j) > 1){
                temp = substr(s,j+1,i-1);
                move(stack,temp,&top,max);
                free(temp);
            }
            j = i;
        }
    }
    i--;
    if(s[i] != '/'){
        temp = substr(s,j+1,i);
        move(stack,temp,&top,max);
        free(temp);
    }
    printpath(stack,top);
    for(i=0; i<max; i++){
        free(stack[i]);
    }
    free(stack);
    return 0;
}