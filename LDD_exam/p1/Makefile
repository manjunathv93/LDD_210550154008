obj-m:=waitq.o
KERNELDIR = /lib/modules/$(shell uname -r)/build
PWD:=$(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	gcc -o readappln readappln.c
	gcc -o writeappln writeappln.c
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm readappln writeappln
