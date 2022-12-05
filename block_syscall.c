#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nicolas VINCENT");
MODULE_DESCRIPTION("A simple Linux module.");
MODULE_VERSION("0.01");

static dev_t device_number;
static struct class *bs_class;
static struct cdev bs_cdev;
static struct mutex bs_mut;

static int bs_open(struct inode *inode, struct file *file)
{
	int err;
	pr_info("acquire mutex\n");
	mutex_lock(&bs_mut);
	mutex_unlock(&bs_mut);
	pr_info("got mutex\n");
	return 0;
}

static int bs_release(struct inode *inode, struct file *file)
{
	pr_info("Release file\n");
	return 0;
}

static ssize_t bs_read(struct file *file, char __user *user_buffer,
		       size_t count, loff_t *offset)
{
	return 0;
}

static struct file_operations file_ops = {
	.owner = THIS_MODULE,
	.read = bs_read,
	.open = bs_open,
	.release = bs_release,
};

static int __init bs_init(void)
{
	int err = 0;
	err = alloc_chrdev_region(&device_number, 0, 1, "block_syscall");
	if (err != 0) {
		pr_err("Failed to allocate device number (%d)\n", err);
		goto err_chrdev;
	}
	pr_info("Major: %d; Minor: %d\n",
		MAJOR(device_number), MINOR(device_number));

	bs_class = class_create(THIS_MODULE, "block_syscall_class");
	if (IS_ERR(bs_class)) {
		pr_err("Cannot create sysclass\n");
		err = PTR_ERR(bs_class);
		goto err_class;
	}
	cdev_init(&bs_cdev, &file_ops);
	err = cdev_add(&bs_cdev, device_number, 1);
	if (err != 0) {
		goto err_cdev;
	}

	device_create(bs_class, NULL, device_number, NULL, "block_syscall_dev");

	mutex_init(&bs_mut);
	mutex_lock_interruptible(&bs_mut);

	pr_info("module loaded with major num %d\n", MAJOR(device_number));
	return 0;

	cdev_del(&bs_cdev);
err_cdev:
	class_destroy(bs_class);
err_class:
	unregister_chrdev_region(device_number, 1);
err_chrdev:
	return err;
}

static void __exit bs_exit(void)
{
	device_destroy(bs_class, device_number);
	cdev_del(&bs_cdev);
	class_destroy(bs_class);
	unregister_chrdev_region(device_number, 1);
	pr_info("Goodbye, World!\n");
}

module_init(bs_init);
module_exit(bs_exit);
