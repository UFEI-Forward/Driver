#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>

DEFINE_SPINLOCK(chr_spinlock);

//Declare thread1 and thread2
static struct task_struct *chr_thread1;
static struct task_struct *chr_thread2;

unsigned long chr_spinlock_variable = 0;

int thrd_func1(void *p)
{
	//Check whether "chr_thread1" is stopped [Must be included]
	while(!kthread_should_stop()) {
		//Check whether "chr_spinlock" is locked
		if(!spin_is_locked(&chr_spinlock)) {
			printk(KERN_INFO"spinlock is not locked in thread function1..\n");
		}

		//Lock "chr_spinlock"
		spin_lock(&chr_spinlock);
		if(spin_is_locked(&chr_spinlock)) {
			printk(KERN_INFO"spinlock is locked in thread function 1..\n");
		}

		//chr_spinlock_variable increment
		chr_spinlock_variable++;
		printk(KERN_INFO"In thread function 1 %lu\n",chr_spinlock_variable);

		//Unlock "chr_spinlock"
		spin_unlock(&chr_spinlock);

		//sleep 1000 ms
		msleep(1000);
	}

	return 0;
}

int thrd_func2(void *p)
{
	while(!kthread_should_stop()) {
		if(!spin_is_locked(&chr_spinlock)) {
			printk(KERN_INFO"spinlock is not locked in thread function2..\n");
		}

		spin_lock(&chr_spinlock);
		if(spin_is_locked(&chr_spinlock)) {
			printk(KERN_INFO"spinlock is locked in thread function 2..\n");
		}

		chr_spinlock_variable++;
		printk(KERN_INFO"In thread function 2 %lu\n",chr_spinlock_variable);

		spin_unlock(&chr_spinlock);
		msleep(1000);
	}
	
	return 0;
}
 
static int __init chr_driver_init(void)
{
	//Bind "chr_thread1" with function thrd_func1() 
	chr_thread1 = kthread_create(thrd_func1,NULL,"chr thread 1");
	if(chr_thread1) {
		//Wake up "chr_thread1"
		wake_up_process(chr_thread1);
	}
	else {
		printk(KERN_INFO "Unable to create the thread.\n");
	}

	//Bind "chr_thread2" with function thrd_func2() and wake up it
	chr_thread2 = kthread_run(thrd_func2,NULL,"chr thread 2");
	if(chr_thread2) {
		printk(KERN_INFO "Successfully created the kernel thread..\n");
	}
	else {
		printk(KERN_INFO"unable to create the thread..\n");
	}

    printk(KERN_INFO "Device Driver Insert...Done!!!\n");

    return 0;
}
 
void __exit chr_driver_exit(void)
{
	kthread_stop(chr_thread1);
	kthread_stop(chr_thread2);
    printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}
 
module_init(chr_driver_init);
module_exit(chr_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechoGenius Academy");
MODULE_DESCRIPTION("Character Device Driver using Interrupts");