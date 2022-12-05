#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define BS_DEV "/dev/block_syscall_dev"

static void parent_func(void)
{
	while(1) {
		sleep(5);
	}
}

static void *thread_func(void *arg)
{
	int fd;

	printf("Opening device %s\n", BS_DEV);
	fd = open(BS_DEV, O_RDONLY);
	if (fd < 0) {
		printf("Error while opening device %s (%d)\n",
		       BS_DEV, errno);
		return NULL;
	}
	close(fd);
}

int main(int argc, char **argv)
{
	int err;
	pid_t childpid;
	pthread_t tid;

	err = pthread_create(&tid, NULL, &thread_func, NULL);
	parent_func();
}
