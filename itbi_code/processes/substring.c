#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <string.h>


#define PROCESS_SPLIT_LIMIT 1024 * 1000

volatile int cnt = 0;
void sigusr1_handler(int s) { ++cnt; }

static inline int min(int a, int b) { return (a < b) ? a : b; }
static inline int max(int a, int b) { return (a > b) ? a : b; }

int main(int argc,char** argv){
    if (argc != 3){
        fprintf(stderr,"Mod de utilizare: %s <substring> <fisier>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    char* substr=argv[1],*filename=argv[2];

    struct stat st;
    if (stat(filename,&st)) {
        perror(filename);
    }

    struct sigaction sa = {0};
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    sa.sa_sigaction = sigusr1_handler;
    sigemptyset(&sa.sa_mask);
    if(sigaction(SIGRTMIN, &sa, NULL)==-1){ perror("sigaction"); exit(1); }


    long cores = sysconf(_SC_NPROCESSORS_ONLN);
    int buf_size =4096;
    char buf[buf_size];
    int process_cnt = max(min(st.st_size / PROCESS_SPLIT_LIMIT,cores),1);
    int increment_cnt = st.st_size / process_cnt;
    int offset_start =0,offset_end=increment_cnt, substr_len = strlen(substr);
    for(int i = 0 ; i < process_cnt; ++i){
        if (!fork()){
            FILE* file = fopen(filename,"r");
            if (file == NULL){
                perror(filename);
                exit(EXIT_FAILURE);
            }
            fseek(file,offset_start,SEEK_SET);

            int read_cnt = 0,j=0;
            while(offset_start < offset_end && (read_cnt = fread(buf,sizeof(char),min(buf_size, offset_end - offset_start),file)) >0){
                int i = 0;
                while(i < read_cnt){
                    while (i < read_cnt && buf[i] != substr[j]) ++i;
                    while(i < read_cnt && j < substr_len && buf[i] == substr[j]){++j;++i;}
                    if (j == substr_len){
                        union sigval v; v.sival_int = 1;
                        sigqueue(getppid(), SIGRTMIN, v); 
                        j=0;
                    }else if (i != read_cnt){
                        j = 0;
                        ++i;
                    }
                }
                offset_start += read_cnt;
            } 

            fclose(file);
            exit(EXIT_SUCCESS);
        }else{
            offset_start = offset_end - substr_len + 1;
            offset_end += increment_cnt;
        };
    }

    while(wait(NULL) != -1);
    
    printf("Total: %d\n",cnt);
    exit(EXIT_SUCCESS);
}