#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

char ldc[256], *argv[256], *p;
int argc, i, inword;
int main(){
    while(1){
        printf(">>");
        gets(ldc); // citeste pana la'\n'
        inword = 0; argc = 0;
        for(p = ldc; *p; ++p){
            if(!isspace(*p) && !inword) {inword = 1; argv[argc++] = p;}
            else if(isspace(*p) && inword) {inword = 0; *p = '\0';}
        }
        argv[argc] = NULL;
        if(argc == 0) continue;
        if(!strcmp(argv[0],"exit")){
            exit(0);
        }else if(fork()){
            wait(NULL);
        }else{
            int d, s, i, j;
            for(i = 1; i < argc - 1; ++i) {
                if(!strcmp(argv[i], ">")) {
                    d = open(argv[i+1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR); s = 1;
                } else if(!strcmp(argv[i], ">>")) {
                    d = open(argv[i+1], O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR); s = 1;
                } else if(!strcmp(argv[i], "2>")) {
                    d = open(argv[i+1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR); s = 2;
                } else if(!strcmp(argv[i], "2>>")) {
                    d = open(argv[i+1], O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR); s = 2;
                } else if(!strcmp(argv[i], "<")) {
                    d = open(argv[i+1], O_RDONLY); s = 0;
                } else continue;
                if(d == -1) {perror(argv[i]); exit(1);}
                dup2(d, s); close(d);
                // caz /bin/cat < test1 > test2                => caz /bin/cat > test2
                // argv[] = /bin/cat, <, test1, >, test2, NULL => argv[] = /bin/cat, >, test2, NULL
                for(j = i; j < argc - 1; ++j) argv[j] = argv[j+2];
                --i; argc -= 2;
            }
        execv(argv[0],argv);
        perror(argv[0]);
        return 1;
        }
    }
    return 0;
}