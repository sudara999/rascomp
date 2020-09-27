#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

int main(){
	char filename[] = "./testLexer.txt";
	if (!initLexer(filename)) {
		printf("File could not be opened.\n");
		return -1;
	} 
	else {
		struct token *token = getNextToken();
		while (token != NULL) {
			printf("%s\n", token->token_repr);
			free(token);
			token = getNextToken();
		}
		return 0;
	}
}
