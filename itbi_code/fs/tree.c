#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char* gen_space(int depth){
    if (depth == 0) return NULL;
    char* space= malloc(depth * (sizeof(char) + 1 ));
    
    for(int i = 0; i < depth;i++){
        if (i % 2 == 0){
            space[i] = '|';
        }else{
            space[i] = '-';
        }
    } 
    return space;
}

int print_tree(const char* dirname,int depth){
    DIR* dir = opendir(dirname);
    if (dir == NULL){
        return -1;
    }

    chdir(dirname);
    char * space = gen_space(depth);
    struct dirent* dirent;
    struct stat st;
    while((dirent = readdir(dir)) != NULL){
       if (strcmp(dirent->d_name,".") == 0 || strcmp(dirent->d_name,"..") == 0) continue;
       lstat(dirent->d_name,&st);
       if (S_ISDIR(st.st_mode)){
        printf("%s",space);
        printf("%s\n",dirent->d_name);
        print_tree(dirent->d_name,depth + 2);
       }else{
        printf("%s",space);
        printf("%s\n",dirent->d_name);
       } 
    }

    closedir(dir);
    free(space);
    chdir("..");
    return 0;
}

int main(int argc, char** argv){
    printf(".\n");
    print_tree(".",2);
    return 0;
}