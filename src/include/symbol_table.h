#ifndef SYM_TAB
#define SYM_TAB

#include <compiler_api.h>	// for SUCC and FAIL and MAX_TOKEN_LEN

// Scopes
#define LANG_DEF -1
#define GLOBAL_SCOPE 0

// Symbol Attributes are defined here
// 	according to the order they are defined in struct symbol
#define ATTR_SCOPE 0 	// scope
#define ATTR_TYPE 1	// type
#define ATTR_MEM_LOC 2	// memory location
#define ATTR_OTHER 3 	// other information related to the attribute

// Symbols for language defined data-types
struct symbol *sym_int;
struct symbol *sym_float;
struct symbol *sym_array;

// A symbol in the symbol table
struct symbol {
	char id[MAX_TOKEN_LEN];
	int scope;
	struct symbol *type;
	void *mem_loc;
	void *other;	// create a struct for additional details in heap because symbol persists for other phases of the compiler
					//	! and free when necessary)
};

// A structure to store information about an array object
struct array_info {
	struct symbol *type;
	int num_dim;
	int *dims;	// Pointer to array of integers of size num_dim containing the sizes of the dimensions
				// Create the array in heap because the symbol table persists for other phases of the compiler
				// ! Free the memory when necessary
};

// Functions below are described in the README

void initSymTab ();

int newScope ();

void exitScope ();

int addSymbol (char *identifier);

int addAttributeToSymbol (char *identifier, int scope, int attribute, void *value);

int symbolInTable (char *identifier, int scope);

struct symbol *getSymbol (char *identifier, int scope);

// Create an array_info struct and return a pointer to it
struct array_info *create_arr_info (struct symbol *type, int num_dim, ...);

#endif
