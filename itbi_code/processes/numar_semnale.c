#define _DEFAULT_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

volatile int cnt = 0;

void sigusr1HandlerParinte(int s){
    signal(s,sigusr1HandlerParinte);
    ++cnt;
}
void sigusr1HandlerCopil(int s){
    signal(s,sigusr1HandlerCopil);
}

void sigchldHandler(int s){
    printf("Parinte: %d\n",cnt);
    exit(EXIT_SUCCESS);
}

int main(){
    signal(SIGCHLD,sigchldHandler);
    sigset_t set;
    sigemptyset(&set);sigaddset(&set,SIGUSR1);sigaddset(&set,SIGCHLD);sigprocmask(SIG_SETMASK,&set,NULL);
    sigemptyset(&set);
    int childId;
    if (childId = fork()){
        signal(SIGUSR1,sigusr1HandlerParinte);
        while(1){
            sigsuspend(&set);
            kill(childId,SIGUSR1);
        }
    }else{
        srand(time(NULL));
        signal(SIGUSR1,sigusr1HandlerCopil);
        int signal_cnt = rand() % (1 << 15);
        for(int i = 0; i < signal_cnt; ++i){
            kill(getppid(),SIGUSR1);
            sigsuspend(&set);
            ++cnt;
        }
        
        printf("Total semnale: %d\n",cnt);
        printf("Copil: %d\n",cnt);
        exit(EXIT_SUCCESS);
    }
}