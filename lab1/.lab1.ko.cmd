cmd_/home/craacky/lab1/lab1.ko := ld -r -m elf_x86_64 -z noexecstack --no-warn-rwx-segments --build-id=sha1  -T scripts/module.lds -o /home/craacky/lab1/lab1.ko /home/craacky/lab1/lab1.o /home/craacky/lab1/lab1.mod.o;  true
