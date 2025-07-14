#pragma once

#include "token.h"

typedef enum {
    AST_INT_LITERAL,
    AST_STRING_LITERAL,
    AST_IDENTIFIER,
    AST_BINARY_EXPR,
    AST_IF_EXPR,
    AST_LET_STMT,
    AST_RETURN_STMT,
    AST_FUNC_DECL,
    AST_CALL_EXPR,
    AST_BLOCK,
} AST_Kind;

typedef struct AST_Node AST_Node;

typedef struct AST_NodeList {
    AST_Node *node;
    struct AST_NodeList *next;
} AST_NodeList;

struct AST_Node {
    AST_Kind kind;
    int line, col;

    union {
        struct {
            long value;
        } int_literal;

        struct {
            char *value;
        } string_literal;

        struct {
            char *name;
        } identifier;

        struct {
            AST_Node *left;
            AST_Node *right;
            Token_Type op;
        } binary_expr;

        struct {
            AST_Node *condition;
            AST_Node *then_branch;
            AST_Node *else_branch;
        } if_expr;

        // let <name>: <type_annotation> = <expr>
        struct {
            char *name;
            char *type_annotation;
            AST_Node *value;
        } let_stmt;

        // return <expr>
        struct {
            AST_Node *value;
        } return_stmt;

        // fn <name>(<params>): <return_type> { <body> }
        struct {
            char *name;
            AST_NodeList *params; // list of identifier nodes
            char *return_type;
            AST_Node *body;
        } func_decl;

        // <callee>(<args>)
        struct {
            AST_Node *callee;
            AST_NodeList *args;
        } call_expr;

        // { stmt1; stmt2; ... }
        struct {
            AST_NodeList *statements;
        } block;
    } as;
};