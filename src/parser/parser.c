#include <stdio.h>
#include <stdlib.h>
#include <compiler_api.h>
#include <lexer.h>
#include <parser.h>

FILE *logfile;
struct token nextToken;
struct log_list parse_log;

// Match errors happen when a non-terminal (NT) which is not at the start of a production
// 	fails to match the next token. An error should be returned to the non-terminal that led to the
// 	current production and the error should propagate to the root of the parse tree.
int match_error(char *production)
{
	// Print an error log
	fprintf(logfile, "ERROR: %s\n", ERR_NO_MATCH_MESSAGE);
	fprintf(logfile, "\tExpected production: %s", production);
	fprintf(logfile, "\tLookahead: %s\n", nextToken.token_repr);
	return ROLLBACK;
}

int depth_error(int depth, struct log_list *parent_log){
	// Print an error log
	fprintf(logfile, "%*c %s", 4*parent_log->indent, '|', parent_log->log);
	fprintf(logfile, "ERROR: %s\n", ERR_MAX_DEPTH_MESSAGE);
	fprintf(logfile, "\tError reported at depth: %d\n", depth);
	fprintf(logfile, "\tMaximum depth: %d\n", PARSER_MAX_DEPTH);
	fprintf(logfile, "\tLast Lookahead: %s\n", nextToken.token_repr);
	fprintf(logfile, "\tLast production: %s", parent_log->prev->log);
	return ROLLBACK;
}

int invalid_sym_error()
{
	fprintf(logfile, "ERROR: Invalid Symbol read\n");
	fprintf(logfile, "\tInvalid symbol: %s\n", nextToken.token_val.str_val);
	return ROLLBACK;
}

int parse ()
{
	int status;
	getNextToken(&nextToken);
	parse_log.log = "PARSE() -> .START\n";
	parse_log.indent = 0;
	parse_log.next = &parse_log;
	parse_log.prev = &parse_log;
	if ((status = start(&parse_log, 1)) == PROGRESS)
		return SUCC;
	else if (status == FALLBACK)
		match_error(parse_log.log);
	return FAIL;
}

int main (int argc, char *argv[])
{
	char logfile_path[100];
	int status = FAIL;
	// a file to parse must be provided
	if (argc != 2)
		return FAIL;
	// initialise the lexer
	if (initLexer(argv[1]) == FAIL)
		return FAIL;
	// create the logfile
	snprintf(logfile_path, sizeof(logfile_path), "%s%s", argv[1], ".log");
	logfile = fopen(logfile_path, "w");
	// parse the source file provided
	status = parse();
	// return the status and log
	fclose(logfile);
	return status;
}

// Check if the look-ahead is equal to the starting token of a production.
//     If so, log the productions that led to the current production/derivation.
//     Then log the current production. Afterwards, update the lookahead.
int match_start (enum token_type token, char *production, struct log_list *root_production, int depth)
{
	if (token == nextToken.token_type) {
		match_eps(production, root_production, depth);
		// print the token-value of the token matched
		fprintf(logfile, "%*c %s\n", 4*depth, '*', nextToken.token_repr);
		// update the current token
		getNextToken(&nextToken);
		return PROGRESS;
	}
	if (nextToken.token_type == INVALID_SYM)
		return invalid_sym_error();		
	return FALLBACK;
}

// Check if the look-ahead is equal to a token found in a production.
//     If so, log the current production/derivation. Afterwards, update the lookahead.
int match_next (enum token_type token, char *production, int depth)
{
	if (token == nextToken.token_type) {
		// print the current production
		fprintf(logfile, "%*c %s", 4*depth, '|', production);
		// print the token matched
		fprintf(logfile, "%*c %s\n", 4*depth, '*', nextToken.token_repr);
		// update the current token
		getNextToken(&nextToken);
		return PROGRESS;
	}		
	return match_error(production);
}

// An epsilon production is being matched.
//     Log the epsilon production after logging the productions that led to the current production.
int match_eps (char *production, struct log_list *root_production, int depth)
{
	// Log the previous productions
	struct log_list *prod_list;
	fprintf(logfile, "%*c %s", 4*root_production->indent, '|', root_production->log);
	prod_list = root_production->next;
	while (prod_list != root_production){
		fprintf(logfile, "%*c %s", 4*prod_list->indent, '|', prod_list->log);
		prod_list = prod_list->next;
	}
	// print the current production
	fprintf(logfile, "%*c %s", 4*depth, '|', production);
	return PROGRESS;
}
