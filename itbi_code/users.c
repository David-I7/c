#include "dts/Vector.h"
#include "stdlib.h"

int main(){
    struct Vector* v=vector_create();
    
    int size =25;
    for(int i = 0 ; i < size;i++){
       int* x = malloc(sizeof(int));
       *x = i;
        vector_push(v,(void*)x);
    }

    
    while(v->size > 10){
        vector_pop(v);
    }
    vector_print(v,0);
    vector_free(v);
    return 0;
}