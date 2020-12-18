#include <linux/kernel.h>
#include <linux/kprobes.h>

#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt,__func__


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

static int __init pmon_driver_init(void)
{
	int ret;

    ret = init_kprobe();

    if(ret > 0)
    {
        return ret;
    }

	pr_info("pmon module loaded\n");

    return ret;
}



static void __exit pmon_driver_cleanup(void)
{
    exit_kprobe();

	pr_info("pmon module unloaded\n");
}


module_init(pmon_driver_init);
module_exit(pmon_driver_cleanup);

MODULE_LICENSE("GPL");

