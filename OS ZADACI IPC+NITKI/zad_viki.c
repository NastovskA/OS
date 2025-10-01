/*Да се напише програма во C која како аргумент добива име на непостоечка датотека.
Програмата најпрво креира дете процес. Родител процесот треба да генерира 1000 случајни
броеви со помош на функцијата rand() (од библиотеката stdlib.h) и истите ги запишува на екран
(кои со помош на редирекција се запишуваат во датотеката) и родител процесот завршува. Дете
процесот проверува дали може да ја отвори датотеката, и ако не може заспива 1 секунда. Потоа
проверува пак, и заспива пак 1 секунда, се додека не успее да ја отвори датотеката. Чим успее
да ја отвори, чита 1000 броеви од тастатура (редиректирани од датотеката) и ги сместува во
низа. Потоа, дете процесот, со помош на 4 нитки (секоја нитка пребарува по 250 броеви), го
наоѓа најголемиот број и го печати на екран.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>


#define MAX_NITKI 4
int broevi[1000];
int maksimalen_broj_po_nitka=0;

void *prebaraj(void *arg){

    int indeks = *(int *)arg; 
    int start, end;

    //gi ogranicuvam da ne se pletkaat edna vo druga
    if(indeks == 0){
        start = 0;
        end = 249;
    }else if(indeks == 1){
        start = 250;
        end = 499;
    }else if(indeks == 2){
        start = 500;
        end = 749;
    }else if(indeks == 3){
        start = 750;
        end = 999;
    }

    int lokalno_maksimalen_broj=broevi[start];
    for(int i=start; i<end; i++){
        if(broevi[i]>lokalno_maksimalen_broj){
            lokalno_maksimalen_broj=broevi[i];
        }
    }

    if(lokalno_maksimalen_broj>maksimalen_broj){
        maksimalen_broj=lokalno_maksimalen_broj;
    }

    return NULL;

}

int main(int argc, char *argv[]){

    if(argc < 2){
        printf("Nevaliden broj na argumenti");
        return EXIT_FAILURE;
    }

    pthread_t dete; //kreiram dete
    dete = fork();

    if(dete == 0){
        
        int fd;

        while((fd=open(argv[1], O_RDONLY)) == -1){
            sleep(1);
        }

        //treba da cita 1000 bajti od tastatura
        while(i<1000 && read(fd, &broevi[i], sizeof(int))>0){
            i++;
        }

        pthread_t thread[MAX_NITKI]; //kreiram 4 nitki
        int nitki_indeks[4];

        for(int i=0; i<4; i++){
            nitki_indeks[i] = i;
            pthread_create(&thread[i], NULL, prebaraj, &nitki_indeks[i]);
        }

        for(int i=0; i<4; i++){
            pthread_join(thread[i], NULL);
        }
        printf("Najgolemiot broj od tie 1000 e: %d", maksimalen_broj);

    }else{
        int fd=open("dat.txt",O_RDWR | O_CREAT | O_TRUNCATE,0x666); //kreirame datoteka koja kje ni bide dostapna preku mmap
        if(fd==-1){
            printf("Ne uspea fd");
            return EXIT_FAILURE;
        }

        srand(time(NULL));
        close(fd);
        
        for(int i=0; i<1000; i++){//gi generiram breovite
            printf("%d", rand()%1000);
        }
        return 0;
    }
}