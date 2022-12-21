%{
//#include "tokens.hpp"
#include "hw3_output.hpp"
#include "parser.tab.hpp"
using namespace output;


%}


%option yylineno
%option noyywrap


lf		(\n)
cr		(\r)
tab		(\t)
space		(" ")



%%
void							return VOID;
int								return INT;
byte							return BYTE;
b								return B;
bool							return BOOL;
and								return AND;
or								return OR;
not								return NOT;
true							return TRUE;
false							return FALSE;
return							return RETURN;
if								return IF;
else							return ELSE;
while							return WHILE;
break							return BREAK;
continue						return CONTINUE;
;								return SC;
,								return COMMA;
\(								return LPAREN;
\)								return RPAREN;
\{								return LBRACE;
\}								return RBRACE;
=								return ASSIGN;
([=!<>]=)|[<>]					return RELOP;
[\+\-]							return ADDITIVE;
[\*\/]							return MULTIPLICATIVE;
[a-zA-Z][a-zA-Z0-9]*			return ID;
0|[1-9][0-9]*					return NUM;
\"([^\n\r\"\\]|\\[rnt"\\])+\"	return STRING;

\/\/[^\r\n]*[\r|\n|\r\n]?		; //ignor

({tab}|{space}|{lf}|{cr})		; //ignor

.								    errorLex(yylineno);
%%
