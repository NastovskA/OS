// Да се напише програма во C која како аргумент добива име на непостоечка датотека.
// Програмата најпрво креира дете процес. Родител процесот треба да генерира 1000 случајни
// броеви со помош на функцијата rand() (од библиотеката stdlib.h) и истите ги запишува на екран
// (кои со помош на редирекција се запишуваат во датотеката) и родител процесот завршува. Дете
// процесот проверува дали може да ја отвори датотеката, и ако не може заспива 1 секунда. Потоа
// проверува пак, и заспива пак 1 секунда, се додека не успее да ја отвори датотеката. Чим успее
// да ја отвори, чита 1000 броеви од тастатура (редиректирани од датотеката) и ги сместува во
// низа. Потоа, дете процесот, со помош на 4 нитки (секоја нитка пребарува по 250 броеви), го
// наоѓа најголемиот број и го печати на екран. 


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

#define NITKI 4;
int niza_broevi[1000];
int max_broj=0;

void *prebaraj(void *arg){
    int indeks = *(int *)arg;
    int start,end;

    if(indeks == 0){
        start=0;
        end=249;
    }else if(indeks == 1){
        start=250;
        end=499;
    }else if(indeks == 2){
        start =500;
        end = 749;
    }else if(indeks == 3){
        start = 750;
        end = 999;
    }
    int lokalen_maksimum=niza_broevi[start];
    for(int i=start; i<end; i++){
        if(niza_broevi[i] > lokalen_maksimum){
            lokalen_maksimum = niza_broevi[i];
        }
    }
    if(lokalen_maksimum>max_broj){
        max_broj = lokalen_maksimum;
    }

}

int main(int argc, char *argv[]){
    if(argc < 2){ //ime na datoteka + 1 
        printf("Nevaliden broj na argumenti");
        return EXIT_FAILURE;
    }

    ime_datoteka = argv[1];


    pid_t dete = fork(); //kreiram
    if(dete < 0){
        perror("FORK FAILED");
    }

    if(dete == 0){
    int fd = open(ime_datoteka, O_RDONLY);
    while(fd == -1) {
        perror("Ne mozam da ja otvoram datotekata");
        sleep(1);
    }
    
    while (i<1000 && read(fd,&niza_broevi[i],sizeof(int))>0){
        i++;
    }

    pthread_t threads[4]; //inicijalizacija
    int nitki_indeks[4]; //argument na prebaraj
    for(int i=0; i<4; i++){
        nitki_indeks[i] = i;
        pthread_create(&threads[i], NULL, prebaraj, &nitki_indeks[i]);//kreiram nitka,    ,    ,  id na nitka
    }


    //site nitki da zavrsat pred da prodolzam ponatamu
    for(int i=0; i<4; i++){
        pthread_join(threads[i], NULL);
    }

    printf("Najgolem broj e %d", max_broj);
           
    }
    else{//vo roditel sum
        
        int fd = open(argv[1], O_RDONLY); // Отвори датотека во режим за читање
        if (fd == -1) {
            perror("Ne mozam da ja otvoram datotekata");
            return 1;
        }

        srand(time(NULL));

        for(int i=0; i<1000; i++){
            printf("%d", rand()%1000);
        }
        return 0;

    }

}