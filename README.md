# Block Syscall

This repository contains code to compile a Linux Kernel Module that will create a device (`/dev/block_syscall_dev`) with a blocking (forever) operation on `open` syscall.

The goal of this is to reproduce a bug where the process blocking in the syscall turns zombie after being killed although there is a parent to `waitpid` on the zombie process.

For that purposes, a userland application is available in that repository. It launches a thread which will open the device blocking.

If an application open the devices directly from the main (`sudo cat /dev/syscall_dev` for instance), killing the `cat` process does not turn it into zombie because the process is in `D` state (uninteruptible sleep).

## Usage
### x86
```
make
sudo insmod block_syscall.ko
sudo ./userland
```
Beware, you will not be able to kill `userland` (`TERM` or `KILL`). The only way out is to poweroff the host, for that reason it is recommended to test is inside a [Virtual Machine](https://ubuntu.com/tutorials/how-to-run-ubuntu-desktop-on-a-virtual-machine-using-virtualbox).

### R4IP
First store r4ip buildroot directory path into a variable:
```bash
export R4IP_DIR=<path to r4ip-buildroot>
```
Compile userland application:
```bash
CC=${R4IP_DIR}/output/host/bin/powerpc-buildroot-linux-gnu-gcc make userland
```

Compile Linux Kernel Module:
```bash
PATH="${R4IP_DIR}/output/host/bin:${R4IP_DIR}/output/host/sbin:${PATH}" BUILD_KERNEL="ce047afcbfcd6bc92e386cce5beb6e33ac725695" KSRC="${R4IP_DIR}/output/build/linux-ce047afcbfcd6bc92e386cce5beb6e33ac725695" KMOD="${R4IP_DIR}/output/target/lib/modules/ce047afcbfcd6bc92e386cce5beb6e33ac725695" /usr/bin/make -j13 -C $(pwd) HOSTCC="/usr/bin/gcc -O2 -I${R4IP_DIR}/output/host/include -L${R4IP_DIR}/output/host/lib -Wl,-rpath,${R4IP_DIR}/output/host/lib" ARCH=powerpc INSTALL_MOD_PATH=${R4IP_DIR}/output/target CROSS_COMPILE="${R4IP_DIR}/output/host/bin/powerpc-buildroot-linux-gnu-" DEPMOD=${R4IP_DIR}/output/host/sbin/depmod INSTALL_MOD_STRIP=1 module
```
Make sure that kernel version is up to date.


## references:
[Processes states](https://idea.popcount.org/2012-12-11-linux-process-states/)
