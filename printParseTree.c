/*
Group Number 64
Chirag C D - 2018A7PS0277P
R Rohit - 2018A7PS0224P
Abhishek Agarwal - 2018A7PS0177P
Akarsh Gowtham Shroff - 2018A7PS1121P
*/

#include "parser.h"

void get_str(TermType t, char * buf, int is_term) {

    if(is_term == 0) {
        if(t.nt == PROGRAM) strcpy(buf, "PROGRAM");
        if(t.nt == DECL_LIST) strcpy(buf, "DECL_LIST");
        if(t.nt == ASSGN_LIST) strcpy(buf, "ASSGN_LIST");
        if(t.nt == DECL_STMT) strcpy(buf, "DECL_STMT");
        if(t.nt == ASSGN_STMT) strcpy(buf, "ASSGN_STMT");
        if(t.nt == INDEX) strcpy(buf, "INDEX");
        if(t.nt == RNG) strcpy(buf, "RNG");
        if(t.nt == TYPE) strcpy(buf, "TYPE");
        if(t.nt == ARRAY_RANGE) strcpy(buf, "ARRAY_RANGE");
        if(t.nt == DTYPE) strcpy(buf, "DTYPE");
        if(t.nt == ONE_OR_MORE_VAR) strcpy(buf, "ONE_OR_MORE_VAR");
        if(t.nt == LISTVAR) strcpy(buf, "LISTVAR");
        if(t.nt == SINGLEVAR_DEC) strcpy(buf, "SINGLEVAR_DEC");
        if(t.nt == MULTIVAR_DEC) strcpy(buf, "MULTIVAR_DEC");
        if(t.nt == JAGGED2_DEC) strcpy(buf, "JAGGED2_DEC");
        if(t.nt == JAGGED3_DEC) strcpy(buf, "JAGGED3_DEC");
        if(t.nt == JAGGED2_INIT) strcpy(buf, "JAGGED2_INIT");
        if(t.nt == JAGGED3_INIT) strcpy(buf, "JAGGED3_INIT");
        if(t.nt == JAGGED2_IDX) strcpy(buf, "JAGGED2_IDX");
        if(t.nt == JAGGED3_IDX) strcpy(buf, "JAGGED3_IDX");
        if(t.nt == JAGGED2_RNG) strcpy(buf, "JAGGED2_RNG");
        if(t.nt == JAGGED3_RNG) strcpy(buf, "JAGGED3_RNG");
        if(t.nt == JAGGED2_SINGLE_INIT) strcpy(buf, "JAGGED2_SINGLE_INIT");
        if(t.nt == JAGGED3_SINGLE_INIT) strcpy(buf, "JAGGED3_SINGLE_INIT");
        if(t.nt == EXPRESSION) strcpy(buf, "EXPRESSION");
        if(t.nt == ARRAY_SEL) strcpy(buf, "ARRAY_SEL");
        if(t.nt == VAR) strcpy(buf, "VAR");
        if(t.nt == ARRAY_IDX) strcpy(buf, "ARRAY_IDX");
        if(t.nt == ARITHMETIC_EXP) strcpy(buf, "ARITHMETIC_EXP");
        if(t.nt == BOOLEAN_EXP) strcpy(buf, "BOOLEAN_EXP");
        if(t.nt == ADDITIVE_TERM) strcpy(buf, "ADDITIVE_TERM");
        if(t.nt == TERM) strcpy(buf, "TERM");
        if(t.nt == FACTOR) strcpy(buf, "FACTOR");
        if(t.nt == L_OR_TERM) strcpy(buf, "L_OR_TERM");
        if(t.nt == L_AND_TERM) strcpy(buf, "L_AND_TERM");
        if(t.nt == EPSILON) strcpy(buf, "EPSILON");
    }
    if(is_term == 1) {
        if(t.tok.token == BO) strcpy(buf, "BO");
        if(t.tok.token == BC) strcpy(buf, "BC");
        if(t.tok.token == SQO) strcpy(buf, "SQO");
        if(t.tok.token == SQC) strcpy(buf, "SQC");
        if(t.tok.token == CURLO) strcpy(buf, "CURLO");
        if(t.tok.token == CURLC) strcpy(buf, "CURLC");
        if(t.tok.token == CLN) strcpy(buf, "CLN");
        if(t.tok.token == SEMCOL) strcpy(buf, "SEMCOL");
        if(t.tok.token == ROP) strcpy(buf, "ROP");
        if(t.tok.token == ASSGN) strcpy(buf, "ASSGN");
        if(t.tok.token == KEY_PROG) strcpy(buf, "KEY_PROG");
        if(t.tok.token == KEY_DECL) strcpy(buf, "KEY_DECL");
        if(t.tok.token == KEY_LIST) strcpy(buf, "KEY_LIST");
        if(t.tok.token == KEY_OF) strcpy(buf, "KEY_OF");
        if(t.tok.token == KEY_VARS) strcpy(buf, "KEY_VARS");
        if(t.tok.token == KEY_ARR) strcpy(buf, "KEY_ARR");
        if(t.tok.token == R1) strcpy(buf, "R1");
        if(t.tok.token == KEY_SIZE) strcpy(buf, "KEY_SIZE");
        if(t.tok.token == KEY_VALS) strcpy(buf, "KEY_VALS");
        if(t.tok.token == KEY_JAG) strcpy(buf, "KEY_JAG");
        if(t.tok.token == KEY_INT) strcpy(buf, "KEY_INT");
        if(t.tok.token == KEY_REAL) strcpy(buf, "KEY_REAL");
        if(t.tok.token == KEY_BOOL) strcpy(buf, "KEY_BOOL");
        if(t.tok.token == OP_PLUS) strcpy(buf, "OP_PLUS");
        if(t.tok.token == OP_MINUS) strcpy(buf, "OP_MINUS");
        if(t.tok.token == OP_STAR) strcpy(buf, "OP_STAR");
        if(t.tok.token == OP_SLASH) strcpy(buf, "OP_SLASH");
        if(t.tok.token == OP_AND) strcpy(buf, "OP_AND");
        if(t.tok.token == OP_OR) strcpy(buf, "OP_OR");
        if(t.tok.token == ID) strcpy(buf, "ID");
        if(t.tok.token == INT) strcpy(buf, "INT");
        if(t.tok.token == END) strcpy(buf, "END");
    }
}

