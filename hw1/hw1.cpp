#include "tokens.hpp"
#include <stdio.h>
#include <map>
#include <unordered_set>
#include <string>
#include <cstring>
#include <ctype.h>


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

bool handle_strings(char* dst, const char* src){
int j=0,i=1;
int len = strlen(src);
while(i<len-1){
	if (src[i]!='\\'){
		dst[j++] = src[i];
	}
	else{
	
		switch (src[++i]){
			case 'x':
				//bool valid = (len-i>3);
				if(len-i<4) return false;
				try{
					int ord = stoi(string({src[i+1],src[i+2],'\0'}), 0, 16);
					dst[j++] = char(ord);
					i+= 2;
				}
				catch (invalid_argument const &ex){
					printf("got invalid_argument\n");
					return false;
				}
				break;
			case 'n': dst[j++] = '\n'; break;
			case 't': dst[j++] = '\t'; break;
			case 'r': dst[j++] = '\r'; break;
			case '0': dst[j++] = '\0';
			case '"': dst[j++] = '"'; break;
			case '\\': dst[j++] = '\\';
		}
	}
	++i;
}
dst[j] = '\0';
return true;
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
			if(!handle_strings(s, yytext)) exit(0);
			showToken(tokennames.find(token)->second, s); break;
		default: showToken(tokennames.find(token)->second);
		
	}
	}
	return 0;
}
