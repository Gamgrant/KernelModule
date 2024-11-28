#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>         // For kmalloc and kfree
#include <linux/string.h>       // For memcpy, memset, etc.
#include <linux/uaccess.h>      // For copy_to_user and copy_from_user if needed

// Include the kernel-compatible WASM3 headers
#include "../include/wasm3.h"
#include "../include/wasm3_defs.h"
#include "../include/wasm3_kernel.h"
#include "../include/wasm_binary.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GRANT THE G");
MODULE_DESCRIPTION("WASM3 Runtime in Kernel Module");

static IM3Environment env = NULL;
static IM3Runtime runtime = NULL;
static IM3Module module = NULL;

// Kernel-space implementation of the print function
static m3ApiRawFunction(m3_kernel_printer)
{
    m3ApiGetArgMem(const char*, message);
    if (message) {
        printk(KERN_INFO "WASM3: %s\n", message);
    }
    m3ApiSuccess();
}

// Initialize the kernel module
static int __init wasm3_kernel_init(void)
{
    M3Result result = m3Err_none;
    IM3Function run_func = NULL;

    printk(KERN_INFO "Initializing WASM3 kernel module\n");

    // Create the WASM3 environment
    env = m3_NewEnvironment();
    if (!env) {
        printk(KERN_ERR "Failed to create WASM3 environment\n");
        return -ENOMEM;
    }

    // Create the WASM3 runtime with a 64KB stack
    runtime = m3_NewRuntime(env, 64 * 1024, NULL);
    if (!runtime) {
        printk(KERN_ERR "Failed to create WASM3 runtime\n");
        m3_FreeEnvironment(env);
        return -ENOMEM;
    }

    // Parse the WASM module from the embedded binary
    result = m3_ParseModule(env, &module, just_wasm_hello_wasm_wasm, just_wasm_hello_wasm_wasm_len);
    if (result != m3Err_none) {
        printk(KERN_ERR "Failed to parse module: %s\n", result);
        m3_FreeRuntime(runtime);
        m3_FreeEnvironment(env);
        return -EINVAL;
    }

    // Load the module into the runtime
    result = m3_LoadModule(runtime, module);
    if (result != m3Err_none) {
        printk(KERN_ERR "Failed to load module: %s\n", result);
        m3_FreeModule(module);
        m3_FreeRuntime(runtime);
        m3_FreeEnvironment(env);
        return -EINVAL;
    }

    // Link the kernel print function to the WASM module
    result = m3_LinkRawFunction(module, "env", "m3_print", "v(*)", &m3_kernel_printer);
    if (result != m3Err_none) {
        printk(KERN_ERR "Failed to link function: %s\n", result);
        m3_FreeModule(module);
        m3_FreeRuntime(runtime);
        m3_FreeEnvironment(env);
        return -EINVAL;
    }

    // Find the 'run' function exported by the WASM module
    result = m3_FindFunction(&run_func, runtime, "run");
    if (result != m3Err_none) {
        printk(KERN_ERR "Failed to find 'run' function: %s\n", result);
        m3_FreeModule(module);
        m3_FreeRuntime(runtime);
        m3_FreeEnvironment(env);
        return -EINVAL;
    }

    result = m3_CallV(run_func);
    if (result != m3Err_none) {
        printk(KERN_ERR "Failed to call 'run' function: %s\n", result);
        m3_FreeModule(module);
        m3_FreeRuntime(runtime);
        m3_FreeEnvironment(env);
        return -EFAULT;
    }

    printk(KERN_INFO "WASM3 kernel module initialized successfully\n");
    return 0;
}

static void __exit wasm3_kernel_exit(void)
{
    if (module) {
        m3_FreeModule(module);
        module = NULL;
    }
    if (runtime) {
        m3_FreeRuntime(runtime);
        runtime = NULL;
    }
    if (env) {
        m3_FreeEnvironment(env);
        env = NULL;
    }
    printk(KERN_INFO "WASM3 kernel module unloaded\n");
}

module_init(wasm3_kernel_init);
module_exit(wasm3_kernel_exit);
