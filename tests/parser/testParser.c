#include <parser.h>

DEF_NT(typespec)
	START_T(INT, "typespec -> INT.\n",
		END_P)
	START_T(FLOAT, "typespec -> FLOAT.\n",
		END_P)
END_NT

DEF_NT(start)
    START_NT(typespec, "start -> .typespec ID SEMICOLON DD\n", 
        NEXT_T(ID, "start -> typespec ID. SEMICOLON DD\n",
            NEXT_T(SEMICOLON, "start -> typespec ID SEMICOLON. DD\n",
                NEXT_T(NO_SYM, "start -> typespec ID SEMICOLON DD.\n",
                    END_P))))
	START_T(VOID, "start -> VOID. leftrecursion\n",
		START_NT(leftrecursion, "start -> VOID .leftrecursion\n",
			END_P))
END_NT

DEF_NT(leftrecursion)
	START_NT(leftrecursion, "leftrecursion -> .leftrecursion\n",
			END_P)
END_NT

