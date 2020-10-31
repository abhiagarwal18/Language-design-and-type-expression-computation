/*
Group Number 64
Chirag C D - 2018A7PS0277P
R Rohit - 2018A7PS0224P
Abhishek Agarwal - 2018A7PS0177P
Akarsh Gowtham Shroff - 2018A7PS1121P
*/
#include "parser.h"

stackNode * pop(stack * s) {
    if(s->size == 0) return NULL;
    stackNode * temp = s->head;
    s->head = s->head->next;
    return temp;
}

stackNode * peek(stack * s) {
    return s->head;
}

void push(stack * s, stackNode * node) {
    (s->size)++;
    node->next = s->head;
    s->head = node;
}