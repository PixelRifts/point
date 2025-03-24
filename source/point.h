/* date = February 24th 2025 11:25 am */

#ifndef POINT_H
#define POINT_H

//~ DEFINES

#if defined(__clang__)
#  define POINT_COMPILER_CLANG
#elif defined(_MSC_VER)
#  define POINT_COMPILER_CL
#elif defined(__GNUC__)
#  define POINT_COMPILER_GCC
#endif

#if defined(POINT_COMPILER_CLANG)
#  define POINT_FILE_NAME __FILE_NAME__
#else
#  define POINT_FILE_NAME __FILE__
#endif


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#  define POINT_PLATFORM_WIN
#elif defined(__linux__) || defined(__gnu_linux__)
#  define POINT_PLATFORM_LINUX
#elif defined(__APPLE__)
#  if TARGET_OS_IPHONE && TARGET_OS_SIMULATOR
#    error "The project only supports windows, linux and mac for now"
#  elif TARGET_OS_IPHONE && TARGET_OS_MACCATALYST
#    error "The project only supports windows, linux and mac for now"
#  elif TARGET_OS_IPHONE
#    error "The project only supports windows, linux and mac for now"
#  else
#    define POINT_PLATFORM_MACOS
#  endif
#else
#  error "The project only supports windows, linux and mac for now"
#endif


// Unsigned int types.
typedef unsigned char ptu8;
typedef unsigned short ptu16;
typedef unsigned int ptu32;
typedef unsigned long long ptu64;

// Signed int types.
typedef signed char pts8;
typedef signed short pts16;
typedef signed int pts32;
typedef signed long long pts64;

// Regular int types.
typedef char pti8;
typedef short pti16;
typedef int pti32;
typedef long long pti64;

// Floating point types
typedef float ptf32;
typedef double ptf64;

// Boolean types
typedef ptu8  ptb8;
typedef ptu32 ptb32;


#define PointGigabytes(count) (ptu64) (count * 1024 * 1024 * 1024)
#define PointMegabytes(count) (ptu64) (count * 1024 * 1024)
#define PointKilobytes(count) (ptu64) (count * 1024)

#define PointMemoryZero(d,z) memset((d), 0, (z))
#define PointMemoryZeroStruct(d,s) PointMemoryZero((d),sizeof(s))

//~ Data Structure Defs


#define PointDoubleCapacity(x) ((x) <= 0 ? 8 : (x) * 2)
#define PointDStack(type) type##_stack

#define PointDStackPush(type, stack, data) type##_stack##_push(stack, data)
#define PointDStackPop(type, stack) type##_stack##_pop(stack)
#define PointDStackPeek(type, stack) type##_stack##_peek(stack)
#define PointDStackClear(type, stack) type##_stack##_clear(stack)
#define PointDStackFree(type, stack) type##_stack##_free(stack)

#define PointDStackPrototype(Data)\
typedef struct Data##_stack {\
ptu32 cap;\
ptu32 len;\
Data* elems;\
} Data##_stack;\
void Data##_stack_push(Data##_stack* stack, Data data);\
Data Data##_stack_pop(Data##_stack* stack);\
Data Data##_stack_peek(Data##_stack* stack);\
void Data##_stack_clear(Data##_stack* stack);\
void Data##_stack_free(Data##_stack* stack);

#define PointDStackImpl(Data)\
void Data##_stack_push(Data##_stack* stack, Data data) {\
if (stack->len + 1 > stack->cap) {\
void* prev = stack->elems;\
ptu32 new_cap = PointDoubleCapacity(stack->cap);\
stack->elems = calloc(new_cap, sizeof(Data));\
memmove(stack->elems, prev, stack->len * sizeof(Data));\
stack->cap = new_cap;\
free(prev);\
}\
stack->elems[stack->len++] = data;\
}\
Data Data##_stack_pop(Data##_stack* stack) {\
if (stack->len == 0) return (Data){0};\
return stack->elems[--stack->len];\
}\
Data Data##_stack_peek(Data##_stack* stack) {\
if (stack->len == 0) return (Data){0};\
return stack->elems[stack->len - 1];\
}\
void Data##_stack_clear(Data##_stack* stack) {\
stack->len = 0;\
}\
void Data##_stack_free(Data##_stack* stack) {\
stack->cap = 0;\
stack->len = 0;\
free(stack->elems);\
}



#define PointDQueue(type) type##_queue

#define PointDQueueReserve(type, q, len) type##_queue_reserve(q, len)
#define PointDQueuePush(type, q, d) type##_queue_push(q, d)
#define PointDQueuePop(type, q, d) type##_queue_pop(q, d)
#define PointDQueuePeek(type, q) type##_queue_peek(q)
#define PointDQueueClear(type, q) type##_queue_clear(q)
#define PointDQueueFree(type, q) type##_queue_free(q)

#define PointDQueuePrototype(Data)\
typedef struct Data##_queue {\
ptu32 cap;\
ptu32 front;\
ptu32 back; /* One past last */\
Data* elems;\
} Data##_queue;\
void Data##_queue_reserve(Data##_queue* q, ptu32 len);\
void Data##_queue_push(Data##_queue* q, Data data);\
ptb8 Data##_queue_pop(Data##_queue* q, Data* ret);\
Data Data##_queue_peek(Data##_queue* q);\
void Data##_queue_clear(Data##_queue* q);\
void Data##_queue_free(Data##_queue* q);

#define PointDQueueImpl(Data)\
void Data##_queue_reserve(Data##_queue* q, ptu32 len) {\
q->front = 0;\
q->back  = 0;\
q->cap   = len;\
q->elems = malloc(q->cap * sizeof(Data));\
}\
void Data##_queue_push(Data##_queue* q, Data data) {\
if (q->back + 1 > q->cap) {\
ptu32 actual_len = q->back - q->front;\
ptu32 new_cap = PointDoubleCapacity(actual_len < q->cap ? actual_len : q->cap);\
Data* old = q->elems;\
q->elems = malloc(new_cap * sizeof(Data));\
memmove(q->elems, &old[q->front], actual_len * sizeof(Data));\
free(old);\
q->front = 0;\
q->back = actual_len;\
q->cap = new_cap;\
}\
q->elems[q->back++] = data;\
}\
ptb8 Data##_queue_pop(Data##_queue* q, Data* ret) {\
if (q->front >= q->back)\
return false;\
*ret = q->elems[q->front++];\
return true;\
}\
Data Data##_queue_peek(Data##_queue* q) {\
return q->elems[q->front];\
}\
void Data##_queue_clear(Data##_queue* q) {\
q->front = 0;\
q->back = 0;\
}\
void Data##_queue_free(Data##_queue* q) {\
q->front = 0;\
q->back = 0;\
q->cap = 0;\
free(q->elems);\
}


//~ OS

void* PointMemoryReserve(ptu64 size);
void  PointMemoryCommit(void* memory, ptu64 size);
void  PointMemoryDecommit(void* memory, ptu64 size);
void  PointMemoryRelease(void* memory, ptu64 size);

//~ Arena (Linear Allocator)

typedef struct PointArena PointArena;
struct PointArena {
  ptu8* memory;
  ptu64 max;
  ptu64 alloc_position;
  ptu64 commit_position;
  ptb8  static_size;
};

#define POINT_ARENA_MAX         PointGigabytes(1)
#define POINT_ARENA_COMMIT_SIZE PointKilobytes(8)

void* PointArenaAllocUnaligned(PointArena* arena, ptu64 size);
void* PointArenaAlloc(PointArena* arena, ptu64 size);
void* PointArenaAllocZero(PointArena* arena, ptu64 size);
void  PointArenaDealloc(PointArena* arena, ptu64 size);
void  PointArenaDeallocTo(PointArena* arena, ptu64 pos);
void* PointArenaRaise(PointArena* arena, void* ptr, ptu64 size);
void* PointArenaAllocArraySized(PointArena* arena, ptu64 elem_size, ptu64 count);

#define PointArenaAllocArray(arena, elem_type, count) \
PointArenaAllocArraySized(arena, sizeof(elem_type), count)

void PointArenaInit(PointArena* arena);
void PointArenaInitSized(PointArena* arena, ptu64 max);
void PointArenaClear(PointArena* arena);
void PointArenaFree(PointArena* arena);

