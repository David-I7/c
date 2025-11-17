#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

char ldc[4096], *argv[4096], *p;
int argc, inword, nrComenzi, indexComenzi[100];

int main(){

    while(1){
        inword = 0; argc = 0; nrComenzi = 1;
        printf(">> ");
        fgets(ldc,sizeof(ldc),stdin);
        for(p = ldc; *p; ++p){
            if(!isspace(*p) && !inword) {
                if(*p == '|'){
                    argv[argc++] = NULL;
                    indexComenzi[nrComenzi++] = argc;
                }
                else{
                    inword = 1; argv[argc++] = p;
                }
            }
            else if(isspace(*p) && inword) {inword = 0; *p = '\0';}
        }
        argv[argc] = NULL;
        if(argc == 0) continue;
        if(!strcmp(argv[0],"exit")){
            exit(0);
        }else if (nrComenzi == 1){
            if (!fork()){
                execvp(argv[0],argv);
                perror(argv[0]);
                exit(EXIT_FAILURE);
            }

            wait(NULL);
        }else{
            
            int pipefds[nrComenzi -1][2];

            for(int i = 0; i < nrComenzi -1; i++){
                pipe(pipefds[i]);
            }
            
            for(int i =0 ; i < nrComenzi; i++){
                if (!fork()){
                    if (i > 0){
                        dup2(pipefds[i-1][0], STDIN_FILENO);
                    }
                    if (i < nrComenzi -1){
                        dup2(pipefds[i][1], STDOUT_FILENO);
                    }

                    for(int j = 0; j < nrComenzi-1;j++){
                        close(pipefds[j][0]);
                        close(pipefds[j][1]);
                    }

                    execvp(argv[indexComenzi[i]],&argv[indexComenzi[i]]);
                    perror(argv[indexComenzi[i]]);
                    exit(EXIT_FAILURE);
                }
            }

            for(int i = 0; i< nrComenzi -1; ++i){
                close(pipefds[i][0]);
                close(pipefds[i][1]);
            }
            while(wait(NULL) != -1);
        }
    }
}