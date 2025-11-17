#include "set.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    return hash;
}

void set_add(struct Set* s,const char *str){
    if (s->size > s->capacity * 1.5){
        set_data_realloc(s,s->capacity * 2);
    }

    unsigned long h = hash(str) % s->capacity;

    struct LinkedListNode* node;
    if (s->data[h] == NULL){
        node = node_create(str);
        s->data[h] = node;
    }else{
        struct LinkedListNode* tmp = s->data[h];
        if (strcmp(str,tmp->key) == 0) return;
        while(tmp->next != NULL){
            if (strcmp(str,tmp->next->key) == 0) return;
            tmp = tmp->next;
        }
        node = node_create(str);
        tmp->next = node;
    }

    s->size += 1;
};

void set_data_realloc(struct Set* s,long long size){
    struct LinkedListNode** new_data = malloc(sizeof(struct LinkedListNode*) * size);
    for(long long i = 0; i < size;i++){
        new_data[i] = NULL;
    }

    struct LinkedListNode* cur;
    for(int i = 0; i < s->capacity;i++){
        cur = s->data[i];
        while(cur != NULL){
            unsigned long h = hash(cur->key) % size;
            if (new_data[h] == NULL){
                new_data[h] = cur;
                cur = cur->next;
                new_data[h]->next = NULL;
            }else{
                struct LinkedListNode* tmp = new_data[h];
                while(tmp->next !=NULL) tmp = tmp->next;
                tmp->next = cur;
                cur = cur->next;
                tmp->next->next = NULL;
            }
        }
    }

    s->capacity = size;
    free(s->data);
    s->data = new_data;
}

void set_remove(struct Set* s,const char *str){
    if (s->size == 0) return;

    unsigned long h = hash(str) % s->capacity;
    
    struct LinkedListNode* cur = s->data[h];
    if (cur == NULL) return;

    if (strcmp(str,cur->key) == 0){
        s->data[h] = cur->next;
        cur->next =NULL;
        free(cur->key);
        free(cur);
        return;
    }

    while(cur->next !=NULL){
        if (strcmp(str,cur->next->key) == 0){
            struct LinkedListNode* tmp = cur->next;
            cur->next = tmp->next;
            tmp->next = NULL;
            free(tmp->key);
            free(tmp);
            break;
        }
        cur = cur->next;
    }
};

struct Set* set_create(){
    struct Set* s = malloc(sizeof(struct Set));
    s->capacity = 10;
    s->size = 0;
    s->data = malloc(sizeof(struct LinkedListNode*) * s->capacity);
    for (int i = 0; i < s->capacity; i++)
        s->data[i] = NULL;
    return s;
};
void set_free(struct Set* s){
    if(s->size > 0){
        struct LinkedListNode* cur;
        struct LinkedListNode* tmp;
        for(int i = 0 ; i < s->capacity;i++){
            cur = s->data[i];
            while (cur != NULL)
            {
                tmp = cur;
                cur = cur->next;
                tmp->next =NULL;
                free(tmp->key);
                free(tmp);
            }       
        }
    }

    free(s->data);
    free(s);
};
struct LinkedListNode* node_create(const char* str){
    struct LinkedListNode* node =malloc(sizeof(struct LinkedListNode));
    char *key = malloc(sizeof(char) * (strlen(str) + 1 ));
    strcpy(key,str);
    node->key = key;
    node->next =NULL;
    return node;
};
int set_contains(struct Set* s,const char *str){
    if (s->size == 0) return 0;

    unsigned long h = hash(str) % s->capacity;
    
    struct LinkedListNode* cur = s->data[h];
    while(cur != NULL){
        if (strcmp(cur->key,str) == 0){
            return 1;
        }
        cur = cur->next;
    }

    return 0;
};
void set_print(struct Set* s){
    if(s->size == 0) {
        printf("{}\n");
        return;
    }

    printf("{");

    int cnt=0;
    struct LinkedListNode* cur;
    for(int i = 0; i < s->capacity;i++){
        cur = s->data[i];
        while(cur != NULL){
            if (cnt == s->size -1 && cur != NULL) break; 
            printf("%s,",cur->key);
            cur = cur->next;
            ++cnt;
        }
        if (cnt == s->size -1 && cur !=NULL) break; 
    }

    printf("%s",cur->key);
    printf("}\n");
}

char** set_to_array(struct Set* s){
    char** arr = malloc(sizeof(char *) * s->size);
    int idx=0;
    struct LinkedListNode*cur;
    for(int i = 0 ;i < s->capacity;i++){
        cur = s->data[i];
        while(cur!=NULL){
            arr[idx++] = cur->key;
            cur=cur->next;
        }
    }

    return arr;
}