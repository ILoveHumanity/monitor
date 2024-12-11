#include<linux/kernel.h>
#include <linux/module.h> /* Needed by all modules */
#include <linux/printk.h> /* Needed for pr_info() */

MODULE_LICENSE("GPL");

static int __init mod_core1_init(void) {
    pr_info("Welcome to the Tomsk State University!\n");
    return 0;
}

static void __exit mod_core1_exit(void) {
    pr_info("Tomsk State University forever!\n");
}

module_init(mod_core1_init);
module_exit(mod_core1_exit);