#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

MODULE_LICENSE("GPL");

static char* charvar = "module";
static int intvar = 20;

module_param(charvar,charp,S_IRUGO);
module_param(intvar,int,S_IRUGO);

static int __init param_init(void)
{
    printk(KERN_ALERT "we are in init function\n");
    printk(KERN_ALERT "CHARVAR IS:%s\n",charvar);
    printk(KERN_ALERT "INTVAR IS:%d\n",intvar);
    return 0;
}

static void param_exit(void)
{
    printk(KERN_ALERT "We are in Exit function\n");
    printk(KERN_ALERT "Goodbye, World\n");
}

module_init(param_init);
module_exit(param_exit);
