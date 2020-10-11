CC = gcc
CFLAGS = -I .
LEX_DEPS = lexer.h lex_table.h
LEX_OBJ = lexer.c lex_table.c testLexer.c

rascl_lexer: $(LEX_OBJ) $(DEPS)
	$(CC) $(CFLAGS) -o $@ $(LEX_OBJ)
