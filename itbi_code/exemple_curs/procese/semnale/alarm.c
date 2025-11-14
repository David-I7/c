#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
void h(int n){printf("Timpul a expirat\n"); exit(1);}
int main(){char c;
    signal(SIGALRM,h);
    printf("Asa e ? (d/n): "); alarm(10);
    scanf("%c",&c);
    alarm(0);
    printf("Multumesc pentru raspuns !\n");
    return 0;
}
