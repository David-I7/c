#include "matrice_dreptunghiulara.h"
#include <stdlib.h>

int main(int argc,char** argv){
    if (argc !=4){
        fprintf(stderr,"Mod utilizare: %s <f1> <f2> <dest>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE* f1 = fopen(argv[1],"r");
    if (f1 == NULL){
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }
    
    FILE* f2 = fopen(argv[2],"r");
    if (f2 == NULL){
        perror(argv[2]);
        exit(EXIT_FAILURE);
    }

    struct tuple* matrix_size = get_size(f1);
    int m = matrix_size->m,n = matrix_size->n;
    matrix_size = get_size(f2);
    if (m != matrix_size->m || n != matrix_size->n){
        fprintf(stderr,"Matricile au dimensiuni diferite!\n");
        exit(EXIT_FAILURE);
    }

    FILE* dest = fopen(argv[3],"w+");
    if (dest == NULL){
        perror(argv[3]);
        exit(EXIT_FAILURE);
    }

    new_matrix(dest,m,n);
    for(int i = 0 ; i < m; i++){
        for(int j = 0; j < n; j++){
            set(dest,i,j,get(f1,i,j) + get(f2,i,j));
        }
    }
    
    fclose(f1);
    fclose(f2);
    fclose(dest);
    exit(EXIT_SUCCESS);
}