// Да се напишат две програми во C кои ќе работат со процеси, нитки и споделена (shared)
// меморија. Двете програми комуницираат помеѓу себе со споделена (заедничка) меморија со
// клуч 12345. Втората програма, внесува од тастатура низа од 1000 броеви и истите ги запишува
// во заедничката меморија, а потоа, со помош на функцијата rand() (од библиотеката stdlib.h)
// случајно избира една од нитките на првата програма да го пребара својот број. Една нитка не
// може да се избере повеќе пати, така што треба да се грижи втората програма на следно бирање
// да избере некоја од преостанатите нитки.
// 120 мин. Првата програма, дозволува да се внесат Х броеви од тастатура (Х исто така се внесува од
// тастатура и може да е со вредност најмногу 10). Потоа, програмата креира Х нитки, така што
// секоја нитка ќе го пребарува соодветниот број внесен од тастатура. Секоја нитка, чека да и
// дојде редот (да биде избрана од втората програма) за да го пребара бројот во низата од 1000
// броеви. На секоја секунда проверува дали она е на ред да пребарува, и притоа доколку е
// избрана, пребарува низ низата од 1000 броеви и печати на екран колку пати е пронајден
// соодветниот број. Откако ќе заврши, сигнализира на втората програма дека може да избере
// следна нитка за извршување.


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
//PROGRAMA 1
#define MAX_VREDNOST 10
#define SHM_KEY 1234
#define GOLEMINA_NIZA 1000

int X;
int *mapped_data; //pokazuvac kon memorijata
int broj_za_prebaruvanje[MAX_VREDNOST];
int nitki_id[MAX_VREDNOST];

int *prebaraj(void *arg){

    int indeks = *(int *)arg;   //go zimam indeksot na nitkata
    int broj = broj_za_prebaruvanje[indeks]; //zemi go brojot

    while(1){ //celo vreme raboti mesto true -> 1
        sleep(1); //ceka sekunda pred da vidi dali e izbrana
        if(mapped_data[0] != indeks){ //ako ovaa nitka ne e izbrana prodolzi so cekanje
            continue;
        }else if(mapped_data[0] == indeks){//izbrana e nitkata so nekoj indeks
            int brojac=0;

            for(int i=2; i<GOLEMINA_NIZA; i++){  //mapped_data[0] = id na izbrana nitka, mapped_data[1] go cuva brojot na nitki, mapped_data[2] do mapped_data[1000] site drugi broevi
                if(mapped_data[i]==broj){
                    brojac++;
                }
            }
            printf("Nitkata %d, go najde brojot %d, %d pati", mapped_data[0], broj, brojac);
            mapped_data[0] = -1; //deka moze da se bira druga datoteka
        }
    }
}

int main(int argc, char *argv[]){

   
    int shmid=shmget(1234,1001,IPC_CREAT | 0x666);
    mapped_data=(int *) shmat(shmid,NULL,0);
    mapped_data[0]=-1; //niedna nitka ne e zemena

    printf("Vnesi vrednost za X - vrednosti: ");
    scanf("%d", &X);

    if(X>MAX_VREDNOST){
        X=MAX_VREDNOST;
    }
    mapped_data[1]=X;

    pthread_t threads[MAX_VREDNOST];
    int nitka_id[MAX_VREDNOST];

    for(int i=0; i<X; i++){
        nitka_id[i] = i;
        pthread_create(&threads[i], NULL, prebaraj, &nitka_id[i]); 
    }

    for (int i = 0; i < X; i++) {
        pthread_join(threads[i], NULL); // Чекај нитките
    }

    shmdt(mapped_data);
    shmet(shmid,IPC_RMID,0);
    return 0;
}



//PROGRAMA 2
//generira slucajni broevi 1000 i gi zapisuva vo spodelenata memorija
//pritoa izbira slucajna nitka za izvrsuvanje
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

#define MAX_VREDNOST 10
#define SHM_KEY 1234
#define GOLEMINA_NIZA 1000

int *mapped_data;

int main(int argc, char *argv[]){

    int shmid=shmget(1234,1001,IPC_CREAT|0x666);
    if(shmid==-1){
        printf("Nepravilno shmget");
        return 0;
    }
    mapped_data=(int *) shmat(shmid,NULL,0);
    srand(time(NULL));

    printf("Generiram 1000 broevi vo shared mem");
    for(int i=2; i<GOLEMINA_NIZA; i++){
        mapped_data[i]=rand()%1000;
    }

    int broj_nitki = mapped_data[1];
    int izbrani_nitki[broj_nitki];


    for(int i=2; i<broj_nitki; i++){
        while (mapped_data[0] != -1) //ceka da zavrsi
        {
            sleep(1);
        }
        int random_indeks=rand()%broj_nitki;

        while(izbrani_nitki[random_indeks] == 1){ //prethodno bila, zemi nekoja druga sega
            random_indeks=rand()%broj_nitki;
        }

        izbrani_nitki[random_indeks] = 1;
        mapped_data[0]=random_indeks;

        printf("Ja izbrav nitkata so id: %d", random_indeks);
        
    }

    shmdt(mapped_data);
    return 0;

}