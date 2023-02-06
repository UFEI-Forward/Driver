
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
#include<linux/sysfs.h> 
#include<linux/kobject.h> 
#include <linux/interrupt.h>
#include <asm/io.h>
#include<linux/kthread.h>
#include<linux/delay.h>
#include<linux/jiffies.h>
#include<linux/timer.h>
#include <linux/workqueue.h>

#define IRQ_NO 1

struct work_data {
    struct work_struct work;
    int data;
};

struct work_data *mydata;

static void my_print(struct work_data *info) 
{
    printk(KERN_ERR "my print() is called!");
    printk(KERN_INFO "D: mydata: %d -> %p", mydata->data, mydata);
}

static void work_handler(struct work_struct *work)
{
    printk(KERN_ERR "work handler is called!");
    printk(KERN_INFO "B: mydata: %d -> %p", mydata->data, mydata);

    mydata = (struct work_data *)work;
    printk(KERN_INFO "C: mydata: %d -> %p", mydata->data, mydata);

    my_print(mydata);
}

static irqreturn_t irq_handler(int irq, void *dev) {
    printk(KERN_ERR "Key is pressed and irq handler is running!");
    printk(KERN_ERR "%s, %p", mydata->data, mydata);
    
    INIT_WORK(&mydata->work, work_handler);
    schedule_work(&mydata->work);

    return IRQ_HANDLED;
}

static int __init chr_driver_init(void)
{
    printk(KERN_ERR "************************");

    mydata = kmalloc(sizeof(struct work_data), GFP_KERNEL);
    printk(KERN_ERR "A: mydata: %d -> %p", mydata->data, mydata);

    if(request_irq(IRQ_NO, irq_handler, IRQF_SHARED, "chr_device", (void *)(irq_handler))!=0)
    {
        printk("register irq[%d] handler failed. \n", IRQ_NO);
        goto irq;
    }

    printk(KERN_INFO "Device Driver Insert...Done!!!\n");
    return 0;
 
irq:
    free_irq(IRQ_NO, (void *)(irq_handler));
    return -1;
}
 
void __exit chr_driver_exit(void)
{
    printk(KERN_INFO "Device Driver Remove...Done!!!\n");
    free_irq(IRQ_NO, (void *)(irq_handler));

	kfree(mydata);
	mydata = NULL;
}
 
module_init(chr_driver_init);
module_exit(chr_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechoGenius Academy");
MODULE_DESCRIPTION("Character Device Driver using Interrupts");