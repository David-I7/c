#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>


void itostr(char* res,int target){ 
    if (target == 0){
        strcat(res,"0");
        return;
    }else if (target < 0){
        strcat(res,"-");
        target = -target;
    } 
    char c[2]="0\0";
    while(target >0){
        c[0] = target % 10 + 48;
        strcat(res,c);
        target /=10;
    }
    int r = strlen(res) - 1;
    int l = 0;
    while(r > l){
        char tmp = res[l];
        res[l] = res[r];
        res[r] = tmp;
        l++;
        r--;
    }
}

char *getGroups(char*name){
    int capacity = 512;
    char *groups = malloc(capacity);
    groups[0] = '\0';
    int groupslen = 0;
    struct group* grent;
    char res[11]= {'\0'};
    while((grent = getgrent()) != NULL){
        for(char** mem = grent->gr_mem;mem[0] != NULL;++mem){
            if (strcmp(mem[0],name) == 0 ){
                itostr(res,grent->gr_gid);
                int memlen = strlen(grent->gr_name) + strlen(res) + 3;
                if (memlen + groupslen >= capacity){
                    groups = realloc(groups,capacity *= 2);
                }
                strcat(groups,res);
                strcat(groups,"(");
                strcat(groups,grent->gr_name);
                strcat(groups,") ");
                groupslen += memlen;
                res[0]='\0'; 
            }
        }
    }
    if (groupslen == 0){
        strcat(groups,"none");
    }

    return groups;
}

int main(int argc,char ** argv){
    for (int i = 0; i < argc; i++){
        printf("arg[%d] = %s\n", i, argv[i]);
    }
    char * buf = getcwd(NULL,0);
    printf("cwd: %s\n",buf);
    free(buf);
    if (argc != 2){
        fprintf(stderr,"Mod utilizare: %s <username>\n",argv[0]);
        return -1;
    }
    
    struct passwd* pwdent;
    while((pwdent = getpwent()) != NULL){
        if (strcmp(argv[1],pwdent->pw_name) == 0){
            printf("NAME: %s\n", pwdent->pw_name);
            printf("PASSWD: %s\n", pwdent->pw_passwd);
            printf("UID: %d\n", pwdent->pw_uid);
            printf("GID: %d\n", pwdent->pw_gid);
            char *subgroups = getGroups(argv[1]);
            printf("SUBGROUPS: %s\n",subgroups);
            free(subgroups);
            printf("GECOS: %s\n", pwdent->pw_gecos);
            printf("DIR: %s\n", pwdent->pw_dir);
            printf("SHELL: %s\n", pwdent->pw_shell);
            endpwent();
            return 0;
        }
    };

    printf("Could not find user: %s\n",argv[1]);
    return 1;

}