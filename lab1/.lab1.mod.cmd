cmd_/home/craacky/pc-architecture/lab1/lab1.mod := printf '%s\n'   lab1.o | awk '!x[$$0]++ { print("/home/craacky/pc-architecture/lab1/"$$0) }' > /home/craacky/pc-architecture/lab1/lab1.mod
