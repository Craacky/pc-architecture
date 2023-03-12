#include <linux/module.h>   
#include <linux/kernel.h>   
#include <linux/init.h>   
#include <linux/io.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student Craacky");
MODULE_DESCRIPTION("Linux module");
MODULE_VERSION("1.0");

static int readPort(int reg) 
{
	outb(reg, 0x70);
	return inb(0x71);
}

static int lab1_init(void)
{
	int value;
	printk(KERN_ALERT "Module Pushed \n");
	asm("cli");
	value = readPort(0x14);
	asm("sti");
	printk(KERN_ALERT "Port_read_module: %x\n", value);

	if (value & 1)
	{		
		printk(KERN_ALERT "Floppy disk drive: true\n");
	}
	else
	{
		printk(KERN_ALERT "Floppy disk drive: false \n");
	}	
	
	if (value & 2)
	{
		printk(KERN_ALERT "Processor block for floating-point operations: true\n");
	}
	else
	{
		printk(KERN_ALERT "Processor block for floating-point operation: false\n");
	}
	
	if (value & 4)
	{
		printk(KERN_ALERT "Keyboard: true\n");
	}
	else
	{
		printk(KERN_ALERT "Keyboard: false\n");
	}

	//printk(KERN_ALERT "Display: %s\n", value & 8 ? "true": "false");

	if (value & 8)
	{
		printk(KERN_ALERT "Display: true\n");
	}
	else
	{
		printk(KERN_ALERT "Display: false\n");
	}

	const int vc = value & 24;
	switch(vc >> 3)
	{
	case 0: // 00 - 0
		printk(KERN_ALERT "Main video adapter: EGA or VGA\n");
		break;
	case 1: // 01 - 8
	case 2: // 10 - 16
		printk(KERN_ALERT "Main video adapter: CGA\n");
		break;
	case 3: // 11 - 24
		printk(KERN_ALERT "Main video adapter: MDA\n");
		break;
	}

	const int disks = (value & 192) >> 6;
	printk(KERN_ALERT "Number of disk drivers: %d\n", disks);
	return 0;
}

static void lab1_exit(void)
{
	printk(KERN_ALERT "Module Deleted\n");
}

module_init(lab1_init);
module_exit(lab1_exit);
