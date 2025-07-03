#include <stdio.h>
#include "ast.h"

// 由 Bison 提供的接口
extern int yyparse();
extern FILE *yyin;
extern ASTNode *root;

// 打印缩进用
void print_indent(int indent) {
    for (int i = 0; i < indent; ++i) printf("  ");
}

// 递归打印 AST
void print_ast(ASTNode *node, int indent) {
    if (!node) return;

    switch (node->type) {
        case AST_NUM:
            print_indent(indent);
            printf("Number: %d\n", node->num);
            break;
        case AST_IDENT:
            print_indent(indent);
            printf("Identifier: %s\n", node->name);
            break;
        case AST_BINOP:
            print_indent(indent);
            printf("BinaryOp: %c\n", node->binop.op);
            print_ast(node->binop.left, indent + 1);
            print_ast(node->binop.right, indent + 1);
            break;
        case AST_ASSIGN:
            print_indent(indent);
            printf("Assign: %s\n", node->assign.name);
            print_ast(node->assign.value, indent + 1);
            break;
        case AST_VARDECL:
            print_indent(indent);
            printf("VarDecl: %s\n", node->vardecl.name);
            if (node->vardecl.value) {
                print_ast(node->vardecl.value, indent + 1);
            }
            break;
        case AST_RETURN:
            print_indent(indent);
            printf("Return:\n");
            print_ast(node->ret_expr, indent + 1);
            break;
        case AST_IF:
            print_indent(indent);
            printf("If:\n");
            print_indent(indent + 1); printf("Condition:\n");
            print_ast(node->control.cond, indent + 2);
            print_indent(indent + 1); printf("Then:\n");
            print_ast(node->control.then_branch, indent + 2);
            if (node->control.else_branch) {
                print_indent(indent + 1); printf("Else:\n");
                print_ast(node->control.else_branch, indent + 2);
            }
            break;
        case AST_WHILE:
            print_indent(indent);
            printf("While:\n");
            print_indent(indent + 1); printf("Condition:\n");
            print_ast(node->control.cond, indent + 2);
            print_indent(indent + 1); printf("Body:\n");
            print_ast(node->control.then_branch, indent + 2);
            break;
        case AST_FUNCALL:
            print_indent(indent);
            printf("Function Call: %s\n", node->funcall.name);
            for (int i = 0; i < node->funcall.arg_count; ++i) {
                print_ast(node->funcall.args[i], indent + 1);
            }
            break;
        case AST_FUNDEF:
            print_indent(indent);
            printf("Function Definition: %s\n", node->fundef.name);
            print_indent(indent + 1);
            printf("Params:");
            for (int i = 0; i < node->fundef.param_count; ++i) {
                printf(" %s", node->fundef.params[i]);
            }
            printf("\n");
            print_ast(node->fundef.body, indent + 1);
            break;
        case AST_COMPOUND:
            print_indent(indent);
            printf("Compound Statement:\n");
            for (int i = 0; i < node->compound.count; ++i) {
                print_ast(node->compound.stmts[i], indent + 1);
            }
            break;
        default:
            print_indent(indent);
            printf("Unknown AST node type: %d\n", node->type);
            break;
    }
}

// 主函数：打开文件并调用解析
int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "用法: %s <source.c>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("无法打开源文件");
        return 1;
    }

    yyin = fp;
    if (yyparse() == 0) {
        printf("语法分析成功。\n");
        if (root) {
            printf("抽象语法树结构：\n");
            print_ast(root, 0);
        } else {
            printf("没有生成语法树（root 为空）。\n");
        }
    } else {
        printf("语法分析失败。\n");
    }

    fclose(fp);
    return 0;
}
