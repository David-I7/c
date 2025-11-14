#define _DEFAULT_SOURCE

#include<setjmp.h>
#include<signal.h>
#include<stdio.h>

sigjmp_buf stare;
void h(int n){siglongjmp(stare,1);}
int main(){
    int n,*p=NULL;
    signal(SIGSEGV,h);
    if(!sigsetjmp(stare, 1)) n=*p;
    else fprintf(stderr,"Eroare.\n");
    return 0;
}