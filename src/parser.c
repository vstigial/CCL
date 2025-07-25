#include "parser.h"

#include "cstd/stdlib.h"

static Token *TokenArray;
static int idx;
static int count;

AST_Node *parse_program(Token *toks, size_t prog_len) {
    TokenArray = toks;
    idx        = 0;
    count      = prog_len;

    AST_NodeList *stmts = NULL;
    while (peek() && peek()->type != TokenType_EndOfFile) {
        AST_Node *stmt = parse_statement(true);
        stmts = ast_node_list_append(stmts, stmt);
    }

    if (peek() && peek()->type == TokenType_EndOfFile) {
        advance();
    }

    int line = stmts ? stmts->node->line : 0;
    int col  = stmts ? stmts->node->col  : 0;
    return make_block(stmts, line, col);
}

AST_Node *parse_statement(bool toplevel) {
    if (toplevel && checkCurrentType(TokenType_Fn))
        return parse_function_decl();
    if (checkCurrentType(TokenType_Let))
        return parse_let_stmt();
    if (checkCurrentType(TokenType_Return))
        return parse_return_stmt();

    // else must be an expression
    return parse_expression();
}

AST_Node *parse_block() {
    Token *lbrace = consumeCurrentType(TokenType_LBrace);

    AST_NodeList *stmts = NULL;
    while (!checkCurrentType(TokenType_RBrace)) {
        AST_Node *stmt = parse_statement(false);
        stmts = ast_node_list_append(stmts, stmt);
    }

    consumeCurrentType(TokenType_RBrace);

    return make_block(stmts, lbrace->line, lbrace->col);
}

AST_Node *parse_if_expr() {
    Token *if_tok = advance();
    consumeCurrentType(TokenType_LParen);
    AST_Node *cond = parse_expression();
    consumeCurrentType(TokenType_RParen);

    AST_Node *then_branch = NULL;
    if (checkCurrentType(TokenType_LBrace)) {
        then_branch = parse_block();
    } else {
        then_branch = parse_expression();
    }

    AST_Node *else_branch = NULL;

    AST_Node *n = malloc(sizeof *n);
    n->kind                    = AST_IF_EXPR;
    n->line                    = if_tok->line;
    n->col                     = if_tok->col;
    n->as.if_expr.condition    = cond;
    n->as.if_expr.then_branch  = then_branch;
    n->as.if_expr.else_branch  = else_branch;
    return n;
}

AST_Node *parse_function_decl() {
    Token *fn_token = consumeCurrentType(TokenType_Fn);
    Token *name_token = consumeCurrentType(TokenType_Identifier);

    consumeCurrentType(TokenType_LParen);
    AST_NodeList *params = NULL;
    
    if (!checkCurrentType(TokenType_RParen)) {
        do {
            Token *param_name = consumeCurrentType(TokenType_Identifier);
            consumeCurrentType(TokenType_Colon);
            Token *type_token = consumeCurrentType(TokenType_Identifier);

            AST_Node *param_node = malloc(sizeof *param_node);
            param_node->kind = AST_IDENTIFIER;
            param_node->line = param_name->line;
            param_node->col  = param_name->col;
            param_node->as.identifier.name = param_name->value;
            
            // TODO: type is dropped here

            params = ast_node_list_append(params, param_node);
        } while (matchCurrentType(TokenType_Comma));
    }

    consumeCurrentType(TokenType_RParen);

    char *ret_type = NULL; // TODO: "void" instead of NULL
    if (matchCurrentType(TokenType_Colon)) {
        Token *ret_token = consumeCurrentType(TokenType_Identifier);
        ret_type = ret_token->value;
    }

    AST_Node *body = parse_block();

    AST_Node *node = malloc(sizeof *node);
    node->kind = AST_FUNC_DECL;
    node->line = fn_token->line;
    node->col  = fn_token->col;
    node->as.func_decl.name = name_token->value;
    node->as.func_decl.params = params;
    node->as.func_decl.return_type = ret_type;
    node->as.func_decl.body = body;

    return node;
}

