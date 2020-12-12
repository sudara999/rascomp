#include <parser.h>

DEF_NT(start)
    START_T(LPAREN, "start -> .LPAREN PLUS INTEGER INTEGER RPAREN\n", 
        NEXT_T(PLUS, "start -> LPAREN .PLUS INTEGER INTEGER RPAREN\n",
            NEXT_T(INTEGER, "start -> LPAREN PLUS .INTEGER INTEGER RPAREN\n",
                NEXT_T(INTEGER, "start -> LPAREN PLUS INTEGER .INTEGER RPAREN\n",
					NEXT_T(RPAREN, "start -> LPAREN PLUS INTEGER INTEGER .RPAREN\n",
	                    END_P)))))
	START_T(RPAREN, "start -> .RPAREN leftrecursion LPAREN\n",
		NEXT_NT(leftrecursion, "start -> RPAREN .leftrecursion LPAREN\n",
			END_P))
END_NT

DEF_NT(leftrecursion)
	START_NT(leftrecursion, "leftrecursion -> .leftrecursion\n",
			END_P)
END_NT