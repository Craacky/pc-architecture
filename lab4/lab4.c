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

unsigned long *syscall_table = (unsigned long *)0xffffffff81a001c0;
asmlinkage int (*original_write)( const char *, int);
asmlinkage int open(const char *pathname, int flags)
{
// измененная функция write
printk(KERN_ALERT "open HIJACKED");
printk(KERN_ALERT "%s",pathname);
return (*original_write)(pathname, flags);
}
static int init(void) {
printk(KERN_ALERT "\nHIJACK INIT\n");
write_cr0 (read_cr0 () & (~ 0x10000));
original_write = (void *)syscall_table[__NR_open];
syscall_table[__NR_open] = open;
write_cr0 (read_cr0 () | 0x10000);
return 0;
}
static void exit(void) {
write_cr0 (read_cr0 () & (~ 0x10000));
syscall_table[__NR_open] = original_write;
write_cr0 (read_cr0 () | 0x10000);
printk(KERN_ALERT "MODULE EXIT\n");
return;
}
module_init(init);
module_exit(exit);

