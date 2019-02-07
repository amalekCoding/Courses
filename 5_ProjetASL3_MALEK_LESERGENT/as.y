%{
/* as2.y */
/* Syntaxe du TPC pour le projet d'analyse syntaxique de 2018-2019 */

/* Cette syntaxe n'est pas récursive à gauche. */

#include <stdio.h>



int errors = 0;

extern void yyerror();
extern void error_summary(const char *s);
extern int yylex();
%}

%token <ident> IDENT
%token <value> NUM
%token <value> CARACTERE

%token WHILE
%token IF
%token ELSE
%token RETURN

%token READE
%token READC
%token PRINT

%token TYPE
%token VOID

%token ADDSUB
%token DIVSTAR

%token EQ
%token OR
%token AND
%token ORDER

%token CONST



%expect 1



%union {
	char ident[64];
	int value;
}

%%

Prog: DeclConsts DeclGlobals
	;

DeclConsts:
	| CONST ListConst ';' DeclConsts

	| CONST error ';' DeclConsts	{error_summary("Illegal constant declaration");}
	;


ListConst: IDENT '=' Litteral
	| IDENT ',' ListConst
	;

Litteral: SignedNum
	| CARACTERE
	;

SignedNum: NUM
	| ADDSUB NUM
	;


DeclGlobals:
	| TYPE IDENT DeclGlobal DeclGlobals
	| VOID IDENT '(' Params ')' FuncBody DeclGlobals

	| error IDENT DeclGlobal DeclGlobals	{error_summary("Invalid type or identifier");}
	;


DeclGlobal: Index DeclGlobalVars ';'
	| '(' Params ')' FuncBody

	| error DeclGlobalVars ';'	{error_summary("Illegal global variable declaration");}
	| error FuncBody	{error_summary("Illegal function declaration");}
	;

DeclGlobalVars:
	| ',' Variable DeclGlobalVars
	;

ListVars: 
	| ',' Variable ListVars
	;

Variable: IDENT Index
	;

Index:
	| '[' NUM ']' Index
	;

Params: VOID
	| TYPE IDENT TypedListVars

	| error TypedListVars	{error_summary("Illegal argument");}
	;

TypedListVars: 
	| ',' TYPE IDENT TypedListVars
	;

FuncBody: '{' DeclConsts DeclVars Instructions '}'
	;

DeclVars:
	| TYPE Variable ListVars ';' DeclVars
	;


Instructions: 
	| Instruction Instructions
	;


Instruction: Exp ';'
	| ';'
	| RETURN Exp ';'
	| RETURN ';'
	| READE '(' IDENT ')' ';'
	| READC '(' IDENT ')' ';'
	| PRINT '(' Exp ')' ';'
	| IF '(' Exp ')' Instruction
	| IF '(' Exp ')' Instruction ELSE Instruction
	| WHILE '(' Exp ')' Instruction
	| '{' Instructions '}'

	| error ';'	{error_summary("Illegal instruction");}
	;

Exp: LValue '=' Exp
	| EB
	;

LValue: IDENT IndexExpr
	;

IndexExpr: 
	| '[' Exp ']' IndexExpr
	;

EB: TB EBBis
	;
EBBis:
	| OR EB
	;

TB: FB TBBis
	;
TBBis:
	| AND TB
	;

FB: M FBBis
	;
FBBis:
	| EQ FB
	;

M: E MBis
	;
MBis:
	| ORDER M
	;

E: T EBis
	;
EBis:
	| ADDSUB E
	;

T: F TBis
	;
TBis:
	| DIVSTAR T
	;

F: ADDSUB F
	| '!' F
	| '(' Exp ')'
	| LValue
	| NUM
	| CARACTERE
	| IDENT '(' Args ')'
	;

Args:
	| Exp ListExp
	;

ListExp: 
	| ',' Exp ListExp
	;

%%

int main(int argc, char **argv)
{
	if(yyparse() || errors)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}