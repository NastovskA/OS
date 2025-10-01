//PRISTAP DO SHARED MEMORIJA I ZAPISUVANJE I ZEMANJE OD NEA
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

//KONSTANTI STO NEMA DA SE SMENATI I VO DVETE PROGRAMI
#define KEY 12345
#define maksimalen_broj_nitki 10
#define niza_size 1000

//RABOTI KOI MI TREBAAT VO SHARED MEMORIJATA
int *mem; //pokazuvac kon memorija - share mem, ja sodrzi adresata na pocetokot, OTKAKO KE SE NAPRAVI SHMAT SE DOBIVA PRISTAP DO MEM[0], MEM[1]...
int nitki_id[maksimalen_broj_nitki]; //se praka kako argument na func baraj
int broj_za_prebaruvanja[maksimalen_broj_nitki]; //broj_za_prebaruvanja = {13,65,90,36,84}

void *baraj(void *arg){ //sekoja nitka ja povikuva ovaa funkcija, arg e pokazuvac kon int
    int indeks = *(int *) arg; //koja nitka e po red {indeks = 3}
    int broj = broj_za_prebaruvanja[indeks]; //BROJOT STO TREBA DA SE BARA , 36

    while(1){
        sleep(1);
        if (mem[0] != indeks){ //NITKATA NE E IZBRANA
            continue;


        }else{ //AKO NITKATA E IZBRANA OD VTORATA PROGRAMA
            int brojac = 0; //nizata e od mem[2] do mem[1001]
            for (int i=2; i<niza_size; i++){ //pocnuvame od 2 bidejki na 0-indeks na momentalno izbrana niska, na 1 -moze da e rezerva i na 2-stoi nizata do 100broevi
                if(mem[i] == broj){
                    brojac++;
                }
            }
            print("Nitka %d, go najde brojot %d, %d pati", mem[0], broj, brojac);
            mem[0]=-1; //moze da se bira druga nitka
        }
    }
    return NULL;
}


int main(int argc, char *argv[]){ //POVRZUVANJE SO MEMORIJATA
    int shared_mem_id = shmget(KEY, 1024, 0666); //PRISTAP SO KEY, KOLKAV PROSTOR IMA, DOZVOLENO ZA CITANJE/PISUVANJE
    if(shared_mem_id == -1){
        perror("Neuspesno mapiranje vo shared mem");
        exit(1);
    }

    mem = shmat(shared_mem_id,NULL,0); //MOZES DA RABOTIS SO MEMORIJATA KAKO SO NIZA
    mem[0] =-1; //ne e izbrana nitka inicijalno, i site cekaat

    printf("Vnesi broj na nitki: ");
    int br_nitki;
    scanf("%d", &br_nitki);

    if(br_nitki > maksimalen_broj_nitki){
        br_nitki = maksimalen_broj_nitki;
    }

    mem[1] = br_nitki; //zapisuva vo memorija kolku nitki ima vkupno

    for(int i=0; i<br_nitki; i++){
        scanf("%d", &broj_za_prebaruvanja[i]);
        nitki_id[i] = i;
    }

    pthread_t thread[br_nitki]; //NIZA OD NITKI
    for(int i=0; i<br_nitki; i++){
        pthread_create(&threads[i], NULL, baraj, &nitki_id[i]); //se dodeluva indeksot na sekoja nitka i taa se startuva
    }

    for(int i=0; i<br_nitki; i++){ //glavnata gi ceka site drugi
        pthread_join(threads[i], NULL); //CEKAM DA ZAVRSAM
    }

    shmdt(mem); //trgni pokazuvac kon shared memorija
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

//KONSTANTI STO NEMA DA SE SMENATI I VO DVETE PROGRAMI
#define KEY 12345
#define maksimalen_broj_nitki 10
#define niza_size 1000

int *mem;

int main(int argc, char *argv[]){

    int shared_mem_id = shmget(KEY, 1024, 0666); //PRISTAP SO KEY, KOLKAV PROSTOR IMA, DOZVOLENO ZA CITANJE/PISUVANJE
    if(shared_mem_id == -1){
        perror("Neuspesno mapiranje vo shared mem");
        exit(1);
    }

    mem = (int *) shmat(shared_mem_id,NULL,0); //MOZES DA RABOTIS SO MEMORIJATA KAKO SO NIZA
    
    srand(time(NULL)); //GENERATOR NA SLUCAEN BROJ
    printf("Generiram 1000 broevi vo shared memorijata!");
    for(int i=2; i<niza_size+2; i++){ //0-bira nitki, 1-brojot na nitki
        mem[i] = rand()%1000;
    }

    int br_nitki = mem[1] //programata 1 go postavile mem[1] vo memorija i jas mozam da si go zemam
    int izbrani_nitki[br_nitki];

    for(int i=0; i<br_nitki; i++){
        while(mem[0] != -1){
            sleep(1);
        }
        int random_indeks=rand()%br_nitki; //slucajno biram indeks

        while(izbrani_nitki[random_indeks] == 1){ //ako bila izbrana veke birame od pocetok se dodeka ne najdeme nova sto ne bila izbrana
            random_indeks=rand()%br_nitki;
        }
        izbrani_nitki[random_indeks]=1;
        mem[0] = random_indeks;

        printf("ja izbrav nitkata so id: %d", random_indeks);
    }
    shmdt(mem);
    return 0;
    
}