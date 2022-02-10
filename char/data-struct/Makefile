CC=gcc
obj-m += data-struct-main.o
data-struct-main-objs := data-struct.o link-list.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

modules: usr-data-struct
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

usr-data-struct:
		$(CC) -o $@ $@.c

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

clean:
	rm usr-data-struct
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

.PHONY: modules modules_install clean
