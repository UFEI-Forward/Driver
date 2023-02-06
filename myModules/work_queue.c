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

static struct work_data * mydata;

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

    printk(KERN_INFO "C: mydata: %d -> %p", mydata->data, mydata);
    my_print(mydata);
}

static void wq_init_test(void) {
    wq = create_workqueue("wq_test");

    mydata = kmalloc(sizeof(struct work_data), GFP_KERNEL);
    printk(KERN_INFO "A: mydata: %d -> %p", mydata->data, mydata);

    INIT_WORK(&mydata->work, work_handler);
    queue_work(wq, &mydata->work);
}

static int wq_init(void)
{
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