#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define max_linii_vo_datoteka 1000
#define max_linii_procitani 10
#define max_nitki 100

char *ime_datoteka;
int broj_mali_bukvi[max_nitki]; //kreiram niza globalna
int broj_golemi_bukvi[max_nitki]; //kreiram niza globalna za sekoja nitka
int redovi =0;
int broj_nitki =0;


void *prebaraj(void *arg){

    int indeks = *(int*)arg; //od listata nitki jas rabotam so 1 nitka
    int start_linija = indeks*max_linii_procitani;
    int end_linija = indeks*max_linii_procitani + max_linii_procitani;

   if(end_linija > redovi){
    end_linija = redovi;
   }

   FILE *file_open = fopen(ime_datoteka, "r"); //MORAM PAK DA JA OTVORAM BIDEJKI NE DOZVOLUVA SITE NITKI DA CITAAT ISTOVREMENO
    if(file_open == NULL){
        perror("Datotekata ne moze da se otvori!");
        exit(1);
    }

    char linija[1024];
    int momentalno = 0;
    int br_mal = 0;
    int br_golemi =0;


    //so ova go postavuvam na pocetna pozicija vo samata datoteka
    while(momentalno < start_linija && fgets(linija, sizeof(linija), file_open)){
        momentalno++;
    }

    while(momentalno < end_linija && fgets(linija, sizeof(linija), file_open)){
        momentalno++;
        for(int i=0; linija[i] != '\0'; i++){

            if(isupper((unsigned char)linija[i])){
                br_golemi++;
            }else if(islower((unsigned char)linija[i])){
                br_mal++;
            }
        }
    }
    fclose(file_open);
    printf("Golemi bukvi za nitkata so indeks %d ima %d!", indeks,br_golemi);
    printf("Mali bukvi za nitkata so indeks %d ima %d!", indeks,br_mal);

    broj_golemi_bukvi[indeks] = br_golemi;
    broj_mali_bukvi[indeks] = br_mal;

    return NULL;
}

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Nema dovolno argumeni od komandna linija!");
        exit(1);
    }

    ime_datoteka = argv[1];

    FILE *file_open = fopen(ime_datoteka, "r");
    if(file_open == NULL){
        perror("Datotekata ne moze da se otvori!");
        exit(1);
    }

    //SO OVA JA DOBIVAM GOLEMINATA VO BAJTI
    fseek(file_open, 0, SEEK_END); // odi do kraj i vraka 0 ako e uspesno
    long golemina = ftell(file_open); //vraka pozicija
    fseek(file_open, 0 SEEK_SET); //vraka na pocetok

    if (golemina == 0)
    {   printf("Datotekata e prazna!");
        exit(1);
    }

    //DA PROCITAM BROJ NA REDOVI OD DATOTEKA
    redovi = 0;
    char linija[1024];

    while(fgets(linija, sizeof(linija), file_open) != NULL){
        redovi++;
    }

    int ostatok = redovi % 10;
    if( ostatok != 0){
        //kreirame redovi / 10 + 1(tuka ke bide ostatokot od liniite)
        broj_nitki = redovi / 10 + 1;
    }else{
        //tocno ke se podelat site redovi/10 i ke si imaat site po 10 linii
        broj_nitki = redovi / 10;
    }


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

    int vkupno_golemi =0;
    int vkupno_mali = 0;

    for(int i=0; i<broj_nitki; i++){
        vkupno_golemi += broj_golemi_bukvi[i];
        vkupno_mali += broj_mali_bukvi[i];
    }

    printf("Vkupno golemi bukvi ima %d!", vkupno_golemi);
    printf("Vkupno mali bukvi ima %d!", vkupno_mali);



    return 0;
}