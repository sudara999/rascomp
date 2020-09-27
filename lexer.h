#ifndef LEXER
#define LEXER

#define max_token_length 100

enum token_type {
	ASSIGN,	// =
	LT,	// <
	LE,	// <=
	GT,	// >
	GE,	// >=
	EQ,	// ==
	NE,	// !=
	NOT	// !
};

union token_val{
	char str_val[max_token_length + 1];
	int i_val;
	float f_val;
};

struct token {
	enum token_type token_type;
	union token_val token_val;
	char token_repr[max_token_length + 1];
};

extern int initLexer (char *filename); 
// This function will initialise the lexical analyser with the source file. 
// Returns 1 if the file is opened successfully, 0 if not.

extern struct token* getNextToken (void);
// This function will return the next token.
// If there is none, it will return NULL.
// Caution: Free the token after use, since the token is created in the heap.

#endif