typedef struct PointArenaTemp PointArenaTemp;
struct PointArenaTemp {
  PointArena* arena;
  ptu64 pos;
};

PointArenaTemp PointArenaBeginTemp(PointArena* arena);
void           PointArenaEndTemp(PointArenaTemp temp);

//~ Pool Allocator

typedef struct PointPoolFreeNode PointPoolFreeNode;
struct PointPoolFreeNode { PointPoolFreeNode* next; };

typedef struct PointPool PointPool;
struct PointPool {
	ptu8* memory;
	ptu64 max;
	ptu64 commit_position;
	ptu64 element_size;
	
	PointPoolFreeNode* head;
};

#define POINT_POOL_MAX PointGigabytes(1)
#define POINT_POOL_COMMIT_CHUNK 32

void PointPoolInit(PointPool* pool, ptu64 element_size);
void PointPoolClear(PointPool* pool);
void PointPoolFree(PointPool* pool);

void* PointPoolAlloc(PointPool* pool);
void  PointPoolDealloc(PointPool* pool, void* ptr);
void  PointPoolDeallocRange(PointPool* pool, void* ptr, ptu64 count);

//~ Strings

typedef struct PointString PointString;
struct PointString {
  ptu8* str;
  ptu64 size;
};

typedef struct PointStringNode PointStringNode;
struct PointStringNode {
  PointString str;
  struct PointStringNode* next;
};

typedef struct PointStringList PointStringList;
struct PointStringList {
  PointStringNode* first;
  PointStringNode* last;
  pti32 node_count;
  ptu64 total_size;
};

#define PointStrLit(s) (PointString) { .str = (ptu8*)(s), .size = sizeof(s) - 1 }
#define PointStrMake(s) (PointString) { .str = (ptu8*)(s), .size = strlen(s) }
#define PointStrExpand(s) (pti32)(s).size, (s).str

// NOTE(EVERYONE): this will try to get one extra byte for \0
PointString PointStringAlloc(PointArena* arena, ptu64 size);
PointString PointStringCopy(PointArena* arena, PointString other);
PointString PointStringCat(PointArena* arena, PointString a, PointString b);
PointString PointStringFromFormat(PointArena* arena, const char* format, ...);
PointString PointStringReplaceAll(PointArena* arena, PointString to_fix, PointString needle, PointString replacement);
ptu64 PointStringSubstrCount(PointString str, PointString needle);
ptu64 PointStringFindFirst(PointString str, PointString needle, ptu32 offset);
ptu64 PointStringFindLast(PointString str, PointString needle, ptu32 offset);
ptu32 PointStringHash(PointString str);
ptu64 PointStringHash64(PointString str);

ptb8 PointStringEqual(PointString a, PointString b);
ptb8 PointStringIsNull(PointString k);

void PointStringListPushNode(PointStringList* list, PointStringNode* node);
void PointStringListPush(PointArena* arena, PointStringList* list, PointString str);
ptb8 PointStringListEquals(PointStringList* a, PointStringList* b);
ptb8 PointStringListContains(PointStringList* a, PointString needle);
PointString PointStringListFlatten(PointArena* arena, PointStringList* list);


typedef struct PointString16 PointString16;
struct PointString16 {
  ptu16* str;
  ptu64 size;
};

PointString16 PointStr16CString(ptu16* cstr);
PointString16 PointStr16FromStr8(PointArena* arena, PointString str);
PointString   PointStr8FromStr16(PointArena* arena, PointString16 str);


//~ Types

typedef enum PointTypeKind {
  Point_TK_None,
  Point_TK_Int,
  Point_TK_Float,
  Point_TK_Func,
  Point_TK_Type,
  Point_TK_Void,
  Point_TK_Bool,
  Point_TK_Pointer,
  Point_TK_Array,
  
  Point_TK_MAX,
} PointTypeKind;

typedef struct PointType PointType;

typedef struct PointTypeInt PointTypeInt;
struct PointTypeInt {
  ptb32 is_signed;
};

typedef struct PointTypeFunc PointTypeFunc;
struct PointTypeFunc {
  PointType*  ret_t;
  PointType** arg_ts;
  ptu32       arity;
  ptb32       is_varargs;
};

typedef struct PointTypePointer PointTypePointer;
struct PointTypePointer {
  PointType* sub_t;
};

typedef struct PointTypeArray PointTypeArray;
struct PointTypeArray {
  PointType* sub_t;
  ptu64      count;
};

struct PointType {
  PointTypeKind type;
  ptu64 size;
  ptu64 hash;
  ptu32 uid; // Only used when serializing/deserializing
  
  union {
    PointTypeInt int_t;
    PointTypeFunc func_t;
    PointTypePointer ptr_t;
    PointTypeArray array_t;
  };
};


ptu64 PointTypeHash(PointType* type);
ptb8  PointTypeEquals(PointType* a, PointType* b);
void  PointTypePrint(PointType* type, int indent);

typedef struct PointTypeBucket PointTypeBucket;
struct PointTypeBucket {
  PointTypeBucket* next;
  PointType        type;
};

typedef struct PointTypeCache PointTypeCache;
struct PointTypeCache {
  PointArena misc_arena;
  PointPool type_pool;
  
  PointTypeBucket** buckets;
  ptu64 bucket_count;
  ptu32 type_count;
};

void PointTypeCacheInit(PointArena* arena, PointTypeCache* cache);
void PointTypeCacheFree(PointTypeCache* cache);

PointType* PointTypeCacheRegister(PointTypeCache* cache,   PointType type);
void       PointTypeCacheUnregister(PointTypeCache* cache, PointType* ctype);


//~ Scope Stack

typedef struct PointScopeEntry PointScopeEntry;
struct PointScopeEntry {
  PointScopeEntry* next;
  PointString      name;
  PointType*       type;
  // Other data I can't think of yet, probably
};

typedef struct PointScope PointScope;
struct PointScope {
  PointScope* parent;
  
  PointScope* next;
  PointScope* prev;
  
  PointScope* first;
  PointScope* last;
  
  PointScopeEntry* entries;
  
  ptu32 scope_count;
  ptu32 entry_count;
};


typedef struct PointScopeStack PointScopeStack;
struct PointScopeStack {
  PointArena names_arena;
  PointPool scopes_pool;
  PointPool entries_pool;
  
  PointScope* root;
};

typedef PointScope* PointScopeRef;
PointDStackPrototype(PointScopeRef);

PointScope*      PointScopeCreate(PointScopeStack* scopes, PointScope* parent);
void             PointScopeDelete(PointScopeStack* scopes, PointScope* scope);
PointScopeEntry* PointScopeEntryCreate(PointScopeStack* scopes, PointScope* scope, PointScopeEntry symbol);
void             PointScopeEntryDelete(PointScopeStack* scopes, PointScope* scope, PointScopeEntry* symbol);

void PointScopeStackInit(PointScopeStack* scopes);
void PointScopeStackFree(PointScopeStack* scopes);



//~ Tree

typedef enum PointNodeType {
  // Expressions
  Point_NT_Expr_IntLit, Point_NT_Expr_FloatLit,
  Point_NT_Expr_Add, Point_NT_Expr_Sub, Point_NT_Expr_Mul, Point_NT_Expr_Div,
  Point_NT_Expr_Mod, Point_NT_Expr_Identity, Point_NT_Expr_Negate, Point_NT_Expr_Not,
  Point_NT_Expr_Eq,  Point_NT_Expr_Neq, Point_NT_Expr_Less, Point_NT_Expr_Greater,
  Point_NT_Expr_LessEq, Point_NT_Expr_GreaterEq, Point_NT_Expr_FuncProto, Point_NT_Expr_Func,
  Point_NT_Expr_ShiftLeft, Point_NT_Expr_ShiftRight, Point_NT_Expr_BitAND, Point_NT_Expr_BitOR,
  Point_NT_Expr_Index, Point_NT_Expr_Addr, Point_NT_Expr_Deref, Point_NT_Expr_Call,
  Point_NT_Expr_Ident, Point_NT_Expr_Cast, Point_NT_Expr_Access, Point_NT_Expr_ArrayLit,
  
  // Types
  Point_NT_Type_Integer, Point_NT_Type_Float, Point_NT_Type_Void,
  Point_NT_Type_Func, Point_NT_Type_Struct, Point_NT_Type_Union,
  Point_NT_Type_Pointer, Point_NT_Type_Array,
  
  // Statements
  Point_NT_Stmt_Assign, Point_NT_Stmt_Expr, Point_NT_Stmt_Block,
  Point_NT_Stmt_While, Point_NT_Stmt_If, Point_NT_Stmt_Return, Point_NT_Stmt_Write,
  
  // Declaration
  Point_NT_Decl,
  
  Point_NT_COUNT,
} PointNodeType;

