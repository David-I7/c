#pragma once
struct Vector
{
    int size;
    int capacity;
    void **buf;
};

enum item_type{
    it_int =0,
    it_float=1,
    it_str=2
};

void vector_push(struct Vector *v, void *item);
void vector_pop(struct Vector *v);
void vector_print(struct Vector *v,enum item_type);
void vector_free(struct Vector *v);
struct Vector *vector_create();