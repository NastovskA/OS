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

char originalna[1024];
char ogledalna[1024];
int use;
char *pocetok, *kraj;

static int r_init(void);
static void r_cleanup(void);
module_init(r_init);
module_exit(r_cleanup);

static int r_init(void)
{

    use = 0;
    pocetok = originalna;
    kraj = ogledalna + 1024 - 1;

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
    if (use == 0) // seuste sum vo orginalniot magacin
    {
        int kk; //kolku mozeme da procitame
        if (pocetok - orig >= count) //kolku imame izminato, ako e poveke mozeme samo da citame max kolku sto ni dava count
        {
            kk = count; 
        }
        else
        {
            kk = pocetok - orig; //kolku sto imame izminato
        }

        int flag = 1; //deka podatocite se isti i vo orginalna i vo ogledalna

        char *p, *k;
        p = pocetok; //pokazuva kon krajot na zapisani podatoci vo orginalnna
        k = kraj - kk; // na pocetok od kopiranite podatoci vo ogled

        for (int i = 0; i < kk; i++)
        {
            if (*p != *k) //dali bajt po bajt se isti vo org i vo ogl
            {
                flag = 0; //nasol edna razlika 
                break; //prekinuva
            }
            p--;
            k++;
        }
        copy_to_user(buff, flag == 1 ? "2" : "1", 1); // ako flag = 1 znaci imame isti podatoci vo org i vo ogl => ke isprati 2 sto znaci Double chech uspese, 1 neuspesen
        copy_to_user(buff + 1, pocetok - kk, kk);
        return kk;
    }
    else //ako sme vo ogledaloto
    {
        if (pocetok > ogledalo) //pisuvame vo ogledaloto
        {
            if (pocetok - ogledalo > count)
            {
                copy_to_user(buff, "0", 1); //0 vo bafer -> double check ne e mozno poradi koristenje na ogledalna memorija
                copy_to_user(buff + 1, pocetok - count, count); //kopiram COUNT od POCETOK-COUNT vo BUFF+1
                return count;
            }
            else
            {
                int k = pocetok - ogledalo;
                copy_to_user(buff, "0", 1); 
                copy_to_user(buff + 1, ogledalo, k);
                copy_to_user(buff + 1 + k, orig + 10240 - (count - k), count - k);
                pocetok = orig + 102040 - (count - k);
                return count;
            }
        }
        else
        {
            if (pocetok - orig > count)
            {
                copy_to_user(buff, "0", 1);
                copy_to_user(buff + 1, pocetok - count, count);
                return count;
            }
            else
            {
                int k = pocetok - orig;
                copy_to_user(buff, "0", 1);
                copy_to_user(buff + 1, orig, k);
                use = 0;
                return k;
            }
        }
    }
}
ssize_t my_write(struct file *filep, const char *buff, size_t count, loff_t *offp)
{
    if (use == 0) //pisuvame vo orginalnata
    {
        if (pocetok + count < orig + 1024)
        {
            copy_from_user(orig + pocetok, buff, count);
            for (int i = 0; i < count; i++)
            {
                *kraj = *poc;
                kraj--;
                poc++;
            }
            return count;
        }
        else
        {
            int k = orig + 10240 - pocetok;
            copy_from_user(pocetok, buff, k);
            copy_from_user(ogledalo, buff + k, count - k);
            pocetok = ogledalo + count - k;
            use = 1;
        }
    }
    else //pisuvame i vo ogledaloto
    {
        if (pocetok + count < ogledalo + 10240)
        {
            copy_from_user(pocetok, buff, count);
            pocetok + = count;
            return count;
        }
        else
        {
            int k = ogledalo + 10240 - pocetok;
            copy_from_user(pocetok, buff, k);
            pocetok += k;
            return k;
        }
    }
}
#endif