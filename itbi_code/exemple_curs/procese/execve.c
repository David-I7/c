#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
extern char **environ;
// utilizare prog -e var1=hello var2=world -a executabil.exe
int main(int argc, char* argv[]) {
    int i; char **starta, **starte;
    starta = argv; starte = environ;
    for(i = 1; i < argc; ++i){
        if(!strcmp(argv[i], "-a"))
        {starta = argv + (i + 1); argv[i] = NULL;}
        else if(!strcmp(argv[i], "-e"))
        {starte = argv + (i + 1); argv[i] = NULL;}
    }
    execve(argv[1], starta, starte);
    perror("execve()");
    return EXIT_FAILURE;
}