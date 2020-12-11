#include <parser.h>
#include <stdio.h>

// ## The declarations

DEF_NT(dvariablelisttail)
	START_T(COMMA, "dvariablelisttail -> COMMA. dvariable dvariablelisttail\n",
		NEXT_NT(dvariable, "dvariablelisttail -> COMMA .dvariable dvariablelisttail\n",
			NEXT_NT(dvariablelisttail, 
					"dvariablelisttail -> COMMA dvariable .dvariablelisttail\n",
				END_P)))
	START_T(SEMICOLON, "dvariablelisttail -> SEMICOLON.\n",
		END_P)
END_NT

DEF_NT(darraydim)
	START_T(LBRACKET, "darraydim -> LBRACKET. ICONST RBRACKET dvariabletail\n",
		NEXT_T(ICONST, "darraydim -> LBRACKET ICONST. RBRACKET dvariabletail\n",
			NEXT_T(RBRACKET, "darraydim -> LBRACKET ICONST RBRACKET. dvariabletail\n",
				NEXT_NT(dvariabletail, "darraydim -> LBRACKET ICONST RBRACKET .dvariabletail\n",
					END_P))))
END_NT

DEF_NT(dvariabletail)
	START_NT(darraydim, "dvariabletail -> .darraydim\n",
		END_P)
	EPS_P("dvariabletail -> e.\n")
END_NT

DEF_NT(dvariable)
	struct symbol *var;
	int num_dims = 1;
	START_T(ID, "dvariable -> ID. dvariabletail\n",
		{
		// Create a symbol table entry for the symbol
		//	with the label/ID read
		if (addSymbol(POP_ATTR(value).str_val) == FAIL){
			ERR_QUIT("ERROR: Could not add symbol to symbol table\n");
		}
		var = getSymbol(POP_ATTR(value).str_val, GLOBAL_SCOPE);
		}
		NEXT_NT(dvariabletail, "dvariable -> ID .dvariabletail\n",
			{
			// print the instructions for declaring the variable
			if (attr->type == sym_float){
				printf(".float, 0, %d, %s\n", num_dims, var);
			}
			else
				printf(".int, 0, %d, %s\n", num_dims, var);
			}
			END_P))
END_NT

DEF_NT(dvariablelist)
	START_NT(dvariable, "dvariablelist -> .dvariable dvariablelisttail\n",
		NEXT_NT(dvariablelisttail, "dvariablelist -> dvariable .dvariablelisttail\n",
			END_P))
END_NT

DEF_NT(typespec)
	START_T(INT, "typespec -> INT.\n",
		{attr->type = sym_int;}
		END_P)
	START_T(FLOAT, "typespec -> FLOAT.\n",
		{attr->type = sym_float;}
		END_P)
END_NT

DEF_NT(decl)
	START_NT(typespec, "decl -> .typespec dvariablelist\n",
		NEXT_NT(dvariablelist, "decl -> typespec .dvariablelist\n",
			END_P))
END_NT

DEF_NT(decllist)
	START_NT(decl, "decllist -> .decl decllist\n",
		NEXT_NT(decllist, "decllist -> decl .decllist\n",
			END_P))
	EPS_P("decllist -> e.\n")	
END_NT

// ### If

DEF_NT(iftail)
	START_T(ELSE, "iftail -> ELSE. compoundstatement\n", 
		NEXT_NT(compoundstatement, "iftail -> ELSE .compoundstatement\n", 
			END_P))
	EPS_P("iftail -> e.\n") 
END_NT

DEF_NT(ifstatement)
	START_T(IF, "ifstatement -> IF. LPAREN relationalexpr RPAREN compoundstatement iftail\n", 
		NEXT_T(LPAREN, "ifstatement -> IF LPAREN. relationalexpr RPAREN compoundstatement iftail\n",
			NEXT_NT(relationalexpr, "ifstatement -> IF LPAREN .relationalexpr RPAREN compoundstatement iftail\n",
				NEXT_T(RPAREN, "ifstatement -> IF LPAREN relationalexpr RPAREN. compoundstatement iftail\n",
					NEXT_NT(compoundstatement, "ifstatement -> IF LPAREN relationalexpr RPAREN .compoundstatement iftail\n",
						NEXT_NT(iftail, "ifstatement -> IF LPAREN relationalexpr RPAREN compoundstatement .iftail\n",
							END_P))))))