typedef struct PointNode PointNode;

typedef struct PointBinaryOpNode PointBinaryOpNode;
struct PointBinaryOpNode {
  PointNode* left;
  PointNode* right;
};


typedef struct PointUnaryOpNode PointUnaryOpNode;
struct PointUnaryOpNode {
  PointNode* operand;
};

typedef struct PointArrayIndexNode PointArrayIndexNode;
struct PointArrayIndexNode {
  PointNode* left;
  PointNode* idx;
};

typedef struct PointFuncCallNode PointFuncCallNode;
struct PointFuncCallNode {
  PointNode* called;
  PointNode* args;
  ptu32      arity;
};

typedef struct PointFuncProtoNode PointFuncProtoNode;
struct PointFuncProtoNode {
  PointNode*       return_type;
  PointNode*       arg_types;
  
  // Array, and not a Linked List
  PointScopeEntry* arg_names;
  ptu32            arity;
};

typedef struct PointFuncNode PointFuncNode;
struct PointFuncNode {
  PointNode* proto;
  PointNode* body;
  ptu64 total_local_size;
  ptu64 tracking_local_size;
};

typedef struct PointCastNode PointCastNode;
struct PointCastNode {
  PointNode* casted;
  PointNode* type;
};

typedef struct PointAccessNode PointAccessNode;
struct PointAccessNode {
  PointNode* left;
  PointScopeEntry right;
  ptb8 deref;
};

typedef struct PointArrayLitNode PointArrayLitNode;
struct PointArrayLitNode {
  PointNode* type;
  PointNode* values;
  ptu32 count;
};

typedef struct PointIntegerTypeNode PointIntegerTypeNode;
struct PointIntegerTypeNode {
  ptu32 size;
  ptb8 is_signed;
};

typedef struct PointFloatTypeNode PointFloatTypeNode;
struct PointFloatTypeNode {
  ptu32 size;
};

typedef struct PointPointerTypeNode PointPointerTypeNode;
struct PointPointerTypeNode {
  PointNode* sub;
};

typedef struct PointArrayTypeNode PointArrayTypeNode;
struct PointArrayTypeNode {
  PointNode* count;
  PointNode* sub;
};

typedef struct PointFuncTypeNode PointFuncTypeNode;
struct PointFuncTypeNode {
  PointNode* return_type;
  PointNode* arg_types;
  ptu32      arity;
};

typedef struct PointWhileLoopNode PointWhileLoopNode;
struct PointWhileLoopNode {
  PointNode*  condition;
  PointNode*  body;
  PointScope* scope;
};

typedef struct PointIfStmtNode PointIfStmtNode;
struct PointIfStmtNode {
  PointNode*  condition;
  PointNode*  then_body;
  PointScope* then_scope;
  PointNode*  else_body;
  PointScope* else_scope;
};

typedef struct PointDeclNode PointDeclNode;
struct PointDeclNode {
  PointScopeEntry ref;
  PointNode*      type;
  PointNode*      val;
  
  ptb8 is_constant;
};


typedef enum PointConstantValueType {
  CVT_None,
  CVT_Int,
  CVT_Float,
  CVT_Type,
  CVT_Buffer,
} PointConstantValueType;

typedef struct PointConstantValue PointConstantValue;
struct PointConstantValue {
  PointConstantValueType type;
  
  union {
    pti64 int_lit;
    ptf64 float_lit;
    PointType* type_lit;
    struct { ptu8* buf; ptu64 size; } buf_lit;
  };
};


struct PointNode {
  // Basic Things
  PointNodeType type;
  PointNode* next;
  
  ptu64 hash;
  PointNode* hash_next;
  
  // Constant Values
  ptb8 is_constant;
  PointConstantValue constant_val;
  
  PointType* expr_type;
  
  // Subtypes
  union {
    PointScopeEntry* ident;
    PointArrayLitNode array_lit;
    PointBinaryOpNode binary_op;
    PointUnaryOpNode  unary_op;
    PointFuncProtoNode proto;
    PointFuncNode func;
    PointArrayIndexNode index;
    PointFuncCallNode call;
    PointNode* addr;
    PointNode* deref;
    PointCastNode cast;
    PointAccessNode access;
    
    PointIntegerTypeNode int_type;
    PointFloatTypeNode float_type;
    PointFuncTypeNode func_type;
    PointArrayTypeNode array_type;
    PointPointerTypeNode pointer_type;
    
    PointNode* expr_stmt;
    PointNode* return_stmt;
    PointWhileLoopNode while_loop;
    PointIfStmtNode if_stmt;
    PointNode* block;
    PointDeclNode decl;
  };
};

typedef struct PointProgram PointProgram;
struct PointProgram {
  PointArena  node_arena;
  
  PointScopeStack scopes;
  PointNode*       decls;
  PointTypeCache   types;
};

void PointProgramInit(PointProgram* prog);
void PointProgramLoad(PointProgram* prog, PointString filename);
void PointProgramSave(PointProgram* prog, PointString filename);
void PointProgramFree(PointProgram* prog);

#endif //POINT_H



#ifdef POINT_IMPLEMENTATION
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <stdbool.h>


//~ OS IMPL


#if defined(POINT_PLATFORM_WIN)
#  include <Windows.h>
#elif defined(POINT_PLATFORM_LINUX)
#  include <sys/mman.h>
#endif


#if defined(POINT_PLATFORM_WIN)
void* PointMemoryReserve(ptu64 size) {
  return VirtualAlloc(0, size, MEM_RESERVE, PAGE_NOACCESS);
}

void  PointMemoryCommit(void* memory, ptu64 size){
  VirtualAlloc(memory, size, MEM_COMMIT, PAGE_READWRITE);
}

void  PointMemoryDecommit(void* memory, ptu64 size) {
  VirtualFree(memory, size, MEM_DECOMMIT);
}

void  PointMemoryRelease(void* memory, ptu64 size) {
  VirtualFree(memory, 0, MEM_RELEASE);
}

#elif defined(POINT_PLATFORM_LINUX)

void* PointMemoryReserve(ptu64 size) {
  return mmap(NULL, size, PROT_NONE, MAP_PRIVATE | MAP_ANON, -1, 0);
}

void  PointMemoryCommit(void* memory, ptu64 size) {
  mprotect(memory, size, PROT_READ | PROT_WRITE);
}

void  PointMemoryDecommit(void* memory, ptu64 size) {
  mprotect(memory, size, PROT_NONE);
}

void  PointMemoryRelease(void* memory, ptu64 size) {
  munmap(memory, size);
}
#endif // PLATFORM


#define POINT_DEFAULT_ALIGNMENT (sizeof(void*))

static ptb8 pt_is_power_of_two(uintptr_t x) {
  return (x & (x-1)) == 0;
}

static ptu64 pt_align_forward_u64(ptu64 ptr, ptu64 align) {
  ptu64 p, a, modulo;
  assert(pt_is_power_of_two(align));
  p = ptr;
  a = (size_t)align;
  modulo = p & (a-1);
  if (modulo != 0) { p += a - modulo; }
  return p;
}


//~ Arena


void* PointArenaAllocUnaligned(PointArena* arena, ptu64 size) {
  void* memory = 0;
  
  if (arena->alloc_position + size > arena->commit_position) {
    if (!arena->static_size) {
      ptu64 commit_size = size;
      
      commit_size += POINT_ARENA_COMMIT_SIZE - 1;
      commit_size -= commit_size % POINT_ARENA_COMMIT_SIZE;
      
      if (arena->commit_position >= arena->max) {
        assert(0 && "Arena is out of memory");
      } else {
        PointMemoryCommit(arena->memory + arena->commit_position, commit_size);
        arena->commit_position += commit_size;
      }
    } else {
      assert(0 && "Static-Size Arena is out of memory");
    }
  }
  
  memory = arena->memory + arena->alloc_position;
  arena->alloc_position += size;
  return memory;
}


