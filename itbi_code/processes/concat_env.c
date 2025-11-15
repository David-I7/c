#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

extern char**environ;

void env(){
    char**p = environ;
    while(*p!= NULL){
        printf("%s\n",*p);
        ++p;
    }
}

int main(){
    if (*environ == NULL){
        printf("----Inainte----\n\"\"\n\n----Dupa----\n\"\"\n");
        return 0;
    }

    printf("----Inainte----\n\n");
    env();

    char **par=&environ[2],**impar=&environ[1],*val;
    while(*impar!= NULL && *par != NULL){
        val = strchr(*par,'=') + 1;
        char* str = malloc(sizeof(char) * strlen(*impar) + sizeof(char) * strlen(val) + 1);
        strcpy(str,*impar);
        strcat(str,val);
        *impar = str;
        impar +=2;
        par +=2;
    }

    printf("\n\n----Dupa----\n");
    env();
}