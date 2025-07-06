#include <stdio.h>
#include <string.h>

void pmatch_all(char *string, char *pat){
/* Knuth, Morris, Pratt string matching algorithm */
    int i = 0, j = 0;
    int lens = strlen(string);
    int lenp = strlen(pat);
    while (i < lens) {
        if (string[i] == pat[j]) {
            i++; j++;
        }
        else if (j == 0) i++;
        else j = 0;

        if(j == lenp){
            printf("%d\n",i-lenp);
            j--;
        }
    }
}

int main(){
    char a[30],b[30];
    scanf("%s %s",a,b);
    pmatch_all(a,b);
    return 0;
}