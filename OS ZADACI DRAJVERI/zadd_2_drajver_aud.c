// Да се напише драјвер за виртуелен уред кој ќе врши
// запишување и читање во меморијата на драјверот
// Карактеристики на драјверот:
// - Бафер од 80 бајти
// - Знаковен уред
// - Мајor number = 60
// - Minor number = 0

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>

#define DEVICE_NAME "hello1"
#define MAJOR_NUM 60
#define BUFFER_SIZE 80

static int major_number;
static char buffer[BUFFER_SIZE];  // Бафер за податоци
static struct class* hello_class = NULL;
static struct device* hello_device = NULL;
static int open_count = 0;  // Бројач на отворања

// Структура за операциите со датотека
static int hello_open(struct inode *inode, struct file *file)
{
    open_count++;
    printk(KERN_INFO "hello_open: successful, opened %d time(s)\n", open_count);
    return 0;
}

static int hello_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "hello_release: successful\n");
    return 0;
}

static ssize_t hello_read(struct file *file, char *buf, size_t count, loff_t *ptr)
{
    int len = strlen(buffer);
    if (*ptr >= len) {
        return 0;  // Не има податоци за читање
    }
    
    // Враќање на податоци од баферот
    if (count > len - *ptr) {
        count = len - *ptr;
    }

    if (copy_to_user(buf, buffer + *ptr, count)) {
        return -EFAULT;
    }

    *ptr += count;
    printk(KERN_INFO "hello_read: %zu bytes read\n", count);
    return count;
}

static ssize_t hello_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
    if (count > BUFFER_SIZE) {
        count = BUFFER_SIZE;
    }

    // Копирање на податоци од корисничка меморија во баферот
    if (copy_from_user(buffer, buf, count)) {
        return -EFAULT;
    }

    printk(KERN_INFO "hello_write: %zu bytes written\n", count);
    return count;
}

// Структура за дефинирање на операции
static struct file_operations hello_fops = {
    .open = hello_open,
    .release = hello_release,
    .read = hello_read,
    .write = hello_write,
};

// Инициализација на модулот
static int __init hello_init(void)
{
    int ret;
    printk(KERN_INFO "Hello Example Init\n");

    ret = register_chrdev(MAJOR_NUM, DEVICE_NAME, &hello_fops);
    if (ret < 0) {
        printk(KERN_ALERT "Error registering hello device\n");
        return ret;
    }

    // Креирање на класата и уредот
    hello_class = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(hello_class)) {
        unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(hello_class);
    }

    hello_device = device_create(hello_class, NULL, MKDEV(MAJOR_NUM, 0), NULL, DEVICE_NAME);
    if (IS_ERR(hello_device)) {
        class_destroy(hello_class);
        unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(hello_device);
    }

    printk(KERN_INFO "Hello: registered module successfully!\n");
    return 0;
}

// Излегување од модулот
static void __exit hello_exit(void)
{
    device_destroy(hello_class, MKDEV(MAJOR_NUM, 0));
    class_unregister(hello_class);
    class_destroy(hello_class);
    unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
    printk(KERN_INFO "Hello Example Exit\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Angela Nastovska");
MODULE_DESCRIPTION("A simple Linux char driver with 80-byte buffer.");
MODULE_VERSION("0.1");
