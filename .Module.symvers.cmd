cmd_/home/ec2-user/Grant/Module.symvers :=  sed 's/ko$$/o/'  /home/ec2-user/Grant/modules.order | scripts/mod/modpost -m -a     -o /home/ec2-user/Grant/Module.symvers -e -i Module.symvers -T - 
