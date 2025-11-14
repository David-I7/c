#include <sys/resource.h>
#include <stdlib.h>
#include <stdio.h>

int n;
void f() {
int buf[500000];
printf("Apel %d\n", ++n);
f();
}
int main(int argc, char *argv[]) {
    struct rlimit lim;
    if (getrlimit(RLIMIT_STACK, &lim) == -1) {
        perror("getrlimit"); return 1;
    }
    printf("Limitele curente hard: %ld, soft: %ld\n", (long)lim.rlim_max, (long)lim.rlim_cur);
    if(argc == 2) {
        lim.rlim_cur = atol(argv[1]);
        if (setrlimit(RLIMIT_STACK, &lim) == -1) {
            perror("setrlimit"); return 1;
        }
    }
    printf("Testare cu limita soft %ld:\n", (long)lim.rlim_cur);
    f();
    return 0;
}