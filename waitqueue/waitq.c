#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>                 
#include <linux/uaccess.h>              
#include <linux/kthread.h>
#include <linux/wait.h>                 
 
 
uint32_t readc = 0;
static struct task_struct *wait_thread;
 
DECLARE_WAIT_QUEUE_HEAD(wait_queue_dev);
 
dev_t dev = 0;
static struct class *dev_class;
static struct cdev my_cdev;
int waitq_flag = 0;


static int      __init dev_driver_init(void);
static void     __exit dev_driver_exit(void);
 

static int      dev_open(struct inode *inode, struct file *file);
static int      dev_release(struct inode *inode, struct file *file);
static ssize_t  dev_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  dev_write(struct file *filp, const char *buf, size_t len, loff_t * off);

static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = dev_read,
        .write          = dev_write,
        .open           = dev_open,
        .release        = dev_release,
};


static int wait_function(void *unused)
{
        
        while(1) 
	{
                printk("In waiting...\n");
                wait_event_interruptible(wait_queue_dev, waitq_flag != 0 );
                if(waitq_flag == 2) 
		{
                        printk("Event occured from exit call\n");
                        return 0;
                }
                printk("Event occured from read - %d\n", ++readc);
                waitq_flag = 0;
        }
        do_exit(0);
        return 0;
}


static int dev_open(struct inode *inode, struct file *file)
{
        printk("In open call...\n");
        return 0;
}


static int dev_release(struct inode *inode, struct file *file)
{
        printk("In close call...\n");
        return 0;
}


static ssize_t dev_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        printk("Read\n");
        waitq_flag = 1;
        wake_up_interruptible(&wait_queue_dev);
        return 0;
}


static ssize_t dev_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        printk("Write\n");
        return len;
}
 

static int __init dev_driver_init(void)
{
        
        if((alloc_chrdev_region(&dev, 0, 1, "my_Dev")) <0)
	{
                printk("major number allocation failed\n");
                return -1;
        }
       printk("Major = %d and Minor = %d \n",MAJOR(dev), MINOR(dev));
 
        
        cdev_init(&my_cdev,&fops);
        my_cdev.owner = THIS_MODULE;
        my_cdev.ops = &fops;
 
        
        if((cdev_add(&my_cdev,dev,1)) < 0)
	{
            printk("device add failed\n");
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
            printk("device not created\n");
		class_destroy(dev_class);
        }
 
        
        wait_thread = kthread_create(wait_function, NULL, "WaitThread");
        if (wait_thread) 
	{
                printk("Thread Created successfully\n");
                wake_up_process(wait_thread);
        } 
	else
                printk("Thread creation failed\n");
        printk("DD insertion completed...\n");
        return 0;
}

 
static void __exit dev_driver_exit(void)
{
        waitq_flag = 2;
        wake_up_interruptible(&wait_queue_dev);
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev, 1);
        printk("DD is removed...\n");
}
 
module_init(dev_driver_init);
module_exit(dev_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("MJ");
