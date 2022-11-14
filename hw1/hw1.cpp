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
{RBRACE, "RBRACE"},
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

bool handle_strings(char* dst, const char* src, char* seq){
int j=0,i=1;
int len = strlen(src);
while(i<len-1){
	if (src[i]!='\\'){
		dst[j++] = src[i];
	}
	else{
		int k=0;
		switch (src[++i]){
			case 'x':
				for(k=0;k<3 && k+i<len-1; ++k)
					seq[k] = src[i+k];
				seq[k] = '\0';
				printf("%s\n", seq);
				if(len-i<4) return false;
				try{
					int ord = stoi(string(seq+1), 0, 16);
					dst[j++] = char(ord);
					i+= 2;
				}
				catch (invalid_argument const &ex){
					return false;
				}
				break;
			case 'n': dst[j++] = '\n'; break;
			case 't': dst[j++] = '\t'; break;
			case 'r': dst[j++] = '\r'; break;
			case '0': dst[j++] = '\0'; break;
			case '"': dst[j++] = '"'; break;
			case '\\': dst[j++] = '\\'; break;
			default: seq[0] = src[i]; seq[1]='\0'; return false; break;
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
			case ERR: printf("Error %s\n", yytext);exit(0); break;
			case WHITESPACE: break;
			case COMMENT: showToken(tokennames.find(token)->second, comment); break;
			case STRING: {char s[1024],err[4] ;
			bool valid = handle_strings(s, yytext, err);
			if(valid == false){
				printf("Error undefined excape sequence %s\n", err);	
		 		exit(0);	
			}
			else{
			//printf("in string, yytext=%s\n",yytext);
			showToken(tokennames.find(token)->second, s);
			}
			break;
}
		case UNCLOSED_STRING: printf("Error unclosed string\n");exit(0); break;
		default: showToken(tokennames.find(token)->second);
		
	}
	}
	return 0;
}
