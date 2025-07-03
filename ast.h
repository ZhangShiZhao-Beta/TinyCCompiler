#ifndef AST_H
#define AST_H

typedef enum {
    AST_NUM,
    AST_IDENT,
    AST_BINOP,
    AST_VARDECL,
    AST_ASSIGN,
    AST_COMPOUND,
    AST_RETURN,
    AST_IF,
    AST_WHILE,
    AST_FUNCALL,
    AST_FUNDEF
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;

    union {
        // number
        int num;

        // identifier
        char *name;

        // binary operation
        struct {
            char op;
            struct ASTNode *left;
            struct ASTNode *right;
        } binop;

        // variable declaration
        struct {
            char *name;
            struct ASTNode *value; // optional
        } vardecl;

        // assignment
        struct {
            char *name;
            struct ASTNode *value;
        } assign;

        // function call
        struct {
            char *name;
            struct ASTNode **args;
            int arg_count;
        } funcall;

        // function definition
        struct {
            char *name;
            char **params;
            int param_count;
            struct ASTNode *body;
        } fundef;

        // compound statement (block)
        struct {
            struct ASTNode **stmts;
            int count;
        } compound;

        // return
        struct ASTNode *ret_expr;

        // if / while
        struct {
            struct ASTNode *cond;
            struct ASTNode *then_branch;
            struct ASTNode *else_branch; // if: optional
        } control;
    };
} ASTNode;

// 创建 AST 节点的函数声明
ASTNode *create_number(int val);
ASTNode *create_identifier(const char *name);
ASTNode *create_binop(char op, ASTNode *left, ASTNode *right);
ASTNode *create_assignment(const char *name, ASTNode *value);
ASTNode *create_vardecl(const char *name, ASTNode *value);
ASTNode *create_return(ASTNode *val);
ASTNode *create_if(ASTNode *cond, ASTNode *then_branch, ASTNode *else_branch);
ASTNode *create_while(ASTNode *cond, ASTNode *body);
ASTNode *create_funcall(const char *name, ASTNode **args, int arg_count);
ASTNode *create_fundef(const char *name, char **params, int param_count, ASTNode *body);
ASTNode *create_compound(ASTNode **stmts, int count);

#endif
