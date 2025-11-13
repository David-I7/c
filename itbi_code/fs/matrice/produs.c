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

    int m1,n1,m2,n2;
    struct tuple* matrix_size = get_size(f1);
    m1 = matrix_size->m;
    n1 = matrix_size->n;
    matrix_size = get_size(f2);
    m2 = matrix_size->m;
    n2 = matrix_size->n;
    if (n1 != m2){
        fprintf(stderr,"Matricile nu se pot inmulti!\n");
        exit(EXIT_FAILURE);
    }

    FILE* dest = fopen(argv[3],"w+");
    if (dest == NULL){
        perror(argv[3]);
        exit(EXIT_FAILURE);
    }

    new_matrix(dest,m1,n2);
    for(int i = 0; i < m1; i++){
        for(int j = 0; j < n2;j++){
            for(int k = 0 ; k < n1; k++){
                set(dest,i,j, get(dest,i,j) + get(f1,i,k) * get(f2,k,j));
            }
        }
    }

    fclose(f1);
    fclose(f2);
    fclose(dest);
    exit(EXIT_SUCCESS);
}