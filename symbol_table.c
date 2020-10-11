#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "lexer.h"
#include "symbol_table.h"

static int last_used_scope;

void initSymTab ()
{
	// Initialises the symbol table.
	// Also creates the language-defined data-types
	// 	as symbols in the table
}

int newScope ()
{
	// Starts a new scope. All symbols added to the table will be defined in this scope.
	// Returns the id of the new scope as an integer.
	return 0;
}

void exitScope ()
{
	// Reverts the current scope to the global scope (0).
}

int addSymbol (char *identifier)
{
	// Adds a symbol with the name identifier to the symbol-table. 
	// 	The scope of the added symbol can be found in the variable `int last_used_scope`.
	// Returns SUCC if added successfully, otherwise returns FAIL.
	return SUCC;
}

int addAttributeToSymbol (char *identifier, int scope, int attribute, void *value)
{
	// Locates the symbol in the symbol-table 
	// 	with the requested identifier at the specified scope. 
	// 	Then the attribute is identified by the order it is defined in `struct symbol`. 
	// 	The attributes are zero-indexed from `int scope` member in `struct symbol`.
	// If the function successfully changes the attributes, then SUCC is returned. 
	// 	Otherwise FAIL is returned.
	return SUCC;
}

int symbolInTable (char *identifier, int scope)
{
	// Verifies whether the symbol with the specified identifier is in the specified scope
	// Returns SUCC if the symbol exists, otherwise returns FAIL.
	return SUCC;
}

struct symbol *getSymbol (char *identifier, int scope)
{
	// Returns the symbol with the specified identifier at the scope specified.
	// Otherwise, returns NULL.
	return NULL;
}

// Create an array_info struct and return a pointer to it
struct array_info *create_arr_info (struct symbol *type, int num_dim, ...)
{	
	va_list dims;
	int *dim_arr = malloc(num_dim*sizeof(int));
	struct array_info *arr_info = malloc(sizeof(struct array_info));
	va_start(dims, num_dim);
	for (int i = 0; i < num_dim; i++)
		dim_arr[i] = va_arg(dims, int);
	va_end(dims);
	arr_info->type = type;
	arr_info->num_dim = num_dim;
	arr_info->dims = dim_arr;
	return arr_info;
}
