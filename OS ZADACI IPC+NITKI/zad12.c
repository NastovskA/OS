// Да се напише С програма која работи со процеси и меѓупроцесна комуникација. Главниот процес, како
// аргументи од командна линија, добива наредба со аргументи која треба да ја изврши. Потоа, преку
// тастатура се внесува колку пати треба да се изврши соодветната наредба Z. Главниот процес креира Z деца
// процеси колку што треба да се изврши наредбата, притоа секој еден од деца процесите ја извршува
// наредбата еднаш. Иако деца процесите се креираат и се извршуваат паралелно, сепак треба да се овозможи
// механизам каде деца процесите ќе се чекаат помеѓу себе т.е. ќе се извршуваат сериски. Главниот процес
// треба да брои колку време им требало на деца процесите да завршат со извршување. Комуникацијата
// помеѓу деца процесите и синхронизацијата за да се постигне сериско извршување да се изведе со помош на
// заедничка (shared) меморија. 

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


int main(int argc, char *argv[]){

    if (argc < 2) {
        printf("Nevaliden broj na argumenti\n");
        return -1;
    }

    int kolku_pati;
    printf("Vnesi kolku pati da se izvrsi naredbata!");
    scanf("%d", &kolku_pati);


    //KREIRANJE NA SPODELENA MEMORIJA
    int shm_id = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget failed");
        return -1;
    }




    pid_t dete[kolku_pati];

    for(int i=0; i<kolku_pati; i++){

        dete[i]=fork();

        if(dete[i] < 0){
            perror("FORK FAILED");
        }

     if(dete[i] == 0){

            int fd = open(argv[i], O_RDONLY);
            if (fd == -1) {
                perror("Cannot open file");
                exit(1);
            }


             // Добиваме големина на датотека
             struct stat st;
             if (stat(argv[i], &st) == -1) {
                 perror("stat failed");
                 close(fd);
                 exit(1);
             }


             // Прикачување на споделената меморија
    float *shared_memory = (float *)shmat(shm_id, NULL, 0);
    if (shared_memory == (float *)-1) {
        perror("shmat failed");
        return -1;
    }

    while(*shared_memory != i){
        sleep(1);
    }

        }
    }
}
