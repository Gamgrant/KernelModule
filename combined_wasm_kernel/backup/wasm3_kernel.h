#ifndef WASM3_KERNEL_H
#define WASM3_KERNEL_H

//---------------------------------------------------------------
// Prevent Inclusion of Standard Headers
// Define macros to prevent standard headers from being included
//---------------------------------------------------------------

#define _STDIO_H
#define _STDLIB_H
#define _STDINT_H
#define _INTTYPES_H
#define _STDARG_H
#define _STRING_H
#define _STDBOOL_H
#define _ASSERT_H
#define _MATH_H
#define _ERRNO_H
#define _STDDEF_H      
#define __STDDEF_H     
#define _STDDEF_H_     

//---------------------------------------------------------------
// Kernel Headers
//---------------------------------------------------------------
#include <linux/kernel.h>     
#include <linux/module.h>     
#include <linux/types.h>      
#include <linux/slab.h>       
#include <linux/string.h>     
#include <linux/errno.h>      
#include <linux/bug.h>        
#include <asm/byteorder.h>    

//---------------------------------------------------------------
// Standard Type Definitions
//---------------------------------------------------------------
typedef __u8    uint8_t;
typedef __s8    int8_t;
typedef __u16   uint16_t;
typedef __s16   int16_t;
typedef __u32   uint32_t;
typedef __s32   int32_t;
typedef __u64   uint64_t;
typedef __s64   int64_t;

typedef uint8_t     u8;
typedef int8_t      i8;
typedef uint16_t    u16;
typedef int16_t     i16;
typedef uint32_t    u32;
typedef int32_t     i32;
typedef uint64_t    u64;
typedef int64_t     i64;

#ifndef NULL
#define NULL ((void*)0)
#endif

typedef __kernel_size_t size_t;
typedef __kernel_ssize_t ssize_t;
typedef long ptrdiff_t;  

typedef float       f32;
typedef double      f64;

#ifndef __cplusplus
typedef _Bool       bool;
#define true        1
#define false       0
#endif

//---------------------------------------------------------------
// Macro Definitions
//---------------------------------------------------------------
#ifndef likely
#define likely(x)       __builtin_expect(!!(x), 1)
#endif
#ifndef unlikely
#define unlikely(x)     __builtin_expect(!!(x), 0)
#endif
#ifndef min
#define min(a, b)       ((a) < (b) ? (a) : (b))
#endif
#ifndef max
#define max(a, b)       ((a) > (b) ? (a) : (b))
#endif
#define and     &&
#define or      ||
#define not     !

//---------------------------------------------------------------
// Function Replacements
//---------------------------------------------------------------

#define malloc(size)            kmalloc(size, GFP_KERNEL)
#define calloc(nmemb, size)     kcalloc(nmemb, size, GFP_KERNEL)
#define realloc(ptr, size)      krealloc(ptr, size, GFP_KERNEL)
#define free(ptr)               kfree(ptr)
#define printf                  printk
#define fprintf(stream, fmt, ...)   printk(fmt, ##__VA_ARGS__)
#define sprintf(buf, fmt, ...)      snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__)
#define snprintf                 snprintf
#define vsnprintf                vsnprintf
#define assert(expr)            BUG_ON(!(expr))

//---------------------------------------------------------------
// Error Handling
//---------------------------------------------------------------
typedef const char* M3Result;

#define m3Err_none                      ((M3Result)NULL)
#define m3Err_mallocFailed              "memory allocation failed"
#define m3Err_unknownError              "unknown error"

//---------------------------------------------------------------
// Type Definitions Required by WASM3
//---------------------------------------------------------------

typedef const u8*      bytes_t;
typedef const u8*      cbytes_t;
typedef const char*    cstr_t;
typedef u8             m3opcode_t;
typedef u8             M3ValueType;

struct M3Module;
struct M3Memory;
struct M3Environment;
struct M3Runtime;
struct M3Function;
struct M3Global;
typedef struct M3Module*        IM3Module;
typedef struct M3Runtime*       IM3Runtime;
typedef struct M3Environment*   IM3Environment;
typedef struct M3Function*      IM3Function;
typedef struct M3Global*        IM3Global;
typedef const void*     m3ret_t;
typedef u64             m3slot_t;
typedef s64             m3reg_t;
typedef m3slot_t*       m3stack_t;
typedef const void*     code_t;
typedef code_t          pc_t;

//---------------------------------------------------------------
// Memory Management Macros
//---------------------------------------------------------------

