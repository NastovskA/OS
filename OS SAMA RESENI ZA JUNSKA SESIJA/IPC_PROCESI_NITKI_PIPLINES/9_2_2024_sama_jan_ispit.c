//PROGRAMA 1
#include <stdio.h>          
#include <stdlib.h>         
#include <unistd.h>         
#include <sys/types.h>      
#include <sys/ipc.h>        
#include <sys/shm.h>        
#include <pthread.h>        
#include <time.h>           
#include <string.h>  

#define KEY 12345
#define dolzina_na_niza 1000
#define maksimalen_broj_nitki 10

int *shared_memory;
int broj_nitki;
int niza_na_vrednost_na_sekoja_nitka[maksimalen_broj_nitki];

void *prebaraj( void *arg){ //LOGIKATA ZA SEKOJA NITKA

    int indeks = *(int *)arg;
    int broj = niza_na_vrednost_na_sekoja_nitka[indeks];

    while(1){
        sleep(1);

        if(shared_memory[dolzina_na_niza] == indeks){ //DALI E NITKATA IZBRANA
            int brojac = 0;
            for(int i=0; i<dolzina_na_niza; i++){
                if(shared_memory[i] == broj){
                    brojac++;
                }
            }
            printf("Nitkata %d go barase brojot %d i go najde %d pati! ", indeks, broj, brojac);
            shared_memory[1001] = 1;
        }
    }
    return NULL;
}


int main(int argc, char *argv[]){

    printf("Vnesi broj na nitki: ");
    scanf("%d", &broj_nitki);

    if (broj_nitki > maksimalen_broj_nitki){
        broj_nitki = maksimalen_broj_nitki;
        return 1;
    }

    int shmid = shmget(KEY, sizeof(int) * 1002, 0666 | IPC_CREATE) // shmget(key, size, permissions | flags)
    // 1002 = 1000(NITKI) + 1(INDEKS KOJA NITKA DA RABOTI) + 1(FLAG DALI E ZAVRSENO)
    //IPC_CREATE ako postoi ke ti go dade postoeckiot segment, a ako ne postoi ke sozdade nov
    if(shmid < 0){
        perror("Neuspesno mapiranje!");
        exit(1);   
    }

    shared_memory = (int *)shmat(shmid, NULL, 0); //shared mem attach, shmat vraka void i zatoa jas imam (int *)

    for(int i=0; i< broj_nitki; i++){
        printf("Vnesi vrednost na samata nitka: ");
        scanf("%d", &niza_na_vrednost_na_sekoja_nitka[i]);
    }

    pthread_t threads[maksimalen_broj_nitki]; //ja cuva ID na sekoja nitka
    int indeks_nitka[maksimalen_broj_nitki]; //preku ova ja prakam nitkata na funkcijata

    for(int i=0; i<broj_nitki; i++){
        indeks_nitka[i] = i;
        pthread_create(&threads[i], NULL, prebaraj, &indeks_nitka[i]); //kreira nova nitka, sekoe prebaraj dobiva razlicen indeks 
        //so &indeks_nitka se praka na funkcijata prebaraj
    }

    for(int i=0; i<broj_nitki; i++){
        pthread_join(threads[i], NULL); //sekoja nitka cekam da zavrsi
    }

    return 0;
}

//PROGRAMA 2
#include <stdio.h>          
#include <stdlib.h>         
#include <unistd.h>         
#include <sys/types.h>      
#include <sys/ipc.h>        
#include <sys/shm.h>        
#include <pthread.h>        
#include <time.h>           
#include <string.h>  

#define KEY 12345
#define dolzina_na_niza 1000
#define maksimalen_broj_nitki 10
int *shared_memory;


int main(int argc, char *argv[]){
    int shmid = shmget(KEY, sizeof(int) * 1002, 0666 | IPC_CREAT) // shmget(key, size, permissions | flags)
    // 1002 = 1000(NITKI) + 1(INDEKS KOJA NITKA DA RABOTI) + 1(FLAG DALI E ZAVRSENO)
    //IPC_CREATE ako postoi ke ti go dade postoeckiot segment, a ako ne postoi ke sozdade nov
    if(shmid < 0){
        perror("Neuspesno mapiranje!");
        exit(1);   
    }

    shared_memory = (int *)shmat(shmid, NULL, 0); //shared mem attach, shmat vraka void i zatoa jas imam (int *)

    srand(time(NULL));

    for(int i=0; i<dolzina_na_niza; i++){ //od 0 do 999 site breovi
        printf("Vnesi vrednost za lokacija %d: ", i);
        scanf("%d", &shared_memory[i]);
    }

    int izvrseni_nitki[maksimalen_broj_nitki] = {0};
    int vkupno = 0;

    while(vkupno <= broj_nitki){
        int indeks_nitka = rand() % broj_nitki;
        if (izvrseni_nitki[indeks_nitka] == 1){
            continue;
        }
        shared_memory[1000] = indeks_nitka; //i kazuvam na prog 1 koja nitka da raboti
        shared_memory[1001] = 0; //za restartiranje

        printf("Aktivirana e nitkata %d. Cekame odgovor.... ", indeks_nitka);

        while(shared_memory[1001] == 0){ //momentalno se izvrsuva dr i ti treba da spies
            sleep(1); 
        }

        printf("Nitkata %d zavrsi so rabota. ", indeks_nitka);
        izvrseni_nitki[indeks_nitka] = 1;
        vkupno++;

    shmdt(shared_memory);
    return 0;
    }
}