END_NT

// ### While

DEF_NT(compoundstatement)
	START_T(LBRACE, "compoundstatement -> LBRACE. statementlist RBRACE\n",
		NEXT_NT(statementlist, "compoundstatement -> LBRACE .statementlist RBRACE\n", 
			NEXT_T(RBRACE, "compoundstatement -> LBRACE statementlist RBRACE.\n",
				END_P)))
END_NT

DEF_NT(whiletail)
	START_NT(compoundstatement, "whiletail -> .compoundstatement\n",
		END_P)
END_NT

DEF_NT(condexprtail)
	START_T(LT, "condexprtail -> LT. otherexpression\n",
		NEXT_NT(otherexpression, "condexprtail -> LT .otherexpression\n",
			END_P))
	START_T(LE, "condexprtail -> LE. otherexpression\n",
		NEXT_NT(otherexpression, "condexprtail -> LE .otherexpression\n",
			END_P))
	START_T(GT, "condexprtail -> GT. otherexpression\n",
		NEXT_NT(otherexpression, "condexprtail -> GT .otherexpression\n",
			END_P))
	START_T(GE, "condexprtail -> GE. otherexpression\n",
		NEXT_NT(otherexpression, "condexprtail -> GE .otherexpression\n",
			END_P))
	START_T(EQ, "condexprtail -> EQ. otherexpression\n",
		NEXT_NT(otherexpression, "condexprtail -> EQ .otherexpression\n",
			END_P))
END_NT

DEF_NT(condexpr)
	START_T(LPAREN, "condexpr -> LPAREN. otherexpression condexprtail RPAREN\n",
		NEXT_NT(otherexpression, "condexpr -> LPAREN .otherexpression condexprtail RPAREN\n",
			NEXT_NT(condexprtail, "condexpr -> LPAREN otherexpression .condexprtail RPAREN\n",
				NEXT_T(RPAREN, "condexpr -> LPAREN otherexpression condexprtail RPAREN.\n",
					END_P))))
	START_T(NOT, "condexpr -> NOT. condexpr\n", 
		NEXT_NT(condexpr, "condexpr -> NOT .condexpr\n",
			END_P))
	START_NT(otherexpression, "condexpr -> .otherexpression condexprtail\n", 
		NEXT_NT(condexprtail, "condexpr -> otherexpression .condexprtail\n",
			END_P))
END_NT

DEF_NT(relationalexprtail)
	START_T(AND, "relationalexprtail -> AND. condexpr\n",
		NEXT_NT(condexpr, "relationalexprtail -> AND .condexpr\n", 
			END_P))
	START_T(OR, "relationalexprtail -> OR. condexpr\n",
		NEXT_NT(condexpr, "relationalexprtail -> OR .condexpr\n", 
			END_P))
	EPS_P("relationalexprtail -> e.\n")
END_NT

DEF_NT(relationalexpr)
	START_NT(condexpr, "relationalexpr -> .condexpr relationalexprtail\n",
		NEXT_NT(relationalexprtail, "relationalexpr -> condexpr .relationalexprtail\n",
			END_P))
END_NT

DEF_NT(whilestatement)
	START_T(WHILE, "whilestatement -> WHILE. LPAREN relationalexpr RPAREN whiletail\n",
		NEXT_T(LPAREN, "whilestatement -> WHILE LPAREN. relationalexpr RPAREN whiletail\n",
			NEXT_NT(relationalexpr, "whilestatement -> WHILE LPAREN .relationalexpr RPAREN whiletail\n",
				NEXT_T(RPAREN, "whilestatement -> WHILE LPAREN relationalexpr RPAREN. whiletail\n",
					NEXT_NT(whiletail, "whilestatement -> WHILE LPAREN relationalexpr RPAREN .whiletail\n",
						END_P)))))
END_NT

// ### variables

