// Да се напише драјвер во C програмскиот јазик кој што ќе овозможува контролирано читање на блокови
// од меморијата на даден уред. Меморијата на уредот е 4KB, притоа предефинирано се читаат по 64 бајти
// (блок) од меморија. На секое последователно читање на бајти од меморијата покажувачот се поместува
// во десно. Кога ќе стигне до крајот на меморијата, се рестартира покажувачот и почнува од почеток.
// Доколку се запише нешто во драјверот, тогаш се менува големината на блоковите што се читаат од
// меморија. Доколку се смени големината на блокот со вредност која што не е делива со 64, тогаш се чита
// следната големина најблиску до број делив со 64 (пример: за блок од 130 бајти се читаат 192 бајти
// меморија). Доколку се внесе вредност поголема од 4K, тогаш најмногу се чита 4К.

#include<linux/module.h>
#include<linux/init.h>
#include"zad2.h"

MODULE_AUTHOR("flaop");
MODULE_DESCRIPTION("flaop driver");

char mem[4096];
int size;
int read;

//ovie 4 se isti 
static int r_init(void);
static void r_cleanup(void);
module_init(r_init);
module_exit(r_cleanup);

static int r_init(void){
    size=64; //POCETNA GOLEMINA NA BLOKOT STO SE CITA
    read=0; //POZICIJA OD KADE TRGNUVA SO CITANJE

    //ova dole e sekade vaka
    printk("<1>Zdravo ninji\n");
if(register_chrdev(60,"cprimer1",&my_fops)){
printk("<1>nejke da registrira\n");
}
return 0;
}

//go ima sekade vaka
static void r_cleanup(void)
{
    printk("<1>sajonara\n");
unregister_chrdev(60,"cprimer1");
return ;
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

int my_open(struct inode *inode,struct file *filep);
int my_release(struct inode *inode,struct file *filep);
ssize_t my_read(struct file *filep,char *buff,size_t count,loff_t *offp );
ssize_t my_write(struct file *filep,const char *buff,size_t count,loff_t *offp );

struct file_operations my_fops={
    open: my_open,
    read: my_read,
    write: my_write,
    release:my_release,
};


int my_open(struct inode *inode,struct file *filep)
{
printk("<1> Sega sum vo OPEN\n");
return 0;
}


int my_release(struct inode *inode,struct file *filep)
{
printk("<1> Sega sum vo RELEASE\n");
return 0;
}
////////////////////////////////////////////////////////////////// nagore se isto


ssize_t my_read(struct file *filep,char *buff,size_t count,loff_t *offp ) //ovoj red e ist
{
if(read + size <=4096){ 
 copy_to_user(buff,mem+read,size); //ja kopirame sodrzinata od memorija mem vo korisnicka memorija buff vo zadaden opseg size
 read+=size; //go premestuvame pokazuvacot na sledna pozicija
 return size; 
} else {
copy_to_user(buff,mem+read,4096-read);
read = 0; //se restartira dosol do kraj na memorijata
return 4096-read;
}
}




ssize_t my_write(struct file *filep,const char *buff,size_t count,loff_t *offp ) //ista za site
{
char newSize[30];
int ns;
copy_from_user(newSize,buff,count); //kopiranje od korisnicka memorija vo jadro
ns = atoi(newSize); //podatocite se pretvvaraat vo cel broj (vlezen tekst vo cel broj)
if(ns==0){ns=64;} //ako ne e validna memorija
if(ns>4096){ns=4096} //ako goleminata e pogolema od max, postavija kako granica max 
if(ns%64>0){ 
 size = (ns/64+1)*64; //ako goleminata ne e deliva so 64, ja zgolemuvame do najbliskata deliva vrednost
}
else {
 size = ns; //novata golemina na blokot
}
return count; //vraka broj na zapisani bajti
}

#endif