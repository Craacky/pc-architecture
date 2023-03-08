cmd_/home/craacky/lab1/lab1.mod := printf '%s\n'   lab1.o | awk '!x[$$0]++ { print("/home/craacky/lab1/"$$0) }' > /home/craacky/lab1/lab1.mod