void* PointArenaAlloc(PointArena* arena, ptu64 size) {
  ptu64 pos = pt_align_forward_u64(arena->alloc_position, POINT_DEFAULT_ALIGNMENT);
  ptu64 offset = pos - arena->alloc_position;
  size += offset;
  return (void*)((ptu8*)PointArenaAllocUnaligned(arena, size) + offset);
}

void* PointArenaAllocZero(PointArena* arena, ptu64 size) {
  void* result = PointArenaAlloc(arena, size);
  memset(result, 0, size);
  return result;
}

void PointArenaDealloc(PointArena* arena, ptu64 size) {
  if (size > arena->alloc_position)
    size = arena->alloc_position;
  arena->alloc_position -= size;
}

void PointArenaDeallocTo(PointArena* arena, ptu64 pos) {
  if (pos > arena->max) pos = arena->max;
  if (pos < 0) pos = 0;
  arena->alloc_position = pos;
}

void* PointArenaRaise(PointArena* arena, void* ptr, ptu64 size) {
  void* raised = PointArenaAlloc(arena, size);
  memcpy(raised, ptr, size);
  return raised;
}

void* PointArenaAllocArraySized(PointArena* arena, ptu64 elem_size, ptu64 count) {
  return PointArenaAlloc(arena, elem_size * count);
}

void PointArenaInit(PointArena* arena) {
  PointMemoryZeroStruct(arena, PointArena);
  arena->max = POINT_ARENA_MAX;
  arena->memory = PointMemoryReserve(arena->max);
  arena->alloc_position = 0;
  arena->commit_position = 0;
  arena->static_size = false;
}

void PointArenaInitSized(PointArena* arena, ptu64 max) {
  PointMemoryZeroStruct(arena, PointArena);
  arena->max = max;
  arena->memory = PointMemoryReserve(arena->max);
  arena->alloc_position = 0;
  arena->commit_position = 0;
  arena->static_size = false;
}

void PointArenaClear(PointArena* arena) {
  PointArenaDealloc(arena, arena->alloc_position);
}

void PointArenaFree(PointArena* arena) {
  PointMemoryRelease(arena->memory, arena->max);
}

//~ Temp arena

PointArenaTemp PointArenaBeginTemp(PointArena* arena) {
  return (PointArenaTemp) { arena, arena->alloc_position };
}

void PointArenaEndTemp(PointArenaTemp temp) {
  PointArenaDeallocTo(temp.arena, temp.pos);
}

//~ Pool Allocator

void PointPoolInit(PointPool* pool, ptu64 element_size) {
  PointMemoryZeroStruct(pool, PointPool);
  pool->memory = PointMemoryReserve(POINT_POOL_MAX);
  pool->max = POINT_POOL_MAX;
  pool->commit_position = 0;
  pool->element_size = pt_align_forward_u64(element_size, POINT_DEFAULT_ALIGNMENT);
  pool->head = NULL;
}

void PointPoolClear(PointPool* pool) {
  for (ptu8* it = pool->memory, *preit = it;
       it <= (ptu8*)pool->memory + pool->commit_position;
       preit = it, it += pool->element_size) {
    ((PointPoolFreeNode*)preit)->next = (PointPoolFreeNode*)it;
  }
  pool->head = (PointPoolFreeNode*)pool->memory;
}

void PointPoolFree(PointPool* pool) {
  PointMemoryRelease(pool->memory, pool->max);
}

void* PointPoolAlloc(PointPool* pool) {
  if (pool->head) {
    void* ret = pool->head;
    pool->head = pool->head->next;
    PointMemoryZero(ret, pool->element_size);
    return ret;
  } else {
    if (pool->commit_position + POINT_POOL_COMMIT_CHUNK * pool->element_size >= pool->max) {
      assert(0 && "Pool is out of memory");
      return NULL;
    }
    void* commit_ptr = pool->memory + pool->commit_position;
    PointMemoryCommit(commit_ptr, POINT_POOL_COMMIT_CHUNK * pool->element_size);
    PointPoolDeallocRange(pool, commit_ptr, POINT_POOL_COMMIT_CHUNK);
    
    pool->commit_position += POINT_POOL_COMMIT_CHUNK * pool->element_size;
    
    return PointPoolAlloc(pool);
  }
}

void PointPoolDealloc(PointPool* pool, void* ptr) {
  ((PointPoolFreeNode*)ptr)->next = pool->head;
  pool->head = ptr;
}

void PointPoolDeallocRange(PointPool* pool, void* ptr, ptu64 count) {
  ptu8* it = ptr;
  for (ptu64 k = 0; k < count; k++) {
    ((PointPoolFreeNode*)it)->next = pool->head;
    pool->head = (PointPoolFreeNode*) it;
    it += pool->element_size;
  }
}


//~ String

PointString PointStringAlloc(PointArena* arena, ptu64 size) {
  PointString str = {0};
  str.str = (ptu8*)PointArenaAlloc(arena, size + 1);
  str.str[size] = '\0';
  str.size = size;
  return str;
}

PointString PointStringCopy(PointArena* arena, PointString other) {
  PointString str = {0};
  str.str = (ptu8*)PointArenaAlloc(arena, other.size + 1);
  str.size = other.size;
  memcpy(str.str, other.str, other.size);
  str.str[str.size] = '\0';
  return str;
}

PointString PointStringCat(PointArena* arena, PointString a, PointString b) {
  PointString final = {0};
  final.size = a.size + b.size;
  final.str = (ptu8*)PointArenaAlloc(arena, final.size + 1);
  memcpy(final.str, a.str, a.size);
  memcpy(final.str + a.size, b.str, b.size);
  final.str[final.size] = '\0';
  return final;
}

PointString PointStringFromFormat(PointArena* arena, const char* format, ...) {
  va_list args;
  va_start(args, format);
  const char buf[8092];
  vsnprintf(buf, 8092, format, args);
  va_end(args);
  ptu64 size = strlen(buf);
  PointString s = PointStringAlloc(arena, size);
  memmove(s.str, buf, size);
  return s;
}

ptb8 PointStringEqual(PointString a, PointString b) {
  if (a.size != b.size) return false;
  return memcmp(a.str, b.str, b.size) == 0;
}

ptb8 PointStringIsNull(PointString k) {
  return k.str == 0 && k.size == 0;
}

PointString PointStringReplaceAll(PointArena* arena, PointString to_fix, PointString needle, PointString replacement) {
  if (needle.size == 0) return to_fix;
  ptu64 replaceable = PointStringSubstrCount(to_fix, needle);
  if (replaceable == 0) return to_fix;
  
  ptu64 new_size = (to_fix.size - replaceable * needle.size) + (replaceable * replacement.size);
  PointString ret = PointStringAlloc(arena, new_size);
  
  ptb8 replaced;
  ptu64 o = 0;
  for (ptu64 i = 0; i < to_fix.size;) {
    replaced = false;
    if (to_fix.str[i] == needle.str[0]) {
      if ((to_fix.size - i) >= needle.size) {
        PointString sub = { .str = to_fix.str + i, .size = needle.size };
        if (PointStringEqual(sub, needle)) {
          // replace this one
          memmove(ret.str + o, replacement.str, replacement.size);
          replaced = true;
        }
      }
    }
    
    if (replaced) {
      o += replacement.size;
      i += needle.size;
      continue;
    }
    
    ret.str[o] = to_fix.str[i];
    o++; i++;
  }
  
  return ret;
}

ptu64 PointStringSubstrCount(PointString str, PointString needle) {
  ptu32 ct = 0;
  ptu64 idx = 0;
  while (true) {
    idx = PointStringFindFirst(str, needle, idx);
    if (idx == str.size)
      break;
    ct++;
    idx++;
  }
  return ct;
}

