/*
Group Number 64
Chirag C D - 2018A7PS0277P
R Rohit - 2018A7PS0224P
Abhishek Agarwal - 2018A7PS0177P
Akarsh Gowtham Shroff - 2018A7PS1121P
*/

#include "parser.h"

int is_whitespace(char * source) {
    if(*source == ' ' || *source == '\t' || *source == '\v' || *source == '\f' || *source == '\r') return 1;
    return 0;
}

void read_symbol(char *buf, tokenStream *s, int *read) {
    if(*read) return;

    if(buf[0] == '(') {
        s->token = BO;
        *read = 1;
    }
    if(buf[0] == ')') {
        s->token = BC;
        *read = 1;
    }
    if(buf[0] == '[') {
        s->token = SQO;
        *read = 1;
    }
    if(buf[0] == ']') {
        s->token = SQC;
        *read = 1;
    }
    if(buf[0] == '{') {
        s->token = CURLO;
        *read = 1;
    }
    if(buf[0] == '}') {
        s->token = CURLC;
        *read = 1;
    }
    if(buf[0] == ':') {
        s->token = CLN;
        *read = 1;
    }
    if(buf[0] == ';') {
        s->token = SEMCOL;
        *read = 1;
    }
    if(buf[0] == '=') {
        s->token = ASSGN;
        *read = 1;
    }
    if(buf[0] == '+') {
        s->token = OP_PLUS;
        *read = 1;
    }
    if(buf[0] == '-') {
        s->token = OP_MINUS;
        *read = 1;
    }
    if(buf[0] == '*') {
        s->token = OP_STAR;
        *read = 1;
    }
    if(buf[0] == '/') {
        s->token = OP_SLASH;
        *read = 1;
    }
    if(strcmp(buf, "–") == 0) {       
        s->token = OP_MINUS;
        *read = 1;
    }
    if(strcmp(buf, "..") == 0) {
        s->token = ROP;
        *read = 2;
    }
    if(strcmp(buf, "|||") == 0) {
        s->token = OP_OR;
        *read = 3;
    }
    if(strcmp(buf, "&&&") == 0) {
        s->token = OP_AND;
        *read = 3;
    }
}

void read_number(char *buf, tokenStream *s, int *read) {
    if(*read) return;

    int success = 1;
    int len = strlen(buf);
    for(int i = 0; i < len; i++) if(!(buf[i] >= '0' && buf[i] <= '9')) success = 0;
    if(!success) return;

    *read = len;
    s->token = INT;
}

void read_keyword(char *buf, tokenStream *s, int *read) {
    if(*read) return;

    int success = 0;
    if(strcmp(buf, "program") == 0) {
        s->token = KEY_PROG;
        *read = strlen(buf);
    }
    if(strcmp(buf, "declare") == 0) {
        s->token = KEY_DECL;
        *read = strlen(buf);
    }
    if(strcmp(buf, "list") == 0) {
        s->token = KEY_LIST;
        *read = strlen(buf);
    }
    if(strcmp(buf, "of") == 0) {
        s->token = KEY_OF;
        *read = strlen(buf);
    }
    if(strcmp(buf, "variables") == 0) {
        s->token = KEY_VARS;
        *read = strlen(buf);
    }
    if(strcmp(buf, "array") == 0) {
        s->token = KEY_ARR;
        *read = strlen(buf);
    }
    if(strcmp(buf, "R1") == 0) {
        s->token = R1;
        *read = strlen(buf);
    }
    if(strcmp(buf, "size") == 0) {
        s->token = KEY_SIZE;
        *read = strlen(buf);
    }
    if(strcmp(buf, "values") == 0) {
        s->token = KEY_VALS;
        *read = strlen(buf);
    }
    if(strcmp(buf, "jagged") == 0) {
        s->token = KEY_JAG;
        *read = strlen(buf);
    }
    if(strcmp(buf, "integer") == 0) {
        s->token = KEY_INT;
        *read = strlen(buf);
    }
    if(strcmp(buf, "real") == 0) {
        s->token = KEY_REAL;
        *read = strlen(buf);
    }
    if(strcmp(buf, "boolean") == 0) {
        s->token = KEY_BOOL;
        *read = strlen(buf);
    }
}

int is_alphanum(char * source) {
    if(*source >= '0' && *source <= '9') return 1;
    if(*source >= 'a' && *source <= 'z') return 1;
    if(*source >= 'A' && *source <= 'Z') return 1;
    if(*source == '_') return 1;
    return 0;
}

void read_id(char *buf, tokenStream *s, int *read) {
    if(*read) return;

    int len = strlen(buf);
    int success = 1;
    for(int i = 0; i < len; i++) if(!is_alphanum(buf + i)) success = 0;
    if(buf[0] >= '0' && buf[0] <= '9') success = 0;

    if(!success) return;
    *read = len;
    s->token = ID;
}

void tokeniseSourcecode(char * source_file, tokenStream *s) {
    long size;
    char * source = loadfile(source_file, &size);

    int line_num = 1;
    int read = 0;
    char buf[21];
    buf[20] = '\0';
    int temp;

    if(!s) {
        printf("Provide proper tokenStream head\n");
        return;
    }

    while(*source) {

        if(*source == '\n') {
            line_num++;
            source++;
            continue;
        }
        if(is_whitespace(source)) {
            source++;
            continue;
        }
        read = 0;
        temp = 0;
        while(source[temp] && source[temp] != '\n' && !is_whitespace(source + temp) && temp < 20) {
            buf[temp] = source[temp];
            temp++;
        }
        buf[temp] = '\0';
        strcpy(s->lexeme, buf);
        s->line_num = line_num;
        if(temp == 20) {
            printf("Aborting - Lexeme %s... at line %d too long\n", buf, line_num);
            exit(0);
        }

        read_symbol(buf, s, &read);
        read_number(buf, s, &read);
        read_keyword(buf, s, &read);
        read_id(buf, s, &read);

        if(read == 0) {
            printf("Aborting - Token %s at line %d not recognised\n", buf, line_num);
            exit(0);
        }
        source += strlen(s->lexeme);

        s->next = (tokenStream *) malloc(sizeof(tokenStream));
        s = s->next;
    }

    s->token = END;
    s->line_num = line_num;
    s->next = NULL;
}