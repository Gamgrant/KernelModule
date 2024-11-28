#include <stdio.h>
#include "../wasm3/source/wasm3.h"
#include "../wasm3/source/m3_env.h"
#include <stdlib.h>
#include <sys/stat.h>

// this is the function to read the WASM file
unsigned char* read_wasm_file(const char* filename, unsigned int* size) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Could not open file %s\n", filename);
        return NULL;
    }

    struct stat st;
    stat(filename, &st);
    *size = st.st_size;

    unsigned char* buffer = (unsigned char*)malloc(*size);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, *size, file);
    fclose(file);
    return buffer;
}

m3ApiRawFunction(m3_wasm_printer) {
    m3ApiGetArgMem(const char*, message);
    printf("%s", message);
    m3ApiSuccess();
}

int main() {
    M3Result result = m3Err_none;
    
    // inside main() we need to create environment and runtime 
    M3Environment* env = m3_NewEnvironment();
    M3Runtime* runtime = m3_NewRuntime(env, 64*1024, NULL);
    M3Module* module = NULL;
    
    // load WASM module here
    unsigned char* wasm_bytes = NULL;
    unsigned int wasm_size = 0;
    
    wasm_bytes = read_wasm_file("just_wasm/hello_wasm.wasm", &wasm_size);
    if (!wasm_bytes) {
        printf("Failed to read WASM file\n");
        return 1;
    }
    
    result = m3_ParseModule(env, &module, wasm_bytes, wasm_size);
    if (result) {
        printf("ParseModule error: %s\n", result);
        free(wasm_bytes);
        return 1;
    }
    
    result = m3_LoadModule(runtime, module);
    if (result) {
        printf("LoadModule error: %s\n", result);
        free(wasm_bytes);
        return 1;
    }
    
    // Link the print function here
    result = m3_LinkRawFunction(module, "env", "m3_print", "v(*)", &m3_wasm_printer);
    if (result) {
        printf("LinkFunction error: %s\n", result);
        free(wasm_bytes);
        return 1;
    }
    
    // IM3Function will help us find and call the run function 
    IM3Function f;
    result = m3_FindFunction(&f, runtime, "run");
    if (result) {
        printf("FindFunction error: %s\n", result);
        free(wasm_bytes);
        return 1;
    }
    
    result = m3_Call(f, 0, NULL);
    if (result) {
        printf("Call error: %s\n", result);
        free(wasm_bytes);
        return 1;
    }
    
    free(wasm_bytes);
    return 0;
}