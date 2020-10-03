# CSE 304 Project

This is a simple compiler for the language 'rascl'

## Phase 1: Lexical Analysis

The lexical analyser is implemented in lexer.c

### Lexical Analyser function calls

The lexical analyser, lexer.c, contains the following the function calls:

#### int initLexer(char *filename);

This function will initialise the lexical analyser with the source file. 
Returns SUCC if the file is opened successfully, FAIL if not.

#### int getNextToken(struct token *nextToken);

This function will return the next token.
Returns SUCC on success. 
If there is none, it will return FAIL. nextToken's token_type field will be NO_SYM.
If an invalid symbol is read, then the symbol is stored in nextToken's token_val field. The token_type field will be INVALID_SYM. FAIL is returned. 

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
	+ token_val: The default value/type of a token is the lexeme that was scanned stored in a char array. To change the default value/type, the function onTokenDetect is used. 
	+ token_repr: The textual representation of the token. The textual representation of the tokens are stored in an array, char *token_reps[]. To change the default textual representation, the function onTokenDetect is used.

Please refer to lexer.h for more details.

### How the lexical analyser works

Lexical Analysis will be done by table-driven scanning. The delta function will return the next state based on the current state and the input symbol. If the current state is a final state, the accept function will return the token associated with the current state. By default, this token is returned to the user. But, the token can be ignored (not returned) by returning the next possible token instead within the onTokenDetect function. 
The source file will be read line-by-line and scanned for tokens. The tokens are assumed to be contained within a single line, and not spreading over multiple lines. So multiline comments may not be detected, for example. The lines have a maximum size limitation, MAX_LINE_LEN, and the tokens have a maximum size length, MAX_TOKEN_LEN.

### Testing the lexical analyser

The program testLexer.c will use lexer.c to get tokens from the test file, testLexer.txt.
