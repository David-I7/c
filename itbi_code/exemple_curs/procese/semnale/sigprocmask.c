#define _DEFAULT_SOURCE

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>


int main() {
    sigset_t ms; int i;
    sigemptyset(&ms); sigaddset(&ms, SIGCHLD); sigaddset(&ms, SIGSEGV);
    sigprocmask(SIG_SETMASK, &ms, NULL);
    if(! fork()) {
        pid_t pp = getppid();
        kill(pp, SIGSEGV); kill(pp, SIGSEGV);
        return 0;
    }
    while(wait(NULL) != -1);
    sigpending(&ms);
    for(i = 1; i < NSIG; ++i) if(sigismember(&ms, i)) printf("%d ", i);
    printf("\n");
    sigemptyset(&ms); sigprocmask(SIG_SETMASK, &ms, NULL);
    return 0;
}