#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
int main(){
    char *buf, *buf_nou; size_t size, size_old;
    size = 0; buf = NULL;
    do{
        size_old = size;
        size += PATH_MAX;
        if(size <= size_old) {
            fprintf(stderr, "Cale prea lunga\n");
            free(buf);
            exit(1);
        }
        if((buf_nou = realloc(buf, size)) == NULL) {
            perror("realloc");
            free(buf);
            exit(1);
        }
        buf = buf_nou;
    }while(getcwd(buf, size) == NULL);
    printf("%s\n", buf);
    free(buf);
    return 0;
}