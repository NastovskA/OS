// Да се напише програма во C која како аргумент од командна линија добива имиња на влезни текстуални
// датотеки. Програмата треба да го најде поединечниот и апсолутниот просек на бројот на букви кои се
// појавуваат во низа од 100 знаци. Поединечниот просек се прави за секоја датотека одделно, додека пак
// апсолутниот просек се преметува како просек од поединечните просеци од секоја датотека.
// Поединечните просеци се прават така што се земаат 100 по 100 бајти од влезната датотека (додека не
// стигне до крај на датотеката) и проверува колку букви имало во секој од тие 100 бајти. На крај од
// датотеката се прави просек од пронајдените букви. Читањето да се прави преку заедничка меморија.

//SPODELENA MEMORIJA

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

#define SHM_SIZE 1024
#define BLOCK_SIZE 100

int broj_bukvi(char *niza, int golemina){
    int brojac=0;
    for(int i=0; i<golemina; i++){
        if(isalpha(niza[i])){
            brojac++;
        }
    }
    return brojac;
}

int main(int argc, char *argv[]){

    if (argc < 2) {
        printf("Nevaliden broj na argumenti\n");
        return -1;
    }

    //KREIRANJE NA SPODELENA MEMORIJA
    int shm_id = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget failed");
        return -1;
    }

    // Прикачување на споделената меморија
    float *shared_memory = (float *)shmat(shm_id, NULL, 0);
    if (shared_memory == (float *)-1) {
        perror("shmat failed");
        return -1;
    }

    for(int i=0; i<argc; i++){

        pid_t dete = fork();

        if(dete == 0){

            int fd = open(argv[i], O_RDONLY);
            if (fd == -1) {
                perror("Cannot open file");
                exit(1);
            }


             char buffer[BLOCK_SIZE];
             int vkupno_bukvi = 0;
             int broj_blokovi = 0;
             ssize_t bytes_read;


             //Citame blok po blok od 100 bajti
             while((bytes_read=read(fd, buffer, BLOCK_SIZE))> 0){
                vkupno_bukvi+=broj_bukvi(buffer, bytes_read);
                broj_blokovi++;
             }

             close(fd);

             float prosek = (float)vkupno_bukvi/broj_blokovi;
             shared_memory[i-1] = prosek;

             printf("Prosek za datoteka %s: %.2f\n", argv[i], prosek);
            exit(0);
        }
    }

    // Родител чека сите деца да завршат
    for (int i = 1; i < argc; i++) {
        wait(NULL);
    }

     // Пресметување на апсолутен просек
     float vkupen_prosek = 0;
     for (int i = 0; i < argc-1; i++) {
         vkupen_prosek += shared_memory[i];
     }
     vkupen_prosek /= (argc-1);
 
     printf("Apsoluten prosek: %.2f\n", vkupen_prosek);
 
     // Откачување и бришење на споделената меморија
     shmdt(shared_memory);
     shmctl(shm_id, IPC_RMID, NULL);
 
     return 0;

}