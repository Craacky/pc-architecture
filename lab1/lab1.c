#include <linux/module.h>   
#include <linux/kernel.h>   
#include <linux/init.h>   
#include <linux/io.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student Craacky");
MODULE_DESCRIPTION("Linux module");
MODULE_VERSION("1.0");


int readPort(int reg) 
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
	
	
	if (value & 8)
	{
		printk(KERN_ALERT "Display: true\n");
  }
  else
  {
		printk(KERN_ALERT "Display: false\n");
  }
    
    
	if ((value & 48) == 0)
	{
		printk(KERN_ALERT "Main video adapter: EGA\n");
	}
	else if  ((value & 48) == 16)
	{
		printk(KERN_ALERT "Main video adapter: VGA\n");
	} 
	else if ((value & 48) == 32)
	{
		printk(KERN_ALERT "Main video adapter: CGA\n");
	}
	else if ((value & 48) == 48) 
	{
		printk(KERN_ALERT "Main video adapter: Text monochrome display\n");
	}
	
		
	if ((value & 192) == 0) 
	{
		printk(KERN_ALERT "Number of disk drivers: 1\n");
	}
	else if ((value & 192) == 64) 
	{
		printk(KERN_ALERT "Number of disk drivers: 2\n");
	}
	else if ((value & 192) == 128) 
	{
		printk(KERN_ALERT "Number of disk drivers: 3\n");
	}
	else if ((value & 192) == 192) 
	{
		printk(KERN_ALERT "Number of disk drivers: 4\n");
	}
	else
	{
		printk(KERN_ALERT "Number of disk drivers: unknown\n");
	} 
		 
	return 0;
}

static void lab1_exit(void)
{
	printk(KERN_ALERT "Module Deleted\n");
}


module_init(lab1_init);
module_exit(lab1_exit);
