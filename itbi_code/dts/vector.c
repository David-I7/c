#include "vector.h"
#include <stdlib.h>
#include <stdio.h>

void vectorPushback(struct Vector *v, void *item){
    if(v->size == v->capacity){
        v->capacity<<=1;
        v->buf = realloc(v->buf,v->capacity * sizeof(void*));
    }
    v->buf[v->size++] = item;
};  
void vectorPopback(struct Vector *v){
    if (v->size == 0) return;
    free(v->buf[--v->size]);
    v->buf[v->size] = NULL;
    if(v->size < (v->capacity >> 1)){
        v->capacity>>=1;
        v->buf = realloc(v->buf,v->capacity * sizeof(void*));
    }
};
void vectorPopfront(struct Vector *v){
    if (v->size == 0) return;
    free(v->buf[0]);
    for(int i = 1; i < v->size;i++){
        v->buf[i-1] = v->buf[i];
    }
    v->buf[--v->size] = NULL;
    if(v->size < (v->capacity >> 1)){
        v->capacity>>=1;
        v->buf = realloc(v->buf,v->capacity * sizeof(void*));
    }
    
};

void vectorPrint(struct Vector *v, enum VectorItemType type){
    if (v->size == 0){
        puts("[]");
        return;
    }

    printf("[");
    for (int i = 0; i < v->size - 1; i++)
    {
        switch (type)
        {
            case VECTOR_INT:{
                printf("%d,", *((int*)v->buf[i]));
                break;
            }
            case VECTOR_FLOAT:{
                printf("%f,", *((float *)v->buf[i]));
                break;
            }
            case VECTOR_STR:{
                printf("%s,", ((char*)v->buf[i]));
                break;
            }
        }
    }
     switch (type)
        {
            case VECTOR_INT:{
                printf("%d", *((int*)v->buf[v->size - 1]));
                break;
            }
            case VECTOR_FLOAT:{
                printf("%f", *((float *)v->buf[v->size - 1]));
                break;
            }
            case VECTOR_STR:{
                printf("%s", ((char*)v->buf[v->size - 1]));
                break;
            }
        }
    puts("]");
};
void vectorFree(struct Vector *v){
    for(int i = 0; i < v->size; i++){
        free(v->buf[i]);
    }
    free(v);
}
struct Vector* vectorCreate(){
    struct Vector* v= malloc(sizeof(struct Vector));
    v->size = 0;
    v->capacity = 10;
    v->buf = malloc(10 * sizeof(void*));
    return v;
};

void* vectorBack(struct Vector *v){
    if (v->size == 0) return NULL;
    return v->buf[v->size -1];
};
void* vectorFront(struct Vector *v){
    if (v->size == 0) return NULL;
    return v->buf[0];
};