#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../dts/Vector.h"

const char DELIMITER[2] = "/";

int main(int argc,char** argv){
    if (argc != 2){
        fprintf(stderr,"Mod utilizare: %s <cale_relativa>\n",argv[0]);
        exit(EXIT_FAILURE);;
    }
    if (argv[1][0] == '/'){
        fprintf(stderr,"%s este o cale absoluta\n",argv[1]);
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[1],O_RDONLY);
    if (fd < 0){
        perror("open()");
        exit(EXIT_FAILURE);
    }
    struct Vector* fileNames = vector_create();

    struct stat st;
    fstat(fd,&st);
    close(fd);
    char*dir;
    if (S_ISDIR(st.st_mode)){
        dir = argv[1];
    }else{
        int r = strlen(argv[1]) -1;
        while(r > 0 &&  argv[1][r] !='/'){
            r--;
        }
        if(r == 0){ 
            // path-ul a fost de ex fisier.txt
            dir =".";
            char* fileName = malloc(sizeof(char) * (strlen(argv[1]) + 1));
            strcpy(fileName,argv[1]);
            vector_push(fileNames,fileName);
        }else{
            // path-ul a fost de ex a/fisier.txt
            argv[1][r] = '\0';
            char* fileName = malloc(sizeof(char) * (strlen(argv[r+1]) + 1));
            strcpy(fileName,&argv[1][r+1]);
            vector_push(fileNames,fileName);
            dir = argv[1];
        }
    }

    struct dirent* dirent;
    ino_t prevInode,curInode;
    chdir(dir);
    stat(".",&st);
    prevInode = st.st_ino;
    chdir("..");
    stat(".",&st);
    curInode = st.st_ino;

    while(prevInode != curInode){
        DIR* curDir = opendir(".");
        while((dirent =readdir(curDir)) != NULL){
            if (dirent->d_ino == prevInode){
                char* fileName = malloc(sizeof(char) * (strlen(dirent->d_name) + 1));
                strcpy(fileName,dirent->d_name);
                vector_push(fileNames,fileName);
                prevInode = curInode;
                chdir("..");
                stat(".",&st);
                curInode = st.st_ino;
                closedir(curDir);
                break;
            }
        }
    }
    
    printf("/");
    for(int i = fileNames->size -1; i >0 ;--i){
        printf("%s/",((char*)(fileNames->buf[i])));
    }
    printf("%s\n",((char*)(fileNames->buf[0])));
    vector_free(fileNames);
    exit(EXIT_SUCCESS);
}