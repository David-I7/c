#include "matrice_dreptunghiulara.h"
#include <stdlib.h>

int main(int argc,char ** argv){
    if (argc !=2){
        fprintf(stderr,"Mod utilizare: %s <fisier>",argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE* f1 = fopen(argv[1],"r");
    if (f1 == NULL){
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }

    struct tuple* matrix_size = get_size(f1);

    printf("[\n");
    for(int i = 0 ; i< matrix_size->m; i++){
        printf("  [");
        int j = 0;
        for(j; j < matrix_size->n -1; j++){
            printf("%f,",get(f1,i,j));
        }
        printf("%f",get(f1,i,j));
        printf("]\n");
    }
    printf("]\n");

    fclose(f1);
    exit(EXIT_SUCCESS);
}

