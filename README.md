# CSE 304 Project

This is a simple compiler for the language ***rascl***, a small subset of C.

## Table of Contents

- [CSE 304 Project](#cse-304-project)
  * [Phase 1: Lexical Analysis](#phase-1-lexical-analysis)
    + [Lexical Analyser functions](#lexical-analyser-functions)
      - [int initLexer(char *filename);](#int-initlexerchar-filename)
      - [int getNextToken(struct token *nextToken);](#int-getnexttokenstruct-token-nexttoken)
    + [Lexical Analyser data structures](#lexical-analyser-data-structures)
      - [Tokens](#tokens)
    + [How the lexical analyser works](#how-the-lexical-analyser-works)
    + [Testing the lexical analyser](#testing-the-lexical-analyser)
    + [Future Features](#future-features)
  * [Phase 2: Symbol Table Manager](#phase-2-symbol-table-manager)
    + [Symbol Table functions](#symbol-table-functions)
      - [void initSymTab ();](#void-initsymtab)
      - [int newScope ();](#int-newscope)
      - [void exitScope ();](#void-exitscope)
      - [int addSymbol (char *identifier)](#int-addsymbol-char-identifier)
      - [int addAttributeToSymbol (char *identifier, int scope, int attribute, void *value);](#int-addattributetosymbol-char-identifier-int-scope-int-attribute-void-value)
      - [int symbolInTable (char *identifier, int scope);](#int-symbolintable-char-identifier-int-scope)
      - [struct symbol *getSymbol (char *identifier, int scope);](#struct-symbol-getsymbol-char-identifier-int-scope)
    + [Symbol Table data structures](#symbol-table-data-structures)
      - [Symbols](#symbols)
      - [The Symbol Table](#the-symbol-table)
    + [Testing the Symbol Table](#testing-the-symbol-table)

## Phase 1: Lexical Analysis

The lexical analyser is implemented in `lexer.c`.

### Lexical Analyser functions

The lexical analyser, `lexer.c`, contains the following the function calls:

#### int initLexer(char *filename);

This function will initialise the lexical analyser with the source file. 

Returns `SUCC` if the file is opened successfully; `FAIL` if not.

#### int getNextToken(struct token *nextToken);

This function will return the next token.

Returns `SUCC` on success.
If there are no tokens, `FAIL` will be returned. `nextToken`'s `token_type` field will be `NO_SYM`. 
If an invalid symbol is read, `FAIL` will be returned. The invalid symbol is stored in `nextToken`'s `token_val` field. The `token_type` field will be `INVALID_SYM`.

### Lexical Analyser data structures

#### Tokens

Tokens are stored as `struct token`.

```C
struct token {
	enum token_type token_type;
	union token_val token_val;
	char token_repr[max_token_length + 1];
};
```

They have the following members:

+ token_type: The type of the token (for example, the `IF` token)

+ token_val: The default value/type of a token is the lexeme that was scanned stored in a char array. To change the default value/type, the function `onTokenDetect` is used. 

+ token_repr: The textual representation of the token. The textual representation of the tokens are stored in an array, `char *token_reps[]`. To change the default textual representation, the function `onTokenDetect` is used.

Please refer to lexer.h for more details.

### How the lexical analyser works

Lexical Analysis will be done by table-driven scanning. The function `int delta (int current_state, char input_symbol)` defined in lex_table.c will return the next state based on the current state and the input symbol. If the current state is a final state, the function `enum token_type accept (int current_state)` defined in lex_table.c will return the token-type associated with the current state. The lexical analyser will simulate the DFA until there are no more transitions. Then the last accepted token-type and its associated lexeme is assigned to the token to be returned. The token can also be ignored (not returned) by returning the next possible token instead within the `onTokenDetect` function. 

The source file will be read line-by-line and scanned for tokens. The tokens are assumed to be contained within a single line, and not spreading over multiple lines. So multiline comments may not be detected, for example. The lines have a maximum size limitation, `MAX_LINE_LEN`, and the tokens have a maximum size limitation, `MAX_TOKEN_LEN`.

### Testing the lexical analyser

The program `testLexer.c` will use `lexer.c` to get tokens from the test file, `testLexer.txt`.

### Future Features

+ A function `destroy_lex_table()` to destroy the hash-table created in `lex_table.c`. This function will free up memory that is not being used.

## Phase 2: Symbol Table Manager

The symbol-table manager is implemented in `symbol_table.c`.

### Symbol Table functions

#### void initSymTab ();

Initialises the symbol table. Also creates the language-defined data-types (for example, `int`) as symbols in the table.

#### int newScope ();

Starts a new scope. All symbols added to the table after calling the function will be defined in this new scope. The global scope is defined as 0. Any new scopes created would have increasing ids from the global scope, following the order of creation.

Returns the id of the new scope as an integer.

#### void exitScope ();

Reverts the current scope to the global scope (0). 

#### int addSymbol (char *identifier)

Adds a symbol with the name identifier to the symbol-table. The scope of the symbol depends on the scope that is entered by calling `newScope()` or `exitScope()`. If there is a symbol with the same identifier in the scope, the symbol is not added and `FAIL` is returned.

Returns `SUCC` if added successfully; otherwise returns `FAIL`.

#### int addAttributeToSymbol (char *identifier, int scope, int attribute, void *value);

Locates the symbol in the symbol-table with the requested identifier at the specified scope. Then the attribute is identified by the order it is defined in `struct symbol` and given the value specified in the function call. The attributes are zero-indexed from `int scope` member in `struct symbol`.

If the function successfully changes the attributes, `SUCC` is returned. Otherwise `FAIL` is returned.

#### int symbolInTable (char *identifier, int scope);

Verifies whether the symbol with the specified identifier is in the specified scope within the symbol-table.

Returns `SUCC` if the symbol exists, otherwise returns `FAIL`.

#### struct symbol *getSymbol (char *identifier, int scope);

Returns the symbol with the specified identifier at the scope specified. If the symbol is not defined in the specified scope, it is searched in the global scope. Otherwise returns `NULL` if the symbol is not found.

### Symbol Table data structures

#### Symbols

Symbols are stored as `struct symbol`. 

```C
struct symbol {
	char id[MAX_TOKEN_LEN];
	int scope;
	struct symbol *type;
	void *mem_loc;
	void *other;
};
```

They have the following members:

+ id: The identifier of the symbol.
+ scope: Scope of the symbol.
+ type: The type of the symbol.
+ mem_loc: Memory location where the symbol is stored.
+ other: Other details about the symbol.

#### The Symbol Table

The symbol table is stored as a hash-table called `symbol_table`. The implementation of the hash-table can be found in the file `hash_tab.c`.

### Testing the Symbol Table

The program `testSymbolTable.c` will test the Symbol Table functions implemented in `symbol_table.c`. Run `make` in the root of the project directory to compile the symbol-table manager tester, `test_symbol_table`.