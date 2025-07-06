#include <stdio.h>
#include <stdlib.h>
#define MAX_TERM 50
#define COMPARE(x,y) ((x<y)? -1:((x==y)? 0:1))

typedef struct poly_node *ptr;
typedef struct poly_node{
    int coef;
    int exp;
    ptr next;
} poly_node;

int c[MAX_TERM];
int e[MAX_TERM];
int asize,bsize;

void perase(ptr *x) {
    ptr start = (*x)->next;
    ptr t = start, p;
    if (!start) {
        free(*x);
        return;
    }
    do {
        p = t;
        t = t->next;
        free(p);
    } while (t != start);
    free(*x);
}


void attach(int coefficient, int exponent, ptr *x){
    ptr temp;
    temp = (ptr)malloc(sizeof(poly_node));
    temp->coef = coefficient;
    temp->exp = exponent;
    (*x)->next = temp;
    *x = temp;
}

ptr pread(int terms){
    ptr x = (ptr)malloc(sizeof(poly_node));
    ptr last = x;
    for(int i=0; i<terms; i++){
        ptr t = (ptr)malloc(sizeof(poly_node));
        t->coef = c[i];
        t->exp = e[i];
        last->next = t;
        last = last->next;
    }
    last->next = x->next;
    return x;
}

void pwrite(ptr x){
    printf("Result:");
    if(x->next == NULL){
        printf("0\n");
        return;
    }
    ptr t = x->next;
    if(t->exp == 0) printf (" %d ",t->coef);
    else printf(" %dx^%d ",t->coef,t->exp);
    t = t->next;
    while(t != x->next){
        if(t->exp == 0){
            if(t->coef > 0) printf("+ %d",t->coef);
            else printf("- %d",-1*t->coef);
        }
        else{
            if(t->coef > 0) printf("+ %dx^%d ",t->coef,t->exp);
            else printf("- %dx^%d ",-1*t->coef,t->exp);
        }
        t = t->next;
    }
    printf("\n");
}

void padd(ptr a, ptr b){
    ptr c = (ptr)malloc(sizeof(poly_node));
    ptr lastc = c;
    ptr ta = a->next;
    ptr tb = b->next;
    int i=0, j=0;
    while(i < asize && j < bsize){
        switch(COMPARE(ta->exp,tb->exp)){
            case -1:
                attach(tb->coef, tb->exp, &lastc);
                tb = tb->next;
                j++;
                break;
            case 0:
                int sum = ta->coef + tb->coef;
                if(sum) attach(sum, ta->exp, &lastc);
                i++; j++;
                ta = ta->next; tb = tb->next;
                break;
            case 1:
                attach(ta->coef, ta->exp, &lastc);
                ta = ta->next;
                i++;
                break;
        }
    }
    while(i<asize){
        attach(ta->coef,ta->exp,&lastc);
        ta = ta->next;
        i++;
    }
    while(j<bsize){
        attach(tb->coef,tb->exp,&lastc);
        tb = tb->next;
        j++;
    }
    lastc->next = c->next;
    pwrite(c);
    perase(&c);
}


void psub(ptr a, ptr b){
    ptr c = (ptr)malloc(sizeof(poly_node));
    ptr lastc = c;
    ptr ta = a->next;
    ptr tb = b->next;
    int i=0, j=0;
    while(i < asize && j < bsize){
        switch(COMPARE(ta->exp,tb->exp)){
            case -1:
                attach(-1*tb->coef, tb->exp, &lastc);
                tb = tb->next;
                j++;
                break;
            case 0:
                int sum = ta->coef - tb->coef;
                if(sum) attach(sum, ta->exp, &lastc);
                i++; j++;
                ta = ta->next; tb = tb->next;
                break;
            case 1:
                attach(ta->coef, ta->exp, &lastc);
                ta = ta->next;
                i++;
                break;
        }
    }
    while(i<asize){
        attach(ta->coef,ta->exp,&lastc);
        ta = ta->next;
        i++;
    }
    while(j<bsize){
        attach(-1*tb->coef,tb->exp,&lastc);
        tb = tb->next;
        j++;
    }
    lastc->next = c->next;
    pwrite(c);
    perase(&c);
}

void pmult(ptr a, ptr b){
    ptr c = (ptr)malloc(sizeof(poly_node));
    ptr lastc = c;
    ptr ta = a->next;
    ptr tb = b->next;
    ptr t;
    int i=0, j=0, csize = 0;
    for(int i=0; i<asize; i++){
        for(int j=0; j<bsize; j++){
            int mul_coef = ta->coef * tb->coef;
            int mul_exp = ta->exp + tb->exp;
            if(!csize){
                attach(mul_coef,mul_exp,&lastc);
                csize++;
            }
            else{
                int added = 0;
                for(t =c; t != lastc; t = t->next){
                    if(t->next->exp == mul_exp){
                        t->next->coef += mul_coef;
                        added = 1; break;
                    }
                    else if(t->next->exp < mul_exp){
                        ptr tmp = t->next;
                        attach(mul_coef,mul_exp,&t);
                        t->next = tmp;
                        added = 1; break;
                    }
                }
                if(!added){
                    attach(mul_coef,mul_exp,&lastc);
                }
            }
            tb = tb->next;
        }
        ta = ta->next;
    }
    lastc->next = c->next;
    pwrite(c);
    perase(&c);
}

int eval(ptr x, int a){
    int sum = 0;
    ptr t = x->next;
    for(int i=0; i<asize; i++){
        int tmp = 1;
        for(int i=0; i<t->exp; i++){
            tmp *= a;
        }
        sum += tmp*t->coef;
        t = t->next;
    }
    return sum;
}



int main(){
    int n,m;
    printf("Enter number of terms in polynomial A: ");
    scanf("%d", &n);
    printf("Enter terms for A (coef exp):\n");
    for(int i=0; i<n; i++){
        scanf("%d %d", &c[i], &e[i]);
    }
    ptr a = pread(n);
    printf("\nEnter number of terms in polynomial B: ");
    scanf("%d", &m);
    printf("Enter terms for B (coef exp):\n");
    for(int i=0; i<m; i++){
        scanf("%d %d", &c[i], &e[i]);
    }
    ptr b = pread(m);
    asize = n;
    bsize = m;
    int commend = 0;
    do{
        printf("\nChoose operation:\n");
        printf("1. Addition\n");
        printf("2. Subtraction\n");
        printf("3. Multiplication\n");
        printf("4. Evaluation\n");
        printf("5. Quit\n");
        printf("Enter choice: ");
        scanf("%d",&commend);
        switch(commend){
            case 1:
                padd(a,b);
                break;
            case 2:
                psub(a,b);
                break;
            case 3:
                pmult(a,b);
                break;
            case 4:
                printf("Enter x value: ");
                scanf("%d",&n);
                printf("A(%d) = %d\n",n,eval(a,n));
                printf("B(%d) = %d\n",n,eval(b,n));
                break;
        }
    } while(commend != 5);
    printf("Good Bye\n");
    return 0;
}