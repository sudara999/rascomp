#ifndef PARSER
#define PARSER

#include <lexer.h>	// for enum token_type
#include <stdio.h>

#define PROGRESS 0
#define FALLBACK 1
#define ROLLBACK -1

#define PARSER_MAX_DEPTH 100	// prevent infinite recursion from bad grammars

#define ERR_NO_MATCH_MESSAGE "Syntax Error"
#define ERR_MAX_DEPTH_MESSAGE "Max depth reached"

// A log-list is used for logging the productions in the right order
struct log_list {
	char* log;
	int indent;
	struct log_list *next;
	struct log_list *prev;
};

// Always define this function as the starting symbol in your grammar 
int start (struct log_list *, int);

// Define a non-terminal by using this function template
#define DEF_NT(nt_name)\
	int nt_name (struct log_list *root_production, int depth) {\
		int status;\
		struct log_list log_list;\
		char nt_production[101];\
		log_list.log = nt_production;\
		log_list.indent = depth;\
		if (depth > PARSER_MAX_DEPTH) return depth_error(depth, root_production);

// End of definition of non-terminal
#define END_NT return FALLBACK; }

// Match the starting token of a production in a non-terminal
#define START_T(token, production, prod_tail)\
	if ((status = match_start(token, production, root_production, depth)) == PROGRESS) { prod_tail }\
	else if (status == ROLLBACK) return ROLLBACK; 

// Match a token of a production in a non-terminal
#define NEXT_T(token, production, prod_tail)\
	if ((status = match_next(token,production, depth)) == PROGRESS) { prod_tail }\
	else return ROLLBACK;

// Match a non-terminal of a production in a non-terminal
#define NEXT_NT(nt_name, production, prod_tail)\
	snprintf(nt_production, sizeof(nt_production), "%s", production);\
	log_list.next = &log_list;\
	log_list.prev = &log_list;\
	if ((status = nt_name(&log_list, depth+1)) == PROGRESS) { prod_tail }\
	else if (status == ROLLBACK) return ROLLBACK;\
	else return match_error(production);

// Match the starting non-terminal of a production in a non-terminal
#define START_NT(nt_name, production, prod_tail)\
	snprintf(nt_production, sizeof(nt_production), "%s", production);\
	log_list.next = root_production;\
	log_list.prev = root_production->prev;\
	root_production->prev->next = &log_list;\
	root_production->prev = &log_list;\
	if ((status = nt_name(root_production, depth+1)) == PROGRESS) { prod_tail }\
	else if (status == ROLLBACK) return ROLLBACK;\
	log_list.prev->next = root_production;\
	root_production->prev = log_list.prev;

// Finish matching a production in a non-terminal
#define END_P return PROGRESS;

// Match an epsilon production
#define EPS_P(production) return match_eps(production, root_production, depth);

// Check if the look-ahead is equal to the starting token of a production.
//     If so, log the productions that led to the current production/derivation.
//     Then log the current production. Afterwards, update the lookahead.
int match_start (enum token_type token, char *production, struct log_list *root_production, int depth);

// Check if the look-ahead is equal to a token found in a production.
//     If so, log the current production/derivation. Afterwards, update the lookahead.
int match_next (enum token_type token, char *production, int depth);

// An epsilon production is being matched.
//     Log the epsilon production after logging the productions that led to the current production.
int match_eps (char *production, struct log_list *root_production, int depth);

// Log an error message and prevent other productions on fallback during a rollback
int match_error(char *production);
int depth_error(int depth, struct log_list *parent_log);

#endif
