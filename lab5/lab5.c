#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/errno.h>
#include<linux/types.h>
#include<linux/unistd.h>
#include<asm/cacheflush.h>
#include<asm/page.h>
#include<asm/current.h>
#include<linux/sched.h>
#include<linux/kallsyms.h>
#include <linux/fs.h>
#include <linux/fcntl.h>

unsigned long *syscall_table;
asmlinkage int (*original_open)(const char *pathname, int flags);
asmlinkage int new_open(const char *pathname, int flags)
{
printk("Open\n");
return (*original_open)(pathname, flags);
}


unsigned int getLengthVersion(char* buff, int offset)
{
	char *tmp = buff + offset;
	int length = 0;
	int i = 0;
	for(i = 0; *(tmp + i) != ' '; i++)
	{
	length++;
	}
return length;
}



static unsigned long getSysCallTableAddress(void)
{
struct file *f; 
static char *fname = "/proc/version";  
static char *str1 = "/boot/System.map-"; 
static char buff[501]; 
size_t n, i = 0, sizeBuff = 0; 
// Открытие файла /proc/version 
f = filp_open(fname, O_RDONLY, 0); 
// Извлечение данных из файла 
n = kernel_read(f, 0, buff, 500); 
if(n) { 
	buff[n] = 0; 
} 
filp_close(f, NULL);
printk(KERN_ALERT "filename: %s", buff);
int offset = 14;
int length = getLengthVersion(buff, offset);
char version[length + 1];
version[length] = 0;
for(i = 0; i < length; i++)
{
	version[i] = *(buff + offset + i);
}
printk("%s\n", version);
/*while(buff[offset] != ' ')
{
	
}*/
int lengthOfSystemMap = strlen(str1);
char pathToSystemMap[length + 1 + lengthOfSystemMap];
 pathToSystemMap[length + lengthOfSystemMap] = 0;
for(i = 0; i < lengthOfSystemMap; i++)
{
	pathToSystemMap[i] = str1[i];
}
for(i = 0; i < length; i++)
{
	pathToSystemMap[i + lengthOfSystemMap] = version[i];
}
printk("%s\n", pathToSystemMap);

f = filp_open(pathToSystemMap, O_RDONLY, 0);
char* syscallTableName = "sys_call_table";
i = 0; 
char *tmpStr = NULL;
while(true){

kernel_read(f, i, buff, 500);
i += 500;
tmpStr = strstr(buff, syscallTableName);
if(tmpStr)
{
printk("Find!\n");
break;
}
}
tmpStr -= 19;
char sysCallTableAdrress[17];
sysCallTableAdrress[16] = 0;
for(i = 0; i < 16; i++)
{
sysCallTableAdrress[i] = *(tmpStr + i);
}


filp_close(f, NULL);
unsigned long address = 0, check;

printk("%s\n", sysCallTableAdrress);
check = kstrtoul(sysCallTableAdrress, 16,  &address);
//address = address +0xffffffff00000000;
printk("%x\n", address, check);
return address;
}


static int init(void) {
printk(KERN_ALERT "\nHIJACK INIT\n");
unsigned long address = getSysCallTableAddress();
syscall_table = (unsigned long *)address;
write_cr0 (read_cr0 () & (~ 0x10000));
original_open = (void *)syscall_table[__NR_open];
syscall_table[__NR_open] = new_open;
write_cr0 (read_cr0 () | 0x10000);
return 0;
}
static void exit(void) {
write_cr0 (read_cr0 () & (~ 0x10000));
syscall_table[__NR_open] = original_open;
write_cr0 (read_cr0 () | 0x10000);
printk(KERN_ALERT "MODULE EXIT\n");
return;
}
module_init(init);
module_exit(exit);
