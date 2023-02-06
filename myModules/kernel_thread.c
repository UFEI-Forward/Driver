#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>

//Declare a thread variable
static struct task_struct *chr_thread1;

int thrd_func1(void *p)
{
    //The function must be added
	while(!kthread_should_stop()) {
		printk(KERN_ALERT "thrad is running test() function!");
		msleep(1000);
	}

	return 0;
}
 
static int __init chr_driver_init(void)
{
    //Assign a value to the thread
	chr_thread1 = kthread_create(thrd_func1,NULL,"chr thread 1");
	if(chr_thread1) {
        //Wake up the thread
		wake_up_process(chr_thread1);
	}
	else {
		printk(KERN_INFO "Unable to create the thread.\n");
	}

    return 0;
}
 
void __exit chr_driver_exit(void)
{
    //Stop the thread
	kthread_stop(chr_thread1);
}
 
module_init(chr_driver_init);
module_exit(chr_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechoGenius Academy");
MODULE_DESCRIPTION("Character Device Driver using Interrupts");