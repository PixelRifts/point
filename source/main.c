
#define POINT_IMPLEMENTATION
#include "point.h"

int main() {
    PointProgram program = {0};
    
    PointProgramLoad(&program, PointStrLit("Hello.point"));
    PointProgramFree(&program);
}
