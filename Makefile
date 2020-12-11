obj-m := main.o

all:
	make -C /usr/lib/modules/4.19.0-12-amd64/build M=$(PWD) modules

clean:
	make -C /usr/lib/modules/4.19.0-12-amd64/build M=$(PWD) clean

install:
	sudo insmod main.ko

uninstall:
	sudo rmmod main.ko

list:
	sudo lsmod