ptu64 PointStringFindFirst(PointString str, PointString needle, ptu32 offset) {
  ptu64 i = 0;
  if (needle.size > 0) {
    i = str.size;
    if (str.size >= needle.size) {
      i = offset;
      pti8 c = needle.str[0];
      ptu64 one_past_last = str.size - needle.size + 1;
      for (; i < one_past_last; i++) {
        if (str.str[i] == c) {
          if ((str.size - i) >= needle.size) {
            PointString sub = { .str = str.str + i, .size = needle.size };
            if (PointStringEqual(sub, needle)) break;
          }
        }
      }
      if (i == one_past_last) {
        i = str.size;
      }
    }
  }
  return i;
}

ptu64 PointStringFindLast(PointString str, PointString needle, ptu32 offset) {
  ptu64 prev = str.size;
  if (offset == 0)
    offset = str.size;
  ptu64 idx = 0;
  while (true) {
    prev = idx;
    idx = PointStringFindFirst(str, needle, idx);
    if (idx >= offset) break;
    idx++;
  }
  return prev;
}

ptu32 PointStringHash(PointString str) {
  ptu32 hash = 2166136261u;
  for (int i = 0; i < str.size; i++) {
    hash ^= str.str[i];
    hash *= 16777619;
  }
  return hash;
}

ptu64 PointStringHash64(PointString str) {
  ptu64 hash = 2166136261u;
  for (int i = 0; i < str.size; i++) {
    hash ^= str.str[i];
    hash *= 16777619;
  }
  return hash;
}

void PointStringListPushNode(PointStringList* list, PointStringNode* node) {
  if (!list->first && !list->last) {
    list->first = node;
    list->last = node;
  } else {
    list->last->next = node;
    list->last = node;
  }
  list->node_count += 1;
  list->total_size += node->str.size;
}

void PointStringListPush(PointArena* arena, PointStringList* list, PointString str) {
  PointStringNode* node = (PointStringNode*) PointArenaAlloc(arena, sizeof(PointStringNode));
  node->str = str;
  PointStringListPushNode(list, node);
}

ptb8 PointStringListEquals(PointStringList* a, PointStringList* b) {
  if (a->total_size != b->total_size) return false;
  if (a->node_count != b->node_count) return false;
  PointStringNode* curr_a = a->first;
  PointStringNode* curr_b = b->first;
  
  while (curr_a != NULL || curr_b != NULL) {
    if (!PointStringEqual(curr_a->str, curr_b->str)) return false;
    
    curr_a = curr_a->next;
    curr_b = curr_b->next;
  }
  return true;
}

ptb8 PointStringListContains(PointStringList* a, PointString needle) {
  PointStringNode* curr = a->first;
  while (curr != NULL) {
    if (PointStringEqual(needle, curr->str))
      return true;
    curr = curr->next;
  }
  return false;
}

PointString PointStringListFlatten(PointArena* arena, PointStringList* list) {
  PointString final = PointStringAlloc(arena, list->total_size);
  ptu64 current_offset = 0;
  for (PointStringNode* node = list->first; node != NULL; node = node->next) {
    memcpy(final.str + current_offset, node->str.str, node->str.size);
    current_offset += node->str.size;
  }
  return final;
}

//~ Encoding stuff

typedef struct str_decode {
  ptu32 codepoint;
  ptu32 size;
} str_decode;

PointString16 PointStr16CString(ptu16 *cstr){
  ptu16 *ptr = cstr;
  for (;*ptr != 0; ptr += 1);
  PointString16 result = { cstr, (ptu64) (ptr - cstr) };
  return result;
}

static str_decode str_decode_utf8(ptu8 *str, ptu32 cap){
  ptu8 length[] = {
    1, 1, 1, 1, // 000xx
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    0, 0, 0, 0, // 100xx
    0, 0, 0, 0,
    2, 2, 2, 2, // 110xx
    3, 3,     // 1110x
    4,      // 11110
    0,      // 11111
  };
  ptu8 first_byte_mask[] = { 0, 0x7F, 0x1F, 0x0F, 0x07 };
  ptu8 final_shift[] = { 0, 18, 12, 6, 0 };
  
  str_decode result = {0};
  if (cap > 0) {
    result.codepoint = '#';
    result.size = 1;
    
    ptu8 byte = str[0];
    ptu8 l = length[byte >> 3];
    if (0 < l && l <= cap) {
      ptu32 cp = (byte & first_byte_mask[l]) << 18;
      switch (l) {
        case 4: cp |= ((str[3] & 0x3F) << 0);
        case 3: cp |= ((str[2] & 0x3F) << 6);
        case 2: cp |= ((str[1] & 0x3F) << 12);
        default: break;
      }
      cp >>= final_shift[l];
      
      result.codepoint = cp;
      result.size = l;
    }
  }
  
  return result;
}

static ptu32 str_encode_utf8(ptu8 *dst, ptu32 codepoint){
  ptu32 size = 0;
  if (codepoint < (1 << 8)) {
    dst[0] = codepoint;
    size = 1;
  } else if (codepoint < (1 << 11)) {
    dst[0] = 0xC0 | (codepoint >> 6);
    dst[1] = 0x80 | (codepoint & 0x3F);
    size = 2;
  } else if (codepoint < (1 << 16)) {
    dst[0] = 0xE0 | (codepoint >> 12);
    dst[1] = 0x80 | ((codepoint >> 6) & 0x3F);
    dst[2] = 0x80 | (codepoint & 0x3F);
    size = 3;
  } else if (codepoint < (1 << 21)) {
    dst[0] = 0xF0 | (codepoint >> 18);
    dst[1] = 0x80 | ((codepoint >> 12) & 0x3F);
    dst[2] = 0x80 | ((codepoint >> 6) & 0x3F);
    dst[3] = 0x80 | (codepoint & 0x3F);
    size = 4;
  } else {
    dst[0] = '#';
    size = 1;
  }
  return size;
}

static str_decode str_decode_utf16(ptu16 *str, ptu32 cap){
  str_decode result = {'#', 1};
  ptu16 x = str[0];
  if (x < 0xD800 || 0xDFFF < x) {
    result.codepoint = x;
  } else if (cap >= 2) {
    ptu16 y = str[1];
    if (0xD800 <= x && x < 0xDC00 &&
        0xDC00 <= y && y < 0xE000) {
      ptu16 xj = x - 0xD800;
      ptu16 yj = y - 0xDc00;
      ptu32 xy = (xj << 10) | yj;
      result.codepoint = xy + 0x10000;
      result.size = 2;
    }
  }
  return result;
}

static ptu32 str_encode_utf16(ptu16 *dst, ptu32 codepoint){
  ptu32 size = 0;
  if (codepoint < 0x10000) {
    dst[0] = codepoint;
    size = 1;
  } else {
    ptu32 cpj = codepoint - 0x10000;
    dst[0] = (cpj >> 10) + 0xD800;
    dst[1] = (cpj & 0x3FF) + 0xDC00;
    size = 2;
  }
  return(size);
}

PointString16 PointStr16FromStr8(PointArena *arena, PointString str) {
  ptu16* memory = PointArenaAllocArray(arena, ptu16, str.size * 2 + 1);
  
  ptu16* dptr = memory;
  ptu8* ptr = str.str;
  ptu8* opl = str.str + str.size;
  for (; ptr < opl;) {
    str_decode decode = str_decode_utf8(ptr, (ptu64)(opl - ptr));
    ptu32 enc_size = str_encode_utf16(dptr, decode.codepoint);
    ptr += decode.size;
    dptr += enc_size;
  }
  
  *dptr = 0;
  
  ptu64 alloc_count = str.size*2 + 1;
  ptu64 string_count = (ptu64)(dptr - memory);
  ptu64 unused_count = alloc_count - string_count - 1;
  PointArenaDealloc(arena, unused_count * sizeof(*memory));
  
  PointString16 result = { memory, string_count };
  return result;
}

