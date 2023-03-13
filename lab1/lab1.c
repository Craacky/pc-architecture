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
	printk(KERN_ALERT "Module Pushed");

	asm("cli");
	value = readPort(0x14);
	asm("sti");
	printk(KERN_ALERT "Port_read_module: %x", value); // hex df = dec 223 = bin 1101 1111
        
    printk(KERN_ALERT "Floppy disk drive: %s", value & 1 ? "true": "false"); 
    printk(KERN_ALERT "Processor block for floating-point operation: %s", value & 2 ? "true": "false");
    printk(KERN_ALERT "Keyboard: %s", value & 4 ? "true": "false");
	printk(KERN_ALERT "Display: %s", value & 8 ? "true": "false");

	int vc = value & 48; // dec:48 = 2^5+2^4 (5 and 4 bit)
	switch(vc >> 4) // before 11(01) 1111 after(>>4) 00(01)
	{
	case 0: // dec 0 = bin 0000
		printk(KERN_ALERT "Main video adapter: EGA or VGA");
		break;
	case 1: // dec 1 = bin 0001
	case 2: // dec 2 = bin 0010
		printk(KERN_ALERT "Main video adapter: CGA");
		break;
	case 3: // dec 3 = bin 0011
		printk(KERN_ALERT "Main video adapter: MDA");
		break;
	}

	int disks = (value & 192) >> 6;
    // before (11)01 1111 after 00(11)
    // 0000 - 1
    // 0001 - 2
    // 0010 - 3
    // 0011 - 4
	printk(KERN_ALERT "Number of disk drivers: %d", disks + 1);
	return 0;
}

static void lab1_exit(void)
{
	printk(KERN_ALERT "Module Deleted");
}

module_init(lab1_init);
module_exit(lab1_exit);
