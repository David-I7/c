#include <stdlib.h>
#include <string.h>
#include <stdio.h>
extern char **environ;
int main(int argc, char* argv[]) {
    int i, dim, s; char *buf;
    dim = 0;
    for(i = 1; i < argc; ++i) dim += (strlen(argv[i]) + 1);
    if ((buf = malloc(dim * sizeof(char))) == NULL) {
        perror("malloc"); return 1;
    }
    strcpy(buf, "");
    for(i = 1; i < argc - 1; ++i) {strcat(buf, argv[i]); strcat(buf, " ");}
    if(i < argc) strcat(buf, argv[i]);
    s = system(buf);
    printf("Codul returnat de ’sistem()’ este: %d\n", s);
    free(buf);
    return 0;
}