//Initialization functions...
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>

#define NAME mycdev

//Function prototypes
int NAME_open(struct inode *inode, struct file *filp);
int NAME_release(struct inode *inode, struct file *filp);

//structure that defines the operations that the driver provides
struct file_operations fops =
{
	.owner	= THIS_MODULE,
	.open	= NAME_open,
	.release= NAME_release,
};

//structure for a character driver
struct cdev *my_cdev;

//Init module
static int chardevice_init(void)
{
	int result;
	int maj, min;
	dev_t mydevice;
	mydevice = MKDEV(255,0);   //device number creation
	maj = MAJOR(mydevice);
	min = MINOR(mydevice);
	printk("\n The Major number is %d and the minor number is %d\n",maj,min);
	result = register_chrdev_region(mydevice,1,"mycdev");  // device region to be registered
	if(result < 0)
	{
		printk("The request made is not successful\n");
		return(-1);
	}

	my_cdev = cdev_alloc(); // allocate memory to chardevice structure
	my_cdev->ops = &fops;   // link file operations to char device


	result = cdev_add(my_cdev,mydevice,1);
	if(result < 0)
	{
		printk("\n The char device is not created\n");
		unregister_chrdev_region(mydevice,1);
		return(-1);
	}
	return 0;
}



//cleanup module
static void chardevice_exit(void)
{
	dev_t mydevice;
	int maj, min;
	mydevice = MKDEV(255,0);
	maj = MAJOR(mydevice);
	min = MINOR(mydevice);
	printk("The major number is %d and the minor number is %d\n",maj,min);
	unregister_chrdev_region(mydevice,1);
	cdev_del(my_cdev);
	printk("unregister is done...goodbye\n");
	return;
}


//open system call
int NAME_open(struct inode *inode,struct file *filp)
{
	printk("In the open call...\n");
	return 0;
}


//close system call
int NAME_release(struct inode *inode,struct file *filp)
{
	printk("In the close call...\n");
	return 0;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MJ");

module_init(chardevice_init);
module_exit(chardevice_exit);

