#include "matrice_dreptunghiulara.h"
#include <stdlib.h>

int main(int argc,char** argv){
    if (argc !=4){
        fprintf(stderr,"Mod utilizare: %s <nr_linii> <nr_coloane> <dest>\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    
    int m,n;
    char* tmp;
    m = strtol(argv[1],&tmp,10);
    n = strtol(argv[2],&tmp,10);
    if (m < 0 || n < 0){
        fprintf(stderr,"m si n trebuie sa fie >= 0\n");
        exit(EXIT_FAILURE);
    }

    FILE* dest = fopen(argv[3],"w+");
    if(dest == NULL){
        perror(argv[3]);
        exit(EXIT_FAILURE);
    }

    new_matrix(dest,m,n);
    float x;
    for(int i =0;i < m; i++){
        printf("Linia %d:\n",i);
        for(int j = 0; j < n; j++){
            if (scanf("%f",&x)){
                set(dest,i,j,x);
            }else{
                fprintf(stderr,"\nInput invalid.\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(dest);
    exit(EXIT_SUCCESS);
}