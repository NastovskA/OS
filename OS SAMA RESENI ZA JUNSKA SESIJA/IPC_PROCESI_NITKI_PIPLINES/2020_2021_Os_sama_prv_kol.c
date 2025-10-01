#include <stdio.h>
#include <stdlib.h>         
#include <unistd.h>     //za pid_t     
#include <sys/types.h>      
#include <time.h>           
#include <string.h>  
#include <fcntl.h>          // za open()
#include <sys/stat.h>       // za struct stat


#define golemina_niza 1000
#define broj_nitki 10
#define broj_procesi 10

int niza[golemina_niza];

void *prebaraj(void *arg){
    int indeks = *(int*)arg; //koja nitka ja zimam
    int start = indeks * 100;
    int end = (indeks+1) *100;
    int brojac=0;

    for(int i=start; i<end; i++){
        if(niza[i] == 0){
            brojac++;
        }
    }
    printf("Nitkata so id %d go pronajde brojot nula %d pati", indeks, brojac);
    return NULL;
}

int main(int argc, char *argv[]){


    for(int i=0; i<golemina_niza; i++){
        niza[i] = rand() % 1000; //tuka ke imam broj od 0 do 999
    }

    int vreme1;
    int vreme2;

    srand(time(NULL));

    pid_t dete1 = fork();

    if(dete1 < 0){
        printf("Ne raboti, ni deteto ni glavnat programa!");
    }

    clock_t start_vreme, end_vreme;

    if(dete1 == 0){
        start_vreme = clock();
        for(int i=0; i<broj_procesi; i++){
            int brojac = 0;
            pid_t proces = fork();
            if(proces == 0){
                int start = i*100; //od 0 ...................900
                int end = (i+1)*100; // do 99  ................999
                for(int j=start; j<end; j++){ //od kaj do kaj da ide niz niza
                    if(niza[j] == 0){
                        brojac++;
                    }
                }
                printf("Procesot so reden broj %d ja pronasol 0, %d pati", i+1, brojac);
                exit(0);
            }
        }
        exit(0);
    }
    
    for(int i=0; i<broj_procesi; i++){
            wait(NULL);
    }

    
    clock_t end_vreme = clock();
    vreme1 = end_vreme-start_vreme;


    pid_t dete2 = fork();

    if(dete2<0){
        printf("Nema da raboti nitu deteto nitu glavnata programa!");
        exit(1);
    }

    clock_t start_vreme = clock();
    if(dete2 == 0){

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

        exit(1);
    }

    wait(NULL);
    clock_t end_vreme = clock();
    vreme2=end_vreme-start_vreme;


    if(vreme1> vreme2){
        printf("Pobrzo e prebaruvanjeto so nitki, i toa za vreme %d", vreme2);
    }else if(vreme2 > vreme1){
        printf("Pobrzo e prebaruvanjeto so procesi i toa zavrsuva za %d", vreme1);
    }else{
        printf("Izvrsuvanjeto na dvata nacina e za isto vreme i toa iznesuva vreme1!");
    }

    return 0;
}