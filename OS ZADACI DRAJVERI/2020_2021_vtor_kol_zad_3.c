// Да се напише Модул за Линукс ОС за запишување на податоци. Модулот работи со меморија од 512
// бајти, притоа не дозволува да се чита од меморијата додека истата не е целосно пополнета и
// обратно. Иницијално, при вклучување на модулот, меморијата е празна. Додека меморијата не се
// пополни целосно, модулот дозволува само запишување во меморијата. Откако меморијата ќе се
// пополни целосно, модулот дозволува само читање од меморијата, притоа, читањето го прави од
// десно на лево. Доколку се побараат 10 бајти да бидат прочитани, тие ќе бидат последните 10 бајти
// од меморијата. На следното читање, ќе се вратат предпоследните count бајти и т.н. се додека не се
// прочита целата меморија до почеток. Потоа, системот повторно дозволува само запишување во
// меморијата до нејзино целосно исполнување.


#include <linux/module.h>
#include <linux/init.h>
#include "zad2.h"

MODULE_AUTHOR("flaop");
MODULE_DESCRIPTION("flaop driver");

char memorija[512];
int r,w;
int locked; // 0 - zapisuvame, 1-citame


static int r_init(void);
static void r_cleanup(void);
module_init(r_init);
module_exit(r_cleanup);

static int r_init(void)
{

    r=0;
    w=0;
    locked=0;

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
////////////////////////////////////////////////////////////////// nagore se isto

ssize_t my_read(struct file *filep, char *buff, size_t count, loff_t *offp)
{
    //count = kolku bajti sakam da procitam
    //w = kade posledno e zapisan bajt
    //r= kolku bajti se procitani od skupno 512

    if(locked == 1){                                       //cpunter = 5, imas procitano 7 bajti
        if((512-r) > count){                               //citam od pozadi, 512 - 7 = 505 dali 505 > 5 daaa, znaci moze uste da citame 
            copy_to_user(buff,memorija+512-r-count,count); //citam od indeks 500 i zimam 5 bajte
            r += count;                                    //sega r = 10 bidejki imam vkupno 10 bajti procitano (5prethodno+5novi)
            return count;
        }
        else{ //r==count==512
            int k=r; //k=0
            copy_to_user(buff,memorija,r); //kopirame 512 bajti vo baf
            w=0; //za povtorno da zapisuvame
            locked=0; //rezim na pisuvanje
            return k; //treba da vrati 512 kako uspesno procitano se
        }
    }else{
        return 0;
    }
}


ssize_t my_write(struct file *filep, const char *buff, size_t count, loff_t *offp)
{
    if(locked==0){
        if((w+count) < 512){
            copy_from_user(memorija+w, buff, count);
            w+=count;
            return count;
        }else{

            int k = 512-w; //imam uste k biti slobodni

            copy_from_user(memorija+w, buff, count); //od bafer napisi gi tie so falat
            w=512;
            locked=1;
            r=0;
            return k;

        }
    }else{
        return 0;
    }
}

#endif