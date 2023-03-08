cmd_/home/craacky/lab1/Module.symvers :=  sed 's/ko$$/o/'  /home/craacky/lab1/modules.order | scripts/mod/modpost  -a   -N -o /home/craacky/lab1/Module.symvers -e -i Module.symvers -T - 