void print_rule (parseTree * t) {
    char buf[25];
    get_str(t->term.type, buf, 0);
    printf("Grammar- %s ->", buf);
    for(int i = 0; i < t->num_children; i++) {
        get_str(t->children[i]->term.type, buf, t->children[i]->term.is_term);
        printf(" %s", buf);
    }
}

void print_type_info(parseTree * t) {
    printf("Id (If appl)-%-20s\t", t->type_info.id);
    char arr_type[5];
    if(t->type_info.arr_info == PRIMITIVE) strcpy(arr_type, "PRIM");
    if(t->type_info.arr_info == RECT_ARR) strcpy(arr_type, "RECT");
    if(t->type_info.arr_info == JAG_ARR) strcpy(arr_type, "JAG");
    printf("Arrtype (If appl)-%-4s\t", arr_type);
    if(t->type_info.arr_storage == STATIC) strcpy(arr_type, "STAT");
    if(t->type_info.arr_storage == DYNAMIC) strcpy(arr_type, "DYN");
    if(t->type_info.arr_storage == NONE) strcpy(arr_type, "NONE");
    printf("Arrstorage (If appl)-%-4s\t", arr_type);
}

void internalPrintParseTree(parseTree * t, int depth, int print_type) {

    char buf[25];
    int is_term = t->term.is_term;
    char * lexeme = (t->term.is_term) ? t->term.type.tok.lexeme : NULL;
    int line_num = (t->term.is_term) ? t->term.type.tok.line_num : -1;
    get_str(t->term.type, buf, t->term.is_term);
    printf("Depth-%3d\t", depth);
    printf("Symbol-%-20s\t", buf);
    printf("Is_term-%d\t", is_term);
    if(is_term) printf("Lexeme-%-20s\t", lexeme);
    if(is_term) printf("Line-%3d\t", line_num);
    else {
        printf("Rule-");
        print_rule(t);
    }
    if(print_type && !is_term) {printf("\tType info- ");    printTypeLine(&(t->type_info));}
    else printf("\n");
    for(int i = 0; i < t->num_children; i++) internalPrintParseTree(t->children[i], depth+1, print_type);
}

void printParseTree(parseTree * t, int print_type) {
    printf("Printing tree\n");
    internalPrintParseTree(t, 0, print_type);
    printf("Print tree complete\n");
}