DEF_NT(arraydimtail)
	START_T(ID, "arraydimtail -> ID. RBRACKET arraydim\n",
		NEXT_T(RBRACKET, "arraydimtail -> ID RBRACKET. arraydim\n",
			NEXT_NT(arraydim, "arraydimtail -> ID RBRACKET .arraydim\n",
				END_P)))
	START_T(ICONST, "arraydimtail -> ICONST. RBRACKET arraydim\n",
		NEXT_T(RBRACKET, "arraydimtail -> ICONST RBRACKET. arraydim\n",
			NEXT_NT(arraydim, "arraydimtail -> ICONST RBRACKET .arraydim\n",
				END_P)))
END_NT

DEF_NT(arraydim)
	START_T(LBRACKET, "arraydim -> LBRACKET. arraydimtail\n",
		NEXT_NT(arraydimtail, "arraydim -> LBRACKET .arraydimtail\n",
			END_P))
	EPS_P("arraydim -> e.\n")
END_NT

DEF_NT(variabletail)
	START_NT(arraydim, "variabletail -> .arraydim\n",
		END_P)
END_NT

DEF_NT(variable)
	START_T(ID, "variable -> ID. variabletail\n",
		NEXT_NT(variabletail, "variable -> ID .variabletail\n",
			END_P))
END_NT

// ### Assignment

DEF_NT(arglistrem)
	START_T(COMMA, "arglistrem -> COMMA. arg arglistrem\n",
		NEXT_NT(arg, "arglistrem -> COMMA .arg arglistrem\n", 
			NEXT_NT(arglistrem, "arglistrem -> COMMA arg .arglistrem\n",
				END_P)))
	EPS_P("arglistrem -> e.\n")
END_NT

DEF_NT(arg)
	START_NT(otherexpression, "arg -> .otherexpression\n",
		END_P)
END_NT

DEF_NT(arglist)
	START_NT(arg, "arglist -> .arg arglistrem\n",
		NEXT_NT(arglistrem, "arglist -> arg .arglistrem\n",
			END_P))
	EPS_P("arglist -> e.\n")
END_NT

DEF_NT(funccalltail)
	START_T(LPAREN, "funccalltail -> LPAREN. arglist RPAREN\n",
		NEXT_NT(arglist, "funccalltail -> LPAREN .arglist RPAREN\n",
			NEXT_T(RPAREN, "funccalltail -> LPAREN arglist RPAREN.\n",
				END_P)))
END_NT

DEF_NT(factortail)
	START_NT(funccalltail, "factortail -> .funccalltail\n",
		END_P)
	START_NT(variabletail, "factortail -> .variabletail\n",
		END_P)
END_NT

DEF_NT(factor)
	START_T(ICONST, "factor -> ICONST.\n",
		END_P)
	START_T(FCONST, "factor -> FCONST.\n",
		END_P)
	START_T(LPAREN, "factor -> LPAREN. otherexpression RPAREN\n",
		NEXT_NT(otherexpression, "factor -> LPAREN .otherexpression RPAREN\n",
			NEXT_T(RPAREN, "factor -> LPAREN otherexpression RPAREN.\n",
				END_P)))
	START_T(MINUS, "factor -> MINUS. factor\n",
		NEXT_NT(factor, "factor -> MINUS .factor\n",
			END_P))
	START_T(ID, "factor -> ID. factortail\n",
		NEXT_NT(factortail, "factor -> ID .factortail\n",
			END_P))
END_NT

DEF_NT(termtail)
	START_T(MULT, "termtail -> MULT. factor termtail\n",
		NEXT_NT(factor, "termtail -> MULT .factor termtail\n",
			NEXT_NT(termtail, "termtail -> MULT factor .termtail\n",
				END_P)))
	START_T(DIV, "termtail -> DIV. factor termtail\n",
		NEXT_NT(factor, "termtail -> DIV .factor termtail\n",
			NEXT_NT(termtail, "termtail -> DIV factor .termtail\n",
				END_P)))
	EPS_P("termtail -> e.\n")
END_NT

DEF_NT(term)
	START_NT(factor, "term -> .factor termtail\n",
		NEXT_NT(termtail, "term -> factor .termtail\n",
			END_P))
END_NT

