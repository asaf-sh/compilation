#include "tokens.hpp"
#include <stdio.h>
#include <map>
#include <string>


using namespace std;

map<int, const char*> tokennames = {{VOID, "VOID"}, {INT,"INT"},  {BYTE,"BYTE"}, {B,"B"}, {BOOL,"BOOL"}, {AND,"AND"}, {OR,"OR"}, {NOT,"NOT"}, {TRUE,"TRUE"}, {FALSE,"FALSE"}, {RETURN,"RETURN"}, {IF, "IF"}, {ELSE, "ELSE"}, {WHILE,"WHILE"}, {BREAK,"BREAK"}, {CONTINUE,"CONTINUE"}, {NUM, "NUM"}, {ID, "ID"}};


void showToken(const char* name)
{
	//printf("Lex found a %s, the lexeme is %s and its length is %d\n", name, yytext, yyleng);
	printf("%d %s %s\n", yylineno, name, yytext);
}

int main()
{
	int token;
	while ((token = yylex())) {
	  switch(token){
		//case VOID : showToken("VOID"); break;
		//case INT: showToken("INT"); break;
		//case NUM: showToken("NUM"); break;
		//case ID: showToken("ID"); break;
		case ERR: printf("error\n"); break;
		default: showToken(tokennames.find(token)->second);
		
	}
	}
	return 0;
}
