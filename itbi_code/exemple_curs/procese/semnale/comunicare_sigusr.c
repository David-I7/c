#define _DEFAULT_SOURCE

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

volatile int nr;
void f(int n){signal(n,f);}
void g(int n){signal(n,g); ++nr;}
void h(int n){printf("Primit: %d\n", nr); exit(0);}

// comunicare prin semnale, parintele trimite semnal catre proces copil, asteapta sa primeasca confirmare si apoi continua,
// trimite sigurs2 pentru a anunta copilul sa se termine
int main(){
    pid_t p; sigset_t ms;
    sigemptyset(&ms); sigaddset(&ms,SIGUSR1); sigaddset(&ms,SIGUSR2);
    sigprocmask(SIG_SETMASK,&ms,NULL); sigemptyset(&ms);
    if(p=fork()){
        signal(SIGUSR1,f);
        for(nr = 0; nr < 2000; ++nr) {kill(p,SIGUSR1); sigsuspend(&ms);}
        kill(p, SIGUSR2);
        wait(NULL);
    }else{
        p=getppid(); nr = 0; signal(SIGUSR1, g); signal(SIGUSR2, h);
        while(1){sigsuspend(&ms); kill(p,SIGUSR1);}
    }
    return 0;
}