#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "lex_table.h"

#define NUM_OF_STATES 17	// Number of states, excluding the error state
#define	HASH_SIZE 11		// Hash-table size 
				//	- try to base around average number of transitions per state

#define MAX_CLASS_NAME_LEN 3	// Transitions can be made on character classes
#define NUMBER "NUM"		// Character class for numbers, [0-9]
#define LETTER "LET"		// Character class for letters, [a-zA-Z]

// Transition list-node
// 	The transition list-node maps a character or a character class to a new state
// 	The list-node also points to the next list-node
struct t_list {
	char input_symbol[MAX_CLASS_NAME_LEN+1];
	int next_state;
	struct t_list *next;
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
	FILE *fp;
	fp = fopen(TABLE_SRC, "r");
	if (fp == NULL){
		printf("The DFA table could not be found.\n");
		printf("Please create the table in %s.\n", TABLE_SRC);	
		return FAIL;
	}
	else{
		int current_state, next_state;
		char input_symbol[MAX_CLASS_NAME_LEN+1];
		while (fscanf(fp, "%d %s %d", 
			&current_state, input_symbol, &next_state) == 3)
				insert(current_state, input_symbol, next_state);
		
		insert(0, "=", 1);
		insert(1, "=", 2);
		insert(0, "\n", 3);	

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
	char symbol[MAX_CLASS_NAME_LEN+1];
	symbol[0] = input_symbol;
	symbol[1] = '\0';
	if (current_state < 0)	// The error state only has transitions to itself
		return ERR_STATE;
	if ((tl = lookup(current_state, symbol)) == NULL)
		return ERR_STATE;
	else
		return tl->next_state;	
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

