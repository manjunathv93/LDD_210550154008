#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>                 
#include<linux/uaccess.h>              
 

#define mem_size        1024            
dev_t dev = 0;
static struct class *dev_class;
static struct cdev my_cdev;
uint8_t *kernel_buffer;


static int      __init MY_driver_init(void);
static void     __exit MY_driver_exit(void);
static int      MY_open(struct inode *inode, struct file *file);
static int      MY_release(struct inode *inode, struct file *file);
static ssize_t  MY_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  MY_write(struct file *filp, const char *buf, size_t len, loff_t * off);



static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = MY_read,
        .write          = MY_write,
        .open           = MY_open,
        .release        = MY_release,
};
 

static int MY_open(struct inode *inode, struct file *file)
{
        
        if((kernel_buffer = kmalloc(mem_size , GFP_KERNEL)) == 0)
	{
            printk("Cannot allocate memory in kernel\n");
            return -1;
        }
        printk("Device File Opened...!!!\n");
        return 0;
}


static int MY_release(struct inode *inode, struct file *file)
{
        kfree(kernel_buffer);
        printk("Device File Closed...!!!\n");
        return 0;
}


static ssize_t MY_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        
        if( copy_to_user(buf, kernel_buffer, mem_size))
        {
                printk("Error occured\n");
        }
        printk("Data Read : completed\n");
        return mem_size;
}


static ssize_t MY_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        
        if( copy_from_user(kernel_buffer, buf, len))
        {
                printk("Error occured\n");
        }
        printk("Data Write : completed\n");
        return len;
}


static int __init MY_driver_init(void)
{
        
        if((alloc_chrdev_region(&dev, 0, 1, "my_Dev")) <0)
	{
                printk("major number allocation failed\n");
                return -1;
        }
        printk("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
        
        cdev_init(&my_cdev,&fops);
 
        
        if((cdev_add(&my_cdev,dev,1)) < 0)
	{
            printk("add the device to the system failed\n");
	    unregister_chrdev_region(dev,1);
            return -1;
        }
 
        
        if((dev_class = class_create(THIS_MODULE,"my_class")) == NULL)
	{
            printk("struct class not created\n");
            unregister_chrdev_region(dev,1);
            return -1;
        }
 
        
        if((device_create(dev_class,NULL,dev,NULL,"my_device")) == NULL)
	{
            printk("Cannot create the Device 1\n");
	    class_destroy(dev_class);
        }
        printk("DD insertion completed...\n");
        return 0;
}


static void __exit MY_driver_exit(void)
{
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev, 1);
        printk("DD is removed...\n");
}
 
module_init(MY_driver_init);
module_exit(MY_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("MJ");
