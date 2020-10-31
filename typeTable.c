/*
Group Number 64
Chirag C D - 2018A7PS0277P
R Rohit - 2018A7PS0224P
Abhishek Agarwal - 2018A7PS0177P
Akarsh Gowtham Shroff - 2018A7PS1121P
*/

#include "parser.h"

int hash(char * str) {
    int i = 0, h = 0;
    while(str[i]) {
        h += str[i];
        h %= 16;
        i++;
    }
    return h;
}

link * get_link(typeExpressionTable * t, char * str) {
    int bucket_num = hash(str);
    link * l = t->heads[bucket_num];
    while(l) {
        if(strcmp(l->id, str) == 0) return l;
        l = l->next;
    }
    return NULL;
}

void put_link(typeExpressionTable * t, link * l) {
    if(!l) return;
    if(get_link(t, l->id)) {
        printf("Error - ID already used\n");
        return;
    }
    int bucket_num = hash(l->id);
    l->prev = *(t->heads) + bucket_num;
    l->next = t->heads[bucket_num];
    if(l->next) l->next->prev = l;
    t->heads[bucket_num] = l;
    return;
}
