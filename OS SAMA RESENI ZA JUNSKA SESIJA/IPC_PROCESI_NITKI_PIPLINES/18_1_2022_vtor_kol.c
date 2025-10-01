// Да се напише С програма која работи со процеси и меѓупроцесна комуникација. Главниот процес, како
// аргументи од командна линија, добива наредба со аргументи која треба да ја изврши. Потоа, преку
// тастатура се внесува колку пати треба да се изврши соодветната наредба Z. Главниот процес креира Z деца
// процеси колку што треба да се изврши наредбата, притоа секој еден од деца процесите ја извршува
// наредбата еднаш. Иако деца процесите се креираат и се извршуваат паралелно, сепак треба да се овозможи
// механизам каде деца процесите ќе се чекаат помеѓу себе т.е. ќе се извршуваат сериски. Главниот процес
// треба да брои колку време им требало на деца процесите да завршат со извршување. Комуникацијата
// помеѓу деца процесите и синхронизацијата за да се постигне сериско извршување да се изведе со помош на
// заедничка (shared) меморија.
// Пример:
// ./prva wc -c -l -d


//PROCES
#include <stdio.h>          
#include <stdlib.h>         
#include <unistd.h>         
#include <sys/types.h>      
#include <sys/ipc.h>        
#include <sys/shm.h>        
#include <pthread.h>        
#include <time.h>           
#include <string.h>  
#include <fcntl.h>          // za open()
#include <sys/stat.h>       // za struct stat

#define KEY 12345

int main(int argc, char *argv[]){
    if (argc < 2){
        printf("NEdovolen broj na argumenti!");
        exit(1);
    }

    char *naredba = argv[1];

    int broj_naredbi;
    printf("Vnesi broj za kolku pati da se izvrsi naredbata:  ");
    scanf("%d", &broj_naredbi);

    int shmid = shmget(KEY, sizeof(int), 0666 | IPC_CREAT);
    if(shmid < 0){
        perror("ERROR");
        exit(1);
    }

    int *shared_memory = (int *)shmat(shmid, NULL, 0); //shared mem attach, shmat vraka void i zatoa jas imam (int *)
    if(shared_memory == (void *) -1){
        perror("Neuspesno povrzuvanje na shared memory!");
        exit(1);
    }

    *shared_memory = 0; //PRVIOT PROCES KE BIDE NA RED
    int vreme = 0;


    for(int i=0; i<broj_naredbi; i++){
        pid_t pid = fork();

        if(pid == 0){ //DETE PROCES
            while(*shared_memory != i); //SAMO 1 PROCES KE VLEZE DRUGITE CEKAAT DODEKA NE IM DOJDE REDOT
            char *args[argc]; //vaka se zimaat argumenti od komandna linija, T.E NIZA SO GOLEMINA KOLKU STO IMA BROOJ NA ARGUMENTI
            for(int j=1; j<argc; j++){
                args[j-1] = argv[j]; //vo ARGS ke si pocnat od 0,1,2,3 da se popolnuvaat, dodeka od ARGV se zimaat od 1,2,3 bidejki prviot e ime na programa
            }
            args[argc-1] = NULL; //za da znae deka zavrsuva mora null


            // char *args[] = {"ls", "-l", "-a", NULL};
            // execvp("ls", args);
s
            execvp(args[0], args); // ako ova raboti, kodot posle nego ne se izvrsuva
            perror("Neuspesno izvrsuvanje");
            exit(1);
        }
    }

    for(int i=0; i<broj_naredbi;  i++){
        wait(NULL);
        (*shared_memory)++; //dozvola za sleden proces i taka ke se poklopat so i
        vreme++;
        sleep(1);
    }

    // time_t start_time = time(NULL);
    // time_t end_time = time(NULL);
    
    printf("Vkupno vreme za izvrsuvanje %d: ", vreme);
    shmdt(shared_memory);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}