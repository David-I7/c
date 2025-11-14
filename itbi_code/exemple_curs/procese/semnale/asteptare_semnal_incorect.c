#define _DEFAULT_SOURCE

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

void h(int n) {signal(n, h);}
int main() {
    sigset_t ms; pid_t pid[10]; int i;
    signal(SIGTERM, h);
    sigemptyset(&ms); sigaddset(&ms, SIGTERM);
    sigprocmask(SIG_SETMASK, &ms, NULL);
    sigemptyset(&ms);
    for(i = 0; i < 10; ++i){
        if(pid[i] = fork()) {usleep(10); kill(pid[i], SIGTERM);}
        else {
            sigprocmask(SIG_SETMASK, &ms, NULL); //
            pause();                             // se executa neatomic   
            printf("Terminat %d\n", i);
            return 0;
        }
    }
    sleep(1);
    for(i = 0; i < 10; ++i) kill(pid[i], SIGKILL);
    return 0;
}