DEF_NT(otherexpressiontail)
	START_T(PLUS, "otherexpressiontail -> PLUS. term otherexpressiontail\n",
		NEXT_NT(term, "otherexpressiontail -> PLUS .term otherexpressiontail\n",
			NEXT_NT(otherexpressiontail, "otherexpressiontail -> PLUS term .otherexpressiontail\n",
				END_P)))
	START_T(MINUS, "otherexpressiontail -> MINUS. term otherexpressiontail\n",
		NEXT_NT(term, "otherexpressiontail -> MINUS .term otherexpressiontail\n",
			NEXT_NT(otherexpressiontail, "otherexpressiontail -> MINUS term .otherexpressiontail\n",
				END_P)))
	EPS_P("otherexpressiontail => e.\n")
END_NT

DEF_NT(otherexpression)
	START_NT(term, "otherexpression -> .term otherexpressiontail\n",
		NEXT_NT(otherexpressiontail, "otherexpression -> term .otherexpressiontail\n",
			END_P))
END_NT

DEF_NT(assignmentexpression)
	START_NT(variable, "assignmentexpression -> .variable ASSIGN otherexpression\n",
		NEXT_T(ASSIGN, "assignmentexpression -> variable ASSIGN. otherexpression\n",
			NEXT_NT(otherexpression, "assignmentexpression -> variable ASSIGN .otherexpression\n",
				END_P)))
END_NT

// ### Print and read

DEF_NT(readexpression)
	START_T(READ, "readexpression -> READ. variable\n",
		NEXT_NT(variable, "readexpression -> READ .variable\n",
			END_P))
END_NT

DEF_NT(printexpression)
	START_T(PRINT, "printexpression -> PRINT. variable\n",
		NEXT_NT(variable, "printexpression -> PRINT .variable\n",
			END_P))
END_NT

DEF_NT(printstatement)
	START_T(PRINT, "printstatement -> PRINT. otherexpression\n",
		NEXT_NT(otherexpression, "printstatement -> PRINT .otherexpression\n",
			END_P))
END_NT

// ## The statements

DEF_NT(statementtail)
	START_T(SEMICOLON, "statementtail -> SEMICOLON.\n",
		END_P)
	EPS_P("statementtail -> e.\n")
END_NT

DEF_NT(statement)
	START_NT(whilestatement, "statement -> .whilestatement statementtail\n",
		NEXT_NT(statementtail, "statement -> whilestatement .statementtail\n",
			END_P))
	START_NT(ifstatement, "statement -> .ifstatement statementtail\n",
		NEXT_NT(statementtail, "statement -> ifstatement .statementtail\n",
			END_P))
	START_NT(assignmentexpression, "statement -> .assignmentexpression SEMICOLON\n",
		NEXT_T(SEMICOLON, "statement -> assignmentexpression SEMICOLON.\n",
			END_P))
	START_NT(printstatement, "statement -> .printstatement SEMICOLON\n",
		NEXT_T(SEMICOLON, "statement -> printstatement SEMICOLON.\n",
			END_P))
	START_NT(readexpression, "statement -> .readexpression SEMICOLON\n",
		NEXT_T(SEMICOLON, "statement -> readexpression SEMICOLON.\n",
			END_P))
END_NT

DEF_NT(statementlist)
	START_NT(statement, "statementlist -> .statement statementlist\n",
			NEXT_NT(statementlist, "statementlist -> statement .statementlist\n",
				END_P))
	EPS_P("statementlist -> e.\n")
END_NT

DEF_NT(bstatementlist)
	START_T(LBRACE, "bstatementlist -> LBRACE. statementlist RBRACE\n",
		NEXT_NT(statementlist, "bstatementlist -> LBRACE .statementlist RBRACE\n",
			NEXT_T(RBRACE, "bstatementlist -> LBRACE statementlist RBRACE.\n",
				END_P)))
END_NT

// ### Function statements

DEF_NT(fiftail)
	START_T(ELSE, "fiftail -> ELSE. fbstatementlist\n", 
		NEXT_NT(fbstatementlist, "fiftail -> ELSE .fbstatementlist\n", 
			END_P))
	EPS_P("fiftail -> e.\n")
END_NT

