%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int yylex();
void yyerror(const char *s);

ASTNode *root;
%}

%union {
    int num;
    char *str;
    ASTNode *node;
    ASTNode **nodelist;
    char **strlist;
}

%token <str> IDENTIFIER
%token <num> NUMBER

%token INT IF ELSE RETURN WHILE
%token EQ NE LT GT LE GE
%token ASSIGN
%token PLUS MINUS MUL DIV
%token LPAREN RPAREN
%token LBRACE RBRACE
%token SEMICOLON COMMA

%left PLUS MINUS
%left MUL DIV

%type <node> expr stmt stmt_list program
%type <node> opt_expr args opt_else param_list

%%

program
    : stmt_list                   { root = $1; }
    ;

stmt_list
    : stmt                        { $$ = create_compound((ASTNode*[]){$1}, 1); }
    | stmt_list stmt              {
                                     // Merge two statement node lists
                                     ASTNode **stmts = malloc(sizeof(ASTNode*) * ($1->compound.count + 1));
                                     memcpy(stmts, $1->compound.stmts, sizeof(ASTNode*) * $1->compound.count);
                                     stmts[$1->compound.count] = $2;
                                     $$ = create_compound(stmts, $1->compound.count + 1);
                                  }
    ;

stmt
    : INT IDENTIFIER SEMICOLON              { $$ = create_vardecl($2, NULL); }
    | INT IDENTIFIER ASSIGN expr SEMICOLON  { $$ = create_vardecl($2, $4); }
    | IDENTIFIER ASSIGN expr SEMICOLON      { $$ = create_assignment($1, $3); }
    | RETURN expr SEMICOLON                 { $$ = create_return($2); }
    | expr SEMICOLON                        { $$ = $1; }
    | IF LPAREN expr RPAREN stmt opt_else   { $$ = create_if($3, $5, $6); }
    | WHILE LPAREN expr RPAREN stmt         { $$ = create_while($3, $5); }
    | LBRACE stmt_list RBRACE               { $$ = $2; }
    ;

opt_else
    : /* empty */                           { $$ = NULL; }
    | ELSE stmt                             { $$ = $2; }
    ;

expr
    : expr PLUS expr              { $$ = create_binop('+', $1, $3); }
    | expr MINUS expr             { $$ = create_binop('-', $1, $3); }
    | expr MUL expr               { $$ = create_binop('*', $1, $3); }
    | expr DIV expr               { $$ = create_binop('/', $1, $3); }
    | IDENTIFIER                  { $$ = create_identifier($1); }
    | NUMBER                      { $$ = create_number($1); }
    | IDENTIFIER LPAREN args RPAREN { $$ = create_funcall($1, $3, -1); }
    | LPAREN expr RPAREN          { $$ = $2; }
    ;

args
    : /* empty */                 { $$ = NULL; }
    | expr                        { ASTNode **a = malloc(sizeof(ASTNode*)); a[0] = $1; $$ = a; }
    | args COMMA expr             {
                                     int count = 0;
                                     while ($1[count]) count++;
                                     ASTNode **a = realloc($1, sizeof(ASTNode*) * (count + 1));
                                     a[count] = $3;
                                     $$ = a;
                                  }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error: %s\n", s);
}
