#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>
 
static int value = 11;
static char *name = "module";

 
module_param(value, int, S_IRUSR|S_IWUSR);                     
module_param(name, charp, S_IRUSR|S_IWUSR);                     

 

static int __init hello_world_init(void)
{
        printk("Value = %d  \n", value);
        printk("Name = %s \n", name);
        printk("Kernel Module is Inserted Successfully...\n");
    return 0;
}

static void __exit hello_world_exit(void)
{
    printk("Kernel Module is Removed Successfully...\n");
}
 
module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MJ");
