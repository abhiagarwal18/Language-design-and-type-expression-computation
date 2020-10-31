/*
Group Number 64
Chirag C D - 2018A7PS0277P
R Rohit - 2018A7PS0224P
Abhishek Agarwal - 2018A7PS0177P
Akarsh Gowtham Shroff - 2018A7PS1121P
*/
#include "parser.h"

int get_length(Term * rule) {
	if(!rule) return 0;
	return 1 + get_length(rule->next);
}

stackNode * reverse_dump(Term * rule) {
	if(!rule) return NULL;

	stackNode * node = (stackNode *) malloc(sizeof(stackNode));
	node->next = reverse_dump(rule->next);
	node->is_term = rule->is_term;
	node->type = rule->type;

	return node;
}

void print_rule_local(Term * rule) {
	if(!rule) return;
	char buf[25];
	get_str(rule->type, buf, rule->is_term);
	printf("%s (is_term %d) ", buf, rule->is_term);
	print_rule_local(rule->next);
}

void free_children(parseTree * t) {
	//printf("Freeing %p\n", t);
	if(t == NULL || t->children == NULL) return;
	for(int i = 0; i < t->num_children; i++) free_children(t->children[i]);
	for(int i = 0; i < t->num_children; i++) free(t->children[i]);
	free(t->children);
	//printf("Iter complete\n");
}

int apply(Grammar * g, parseTree * t, tokenStream ** stream, Term * rule) {

	tokenStream * s = *stream;
	char buffer[25];
	get_str(t->term.type, buffer, t->term.is_term);
	// printf("Applying rule %d %s:\n", t->term.is_term, buffer);
	// print_rule_local(rule);
	// printf("\n");

	stack * local_stack = (stack *) malloc(sizeof(stack));
	local_stack->head = reverse_dump(rule);
	stackNode * mover = local_stack->head;

	int length = get_length(rule);
	local_stack->size = length;
	t->num_children = length;
	t->children = (parseTree **) malloc(length * sizeof(parseTree *));
	for(int i = 0; i < length; i++) t->children[i] = (parseTree *) malloc(sizeof(parseTree));
	
	for(int i = 0; i < length; i++) {

		stackNode * temp = pop(local_stack);
		// printf("Looking at %s line %d\n", s->lexeme, s->line_num);
		// if(temp == NULL) printf("Underflow\n");		// Should not occur
		int is_term = temp->is_term;
		TermType type = temp->type;
		int error = 1;

		// get_str(type, buffer, is_term);
		// printf("%s\n", buffer);

		if(is_term) {
			error = (type.tok.token != s->token);
			if(error) return -1;
			t->children[i]->num_children = 0;
			t->children[i]->children = NULL;
			t->children[i]->term.is_term = 1;
			t->children[i]->term.type.tok.token = s->token;
			t->children[i]->term.type.tok.line_num = s->line_num;
			strcpy(t->children[i]->term.type.tok.lexeme, s->lexeme);
			s = s->next;
			continue;
		}

		if(type.nt == EPSILON) {
			t->children[i]->num_children = 0;
			t->children[i]->children = NULL;
			t->children[i]->term.is_term = 0;
			t->children[i]->term.type.nt = EPSILON;
			continue;
		}

		int num_rules;
		Term ** rules = get_rules(g, type, &num_rules);
		for(int j = 0; j < num_rules; j++) {
			t->children[i]->term.is_term = 0;
			t->children[i]->term.type = type;
			error = apply(g, t->children[i], &s, rules[j]);
			// printf("Sublooping %d, error is %d\n", j, error);
			if(!error) break;
			// free_children(t->children[i]);
		}
		if(error) return -1;
	}
	*stream = s;
	return 0;
}

void createParseTree(parseTree *t, tokenStream *s, Grammar g) {

	printf("Building parse tree\n");
	stackNode *starter = (stackNode *) malloc(sizeof(stackNode));
	starter->is_term = 0;
	starter->type.nt = PROGRAM;
	stack * st = (stack *) malloc(sizeof(stack));
	push(st, starter);

	t->term.is_term = 0;
	t->term.type.nt = PROGRAM;

	int num_rules;
	Term ** rules = get_rules(&g, starter->type, &num_rules);

	int error = apply(&g, t, &s, rules[0]);
	if(error) {
		printf("Failed to build parse tree\n");
		exit(1);
	}
	if(s->token != END) printf("Ignoring extra characters after end\n");
	printf("Successfully built parse tree\n");
}