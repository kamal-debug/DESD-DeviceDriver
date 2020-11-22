obj-m = sample.o

KERNELDIR = /lib/modules/$(shell uname -r)/build

PWD := $(shell pwd)

default:
		$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

prepare:
		
		$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_prepare

install:

		$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

clean:

		$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
