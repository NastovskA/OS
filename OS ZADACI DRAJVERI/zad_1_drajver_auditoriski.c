// • Да се креира виртуелен знаковен уред до кој би праќале податоци.
// Потоа да се креира драјвер кој ќе може да запише и да прочита од овој
// уред.
// • Решение:
// Креирање на виртуелен знаковен
// sudo mknod /dev/ured c 60 0
// C  ќе креира знаковен уред
// 60 Major number
// 0  Minor number

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>

#define DEVICE_NAME "hello1"
#define MAJOR_NUM 60

static int major_number;
static struct class* hello_class = NULL;
static struct device* hello_device = NULL;

// Структура за операциите со датотека
static int hello_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "hello_open: successful\n");
    return 0;
}

static int hello_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "hello_release: successful\n");
    return 0;
}

static ssize_t hello_read(struct file *file, char *buf, size_t count, loff_t *ptr)
{
    printk(KERN_INFO "hello_read: returning zero bytes\n");
    return 0; // нема податоци за читање
}

static ssize_t hello_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
    printk(KERN_INFO "hello_write: accepting zero bytes\n");
    return count; // нема податоци за пишување
}

// Структура за дефинирање на операции
static struct file_operations hello_fops = {
    .open = hello_open,
    .release = hello_release,
    .read = hello_read,
    .write = hello_write,
};

// Инициализација на модулот
static int hello_init(void)
{
    int ret;
    printk(KERN_INFO "Hello Example Init\n");

    ret = register_chrdev(MAJOR_NUM, DEVICE_NAME, &hello_fops);
    if (ret < 0) {
        printk(KERN_ALERT "Error registering hello device\n");
        return ret;
    }

    printk(KERN_INFO "Hello: registered module successfully!\n");
    return 0;
}

// Излегување од модулот
static void hello_exit(void)
{
    unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
    printk(KERN_INFO "Hello Example Exit\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Angela Nastovska");
MODULE_DESCRIPTION("A simple Linux char driver for the Hello world example.");
MODULE_VERSION("0.1");
