#include "ast.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Helper function: Generic AST allocator
static ASTNode* alloc_node(ASTNodeType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    memset(node, 0, sizeof(ASTNode));
    node->type = type;
    return node;
}

// Numeric literals
ASTNode* create_number(int val) {
    ASTNode* node = alloc_node(AST_NUM);
    node->num = val;
    return node;
}

// Identifier
ASTNode* create_identifier(const char *name) {
    ASTNode* node = alloc_node(AST_IDENT);
    node->name = strdup(name);
    return node;
}

// Binary Operators
ASTNode* create_binop(char op, ASTNode *left, ASTNode *right) {
    ASTNode* node = alloc_node(AST_BINOP);
    node->binop.op = op;
    node->binop.left = left;
    node->binop.right = right;
    return node;
}

// Variable Declaration
ASTNode* create_vardecl(const char *name, ASTNode *value) {
    ASTNode* node = alloc_node(AST_VARDECL);
    node->vardecl.name = strdup(name);
    node->vardecl.value = value;
    return node;
}

// Assignment Statement
ASTNode* create_assignment(const char *name, ASTNode *value) {
    ASTNode* node = alloc_node(AST_ASSIGN);
    node->assign.name = strdup(name);
    node->assign.value = value;
    return node;
}

// return statement
ASTNode* create_return(ASTNode *val) {
    ASTNode* node = alloc_node(AST_RETURN);
    node->ret_expr = val;
    return node;
}

// If Statement
ASTNode* create_if(ASTNode *cond, ASTNode *then_branch, ASTNode *else_branch) {
    ASTNode* node = alloc_node(AST_IF);
    node->control.cond = cond;
    node->control.then_branch = then_branch;
    node->control.else_branch = else_branch;
    return node;
}

// while Statement
ASTNode* create_while(ASTNode *cond, ASTNode *body) {
    ASTNode* node = alloc_node(AST_WHILE);
    node->control.cond = cond;
    node->control.then_branch = body;
    node->control.else_branch = NULL;
    return node;
}

// Function Call
ASTNode* create_funcall(const char *name, ASTNode **args, int arg_count) {
    ASTNode* node = alloc_node(AST_FUNCALL);
    node->funcall.name = strdup(name);
    node->funcall.args = args;
    node->funcall.arg_count = arg_count;
    return node;
}

// Function definition
ASTNode* create_fundef(const char *name, char **params, int param_count, ASTNode *body) {
    ASTNode* node = alloc_node(AST_FUNDEF);
    node->fundef.name = strdup(name);
    node->fundef.params = params;
    node->fundef.param_count = param_count;
    node->fundef.body = body;
    return node;
}

// Code blocks/compound statements
ASTNode* create_compound(ASTNode **stmts, int count) {
    ASTNode* node = alloc_node(AST_COMPOUND);
    node->compound.stmts = stmts;
    node->compound.count = count;
    return node;
}
