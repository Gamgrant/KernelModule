# KernelModule
```bash
# to load the kerel module into the kernel
sudo insmod hello_module.ko

# check kernel logs to see printk message was logges
dmesg | tail

# remove module v
sudo rmmod hello_module

