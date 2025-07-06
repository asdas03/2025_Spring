#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*STACK*/
char* CreateS(int max){
    char* x = (char*)malloc(max*sizeof(char));
    return x;
}

int isempty(int top){
    return top == -1;
}

int isfull(int top, int max){
    return top == max-1;
}

void push(char* s, char item, int* top, int max){
    if(isfull(*top,max)){
        fprintf(stderr,"The stack is full!\n");
        exit(1);
    }
    s[++(*top)] = item;
}

char pop(char*s, int* top){
    if(isempty(*top)){
        fprintf(stderr,"The stack is empty!\n");
        exit(1);
    }
    return s[(*top)--];
}

int main(){
    int top = -1; int max = 20;
    char* stack = CreateS(max);
    char* s = CreateS(max);
    scanf("%s",s);
    int len = strlen(s);
    int NO = 0;
    for(int i=0; i<len && !NO; i++){
        if(s[i] == ')'){
            if(isempty(top) || stack[top] != '(') NO = 1;
            else pop(stack,&top);
        }
        else if(s[i] == ']'){
            if(isempty(top) || stack[top] != '[') NO = 1;
            else pop(stack,&top);
        }
        else if(s[i] == '}'){
            if(isempty(top) || stack[top] != '{') NO = 1;
            else pop(stack,&top);
        }
        else{
            push(stack,s[i],&top,max);
        }
    }
    if(NO || !isempty(top)){
        printf("F\n");
    }
    else printf("T");
    free(stack); free(s);
    return 0;
}