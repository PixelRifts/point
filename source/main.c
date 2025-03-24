
#define POINT_IMPLEMENTATION
#include "point.h"

int main() {
  PointProgram program = {0};
  PointProgramInit(&program);
  
  
  
  PointType my_int_type = {
    .type = Point_TK_Int,
    .size = 8,
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
  
  
  PointProgramSave(&program, PointStrLit("output.point"));
  PointProgramFree(&program);
  
  PointProgramLoad(&program, PointStrLit("output.point"));
  PointProgramFree(&program);
}
