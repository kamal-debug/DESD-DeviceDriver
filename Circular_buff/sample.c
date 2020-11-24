#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/circ_buf.h>
#include<linux/slab.h>
#include<linux/uaccess.h>


#define SIZE 32



MODULE_LICENSE("GPL");
static dev_t devnum;
static struct cdev _cdev;
struct circ_buf cbuf;

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
    int i,ret,mini;

    mini = min(count,(size_t)CIRC_CNT(cbuf.head,cbuf.tail,SIZE));

    printk("Size from Userspace for Read:%d\n",(int)count);
    for ( i = 0; i < mini; i++)
    {
        ret=copy_to_user(buff+i,cbuf.buf+cbuf.tail,1);
        if(ret)
        {
            printk("Error copying to user");
            return -EFAULT;
        }
        printk("Copied %c to user\n",cbuf.buf[cbuf.tail]);
        cbuf.tail = (cbuf.tail+1) & (SIZE-1);
    }

    return i;
    
}
static ssize_t sample_write(struct file *filp,const char __user *buff,size_t count, loff_t *offp)
{
    int ret,i;
    printk("Size sent from UserSpace %d\n",(int)count);
    for ( i = 0; i < count; i++)
    {
        ret= copy_from_user(cbuf.buf+cbuf.head,buff+i,1);
        if(ret)
        {
            printk("Error Copying From User");
            return -EFAULT;
        }
        printk("Copied From User %c \t",cbuf.buf[cbuf.head]);
        cbuf.head = (cbuf.head + 1) & (SIZE-1);
    }
    
    return i;
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

    cbuf.buf = kmalloc(SIZE,GFP_KERNEL);
    if(!cbuf.buf)
    {
        printk("Memory is not Allocated\n");
        unregister_chrdev_region(devnum,1);
        return 0;
    }

    ret = cdev_add(&_cdev,devnum,1);
    if(ret)
    {
        printk("unable to Add\n");
        unregister_chrdev_region(devnum,1);
        kfree(cbuf.buf);
        return ret;
    }
    printk("Init Successful\n");
    return 0;
    
}

static void __exit sample_exit(void)
{

    cdev_del(&_cdev);
    kfree(cbuf.buf);
    unregister_chrdev_region(devnum,1);
    printk("unregister Successful\n");
}

module_init(sample_init);
module_exit(sample_exit);