PointString PointStr8FromStr16(PointArena *arena, PointString16 str) {
  ptu8 *memory = PointArenaAllocArray(arena, ptu8, str.size * 3 + 1);
  
  ptu8 *dptr = memory;
  ptu16 *ptr = str.str;
  ptu16 *opl = str.str + str.size;
  for (; ptr < opl;) {
    str_decode decode = str_decode_utf16(ptr, (ptu64)(opl - ptr));
    ptu16 enc_size = str_encode_utf8(dptr, decode.codepoint);
    ptr += decode.size;
    dptr += enc_size;
  }
  
  *dptr = 0;
  
  ptu64 alloc_count = str.size*3 + 1;
  ptu64 string_count = (ptu64)(dptr - memory);
  ptu64 unused_count = alloc_count - string_count - 1;
  PointArenaDealloc(arena, unused_count * sizeof(*memory));
  
  PointString result = { memory, string_count };
  return result;
}


//~ Scope Stack

PointScope* PointScopeCreate(PointScopeStack* scopes, PointScope* parent) {
  PointScope* scope = PointPoolAlloc(&scopes->scopes_pool);
  if (!parent) return scope;
  parent->scope_count += 1;
  
  scope->parent = parent;
  // DLL_Insert
  if (!parent->first || !parent->last) {
    parent->first = scope;
    parent->last = scope;
  } else {
    parent->last->next = scope;
    scope->prev = parent->last;
    parent->last = scope;
  }
  
  return scope;
}

void PointScopeDelete(PointScopeStack* scopes, PointScope* scope) {
  if (!scope->parent) {
    PointPoolDealloc(&scopes->scopes_pool, scope);
    scopes->root = NULL;
  } else {
    // DLL_Remove
    PointScope* parent = scope->parent;
    if (parent->first == scope) parent->first = scope->next;
    if (parent->last  == scope) parent->last  = scope->prev;
    if (scope->prev) scope->prev->next = scope->next;
    if (scope->next) scope->next->prev = scope->prev;
    
    parent->scope_count -= 1;
    PointPoolDealloc(&scopes->scopes_pool, scope);
  }
}

PointScopeEntry* PointScopeEntryCreate(PointScopeStack* scopes, PointScope* scope, PointScopeEntry symbol) {
  PointScopeEntry* entry = PointPoolAlloc(&scopes->entries_pool);
  memmove(entry, &symbol, sizeof(PointScopeEntry));
  scope->entry_count += 1;
  PointScopeEntry* iter = scope->entries;
  if (iter) {
    while (iter->next) iter = iter->next;
    iter->next = entry;
  } else {
    scope->entries = entry;
  }
  
  return entry;
}

void PointScopeEntryDelete(PointScopeStack* scopes, PointScope* scope, PointScopeEntry* symbol) {
  PointScopeEntry* iter = scope->entries;
  while (iter->next != symbol) iter = iter->next;
  iter->next = symbol->next;
  PointPoolDealloc(&scopes->entries_pool, symbol);
}

void PointScopeStackInit(PointScopeStack* scopes) {
  PointMemoryZeroStruct(scopes, PointScopeStack);
  PointArenaInit(&scopes->names_arena);
  PointPoolInit(&scopes->scopes_pool, sizeof(PointScope));
  PointPoolInit(&scopes->entries_pool, sizeof(PointScopeEntry));
  
  scopes->root = PointScopeCreate(scopes, NULL);
}

void PointScopeStackFree(PointScopeStack* scopes) {
  PointPoolFree(&scopes->scopes_pool);
  PointPoolFree(&scopes->entries_pool);
  PointArenaFree(&scopes->names_arena);
}

//~ Types

#define PT_FNV64_BASE  14695981039346656037ULL
#define PT_FNV64_PRIME 1099511628211ULL
ptu64 PointTypeHash(PointType* type) {
  ptu64 hash = PT_FNV64_BASE;
  hash ^= type->type;
  hash *= PT_FNV64_PRIME;
  hash ^= type->size;
  hash *= PT_FNV64_PRIME;
  
  switch (type->type) {
    case Point_TK_None: {} break;
    
    case Point_TK_Int: {
      hash ^= type->int_t.is_signed;
      hash *= PT_FNV64_PRIME;
    } break;
    
    case Point_TK_Float: {} break;
    
    case Point_TK_Func: {
      hash ^= type->func_t.ret_t->hash;
      hash *= PT_FNV64_PRIME;
      for (int i = 0; i < type->func_t.arity; i++) {
        hash ^= type->func_t.arg_ts[i]->hash;
        hash *= PT_FNV64_PRIME;
      }
    } break;
    
    case Point_TK_Type: {} break;
    case Point_TK_Void: {} break;
    case Point_TK_Bool: {} break;
    
    case Point_TK_Pointer: {
      hash ^= type->ptr_t.sub_t->hash;
      hash *= PT_FNV64_PRIME;
    } break;
    
    case Point_TK_Array: {
      hash ^= type->array_t.sub_t->hash;
      hash *= PT_FNV64_PRIME;
      hash ^= type->array_t.count;
      hash *= PT_FNV64_PRIME;
    } break;
    
  }
  
  type->hash = hash;
  return hash;
}

ptb8 PointTypeEquals(PointType* a, PointType* b) {
  if (a == b) return true;
  if (a->hash == b->hash) return true;
  return false;
}

void PointTypePrint(PointType* type, int indent) {
  for (int i = 0; i < indent; i++) printf("  ");
  
  switch (type->type) {
    case Point_TK_None: { printf("None\n"); } break;
    
    case Point_TK_Int: {
      printf("Int (%llu bytes)\n", type->size);
    } break;
    
    case Point_TK_Float: {
      printf("Float (%llu bytes)\n", type->size);
    } break;
    
    case Point_TK_Func: {
      printf("Func (%d args)%s\n", type->func_t.arity, type->func_t.is_varargs ? " + varargs" : "");
      for (int i = 0; i < indent; i++) printf("  ");
      printf("Returns:\n");\
      PointTypePrint(type->func_t.ret_t, indent+1);
      for (int i = 0; i < indent; i++) printf("  ");
      printf("Arguments:\n");
      for (int i = 0; i < type->func_t.arity; i++) {
        PointTypePrint(type->func_t.arg_ts[i], indent+1);
      }
    } break;
    
    case Point_TK_Type: { printf("Type\n"); } break;
    case Point_TK_Void: { printf("Void\n"); } break;
    case Point_TK_Bool: { printf("Bool\n"); } break;
    
    case Point_TK_Pointer: {
      printf("Pointer:\n");
      PointTypePrint(type->ptr_t.sub_t, indent+1);
    } break;
    
    case Point_TK_Array: {
      printf("Array (%llu elements):\n", type->array_t.count); 
      PointTypePrint(type->array_t.sub_t, indent+1);
    } break;
    
  }
  
}

//~ Type Cache



void PointTypeCacheInit(PointArena* arena, PointTypeCache* cache) {
  PointMemoryZeroStruct(cache, PointTypeCache);
  PointArenaInit(&cache->misc_arena);
  PointPoolInit(&cache->type_pool, sizeof(PointTypeBucket));
  cache->bucket_count = 32; // NOTE(voxel): Make Configurable?
  cache->type_count   = 0;
  cache->buckets      = PointArenaAlloc(arena, cache->bucket_count * sizeof(PointTypeBucket*));
}

void PointTypeCacheFree(PointTypeCache* cache) {
  for (int i = 0; i < cache->bucket_count; i++)
    cache->buckets[i] = NULL;
  cache->type_count = 0;
  PointPoolFree(&cache->type_pool);
  PointArenaFree(&cache->misc_arena);
}

PointType* PointTypeCacheRegister(PointTypeCache* cache, PointType type) {
  ptu64 hash = PointTypeHash(&type);
  ptu64 bucket_index = hash % cache->bucket_count;
  
  PointTypeBucket* bucket = cache->buckets[bucket_index];
  PointTypeBucket** assignee = NULL;
  if (!bucket) {
    assignee = &cache->buckets[bucket_index];
  } else {
    PointTypeBucket* iter = bucket;
    while (!PointTypeEquals(&iter->type, &type) && iter->next) {
      iter = iter->next;
    }
    
    // Found already registered copy
    if (PointTypeEquals(&iter->type, &type))
      return &iter->type;
    
    assignee = &iter->next;
  }
  
  PointTypeBucket* new_alloc = PointPoolAlloc(&cache->type_pool);
  memmove(&new_alloc->type, &type, sizeof(PointType));
  cache->type_count += 1;
  *assignee = new_alloc;
  
  return &new_alloc->type;
}

