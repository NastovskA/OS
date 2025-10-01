// PROGRAMA
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

#define broj_procesi 5
#define max_broj_znaci 1000
#define KEY 12345

char *mmap_memory; //pokazuvacot vo mapmemorijata

int main(int argc, char *argv[]){
     if(argc != 2){
        printf("Nema dovolno argumenti na komandna linija!");
        exit(1);
    }

    char *datoteka = argv[1];

    int fd = open(datoteka, O_RDONLY);
    if(fd == -1){
        perror("ERROR");
        exit(1);
    } 

    int golemina = lseek(fd, 0, SEEK_END); //odi do kraj
    lseek(fd, 0, SEEK_SET); //vraka na pocetok

    if (golemina == 0){
        printf("Datotekata e prazna!");
        exit(1);
    }

    char *mapirani_podatoci = mmap(NULL, golemina, PROT_READ, MAP_SHARED,fd, 0); //void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
    if(mapirani_podatoci == MAP_FAILED){
        perror("ERROR");
        exit(1);
    }
    close(fd);


    //KREIRANJE NA SHARE MEMORIJA ZA DA CUVA VO NEA FLAG, INDEKS, KRAJ
    int shmid = shmget(KEY, sizeof(int) * 3, 0666 | IPC_CREAT); //PAZI SIZEOFF(CHAR)!!!!!!
    if(shmid < 0){
        perror("ERROR");
        exit(1);
    }

    int *shared_memory = (int *)shmat(shmid, NULL, 0); //shared mem attach, shmat vraka void i zatoa jas imam (int *)
    if(shared_memory == (void *) -1){
        perror("Neuspesno povrzuvanje na shared memory!");
        exit(1);
    }

    int *flag = &shared_memory[0]; //proces dali e vo kriticna sostojba, 0-sloboden pristap, 1-ima proces
    int *indeks = &shared_memory[1]; //deka citame od pocetok, pa ponatamu se menuva
    int *kraj = &shared_memory[2]; //koga kraj = 1 togash e signalizacija za kraj

    *flag = *indeks = *kraj =0;

    for(int i=0; i<broj_procesi; i++){
        pid_t pid = fork();
        if(pid == 0){
            srand(time(NULL)); //sekogash koga imam random nesto koristi go ova

            while(1){
                if(*kraj){
                    exit(1);
                }
                if(*flag == 0){
                    *flag=1;

                    int ostatok = golemina - *indeks;
                    if(ostatok <= 0){
                        *kraj = 1;
                        *flag = 0;
                        exit(1);
                    }

                    int x = rand()%90 + 10;
                    if (x > ostatok) {
                        x = ostatok;
                    }

                    for(int j=0; j<x; j++){
                        putchar(mapirani_podatoci[*indeks+j]); //citam
                    }
                    putchar('\n');
                    *indeks += x;
                    *flag = 0;

                    sleep(2);
                }

            }
        }
    }

    for(int i=0; i<broj_procesi; i++){
        wait(NULL);
    }

    munmap(mapirani_podatoci, golemina);
    shmdt(shared_memory);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;

}


// mmap() е системска функција која ти овозможува директно да ја мапираш содржината на датотека во меморија, така што можеш да пристапуваш до неа како до обична низа (char *).

//KOGA KORISTAM * NE SE MENUVA ADRESATA, TUKU SAMO VREDNOSTA NA TAA ADRESA