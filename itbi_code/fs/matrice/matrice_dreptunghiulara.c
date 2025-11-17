#include "matrice_dreptunghiulara.h"
#include "string.h"

struct tuple matrix_size;

void new_matrix(FILE *f,int m,int n){
    if (!f) {
        return;
    }

    fwrite(&m,sizeof(int),1,f);
    fwrite(&n,sizeof(int),1,f);
    float x =0;
    for(int i = 0 ; i < m*n ;i++){
        fwrite(&x,sizeof(float),1,f);
    }
}
float get(FILE *f,int i,int j){
    get_size(f);
    fseek(f,((sizeof(float) * matrix_size.n *  i) + sizeof(float) * j),SEEK_CUR);
    float x;
    fread(&x,sizeof(float),1,f);
    return x;
}
void set(FILE *f,int i,int j,float x){
    get_size(f);
    fseek(f,((sizeof(float) * matrix_size.n *  i) + sizeof(float) * j),SEEK_CUR);
    fwrite(&x,sizeof(float),1,f);
}
struct tuple* get_size(FILE *f){
    rewind(f);
    fread(&matrix_size.m,sizeof(int),1,f);
    fread(&matrix_size.n,sizeof(int),1,f);
    return &matrix_size;
}
