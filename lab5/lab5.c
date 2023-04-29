#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/unistd.h>
#include <asm/cacheflush.h>
#include <asm/page.h>
#include <asm/current.h>
#include <linux/sched.h>
#include <linux/kallsyms.h>
#include <linux/fs.h>
#include <linux/bio.h>

MODULE_LICENSE("GPL");

static char addressHx[16];

static char buff[501];
static char version[50];
static char fullFilePath[500];

static char *fname = "/proc/version";
static char *fMapName, *mySomeStr = "sys_call_table", *str3, *str4;
static char *str1 = "/boot/System.map-";


static void readCoreInfo(void) {
	struct file *f;
	size_t n, i, sizeBuff = 0;

	f = filp_open(fname, O_RDONLY, 0);
	n = kernel_read(f, 0, buff, 500);
	if(n) {
		buff[n] = 0;
	}

	filp_close(f, current->files);
}

static void cutVersionFromCoreInfo(void) {
	int start = 13, versionId = 0, error = 50;
	while (buff[++start] != ' ') {
		version[versionId] = buff[start];
		versionId++;

		if (start == error) {break;}
	}

	printk(KERN_ALERT "Version: %s", version);
}

static void fillFilePath(void) {
	int j = 0;
	while (j < 17) {
		char c = str1[j];
		fullFilePath[j] = c;
		j++;
	}

	int i = 0;
	while (i < 50) {
		char c = version[i];
		fullFilePath[j] = c;
		j++;
		i++;
	}
	
	printk(KERN_ALERT "File path: %s", fullFilePath);
}


static void getSysCallTableAddressHx(void) {
	struct file *f;
	size_t n, i, sizeBuff = 0;
	static char buff2[1700001];
	// Открытие файла /boot/System.map-4.4.0-142-generic
	f = filp_open(fullFilePath, O_RDONLY, 0);
	// Извлечение данных из файла
	n = kernel_read(f, 0, buff2, 1700000);
	if(n) {
		buff2[n] = 0;
	}
	filp_close(f, current->files);

	char mySomeBuff[200];
	int addressCounter = 15;
	int num = 0, j = 0, mySomeSize = 14, mySomeCount = 0;
	while (j < 1700000) {
		if (mySomeStr[mySomeCount] == buff2[j]) {
			mySomeBuff[mySomeCount] = buff2[j];
			mySomeCount++;
			if (mySomeCount == mySomeSize) {
				int k = 0;
				j -= 17;
				while (k < 16) {					
					addressHx[addressCounter] = buff2[j];
					k++;
					j--;
					addressCounter--;
				}
				break;
			}
		} else {
			mySomeCount = 0;
		}
	
		j = j + 1;
	}

	int k = 0;
	addressCounter = 0;
	while (k < 16) {					
		printk(KERN_ALERT "%c\n", addressHx[addressCounter]);
		addressCounter++;
		k++;
	}

	printk(KERN_ALERT "FINISH");
}


static int init(void) {
	printk(KERN_ALERT "\nHIJACK INIT\n");

	readCoreInfo();
	cutVersionFromCoreInfo();
	fillFilePath();
	getSysCallTableAddressHx();
	
	//write_cr0 (read_cr0 () & (~ 0x10000));
	//original_write = (void *)syscall_table[__NR_write];
	//syscall_table[__NR_write] = new_write;
	//write_cr0 (read_cr0 () | 0x10000);
	return 0;
}

static void exit(void) {
	//write_cr0 (read_cr0 () & (~ 0x10000));
	//syscall_table[__NR_write] = original_write;
	//write_cr0 (read_cr0 () | 0x10000);
	//printk(KERN_ALERT "MODULE EXIT\n");
	return;
}

module_init(init);
module_exit(exit);
