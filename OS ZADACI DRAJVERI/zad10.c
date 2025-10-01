// Да се напише модул за работа со меморија. Модулот има три мемории од 512 бајти. Кога некој ќе се
// обиде да запише во модулот, се запишува паралелно во сите три мемории истиот податок, притоа секое
// следно запишување продолжува од каде што прекинало претходното (append тип на запишување).
// Доколку стигне на крајот од меморијата, почнува да запишува од почеток, игнорирајќи го остатокот од
// меморијата.
// При читање од драјверот, на првото (четвртото, седмо, десетто и т.н.) читање се чита од првата
// меморија, на второто (петто, осмо, единаесето и т.н.) читање од втората меморија, и на трето (шесто,
// деветто, дванаесето и т.н.) се чита од третата меморија. Притоа, кога е четвртото читање (тоа значи од
// првата меморија), тоа продолжува да чита од таму од каде што прекинало претходното читање на таа
// меморија (во случајот првото читање) и т.н.


#include <linux/module.h>
#include <linux/init.h>
#include "zad2.h"

MODULE_AUTHOR("flaop");
MODULE_DESCRIPTION("flaop driver");

char mem1[512];
char mem2[512];
char mem3[512];
int w;
int r1,r2,r3;

// ovie 4 se isti
static int r_init(void);
static void r_cleanup(void);
module_init(r_init);
module_exit(r_cleanup);

static int r_init(void)
{
    w=0;
    r1=r2=r3=0;

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
    int *r; //pokazuva kon soodvetnoto citanje
    char *mem; // pokazuvac kon soodvetna memorija

    if (w==0){
        return 0;
    }

    switch (*offp % 3)
    {
    case 0:
        mem = mem1;
        r=&r1;
        break;
    case 1:
        mem = mem2;
        r=&r2;
        break;
    case 2:
        mem = mem3;
        r=&r3;
        break;
    default:
        break;
    }
    if(*r >= 512){
        *r=0;
    }

    int ostanati_biti_za_citanje;

    if(*r == w){ //pozicija od kade sto posledno se zapisalo
        return 0;
    }

    if(w >= *r){
        ostanati_biti_za_citanje = w - *r;
    }
    
    if (count > ostanati_biti_za_citanje){
        count = ostanati_biti_za_citanje;
    }

    if(r*+count <= 512){

        if(copy_to_user(buff, mem+r* , count)){
            return -EFAULT;
        }
        r*+=count;
    }else{
        int ostanato = 512-r;
        int od_pocetok_dodadeni = count - ostanato;

        if(copy_to_user(buff, mem+r*, ostanato)){
            return -EFAULT;
        }
        // if(copy_to_user(buff+ostanato, mem, od_pocetok_dodadeni)){
        //     return -EFAULT;
        // }
       // *r=od_pocetok_dodadeni;
       *r=0;
    }
    
    (*offp)++; //zgolemuvanje na broj na citanje za da se rotira niz memoriite
        return count;



}


ssize_t my_write(struct file *filep, const char *buff, size_t count, loff_t *offp) // ista za site
{
    if(count > 512){
        count=512;
    }

    if(w+count <= 512){

        if(copy_from_user(mem1+w, buff , count)){
            return -EFAULT;
        }
        if(copy_from_user(mem2+w, buff , count)){
            return -EFAULT;
        }
        if(copy_from_user(mem3+w, buff , count)){
            return -EFAULT;
        }
        w+=count;
    }else{
        int ostanato = 512-w;
        int od_pocetok_dodadeni = count - ostanato;

        if(copy_from_user(mem1+w, buff , ostanato)){
            return -EFAULT;
        }
        if(copy_from_user(mem2+w, buff , ostanato)){
            return -EFAULT;
        }
        if(copy_from_user(mem3+w, buff , ostanato)){
            return -EFAULT;
        }


        if(copy_from_user(mem1, buff+ostanato , od_pocetok_dodadeni)){
            return -EFAULT;
        }
        if(copy_from_user(mem2, buff+ostanato , od_pocetok_dodadeni)){
            return -EFAULT;
        }
        if(copy_from_user(mem3, buff+ostanato , od_pocetok_dodadeni)){
            return -EFAULT;
        }
        w = od_pocetok_dodadeni;
    }
    posledno_zapisan = count;

    return count;
}
