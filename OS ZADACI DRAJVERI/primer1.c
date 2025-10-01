

#include<linux/module.h>
#include<linux/init.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <asm/current.h>
#include <asm/segment.h>
#include <asm/uaccess.h>

MODULE_AUTHOR("flaop");
MODULE_DESCRIPTION("flaop driver");
static int r_init(void);
static void r_cleanup(void);

module_init(r_init);
module_exit(r_cleanup);

static int my_open(struct inode *inode,struct file *filep);
static int my_release(struct inode *inode,struct file *filep);
static ssize_t my_read(struct file *filep,char *buff,size_t count,loff_t *offp );
static ssize_t my_write(struct file *filep,const char *buff,size_t count,loff_t *offp );

struct file_operations my_fops={
	open: my_open,
	read: my_read,
	write: my_write,
	release:my_release,
};

static int my_open(struct inode *inode, struct file *file)
{
printk("hello_open: successful\n");
return 0;
}
static int my_release(struct inode *inode, struct file *file)
{
printk("hello_release: successful\n");
return 0;
}
static int r_init(void)
{
printk("<1>Zdravo ninji\n");
if(register_chrdev(60,"primer1",&my_fops)){
	printk("<1>nejke da registrira\n");
}
return 0;
}
static void r_cleanup(void)
{
printk("<1>sajonara\n");
unregister_chrdev(60,"cprimer1");
return ;
}
static ssize_t my_read(struct file *file, char *buf,
size_t count,loff_t *ptr)
{
printk("hello_read: returning zero bytes\n");
return 0;
}
static ssize_t my_write(struct file *file, const char
*buf, size_t count,
loff_t * ppos)
{
printk("hello_write: accepting zero bytes\n");
return 0;
}
MODULE_LICENSE("DUAL BSD/GPL");
