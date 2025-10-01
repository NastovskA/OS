// Да се напише драјвер во C програмскиот јазик кој што ќе овозможува контролирано
// читање на блокови од меморијата на даден уред.Меморијата на уредот е 4KB, притоа предефинирано се
// читаат по 64 бајти (блок) од меморија.На секое последователно читање на бајти од
// меморијата покажувачот се поместува во десно.Кога ќе стигне до крајот на меморијата, се
// рестартира покажувачот и почнува од почеток. Доколку се запише нешто во драјверот,
// тогаш се менува големината на блоковите што се читаат од меморија.Доколку се смени
// големината на блокот со вредност која што не е делива со 64, тогаш се чита следната
// големина најблиску до број делив со 64 (пример: за блок од 130 бајти се читаат 192 бајти
// меморија).Доколку се внесе вредност поголема од 4K, тогаш најмногу се чита 4К.


#include <linux/module.h>
#include <linux/init.h>
#include "zad2.h"

MODULE_AUTHOR("flaop");
MODULE_DESCRIPTION("flaop driver");


char memorija[4*1024];
int blok;
int r, w;
int flag; // 0 citanje 1 zapisuvanje

// ovie 4 se isti
static int r_init(void);
static void r_cleanup(void);
module_init(r_init);
module_exit(r_cleanup);

static int r_init(void)
{

    blok = 64;
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
    int mozam_da_citam;
    if(count > blok){
        mozam_da_citam = blok;
    }else{
        mozam_da_citam=count;
    }

    if(r + mozam_da_citam > (4*1024)){
        mozam_da_citam = (4*1024) - r;
    }

    if(copy_to_user(buff, memorija+r, mozam_da_citam)){ //dokolku ne uspee neso za da se isprati, se vraka EFAULT
        return -EFAULT;
    }
    r += mozam_da_citam;

    if(r >= 4*1024){
        r=0;
    }
    return mozam_da_citam;
}


ssize_t my_write(struct file *filep, const char *buff, size_t count, loff_t *offp) // ista za site
{
    char pom[100]; //od 100 bajti, ovde ke bide brojot sto ke sakam da se prosiri

    if((copy_from_user(pom, buff, count))){ //gi zimam podatocite od korisnikot
        return _EFAULT;
    }
//kaj ssakam da ja napravam konverzijata, null, vo koj sistem - 10 za decimalen
    int ns = simple_strtol(pom, NULL, 10);//konverzija za vrednostite od string da bidat int vrednosti

    if(ns <= 0){
        blok = 64;
    }else if(ns>4096){
        blok = 4096;
    }else if(ns % 64 > 0){
        blok = ((ns / 64) + 1) * 64; //bez ostatok
    }else{
        blok = ns;
    }
    return count;
}
