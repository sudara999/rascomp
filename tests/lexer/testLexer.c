#include <stdio.h>
#include <stdlib.h>
#include <compiler_api.h>
#include <lexer.h>

int main(){
	char filename[] = "./tests/lexer/testLexer.txt";
	if (initLexer(filename) == SUCC) {
		struct token *token = malloc(sizeof(struct token));
		// ! error handling
		while (getNextToken(token) == SUCC)
			printf("%s\n", token->token_repr);
		printf("Final return value: %d\n", token->token_type);
		free(token);
		return 0;
	} 
	else
		return -1;
}
