#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void f1(void) {printf("Bye !\n");}
void f2(void) {printf("Pa !\n");}
void f3(int n, void *s) {printf("%d %s\n", n, (char *) s);}
int main() {
    long limita;
    limita = sysconf(_SC_ATEXIT_MAX);
    printf("ATEXIT_MAX = %ld\n", limita);
    if(atexit(f1)) {fprintf(stderr, "Err\n"); exit(EXIT_FAILURE);}
    if(on_exit(f3, "salutari !")){fprintf(stderr,"Err\n");exit(EXIT_FAILURE);}
    if(atexit(f2)) {fprintf(stderr, "Err\n"); exit(EXIT_FAILURE);}
    if(atexit(f1)) {fprintf(stderr, "Err\n"); exit(EXIT_FAILURE);}
    if(on_exit(f3, "felicitari !")){fprintf(stderr,"Err\n");exit(EXIT_FAILURE);}
    exit(EXIT_SUCCESS);
    return 0;
}