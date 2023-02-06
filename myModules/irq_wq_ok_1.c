#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/interrupt.h>

#define IRQ_NO 1

struct work_data {
    struct work_struct work;
    int data;
};

struct work_data *mydata;

#define MY_DEV_ID	   		1226
struct myirq
{
    int devid;
};
struct myirq mydev  = { MY_DEV_ID };

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
    printk(KERN_ERR "Running irq handler!");

    struct myirq mydev;
    mydev = *(struct myirq*)dev;

    printk(KERN_ERR "%s, %p", mydata->data, mydata); // no data about mydata

    if (MY_DEV_ID == mydev.devid) {
        INIT_WORK(&mydata->work, work_handler);
        schedule_work(&mydata->work);
        printk("Key is pressed! \n");
    }

    return IRQ_HANDLED;
}

static int __init chr_driver_init(void)
{
    printk(KERN_ERR "************************");

    mydata = kmalloc(sizeof(struct work_data), GFP_KERNEL);
    printk(KERN_ERR "A: mydata: %d -> %p", mydata->data, mydata);

    if(request_irq(IRQ_NO, irq_handler, IRQF_SHARED, "chr_device", &mydev)!=0)
    {
        printk("register irq[%d] handler failed. \n", IRQ_NO);
        goto irq;
    }

    printk(KERN_INFO "Device Driver Insert...Done!!!\n");
    return 0;
 
irq:
    free_irq(IRQ_NO, &mydev);
    return -1;
}
 
static void __exit chr_driver_exit(void)
{
    printk(KERN_INFO "Device Driver Remove...Done!!!\n");
    free_irq(IRQ_NO, &mydev);

	kfree(mydata);
	mydata = NULL;
}
 
module_init(chr_driver_init);
module_exit(chr_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechoGenius Academy");
MODULE_DESCRIPTION("Character Device Driver using Interrupts");