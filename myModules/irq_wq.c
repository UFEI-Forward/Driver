
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

//#define IRQ_NO 1
static int IRQ_NO;
//unsigned int i = 0;
//unsigned int j = 0;

//struct workqueue_struct *wq;

/*
struct work_data {
    struct work_struct work;
    int data;
};
*/

//struct work_data *mydata;

static struct work_struct mywork;

#define MY_DEV_ID	   		1226
struct myirq
{
    int devid;
};
struct myirq mydev  ={ MY_DEV_ID };

/*
static void my_print(struct work_data *info) 
{
    printk(KERN_INFO "info: %d -> %p", info->data, info);
    //printk(KERN_INFO "D: mydata: %d -> %p", mydata->data, mydata);
}
*/


static void work_handler(struct work_struct *work)
{
    printk(KERN_ERR, "work handler is called!");
    //printk(KERN_INFO "B: mydata: %d -> %p", mydata->data, mydata);

    /*
    printk(KERN_INFO "-: work: -> %p", work);
    mydata->data = 9;
    mydata = (struct work_data *)work;
	//printk(KERN_ERR "num is %d", mydata->data);

    printk(KERN_INFO "C: mydata: %d -> %p", mydata->data, mydata);
    my_print(mydata);
    */
}

static irqreturn_t irq_handler(int irq, void *dev) {
    //printk(KERN_ERR "********************************");
    //printk(KERN_ERR "irq times is %d", i++);
    //INIT_WORK(&mydata->work, work_handler);
    //schedule_work(&mydata->work);
    //queue_work(wq, &mydata->work);

    struct myirq mydev;
    mydev = *(struct myirq*)dev;	
	
	// 检查设备 id，只有当相等的时候才需要处理
	if (MY_DEV_ID == mydev.devid)
	{
		// 读取键盘扫描码
	
		if (true)
		{			
			// 初始化工作项
			INIT_WORK(&mywork, work_handler);
			
			// 加入到工作队列 system_wq
        	schedule_work(&mywork);

            printk("ESC key is pressed! \n");
		}
	}	

    return IRQ_HANDLED;
}

static int __init chr_driver_init(void)
{
    /*
    printk(KERN_ERR "************************");
    struct work_data * mydata;
    mydata = kmalloc(sizeof(struct work_data), GFP_KERNEL);
    */

    //printk(KERN_ERR "A: mydata: %d -> %p", mydata->data, mydata);
    
    //wq = create_workqueue("wq_test");

    IRQ_NO = 1;
    printk(KERN_ERR "--------------------------------");
    if(request_irq(IRQ_NO, irq_handler, IRQF_SHARED, "chr_device", &mydev)!=0)
    {
        printk("register irq[%d] handler failed. \n", IRQ_NO);
        return -1;
    }
    /*
    if (request_irq(IRQ_NO, irq_handler, IRQF_SHARED, "chr_device", (void *)(irq_handler))) {
        printk(KERN_INFO "chr_device: cannot register IRQ ");
        goto irq;
    }
    */

    printk(KERN_INFO "Device Driver Insert...Done!!!\n");
    return 0;
 
//irq:
    //free_irq(IRQ_NO, &mydev);
    //free_irq(IRQ_NO,(void *)(irq_handler));
    //return -1;
}
 
void __exit chr_driver_exit(void)
{
    printk(KERN_INFO "Device Driver Remove...Done!!!\n");
    free_irq(IRQ_NO, &mydev);
    //free_irq(IRQ_NO,(void *)(irq_handler));

    //flush_workqueue(wq);
    //destroy_workqueue(wq);

	//kfree(mydata);
	//mydata = NULL;
}
 
module_init(chr_driver_init);
module_exit(chr_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechoGenius Academy");
MODULE_DESCRIPTION("Character Device Driver using Interrupts");







/*
#include <linux/init.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/slab.h>
 
MODULE_LICENSE("Dual BSD/GPL");
 
#define printd() \
    printk(KERN_ALERT "workqueue_test: %s %d\n", __FUNCTION__, __LINE__); 

struct workqueue_struct *wq;
 
struct work_data {
    struct work_struct work;
    int data;
};

static void my_print(struct work_data *info) 
{
    printk(KERN_INFO "info: %d -> %p", info->data, info);
    printk(KERN_INFO "D: mydata: %d -> %p", mydata->data, mydata);
}

static void work_handler(struct work_struct *work)
{
    printk(KERN_INFO "B: mydata: %d -> %p", mydata->data, mydata);
    
    printk(KERN_INFO "-: work: -> %p", work);
    mydata->data = 9;
    mydata = (struct work_data *)work;
	//printk(KERN_ERR "num is %d", mydata->data);

    printk(KERN_INFO "C: mydata: %d -> %p", mydata->data, mydata);
    my_print(mydata);
}

static void wq_init_test(void) {
    wq = create_workqueue("wq_test");

    //printk(KERN_INFO " : mydata: %d -> %p", mydata->data, &mydata);
    static struct work_data * mydata;
    mydata = kmalloc(sizeof(struct work_data), GFP_KERNEL);
    printk(KERN_INFO "A: mydata: %d -> %p", mydata->data, mydata);

    INIT_WORK(&mydata->work, work_handler);
    queue_work(wq, &mydata->work);
}

static void floor1(void) {
    wq_init_test();
}

static int wq_init(void)
{
	//mydata->data = 1;
    printk(KERN_INFO "-----------------------");
    wq_init_test();
 
    return 0;
}
 
static void wq_exit(void)
{
    flush_workqueue(wq);
    destroy_workqueue(wq);

	kfree(mydata);
	mydata = NULL;
}
 
module_init(wq_init);
module_exit(wq_exit);
*/