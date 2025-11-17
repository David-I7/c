#pragma once

enum VectorItemType{
    VECTOR_INT ,
    VECTOR_FLOAT,
    VECTOR_STR,
};


typedef struct Vector
{
    void **buf;
    int size;
    int capacity;
} Vector;


void vectorPushback(struct Vector *v, void *item);
void* vectorBack(struct Vector *v);
void* vectorFront(struct Vector *v);
void vectorPopback(struct Vector *v);
void vectorPopfront(struct Vector *v);
void vectorPrint(struct Vector *v);
void vectorFree(struct Vector *v);
struct Vector *vectorCreate();