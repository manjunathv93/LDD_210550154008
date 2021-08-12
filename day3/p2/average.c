#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>
#include "Header.h" 


int a_test=25,b_test=15;


static int hello_init(void)
{
	int Avg;
	printk("Init module and the module parameter is %d\n",a_test);
	printk("Hello_world\n");
	Avg = hello_add(a_test,b_test)/2;
	printk("\n The Average of a and b is %d\n",Avg);
	return 0;	
}

static void hello_exit(void)
{
	printk("Cleanup module and the module parameter is %d\n",a_test);
	printk("goodbye\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");


