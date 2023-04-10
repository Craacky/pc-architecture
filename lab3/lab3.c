
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/io.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student Craacky");
MODULE_DESCRIPTION("Arch Linux Module");
MODULE_VERSION("1.0");

/*
Передача нажатия клавиши
*/

static void write(int value) {
	while (1) {
		int answer = inb_p(0x64);
		if ((answer & 2) == 0) {
			outb_p(value, 0x60);
			break;		
		}
	}
}


// Передача отпускания клавиши в порт 60
static void clear(int value) {
	while (1) {
		int answer = inb_p(0x64);
		if ((answer & 2) == 0) {
			outb_p(value, 0x60);
			break;		
		}
	}
}

void writeName (int* array, int arraySize) {

	int i = 0;
	for (i = 0; i < arraySize; i++) {
		while (1) {
			int answer = inb_p(0x64);
			if ((answer & 2) == 0) {
				printk(KERN_INFO "inb empty");
				
				// запись в порт 64 команды имитации нажатия или отпускания клавиши
				// методичка, страница 8.
				outb_p(0xd2, 0x64);
				
				// *(array + i) - получение значения элемента массива
				// array - это адрес массива
				// i - смещение на номер элемента в массиве
				// * - получает значение выражения 
				// к адресу массива прибавляется смещение, чтобы получить нужный элемент
				write(*(array + i)); // нажатие
				
				// к значению элемента прибавляется значение 0x80 для отжатия клавиши
				// Почему именно это значение?
				// методичка второй лабы, 6 страница, предпоследний абзац
				clear(*(array + i) + 0x80); // отпускание
				
				break;
			}
		}
	}
}

static int lab3_init(void) {

	// массив сканкодов НАЖАТИЯ
	int scanCodes[] = {
		0x25, // сканкод нажатия e
		0x18, // сканкод нажатия g
		0x26, // сканкод нажатия o
		0x15,
		0x1e,// сканкод нажатия r
	};
	
	// подсчет длины массива сканкодов
	// длина массива в байтах делится на величину байт первого элемента
	// Пример: данный массив состоит из 4 элементов типа int
	// Каждый элемент это 2 байта (в зависимости от архитектуры)
	// Соответсвенно, 2 * 4 = 8 - длина массива в байтах
	size_t arraySize = sizeof(scanCodes) / sizeof(scanCodes[0]);
	
	writeName(scanCodes, arraySize);
	
	return 0;

	
}

static void lab3_exit(void) {
	printk(KERN_ALERT "Goodbye module.\n");
}

module_init(lab3_init);
module_exit(lab3_exit)
