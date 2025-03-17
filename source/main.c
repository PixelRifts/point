
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
  
  
  PointType my_void_type  = {
    .type = Point_TK_Void,
    .size = 0,
  };
  PointType* void_canonical = PointTypeCacheRegister(&program.types, my_void_type);
  
  
  PointType my_func_type = {
    .type = Point_TK_Func,
    .func_t.ret_t = int64_canonical,
    .func_t.arg_ts = &int64_canonical,
    .func_t.arity = 1,
    .func_t.is_varargs = false,
  };
  PointType* proc_i64_i64_canonical = PointTypeCacheRegister(&program.types, my_func_type);
  
  
  PointType my_func_funcprod_type = {
    .type = Point_TK_Func,
    .func_t.ret_t = proc_i64_i64_canonical,
    .func_t.arg_ts = &void_canonical,
    .func_t.arity = 1,
    .func_t.is_varargs = true,
  };
  PointType* proc_proc_void_canonical = PointTypeCacheRegister(&program.types, my_func_funcprod_type);
  
  PointProgramSave(&program, PointStrLit("output.point"));
  PointProgramFree(&program);
  
  PointProgramLoad(&program, PointStrLit("output.point"));
  PointProgramFree(&program);
}
