#include <stdlib.h>
#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>
#include <string.h> 
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

int set_nice(int new_nice){
    if (new_nice <= -20) new_nice = -20;
    else if (new_nice > 19) new_nice = 19;

    struct rlimit rlim;
    if (getrlimit(RLIMIT_NICE,&rlim))return -1;
    
    rlim.rlim_cur = rlim.rlim_max;
    if (setrlimit(RLIMIT_NICE,&rlim)) return -1;

    if (setpriority(PRIO_PROCESS,0,new_nice)) return -1;

    return 0;
}   

void copy_null_array(char**dest,char**src,int cnt){
    for(int i = 0; i < cnt; i++){
        dest[i] = src[i];
    }
    dest[cnt] = NULL;
}

int main(int argc, char** argv){
    int nice_val,exit_code;
    char*end;
    struct stat st;
    if (argc == 1){
        nice_val = getpriority(PRIO_PROCESS,getpid());
        printf("%d\n",nice_val);
        exit(EXIT_SUCCESS);
    }else if (argc == 2){
        if (strcmp(argv[1],"-n") == 0){
            fprintf(stderr,"Option 'n' requires an argument\n");
            exit(EXIT_FAILURE);
        }
        if (!fork()){
            execlp(argv[1],argv[1],(char *)NULL);
            perror(argv[1]);
            exit(EXIT_FAILURE);
        }else{
            waitpid(-1,&exit_code,0);
            exit(WEXITSTATUS(exit_code));
        }
    }else {
        if (strcmp(argv[1],"-n") == 0){
            end = argv[2];
            nice_val = strtol(argv[2],&end,10);
            if (end == argv[2]){
                fprintf(stderr,"Invalid integer value for '-n' option\n");
                exit(EXIT_FAILURE);
            }else if (argc == 3){
                fprintf(stderr,"You must specify a command to apply the nice value\n");
                exit(EXIT_FAILURE);
            }

            if (!fork()){
                if (set_nice(nice_val)){
                    perror("set_nice()");
                    exit(EXIT_FAILURE);
                }
                char* arr[argc - 3 + 1];
                copy_null_array(arr,&argv[3],argc-3);
                execvp(argv[3],arr);
                perror(argv[3]);
                exit(EXIT_FAILURE);
            }else{
                waitpid(-1,&exit_code,0);
                exit(WEXITSTATUS(exit_code));
            }
        }else{
            if (!fork()){
                char* arr[argc - 1 + 1];
                copy_null_array(arr,&argv[1],argc-1);
                execvp(argv[1],arr);
                perror(argv[1]);
                exit(EXIT_FAILURE);
            }else{
                waitpid(-1,&exit_code,0);
                exit(WEXITSTATUS(exit_code));
            }
        }

    }
}