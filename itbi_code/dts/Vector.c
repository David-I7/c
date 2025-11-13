#include "Vector.h"
#include <stdlib.h>
#include <stdio.h>

void vector_push(struct Vector *v, void *item){
    if(v->size == v->capacity){
        v->capacity<<=1;
        v->buf = realloc(v->buf,v->capacity * sizeof(void*));
    }
    v->buf[v->size++] = item;
};  
void vector_pop(struct Vector *v){
    if (v->size == 0) return;
    free(v->buf[--v->size]);
    v->buf[v->size] = NULL;
    if(v->size < (v->capacity >> 1)){
        v->capacity>>=1;
        v->buf = realloc(v->buf,v->capacity * sizeof(void*));
    }
    
};
void vector_print(struct Vector *v,enum item_type type){
    if (v->size == 0){
        puts("[]");
        return;
    }

    printf("[");
    for (int i = 0; i < v->size - 1; i++)
    {
        switch (type)
        {
            case it_int:{
                printf("%d,", *((int*)v->buf[i]));
                break;
            }
            case it_float:{
                printf("%f,", *((float *)v->buf[i]));
                break;
            }
            case it_str:{
                printf("%s,", ((char*)v->buf[i]));
                break;
            }
        }
    }
     switch (type)
        {
            case it_int:{
                printf("%d", *((int*)v->buf[v->size - 1]));
                break;
            }
            case it_float:{
                printf("%f", *((float *)v->buf[v->size - 1]));
                break;
            }
            case it_str:{
                printf("%s", ((char*)v->buf[v->size - 1]));
                break;
            }
        }
    puts("]");
};
void vector_free(struct Vector *v){
    for(int i = 0; i < v->size; i++){
        free(v->buf[i]);
    }
    free(v);
}
struct Vector* vector_create(){
    struct Vector* v= malloc(sizeof(struct Vector));
    v->size = 0;
    v->capacity = 10;
    v->buf = malloc(10 * sizeof(void*));
    return v;
};
