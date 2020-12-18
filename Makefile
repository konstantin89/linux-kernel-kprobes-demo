obj-m := main.o


all:
	make -C /usr/lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /usr/lib/modules/$(shell uname -r)/build M=$(PWD) clean

install:
	sudo insmod main.ko

uninstall:
	sudo rmmod main.ko

list:
	sudo lsmod


