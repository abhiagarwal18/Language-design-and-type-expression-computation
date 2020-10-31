/*
Group Number 64
Chirag C D - 2018A7PS0277P
R Rohit - 2018A7PS0224P
Abhishek Agarwal - 2018A7PS0177P
Akarsh Gowtham Shroff - 2018A7PS1121P
*/
#include "parser.h"

char * loadfile (char * filename, long * sizeptr) {
	FILE * file = fopen(filename, "r");
    if(!file) {
        printf("File %s not found\n", filename);
        exit(0);
    }
	
	fseek(file, 0L, SEEK_END);
	*sizeptr = ftell(file);
	fseek(file, 0L, SEEK_SET);

	char * content = (char *) malloc((*sizeptr + 1) * sizeof(char));
	content[*sizeptr] = '\0';
	for(long i = 0; i < *sizeptr; i++) content[i] = (char) fgetc(file);
	
	fclose(file);
	return content;
}