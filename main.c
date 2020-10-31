/*
Group Number 64
Chirag C D - 2018A7PS0277P
R Rohit - 2018A7PS0224P
Abhishek Agarwal - 2018A7PS0177P
Akarsh Gowtham Shroff - 2018A7PS1121P
*/

#include "parser.h"

void driver(char * filename) {
    while (1)
    {
        int option;
        char buf[2];
        printf("\nEnter option:\n0-exit\n1-create_tree\n2-traverse_tree\n3-print_tree (with type info) (WARNING - lines spill over)\n4-print_table\n5-print_tree (without type info) (pretty)\n");
        scanf("%s", buf);
        buf[1] = '\0';
        if(!(buf[0] >= '0' && buf[0] <= '5')) {
            printf("Invalid option\n");
            continue;
        }
        option = atoi(buf);

        if(option == 0) {
            printf("Exiting\n");
            break;
        }

        tokenStream Stream;
        parseTree Tree;
        Grammar G;

        readGrammar("grammar.txt", &G);
        tokeniseSourcecode(filename, &Stream);
        createParseTree(&Tree, &Stream, G);

        if(option == 1) continue;

        typeExpressionTable Table;
        for(int i = 0; i < 16; i++) Table.heads[i] = NULL;
        traverseParseTree(&Tree, &Table);

        if(option == 2) continue;

        if(option == 3) {
            printParseTree(&Tree, 1);
            continue;
        }

        if(option == 4) {
            printTypeExpressionTable(Table);
            continue;
        }

        if(option == 5) {
            printParseTree(&Tree, 0);
            continue;
        }
    }
    
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        printf("Proper usage - ./exec <input file>\n");
        exit(1);
    }
    driver(argv[1]);
    return 0;
}