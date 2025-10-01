// Да се напише драјвер за комуникација со уред. Уредот има меморија од 10КВ, поделена
// во два дела (6 КВ се за читање и запишување, 4 КВ се резервирани за дополнително
// користење). Податоците во меморијата се запишуваат од лево на десно и доколку нема
// слободна меморија, не се запишува ништо. Доколку корисникот се обиде да запише N
// Бајти меморија, тие ги запишува во слободниот простор во кој нема веќе запишано
// меморија. Доколку има помалку од N бајти слободна меморија, тие податоци воопшто
// не се запишуваат во уредот. Читањето од уредот се прави од лево на десно, така што
// доколку корисникот се обиде да прочита N бајти од уредот, се читаат последните
// запишани N бајти во уредот почнувајќи од последниот. Доколку има преостанато
// помалку од N бајти за читање, тогаш се враќаат само преостанатите бајти. Кога ќе се
// прочитаат N (или помалку од N бајти) од уредот, истите се бришат (т.е. се поставуваат
// како слободни за запишување).

#include <linux/module.h>
#include <linux/init.h>
#include "zad2.h"

MODULE_AUTHOR("flaop");
MODULE_DESCRIPTION("flaop driver");

char mem[1024 * 6];
char dop[1024 * 4];
int flag;
int w;

static int r_init(void);
static void r_cleanup(void);
module_init(r_init);
module_exit(r_cleanup);

static int r_init(void)
{

    w = 0;
    flag = 0; // 0-nema memorija 1-moze da zapisuva

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

ssize_t my_read(struct file *filep, char *buff, size_t count, loff_t *offp)
{
    int z;

    if (flag == 0)
    {
        if (w >= count)
        {
            z = count;
        }
        else
        {
            z = w;
        }

        copy_to_user(buff, (mem + w) - z, z);
        w -= z;
    }
    else
    {
        if (w >= count)
        {
            z = count;
            copy_to_user(buff, (dop + w) - z, z);
            w -= z;
        }
        else
        {
            copy_to_user(buff, dop, w);
            z = count - w;
            w = 6 * 1024;
            copy_to_user(buff, mem + w - z, z);
            w -= z;
            flag = 0;
        }
    }
}