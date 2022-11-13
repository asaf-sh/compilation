#include "tokens.hpp"
#include <stdio.h>
#include <map>
#include <unordered_set>
#include <string>
#include <cstring>
#include <ctype>


using namespace std;

map<int, const char*> tokennames = {
{VOID, "VOID"},
{INT,"INT"},
{BYTE,"BYTE"},
{B,"B"},
{BOOL,"BOOL"},
{AND,"AND"}, 
{OR,"OR"}, 
{NOT,"NOT"}, 
{TRUE,"TRUE"}, 
{FALSE,"FALSE"}, 
{RETURN,"RETURN"}, 
{IF, "IF"}, 
{ELSE, "ELSE"}, 
{WHILE,"WHILE"}, 
{BREAK,"BREAK"}, 
{CONTINUE,"CONTINUE"},
{SC, "SC"},
{COMMA, "COMMA"},
{LPAREN, "LPAREN"},
{RPAREN, "RPAREN"},
{LBRACE, "LBRACE"},
{ASSIGN, "ASSIGN"},
{RELOP, "RELOP"},
{BINOP, "BINOP"},
{COMMENT, "COMMENT"},
{ID, "ID"},
{NUM, "NUM"}, 
{STRING, "STRING"}
};



char comment[3] = {'/', '/', '\0'};



char* string_slice(char* s, int from, int to){
	s = s+from;
	s[to>0 ? to : strlen(s) + to] = '\0';
	return s;
}

void handle_strings(char* dst, const char* src){
int j=0,i=1;
int len = strlen(src);
while(i<strlen(text)-1){
if (src[i]!='\\'){
	dst[j++] = src[i++]
	continue
}
else{
	
	switch (src[++i]){
		case x:
			bool valid = (len-i>3);if (!valid) return false;
			valid = valid && src[i+1]
			
			
			int ord = ((int)src[i+1])*10+(int)src[i+2];
			valid = valid && ord>=0x20 && ord<=7e
			int ord = (int)sr
			
	
	}
}
}
}
}

void showToken(const char* tokenname, char* value = NULL)
{
	//printf("Lex found a %s, the lexeme is %s and its length is %d\n", name, yytext, yyleng);
	printf("%d %s %s\n", yylineno, tokenname, value ? value : yytext);
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
		case WHITESPACE: break;
		case COMMENT: showToken(tokennames.find(token)->second, comment); break;
		case STRING: char s[1024]; 
			//strcpy(s, yytext);
			handle_strings(s, yytext); 
			showToken(tokennames.find(token)->second, s); break;
		default: showToken(tokennames.find(token)->second);
		
	}
	}
	return 0;
}
