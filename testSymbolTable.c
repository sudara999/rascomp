#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"	// For SUCC and FAIL
#include "symbol_table.h"

#define CHKFAILQUIT(cond) if (cond == FAIL) \
		printf("ERROR: Function call returned FAIL in line number %d\n", __LINE__)  

#define SYM_INT (void *) sym_int
#define SYM_FLOAT (void *) sym_float
#define SYM_ARRAY (void *) sym_array

void check_symbol (char *id, int scope) {
	if (symbolInTable(id, scope) == SUCC)
		printf("Symbol %s is in scope %d.\n", id, scope);
	else
		printf("Symbol %s in scope %d was not found in table.\n", id, scope);
}

void print_attr (char * identifier, int scope) {
	struct symbol *sym;
	if ((sym = getSymbol(identifier, scope)) == NULL)
	       printf("The symbol %s in scope %d was not found.\n",
			       identifier, scope);	
	printf("Symbol name: %s\n\tscope: %d\n\ttype: %s\n\tmemory-location: %p\n",
			sym->id, sym->scope, sym->type->id, sym->mem_loc);
}

int main(){
	// Initialise the symbol table
	initSymTab();
	// Add 3 global variables
	CHKFAILQUIT(addSymbol("temperature"));
	CHKFAILQUIT(addSymbol("velocity"));
	CHKFAILQUIT(addSymbol("temp"));
	// Add type:int to variables temperature and velocity
	CHKFAILQUIT(addAttributeToSymbol("temperature", GLOBAL, ATTR_TYPE, SYM_INT));	
	CHKFAILQUIT(addAttributeToSymbol("velocity", GLOBAL, ATTR_TYPE, SYM_INT));	
	// Enter a new scope
	int new_scope = newScope();
	printf("The id of the new scope is %d\n.", new_scope);
	// Add 2 symbols
	CHKFAILQUIT(addSymbol("velocity"));
	CHKFAILQUIT(addSymbol("position"));
	// Add attributes to the symbols
	CHKFAILQUIT(addAttributeToSymbol("velocity", new_scope, ATTR_TYPE, SYM_FLOAT));	
	CHKFAILQUIT(addAttributeToSymbol("position", new_scope, ATTR_TYPE, SYM_INT));
	// Check if symbols are in the table
	check_symbol("temperature", GLOBAL);
	check_symbol("bang", GLOBAL);
	// Retrieve a symbol from the table and print its type
	struct symbol *sym = getSymbol("position", new_scope);
	printf("The type of the symbol 'position' is %s.\n", sym->type->id);
	// Go back to global scope
	exitScope();	
	// Add attributes to symbols
	// 	Add memory locations to a symbol
	CHKFAILQUIT(addAttributeToSymbol("temperature", GLOBAL,
				ATTR_MEM_LOC, (void *)0x800000));	
	CHKFAILQUIT(addAttributeToSymbol("velocity", GLOBAL,
				ATTR_MEM_LOC, (void *)0x800020));	
	CHKFAILQUIT(addAttributeToSymbol("temp", GLOBAL,
				ATTR_MEM_LOC, (void *)0x800040));
	//	Define 'temp' as an int[16][11] array
	struct array_info *temp_arr = create_arr_info(SYM_INT, 2, 16, 11);
	CHKFAILQUIT(addAttributeToSymbol("temp", GLOBAL,
				ATTR_TYPE, SYM_ARRAY));	
	CHKFAILQUIT(addAttributeToSymbol("temp", GLOBAL,
				ATTR_OTHER, temp_arr));	
	// Print the attributes of symbols
	print_attr("temperature", GLOBAL);
       	print_attr("velocity", new_scope);
       	print_attr("position", new_scope);
       	print_attr("velocity", GLOBAL);
       	print_attr("bang", GLOBAL);
	return 0;
}
