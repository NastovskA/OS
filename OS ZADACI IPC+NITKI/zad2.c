// Да се напише програма во C програмскиот јазик каде ќе се овозможи внесување на низа од цели броеви
// како аргументи на програмата. Потоа, програмата треба да дозволи внесување на непознат број на цели
// броеви за пребарување во низата. За секој внесен број, програмата креира нова нитка, преку која го
// пребарува тој број во низата и печати на екран колку пати се појавува тој број и колку има броеви во
// низата кои што се делители на тој број. Печатењето на екран не го прават нитките што го прават
// пребарувањето туку тие резултатот го препраќаат до друг процес преку мемориско мапирање.
// Внесувањето на броевите за пребарување прекинува кога ќе се внесе од тастатура нешто различно од
// цел број. 


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


int niza_numbers[100];
int golemina_na_niza;

int *mmap_rezultat;

void *prebaraj(void *arg){
    
    int broj = *(int *)arg; //broj sto go baram vo nizata na broevi
    int brojac_za_pojavuvanja = 0;
    int brojac_za_deliteli = 0;

    for(int i=0; i<golemina_na_niza; i++){
        if(niza_numbers[i] == broj){
            brojac_za_pojavuvanja++;
        }
        if(broj % niza_numbers[i] == 0){
            brojac_za_deliteli++;
        }
    }

    mmap_rezultat[0] = broj;
    mmap_rezultat[1] = brojac_za_pojavuvanja;
    mmap_rezultat[2] = brojac_za_deliteli;

    return NULL;
}




int main(int argc, char *argv[]){

    if(argc < 2){
        printf("Nevaliden broj na argumenti");
        return EXIT_FAILURE;
    }


    golemina_na_niza = argc -1; //kolku broevi se primeni kako argumenti

    for(int i=0; i<golemina_na_niza; i++){
        niza_numbers[i]=atoi(argv[i+1]);  //СИТЕ ЕЛЕМЕНТИ ЌЕ СЕ ЗАЧУВААТ ВО НИЗАТА
    }

    int fd=open("dat.txt",O_RDWR | O_CREAT | O_TRUNCATE,0x666); //kreirame datoteka koja kje ni bide dostapna preku mmap
    if(fd==-1){
        printf("Ne uspea fd");
        return EXIT_FAILURE;
    }

    //kreiranje na mmap ЗА КОМУНИКАЦИЈА МЕЃУ ПРОЦЕСОТ И НИТКАТА
    mmap_result=mmap(NULL,4000, PROT_READ | PROT_WRITE ,MAP_SHARED,fd,0);
    if(mmap_result==MAP_FAILED){
        printf("Ne uspea mmap");
        return -1;
    }

    printf("Pocni so vnesuvanje na broevite koi sakas da se prebaruvaat");
    printf("Za stop, vnesi 1");

    while(true){
        int broj;
        if(scanf("%d", &broj) != 1) break; //1 ke bide za stop

        //КРЕИРАЈ НИТКА ЗА СЕКОЈ БРОЈ
        pthread_t tid;
        pthread_create(&tid, NULL,PREBARAJ, &broj);//brojot kako argumet namesto indeksot

        pthread_join(tid, NULL); //ЧЕКАМ НИТКАТА ДА ЗАВРШИ

        printf("Brojot: %d e najden %d pati i ima %d deliteli  ",mmap_result[0],mmap_result[1],mmap_result[2]); //ГИ ЧИТА РЕЗУЛТАТИТЕ ОД МЕМ МАП
    }

    munmap(mmap_rezultat); //ПРЕКИН СО МАПИРАЊЕ
    close(fd); //ЗАТВАРА ДАТОТЕКА

    return 0;
}