#include <unistd.h>
#include <stdio.h>
int pushint(int n) {
    int *p;
    if((p = sbrk(sizeof(int))) == (void *) -1) return -1;
    *p = n;
    return 0;
}
int popint(int *n) {
    int *p;
    if(n == NULL) return -1;
    *n = ((int *) sbrk(0)) [-1];
    if((p = sbrk(- sizeof(int))) == (void *) -1) return -1;
    return 0;
}
int main() {
    int i, n;
    for(i = 0; i < 10000; ++i)
    if(pushint(i) == -1) {perror("pushint()"); return 0;}
    for(i = 9999; i >= 0; --i) {
        if(popint(&n) == -1) {perror("popint()"); return 0;}
        if(n != i) {printf("Eroare: i = %d, n = %d\n", i, n); return 0;}
    }
    return 0;
}