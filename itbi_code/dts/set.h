#pragma once

struct LinkedListNode{
    char* key;
    struct LinkedListNode* next;
};

struct Set{
    int size;
    int capacity;
    struct LinkedListNode** data;
};

unsigned long hash(const char *str);
void set_add(struct Set* s,const char *str);
void set_remove(struct Set* s,const char *str);
struct Set* set_create();
void set_free(struct Set* s);
int set_contains(struct Set* s,const char *str);
struct LinkedListNode* node_create(const char* str);
void set_print(struct Set* s);
void set_data_realloc(struct Set* s,long long size);
char** set_to_array(struct Set* s);