#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sysfs.h> 

unsigned int sysfs_val = 0;
struct kobject *kobj_ref;

static ssize_t sysfs_show(struct kobject *kobj,struct kobj_attribute *attr,char *buf)
{
	printk(KERN_INFO" Reading - sysfs show func...\n");
	return sprintf(buf,"%d",sysfs_val);
}

static ssize_t sysfs_store(struct kobject *kobj,struct kobj_attribute *attr,const char *buf,size_t count)
{
	printk(KERN_INFO" Writing - sysfs store function....\n");
	sscanf(buf,"%d",&sysfs_val);
	return count;
}

struct kobj_attribute sysfs_attr = __ATTR(sysfs_val,0660,sysfs_show,sysfs_store);

static int __init chr_driver_init(void)
{
	/* creating the directory in /sys/kernel */
	kobj_ref = kobject_create_and_add("my_sysfs",kernel_kobj);

	/* creating the sysfs file */
	if(sysfs_create_file(kobj_ref,&sysfs_attr.attr))
	{
		printk(KERN_INFO"Unable to create the sysfs file...\n");
		goto r_sysfs;
	}

    printk(KERN_INFO "Device Driver Insert...Done!!!\n");
    return 0;

r_sysfs:
	kobject_put(kobj_ref);
	sysfs_remove_file(kernel_kobj,&sysfs_attr.attr);
    return -1;
}

void __exit chr_driver_exit(void)
{
    kobject_put(kobj_ref);
	sysfs_remove_file(kernel_kobj,&sysfs_attr.attr);
    printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}

module_init(chr_driver_init);
module_exit(chr_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechoGenius Academy");
MODULE_DESCRIPTION("Character Device Driver using Interrupts");