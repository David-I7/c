#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

void myTask(int fd){
    write(fd,"TaskCompleted",14);
}

void spawn_child(void(*task)(int)){
    int fd[2];
    pipe(fd);
    pid_t pid = fork();

    if (pid == 0){
        close(fd[0]);
        task(fd[1]);
        close(fd[1]);
        exit(0);
    }else{
        close(fd[1]);
        char msg[40];
        read(fd[0],msg,sizeof(msg));
        printf("Got: %s\n", msg);
        close(fd[0]);
    }
}

int main(){
    spawn_child(myTask);
    return 0;
}