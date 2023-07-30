#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>


static int __init LKM_init(void)
{
	
	int i;
	
	for(i=0; i < 10; i++)
	 printK(KERN_INFO ,"hello checkpoint\n");
}

static void __exit LKM_exit(void)
{
	printk(KERN_INFO,"Goodbye\n");
}

module_init(LKM_init);
module_exit(LKM_exit);

