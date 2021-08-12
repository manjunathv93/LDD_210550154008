#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>

int Helloworld_type=1;
static int hello_add(int a,int b);

static int hello_init(void)
{
	printk("Init module and the module parameter is %d\n",Helloworld_type);
	printk("Hello_world\n");
	return 0;
}

static int hello_add(int a,int b)
{
	printk("Hello_add of module\n");
	return(a+b);

}

EXPORT_SYMBOL_GPL(hello_add);



static void hello_exit(void)
{
	printk("cleanup Module and The module parameter is %d\n",Helloworld_type);
	printk("goodbye\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MJ");

