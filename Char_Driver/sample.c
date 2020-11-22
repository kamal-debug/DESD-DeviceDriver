#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/cdev.h>
#include<linux/fs.h>




MODULE_LICENSE("GPL");
dev_t devnum;
static struct cdev _cdev;


static int sample_open(struct inode *inode, struct file *filp)
{
    printk("Sample Open\n");
    return 0;
}
static int sample_release(struct inode *inode, struct file *filp)
{
    printk("Sample Release\n");
    return 0;
}
static ssize_t sample_read(struct file *filp,char __user *buff,size_t count, loff_t *offp)
{
    printk("Sample Read");
    return 0;
}
static ssize_t sample_write(struct file *filp,const char __user *buff,size_t count, loff_t *offp)
{
   
    printk("Sample Write\n");
   
    
    return 0;
}
struct file_operations fops={
    .open=sample_open,
    .release=sample_release,
    .read = sample_read,
    .write = sample_write,
};

static int __init sample_init(void)
{
    int ret;
    devnum = MKDEV(42,0);

    ret = register_chrdev_region(devnum,1,"sample_dev");
    if(ret){
        printk("kernel did not grant this Device no.");
        return ret;
    }
    

    cdev_init(&_cdev,&fops);

    

    ret = cdev_add(&_cdev,devnum,1);
    if(ret)
    {
        printk("unable to Add\n");
        unregister_chrdev_region(devnum,1);
        
        return ret;
    }
    printk("Init Successful\n");
    return 0;
    
}

static void __exit sample_exit(void)
{
    cdev_del(&_cdev);
    unregister_chrdev_region(devnum,1);
    printk("unregister Successful\n");
}

module_init(sample_init);
module_exit(sample_exit);

