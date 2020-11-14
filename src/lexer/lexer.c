#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <compiler_api.h>
#include <lexer.h> // Look here for the lexer API
#include <lex_table.h> // lex_table.c will contain the DFA tables

// The textual representations of the token-types
static char *token_reps[] = {
	// OPERATORS
	"ASSIGN",	// =
	"LT",		// <
	"LE",		// <=
	"GT",		// >
	"GE",		// >=
	"EQ",		// ==
	"NE",		// !=
	"PLUS", 	// +
	"MINUS",	// -
	"MULT",		// *
	"DIV",		// /
	"NOT",		// !
	"AND",		// &&
	"OR",		// ||
	// PUNCTUATION
	"SEMICOLON",	// ;
	"LPAREN",	// (
	"RPAREN",	// )
	"COMMA",	// ,
	"LBRACE",	// {
	"RBRACE", 	// }
	"LBRACKET",	// [
	"RBRACKET",	// ]
	// KEYWORDS
	"Keyword:IF",		// if
	"Keyword:ELSE", 	// else
	"Keyword:WHILE", 	// while
	"Keyword:INT", 		// int
	"Keyword:FLOAT", 	// float
	"Keyword:VOID",		// void
	"Keyword:PRINT",	// print
	"Keyword:READ", 	// read
	"Keyword:FUNCTION",	// function
	"Keyword:RETURN",	// return
	// OTHER
	"ID:",		// <identifier>
	"ICONST:",	// <integer constant>
	"FCONST:",	// <Floating constant>
	"COMMENT"	// <comment>
};

static FILE *fp;
static char line_buffer[MAX_LINE_LEN] = "\0";
static int start_pos = 0;	// starting position of the lexeme on the line_buffer
static int end_pos = 0;		// ending position (char here is not included in the lexeme)	

// Function called after a token is detected.
// Can be used to change the default values of the token. 
static int onTokenDetect (struct token *token);

int initLexer (char *filename)
{
	// open the file for reading
	fp = fopen(filename, "r");
	if (fp == NULL){
		// file cannot be opened :(
		printf("The file %s could not be opened.\n", filename);
		return FAIL;
	}
	else
		// file opened :)
		return create_lex_table();	// create the DFA tables	
}

int getNextToken (struct token *token)
{
	int current_state 
		= START_STATE;			// Current state of DFA
	enum token_type is_accepted_state
		= NOT_ACC_STATE;		// Is the current state an accepted state;
						// if it is, what is the accepted token 
	enum token_type last_accepted_token
		= is_accepted_state;		// What was the last accepted token		
	int last_accepted_pos 
		= end_pos;			// Where was the last accepted token

	// if the file is not opened yet, or has been closed, tokens cannot be read.
	if (fp == NULL){
		token->token_type = NO_SYM;
		return FAIL;
	}
	
	// if characters in the line_buffer are exhausted
	// 	read a new line from the file into the buffer (faster than reading char-by-char)
	if (line_buffer[start_pos] == '\0') {
		// if the file is exhausted, close the file and return FAIL
		if (fgets(line_buffer, MAX_LINE_LEN, fp) == NULL) {
			fclose(fp);
			token->token_type = NO_SYM;
			snprintf(token->token_repr, MAX_LINE_LEN+1, "EOF");
			return FAIL;
		}
		// reinitialize position variables on the line_buffer
		else {
			// A token is assumed to be contained within a line
			// 	and not spread over multiple lines
			start_pos = 0;
			end_pos = 0;
		}
	}

	// Simulate the DFA
	// 	until there are no more transitions (i.e. until an error state is reached)
	// 	or until there are no more characters in the line
	do {
		current_state = delta(current_state, line_buffer[end_pos++]);
		is_accepted_state = accept(current_state);
		if (is_accepted_state != NOT_ACC_STATE){
			last_accepted_token = is_accepted_state;
			last_accepted_pos = end_pos;
		}
	} while (current_state != ERR_STATE && line_buffer[end_pos] != '\0');
	
	// Check if there was an accepted state
	// 	after the starting character was read
	// 	If there is,
	// 	set the end position of the lexeme to the position
	// 	where the accepted state was detected
	// If there are no accepted states, then FAIL is returned since an invalid
	// 	symbol was detected
	if (last_accepted_token == NOT_ACC_STATE){
		// return the invalid symbol
		token->token_type = INVALID_SYM;
		token->token_val.str_val[0] = line_buffer[start_pos];
		token->token_val.str_val[1] = '\0';
		// skip the invalid symbol in the next run	
		end_pos = ++start_pos;
		return FAIL;
	}
	end_pos = last_accepted_pos;

	// Return the accepted token
	token->token_type = last_accepted_token;	// token-type is assigned
	strncpy(token->token_val.str_val,		// default token-value
			&line_buffer[start_pos], 	// 	is the lexeme scanned
			end_pos - start_pos);
	token->token_val.str_val[end_pos - start_pos] = '\0';
	if ((int)(token->token_type) >= 0)
		strcpy(token->token_repr, 		// token-representation is assigned
			token_reps[token->token_type]);	
	start_pos = end_pos;		// set the start pos for the next run
	return onTokenDetect(token);	// This funtion is called since a token is detected
}	 

static int onTokenDetect (struct token *token)
{
	// This function is called when a token is detected. Any additional changes
	// 	can be made to the token.
	enum token_type token_type = token->token_type;	
	// I-tokens are special tokens that will be ignored
	// 	by returning the next token instead
	if (token_type == I_TOKEN)
		return getNextToken(token);
	// The textual representation of ID, ICONST or FCONST tokens should show the lexeme
	if (token_type == ID || token_type == ICONST || token_type == FCONST)
		strcat(token->token_repr, token->token_val.str_val);
	// ICONST token value should be changed to an integer that matches the lexeme
	if (token_type == ICONST)
		token->token_val.i_val = atoi(token->token_val.str_val);
	// FCONST token value should be changed to an float that matches the lexeme
	if (token_type == FCONST)
		token->token_val.f_val = atof(token->token_val.str_val);
	// Ignore comments
	if (token_type == COMMENT)
		return getNextToken(token);
	return SUCC;	
}

