#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h> 

dev_t dev = 0;
static struct cdev chr_cdev;
static struct class *dev_class;

/**********************chr_op function defination*****************/ 
static ssize_t chr_read(struct file *filp,char __user *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Read function\n");
        return 0;
}
static ssize_t chr_write(struct file *filp,const char __user *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Write Function\n");
        return 0;
}

static int chr_open(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Device File Opened...!!!\n");
        return 0;
}
 
static int chr_release(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Device File Closed...!!!\n");
        return 0;
}

static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = chr_read,
        .write          = chr_write,
        .open           = chr_open,
        .release        = chr_release,
};

static int __init chr_driver_init(void)
{
    /*Allocate Major number and ‘chr_dev’ is char device’s name*/
    if((alloc_chrdev_region(&dev, 0, 1, "chr_Dev")) < 0){
        printk(KERN_INFO "Cannot allocate major number");
        return -1;
    }
    printk(KERN_ERR "Major = %d Minor = %d",MAJOR(dev), MINOR(dev));

    /*Creat cdev structure*/
    cdev_init(&chr_cdev, &fops);

    /*Add character device structure to the system*/
    if((cdev_add(&chr_cdev,dev,1)) < 0){
        printk(KERN_INFO "Cannot add the device to the system");
        goto r_class;
    }

    /*Creat struct class under ‘/sys/class’*/
    if((dev_class = class_create(THIS_MODULE,"chr_class3")) == NULL){
        printk(KERN_INFO "Cannot create the struct class\n");
        goto r_class;
    }

    /*Creat char device under ‘/sys/dev/char’ and /dev [can be operated]*/
    if((device_create(dev_class,NULL,dev,NULL,"chr_device")) == NULL){
        printk(KERN_INFO "Cannot create the Device 1\n");
        goto r_device;
    }

    printk(KERN_INFO "Device Driver Insert...Done!!!\n");
    return 0;

r_device:
    class_destroy(dev_class);

r_class:
    cdev_del(&chr_cdev);
    unregister_chrdev_region(dev,1);
    return -1;
}

void __exit chr_driver_exit(void)
{
    device_destroy(dev_class,dev);
    class_destroy(dev_class);
	cdev_del(&chr_cdev);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}

module_init(chr_driver_init);
module_exit(chr_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechoGenius Academy");
MODULE_DESCRIPTION("Character Device Driver using Interrupts");