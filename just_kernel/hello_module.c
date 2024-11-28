#include <linux/init.h>
#include <linux/module.h>

static int __init hello_init(void) {
    printk(KERN_INFO "This is Grant, hey kernel!\n");
    return 0;
}

static void __exit hello_exit(void) {
    printk(KERN_INFO "Bye bye Kernel\n");
    barrier(); 
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("GRANT THE G");
MODULE_DESCRIPTION("A SIMPLE HELLO KERNEL MODULE");