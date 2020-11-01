#ifndef LEX_TABLE
#define LEX_TABLE

#include <lexer.h>	// for enum token_type

#define TABLE_SRC "./src/lexer/lex_table.txt" 

int create_lex_table();
// Create the transition table for the DFA
// The table is created from the file TABLE_SRC

int delta (int current_state, char input_symbol);
// DFA Transition table function
// Returns the next state
// On no transitions, returns ERR_STATE, the error state

enum token_type accept (int current_state);
// DFA accept-states
// If the state is an accept-state, returns the token-type of the lexeme
// Otherwise returns NOT_ACC_STATE

#endif
