#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
int main(){
    pid_t p; int s;
    if(!fork()) exit(1);
    if(!fork()) exit(2);
    if(!fork()) exit(3);
    while((p = wait(&s)) != -1)
    if(WIFEXITED(s))
    printf("Copil %d, cod retur %d\n", (int) p, WEXITSTATUS(s));
    return 0;
}
