#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/kdev_t.h>
#include<linux/uaccess.h>

#include <linux/kernel.h>
#include <linux/kprobes.h>

#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt,__func__

/* This holds the device number */
dev_t device_number;

/* Cdev variable */
struct cdev pcd_cdev;

/*holds the class pointer */
struct class *class_pcd;

struct device *device_pcd;

//do_execve
//_do_fork
static struct kprobe kp = {
	.symbol_name	= "do_exit",
};

static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
// Code is running with preemtion disabled.

#ifdef CONFIG_X86_64 

	long exit_code = (long)regs->di;

	pr_info("handler_pre: name [%s], pid [%d], exit_code [%ld] \n", 
		current->comm, current->pid, exit_code);
#endif

	return 0;
}

static void handler_post(struct kprobe *p, struct pt_regs *regs, unsigned long flags)
{
// Code is running with preemtion disabled.

#ifdef CONFIG_X86_64 
	/* EMPTY*/
#endif
}

static int handler_fault(struct kprobe *p, struct pt_regs *regs, int trapnr)
{
// Code is running with preemtion disabled.

#ifdef CONFIG_X86_64 
	/* EMPTY*/
#endif

	return 0;
}

int init_kprobe(void)
{
    int ret;
    kp.pre_handler = handler_pre;
    kp.post_handler = handler_post;
    kp.fault_handler = handler_fault;

    ret = register_kprobe(&kp);

    if (ret < 0) 
    {
        pr_info("register_kprobe failed, returned %d\n", ret);
        return ret;
    }
    
	pr_info("Planted kprobe at %p\n", kp.addr);
    return ret;
}

void exit_kprobe(void)
{
    unregister_kprobe(&kp);
}

/**
* @brief: Function that implements the read operation of the character device.
*/
ssize_t pmon_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos)
{
	pr_info("Read requested for %zu bytes \n",count);
	pr_info("Current file position = %lld\n",*f_pos);

	pr_info("Number of bytes successfully read = %zu\n",count);
	pr_info("Updated file position = %lld\n",*f_pos);

	/*Return number of bytes which have been successfully read */
	return count;
}


/**
* @brief: Function that implements the open operation of the character device.
*/
int pmon_open(struct inode *inode, struct file *filp)
{
	pr_info("open was successful\n");

	return 0;
}

/**
* @brief: Function that implements the release operation of the character device.
*/
int pmon_release(struct inode *inode, struct file *flip)
{
	pr_info("release was successful\n");

	return 0;
}


/* file operations of the driver */
struct file_operations pcd_fops=
{
	.open = pmon_open,
	.release = pmon_release,
	.read = pmon_read,
	.owner = THIS_MODULE
};


int init_char_device(void)
{
    int ret;

	/*1. Dynamically allocate a device number */
	ret = alloc_chrdev_region(&device_number,0,1,"pmon_devices");

	if(ret < 0)
    {
		pr_err("Alloc chrdev failed\n");
		goto out;
	}

	pr_info("Device number <major>:<minor> = %d:%d\n",MAJOR(device_number),MINOR(device_number));

	/*2. Initialize the cdev structure with fops*/
	cdev_init(&pcd_cdev,&pcd_fops);

	/* 3. Register a device (cdev structure) with VFS */
	pcd_cdev.owner = THIS_MODULE;
	ret = cdev_add(&pcd_cdev,device_number,1);
	if(ret < 0)
    {
		pr_err("Pmon add failed\n");
		goto unreg_chrdev;
	}

	/*4. create device class under /sys/class/ */
	class_pcd = class_create(THIS_MODULE,"pcd_class");
	if(IS_ERR(class_pcd))
    {
		pr_err("Class creation failed\n");
		ret = PTR_ERR(class_pcd);
		goto cdev_del;
	}

	/*5.  populate the sysfs with device information */
	device_pcd = device_create(class_pcd,NULL,device_number,NULL,"pmon");
	if(IS_ERR(device_pcd))
    {
		pr_err("Device create failed\n");
		ret = PTR_ERR(device_pcd);
		goto class_del;
	}

	pr_info("Character device init was successful\n");

	return 0;

class_del:
	class_destroy(class_pcd);
cdev_del:
	cdev_del(&pcd_cdev);	
unreg_chrdev:
	unregister_chrdev_region(device_number,1);
out:
	pr_info("haracter device insertion failed\n");
	return ret;
}

void exit_char_device(void)
{
	device_destroy(class_pcd,device_number);
	class_destroy(class_pcd);
	cdev_del(&pcd_cdev);
	unregister_chrdev_region(device_number,1);

	pr_info("Character device unloaded\n");
}


static int __init pmon_driver_init(void)
{
	int ret;

    ret = init_kprobe();

    if(ret > 0)
    {
        return ret;
    }
    
    ret = init_char_device();
    
    if(ret > 0)
    {
        return ret;
    }

	pr_info("module loaded\n");

    return ret;
}



static void __exit pmon_driver_cleanup(void)
{
    exit_kprobe();
    exit_char_device();
}


module_init(pmon_driver_init);
module_exit(pmon_driver_cleanup);

MODULE_LICENSE("GPL");

