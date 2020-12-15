#define LAZY_PARSE

#include <parser.h>

extern struct token nextToken;

// Refered to https://www.scheme.com/tspl2d/grammar.html

DEF_NT(start)
    START_NT(form,
        END_P)
END_NT

DEF_NT(form)
    START_T(LPAREN,
        NEXT_NT(pform,
            NEXT_T(RPAREN,
                NEXT_NT(form,
                    END_P))))
    START_NT(s_expression,
        NEXT_NT(form,
            END_P))
    EPS_P
END_NT

DEF_NT(s_expression)
    START_NT(constant,
        END_P)
    START_NT(variable,
        END_P)
    START_T(QUOTE,
        NEXT_NT(datum,
            END_P))
END_NT

DEF_NT(constant)
    START_T(BOOLEAN,
        END_P)
    START_T(CHAR,
        END_P)
    START_T(STRING,
        END_P)
    START_NT(number,
        END_P)
END_NT

DEF_NT(variable)
    START_T(SYMBOL,
        END_P)
    START_NT(relational_op,
        END_P)
    START_NT(arith_op,
        END_P)
END_NT

DEF_NT(arith_op)
    START_T(PLUS,
        END_P)
    START_T(MINUS,
        END_P)
    START_T(MULT,
        END_P)
    START_T(DIV,
        END_P)
END_NT

DEF_NT(relational_op)
    START_T(EE,
        END_P)
    START_T(LT,
        END_P)
    START_T(LE,
        END_P)
    START_T(GT,
        END_P)
    START_T(GE,
        END_P)    
END_NT

DEF_NT(number)
    START_T(REAL,
        END_P)
    START_T(INTEGER,
        END_P)
END_NT

DEF_NT(datum)
    START_T(SYMBOL,
        END_P)
    START_T(BOOLEAN,
        END_P)
    START_T(CHAR,
        END_P)
    START_T(STRING,
        END_P)
    START_NT(number,
        END_P)
    START_NT(list,
        END_P)
END_NT

DEF_NT(list)
    START_T(LPAREN,
        NEXT_NT(datums,
            NEXT_T(RPAREN,
                END_P)))
END_NT

DEF_NT(datums)
    START_NT(datum,
        NEXT_NT(datums,
            END_P))
    EPS_P
END_NT

DEF_NT(pform)
    START_NT(pdefinition,
        END_P)
    START_NT(pexpression,
        END_P)
END_NT

DEF_NT(pdefinition)
    START_NT(pvariable_def,
        END_P)
END_NT

DEF_NT(pvariable_def)
    if (nextToken.token_type == SYMBOL){
        if (strcmp(nextToken.token_val.str_val, "define") == 0) {
            START_T(SYMBOL,
                NEXT_T(SYMBOL,
                    NEXT_NT(expression,
                        END_P)))
        }
    }
END_NT

DEF_NT(expression)
    START_T(LPAREN,
        NEXT_NT(pexpression,
            NEXT_T(RPAREN,
                END_P)))
    START_NT(s_expression,
        END_P)
END_NT

DEF_NT(pexpression)
    if (nextToken.token_type == SYMBOL){
        if (strcmp(nextToken.token_val.str_val, "lambda") == 0) {
            START_T(SYMBOL,
                NEXT_NT(formals,
                    NEXT_NT(body,
                        END_P)))
        }
    }
    if (nextToken.token_type == SYMBOL){
        if (strcmp(nextToken.token_val.str_val, "if") == 0) {
            START_T(SYMBOL,
                NEXT_NT(expression,
                    NEXT_NT(expression,
                        NEXT_NT(else_exp,
                            END_P))))
        }
    }
    START_NT(papplication,
        END_P)
END_NT

DEF_NT(else_exp)
    START_NT(expression,
        END_P)
    EPS_P
END_NT

DEF_NT(papplication)
    START_NT(expression,
        NEXT_NT(expressions,
            END_P))
END_NT

DEF_NT(expressions)
    START_NT(expression,
        NEXT_NT(expressions,
            END_P))
    EPS_P
END_NT

DEF_NT(formals)
    START_T(SYMBOL,
        END_P)
    START_T(LPAREN,
        NEXT_NT(args,
            NEXT_T(RPAREN,
                END_P)))
END_NT

DEF_NT(args)
    START_T(SYMBOL,
        NEXT_NT(args,
            END_P))
    EPS_P
END_NT

DEF_NT(body)
        START_NT(expression,
            NEXT_NT(expressions,
                END_P))
END_NT
