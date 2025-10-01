// Да се напише драјвер за комуникација со уред. Уредот има меморија од 10KB, од која
// првата половина се користи за читање, додека пак втората половина за запишување.
// Податоците во меморијата се запишуваат/читаат во вид на кружен бафер. Доколку
// корисникот се обиде да прочита од уредот N бајти, тогаш драјверот ги чита првите N бајти.
// При следното читање на М бајти од страна на корисникот, драјверот ги чита бајтите од
// N+1 до N+M+1, доколку стигне до крај на меморијата, драјверот почнува од почеток да
// чита. Податоците се запишуваат во меморијата на ист начин. 

#include <linux/module.h>
#include <linux/init.h>
#include "zad2.h"

MODULE_AUTHOR("flaop");
MODULE_DESCRIPTION("flaop driver");

char mem[10*1024];
int r, w;

// ovie 4 se isti
static int r_init(void);
static void r_cleanup(void);
module_init(r_init);
module_exit(r_cleanup);

static int r_init(void)
{

    r=0;
    w= 5*1024;

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
    int mozam_da_procitam = (5*1024)-r; //slobodniot prostor
    int start = 0;
    int ke_procita =0;
    if(count > 5*1024){
        count = 5*1024;
    }
    
    if(r+count <= 5*1024){

        if(copy_to_user(buff, mem + r , count)){
            return -EFAULT;
        }
        r+=count;
    }else{
        int ostanato = 5*1024-r;
        int od_pocetok_dodadeni = count - ostanato;

        if(copy_to_user(buff, mem + r , ostanato)){
            return -EFAULT;
        }
        if(copy_to_user(buff+ostanato, mem , od_pocetok_dodadeni)){
            return -EFAULT;
        }
        r=od_pocetok_dodadeni;
    }

    return count;
}


ssize_t my_write(struct file *filep, const char *buff, size_t count, loff_t *offp) // ista za site
{
    int start = 5*1024;
    int mozam_da_zapisam = 10*1024 - start;

    int zapisuvam;

    if(count > mozam_da_zapisam){
        zapisuvam = mozam_da_zapisam;
    }else{
        zapisuvam = count;
    }

    if(w+count <= 10*1024){

        if(copy_from_user(mem+w, buff , count)){
            return -EFAULT;
        }
        w+=count;
    }else{
        int ostanato = 10*1024-w;
        int od_pocetok_dodadeni = count - ostanato;

        if(copy_from_user(mem+w, buff , ostanato)){
            return -EFAULT;
        }
        if(copy_from_user(mem+start, buff+ostanato , od_pocetok_dodadeni)){
            return -EFAULT;
        }
        w=start + od_pocetok_dodadeni;
    }

    return count;

}
#endif