%{

#include "tokens.hpp"

%}



%option yylineno
%option noyywrap


nzdigit		([1-9])
digit		([0-9])
letter		([a-zA-Z])
lf		(\n)
cr		(\r)
tab		(\t)
space		(" ")



%%
void					return VOID;
int					return INT;
byte					return BYTE;
b					return B;
bool					return BOOL;
and					return AND;
or					return OR;
not					return NOT;
true					return TRUE;
false					return FALSE;
return					return RETURN;
if					return IF;
else					return ELSE;
while					return WHILE;
break					return BREAK;
continue				return CONTINUE;

;					return SC;
,					return COMMA;
\(					return LPAREN;
\)					return RPAREN;
\{					return LBRACE;
\}					return RBRACE;
=					return ASSIGN;
([=!<>]=)|[<>]				return RELOP;
[\+\-\*\/]				return BINOP;
\/\/[^\n\r]*				return COMMENT;



{letter}({digit}|{letter})*		return ID;
0|({nzdigit}{digit}*)			return NUM;
\"[\x20-\x7e]*([\x20-\x7e]{-}[\\])\"	        return STRING;

(\"[^\"\n]*\n)|(\"[^\"\n]*\\\"\n)				return UNCLOSED_STRING;

({tab}|{space}|{lf}|{cr})		return WHITESPACE;

.					return ERR;
%%
