CC = gcc

rascl_lexer: lexer.c lex_table.c testLexer.c lex_table.txt testLexer.txt
	$(CC) -o rascl_lexer lexer.c lex_table.c testLexer.c
