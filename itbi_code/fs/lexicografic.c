#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

int min(int a, int b){
    if (a < b) 
        return a;
    return b;
}

int main(int argc,char** argv){
    if(argc != 3){
        fprintf(stderr,"Mod utilizare: %s <fisier1> <fisier2>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE* f1 = fopen(argv[1],"r");
    if (f1 == NULL){
        perror("fopen()");
        exit(EXIT_FAILURE);
    }
    
    FILE* f2 = fopen(argv[2],"r");
    if (f2 == NULL){
        perror("fopen()");
        exit(EXIT_FAILURE);
    }

    int size = 4096;
    char buf1[size];
    char buf2[size];

    int read_count1,read_count2;
    while(1){
        read_count1 = fread(buf1,sizeof(char),size,f1);
        read_count2 = fread(buf2,sizeof(char),size,f2);

        if (read_count1 == read_count2 && read_count1 == 0){
            printf("%s == %s\n",argv[1],argv[2]);
            break;
        }

        int i;
        for(i; i < min(read_count1,read_count2);++i){
            if(buf1[i] == buf2[i]) continue;
            
            char c1 = tolower(buf1[i]);
            char c2 = tolower(buf2[i]);

            if (c1 != c2){
                if(c1 < c2){
                    printf("%s < %s\n",argv[1],argv[2]);
                }else{
                    printf("%s > %s\n",argv[1],argv[2]);
                }
                 break;
            }else if(buf1[i] < buf2[i]){
                printf("%s < %s\n",argv[1],argv[2]);
            }else{
                printf("%s > %s\n",argv[1],argv[2]);
            }
             break;    
        }   

        if (i < min(read_count1,read_count2)) break;;

        if (read_count1 < read_count2){
            printf("%s < %s\n",argv[1],argv[2]);
            break;
        }else if(read_count1 > read_count2){
            printf("%s > %s\n",argv[1],argv[2]);
            break;
        }
    }

    fclose(f1);
    fclose(f2);
    exit(EXIT_SUCCESS);
}