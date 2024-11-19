cmd_/home/ec2-user/Grant/hello_module.mod := printf '%s\n'   hello_module.o | awk '!x[$$0]++ { print("/home/ec2-user/Grant/"$$0) }' > /home/ec2-user/Grant/hello_module.mod
