#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <compiler_api.h>
#include <symbol_table.h>
#include <hash_tab.h>

#define HASH_SIZE 100	// The hash table size of the symbol table 
#define INSERT(sym) insert(symbol_table, (sym)->id, sym, HAS_DUP)
#define LOOKUP(id) get_map(symbol_table, id)

struct symbol *sym_int;
struct symbol *sym_float;
struct symbol *sym_array;

static int last_used_scope = GLOBAL_SCOPE;
static int curr_scope = GLOBAL_SCOPE;
static struct hash_tab *symbol_table;

static struct symbol *create_symbol(char *name, int scope)
{
	struct symbol *sym = malloc(sizeof(struct symbol));
	strcpy(sym->id, name);
	sym->scope = scope;
	sym->type = NULL;
	sym->mem_loc = NULL;
	sym->other = NULL;
	return sym;
}

static inline void create_data_type(struct symbol **sym, char *name)
{
	*sym = create_symbol(name, LANG_DEF);
	INSERT(*sym);
}

void initSymTab ()
{
	// Initialises the symbol table.
	// Also creates the language-defined data-types
	// 	as symbols in the table
	symbol_table = create_hash_tab(HASH_SIZE, string_hash, string_compare);	
	create_data_type(&sym_int, "int");
	create_data_type(&sym_float, "float");
	create_data_type(&sym_array, "$array");	
}

int newScope ()
{
	// Starts a new scope. All symbols added to the table will be defined in this scope.
	// Returns the id of the new scope as an integer.
	curr_scope = ++last_used_scope;
	return curr_scope;
}

void exitScope ()
{
	// Reverts the current scope to the global scope (0).
	curr_scope = GLOBAL_SCOPE;
}

int addSymbol (char *identifier)
{
	// Adds a symbol with the name identifier to the symbol-table. 
	// 	The scope of the added symbol can be found in the variable curr_scope.
	// Returns SUCC if added successfully, otherwise returns FAIL.
	struct symbol *sym, *other;
	other = getSymbol(identifier, curr_scope);
	if (other != NULL)
		if (other->scope == curr_scope || other->scope == LANG_DEF)
			return FAIL;
	sym = create_symbol(identifier, curr_scope);
	INSERT(sym);
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
	struct symbol *sym;
	if (symbolInTable(identifier, scope) == FAIL)
		return FAIL;
	sym = getSymbol(identifier, scope);
	switch (attribute) {
		case ATTR_SCOPE:
			sym->scope = *((int *)value);
			return SUCC;
		case ATTR_TYPE:
			sym->type = (struct symbol *)value;
			return SUCC;
		case ATTR_MEM_LOC:
			sym->mem_loc = value;
			return SUCC;
		case ATTR_OTHER:
			sym->other = value;
			return SUCC;
		default:
			return FAIL;
	}
}

int symbolInTable (char *identifier, int scope)
{
	// Verifies whether the symbol with the specified identifier is in the specified scope
	// Returns SUCC if the symbol exists, otherwise returns FAIL.
	struct symbol *sym = getSymbol(identifier, scope);
	if (sym != NULL)
		if (sym->scope == scope)
			return SUCC;
	return FAIL;
}

struct symbol *getSymbol (char *identifier, int scope)
{
	// Returns the symbol with the specified identifier at the scope specified.
	// If the symbol is not specified in the scope given,
	// 	then the symbol is checked for in the global scope 
	// Otherwise, returns NULL.
	struct symbol *sym, *glo_sym = NULL;
	struct hash_list *sym_list = LOOKUP(identifier);
	for (;sym_list != NULL; sym_list = sym_list->next)
		if (string_compare(identifier, sym_list->key) != 0)
			break;
		else {
			sym = sym_list->value;
			if (sym->scope == scope)
			       return sym;
			else if (sym->scope == GLOBAL_SCOPE)
				glo_sym = sym;	
		}
	return glo_sym;
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
