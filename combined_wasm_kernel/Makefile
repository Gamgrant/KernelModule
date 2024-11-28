# Makefile for building the kernel module with WASM3 integration

# Paths
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
WASM3_DIR := $(PWD)/../wasm3/source

# Module name and source files
obj-m := kernel_wasm3.o
kernel_wasm3-y := src/kernel_wasm3_main.o \
    src/m3_core.o \
    src/m3_env.o \
    src/m3_compile.o \
    src/m3_exec.o \
    src/m3_parse.o \
    src/m3_module.o \
    src/m3_function.o \
    src/m3_code.o \
    src/m3_info.o

# Compilation flags
EXTRA_CFLAGS := \
    -include $(PWD)/include/wasm3_kernel.h \
    -I$(PWD)/include \
    -I$(PWD)/src \
    -Wall \
    -Wno-error=incompatible-pointer-types \
    -DKERNEL_MODE

# Main targets
all: prepare_dirs copy_sources build

prepare_dirs:
	@mkdir -p include src backup
	@cp -n include/wasm3_kernel.h backup/ 2>/dev/null || true

copy_sources: prepare_dirs
	@echo "Copying WASM3 source files..."
	@cp $(WASM3_DIR)/m3_*.c src/
	@cp $(WASM3_DIR)/m3_*.h src/
	@cp $(WASM3_DIR)/wasm3.h include/
	@cp $(WASM3_DIR)/wasm3_defs.h include/
	@echo "Source files copied successfully"

build:
	$(MAKE) -C $(KDIR) M=$(PWD) modules EXTRA_CFLAGS="$(EXTRA_CFLAGS)"

clean:
	@mkdir -p backup
	@cp -f include/wasm3_kernel.h backup/ 2>/dev/null || true
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f src/*.o src/*.ko src/*.mod src/*.mod.c src/*.mod.o
	rm -f src/m3_*.c src/m3_*.h
	@cp -f backup/* include/ 2>/dev/null || true

.PHONY: all clean prepare_dirs copy_sources build
