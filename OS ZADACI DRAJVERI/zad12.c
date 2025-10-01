// Да се напише модул (драјвер) кој работи со големина од 1КВ. Драјверот дозволува
// последователно запишување и читање од меморијата, така што, до каде што претходно
// запишал/прочитал, продолжува од таму со запишување/читање. Не може да се прочита повеќе
// од запишаното во меморијата. Доколку се обиде корисникот да се запише повеќе од што има
// преостанато меморија, се ресетираат бројачите, запишувањето и читањето почнува од почеток
// (тековно што сакал да запише корисникот, ако надминува преостанатата меморија, целото се
// запишува од почеток и бројачот за прочитано се ресетира). За секоја изведена операција
// (читање или запишување), драјверот следните 5 секунди не дозволува никаква операција.
// Дообјаснување: за пресметка за поминати секунди, може да се користи функцијата:
// #include <linux/time.h>
// void getnstimeofday(struct timespec *ts)
// Функцијата на влез ја добива структурата:
// struct timespec {
//  time_t tv_sec; /* seconds */
//  long tv_nsec; /* nanoseconds */
// };
// каде преку tv_sec враќа моменталното време во секунди. 


#include <linux/module.h>
#include <linux/init.h>
#include "zad2.h"
#include <linux/time.h>

MODULE_AUTHOR("flaop");
MODULE_DESCRIPTION("flaop driver");

char mem[1024];
int r,w;
int flag;
struct timespec last_op_time;

// ovie 4 se isti
static int r_init(void);
static void r_cleanup(void);
module_init(r_init);
module_exit(r_cleanup);

static int r_init(void)
{
    
    r=0;
    w=0;
last_op_time.tv_sec=0;

    // ova dole e sekade vaka
    printk("<1>Zdravo ninji\n");
    if (register_chrdev(60, "cprimer1", &my_fops))
    {
        printk("<1>nejke da registrira\n");
    }
    return 0;
}

void getnstimeofday(struct timespec *ts)

struct timespec {
 time_t tv_sec; /* seconds */
 long tv_nsec; /* nanoseconds */
};



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
    struct timespec ts;
    getnstimeofday(&ts);

    int pominati_sekundi = ts.tv_sec - last_op_time.tv_sec;

    if(pominati_sekundi < 5){
        return -EBUSY;
    }

    if(r>=w){
        return 0;
    }

    if(count > w-r){
        count w-r;
    }

    if (copy_to_user(buffer, mem + r, count)) {
        return -EFAULT;
    }
    r+=count;
    return count;

}

ssize_t my_write(struct file *filep, const char *buff, size_t count, loff_t *offp) // ista za site
{
    struct timespec ts;
    getnstimeofday(&ts);

    int pominati_sekundi = ts.tv_sec - last_op_time.tv_sec;

    if(pominati_sekundi < 5){
        return -EBUSY;
    }

    if(count > 1024-w){
        r=0;
        w=0;
    }

    if (copy_from_user(&mem[w], buff, count)) {
        return -EFAULT;
    }

    w+=count;

    return count;
}