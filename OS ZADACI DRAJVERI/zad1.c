// Да се напише Модул за Линукс ОС за запишување на податоци.Модулот работи со
// меморија од 512 бајти, притоа не дозволува да се чита од меморијата додека истата не е
// целосно пополнета и обратно.Иницијално, при вклучување на модулот, меморијата е
// празна.Додека меморијата не се пополни целосно, модулот дозволува само запишување
// во меморијата.Откако меморијата ќе се пополни целосно, модулот дозволува само читање
// од меморијата, притоа, читањето го прави од десно на лево.Доколку се побараат 10 бајти
// да бидат прочитани, тие ќе бидат последните 10 бајти од меморијата.На следното читање,
// ќе се вратат предпоследните count бајти и т.н.се додека не се прочита целата меморија до
// почеток.Потоа, системот повторно дозволува само запишување во меморијата до нејзино
// целосно исполнување.

#include <linux/module.h>
#include <linux/init.h>
#include "zad2.h"

MODULE_AUTHOR("flaop");
MODULE_DESCRIPTION("flaop driver");
char memorija[512];
int r, w;
int flag; // 0 citanje 1 zapisuvanje

// ovie 4 se isti
static int r_init(void);
static void r_cleanup(void);
module_init(r_init);
module_exit(r_cleanup);

static int r_init(void)
{

    r = 0;
    w = 0;
    flag = 0;

    // ova dole e sekade vaka
    printk("<1>Zdravo ninji\n");
    if (register_chrdev(60, "cprimer1", &my_fops))
    {
        printk("<1>nejke da registrira\n");
    }
    return 0;
}

// go ima sekade vaka
static void r_cleanup(void)
{
    printk("<1>sajonara\n");
    unregister_chrdev(60, "cprimer1");
    return;
}

// drv.h
/*
 * my device header file
 */

#ifndef _MY_DEVICE_H
#define _MY_DEVICE_H
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <asm/current.h>
#include <asm/segment.h>
#include <asm/uaccess.h>

int my_open(struct inode *inode, struct file *filep);
int my_release(struct inode *inode, struct file *filep);
ssize_t my_read(struct file *filep, char *buff, size_t count, loff_t *offp);
ssize_t my_write(struct file *filep, const char *buff, size_t count, loff_t *offp);

struct file_operations my_fops = {
    open : my_open,
    read : my_read,
    write : my_write,
    release : my_release,
};

int my_open(struct inode *inode, struct file *filep)
{
    printk("<1> Sega sum vo OPEN\n");
    return 0;
}

int my_release(struct inode *inode, struct file *filep)
{
    printk("<1> Sega sum vo RELEASE\n");
    return 0;
}
//////////////////////////////////////////////////////////////////1 zapisuva   0 cita
ssize_t my_read(struct file *filep, char *buff, size_t count, loff_t *offp) // ovoj red e ist
{
    if (flag == 1)
    {
        printk(KERN_INFO "Read not allowed: Buffer is not full yet\n");
        return -EPERM;
    }

    //r ostanati bajti
    if (flag == 0)
    {
        if(r>=count){

            if(copy_to_user(buff, memorija + r - count, count)){
                return -EFAULT;
            }
            r-=count;
        }else{

            int ostanati = r;
            if(copy_to_user(buff,memorija,ostanati)){
                return - EFAULT;
            }
            r=0;
            w=0;
            flag=1;
            return ostanati;
        }

    }
}

ssize_t my_write(struct file *filep, const char *buff, size_t count, loff_t *offp) // ista za site
{
    if(flag==0){
        printk(KERN_INFO "Read not allowed: Buffer is not full yet\n");
        return -EPERM;
    }

    if(flag==1){

        if((w+count) < 512){
            if(copy_from_user(memorija+w ,buff, count)) //od kade sakam da gi zapisam, do kade, kolku
            {
                return -EFAULT;
            }
            w+=count;
            return count;
        }else{
            int ostanati_do_kraj = 512-w;
            if(copy_from_user(memorija+w, 512, ostanati_do_kraj)){
                return -EFAULT;
            }
            r=512;
            w=512;
            flag=0; //spremno za citanje
            return ostanati_do_kraj;

        }
        
    }
}
#endif