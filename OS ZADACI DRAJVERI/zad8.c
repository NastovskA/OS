// Да се напише драјвер за комуникација со уред. Уредот има меморија од 1КВ. Податоците во меморијата
// се заклучени иницијално со 2 клучеви така што меморијата мора да се отклучи пред истата да се користи.
// Доколку меморијата е заклучена не може да се чита ниту да се запишува во драјверот. За отклучување, во
// драјверот мора да се запише првин клучот, па доколку е точен, следната операција кон драјверот е
// овозможена. Првиот клуч има вредност „juni123“, додека пак, вториот клуч има вредност „juni!@#“.
// Доколку се запише првиот клуч, драјверот е отклучен само за една операција после клучот, и после
// повторно е заклучен истиот. Доколку се запише вториот клуч, драјверот е отклучен за повеќе операции
// после тоа, т.е. е отворен за операции додека повторно не се запише вториот клуч во драјверот. 


#include <linux/module.h>
#include <linux/init.h>
#include "zad2.h"

MODULE_AUTHOR("flaop");
MODULE_DESCRIPTION("flaop driver");

char mem[1024]
int kluc; // 1 - edna operacija 2-poveke operacii 0-zakluceno
int r,w;



// ovie 4 se isti
static int r_init(void);
static void r_cleanup(void);
module_init(r_init);
module_exit(r_cleanup);

static int r_init(void)
{
    r=0;
    w=0;
    kluc=0;
    

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
    if(kluc == 0){
        return -EFAULT;
    }

    if(count > 1024-r){
        count = 1024-r;
    }

    if (copy_to_user(buff, mem + r, count)) {
        return -EFAULT;
    }

    r+=count;

    if(kluc == 1){
        kluc=0;
    }
    return count;
}

ssize_t my_write(struct file *filep, const char *buff, size_t count, loff_t *offp) // ista za site
{
  

    if(kluc == 0){
        char lokalen_bafer[16];

        if (copy_from_user(lokalen_bafer, buff, count)) {
            return -EFAULT;
        }
    
        // Ако се внесе првиот клуч (juni123), времено отклучување
        if (lokalen_bafer[0] == 'j' && lokalen_bafer[1] == 'u' && lokalen_bafer[2] == 'n' &&
            lokalen_bafer[3] == 'i' && lokalen_bafer[4] == '1' && lokalen_bafer[5] == '2' &&
            lokalen_bafer[6] == '3') {
            kluc = 1;  // Времено отклучување
            return count;
        }
    
        // Ако се внесе вториот клуч (juni!@#), постојано отклучување
        if (lokalen_bafer[0] == 'j' && lokalen_bafer[1] == 'u' && lokalen_bafer[2] == 'n' &&
            lokalen_bafer[3] == 'i' && lokalen_bafer[4] == '!' && lokalen_bafer[5] == '@' &&
            lokalen_bafer[6] == '#' && lokalen_bafer[7] == '3') {
            kluc = 2;  // Постојано отклучување
            return count;
        }
    }

    if(count > 1024-w){
        count = 1024-w;
    }

    if (copy_from_user(mem + w, buff, count)) {
        return -EFAULT;
    }
    w+=count;

    if(kluc == 1){
        kluc=0;
    }
    return count;

}

