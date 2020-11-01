#ifndef LEXER
#define LEXER

#define START_STATE 0		// DFA state: start state
#define ERR_STATE -1		// DFA state: error state
#define INVALID_SYM -1		// Token type: No token for a invalid symbol
#define NO_SYM -2		// Token type: No token for no new symbols read
#define NOT_ACC_STATE -1	// Token type: No token for a non-accept state
#define I_TOKEN	-2		// Token type: A token to be ignored (not returned)

enum token_type {
	// OPERATORS
	ASSIGN,	// =
	LT,	// <
	LE,	// <=
	GT,	// >
	GE,	// >=
	EQ,	// ==
	NE,	// !=
	PLUS, 	// +
	MINUS,	// -
	MULT,	// *
	DIV,	// /
	NOT,	// !
	AND,	// &&
	OR,	// ||
	// PUNCTUATION
	SEMICOLON,	// ;
	LPAREN,		// (
	RPAREN,		// )
	COMMA,		// ,
	LBRACE,		// {
	RBRACE, 	// }
	LBRACKET,	// [
	RBRACKET,	// ]
	// KEYWORDS
	IF,		// if
	ELSE, 		// else
	WHILE, 		// while
	INT, 		// int
	FLOAT, 		// float
	PRINT,		// print
	READ, 		// read
	FUNCTION,	// function
	// OTHER
	ID,		// <identifier>
	ICONST,		// <integer constant>
	FCONST,		// <Floating constant>
	COMMENT		// <comment>
};

union token_val{
	char str_val[MAX_TOKEN_LEN + 1];
	int i_val;
	float f_val;
};

struct token {
	enum token_type token_type;
	union token_val token_val;
	char token_repr[MAX_TOKEN_LEN + 1];
};

extern int initLexer (char *filename); 
// This function will initialise the lexical analyser with the source file. 
// Returns SUCC if the file is opened successfully, FAIL if not.

extern int getNextToken (struct token* nextToken);
// This function will populate the nextToken struct with the values of the next token.
// If a token is returned, the function will return SUCC.
// If there is none, it will return FAIL.

#endif
