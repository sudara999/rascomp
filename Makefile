.DEFAULT_GOAL = parser

# Set up the compiler and flags
CC ?= gcc
CFLAGS += -Isrc/include

# Find all the object files for the modules of the compiler

## function to get the object files from the directories
## get_objFdir(directory) -> object files in the directory
get_objFdir = $(patsubst %.c,%.o,$(shell find $(1) -name '*.c'))

## object files for modules
LEX_OBJ = $(call get_objFdir, src/lexer)
SYM_TAB_OBJ = $(call get_objFdir, src/symbol_table)
HASH_TAB_OBJ = $(call get_objFdir, src/util)
PARSER_OBJ = $(call get_objFdir, src/parser)

.PHONY: debug1
debug1:
	$(info $(LEX_OBJ))
	$(info $(DEPS))

# compile the source files into object files,
# 	also taking in to account the dependent header files
DEPS = $(shell find . -name '*.d')
DEPFLAGS = -MMD -MP

include $(DEPS)

%.o: %.c
	$(CC) $(CFLAGS) $(DEPFLAGS) -o $@ -c $<

# Create the tests for each module in the compiler

VPATH += build

.PHONY: tests
tests: test_symbol_table test_hash_table test_lexer test_parser

test_symbol_table: tests/symbol_table/testSymbolTable.o $(SYM_TAB_OBJ) $(HASH_TAB_OBJ) | build
	$(CC) $(CFLAGS) -o build/$@ $^

test_hash_table: tests/util/testHashtable.o $(HASH_TAB_OBJ) | build
	$(CC) $(CFLAGS) -o build/$@ $^

test_lexer: tests/lexer/testLexer.o $(LEX_OBJ) | build
	$(CC) $(CFLAGS) -o build/$@ $^

test_parser: tests/parser/testParser.o src/parser/parser.o $(LEX_OBJ) $(SYM_TAB_OBJ) $(HASH_TAB_OBJ) | build
	$(CC) $(CFLAGS) -o build/$@ $^

build:
	mkdir $@

# Create the parser

parser: $(PARSER_OBJ) $(LEX_OBJ) $(SYM_TAB_OBJ) $(HASH_TAB_OBJ) | build 
	$(CC) $(CFLAGS) -o build/$@ $^

# Clean the files created by make
.PHONY: clean
clean:
	-rm -r build
	find . -name '*.o' -delete
	find . -name '*.d' -delete

