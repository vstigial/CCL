#include "parser.h"

#include "cstd/stdlib.h"

static Token *TokenArray;
static int idx;
static int count;

AST_Node *parse_program() {
    AST_NodeList *stmts = NULL;
    while (peek() && peek()->type != TokenType_EndOfFile) {
        AST_Node *stmt = parse_top_level_statement();
        stmts = ast_node_list_append(stmts, stmt);
    }

    if (peek() && peek()->type == TokenType_EndOfFile) {
        advance();
    }

    int line = stmts ? stmts->node->line : 0;
    int col  = stmts ? stmts->node->col  : 0;
    return make_block(stmts, line, col);
}

AST_Node *parse_top_level_statement() {
    if (checkCurrentType(TokenType_Fn))
        return parse_function_decl();
    if (checkCurrentType(TokenType_Let))
        return parse_let_stmt();
    if (checkCurrentType(TokenType_Return))
        return parse_return_stmt();

    // else must be an expression
    return parse_expression();
}

AST_Node *parse_function_decl() {/* TODO! */}
AST_Node *parse_let_stmt() {/* TODO! */}
AST_Node *parse_return_stmt() {/* TODO! */}
AST_Node *parse_expression() {/* TODO! */}

AST_Node *make_block(AST_NodeList *stmts, int line, int col) {
    AST_Node *n = malloc(sizeof *n);
    n->kind           = AST_BLOCK;
    n->line           = line;
    n->col            = col;
    n->as.block.statements = stmts;
    return n;
}

AST_NodeList *ast_node_list_prepend(AST_Node *node, AST_NodeList *list) {
    AST_NodeList *entry = malloc(sizeof(AST_NodeList));
    entry->node = node;
    entry->next = list;
    return entry;
}

AST_NodeList *ast_node_list_append(AST_NodeList *list, AST_Node *node) {
    if (!list) {
        return ast_node_list_prepend(node, NULL);
    }
    AST_NodeList *head = list;
    while (list->next) list = list->next;
    list->next = ast_node_list_prepend(node, NULL);
    return head;
}

Token *peek() {
    return (idx < count) ? &TokenArray[idx] : NULL;
}

static Token *advance() {
    Token *t = peek();
    if (idx < count) idx++;
    return t;
}

bool checkCurrentType(Token_Type type) {
    Token *t = peek();
    return t && t->type == type;
}

// same as checkCurrentType except consumes token
bool matchCurrentType(Token_Type type) {
    if (checkCurrentType(type)) { advance(); return true; }
    return false;
}

Token *consumeCurrentType(Token_Type type) {
    if (checkCurrentType(type)) return advance();
    exit(1);
}