# KernelModule
```bash
# to load the kerel module into the kernel
sudo insmod hello_module.ko

# check kernel logs to see printk message was logges
sudo dmesg | tail

#Grant your user permission to view logs without sudo:
sudo usermod -aG adm ec2-user
or
dmesg | tail

# remove module v
sudo rmmod hello_module

