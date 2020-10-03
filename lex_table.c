#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "lex_table.h"

#define NUM_OF_STATES 17	// Number of states, excluding the error state
#define	HASH_SIZE 11		// Hash-table size 
				//	- try to base around average number of transitions per state

#define MAX_CLASS_NAME_LEN 3	// The maxmimum name length of a character class

// The following 2 macros were created to convert a macro argument to a string
#define XSTR(s) STR(s)	// Expand the macro arg and then covert to string
#define STR(s) #s	// Convert macro argument to string

// Transition list-node
// 	The transition list-node maps a character or a character class to a new state
// 	The list-node also points to the next list-node
struct t_list {
	char input_symbol[MAX_CLASS_NAME_LEN+1];
	int next_state;
	struct t_list *next;
};

// Class list-node
// 	A list of character-classes that match an input symbol can be stored
struct c_list {
	char class[MAX_CLASS_NAME_LEN+1];
	struct c_list *next;
};

// The Transition table
// 	Each state is mapped to hash-table that will map the input symbol to the next state
struct t_list *lex_table[NUM_OF_STATES][HASH_SIZE] = {NULL};

// The Accept table
// 	Denotes whether each state is an accept state or not
// 	If the state is an accept state, 
// 		the value of the table cell is the token the state represents
// 	If it is not an accept state,
// 		the value is NOT_ACC_STATE
static enum token_type accept_table[] = {
	NOT_ACC_STATE,
	ASSIGN,
	EQ,
	I_TOKEN
};

struct c_list *findClasses(char input_symbol);

// The hash function for the hash tables
int hash(char *s)
{
	int hash_val;
	for (hash_val = 0; *s != '\0'; s++)
		hash_val = hash_val * 31 + *s;
       	return hash_val % HASH_SIZE;
}

// Function to look for transition that matches the input symbol
struct t_list *lookup(int state, char *input_symbol)
{
	struct t_list *tl;
	for (tl = lex_table[state][hash(input_symbol)]; tl != NULL; tl = tl->next)
		if (strcmp(input_symbol, tl->input_symbol) == 0)
			return tl;
	return NULL;
}

// Function to insert a transition to the DFA transition table
struct t_list *insert(int current_state, char *input_symbol, int next_state)
{
	struct t_list *tl;
	int hash_val;
	if ((tl = lookup(current_state, input_symbol)) == NULL){
		tl = (struct t_list *) malloc(sizeof(struct t_list)); 			
		strcpy(tl->input_symbol, input_symbol);
		tl->next_state = next_state;
		hash_val = hash(input_symbol);
		tl->next = lex_table[current_state][hash_val];
		lex_table[current_state][hash_val] = tl;
		return tl;
	}
	else{
		printf("Warning: Multiple transitions on an input symbol are not allowed\n");
		printf("The transition from state %d to state %d on symbol %s will be discarded\n",
				current_state, next_state, input_symbol);
		return NULL;
	}
}


int create_lex_table()
{
	// Create the transition table for the DFA
	// The table is created from the file TABLE_SRC
	FILE *fp;
	fp = fopen(TABLE_SRC, "r");
	if (fp == NULL){
		printf("The DFA table could not be found.\n");
		printf("Please create the table in %s.\n", TABLE_SRC);	
		return FAIL;
	}
	else{
		int current_state, next_state, n;
		char input_symbol[MAX_CLASS_NAME_LEN+1];
		while ((n = fscanf(fp, "%d %" XSTR(MAX_CLASS_NAME_LEN) "s %d", 
			&current_state, input_symbol, &next_state)) != EOF)
				if (n == 3)	
					insert(current_state, input_symbol, next_state);
				else {
					printf("Error: Invalid format detected in %s.\n",
							TABLE_SRC);
					fclose(fp);
					return FAIL;
				}
		fclose(fp);
		return SUCC;
	}
}

int delta (int current_state, char input_symbol)
{
	// DFA Transition table function
	// Returns the next state
	// On no transitions, returns ERR_STATE, the error state
	struct t_list *tl;
	struct c_list *cl;	
	if (current_state < 0)	// The error state only has transitions to itself
		return ERR_STATE;	
	for (cl = findClasses(input_symbol);	// find all the classes the symbol belongs to 
		cl != NULL; cl = cl->next)
			// check if there is a transition for one of the classes
			if ((tl = lookup(current_state, cl->class)) != NULL) 
				return tl->next_state;	
	return ERR_STATE;
}

enum token_type accept (int current_state)	
{
	// DFA accept-states
	// If the state is an accept-state, returns the token-type of the lexeme
	// Otherwise returns NOT_ACC_STATE
	if (current_state < 0)
		return NOT_ACC_STATE;
	else
		return accept_table[current_state];
}


struct c_list *findClasses(char symbol)
{
	// Find the character-classes a symbol belongs to
	// Note: While multiple transitions on the same input symbol are not allowed,
	// 	the implementation of classes may allow an input symbol to have multiple
	// 	transitions if the symbol falls under multiple classes and the classes
	// 	have different transitions for a given state. Thus, the design of the 
	// 	DFA should avoid defining transitions for overlapping classes on a given
	// 	state.
	// 	But, if transitions are defined for overlapping classes, the transition is
	// 	selected based on which class is defined first in this function.
	 
	// The symbol belongs to the class of its own symbol representation
	struct c_list *curr_cl, *cl = (struct c_list *) malloc(sizeof(struct c_list));	
	curr_cl = cl;
	curr_cl->class[0] = symbol;
	curr_cl->class[1] = '\0';
	
	// Use the macro below to add more character classes that symbol can fall into
	#define ADD_CLASS(class_name) { \
		curr_cl->next = (struct c_list *) malloc(sizeof(struct c_list)); \
		curr_cl = curr_cl->next; \
		strcpy(curr_cl->class, class_name); \
	}

	// If the symbol is a letter it belongs to the LET class
	if (isalpha(symbol))
		ADD_CLASS("LET");
	// If the symbol is a digit it belongs to the NUM class
	if (isdigit(symbol))
		ADD_CLASS("NUM");
	// If the symbol is a \n it belongs to the "\\n" class
	// 	This helps the user type out \n
	if (symbol == '\n')
		ADD_CLASS("\\n");
	
	curr_cl->next = NULL;
	return cl;
}
