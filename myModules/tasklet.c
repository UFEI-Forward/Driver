#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include<linux/slab.h>

#define IRQ_NO 1

struct tasklet_struct *tasklet;

void tasklet_func(unsigned long data)
{
	printk(KERN_ERR "tasklet_func is running!");
}

static irqreturn_t irq_handler(int irq,void *dev_id) {
	/* Scheduling the tasklet */    
	tasklet_schedule(tasklet);

    return IRQ_HANDLED;
}

static int task_init(void)
{
    printk(KERN_INFO "-----------------------");

    if (request_irq(IRQ_NO, irq_handler, IRQF_SHARED, "chr_device", (void *)(irq_handler))) {
        printk(KERN_INFO "chr_device: cannot register IRQ ");
        goto irq;
    }

	tasklet = kmalloc(sizeof(struct tasklet_struct),GFP_KERNEL);
	if(tasklet == NULL) {
		printk(KERN_INFO"cannot allocate the memory..\n");
		goto irq;
	}
	
	tasklet_init(tasklet,tasklet_func,0); //Dynamic Method
 
    printk(KERN_INFO "Device Driver Insert...Done!!!\n");

    return 0;

irq:
    free_irq(IRQ_NO,(void *)(irq_handler));
    return -1;
}
 
static void task_exit(void)
{
    free_irq(IRQ_NO,(void *)(irq_handler));
    tasklet_kill(tasklet);

	printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}
 
module_init(task_init);
module_exit(task_exit);

MODULE_LICENSE("Dual BSD/GPL");