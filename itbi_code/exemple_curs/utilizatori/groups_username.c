#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <stdio.h>
int main(int argc, char *argv[]) {
    struct passwd *pu; struct group *pg; char **p;
    if(argc != 2) {
        fprintf(stderr, "Utilizare: %s username\n", argv[0]); return 1;
    }
    if((pu = getpwnam(argv[1])) == NULL) {
        perror(argv[1]); return 1;
    }
        printf("%s :", argv[1]);
    pg = getgrgid(pu -> pw_gid); printf(" %s", pg -> gr_name);
    while((pg = getgrent()) != NULL)
        for(p = pg -> gr_mem; *p; ++p){
            if(strcmp(argv[1], *p) == 0) {
            printf(" %s", pg -> gr_name);
            break;
        }
    }
    printf("\n");
    return 0;
}