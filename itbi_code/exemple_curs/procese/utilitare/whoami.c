#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <stdio.h>
int main(){
    uid_t euid; struct passwd *pw;
    euid = geteuid();
    if((pw = getpwuid(euid)) == NULL){
        perror("getpwuid");
        exit(1);
    };
    printf("%s\n", pw->pw_name);
    return 0;
}
