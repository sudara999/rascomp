#include <stdio.h>
#include <stdlib.h>
#include "lexer.h" // Look here for the lexer API

static FILE *fp;

int initLexer (char *filename)
{
	fp = fopen(filename, "r");
	if (fp == NULL)
		return 0;
	else
		return 1;	
}

struct token* getNextToken ()
{
	// if the file is not opened yet, or has been closed, then there are no tokens.
	if (fp == NULL)
		return NULL;

	// return the next token
	char c = fgetc(fp);
	if (c == EOF) {
		fclose(fp);
		return NULL;
	}
	else {
		struct token *token = malloc(sizeof(struct token));
		token->token_type = 0;
		token->token_val.str_val[0] = c;
		token->token_val.str_val[1] = '\0';
		sprintf(token->token_repr, "char: %s", token->token_val.str_val);
		return token;
	}	
}	
