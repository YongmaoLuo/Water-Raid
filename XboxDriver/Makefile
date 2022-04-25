# We are being compiled as a module: use the Kernel build system
ifneq (${KERNELRELEASE},)
	obj-m = xpad.o
else
	KERNEL_SOURCE := /usr/src/linux-headers-$(shell uname -r)
		PWD := $(shell pwd)

default: module

xbox: xbox_test

module:
	${MAKE} -C ${KERNEL_SOURCE} SUBDIRS=${PWD} modules

.PHONY : clean
clean :
	${MAKE} -C ${KERNEL_SOURCE} SUBDIRS=${PWD} clean
	rm -rf *.o 
endif
