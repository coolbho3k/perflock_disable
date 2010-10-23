KERNEL_BUILD := ~/kernel_bravo
KERNEL_CROSS_COMPILE := /home/mike/android/prebuilt/linux-x86/toolchain/arm-eabi-4.4.0/bin/arm-eabi-

obj-m += perflock_disable.o

all:
	make -C $(KERNEL_BUILD) CROSS_COMPILE=$(KERNEL_CROSS_COMPILE) M=$(PWD) modules
	$(KERNEL_CROSS_COMPILE)strip --strip-debug perflock_disable.ko

clean:
	make -C $(KERNEL_BUILD) M=$(PWD) clean 2> /dev/null
	rm -f modules.order *~
