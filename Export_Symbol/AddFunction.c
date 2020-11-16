#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>

MODULE_LICENSE("GPL");

static int HelloWorld_add(int a,int b);



static int __init hello_init(void) // kernel Initialisation
{
    printk(KERN_ALERT "We are in Init Module of AddFunction..\n");
    printk(KERN_ALERT "Hello, world\n");
    return 0;
}
int HelloWorld_add(int a, int b) // Addition Function
{
    printk(KERN_ALERT "we are in Helloworld_add function\n");
    return a+b;

}
EXPORT_SYMBOL_GPL(HelloWorld_add);// Export the Addition Function

static void __exit hello_exit(void) // kernel Deinitialisation
{
    printk(KERN_ALERT "We are in Cleanup Module of AddFunction\n");
    printk(KERN_ALERT "Goodbye, World\n");
}

module_init(hello_init);
module_exit(hello_exit);