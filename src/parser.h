#pragma once

#include "ast.h"
#include "cstd/stdbool.h"

AST_Node     *parse_program();
AST_Node     *parse_top_level_statement();
AST_Node     *parse_function_decl();
AST_Node     *parse_let_stmt();
AST_Node     *parse_return_stmt();
AST_Node     *parse_expression();
Token        *peek();
static Token *advance();
bool          checkCurrentType(Token_Type type);
bool          matchCurrentType(Token_Type type);

AST_NodeList *ast_node_list_append(AST_NodeList *list, AST_Node *node);
AST_Node     *make_block(AST_NodeList *stmts, int line, int col);