void PointTypeCacheUnregister(PointTypeCache* cache, PointType* ctype) {
  ptu64 bucket_index = ctype->hash % cache->bucket_count;
  
  PointTypeBucket* trail = NULL;
  PointTypeBucket* bucket = cache->buckets[bucket_index];
  while (bucket && !PointTypeEquals(&bucket->type, ctype)) {
    trail = bucket;
    bucket = bucket->next;
  }
  
  if (!bucket) return; // NOTE(voxel): Maybe assertion fail here
  if (!trail) {
    // The type was the first in the bucket.
    cache->buckets[bucket_index] = bucket->next;
  } else {
    // The type was NOT the first in the bucket.
    trail->next = bucket->next;
  }
  
  PointPoolDealloc(&cache->type_pool, bucket);
}

//~ Program

void PointProgramInit(PointProgram* prog) {
  PointMemoryZeroStruct(prog, PointProgram);
  PointArenaInit(&prog->node_arena);
  PointScopeStackInit(&prog->scopes);
  prog->decls = NULL;
  PointTypeCacheInit(&prog->node_arena, &prog->types);
}

void PointProgramFree(PointProgram* prog) {
  PointTypeCacheFree(&prog->types);
  PointScopeStackFree(&prog->scopes);
  
  PointArenaFree(&prog->node_arena);
}


//~ Program File Format Helpers


static void* PointRead(void** ptr, size_t type_size) {
  void* ret = *ptr;
  *((ptu8**)ptr) += type_size;
  return ret;
}

#define PointReadType(ptr, type) (*((type*)PointRead(&ptr, sizeof(type))))


#define POINT_MAGIC_NUMBER 0xBEBEBE544E494F50llu
#define POINT_MAGIC_TYPES  0x45505954u
#define POINT_MAGIC_SCOPES 0x504F4353u
#define POINT_MAGIC_NODES  0x45444F4Eu

typedef struct POINTHEADER POINTHEADER;
struct POINTHEADER {
  ptu64 magic_number;
  ptu32 magic_types;
  ptu32 seg_types;
  ptu32 magic_scopes;
  ptu32 seg_scopes;
  ptu32 magic_nodes;
  ptu32 seg_nodes;
};

typedef struct POINTREDUCEDTYPEHEADER POINTREDUCEDTYPEHEADER;
struct POINTREDUCEDTYPEHEADER {
  PointTypeKind type;
  ptu64 size;
};

typedef struct POINTREDUCEDTYPE POINTREDUCEDTYPE;
struct POINTREDUCEDTYPE {
  PointTypeKind type;
  ptu64 size;
  PointType* canonical;
  
  union {
    struct { ptb32  is_signed;                                                   } int_t;
    struct { ptu32  ret_uid;     ptu32* arg_uids; ptu32 arity; ptb32 is_varargs; } func_t;
    struct { ptu32  sub_uid;                                                     } ptr_t;
    struct { ptu32  sub_uid;     ptu64 count;                                    } array_t;
  };
};

typedef POINTREDUCEDTYPE* POINTREDUCEDTYPEREF;
PointDQueuePrototype(POINTREDUCEDTYPEREF);
PointDQueueImpl(POINTREDUCEDTYPEREF);

static void PointTypeSerialize(PointArena* arena, PointType* type) {
  POINTREDUCEDTYPEHEADER* header = PointArenaAllocUnaligned(arena, sizeof(POINTREDUCEDTYPEHEADER));
  header->type = type->type;
  header->size = type->size;
  
  switch (type->type) {
    case Point_TK_None:
    case Point_TK_Float:
    case Point_TK_Type:
    case Point_TK_Void:
    case Point_TK_Bool:
    case Point_TK_MAX: {} break;
    
    case Point_TK_Int: {
      ptb32* is_signed = PointArenaAllocUnaligned(arena, sizeof(ptb32));
      *is_signed = type->int_t.is_signed;
    } break;
    
    case Point_TK_Func: {
      ptu32* arity = PointArenaAllocUnaligned(arena, sizeof(ptu32));
      *arity = type->func_t.arity;
      ptb32* is_varargs = PointArenaAllocUnaligned(arena, sizeof(ptb32));
      *is_varargs = type->func_t.is_varargs;
      
      ptu32* ids_buf = PointArenaAllocUnaligned(arena, sizeof(ptu32) * ((*arity) + 1));
      ids_buf[0] = type->func_t.ret_t->uid;
      for (int i = 0; i < *arity; i++)
        ids_buf[i+1] = type->func_t.arg_ts[i]->uid;
    } break;
    
    case Point_TK_Pointer: {
      ptu32* sub_id = PointArenaAllocUnaligned(arena, sizeof(ptu32));
      *sub_id = type->ptr_t.sub_t->uid;
    } break;
    
    case Point_TK_Array: {
      ptu64* count = PointArenaAllocUnaligned(arena, sizeof(ptu64));
      *count = type->array_t.count;
      ptu32* sub_id = PointArenaAllocUnaligned(arena, sizeof(ptu32));
      *sub_id = type->array_t.sub_t->uid;
    } break;
  }
}

static void PointTypeDeserialize(PointArena* misc_arena, POINTREDUCEDTYPE* slot, void** ptr) {
  void* track = *ptr;
  POINTREDUCEDTYPEHEADER header = PointReadType(track, POINTREDUCEDTYPEHEADER);
  slot->type = header.type;
  slot->size = header.size;
  slot->canonical = NULL;
  switch (header.type) {
    case Point_TK_None:
    case Point_TK_Float:
    case Point_TK_Type:
    case Point_TK_Void:
    case Point_TK_Bool:
    case Point_TK_MAX: {} break;
    
    case Point_TK_Int: {
      slot->int_t.is_signed = PointReadType(track, ptb32);
    } break;
    
    case Point_TK_Func: {
      slot->func_t.arity      = PointReadType(track, ptu32);
      slot->func_t.is_varargs = PointReadType(track, ptb32);
      
      slot->func_t.ret_uid    = PointReadType(track, ptu32);
      slot->func_t.arg_uids   = PointArenaAlloc(misc_arena, sizeof(ptu32) * slot->func_t.arity);
      for (int i = 0; i < slot->func_t.arity; i++)
        slot->func_t.arg_uids[i] = PointReadType(track, ptu32);
    } break;
    
    case Point_TK_Pointer: {
      slot->ptr_t.sub_uid = PointReadType(track, ptu32);
    } break;
    
    case Point_TK_Array: {
      slot->array_t.count   = PointReadType(track, ptu64);
      slot->array_t.sub_uid = PointReadType(track, ptu32);
    } break;
  }
  
  *ptr = track;
}

static ptb8 PointTypeCheckDependencies(POINTREDUCEDTYPE* curr, POINTREDUCEDTYPE* statuses) {
  ptb8 ret = true;
  
  switch (curr->type) {
    case Point_TK_None:
    case Point_TK_Float:
    case Point_TK_Type:
    case Point_TK_Void:
    case Point_TK_Bool:
    case Point_TK_Int:
    case Point_TK_MAX: {} break;
    
    case Point_TK_Func: {
      ret = ret && (statuses[curr->func_t.ret_uid].canonical != NULL);
      for (int i = 0; i < curr->func_t.arity; i++)
        ret = ret && (statuses[curr->func_t.arg_uids[i]].canonical != NULL);
    } break;
    
    case Point_TK_Pointer: {
      ret = ret && (statuses[curr->ptr_t.sub_uid].canonical != NULL);
    } break;
    
    case Point_TK_Array: {
      ret = ret && (statuses[curr->array_t.sub_uid].canonical != NULL);
    } break;
  }
  
  return ret;
}

