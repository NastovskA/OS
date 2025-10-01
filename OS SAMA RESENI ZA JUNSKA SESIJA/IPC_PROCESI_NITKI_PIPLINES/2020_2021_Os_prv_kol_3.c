#include <stdio.h>

#define golemina_na_niza 100
int niza[golemina_na_niza];
int N;
int K[500];
int indeksi_nitki[500];

void *prebaraj(void * arg){
    int indeks = *(int*)arg; 
    int vrednost_K = K[indeks]; //slucaen broj z anitkava

    for(int i=0; i < vrednost_K; i++){
        int broj_ran = rand() % 100;

        if(indeks <= N/2){
            niza[broj_ran] += 1;
        }else if(indeks > N/2){
            niza[broj_ran] -= 1;
        }
    }
return NULL;
}

int main(int argc, char *argv[]){

    printf("Vnesi vrednost za N: ");
    scanf("%d", &N);

    pid_t dete = fork();
    if(dete < 0){
        printf("Ne se aktivira nikoj od procesite!");
        exit(1);
    }
    if(dete == 0){
        for(int i=0; i<golemina_na_niza; i++){
            niza[i] = 0; 
        }

        pthread_t threads[N]; //ja cuva ID na sekoja nitka
        int indeks_nitka[N]; //preku ova ja prakam nitkata na funkcijata

        for(int i=0; i<N; i++){
                K[i] = rand() % 500 + 1; //najmnogu 500
                indeks_nitka[i] = i;
                pthread_create(&threads[i], NULL, prebaraj, &indeks_nitka[i]); //kreira nova nitka, sekoe prebaraj dobiva razlicen indeks 
                //so &indeks_nitka se praka na funkcijata prebaraj
        }

        for(int i=0; i<N; i++){
            pthread_join(threads[i], NULL); //sekoja nitka cekam da zavrsi
        }

        int promeni = 0;
        for(int i=0; i<golemina_na_niza; i++){
            if(niza[i] != 0){
                promeni++;
            }
        }
        printf("Vrednosta ja imaat promeneto %d nitki", promeni);

        exit(0);
    }

    wait(NULL);
    return 0;
}