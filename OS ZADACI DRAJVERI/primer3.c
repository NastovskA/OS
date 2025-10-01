
#include<linux/module.h>
#include<linux/init.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <asm/current.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
// definiranje na promenlivi
char mem[1024];
int r,w;

MODULE_AUTHOR("OS2024");
MODULE_DESCRIPTION("OS Driver 2024  zadaca3");
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
printk("my_open: successful\n");
return 0;
}
static int my_release(struct inode *inode, struct file *file)
{
printk("hello_release: successful\n");
return 0;
}
static int r_init(void)
{
printk("<1>Simple znakoven modul\n");
if(register_chrdev(60,"zadaca3",&my_fops)){
	printk("<1>nejke da se registrira\n");
}
// inicijalizacija na promenlivi
w=r=0;
return 0;
}
static void r_cleanup(void)
{
printk("<1>Ciao\n");
unregister_chrdev(60,"zadaca3");
return ;
}
static ssize_t my_read(struct file *file, char *buf,
size_t count,loff_t *ptr)
{
int citaj=w-r>=count?count:w-r;
if(copy_to_user(buf,mem,citaj)){
printk("<1> Kernel -> userspace kopiranjeto padna!\n" );
return 0;
};
r+=citaj;
return citaj;
}
static ssize_t my_write(struct file *file, const char
*buf, size_t count,
loff_t * ppos)
{
int maxWrite = (1024-w)/2;
int zapisi=maxWrite>=count?count:maxWrite;
if(copy_from_user(mem,buf,zapisi)){
printk("<1> UserSpace -> kernel kopiranjeto padna!\n" );
return 0;
};
w+=zapisi;
return zapisi;
}
MODULE_LICENSE("DUAL BSD/GPL");
