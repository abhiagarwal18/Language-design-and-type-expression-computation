/*
Group Number 64
Chirag C D - 2018A7PS0277P
R Rohit - 2018A7PS0224P
Abhishek Agarwal - 2018A7PS0177P
Akarsh Gowtham Shroff - 2018A7PS1121P
*/
#include "parser.h"

void print_rect_ranges(rect_arr_te info) {
    printf("NumDims-%2d\t", info.num_dim);
    printf("Ranges- ");
    for(int i = 0; i < info.num_dim; i++) {
        if(info.dim_range[i].is_r1_static) printf("[%d..", info.dim_range[i].r1.r_s);
        if(!info.dim_range[i].is_r1_static) printf("[%s..", info.dim_range[i].r1.r_d);
        if(info.dim_range[i].is_r2_static) printf("%d] ", info.dim_range[i].r2.r_s);
        if(!info.dim_range[i].is_r2_static) printf("%s] ", info.dim_range[i].r2.r_d);
    }
    printf("\t");
}

void print_jagged_type(jagged_arr_te info) {
    printf("NumDims-%2d\t", info.num_dim);
    printf("Basic Range- [%d..%d]\t", info.range_R1.r1, info.range_R1.r2);
    printf("Other ranges- ");
    int num_ranges = info.range_R1.r2 + 1 - info.range_R1.r1;
    for(int i = 0; i < num_ranges; i++) {
        printf("SubDimensions %d Sizes ", info.range_R2[i].num_dim);
        for(int j = 0; j < info.range_R2[i].num_dim; j++) printf("%d ", info.range_R2[i].dims[j]);
    }
    printf("\t");
}

void get_enum_str (primitive_info info, char * buf) {
    if(info == BOOLEAN) strcpy(buf, "BOOL");
    if(info == INTEGER) strcpy(buf, "INT");
    if(info == REAL) strcpy(buf, "REAL");
}

void printTypeLine (link * l) {
    printf("Id-%-20s\t", l->id);
    char arr_type[5];
    if(l->arr_info == PRIMITIVE) strcpy(arr_type, "PRIM");
    if(l->arr_info == RECT_ARR) strcpy(arr_type, "RECT");
    if(l->arr_info == JAG_ARR) strcpy(arr_type, "JAG");
    printf("Arrtype (If appl)-%-4s\t", arr_type);
    if(l->arr_storage == STATIC) strcpy(arr_type, "STAT");
    if(l->arr_storage == DYNAMIC) strcpy(arr_type, "DYN");
    if(l->arr_storage == NONE) strcpy(arr_type, "NONE");
    printf("Arrstorage (If appl)-%-4s\t", arr_type);
    if(l->arr_info == PRIMITIVE) get_enum_str(l->type.prim_info, arr_type);
    if(l->arr_info == RECT_ARR) get_enum_str(l->type.rect_arr_info.betype, arr_type);
    if(l->arr_info == JAG_ARR) get_enum_str(l->type.jagged_arr_info.betype, arr_type);
    printf("Basic-%4s\t", arr_type);
    if(l->arr_info == RECT_ARR) print_rect_ranges(l->type.rect_arr_info);
    if(l->arr_info == JAG_ARR) print_jagged_type(l->type.jagged_arr_info);
    printf("\n");
}

void printTypeExpressionTable(typeExpressionTable T) {

    printf("Printing Table\n");
    for(int i = 0; i < 16; i++) {
        link * mover = T.heads[i];
        while(mover) {
            printTypeLine(mover);
            mover = mover->next;
        }
    }
    printf("Printed Table\n");
}