DEF_NT(fifstatement)
	START_T(IF, "fifstatement -> IF. LPAREN relationalexpr RPAREN fbstatementlist fiftail\n", 
		NEXT_T(LPAREN, "fifstatement -> IF. LPAREN relationalexpr RPAREN fbstatementlist fiftail\n",
			NEXT_NT(relationalexpr, "fifstatement -> IF. LPAREN relationalexpr RPAREN fbstatementlist fiftail\n",
				NEXT_T(RPAREN, "fifstatement -> IF. LPAREN relationalexpr RPAREN fbstatementlist fiftail\n",
					NEXT_NT(fbstatementlist, "fifstatement -> IF. LPAREN relationalexpr RPAREN fbstatementlist fiftail\n",
						NEXT_NT(fiftail, "fifstatement -> IF. LPAREN relationalexpr RPAREN fbstatementlist fiftail\n",
							END_P))))))
END_NT

DEF_NT(fwhilestatement)
	START_T(WHILE, "fwhilestatement -> WHILE. LPAREN relationalexpr RPAREN fbstatementlist\n",
		NEXT_T(LPAREN, "fwhilestatement -> WHILE LPAREN. relationalexpr RPAREN fbstatementlist\n",
			NEXT_NT(relationalexpr, "fwhilestatement -> WHILE LPAREN .relationalexpr RPAREN fbstatementlist\n",
				NEXT_T(RPAREN, "fwhilestatement -> WHILE LPAREN relationalexpr RPAREN. fbstatementlist\n",
					NEXT_NT(fbstatementlist, "fwhilestatement -> WHILE LPAREN relationalexpr RPAREN .fbstatementlist\n",
						END_P)))))
END_NT

DEF_NT(returnexpression)
	START_NT(otherexpression, "returnexpression -> .otherexpression\n", END_P)
	EPS_P("returnexpression => e.\n")
END_NT

DEF_NT(returnstatement)
	START_T(RETURN, "returnstatement -> RETURN. returnexpression SEMICOLON\n",
		NEXT_NT(returnexpression, "returnstatement -> RETURN .returnexpression SEMICOLON\n", 
			NEXT_T(SEMICOLON, "returnstatement -> RETURN returnexpression SEMICOLON.\n",
				END_P)))
END_NT

DEF_NT(fstatement)
	START_NT(returnstatement, "fstatement -> .returnstatement\n", END_P)
	START_NT(fwhilestatement, "fstatement -> .fwhilestatement statementtail\n", 
		NEXT_NT(statementtail, "fstatement -> fwhilestatement .statementtail\n",
			END_P))
	START_NT(fifstatement, "fstatement -> .fifstatement statementtail\n",
		NEXT_NT(statementtail,"fstatement -> .fifstatement statementtail\n",
			END_P))
	START_NT(assignmentexpression, "fstatement -> .assignmentexpression SEMICOLON\n",
		NEXT_T(SEMICOLON, "fstatement -> assignmentexpression SEMICOLON.\n",
			END_P))
	START_NT(printstatement, "fstatement -> .printstatement SEMICOLON\n",
		NEXT_T(SEMICOLON, "fstatement -> printstatement SEMICOLON.\n",
			END_P))
	START_NT(decl, "fstatement -> decl\n", END_P)
END_NT

DEF_NT(fstatementlist)
	START_NT(fstatement, "fstatementlist -> .fstatement fstatementlist\n", 
		NEXT_NT(fstatementlist, "fstatementlist -> fstatement .fstatementlist\n",
			END_P))
	EPS_P("fstatementlist -> e.\n")
END_NT

DEF_NT(fbstatementlist)
	START_T(LBRACE, "fbstatementlist -> LBRACE. fstatementlist RBRACE\n", 
		NEXT_NT(fstatementlist, "fbstatementlist -> LBRACE .fstatementlist RBRACE\n", 
			NEXT_T(RBRACE, "fbstatementlist -> LBRACE fstatementlist RBRACE.\n", 
				END_P)))
END_NT

// ## Functions

