#define _DEFAULT_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

volatile int cnt[32];

const char *signal_names[32] = {
    [0] = "0",

#ifdef SIGHUP
    [SIGHUP] = "SIGHUP",
#endif
#ifdef SIGINT
    [SIGINT] = "SIGINT",
#endif
#ifdef SIGQUIT
    [SIGQUIT] = "SIGQUIT",
#endif
#ifdef SIGILL
    [SIGILL] = "SIGILL",
#endif
#ifdef SIGTRAP
    [SIGTRAP] = "SIGTRAP",
#endif
#ifdef SIGABRT
    [SIGABRT] = "SIGABRT",
#endif
#ifdef SIGBUS
    [SIGBUS] = "SIGBUS",
#endif
#ifdef SIGFPE
    [SIGFPE] = "SIGFPE",
#endif
#ifdef SIGKILL
    [SIGKILL] = "SIGKILL",
#endif
#ifdef SIGUSR1
    [SIGUSR1] = "SIGUSR1",
#endif
#ifdef SIGSEGV
    [SIGSEGV] = "SIGSEGV",
#endif
#ifdef SIGUSR2
    [SIGUSR2] = "SIGUSR2",
#endif
#ifdef SIGPIPE
    [SIGPIPE] = "SIGPIPE",
#endif
#ifdef SIGALRM
    [SIGALRM] = "SIGALRM",
#endif
#ifdef SIGTERM
    [SIGTERM] = "SIGTERM",
#endif
#ifdef SIGCHLD
    [SIGCHLD] = "SIGCHLD",
#endif
#ifdef SIGCONT
    [SIGCONT] = "SIGCONT",
#endif
#ifdef SIGSTOP
    [SIGSTOP] = "SIGSTOP",
#endif
#ifdef SIGTSTP
    [SIGTSTP] = "SIGTSTP",
#endif
#ifdef SIGTTIN
    [SIGTTIN] = "SIGTTIN",
#endif
#ifdef SIGTTOU
    [SIGTTOU] = "SIGTTOU",
#endif
#ifdef SIGURG
    [SIGURG] = "SIGURG",
#endif
#ifdef SIGXCPU
    [SIGXCPU] = "SIGXCPU",
#endif
#ifdef SIGXFSZ
    [SIGXFSZ] = "SIGXFSZ",
#endif
#ifdef SIGVTALRM
    [SIGVTALRM] = "SIGVTALRM",
#endif
#ifdef SIGPROF
    [SIGPROF] = "SIGPROF",
#endif
#ifdef SIGWINCH
    [SIGWINCH] = "SIGWINCH",
#endif
#ifdef SIGIO
    [SIGIO] = "SIGIO",
#endif
#ifdef SIGPWR
    [SIGPWR] = "SIGPWR",
#endif
#ifdef SIGSYS
    [SIGSYS] = "SIGSYS",
#endif
};

void handlerParent(int s){
    signal(s,handlerParent);
    ++cnt[s];
}

void handlerChild(int s){
    signal(s,handlerChild);
}

void sigtermhandler(int s){
    int total=0;
    for(int i =0 ; i< 32; i++){
        if (cnt[i]){
            printf("%s: %d\n",signal_names[i],cnt[i]);
            total += cnt[i];
        };
    }
    printf("Total semnale gestionate: %d\n",total);
    exit(EXIT_SUCCESS);
}

int main(){
    
    sigset_t set;
    sigemptyset(&set);
    for(int i = 0; i < NSIG;i++){
        if (i==SIGTERM || i == SIGCONT || i== SIGSTOP || i == SIGKILL) continue;
        sigaddset(&set,i);
        signal(i,handlerParent);
    }
    sigprocmask(SIG_SETMASK,&set,NULL);
    signal(SIGTERM,sigtermhandler);
    sigemptyset(&set);
    int childId;
    if (childId = fork()){
        while(1){
            sigsuspend(&set);
            kill(childId,SIGUSR1);
        }
    }else{
        signal(SIGUSR1,handlerChild);
        srand(time(NULL));
        int signalCnt = rand() % (1 << 10);
        int totalSignalsSent = 0;
        int randSignal;
        for(int i = 0; i < signalCnt; i++){
            randSignal= rand() % 32;
            if (randSignal== SIGCONT || randSignal== SIGKILL || randSignal== SIGTERM || randSignal== SIGSTOP) continue;
            kill(getppid(),randSignal);
            sigsuspend(&set);
            ++totalSignalsSent;
        }
        printf("Total semnale transmise: %d\n",totalSignalsSent);
        kill(getppid(),SIGTERM);
        exit(EXIT_SUCCESS);
    }
}