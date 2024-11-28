# KernelModule
Kernel Version: 6.1.115-126.197
Architecture: x86_64
Linux version: Amazon Linux 2023.6.20241111
cwd: /home/ec2-user/Grant/KernelModule
===============================================================
Step 0: Set up
sudo yum groupinstall "Development Tools" -y
sudo yum install cmake -y

===============================================================
Step 1: Loading the kernle in module (not wasm3)
# kernel release version
```bash
uname -r
```
# to load the kerel module into the kernel
```bash
sudo insmod hello_module.ko
```
# check kernel logs to see printk message was logges
```bash
sudo dmesg | tail
```
#Grant your user permission to view logs without sudo:
```bash
sudo usermod -aG adm ec2-user
```
# now
```bash
dmesg | tail
```
# remove module v
```bash
sudo rmmod hello_module
sudo dmesg | tail
```
####### not so relevant for now
#Generate a Signing Key:
```bash
openssl req -new -x509 -newkey rsa:2048 -keyout MOK.priv -outform DER -out MOK.der -nodes -days 36500 -subj "/CN=Grant Key/"
```
Sign the Module:
```bash
/usr/src/kernels/$(uname -r)/scripts/sign-file sha256 ./MOK.priv ./MOK.der hello_module.ko
```
Enroll the Key: Use mokutil to enroll the key:
```bash
sudo mokutil --import MOK.der
```

===============================================================
Step 2: Run wasm3 code in wasm3 runtime (no kernel involved) 
```bash
git clone https://github.com/wasm3/wasm3.git
```
To compile C code into WebAssembly. Install Emscripten Emscripten's official installation guide:
```bash
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh

export paths to ~/.bashrc :
export PATH=$PATH:$HOME/Grant/KernelModule/wabt/build
source /home/ec2-user/Grant/KernelModule/emsdk/emsdk_env.sh
```
Compile hello_wasm.c
```bash
emcc just_wasm/hello_wasm.c \
    -s STANDALONE_WASM=1 \
    -s ERROR_ON_UNDEFINED_SYMBOLS=0 \
    --no-entry \
    -o just_wasm/hello_wasm.wasm
```
Build wasm executable 

# If you haven't already cloned wasm3, do it first
```bash
cd wasm3
mkdir build
cd build
cmake ..
make
cd ../..
```
Compile main.c
```bash
gcc just_wasm/main.c -I./wasm3/source ./wasm3/build/source/libm3.a -lm -o just_wasm/wasm3_runner
```
run wasm3 
```bash
./just_wasm/wasm3_runner
```
===============================================================
Step 3: Run Wasm3 inside the kernel 
