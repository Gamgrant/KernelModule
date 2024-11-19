# KernelModule
```bash
# kernel release version
uname -r
===============================================================
# to load the kerel module into the kernel
sudo insmod hello_module.ko
===============================================================
# check kernel logs to see printk message was logges
sudo dmesg | tail
#Grant your user permission to view logs without sudo:
sudo usermod -aG adm ec2-user
# now
dmesg | tail
===============================================================
# remove module v
sudo rmmod hello_module
===============================================================
####### not so relevant for now
#Generate a Signing Key:
openssl req -new -x509 -newkey rsa:2048 -keyout MOK.priv -outform DER -out MOK.der -nodes -days 36500 -subj "/CN=Grant Key/"
Sign the Module:
/usr/src/kernels/$(uname -r)/scripts/sign-file sha256 ./MOK.priv ./MOK.der hello_module.ko
Enroll the Key: Use mokutil to enroll the key:
sudo mokutil --import MOK.der
===============================================================
