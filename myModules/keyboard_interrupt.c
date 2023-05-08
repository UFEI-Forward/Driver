#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/interrupt.h>

//Ref: https://www.oreilly.com/library/view/linux-device-drivers/0596000081/ch09s03.html

//Interrupts type and Interrupts owner: cat /proc/interrupts
#define IRQ_NO 1
unsigned int i = 0;
unsigned int j = 0;

static void myPrint(void) {
	printk(KERN_ERR "values is %d", j++);
}

static irqreturn_t irq_handler(int irq, void *dev_id) {
    printk(KERN_ERR "irq times is %d", i++);
    myPrint();

    return IRQ_HANDLED;
}

static int __init chr_driver_init(void)
{
    printk(KERN_ERR "--------------------------------");
    if (request_irq(IRQ_NO, irq_handler, IRQF_SHARED, "chr_device", (void *)(irq_handler))) {
        printk(KERN_INFO "chr_device: cannot register IRQ ");
        goto irq;
    }

    printk(KERN_INFO "Device Driver Insert...Done!!!\n");
    return 0;
 
irq:
    free_irq(IRQ_NO, (void *)(irq_handler));
    return -1;
}
 
static void __exit chr_driver_exit(void)
{
    free_irq(IRQ_NO, (void *)(irq_handler));
    printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}
 
module_init(chr_driver_init);
module_exit(chr_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechoGenius Academy");
MODULE_DESCRIPTION("Character Device Driver using Interrupts");