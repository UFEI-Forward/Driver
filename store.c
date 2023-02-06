#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static struct task_struct *thread;

int test(void* p) {
	printk(KERN_ALERT "thrad is running test() function!");

	return 0;
}

static int __init chr_driver_init(void) {
	thread = kthread_create(test, NULL, "thread1");
	if(thread) {
		wake_up_process(thread);
	}

	return 0;
}

static void __exit chr_driver_exit(void) {
	kthread_stop(thread);
}

module_init(chr_driver_init);
module_exit(chr_driver_exit);

MODULE_LICENSE("GPL");