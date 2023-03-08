#include <linux/module.h>   // Needed by all modules
#include <linux/kernel.h>   // Needed for KERN_ALERT
#include <linux/init.h>     // Needed for the macros
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
		printk(KERN_INFO "Присутствует дисковод гибких дисков\n");
	}
	else
	{
		printk(KERN_INFO "Отсутствует дисковод гибких дисков \n");
	}
	
	
	if (value & 2)
	{
		printk(KERN_INFO "Присутствует блок процессора для операций с плавающей точкой \n");
    }
    else
    {
        printk(KERN_INFO "Отсутсвует блок процессора для операций с плавающей точкой  \n");
	}
	
	
	if (value & 4)
	{
		printk(KERN_INFO "Есть клавиатура \n");
    }
    else
    {
    	printk(KERN_INFO "Нет клавиатуры\n");
    }
	
	
	if (value & 8)
	{
		printk(KERN_INFO "Есть дисплей \n");
    }
    else
    {
        printk(KERN_INFO "Отсутствует дисплей \n");
    }
    
    
	if ((value & 48) == 0)
	{
		printk(KERN_INFO "Основной видеоадаптер: EGA\n");
	}
	else if  ((value & 48) == 16)
	{
		printk(KERN_INFO "Основной видеоадаптер: VGA\n");
	} 
	else if ((value & 48) == 32)
	{
		printk(KERN_INFO "Основной видеоадаптер: CGA\n");
	}
	else if ((value & 48) == 48) 
	{
		printk(KERN_INFO "Основной видеоадаптер: текстовый монохромный дисплей\n");
	}
	
		
	if ((value & 192) == 0) 
	{
		printk(KERN_INFO "Кол-во дисководов: 1\n");
	}
	else if ((value & 192) == 64) 
	{
		printk(KERN_INFO "Кол-во дисководов: 2\n");
	}
	else if ((value & 192) == 128) 
	{
		printk(KERN_INFO "Кол-во дисководов: 3\n");
	}
	else if ((value & 192) == 192) 
	{
		printk(KERN_INFO "Кол-во дисководов: 4\n");
	}
	else
	{
		printk(KERN_INFO "Количество дисководов гибких дисков: неизвестное значение\n");
	} 
		 
	return 0;
}

static void lab1_exit(void)
{
	printk(KERN_ALERT "Module Deleted\n");
}


module_init(lab1_init);
module_exit(lab1_exit);
