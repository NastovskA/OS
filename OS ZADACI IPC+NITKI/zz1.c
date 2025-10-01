// Да се напише програма во C програмскиот јазик каде ќе се овозможи внесување на
// име на датотека како
// аргумент на програмата. Потоа, програмата треба да изврши мемориско мапирање на
// датотеката во
// адресниот простор. Датотеката е составена од N цели броеви. Главниот процес, 
//треба да ја измине
// датотеката и да провери колку цели броеви има во (мемориски мапираната) датотека.
// Потоа, треба да
// креира 10 нитки, каде што секоја една од нитките ќе генерира случаен број 
//од -10 до 2500, и ќе провери
// дали го има во датотеката. Доколку го има, бројот што е случајно генериран 
//го сместува во низа, приота
// доколку некоја нитка повеќе пати го генерира истиот број, тогаш треба да се 
//знае колку пати е секој
// еден број пронајден (генериран). Секоја нитка генерира 20 случајни броеви и 
//потоа завршува. Откако ќе
// завршат сите нитки, главниот процес ги пренесува најдените броеви на дете 
//процес, кое што тој го
// креира. Дете процесот е задолжено да отпечати на екран кои броеви биле 
//пронајдени и колку пати во
// влезната датотека. 


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


#define Nitki 10
#define MIN_GRANICA -10
#define MAX_GRANICA 2500
#define BROEVI_GENERIRANI 20

int *mapped_data;

int niza_za_broevi[200];
int niza_Za_kolku_pati[200];
int N;
int brojac[200];

void *prebaraj(void *arg){//id na nitkata mi e arg

    int pomosna_niza[20];

    int id = *(int *)arg;

        for(int j = id*BROEVI_GENERIRANI; j < id*BROEVI_GENERIRANI + BROEVI_GENERIRANI; j++){

            int random_broj = (rand() % (MAX_GRANICA - MIN_GRANICA + 1)) + MIN_GRANICA;  //0-2510 + MIN_GRANICA
            
            for(int k =0; k<N; k++){
                if(mapped_data[k] == random_broj){
                    pomosna_niza[j] = random_broj;
                    brojac[random_broj]++;
                    break;
                }
            }
        }
        for(int j=0; j<200; j++){
            if(brojac[j] > 0){
                int najdeno=0;
                for(int k=0; k<200; k++){
                    if(niza_za_broevi[k] == pomosna_niza[j]){
                        niza_Za_kolku_pati[k] += brojac[pomosna_niza[j]];
                        najdeno=1;
                        break;
                    }
                }
                if(!najdeno){
                   
                        if(niza_za_broevi[k] == 0){
                            niza_za_broevi[k] = pomosna_niza[j];
                            niza_Za_kolku_pati[k] = brojac[pomosna_niza[j]];
                            break;
                        }
                    
                }
            }
        }
}


int main(int argc, char *argv[]){

    if(argc < 2){ //ime na datoteka + 1 
        printf("Nevaliden broj na argumenti");
        return EXIT_FAILURE;
    }

    printf("Vnesi broj na elementi: ");
    scanf("%d", &N);

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


    mapped_data = mmap(NULL, golemina_na_datoteka, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped_data == MAP_FAILED) {
        perror("Ne mozam da ja mapiram datotekata");
        close(fd);
        return 1;
    }

    pthread_t threads[10]; //inicijalizacija
    int arg[Nitki]; //argument na prebaraj
    for(int i=0; i<Nitki; i++){
        pthread_create(&threads[i], NULL, prebaraj, (void *)&arg[i]);//kreiram nitka,    ,    ,  id na nitka
    }


    //site nitki da zavrsat pred da prodolzam ponatamu
    for(int i=0; i<Nitki; i++){
        pthread_join(threads[i], NULL);
    }


    pid_t dete = fork(); //kreiram

    if(dete < 0){
        perror("FORK FAILED");
    }

    if(dete == 0){

        printf("Najdeni broevi i niven broj na pojavuvanja: ");
        for(int i=0; i<200; i++){
          printf("Brojot %d e pronajjden %d pati!", niza_za_broevi[i], niza_Za_kolku_pati[i]);
        }
           
    }
    else{
        wait(NULL);
    }

    munmap(mapped_data, golemina_na_datoteka);
    close(fd);

    return 0;

}