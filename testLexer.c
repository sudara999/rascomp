#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

int main(){
	char filename[] = "./testLexer.txt";
	if (initLexer(filename) == SUCC) {
		struct token *token = malloc(sizeof(struct token));
		// ! error handling
		while (getNextToken(token) == SUCC)
			printf("%s\n", token->token_repr);
		free(token);
		return 0;
	} 
	else {
		printf("File could not be opened.\n");
		return -1;
	}
}
