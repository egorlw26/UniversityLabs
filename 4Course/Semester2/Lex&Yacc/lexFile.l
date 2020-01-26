%{
#include <stdio.h>
void yyerror(char*);
#include "y.tab.h"

%}

%%
[1-9][0-9]*						{
										yylval = atoi(yytext);
										return INTEGER;
									}
[+-/*()\n]			 			return *yytext;
[ \t]							; /* skip white spaces*/

.									yyerror("invalid text");

%%

int yywrap(void) {
	return 1;
}