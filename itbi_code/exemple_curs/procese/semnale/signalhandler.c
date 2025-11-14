#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
void h(int n) {
    signal(n, h);
    printf("Incepe h(%d)...\n", n);
    sleep(5);
    printf("... se termina h(%d).\n", n);
}
int main() {
    int sem[] = {2, 2, 2, 3}, n = sizeof(sem)/sizeof(int), i;
    signal(2, h); signal(3, h);
    if(! fork()) {
        pid_t p = getppid();
        for(i = 0; i < n; ++i) {kill(p, sem[i]); sleep(1);}
        return 0;
    }
    wait(NULL);
    return 0;
}