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

#include <linux/timer.h>

#define DRIVER_AUTHOR "grupo 3"
#define DRIVER_DESC   "Kills a given process by its PID"
#define DRIVER_LICENSE "GPL"

// 29 ctrl
// 56 alt
bool ctrl = false;
bool alt = false;
bool a = false;
int pidKeys[8]={-1,-1,-1,-1,-1,-1,-1,-1};
int pid;
int timerKeys[8]={-1,-1,-1,-1,-1,-1,-1,-1};
int time;
static struct timer_list my_timer;

void killProcess(void)
{
	int ret = kill_pid(find_vpid(pid), SIGKILL, 1);
	printk(KERN_INFO "KILL PROCESS %d - RETURN %d\n", pid, ret);
}

void timer_kill( unsigned long data )
{
	killProcess();
	printk( "my_timer_callback called.\n");
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
		if(ctrl && alt && !a && param->value>=2 && param->value<=11)
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
		}
		if(ctrl && alt && param->value==28)
		{
			int i, j, k = 0, timer=0;
			for (j = 0; j<8; j++)
				if(pidKeys[j]==-1)
					break;
			for (i = 0; i < j; i++)
			    k = 10 * k + pidKeys[i];
			pid = k;
			if(a)
			{
				for (j = 0; j<8; j++)
					if(timerKeys[j]==-1)
						break;
				for (i = 0; i < j; i++)
					timer = 10 * timer + timerKeys[i];
				// TODO call killProcess when timer is over
				setup_timer( &my_timer, timer_kill, 0 );
				mod_timer( &my_timer, jiffies + msecs_to_jiffies(timer*1000) );
				printk( "Starting timer to fire in %ds (%ld)\n", timer*1000, jiffies );
			}
			else
			{
				killProcess();
			}
		}
		if(ctrl && alt && param->value==30)
		{
			a = true;
			printk(KERN_INFO "BEGUN LISTENING TO THE TIME\n");
		}
		if(ctrl && alt && a && param->value>=2 && param->value<=11)
		{
			int i, key;
			key = param->value-1;
			if (key == 10)
				key = 0;
			for(i=0;i<8;i++)
			{
				if(timerKeys[i]==-1)
				{
					timerKeys[i]=key;
					break;
				}
			}
			printk(KERN_INFO "READING TIMER VALUE");
		}
		// DEBUG TO FIND OUT WHAT KEY CODE YOU WANT
		// printk(KERN_INFO "%d\n", param->value);
        }
        if(!param->down)
        {
		if(param->value==29)
		{
			int i = 0;
			ctrl = false;
			a = false;
			for(i=0;i<8;i++)
			{
				pidKeys[i]=-1;
				timerKeys[i]=-1;
			}
			printk(KERN_INFO "CONTROL RELEASED\n");
		}
		else if(param->value==56)
		{
			int i = 0;
			alt = false;
			a = false;
			for(i=0;i<8;i++)
			{
				pidKeys[i]=-1;
				timerKeys[i]=-1;
			}
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
