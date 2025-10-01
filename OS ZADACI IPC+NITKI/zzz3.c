// Да се напише програма во С која што ќе креира К нитки (К се внесува од тастатура), кои што ќе
// пребаруваат зборови во датотека. Главната нитка ги креира К-те нитки и притоа на секоја една од
// нитките и кажува кој збор да го пребарува. Тоа значи дека пред да се креира нитката, треба од тастатура
// да се внесе зборот и истиот да и се прати на нитката за пребарување. Секоја нитка го пребарува својот
// збор во датотеката и печати на екран колку пати го нашла зборот. Изминувањето на датотеката да се
// прави преку мемориско мапирање на истата, притоа, секоја една од нитките и кажува на главната нитка
// колку пати го има пронајдено зборот. Оваа информација, нитките ја споделуваат со главната нитка
// преку заедничка меморија. Главната нитка, откако ќе завршат сите останати нитки, на екран печати
// колку вкупно зборови биле најдени (од сите К зборови) во датотеката.


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

#define MAX_DOLZINA_ZBOR 100
#define MAX_NITKI
int K;
char * mapped_data;
char niza_zborovi[MAX_NITKI][MAX_DOLZINA_ZBOR]; //max zborovi so dolzina 100
int *rezultat;

void *prebaraj(void *arg){
    int indeks = *(int *)arg;
    char *zbor = niza_zborovi[indeks];
    int brojac;
    char *pokazuvac = mapped_data;

    while(pokazuvac = strstr(pokazuvac, zbor) != NULL){
        brojac++;
        pokazuvac+=strlen(zbor);
    }
    rezultat[indeks] = brojac;

}

int main(int argc, char *argv[])
{
    printf("Vnesi broj za K: ");
    scanf("%d", &K); //broj na nitki - brojot na zborovite

    for(int i=0; i<K; i++){
        printf("Vnesi zbor za nitka: ");
        scanf("%d", niza_zborovi[i]);
    }


    int fd = open(argv[1], O_RDONLY); // Отвори датотека во режим за читање
    if (fd == -1) {
        perror("Ne mozam da ja otvoram datotekata");
        return 1;
    }

    int golemina_na_datoteka = lseek(fd, 0, SEEK_END); //ja zimam goleminata na datotekata (kon datotekata, pomestuvanje=0, od kade da pocne pomestuvanjeto)
    if(golemina_na_datoteka == -1){
        perror("lseek failed");
        close(fd);
        return -1;
    }
    lseek(fd,0,SEEK_SET);

    mapped_data = mmap(NULL, golemina_na_datoteka, PROT_READ, MAP_PRIVATE, fd, 0); //cel tekst od datoteka kako niza od karakteri
    if (mapped_data == MAP_FAILED) {
        perror("Ne mozam da ja mapiram datotekata");
        close(fd);
        return 1;
    }


    pthread_t threads[MAX_NITKI];
    int nitka_id[MAX_NITKI];

    for(int i=0; i<K; i++){
        nitka_id[i] = i;
        pthread_create(&threads[i], NULL, prebaraj, &nitka_id[i]); 
    }

    
    for (int i = 0; i < K; i++) {
        pthread_join(threads[i], NULL); // Чекај нитките
    }

    int vkupno_najdeni = 0;
    for (int i = 0; i < K; i++) {
        vkupno_najdeni += rezultat[i];
    }
    printf("Vkupno najdeni zborovi: %d\n", vkupno_najdeni); // Испечати вкупно најдени зборови

    
    munmap(mapped_data, golemina_na_datoteka);
    close(fd);

}