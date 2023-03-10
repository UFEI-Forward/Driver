#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/init.h>

#define IOSTART 0x200
#define IOEXTEND 0x40

static unsigned long iostart = IOSTART, ioextend = IOEXTEND, ioend;

static int __init my_init(void) {
	unsigned long ultest = (unsigned long)100;

	printk("---------------------------------------");
	ioend = iostart + ioextend;
	printk(KERN_INFO "requesting the IO region 0x%lx to 0x%lx\n", iostart, ioend);

	//Request port staring at iostart with length ioextend
	if(!request_region(iostart, ioextend, "my_ioport")) {
		printk(KERN_INFO "the IO region is busy, quitting..\n");
		return -EBUSY;
	}

	//Write value into port starting at address iostart
	printk(KERN_INFO "writing a long data = %ld\n", ultest);
	outl(ultest, iostart);

	//Read value from port stating at address iostart
	ultest = inl(iostart);
	printk(KERN_INFO "Reading a long data = %ld\n", ultest);

	return 0;
}

static void __exit my_exit(void) {
	printk(KERN_INFO "releasing the IO region 0x%lx to 0x%lx\n", iostart, ioend);

	//Release port statrting at iostart with length ioextend
	release_region(iostart, ioextend);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("TechoGenius Academy");
MODULE_DESCRIPTION(" requesting the I/O Ports");
MODULE_LICENSE("GPL");