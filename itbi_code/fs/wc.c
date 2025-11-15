#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char** argv){
    if (argc < 2){
        fprintf(stderr,"Mod de utiliizare: %s [OPTIUNI...] FISIERE...\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    int options = 0;
    int fileIndex = 0;
    for(int i = 1 ; i < argc; i++){
        if (argv[i][0] == '-'){
            int len =strlen(argv[i]);
            if (len < 2){
                fprintf(stderr,"Optiune omisa pentru argumentul %d\n",i);
                exit(EXIT_FAILURE); 
            }
            for(int j = 1; j < len;j++){
                switch (argv[i][j])
                {
                case 'l':
                    options = options | 1;
                    break;
                case 'w':
                    options = options | 2;
                    break;
                case 'c':
                    options = options | 4;
                    break;
                default:
                    fprintf(stderr,"Optiune invalida: %s\n",argv[i]);
                    exit(EXIT_FAILURE); 
                    break;
                }
            }
        }else{
            fileIndex = i; 
            break;
        }
    }
    if (fileIndex == 0){
        fprintf(stderr,"Niciun fisier nu a fost specificat\n");
        exit(EXIT_FAILURE); 
    }else if(fileIndex == 1) options = 7;

    unsigned long long ct=0,wt=0,lt=0;
    int size = 4096;
    char buf[size];
    for(int  i = fileIndex;i < argc; ++i){
        unsigned long long c=0,w=0,l=0;
        FILE* file = fopen(argv[i],"r"); 
        if (file == NULL){
            perror(argv[i]);
            continue;
        }
        size_t read_count;
        while((read_count = fread(buf,sizeof(char),size,file)) > 0){
            c+=read_count;
            size_t j = 0;
            while(j < read_count){
                while (j < read_count && !isspace(buf[j])) ++j;
                w+=1;
                while (j < read_count && isspace(buf[j])) {
                    if (buf[j] == '\n') ++l;
                    ++j;
                }
            }
        }
        fclose(file);
        ct+=c;wt+=w;lt+=l;
        if (options & 1)printf("%llu ",l);
        if (options & 2)printf("%llu ",w);
        if (options & 4)printf("%llu ",c);
        printf("%s\n",argv[i]);
    }

    if (argc - fileIndex > 1){
        if (options & 1)printf("%llu ",lt);
        if (options & 2)printf("%llu ",wt);
        if (options & 4)printf("%llu ",ct);
        printf("%s\n","total");
    }

    exit(EXIT_SUCCESS);
}