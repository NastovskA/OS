// Да се напише програма во C која како аргумент добива име на непостоечка датотека.
// Програмата најпрво креира дете процес. Родител процесот треба да генерира 1000 случајни
// броеви со помош на функцијата rand() (од библиотеката stdlib.h) и истите ги запишува на екран
// (кои со помош на редирекција се запишуваат во датотеката) и родител процесот завршува. Дете
// процесот проверува дали може да ја отвори датотеката, и ако не може заспива 1 секунда. Потоа
// проверува пак, и заспива пак 1 секунда, се додека не успее да ја отвори датотеката. Чим успее
// да ја отвори, чита 1000 броеви од тастатура (редиректирани од датотеката) и ги сместува во
// низа. Потоа, дете процесот, со помош на 4 нитки (секоја нитка пребарува по 250 броеви), го
// наоѓа најголемиот број и го печати на екран.

//PROGRAMA
#include <stdio.h>          
#include <stdlib.h>         
#include <unistd.h>         
#include <sys/types.h>      
#include <sys/ipc.h>        
#include <sys/shm.h>        
#include <pthread.h>        
#include <time.h>           
#include <string.h>  

#define dolzina_niza 1000
#define broj_nitki 4

int niza_so_broevi[dolzina_niza];
int niza_za_max_el_lokalno[broj_nitki];

void *prebaraj(void *arg){
    int indeks = *(int*)arg;
    int pocetok, kraj;
    if(indeks == 0){
        pocetok = 0;
        kraj = 249;
    }else if(indeks == 1){
        pocetok = 250;
        kraj = 499;
    }else if(indeks == 2){
        pocetok = 500;
        kraj = 749;
    }else if(indeks == 3){
        pocetok = 750;
        kraj = 999;
    }

    int max = niza_so_broevi[pocetok];
    for(int i=pocetok + 1; i<= kraj; i++){
        if(niza_so_broevi[i] > max){
            max  = niza_so_broevi[i];
        }
    }
    niza_za_max_el_lokalno[indeks] = max;

    return NULL;
}


int main(int argc, char *argv[]){ //ARGC broj argumenti na komandna linija

    if(argc != 2){
        printf("Nema datoteka na sama komandna linija!");
        exit(1);
    }

    char *ime_datoteka = argv[1];
    pid_t pid = fork(); //od ovoj moment imam 2 procesi koi se izvrsuvaat
    
    if(pid < 0){
        perror("ERROR");
        exit(1);
    }

    if(pid > 0){ //RODITEL PROCES

        srand(time(NULL));
        int ekran = dup(1); //privremeno

        for(int i=0; i<dolzina_niza; i++){
            int random_broj = rand() % 1000; // do kade sakam da odi 
            dup2(ekran,1);
            printf("%d", random_broj); //redirekcijata ja prai vo komandna linija,       SAMO ZA EKRANNNNNN
            
            //REDIREKCIJA BEZ GORNIOT PRINTF
            // dup2(FD, 1); //1 ekran, 0 tastatura
            // printf("....")

        }
        return 0; //tuka roditelot zavrsil

    }else if (pid == 0){//DETE PROCES
        FILE *f = NULL;

        while((f = fopen(ime_datoteka, "r")) == NULL){ //se dodeka ne moze da se otvori spij
            sleep(1);
        }

        for(int i=0; i<dolzina_niza; i++){
            if(fscanf(f,"%d",&niza_so_broevi[i]) != 1){
                fprintf(stderr, "Greska pri citanje na broj! ");
                fclose();
                return 1;
            }
        }
        fclose(f);

        pthread_t threads[broj_nitki]; //ja cuva ID na sekoja nitka
        int indeks_nitka[broj_nitki]; //preku ova ja prakam nitkata na funkcijata

        for(int i=0; i<broj_nitki; i++){
            indeks_nitka[i] = i;
            pthread_create(&threads[i], NULL, prebaraj, &indeks_nitka[i]); //kreira nova nitka, sekoe prebaraj dobiva razlicen indeks 
            //so &indeks_nitka se praka na funkcijata prebaraj
        }

        for(int i=0; i<broj_nitki; i++){
            pthread_join(threads[i], NULL); //sekoja nitka cekam da zavrsi
        }

        int globalen_maksimum = niza_za_max_el_lokalno[0];
        for(int i=0; i<broj_nitki; i++){
            id(niza_za_max_el_lokalno[i] > globalen_maksimum){
                globalen_maksimum=niza_za_max_el_lokalno[i];
            }
        }
        printf("Najgolemiot element e: %d", globalen_maksimum);
    }
}
