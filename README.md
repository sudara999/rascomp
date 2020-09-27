# CSE 304 Project

This is a simple compiler for the language 'rascl'

## Phase 1: Lexical Analysis

Lexical Analysis will be done by Table-driven scanning. The lexical analyser is implemented in lexer.c

### Lexical Analyser function calls

The lexical analyser, lexer.c, contains the following the function calls:

#### int initLexer(char *filename);

This function will initialise the lexical analyser with the source file. 
Returns 1 if the file is opened successfully, 0 if not.

#### struct token* getNextToken(void);

This function will return the next token.
If there is none, it will return NULL.
Caution: Free the token after use, since the token is created in the heap.

### Lexical Analyser data structures

#### Tokens

Tokens are stored as `struct token`.

struct token {
	enum token_type token_type;
	union token_val token_val;
	char token_repr[max_token_length + 1];
};

They have the following members:
	+ token_type: The type of the token (for example, the IF token)
	+ token_val: Tokens may have a value associated with them.
	+ token_repr: The textual representation of the token. This may be printed out.

Please refer to lexer.h for more details.

### Testing the lexical analyser

The program testLexer.c will use lexer.c to get tokens from the test file, testLexer.txt.
