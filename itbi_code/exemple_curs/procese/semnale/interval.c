#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
void h(int n) {
    // static face ca variabila sa persiste ca un closure, fara a putea fi acesata in afara scopului curent
    static int t = 5;
    signal(n, h);
    if(t) {printf("%d\n", t--); return;}
    printf("Gata !\n"); exit(0);
}
int main() {
    struct itimerval s1 = {{1, 0}, {0, 500000}}, s2 = {{0, 0}, {0, 0}};
    char buf[256];
    signal(SIGALRM, h);
    printf("Incepe numaratoarea inversa:\n");
    setitimer(ITIMER_REAL, &s1, NULL);
    do fgets(buf, 256, stdin); while(strcmp(buf, "stop\n") != 0);
    setitimer(ITIMER_REAL, &s2, NULL);
    printf("Numaratoarea anulata.\n");
    return 0;
}