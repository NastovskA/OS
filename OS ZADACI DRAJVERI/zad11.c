// Да се напише модул (драјвер) кој работи со големина од 1КВ. Драјверот дозволува
// последователно запишување и читање од меморијата во принцип на кружен бафер, така што, до
// каде што претходно запишал/прочитал, продолжува од таму со запишување/читање. Не може
// да се прочита повеќе од запишаното во меморијата. Се што има прочитано корисникот се смета
// за избришано од меморијата т.е. може да се запишува повторно таму. Доколку се обиде
// корисникот да се запише повеќе од што има преостанато меморија, и доколку од почеток е
// прочитано Х бајти, корисникот ја пополнува меморијата, а преостанатото го запишува од
// почеток (до каде што е прочитано т.е. кружен бафер). 


#include <linux/module.h>
#include <linux/init.h>
#include "zad2.h"

MODULE_AUTHOR("flaop");
MODULE_DESCRIPTION("flaop driver");

char mem[1024];
int r,w;
int posledno_zapisano;

// ovie 4 se isti
static int r_init(void);
static void r_cleanup(void);
module_init(r_init);
module_exit(r_cleanup);

static int r_init(void)
{
    r=0;
    w=0;
    posledno_zapisano=0;

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

    if(count > posledno_zapisano){
        count = posledno_zapisano;
    }

    if(r+count <= 1024){
       
            if(copy_to_user(buff, mem+r , count)){
                return -EFAULT;
            }
            r+=count;
        
    }else{
        int ostanato = 1024-r;
        int od_pocetok_dodadeni = count - ostanato;
        if(copy_to_user(buff, mem+r, ostanato)){
            return -EFAULT;
        }
        if(copy_to_user(buff+ostanato, mem, od_pocetok_dodadeni)){
            return -EFAULT;
        }
        r=od_pocetok_dodadeni;
    }
    posledno_zapisano-=count; //koga ke procitam mi se namaluva za da izbrisam
    return count;
}

ssize_t my_write(struct file *filep, const char *buff, size_t count, loff_t *offp) // ista za site
{

    if(count > 1024){
        count=1024;
    }

    if(w+count <= 1024){

        if(copy_from_user(mem+w, buff , count)){
            return -EFAULT;
        }
        w+=count;
    }else{
        int ostanato = 1024-w;
        int od_pocetok_dodadeni = count - ostanato;

        if(copy_from_user(mem+w, buff , ostanato)){
            return -EFAULT;
        }
        if(copy_from_user(mem, buff+ostanato , od_pocetok_dodadeni)){
            return -EFAULT;
        }
        w = od_pocetok_dodadeni;
    }
    posledno_zapisan += count; //mora da go ddodadam brojot na novo zapisani

    return count;
}