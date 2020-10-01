#ifndef LEXER
#define LEXER

#define SUCC 0			// Function successfully completeted
#define FAIL -1			// Function failed
#define ERR_STATE -1		// DFA state: error state
#define START_STATE 0		// DFA state: start state
#define NOT_ACC_STATE -1	// Token type: No token for a non-accept state
#define I_TOKEN	-2		// Token type: A token to be ignored (not returned)
#define INVALID_SYM -1		// Token type: No token for a invalid symbol
#define NO_SYM -2		// Token type: No token for no new symbols read

#define max_line_length 101
// A line can take up to 101 characters including the null character
// 	in the end of the line

#define max_token_length 100
// The maximum token length measures the total numbers of characters up
// 	to (excluding) the null character

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

// ! try using typedef; token.type, token.val, token.repr
struct token {
	enum token_type token_type;
	union token_val token_val;
	char token_repr[max_token_length + 1];
};

extern int initLexer (char *filename); 
// This function will initialise the lexical analyser with the source file. 
// Returns SUCC if the file is opened successfully, FAIL if not.

extern int getNextToken (struct token* nextToken);
// This function will populate the nextToken struct with the values of the next token.
// If a token is returned, the function will return SUCC.
// If there is none, it will return FAIL.

#endif
