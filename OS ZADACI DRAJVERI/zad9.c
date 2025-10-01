// Да се напише драјвер за дешифрирање на текст. Драјверот работи така што во својата меморија
// чува текст со големина од најмногу 512 бајти. Текстот е иницијално не е шифриран додека не се
// запише нешто во драјверот, притоа првото запишување во драјверот е шифрата која ќе се
// користи понатаму. Доколку се прочита од драјверот пред да се запише шифрата, се враќаат 0
// бајти. Потоа, дешифрирањето на текстот од драјверот се прави со запишување на шифрата во
// драјверот. Доколку шифрата што се запишува во драјверот е точна, тогаш на следното читање од
// драјверот, текстот е дешифриран. На секое читање на текстот, најпрво истиот мора да биде
// дешифриран со запишување на шифрата. Шифрирањето на текстот се прави со помош на
// готовата функција void) меморија. Кај првиот процес, третиот аргумент е името shifriraj(char *shifra, char *tekst, char *shifriran). Доколку по
// дешифрирање на текстот корисникот запише нешто во драјверот, тогаш тоа ќе ја пребрише
// содржината на текстот во драјверот.


#include <linux/module.h>
#include <linux/init.h>
#include "zad2.h"

MODULE_AUTHOR("flaop");
MODULE_DESCRIPTION("flaop driver");

char tekst[512];
int KEY, TEXT;
char kluc[128]; 


// ovie 4 se isti
static int r_init(void);
static void r_cleanup(void);
module_init(r_init);
module_exit(r_cleanup);

void shifriraj(char *shifra, char *tekst, char *shifriran) {
    int i, pom, j = strlen(shifra);
    for (i = 0; i < strlen(tekst); i++) {
        pom = ((int)tekst[i]) + ((int)shifra[i % j]);
        if (pom > 255) pom = 255;
        shifriran[i] = pom;
    }
    shifriran[i] = '\0';
}

void deshifriraj(char *shifra, char *shifriran, char *tekst) {
    int i, pom, j = strlen(shifra);
    for (i = 0; i < strlen(shifriran); i++) {
        pom = ((int)shifriran[i]) - ((int)shifra[i % j]);
        if (pom < 0) pom = 0;
        tekst[i] = pom;
    }
    tekst[i] = '\0';
}


static int r_init(void)
{
   
    KEY=0;
    TEXT=0;

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
    char decrypted[512]; //memorija za desifriran tekst

    if(!KEY){ //ako nemame sifra ne mozes da citash
        shifriraj(kluc, tekst, decrypted);


        if (copy_to_user(buff, decrypted, strlen(decrypted))) {
            return -EFAULT;
        }
        return strlen(decrypted); //dolzina na procitaniot tekst

    }

    
    if (copy_to_user(buff, tekst, strlen(tekst))) {
        return -EFAULT;
    }

    KEY=0;
    return strlen(tekst); //dolzina na procitaniot tekst
}


ssize_t my_write(struct file *filep, const char *buff, size_t count, loff_t *offp) // ista za site
{

    char korisnicki[512]; //pomosna memorija

    if(count >= 512){
        return -EINVAL;
    }

    if (copy_from_user(korisnicki, buff, count)) {
        return -EFAULT;
    }
   
    korisnicki[count] = '\0';

    if(!KEY){ //prvoto zapisuvanje e sifra
        strncpy(kluc, korisnicki, sizeof(kluc));
        KEY = 1; //sifrata e postavena
        return count;
    }

    if (strcmp(korisnicki, kluc) == 0) { //ako sifrata e tocna
        TEXT = 1;  //tekstot e sifriran
        return count;
    }

    if (TEXT) { //ako tekstot e sifriran 
        shifriraj(kluc, korisnicki, TEXT); //se sifrira
        return count;
    }
}