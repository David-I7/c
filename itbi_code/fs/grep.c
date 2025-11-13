#include "../dts/Set.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

void contains_line(char** lines,int size,char*line){
    char*p;
    for(int i = 0; i < size;i++){
        if ((p =strstr(line,lines[i])) != NULL){
            char c = p[0];
            int substr_len = strlen(lines[i]);
            p[0] = '\0';
            printf("%s",line);
            p[0] = c;

            if (substr_len + strlen(p) == strlen(line)){
                printf("\033[1;31m%s\033[0m\n",p);
            }else{
                char c = p[substr_len];
                p[substr_len] = '\0';
                printf("\033[1;31m%s\033[0m",p);
                p[substr_len] = c;
                p = p + substr_len;
                printf("%s\n",p);
            }
            return;
        }
    }
}

int main(int argc,char **argv){
    if(argc != 3){
        fprintf(stderr,"Mod utilizare: %s <fisier_sursa> <fisier_dest>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE* source = fopen(argv[1],"r");
    if(source == NULL){
        perror("fopen():");
        exit(EXIT_FAILURE);
    }
    FILE* dest = fopen(argv[2],"r");  
    if(dest == NULL){
        perror("fopen():");
        exit(EXIT_FAILURE);
    }

    int buf_size = 4096;
    int read_count = 0;
    char buf[buf_size];
    struct Set* set = set_create();

    char* tmp = NULL;
    while((read_count = fread(buf,sizeof(char),buf_size,source)) != 0){
        int l=0,r = 0;
        while(r < read_count){
            while (r < read_count && (buf[r] != '\n')) ++r;
            
            if (r != l){
                if (r == read_count){
                    if (tmp != NULL) free(tmp);
                    int size = (r - l + 1);
                    tmp = malloc(sizeof(char) * size);
                    tmp[0] = '\0';
                    strncat(tmp,&buf[l],size -1);
                    tmp[size -1] = '\0'; 
                }
                else{
                    buf[r] = '\0';
                    int size = (r - l + 1);
                    if (tmp != NULL){
                        char* str = malloc(sizeof(char) * (strlen(tmp) + size));
                        str[0] = '\0';
                        strcat(str,tmp);
                        strcat(str,&buf[l]);
                        set_add(set,str);
                        free(tmp);
                        free(str);
                        tmp = NULL;
                    }else{
                        set_add(set,&buf[l]);
                    }
                }
            }
            r++;
            while (r < read_count && buf[r] == '\n') ++r;
            l = r;  
        }
    }
    fclose(source);
    if (tmp != NULL){
        set_add(set,tmp);
        free(tmp);
        tmp =NULL;
    }

    char** lines = set_to_array(set);
    while((read_count = fread(buf,sizeof(char),buf_size,dest)) != 0){
        int l=0,r=0;
        while(r < read_count){
            while (r < read_count && (buf[r] != '\n')) ++r;
            if (r == read_count){
                if (tmp != NULL) free(tmp);
                int size = (r - l + 1);
                tmp = malloc(sizeof(char) * size);
                tmp[0] = '\0';
                strncat(tmp,&buf[l],size -1);
                tmp[size -1] = '\0'; 
            }
            else{
                buf[r] = '\0';
                int size = (r - l + 1);
                if (tmp != NULL){
                    char* str = malloc(sizeof(char) * (strlen(tmp) + size));
                    str[0] = '\0';
                    strcat(str,tmp);
                    strcat(str,&buf[l]);
                    contains_line(lines,set->size,str);
                    free(tmp);
                    free(str);
                    tmp = NULL;
                }else{
                    contains_line(lines,set->size,&buf[l]);
                }
            }
            ++r;
            while (r < read_count && (buf[r] == '\n')) ++r;
            l = r;
        }
     }
    fclose(dest);
    if (tmp != NULL){
        contains_line(lines,set->size,tmp);
        free(tmp);
        tmp =NULL;
    }


    free(lines);
    set_free(set);
    exit(EXIT_SUCCESS);
}