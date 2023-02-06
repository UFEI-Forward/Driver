#include <linux/init.h>
#include<linux/slab.h>
#include <linux/module.h>

int *val;

static int __init chr_driver_init(void)
{
	printk(KERN_INFO "Device Driver Insert...Done!!!\n");

	//If you defined a global variable, then you must malloc address for it
	val = kmalloc(sizeof(int), GFP_KERNEL);
	*val = 1;
	printk(KERN_ALERT "The value of val is %i", *val);

	//When it is a local variable, it is no need to do malloc operation
	int val2 = 6;
	printk(KERN_ALERT "The value of val2 is %i", val2);
    
    return 0;
}

void __exit chr_driver_exit(void)
{
	kfree(val);
    printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}

module_init(chr_driver_init);
module_exit(chr_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechoGenius Academy");
MODULE_DESCRIPTION("Character Device Driver using Interrupts");