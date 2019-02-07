%{
#include "as2.tab.h"

#define TYPE_CHAR            					1
#define TYPE_INT             					2
#define PLUS                 					3
#define MINUS                					4
#define ASTERISK             					5
#define SLASH                					6
#define EQUAL_OR_LESSER_THAN 					7
#define EQUAL_OR_GREATER_THAN					8

void yyerror();
void error_summary(const char *s);



int current_line = 1;

%}

%x	COMMENT


%%

\/\*	BEGIN(COMMENT);
<COMMENT><<EOF>>	{yyerror(); error_summary("Missing '*/' to complete the comment."); return 0;}
<COMMENT>\*\/	BEGIN(INITIAL);
<COMMENT>.	;
<COMMENT>\n	current_line++;


'\\t'	yylval.value = '\t'; return CARACTERE;
'\\n'	yylval.value = '\n'; return CARACTERE;
'\\'' 	yylval.value = '\''; return CARACTERE;
'\\'  	yylval.value = '\\'; return CARACTERE;
'.'  	yylval.value = yytext[1]; return CARACTERE;

while 	return WHILE;
if    	return IF;
else  	return ELSE;
return	return RETURN;

reade	return READE;
readc	return READC;
print	return PRINT;

char	yylval.value = TYPE_CHAR; return TYPE;
int 	yylval.value = TYPE_INT;  return TYPE;
void	return VOID;

const	return CONST;

\+	yylval.value = PLUS; return ADDSUB;
\-	yylval.value = MINUS; return ADDSUB;
\*	yylval.value = ASTERISK; return DIVSTAR;
\/	yylval.value = SLASH; return DIVSTAR;

;	return ';';
!	return '!';
=	return '=';
,	return ',';
\(	return '(';
\)	return ')';
\[	return '[';
\]	return ']';
\{	return '{';
\}	return '}';




\=\=	return EQ;
\|\|	return OR;
\&\&	return AND;
\<\=	yylval.value = EQUAL_OR_LESSER_THAN; return ORDER;
\>\=	yylval.value = EQUAL_OR_GREATER_THAN; return ORDER;

\n	current_line++;
[\t ]	;

[A-Za-z][A-Za-z0-9_]{0,62}	strcpy(yylval.ident, yytext); return IDENT;
[0-9]+	yylval.value = atoi(yytext); return NUM;

.      	return 0;
<<EOF>>	return 0;

%%
void yyerror()
{
	fprintf(stderr, "*** Line %d: Syntax error! ", current_line);
}
void error_summary(const char *s)
{
	fprintf(stderr, "%s\n", s);
}