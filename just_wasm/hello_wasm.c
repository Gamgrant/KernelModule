#include <emscripten.h>

__attribute__((import_module("env")))
__attribute__((import_name("m3_print")))
extern void m3_print(const char* msg);

EMSCRIPTEN_KEEPALIVE
void _start() {
}

EMSCRIPTEN_KEEPALIVE
int run() {
    m3_print("Hello from WebAssembly!\n");
    return 42;
}