// Да се напише Модул за Линукс ОС за запишување на податоци. Модулот има првична меморија од
// 1024KB и огледална (mirror) меморија од исто толку KB. Иницијално меморијата е празна. Кога
// првичната меморија не е полна, се запишува во таа меморија сите податоци што се запишуваат во
// модулот, а се прави дополнителна копија во огледалната меморија (од крајот кон почетокот). Во
// моментот кога ќе се наполни оригиналната меморија, огледалната меморија служи како дополнителна
// меморија, така што дополнителните податоци се запишуваат таму почнувајќи од почеток. Кога ќе се
// прочита од модулот, доколку огледалната меморија има копија од првичната меморија се прави
// проверка дали тоа што се чита е исто и во првичната и во огледалната (Double Check). Доколку е исто,
// на корисникот му се враќа информација дека меморијата е Double Checked. Доколку не е исто, на
// корисникот се враќа информација дека имало Double Check ама истиот не е успешно извршен. Како трет
// случај се зема кога огледалната меморија се користи како дополнителна меморија, тогаш се враќа
// информација на корисникот дека не е возможно да се направи Double Check. 


#include <linux/module.h>
#include <linux/init.h>
#include "zad2.h"

MODULE_AUTHOR("flaop");
MODULE_DESCRIPTION("flaop driver");

char mem[1024];
char ogl[1024];
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
    if(w==0){
        return 0;
    }

    if(count > 1024){
       count = 1024;
    }
   
    
    for(int i=0; i<count; i++){

        buff[i] = mem[r];

        if(r < 1024){
            if(mem[r] == ogl[1024-r-1]){
                printk(KERN_INFO "Double Checked: OK\n");
            }else{
                printk(KERN_INFO "Double Checked: FAIL\n");
            }
        }else{
            printk(KERN_INFO "Double Check not possible\n");
        }

        r++;
        if(r>=1024){
            r=0;
        }
    }
    return count;

}

ssize_t my_write(struct file *filep, const char *buff, size_t count, loff_t *offp) // ista za site
{
    if(count > 2*1024){
        return -EINVAL;
    }

    if(w+count <= 1024){
        
        if (copy_from_user(mem + w, buff, count)) {
            return -EFAULT;
        }
        w += count;
    int granica_vo_ogl = w;
    for(int i=0; i<granica_vo_ogl; i++){
        ogl[1023-i] = mem[i];
    }

    }else{

        int ostanato = 1024-w;
        int vo_ogledalen_dodadeni = count - ostanato;

        if(copy_from_user(mem+w, buff , ostanato)){
            return -EFAULT;
        }
        
        w=1024;

        if(copy_from_user(ogl, buff+ostanato , vo_ogledalen_dodadeni)){
            return -EFAULT;
        }
    }
    return count;
}