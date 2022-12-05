obj-m += block_syscall.o

all: module userland

module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf userland

userland: userland.c
	gcc -o $@ $^ -lpthread

.PHONY: module clean
