#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/circ_buf.h>
#include<linux/slab.h>
#include<linux/uaccess.h>
#include<linux/sched.h> // header For Wait Queue
#include<linux/wait.h> // header For Wait Queue

#include<linux/delay.h> // for sleep

#include<linux/semaphore.h> // for semaphore

#include "myioctl.h" // header files for ioctl

#define SIZE 32
#define MAJORN 42
#define NMINORS 3



MODULE_LICENSE("GPL");

static dev_t devnum;

struct mydevice{

     struct cdev _cdev;
    struct circ_buf cbuf;
    struct semaphore _sem; // semaphore
     wait_queue_head_t _wq;

}_devs[NMINORS];

static int sample_open(struct inode *inode, struct file *filp)
{
    struct mydevice *_devp = container_of(inode->i_cdev,struct mydevice,_cdev);

    filp->private_data = _devp;
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
    struct mydevice *_devp = filp->private_data;

    wait_event_interruptible(_devp->_wq,CIRC_CNT(_devp->cbuf.head,_devp->cbuf.tail,SIZE)>0); // We are waiting For the Data available to us, otherwisw it will go on Sleep();

    mini = min(count,(size_t)CIRC_CNT(_devp->cbuf.head,_devp->cbuf.tail,SIZE));

    printk("Size from Userspace for Read:%d\n",(int)count);
    for ( i = 0; i < mini; i++)
    {
        ret=copy_to_user(buff+i,_devp->cbuf.buf+_devp->cbuf.tail,1);
        if(ret)
        {
            printk("Error copying to user");
            return -EFAULT;
        }
        printk("Copied %c to user\n",_devp->cbuf.buf[_devp->cbuf.tail]);
        _devp->cbuf.tail = (_devp->cbuf.tail+1) & (SIZE-1);
    }

    return i;
    
}
static ssize_t sample_write(struct file *filp,const char __user *buff,size_t count, loff_t *offp)
{
    int ret,i;
    struct mydevice *_devp = filp->private_data;

    down_interruptible(&_devp->_sem);// Down

    printk("Size sent from UserSpace %d\n",(int)count);
    for ( i = 0; i < count; i++)
    {
        ret= copy_from_user(_devp->cbuf.buf+_devp->cbuf.head,buff+i,1);

        
        if(ret)
        {
            printk("Error Copying From User");
            return -EFAULT;
        }
        msleep(300); // sleep value

        printk("Copied From User %c \t",_devp->cbuf.buf[_devp->cbuf.head]);
        _devp->cbuf.head = (_devp->cbuf.head + 1) & (SIZE-1);
    }
    up(&_devp->_sem); // up Semaphore;
    wake_up(&_devp->_wq); // Notified the wait Queue that now Data Is present To Read(Come out from Wait State)
    return i;
}
long sample_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    switch (cmd)
    {
            case CMD_1:
                        printk("THIS IS ioctl CMD_1\n");
                        break;
            case CMD_2:
                        printk("This is ioctl CMD_2\n");
                        break;
            default:
                    printk("Not Support IOCTL\n");
                    return -ENOTTY;
        
    }
    return 0;

}
struct file_operations fops={
    .open=sample_open,
    .release=sample_release,
    .read = sample_read,
    .write = sample_write,
    .unlocked_ioctl=sample_ioctl, // ioctl Function
};

static int __init sample_init(void)
{
    int ret,i,j;
    dev_t tmpdev;
    devnum = MKDEV(MAJORN,0);
    ret = register_chrdev_region(devnum,NMINORS,"sample0");
    if(ret){
        printk("kernel did not grant this Device no.");
        goto reg_out;
    }
    
    for ( i = 0; i < NMINORS; i++)
    {
    
        cdev_init(&_devs[i]._cdev,&fops);

        _devs[i].cbuf.buf = kmalloc(SIZE,GFP_KERNEL);
        if(!_devs[i].cbuf.buf)
        {
            printk("Memory is not Allocated\n");
            goto cbuf_out;
            
        }
        sema_init(&_devs[i]._sem,1); // Initialize the Semaphore;

        init_waitqueue_head(&_devs[i]._wq); // Initialize Wait Queue

    }

    for ( j = 0; j < NMINORS; j++)
    {
        
        tmpdev = MKDEV(MAJOR(devnum),MINOR(devnum)+j);
        ret = cdev_add(&_devs[j]._cdev,tmpdev,1);
        if(ret)
        {
            printk("unable to Add\n");
            goto cdev_out;
        }

    }

    printk("Init Successful\n");
    return 0;

cdev_out:
            for (--j; j >=0; j--)
            {
                cdev_del(&_devs[j]._cdev);
            }
cbuf_out:
            for (--i; i >=0; i--)
            {
                kfree(_devs[i].cbuf.buf);
            }
reg_out:
            unregister_chrdev_region(devnum,NMINORS);
            return -1;
    
}

static void __exit sample_exit(void)
{
    int i;
    for ( i = 0; i < NMINORS; i++)
    {
        cdev_del(&_devs[i]._cdev);
        kfree(_devs[i].cbuf.buf);
    }

    unregister_chrdev_region(devnum,NMINORS);
    printk("unregister Successful\n");
}

module_init(sample_init);
module_exit(sample_exit);

