obj-m += block_syscall.o

KSRC ?= /lib/modules/$(shell uname -r)/build

all: module userland

module:
	make -C $(KSRC) M=$(PWD) modules

clean:
	make -C $(KSRC) M=$(PWD) clean
	rm -rf userland

userland: userland.c
	$(CC) -o $@ $^ -lpthread

.PHONY: module clean
