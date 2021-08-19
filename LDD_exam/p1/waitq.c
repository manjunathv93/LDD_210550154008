// kernel module to implement sleep during read write appln
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>                 
#include <linux/uaccess.h>
#include <linux/sched.h>              
 

int flag = 1;
char Buff[50];

int  MY_open(struct inode *inode, struct file *file);
int  MY_release(struct inode *inode, struct file *file);
ssize_t  MY_read(struct file *filp, char __user *ubuff, size_t count,loff_t * offp);
ssize_t  MY_write(struct file *filp, const char *ubuff, size_t count, loff_t * offp);



struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = MY_read,
        .write          = MY_write,
        .open           = MY_open,
        .release        = MY_release,
};
 
DECLARE_WAIT_QUEUE_HEAD(mywait);
static struct cdev *my_device;

int MY_open(struct inode *inode, struct file *file)
{
        printk("Device File Opened...!!!\n");
        return 0;
}


int MY_release(struct inode *inode, struct file *file)
{
        printk("Device File Closed...!!!\n");
        return 0;
}


ssize_t MY_read(struct file *filp, char __user *ubuff, size_t count, loff_t *offp)
{
        unsigned long int result;
	ssize_t retval;
	printk("Inside read .....before waitq\n");
	if(flag==1)
	{
		wait_event_interruptible(mywait,flag==0);
	}
	printk("In read...after waitq...");
	result = copy_to_user((char *)ubuff,(char *)Buff,count);
	if(result!=0)
	{
		printk("\n Data is not copied");
		retval=-EFAULT;
		return retval;
	}
	else
	{
		flag =1;
		printk("\ndata is read\n");
		retval=count;
		return retval;
	}

}


ssize_t MY_write(struct file *filp, const char __user *ubuff, size_t count, loff_t *offp)
{
        unsigned long result;
	ssize_t retval;
	printk("\n In write...");
	result=copy_from_user((char *)Buff,(char *)ubuff,count);
	if(result==0)
	{
		printk("\n user message...... %s ......\n",Buff);
		retval = count;
		flag =0;	
		wake_up_interruptible(&mywait);
		return retval;
	}
	else
	{
		printk("error occured\n");
		retval=-EFAULT;
		return retval;
	}
}


static int __init device_init(void)
{
        dev_t mydevice;
	int result;
	mydevice = MKDEV(255,0);
	result = register_chrdev_region(mydevice,1,"mydevice");
	if(result<0)
	{
		printk("Region is not alloted\n");
		return -1;
	}
	my_device = cdev_alloc();
	my_device->ops = &fops;
	
        if(cdev_add(my_device,mydevice,1) < 0)
	{
            printk("add the device to the system failed\n");
            return -1;
        }
        printk("DD insertion completed...\n");
        return 0;
}


static void __exit device_exit(void)
{
	dev_t mydevice;
	mydevice = MKDEV(255,0);        
        unregister_chrdev_region(mydevice, 1);
	cdev_del(my_device);
        printk("DD is removed...\n");
}

module_init(device_init);
module_exit(device_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Manjunath");
MODULE_DESCRIPTION("waitqueue");