#define m3_malloc(size)         kmalloc(size, GFP_KERNEL)
#define m3_free(ptr)            kfree(ptr)
#define m3_realloc(ptr, size)   krealloc(ptr, size, GFP_KERNEL)

#define M3_INIT(field)          memset(&(field), 0, sizeof(field))

//---------------------------------------------------------------
// Byte Swapping Functions
//---------------------------------------------------------------

#define m3_bswap16(x)           __builtin_bswap16(x)
#define m3_bswap32(x)           __builtin_bswap32(x)
#define m3_bswap64(x)           __builtin_bswap64(x)

#ifdef __BIG_ENDIAN
#define M3_BSWAP_u16(x)         m3_bswap16(x)
#define M3_BSWAP_u32(x)         m3_bswap32(x)
#define M3_BSWAP_u64(x)         m3_bswap64(x)
#else
#define M3_BSWAP_u16(x)         (x)
#define M3_BSWAP_u32(x)         (x)
#define M3_BSWAP_u64(x)         (x)
#endif

//---------------------------------------------------------------
// Function Attributes and Macros
//---------------------------------------------------------------

#define M3_NO_INLINE            __attribute__((noinline))
#define M3_ALWAYS_INLINE        inline __attribute__((always_inline))
#define M3_UNREACHABLE          __builtin_unreachable()

#define M3_WEAK                 __attribute__((weak))
#define M3_SECTION(name)        __attribute__((section(name)))
#define M3_EXPORT               __attribute__((visibility("default")))

//---------------------------------------------------------------
// Logging and Debugging Macros
//---------------------------------------------------------------

#define m3_log_error(fmt, ...)      printk(KERN_ERR "WASM3 Error: " fmt, ##__VA_ARGS__)
#define m3_log_warning(fmt, ...)    printk(KERN_WARNING "WASM3 Warning: " fmt, ##__VA_ARGS__)
#define m3_log_info(fmt, ...)       printk(KERN_INFO "WASM3 Info: " fmt, ##__VA_ARGS__)
#define m3_log_debug(fmt, ...)      printk(KERN_DEBUG "WASM3 Debug: " fmt, ##__VA_ARGS__)

//---------------------------------------------------------------
// Error Handling Functions
//---------------------------------------------------------------

static inline void m3_abort(const char* message)
{
    printk(KERN_ERR "WASM3 Abort: %s\n", message);
    BUG();
}

//---------------------------------------------------------------
// Function Signatures Required by WASM3
//---------------------------------------------------------------

typedef m3ret_t (*IM3Operation)(pc_t _pc, m3stack_t _sp, struct M3MemoryHeader* _mem, m3reg_t _r0);
typedef M3Result (*M3RawCall)(IM3Runtime runtime, struct M3ImportContext* _ctx, uint64_t* _sp, void* _mem);

//---------------------------------------------------------------
// Structures Required by WASM3
// Define M3ImportContext and IM3ImportContext
//---------------------------------------------------------------

typedef struct M3ImportContext
{
    void*           userdata;
    IM3Function     function;
} M3ImportContext, *IM3ImportContext;

typedef struct M3ImportInfo
{
    const char* moduleUtf8;
    const char* fieldUtf8;
} M3ImportInfo, *IM3ImportInfo;

typedef struct M3ErrorInfo
{
    M3Result        result;

    IM3Runtime      runtime;
    IM3Module       module;
    IM3Function     function;

    const char*     file;
    uint32_t        line;

    const char*     message;
} M3ErrorInfo;


//---------------------------------------------------------------
// Function Prototypes for WASM3 API
// Functio declaration that are used in the WASM3 API
//---------------------------------------------------------------

IM3Environment m3_NewEnvironment(void);
void m3_FreeEnvironment(IM3Environment env);

IM3Runtime m3_NewRuntime(IM3Environment env, uint32_t stackSizeInBytes, void* userdata);
void m3_FreeRuntime(IM3Runtime runtime);

M3Result m3_ParseModule(IM3Environment env, IM3Module* module, const uint8_t* wasmBytes, uint32_t numWasmBytes);
M3Result m3_LoadModule(IM3Runtime runtime, IM3Module module);
M3Result m3_FindFunction(IM3Function* function, IM3Runtime runtime, const char* functionName);
M3Result m3_CallV(IM3Function function, ...);
M3Result m3_LinkRawFunction(IM3Module module, const char* moduleName, const char* functionName, const char* signature, M3RawCall function);


#endif // WASM3_KERNEL_H