DEF_NT(fparmlistrem)
	START_T(COMMA, "fparmlistrem -> COMMA. fparm fparmlistrem\n",
		NEXT_NT(fparm, "fparmlistrem -> COMMA .fparm fparmlistrem\n",
			NEXT_NT(fparmlistrem, "fparmlistrem -> COMMA fparm .fparmlistrem\n",
				END_P)))
	EPS_P("fparmlistrem -> e.\n")
END_NT

DEF_NT(parmVarTail)
	START_T(LBRACKET, "parmVarTail -> LBRACKET. RBRACKET parmVarTail\n",
		NEXT_T(RBRACKET, "parmVarTail -> LBRACKET RBRACKET. parmVarTail\n",
			NEXT_NT(parmVarTail, "parmVarTail -> LBRACKET RBRACKET .parmVarTail\n",
				END_P)))
	EPS_P("parmVarTail -> e.\n")
END_NT

DEF_NT(parmVar)
	START_T(ID, "parmVar -> ID. parmVarTail\n",
		NEXT_NT(parmVarTail, "parmVar -> ID .parmVarTail\n",
			END_P))
END_NT

DEF_NT(fparm)
	START_NT(typespec, "fparm -> .typespec parmVar\n",
		NEXT_NT(parmVar, "fparm -> typespec .parmVar\n",
			END_P))
END_NT

DEF_NT(fparmlist)
	START_NT(fparm,"fparmlist -> .fparm fparmlistrem\n",
		NEXT_NT(fparmlistrem,"fparmlist -> fparm .fparmlistrem\n",
			END_P))
	EPS_P("fparmlist -> e.\n")
END_NT

DEF_NT(fdeclparms)
	START_T(LPAREN, "fdeclparms -> LPAREN. fparmlist RPAREN\n",
		NEXT_NT(fparmlist, "fdeclparms -> LPAREN .fparmlist RPAREN\n",
			NEXT_T(RPAREN, "fdeclparms -> LPAREN fparmlist RPAREN.\n",
				END_P)))
END_NT

DEF_NT(simplevar)
	START_T(ID, "simplevar -> ID.\n",
		END_P)
END_NT

DEF_NT(ftypespec)
	START_T(VOID, "ftypespec -> VOID.\n",
		END_P)
	START_T(INT, "ftypespec -> INT.\n",
		END_P)
	START_T(FLOAT, "ftypespec -> FLOAT.\n",
		END_P)
END_NT

DEF_NT(funcdecl)
	START_T(FUNCTION, "funcdecl -> FUNCTION. ftypespec simplevar fdeclparms decllist fbstatementlist\n",
		NEXT_NT(ftypespec, "funcdecl -> FUNCTION .ftypespec simplevar fdeclparms decllist fbstatementlist\n",
			NEXT_NT(simplevar, "funcdecl -> FUNCTION ftypespec .simplevar fdeclparms decllist fbstatementlist\n",
				NEXT_NT(fdeclparms, "funcdecl -> FUNCTION ftypespec simplevar .fdeclparms decllist fbstatementlist\n",
					NEXT_NT(decllist, "funcdecl -> FUNCTION ftypespec simplevar fdeclparms .decllist fbstatementlist\n",
						NEXT_NT(fbstatementlist, "funcdecl -> FUNCTION ftypespec simplevar fdeclparms decllist .fbstatementlist\n",
							END_P))))))
END_NT

DEF_NT(funcdecls)
	START_NT(funcdecl, "funcdecls -> .funcdecl funcdecls\n",
			NEXT_NT(funcdecls, "funcdecls -> funcdecl .funcdecls\n",
				END_P))
	EPS_P("funcdecls -> e.\n")
END_NT

// ## The program

DEF_NT(program)
	{printf(".segment, 0, 0, .data\n");}
	START_NT(decllist, "program -> .decllist funcdecls bstatementlist DD\n",
		NEXT_NT(funcdecls, "program -> decllist .funcdecls bstatementlist DD\n",
			{printf("\n.segment, 0, 0, .text\n");}
			NEXT_NT(bstatementlist, "program -> decllist funcdecls .bstatementlist DD\n",
				NEXT_T(NO_SYM, "program -> decllist funcdecls bstatementlist DD.\n",
					END_P))))
END_NT

DEF_NT(start)
	START_NT(program, "START -> .program\n",
			END_P)
END_NT
