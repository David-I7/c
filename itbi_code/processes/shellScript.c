#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

char ldc[4096], *argv[1024], *p;
int argc, inword;
struct stat st;

int main(){
    while(1){
        argc =0; inword =0;
        printf(">> ");
        fgets(ldc,sizeof(ldc),stdin);
        
        for(p = ldc;*p;++p){
            if (!isspace(*p) && !inword){inword = 1; argv[argc++] = p;}
            else if(isspace(*p) && inword){inword = 0; *p='\0';}
        }
        argv[argc] = NULL;
        if (argc == 0) continue;
        if (!strcmp(argv[0],"exit")){
            exit(0);
        }

        if (stat(argv[0],&st)){
            // fisierul nu a fost gasit aici => presupun ca este in path

        }else{
            
        }
    }
}