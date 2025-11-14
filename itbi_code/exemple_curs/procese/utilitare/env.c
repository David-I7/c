#include<stdio.h>
extern char **environ;
int main(){
    char **p;
    for(p = environ; *p; ++p)
    printf("%s\n", *p);
    return 0;
}
