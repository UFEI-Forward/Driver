#include <linux/init.h>
#include <linux/module.h>

//millisecond
#define TIMEOUT 5000

int times = 0;

static struct timer_list chr_timer;

//timer callback function which is called when timer expires
void timer_callback(struct timer_list *timer)
{
	printk(KERN_INFO "Callling timer_callback() %i-th times", ++times);

	// Re-enable the timer which will make this as periodic timer */
	mod_timer(&chr_timer,msecs_to_jiffies(TIMEOUT));
}
 
static int __init chr_driver_init(void)
{
	printk(KERN_ERR "*********************************");
	printk(KERN_INFO "chr_driver_init is running!");
	
	//setup your timer to call timer callback function */
	timer_setup(&chr_timer,timer_callback,0);

	//setup the timer interval to base on TIMEOUT Macro
	mod_timer(&chr_timer,jiffies + msecs_to_jiffies(TIMEOUT));
	
	return 0;
}
 
void __exit chr_driver_exit(void)
{
	del_timer(&chr_timer);

	printk(KERN_INFO "chr_driver_exit is running!");
}
 
module_init(chr_driver_init);
module_exit(chr_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechoGenius Academy");
MODULE_DESCRIPTION("Character Device Driver using Interrupts");