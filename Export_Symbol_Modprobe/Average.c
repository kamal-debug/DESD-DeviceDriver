#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>


MODULE_LICENSE("GPL");
static int HelloWorld_add(int a,int b);

int a=20,b=20;



static int __init hello_init(void) // kernel Initialisation
{
    int Average;
    printk(KERN_ALERT "We are in Init Module of Average..\n");
    printk(KERN_ALERT "Hello, world\n");
    Average=HelloWorld_add(a,b)/2;
    printk(KERN_ALERT "Average is:%d\n",Average);

    return 0;
}



static void __exit hello_exit(void) // kernel Deinitialisation
{
    printk(KERN_ALERT "We are in Cleanup Module of Average\n");
    printk(KERN_ALERT "Goodbye, World\n");
}

module_init(hello_init);
module_exit(hello_exit);

// Module Paramter

module_param(a,int,S_IRUGO);
module_param(b,int,S_IRUGO);