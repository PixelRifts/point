
#define POINT_IMPLEMENTATION
#include "point.h"

int main() {
  PointProgram program = {0};
  PointProgramInit(&program);
  
  
  
  PointType my_int_type = {
    .type = Point_TK_Int,
    .size = 64,
    .int_t.is_signed = true,
  };
  PointType* int64_canonical = PointTypeCacheRegister(&program.types, my_int_type);
  
  
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
  
  PointScope* inner_1 = PointScopeCreate(&program.scopes, program.scopes.root);
  PointScopeEntry type_symbol = {
    .name = PointStrLit("type"),
    .type = type_canonical,
  };
  PointScopeEntry* type_symbol_canonical = PointScopeEntryCreate(&program.scopes, inner_1, type_symbol);
  
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
  
  program.decls = my_add_node;
  
  PointProgramSave(&program, PointStrLit("output.point"));
  PointProgramFree(&program);
  
  PointProgram other = {0};
  
  PointProgramLoad(&other, PointStrLit("output.point"));
  PointProgramFree(&other);
}
