#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/keyboard.h>
#include <linux/signal.h>

#include <linux/sched.h>

#include <asm/siginfo.h>	//siginfo
#include <linux/debugfs.h>
#include <linux/uaccess.h>
#include <linux/pid.h>

#define DRIVER_AUTHOR "grupo 3"
#define DRIVER_DESC   "Kills a given process by its PID"
#define DRIVER_LICENSE "GPL"

// 29 ctrl
// 56 alt
bool ctrl = false;
bool alt = false;
int pidKeys[8]={-1,-1,-1,-1,-1,-1,-1,-1};
int time;

void killProcess(int pid)
{
	int ret = kill_pid(find_vpid(pid), SIGKILL, 1);
	printk(KERN_INFO "KILL PROCESS %d - RETURN %d\n", pid, ret);
}


int killer_notify(struct notifier_block *nblock, unsigned long code, void *_param)
{
    struct keyboard_notifier_param *param = _param;
    if (code == KBD_KEYCODE)
    {
        if(param->down)
        {
		if(param->value==29)
		{
			ctrl = true;
			printk(KERN_INFO "CONTROL PRESSED\n");
		}
		else if(param->value==56)
		{
			alt = true;
			printk(KERN_INFO "ALT PRESSED\n");
		}
		if(ctrl && alt && param->value>=2 && param->value<=11)
		{
			int i, key;
			key = param->value-1;
			if (key == 10)
				key = 0;
			for(i=0;i<8;i++)
			{
				if(pidKeys[i]==-1)
				{
					pidKeys[i]=key;
					break;
				}
			}
			for(i=0;i<8;i++)
			{
            			printk(KERN_INFO "%d - ", pidKeys[i]);
			}
			printk(KERN_INFO "\n");
		}
		if(ctrl && alt && param->value==28)
		{
			int i, j, k = 0;
			for (j = 0; j<8; j++)
				if(pidKeys[j]==-1)
					break;
			for (i = 0; i < j; i++)
			    k = 10 * k + pidKeys[i];
			killProcess(k);
		}
		if(ctrl && alt && param->value==2020)
		{
			// TODO USER PRESSED T, get time and wait that
			// DO NOT USE SLEEP
		}
		// DEBUG TO FIND OUT WHAT KEY CODE YOU WANT
		//printk(KERN_INFO "%d", param->value);
        }
        if(!param->down)
        {
		if(param->value==29)
		{
			int i = 0;
			ctrl = false;
			for(i=0;i<8;i++)
				pidKeys[i]=-1;
			printk(KERN_INFO "CONTROL RELEASED\n");
		}
		else if(param->value==56)
		{
			int i = 0;
			alt = false;
			for(i=0;i<8;i++)
				pidKeys[i]=-1;
			printk(KERN_INFO "ALT RELEASED\n");
		}
        }
    }

    return NOTIFY_OK;
}

static struct notifier_block killer_nb =
{
    .notifier_call = killer_notify
};


static int __init init_killer(void)
{
    register_keyboard_notifier(&killer_nb);
    return 0;
}

static void __exit cleanup_killer(void)
{
    unregister_keyboard_notifier(&killer_nb);
}

module_init(init_killer);
module_exit(cleanup_killer);
MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
