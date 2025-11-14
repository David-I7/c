#define _DEFAULT_SOURCE

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void h(int n) {
    printf("Incepe h(%d)...\n", n);
    sleep(5);
    printf("... se termina h(%d).\n", n);
}

int main() {
    int sem[] = {2, 2, 2, 3}, n = sizeof(sem)/sizeof(int), i;
    struct sigaction s;
    s.sa_handler = h; sigemptyset(&s.sa_mask); s.sa_flags = SA_NODEFER;
    sigaction(2, &s, NULL); sigaction(3, &s, NULL);
    if(! fork()) {
        pid_t p = getppid();
        for(i = 0; i < n; ++i) {kill(p, sem[i]); sleep(1);}
        return 0;
    }
    wait(NULL); return 0;
}