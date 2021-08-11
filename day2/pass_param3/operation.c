#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>
 
static int a=10;
static int b=20;
static char* operand = "add";

 
module_param(a, int, S_IRUSR|S_IWUSR);
module_param(b, int, S_IRUSR|S_IWUSR);
module_param(operand, charp, S_IRUSR|S_IWUSR);                     

 

static int __init hello_world_init(void)
{
        printk("operand = %s \n",operand);
	switch(operand)
	{
		case "add":
			printk("Addition = %d",(a+b));
			break;
		case "sub":
			printk("Subtraction = %d",(a-b));
			break;
		case "mul":
			printk("Multiplication = %d",(a*b));
			break;
		case "div":
			printk("Division = %d",(a/b));
			break;
		default:
			printk("Addition = %d",(a+b));	
			break;
	}

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
