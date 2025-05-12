
#define POINT_IMPLEMENTATION
#include "point.h"

static PointNode* CreateDeclaration(PointProgram* program, PointScopeEntry* symbol, PointNode* val) {
    PointNode* ret = PointPoolAlloc(&program->node_pool);
    ret->type = Point_NT_Decl;
    ret->decl.ref = symbol;
    ret->decl.val = val;
    ret->decl.is_constant = true;
    return ret;
}

int main() {
    
    PointProgram program = {0};
    PointProgramInit(&program);
    
    
    
    PointType my_int_type = {
        .type = Point_TK_Int,
        .size = 64,
        .int_t.is_signed = true,
    };
    PointType* int64_canonical = PointTypeCacheRegister(&program.types, my_int_type);
    
    PointType my_bool_type = {
        .type = Point_TK_Bool,
        .size = 8,
    };
    PointType* bool_canonical = PointTypeCacheRegister(&program.types, my_bool_type);
    
    PointType my_type_type = {
        .type = Point_TK_Type,
        .size = 0,
    };
    PointType* type_canonical = PointTypeCacheRegister(&program.types, my_type_type);
    
    PointType my_void_type  = {
        .type = Point_TK_Void,
        .size = 0,
    };
    PointType* void_canonical = PointTypeCacheRegister(&program.types, my_void_type);
    
    PointType** simple_op_args = PointArenaAlloc(&program.misc_arena, sizeof(PointType) * 2);
    simple_op_args[0] = int64_canonical;
    simple_op_args[1] = int64_canonical;
    PointType my_simple_op_type  = {
        .type = Point_TK_Func,
        .size = 0,
        .func_t.ret_t  = int64_canonical,
        .func_t.arg_ts = simple_op_args,
        .func_t.arity  = 2,
        .func_t.is_varargs = false,
    };
    PointType* simple_op_type_canonical = PointTypeCacheRegister(&program.types, my_simple_op_type);
    
    
    
    
    
    PointScopeEntry i64_symbol = {
        .name = PointStrLit("i64"),
        .type = type_canonical,
    };
    PointScopeEntry* i64_symbol_canonical = PointScopeEntryCreate(&program.scopes, program.scopes.root, i64_symbol);
    
    PointScopeEntry void_symbol = {
        .name = PointStrLit("void"),
        .type = type_canonical,
    };
    PointScopeEntry* void_symbol_canonical = PointScopeEntryCreate(&program.scopes, program.scopes.root, void_symbol);
    
    PointScopeEntry type_symbol = {
        .name = PointStrLit("type"),
        .type = type_canonical,
    };
    PointScopeEntry* type_symbol_canonical = PointScopeEntryCreate(&program.scopes, program.scopes.root, type_symbol);
    
    PointScopeEntry opA_symbol = {
        .name = PointStrLit("comp_a"),
        .type = simple_op_type_canonical,
    };
    PointScopeEntry* opA_symbol_canonical = PointScopeEntryCreate(&program.scopes, program.scopes.root, opA_symbol);
    PointScope* opA_scope = PointScopeCreate(&program.scopes, program.scopes.root);
    
    PointScopeEntry opB_symbol = {
        .name = PointStrLit("comp_b"),
        .type = simple_op_type_canonical,
    };
    PointScopeEntry* opB_symbol_canonical = PointScopeEntryCreate(&program.scopes, program.scopes.root, opB_symbol);
    PointScope* opB_scope = PointScopeCreate(&program.scopes, program.scopes.root);
    
    PointScopeEntry opB_arg0_symbol = {
        .name = PointStrLit("a"),
        .type = int64_canonical,
        .arg_num = 1,
    };
    PointScopeEntry* opB_arg0_canonical = PointScopeEntryCreate(&program.scopes, program.scopes.root, opB_arg0_symbol);
    
    PointScopeEntry opB_arg1_symbol = {
        .name = PointStrLit("b"),
        .type = int64_canonical,
        .arg_num = 2,
    };
    PointScopeEntry* opB_arg1_canonical = PointScopeEntryCreate(&program.scopes, program.scopes.root, opB_arg1_symbol);
    
    //~ comp_a nodes
    PointNode* my_const_int_a = PointPoolAlloc(&program.node_pool);
    my_const_int_a->expr_type = int64_canonical;
    my_const_int_a->type = Point_NT_Expr_IntLit;
    my_const_int_a->is_constant = true;
    my_const_int_a->constant_val.type = Point_CVT_Int;
    my_const_int_a->constant_val.int_lit = 256;
    PointNode* my_const_int_b = PointPoolAlloc(&program.node_pool);
    my_const_int_b->expr_type = int64_canonical;
    my_const_int_b->type = Point_NT_Expr_IntLit;
    my_const_int_b->is_constant = true;
    my_const_int_b->constant_val.type = Point_CVT_Int;
    my_const_int_b->constant_val.int_lit = 512;
    PointNode* my_add_node = PointPoolAlloc(&program.node_pool);
    my_add_node->expr_type = int64_canonical;
    my_add_node->type = Point_NT_Expr_Add;
    my_add_node->binary_op.left  = my_const_int_a;
    my_add_node->binary_op.right = my_const_int_b;
    PointNode* return_const_sum = PointPoolAlloc(&program.node_pool);
    return_const_sum->type = Point_NT_Stmt_Return;
    return_const_sum->return_stmt = my_add_node;
    PointNode* opA_block = PointPoolAlloc(&program.node_pool);
    opA_block->type = Point_NT_Stmt_Block;
    opA_block->block.body = return_const_sum;
    opA_block->block.scope = opA_scope;
    
    //~ comp_b nodes
    PointNode* opB_arg0 = PointPoolAlloc(&program.node_pool);
    opB_arg0->expr_type = int64_canonical;
    opB_arg0->type = Point_NT_Expr_Ident;
    opB_arg0->ident = opB_arg0_canonical;
    PointNode* opB_arg1_ref0 = PointPoolAlloc(&program.node_pool);
    opB_arg1_ref0->expr_type = int64_canonical;
    opB_arg1_ref0->type = Point_NT_Expr_Ident;
    opB_arg1_ref0->ident = opB_arg1_canonical;
    PointNode* opB_arg1_ref1 = PointPoolAlloc(&program.node_pool);
    opB_arg1_ref1->expr_type = int64_canonical;
    opB_arg1_ref1->type = Point_NT_Expr_Ident;
    opB_arg1_ref1->ident = opB_arg1_canonical;
    
    PointNode* const_256 = PointPoolAlloc(&program.node_pool);
    const_256->expr_type = int64_canonical;
    const_256->type = Point_NT_Expr_IntLit;
    const_256->is_constant = true;
    const_256->constant_val.type = Point_CVT_Int;
    const_256->constant_val.int_lit = 256;
    PointNode* if_cond = PointPoolAlloc(&program.node_pool);
    if_cond->type = Point_NT_Expr_Eq;
    if_cond->expr_type = bool_canonical;
    if_cond->binary_op.left  = opB_arg0;
    if_cond->binary_op.right = const_256;
    PointNode* neg_val = PointPoolAlloc(&program.node_pool);
    neg_val->type = Point_NT_Expr_Negate;
    neg_val->expr_type = int64_canonical;
    neg_val->unary_op.operand = opB_arg1_ref1;
    
    PointNode* build_zero = PointPoolAlloc(&program.node_pool);
    build_zero->type = Point_NT_Stmt_If;
    build_zero->if_stmt.condition = if_cond;
    build_zero->if_stmt.then_body = opB_arg1_ref0;
    build_zero->if_stmt.else_body = neg_val;
    PointNode* opB_block = PointPoolAlloc(&program.node_pool);
    opB_block->type = Point_NT_Stmt_Block;
    opB_block->block.body = build_zero;
    opB_block->block.scope = opB_scope;
    
    
    PointNode* opA_decl_node = CreateDeclaration(&program, opA_symbol_canonical, opA_block);
    PointNode* opB_decl_node = CreateDeclaration(&program, opB_symbol_canonical, opB_block);
    opA_decl_node->next = opB_decl_node;
    
    program.decls = opA_decl_node;
    
    PointProgramSave(&program, PointStrLit("output.point"));
    PointProgramFree(&program);
    
}