AST_Node *parse_let_stmt() {
    Token *let_token = consumeCurrentType(TokenType_Let);
    Token *name_token = consumeCurrentType(TokenType_Identifier);

    consumeCurrentType(TokenType_Colon);
    Token *type_token = consumeCurrentType(TokenType_Identifier); // types are treated as identifier tokens
    consumeCurrentType(TokenType_Equal);

    AST_Node *value_expr = parse_expression();

    AST_Node *node = malloc(sizeof(AST_Node));
    node->kind = AST_LET_STMT;
    node->line = let_token->line;
    node->col  = let_token->col;
    node->as.let_stmt.name = name_token->value;
    node->as.let_stmt.type_annotation = type_token->value;
    node->as.let_stmt.value = value_expr;

    return node;
}

AST_Node *parse_return_stmt() {
    Token *tok = consumeCurrentType(TokenType_Return);
    AST_Node *expr = parse_expression();

    AST_Node *node = malloc(sizeof(AST_Node));
    node->kind = AST_RETURN_STMT;
    node->line = tok->line;
    node->col  = tok->col;
    node->as.return_stmt.value = expr;

    return node;
}

AST_Node *parse_term() {
    AST_Node *node = parse_factor();

    while (checkCurrentType(TokenType_Plus) || checkCurrentType(TokenType_Minus)) {
        Token *op_tok = advance();
        AST_Node *right = parse_factor();

        AST_Node *bin = malloc(sizeof *bin);
        bin->kind = AST_BINARY_EXPR;
        bin->line = op_tok->line;
        bin->col  = op_tok->col;
        bin->as.binary_expr.left  = node;
        bin->as.binary_expr.right = right;
        bin->as.binary_expr.op    = op_tok->type;
        node = bin;
    }

    return node;
}

AST_Node *parse_factor() {
    AST_Node *node = parse_primary();

    while (checkCurrentType(TokenType_Star) || checkCurrentType(TokenType_Slash)) {
        Token *op_tok = advance();
        AST_Node *right = parse_primary();

        AST_Node *bin = malloc(sizeof *bin);
        bin->kind = AST_BINARY_EXPR;
        bin->line = op_tok->line;
        bin->col  = op_tok->col;
        bin->as.binary_expr.left  = node;
        bin->as.binary_expr.right = right;
        bin->as.binary_expr.op    = op_tok->type;
        node = bin;
    }

    return node;
}

AST_Node *parse_primary() {
    Token *t;
    
    if (checkCurrentType(TokenType_If)) {
        return parse_if_expr();
    }

    if (checkCurrentType(TokenType_Number)) {
        t = advance();
        AST_Node *n = malloc(sizeof *n);
        n->kind               = AST_INT_LITERAL;
        n->line               = t->line;
        n->col                = t->col;
        n->as.int_literal.value = parse_int_literal(t->value);
        return n;
    }

    if (checkCurrentType(TokenType_String)) {
        t = advance();
        AST_Node *n = malloc(sizeof *n);
        n->kind                  = AST_STRING_LITERAL;
        n->line                  = t->line;
        n->col                   = t->col;

        n->as.string_literal.value = t->value;
        return n;
    }

    if (checkCurrentType(TokenType_Identifier)) {
        t = advance();
        AST_Node *id = malloc(sizeof *id);
        id->kind               = AST_IDENTIFIER;
        id->line               = t->line;
        id->col                = t->col;
        id->as.identifier.name = strdup(t->value);

        if (matchCurrentType(TokenType_LParen)) {
            AST_NodeList *args = NULL;
            if (!checkCurrentType(TokenType_RParen)) {
                do {
                    AST_Node *arg = parse_expression();
                    args = ast_node_list_append(args, arg);
                } while (matchCurrentType(TokenType_Comma));
            }
            consumeCurrentType(TokenType_RParen);

            AST_Node *call = malloc(sizeof *call);
            call->kind          = AST_CALL_EXPR;
            call->line          = t->line;
            call->col           = t->col;
            call->as.call_expr.callee = id;
            call->as.call_expr.args   = args;
            return call;
        }

        // else just identifier
        return id;
    }

    if (matchCurrentType(TokenType_LParen)) {
        AST_Node *inner = parse_expression();
        consumeCurrentType(TokenType_RParen);
        return inner;
    }

    exit(1);
}

AST_Node *parse_expression() {
    return parse_term();
}

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