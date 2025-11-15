#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <wait.h>

void print_arr(int* beg,int *end){
    --end;
    printf("[");
    while(beg != end){
        printf("%d,",*beg);
        ++beg;
    }
    printf("%d]\n",*end);
}

void gen_permutari(int *arr,int n,int k,int* visited){
    for(int i = 0;i < n; ++i){
        if (visited[i]) continue;

        arr[k] = i+1;
        visited[i] = 1;
        if (k+1 == n){  
            //print_arr(arr,arr + n);
        }else{
            gen_permutari(arr,n,k+1,visited);
        }
        visited[i] = 0;
    }
}

void write_arr(int fd,int *arr,int n){
    write(fd,"[",sizeof(char));
    char int_repr[13] = {0};
    int len=0;
    for(int i = 0 ; i < n-1 ;i++){
        sprintf(int_repr,"%d,",arr[i]);
        len = strlen(int_repr);
        write(fd,int_repr,len * sizeof(char));
        int_repr[0]=0;   
    }
    sprintf(int_repr,"%d",arr[n-1]);
    len = strlen(int_repr);
    write(fd,int_repr,len * sizeof(char)); 
    write(fd,"]\n",sizeof(char) * 2);
}   

int main(){
    int n;
    printf("Introdu valoarea n: ");
    scanf("%d",&n);
    int arr[n], visited[n];
    for (int i = 0 ; i < n ; i++){
        visited[i] = 0;
    }

    int pipefd[2];
    pipe(pipefd);
    int k = 0;
    
    struct timeval start, end;
    gettimeofday(&start, NULL);
    if (!fork()){
        close(pipefd[0]);
        for(int i = 0;i < n; ++i){
            if (visited[i]) continue;

            arr[k] = i+1;
            visited[i] = 1;
            if (k+1 == n){  
                write_arr(pipefd[1],arr,n);
            }else{
                if (n-k >8){
                    if (!fork()){
                        ++k;
                        i=-1;
                        continue;
                    }
                }else{
                    gen_permutari(arr,n,k+1,visited);
                }
            }
            visited[i] = 0;
        }
        while((wait(NULL) != -1));
        close(pipefd[1]);
        exit(EXIT_SUCCESS); 
    }else{
        close(pipefd[1]);
        char buf[256];
        int read_count;
        while((wait(NULL) != -1));

        // while((read_count = read(pipefd[0],buf,sizeof(char) * 128)) > 0){
        //     write(STDOUT_FILENO,buf,sizeof(char) * read_count);
        // }
    }

    gettimeofday(&end, NULL);

    long ms = (end.tv_sec - start.tv_sec) * 1000 +
              (end.tv_usec - start.tv_usec) / 1000;

    printf("Elapsed time: %ld ms\n", ms);
    exit(EXIT_SUCCESS);   
}