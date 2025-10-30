
#define POINT_IMPLEMENTATION
#include "point.h"



int main() {
    PointProgram program = {0};
    PointProgramInit(&program);
    
    PointType* str_canonical = PointCreateStringTypeCanonical(&program);
    PointNode* str_type_node = PointCreateStringTypeNode(&program);
    
    PointScopeEntry hi_symbol = {
        .name = PointStrLit("hi"),
        .type = str_canonical,
    };
    PointScopeEntry* hi_canonical = PointScopeEntryCreate(&program.scopes, program.scopes.root, hi_symbol);
    PointNode* str_val = PointCreateStringLiteralNode(&program, PointStrLit("Hello, World!"));
    
    program.decls = PointCreateDeclaration(&program, hi_canonical, str_type_node, str_val, true);
    
    PointProgramSave(&program, PointStrLit("output.point"));
    printf("Saved Program\n");
    
    PointProgram loaded = {0};
    PointProgramLoad(&loaded, PointStrLit("output.point"));
    printf("Loaded Program\n");
    
    PointProgramFree(&program);
}
