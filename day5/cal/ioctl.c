#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/slab.h>          
#include<linux/uaccess.h>       
#include<linux/ioctl.h>


#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)

//Function prototypes
int NAME_open(struct inode *inode, struct file *filp);
int NAME_release(struct inode *inode, struct file *filp);


int32_t value1 = 0 ;
int32_t value2 = 0 ;
int32_t value = 0 ;
int32_t oper = 0 ;
dev_t dev=0;
static struct class *dev_class;
static struct cdev my_cdev ;

static int __init my_driver_init(void);
static void __exit my_driver_exit(void);


static long my_ioctl(struct file *file,unsigned int cmd,unsigned long arg);





static struct file_operations fops = 
{
    .owner      = THIS_MODULE,
     .open	= NAME_open,
     .release= NAME_release,
    .unlocked_ioctl = my_ioctl,

};

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



static long my_ioctl(struct file *file,unsigned int cmd,unsigned long arg)
{
    static int count = 0;
	long ret;
	long val;
    switch(cmd) 
	{
	case WR_VALUE:
		{
            if(count == 0)
                {
                ret = copy_from_user(&oper,(int32_t*)arg,sizeof(oper));
                printk(KERN_INFO "oper = %d\n",oper);
		count+=1;
                 }
            else if(count == 1)
		{
                ret = copy_from_user(&value1,(int32_t*)arg,sizeof(value1));
                printk(KERN_INFO "value1 = %d\n",value1);
		count+=1;
               	 }
            else if(count == 2)
		{
                ret = copy_from_user(&value2,(int32_t*)arg,sizeof(value2));
                printk(KERN_INFO "value2 = %d\n",value2);
		count+=1;
                 }
		break;
		}
        case RD_VALUE:
		{
		if(oper == 1)
                value = value1 + value2 ;
		
            else if(oper == 2)
                value = value1 - value2;
		
            else if(oper == 3)
                value = value1 * value2;
		
            else if(oper == 4)
                value = value1 / value2;
		}
		break;
            }
 val = copy_to_user((int32_t*) arg, &value,sizeof(value));
		
            if(count == 3)
               count = 0 ;

return 0;
}

static int __init my_driver_init(void)
{
    if((alloc_chrdev_region(&dev,0,1,"my_dev")) <0)
	{
        printk(KERN_INFO"cannot allocate major number\n");
        return -1;
        }
    printk(KERN_INFO " MAJOR = %d Minor = %d\n",MAJOR(dev),MINOR(dev));


    cdev_init(&my_cdev,&fops);


    if((cdev_add(&my_cdev,dev,1)) < 0)
	{
        printk(KERN_INFO "cannot add device to the system\n");
       // goto r_class;
	unregister_chrdev_region(dev,1);
        return -1;
        }

     
        if((dev_class = class_create(THIS_MODULE,"my_class")) == NULL)
	{
            printk(KERN_INFO "Cannot create the struct class\n");
           // goto r_class;
	unregister_chrdev_region(dev,1);
        return -1;
        }

/*Creating device*/
        if((device_create(dev_class,NULL,dev,NULL,"my_device")) == NULL)
	{
            printk(KERN_INFO "Cannot create the Device 1\n");
            //goto r_device;
		class_destroy(dev_class);
        }
        printk(KERN_INFO "Device Driver is Inserted...\n");
    return 0;

//r_device:
        
//r_class:
        
}

void __exit my_driver_exit(void)
{
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Device Driver is Removed...\n");
}

module_init(my_driver_init);
module_exit(my_driver_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("MJ");
