#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

void writeOutput(int *x, int depth){
     if (depth == 1){
        printf("%d ",*x);
    }else{
        write(STDOUT_FILENO,x,sizeof(int));
    }
}

int mergeSortMultiProcess(int* arr, int n,int depth){

    if (n == 1){
        writeOutput(&arr[0],depth);
        return 0;
    }

    int pipefds[4][2];
    for(int i = 0; i < 4; ++i) pipe(pipefds[i]);

    for(int i = 0; i < 4; ++i){
        if (!fork()){
            if (i == 0){
                // COPIL 1

                close(pipefds[0][0]);close(pipefds[1][0]);close(pipefds[2][0]);
                close(pipefds[2][1]);close(pipefds[3][0]);close(pipefds[3][1]);

                int j = 0;
                while(j < n){
                    write(pipefds[0][1],&arr[j],sizeof(int));
                    j+=2;
                }
                close(pipefds[0][1]);
                j = 1;
                while(j < n){
                    write(pipefds[1][1],&arr[j],sizeof(int));
                    j+=2;
                }
                close(pipefds[1][1]);
                _exit(0);
            }else if (i == 1){
                // COPIL 2

                close(pipefds[0][0]);close(pipefds[0][1]);close(pipefds[1][0]);
                close(pipefds[1][1]);close(pipefds[2][1]);close(pipefds[3][1]);

                int l=0,r=0,readLeft=1,readRight=1, lEOF = 0, rEOF = 0;
                while(1){
                    if (readLeft){
                        lEOF = read(pipefds[2][0],&l,sizeof(int));readLeft=0;
                    }
                    if (readRight)
                        rEOF = read(pipefds[3][0],&r,sizeof(int));readRight=0;
                    
                    if (lEOF == 0){
                        writeOutput(&r,depth);
                        break;
                    }else if (rEOF == 0){
                        writeOutput(&l,depth);
                        break;
                    }else{
                        if (l < r){
                            writeOutput(&l,depth);
                            readLeft = 1;
                        }else{
                            writeOutput(&r,depth);
                            readRight = 1;
                        }
                    }
                }

                while((lEOF = read(pipefds[2][0],&l,sizeof(int))) != 0){
                    writeOutput(&l,depth);
                }
                while((rEOF = read(pipefds[3][0],&r,sizeof(int))) != 0){
                    writeOutput(&r,depth);
                }
                close(pipefds[3][0]);close(pipefds[2][0]);
                fflush(stdout);
                _exit(0);

            }else if (i==2){
                // COPIL 3

                close(pipefds[0][1]);close(pipefds[1][0]);close(pipefds[1][1]);
                close(pipefds[2][0]);close(pipefds[3][0]);close(pipefds[3][1]);
                dup2(pipefds[2][1],STDOUT_FILENO);close(pipefds[2][1]);

                int size;
                if (n % 2 == 1) size = ((n >> 1) + 1);
                else size = n >> 1;

                int arr[size];
                int i,j=0;
                while(read(pipefds[0][0],&i,sizeof(int)) > 0){
                    arr[j++]=i;
                }
                close(pipefds[0][0]);
                mergeSortMultiProcess(arr,size,depth + 1);
                _exit(0);
            }else{
                // COPIL 4

                close(pipefds[0][0]);close(pipefds[0][1]);close(pipefds[1][1]);
                close(pipefds[2][0]);close(pipefds[2][1]);close(pipefds[3][0]);
                dup2(pipefds[3][1],STDOUT_FILENO);close(pipefds[3][1]);

                int size= n >> 1;
                
                int i,j=0;
                while(read(pipefds[1][0],&i,sizeof(int)) > 0){
                    arr[j++]=i;
                }
                close(pipefds[1][0]);
                mergeSortMultiProcess(arr,size, depth +1);
                _exit(0);
            }
        }
    }

    for(int i = 0 ; i <4; ++i) {
        close(pipefds[i][0]);
        close(pipefds[i][1]);
    }
    while(wait(NULL) != -1);
}

int main(){
    int arr[1000];
    int i=0;

    while (i < (int)(sizeof(arr)/sizeof(arr[0])) && scanf("%d", &arr[i]) == 1) {
        ++i;
    }

    mergeSortMultiProcess(arr,i,1);
    if (i > 0) printf("\n");
    exit(EXIT_SUCCESS);
}