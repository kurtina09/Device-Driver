CC=gcc
obj-m += string-process-main.o
string-process-main-objs := string-process.o uppercase.o nextchar.o finddup.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

modules: usr-string-process
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

usr-string-process:
		$(CC) -o $@ $@.c

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

clean:
	rm usr-string-process
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

.PHONY: modules modules_install clean