static void PointTypeReducedToCanonical(PointTypeCache* types, POINTREDUCEDTYPE* curr, POINTREDUCEDTYPE* type_set) {
  PointType canon_reg = {0};
  canon_reg.type = curr->type;
  canon_reg.size = curr->size;
  
  switch (canon_reg.type) {
    case Point_TK_None:
    case Point_TK_Float:
    case Point_TK_Type:
    case Point_TK_Void:
    case Point_TK_Bool:
    case Point_TK_MAX: {} break;
    
    case Point_TK_Int: {
      canon_reg.int_t.is_signed = curr->int_t.is_signed;
    } break;
    
    case Point_TK_Func: {
      canon_reg.func_t.arity = curr->func_t.arity;
      canon_reg.func_t.is_varargs = curr->func_t.is_varargs;
      
      canon_reg.func_t.ret_t = type_set[curr->func_t.ret_uid].canonical;
      canon_reg.func_t.arg_ts = PointArenaAlloc(&types->misc_arena, canon_reg.func_t.arity * sizeof(PointType*));
      for (int i = 0; i < canon_reg.func_t.arity; i++)
        canon_reg.func_t.arg_ts[i] = type_set[curr->func_t.arg_uids[i]].canonical;
    } break;
    
    case Point_TK_Pointer: {
      canon_reg.ptr_t.sub_t = type_set[curr->ptr_t.sub_uid].canonical;
    } break;
    
    case Point_TK_Array: {
      canon_reg.array_t.count = curr->array_t.count;
      canon_reg.array_t.sub_t = type_set[curr->array_t.sub_uid].canonical;
    } break;
  }
  
  PointType* canonical = PointTypeCacheRegister(types, canon_reg);
  curr->canonical = canonical;
}


PointDStackImpl(PointScopeRef);

static void PointScopeEntriesSerialize(PointArena* arena, PointScope* scope) {
  ptu32* entry_count = PointArenaAllocUnaligned(arena, sizeof(ptu32));
  *entry_count = scope->entry_count;
  
  PointScopeEntry* curr = scope->entries;
  while (curr) {
    ptu64* string_len = PointArenaAllocUnaligned(arena, sizeof(ptu64));
    *string_len = curr->name.size;
    ptu8*  string_buf = PointArenaAllocUnaligned(arena, sizeof(ptu8) * curr->name.size);
    memmove(string_buf, curr->name.str, sizeof(ptu8) * curr->name.size);
    ptu32* type_id    = PointArenaAllocUnaligned(arena, sizeof(ptu32));
    *type_id = curr->type->uid;
    
    curr = curr->next;
  }
}

static void PointScopeEntriesDeserialize(PointArena* misc_arena, PointPool* entries_pool, PointScope* fill, POINTREDUCEDTYPE* type_set, void** ptr) {
  void* track = *ptr;
  fill->entry_count = PointReadType(track, ptu32);
  
  PointScopeEntry* prev = NULL;
  for (int i = 0; i < fill->entry_count; i++) {
    PointScopeEntry* curr = PointPoolAlloc(entries_pool);
    
    if (prev) prev->next = curr;
    else fill->entries = curr;
    
    curr->name.size = PointReadType(track, ptu64);
    curr->name.str = PointArenaAlloc(misc_arena, sizeof(ptu8) * curr->name.size);
    memmove(curr->name.str, track, sizeof(ptu8) * curr->name.size);
    track = ((ptu8*)track) + curr->name.size;
    curr->type = type_set[PointReadType(track, ptu32)].canonical;
    
    prev = curr;
  }
  
  *ptr = track;
}

//~ Program Save / Load

void PointProgramSave(PointProgram* prog, PointString filename) {
  POINTHEADER header = {0};
  header.magic_number = POINT_MAGIC_NUMBER;
  header.magic_types  = POINT_MAGIC_TYPES;
  header.magic_scopes = POINT_MAGIC_SCOPES;
  header.magic_nodes  = POINT_MAGIC_NODES;
  header.seg_types    = prog->types.type_count;
  header.seg_scopes   = 0;
  header.seg_nodes    = 0;
  
  PointArena dynamic_sections = {0};
  PointArenaInit(&dynamic_sections);
  
  //- Types Buffer
  ptu32 typeid_ctr = 0;
  for (int i = 0; i < prog->types.bucket_count; i++) {
    PointTypeBucket* curr = prog->types.buckets[i];
    while (curr) {
      curr->type.uid = typeid_ctr;
      typeid_ctr += 1;
      curr = curr->next;
    }
  }
  
  void* seg_types_ptr = dynamic_sections.memory;
  for (int i = 0; i < prog->types.bucket_count; i++) {
    PointTypeBucket* curr = prog->types.buckets[i];
    while (curr) {
      PointTypeSerialize(&dynamic_sections, &curr->type);
      curr = curr->next;
    }
  }
  
  //- Scopes Buffer
  PointDStack(PointScopeRef) scope_working_set = {0};
  PointDStackPush(PointScopeRef, &scope_working_set, prog->scopes.root);
  while (scope_working_set.len) {
    PointScope* work = PointDStackPop(PointScopeRef, &scope_working_set);
    PointScopeEntriesSerialize(&dynamic_sections, work);
    
    ptu32* scope_count = PointArenaAllocUnaligned(&dynamic_sections, sizeof(ptu32));
    *scope_count = work->scope_count;
    
    PointScope* curr = work->first;
    while (curr) {
      PointDStackPush(PointScopeRef, &scope_working_set, curr);
      curr = curr->next;
    }
  }
  
  
  //- Finalize
  FILE* file = fopen(filename.str, "wb");
  fwrite(&header, sizeof(POINTHEADER), 1, file);
  fwrite(seg_types_ptr, dynamic_sections.alloc_position, 1, file);
  fclose(file);
  
  PointArenaFree(&dynamic_sections);
}

void PointProgramLoad(PointProgram* prog, PointString filename) {
  //- Init
  PointProgramInit(prog);
  PointArena pieces = {0};
  PointArenaInit(&pieces);
  PointArena file_buffer = {0};
  PointArenaInit(&file_buffer);
  
  FILE* file = fopen(filename.str, "rb");
  fseek(file, 0L, SEEK_END);
  size_t filesize = ftell(file);
  void* buffer = PointArenaAlloc(&file_buffer, filesize);
  rewind(file);
  fread(buffer, sizeof(ptu8), filesize, file);
  fclose(file);
  
  
  POINTHEADER header = PointReadType(buffer, POINTHEADER);
  
  //- Types
  
  POINTREDUCEDTYPE* type_set = PointArenaAlloc(&pieces, sizeof(POINTREDUCEDTYPE) * header.seg_types);
  PointDQueue(POINTREDUCEDTYPEREF) type_process_queue = {0};
  PointDQueueReserve(POINTREDUCEDTYPEREF, &type_process_queue, header.seg_types);
  for (int i = 0; i < header.seg_types; i++) {
    PointTypeDeserialize(&pieces, &type_set[i], &buffer);
    PointDQueuePush(POINTREDUCEDTYPEREF, &type_process_queue, &type_set[i]);
  }
  
  ptu32 n = (type_process_queue.back - type_process_queue.front);
  while (n) {
    n -= 1;
    POINTREDUCEDTYPE* curr;
    PointDQueuePop(POINTREDUCEDTYPEREF, &type_process_queue, &curr);
    
    if (PointTypeCheckDependencies(curr, type_set)) {
      PointTypeReducedToCanonical(&prog->types, curr, type_set);
      PointTypePrint(curr->canonical, 0);
      n = (type_process_queue.back - type_process_queue.front);
    } else {
      PointDQueuePush(POINTREDUCEDTYPEREF, &type_process_queue, curr);
    }
  }
  
  //- Scopes
  PointDStack(PointScopeRef) scope_working_set = {0};
  PointDStackPush(PointScopeRef, &scope_working_set, prog->scopes.root);
  
  while (scope_working_set.len) {
    PointScope* work = PointDStackPop(PointScopeRef, &scope_working_set);
    PointScopeEntriesDeserialize(&prog->scopes.names_arena, &prog->scopes.entries_pool, work, type_set, &buffer);
    
    work->scope_count = PointReadType(buffer, ptu32);
    for (int i = 0; i < work->scope_count; i++) {
      PointScope* new_scope = PointScopeCreate(&prog->scopes, work);
      work->scope_count -= 1; // Kindof weird to undo but it's *fine* for now
      PointDStackPush(PointScopeRef, &scope_working_set, new_scope);
    }
  }
  
  
  
  //- Finish
  PointArenaFree(&file_buffer);
  PointArenaFree(&pieces);
}



#endif //POINT_IMPLEMENTATION