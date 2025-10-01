// Да се напише знаковен модул(драјвер) за Linux оперативен систем. Модулот располага
// со меморија од 1КВ знаковна меморија, која иницијално е празна.Запишувањето и
// читањето од меморијата се прави од лево на десно, притоа за секои наредни читања /
// запишувања, продолжува од каде што има застанато со претходното читање /
// запишување, притоа не може да се прочита повеќе од што има запишано во
// меморијата.Модулот, во процесот на запишување во меморијата, дозволува да се запише
// најмногу 50 % од слободната (преостаната) меморија во модулот.Тоа значи дека
// доколку(на пример) во даден момент меморијата е исполнета 512 бајти, најмногу што ќе
// дозволи да се запише во драјверот се половина од преостанатите 512 бајти т.е. 256
// бајти.Доколку се обиде да се запише повеќе од 50 % од преостаната слободна меморија
// во модулот, модулот ќе запише само 50 % од преостанатата меморија.


#include <linux/module.h>
#include <linux/init.h>
#include "zad2.h"

MODULE_AUTHOR("flaop");
MODULE_DESCRIPTION("flaop driver");

char mem[1024];
int r,w;
int flag;

// ovie 4 se isti
static int r_init(void);
static void r_cleanup(void);
module_init(r_init);
module_exit(r_cleanup);

static int r_init(void)
{
    r=0;
    w=0;
    flag=0;


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
    int mozam_da_procitam = w-r;
    
    if(r > w){
        r=0;
        w=0;
        return 0;
    }

    if(count > mozam_da_procitam){
        count = mozam_da_procitam; 
    }

    if(copy_to_user(buff, mem + r, count)){
        return -EFAULT;
    }
    r+=count;

    return count;

}


ssize_t my_write(struct file *filep, const char *buff, size_t count, loff_t *offp) // ista za site
{
    int sloboden_del = 1024 - w;
    int del_za_zapisuvanje = sloboden_del/2;
    int zapisano;

    if(count > del_za_zapisuvanje){
        zapisano = del_za_zapisuvanje;
    }else{
        zapisano=count;
    }

    if(copy_from_user(mem+w, buff, zapisano)){
        return -EFAULT;
    }
    w+=zapisano;
    return zapisano;

}

