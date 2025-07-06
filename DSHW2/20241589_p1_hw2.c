#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_EXPR_SIZE 100
#define MAX_STACK_SIZE 100

int isp[] = {0,19,15,12,12,13,13,13,0};
int icp[] = {20,19,15,12,12,13,13,13,0};
int stack[MAX_STACK_SIZE];
char expr[MAX_EXPR_SIZE];
char res[MAX_EXPR_SIZE];
int top = -1;
int ptop = -1;

typedef enum{
    lparen,
    rparen,
    unary,
    plus,
    minus,
    times,
    divide,
    mod,
    eos,
    operand
} precedence;

precedence pstack[MAX_STACK_SIZE];

void push(int item){
    if(top >= MAX_STACK_SIZE){
        fprintf(stderr,"The Stack is full!\n");
        exit(1);
    }
    stack[++top] = item;
}

int pop(){
    if(top <= -1){
        fprintf(stderr,"The stack is empty!\n");
        exit(1);
    }
    int t = stack[top--];
    return t;
}

precedence getToken(char *symbol, int *n){
    *symbol = expr[(*n)++];
    switch(*symbol){
        case '(': return lparen;
        case ')': return rparen;
        case '#': return unary;
        case '+': return plus;
        case '-': return minus;
        case '/': return divide;
        case '*': return times;
        case '%': return mod;
        case '\0': return eos;
        default: return operand; /*The symbol is not operator!*/
    }
}

int eval(){
    precedence token;
    char symbol;
    int op1,op2;
    top = -1;
    int n = 0; /*counter for the expression string*/
    token = getToken(&symbol,&n);
    while(token != eos){
        if(token == operand) push(symbol-'0');
        else if(token == unary){
            op1 = pop();
            push(op1*(-1));
        }
        else{
            op2 = pop();
            op1 = pop();
            switch(token){
                case plus: push(op1+op2); break;
                case minus: push(op1-op2); break;
                case times: push(op1*op2); break;
                case divide: push(op1/op2); break;
                case mod: push(op1%op2);
            }
        }
        token = getToken(&symbol,&n);
    }
    return pop();
}

void ppush(precedence item){
    if(ptop >= MAX_STACK_SIZE){
        fprintf(stderr,"The pstack is full!\n");
        exit(1);
    }
    pstack[++ptop] = item;
}

precedence ppop(){
    if(ptop <= -1){
        fprintf(stderr,"The pstack is empty!\n");
        exit(1);
    }
    precedence t = pstack[ptop--];
    return t;
}

char Tokentochar(precedence token){
    switch(token){
        case lparen: return '(';
        case rparen: return ')';
        case unary: return '#';
        case plus: return '+';
        case minus: return '-';
        case divide: return '/';
        case times: return '*';
        case mod: return '%';
        case eos: return '\0';
    }
}

void postfix(){
    char symbol;
    int n = 0; ptop = 0;
    int i = 0;
    precedence token,prev;
    /*I added prev in order to distinguish - is unary or minus*/
    pstack[0] = eos;
    for (token = getToken(&symbol, &n); token != eos; token = getToken(&symbol, &n)) {
        if (token == operand){
            res[i++] = symbol;
            prev = operand;
        }
        else if (token == rparen){
            while (pstack[ptop] != lparen) res[i++] = Tokentochar(ppop());
            ppop();
            prev = rparen;
        }
        else{
            if(token == minus && prev != operand && prev != rparen) token = unary;
            while (isp[pstack[ptop]] >= icp[token]) res[i++] = Tokentochar(ppop());
            ppush(token);
            prev = token;
        }
    }
    while ( (token = ppop()) != eos) res[i++] = Tokentochar(token);
}

int main(){
    scanf("%s",expr);
    postfix();
    printf("%s\n",res);
    strcpy(expr,res);
    printf("%d\n",eval());
    return 0;
}