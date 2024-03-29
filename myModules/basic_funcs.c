#include <linux/init.h>
#include <linux/module.h>
#include <asm/current.h>
#include <linux/sched.h>
#include <linux/version.h>
#include <linux/interrupt.h>

#define UTS_RELEASE "2.6.10"

char *whom = "world";
int howmany = 1;
int nums[8] = {0,-1,-2,-3};
int n_nums = 1;

module_param(whom, charp, S_IRUGO);
module_param(howmany, int, S_IRUGO);
module_param_array(nums, int, &n_nums, S_IRUGO);

static int hello(void) {
	int i;

    printk(KERN_INFO "UTS_RELEASE: %s", UTS_RELEASE);
    printk(KERN_INFO "LINUX VERSION: %i", LINUX_VERSION_CODE);
    printk(KERN_INFO "KERNEL VERSION: %i", KERNEL_VERSION(2, 6, 10));

    printk(KERN_INFO "This process is %s (pid %i)\n", \
    current->comm, current->pid);

    printk(KERN_ERR "The number of IRQ is %i", NR_IRQS);

    for(i=0; i<howmany; ++i) {
        printk(KERN_INFO "whom is %s\n", whom);
    }

    for(i=0; i<8; ++i) {
        printk(KERN_INFO "para[%d] : %d", i, nums[i]);
    }
    printk(KERN_INFO "\n");

    for(i=0; i<n_nums; ++i) {
        printk(KERN_INFO "para[%d] : %d", i, nums[i]);
    }
    printk(KERN_INFO "\n");

    return 0;
}

static void bye(void) {
    printk(KERN_INFO "This process is %s (pid %i)\n", \
    current->comm, current->pid);
}

module_init(hello);
module_exit(bye);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("UFEI");
MODULE_DESCRIPTION("UFEI's practice");