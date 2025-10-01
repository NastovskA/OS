//PROGRAMA 1
#include <stdio.h>
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


#define KEY 1234
#define memorija 1024
char *shared_memory;

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("Nema dovolno argumenti na komandna linija!");
        exit(1);
    }

    char *naredba = argv[1];

    //SHARED MEMORIJA
    int shmid = shmget(KEY, memorija, 0666 | IPC_CREAT);
    
    if(shmid >= 0){
        int *shared_memory = (int *)shmat(shmid, NULL, 0); //shared mem attach, shmat vraka void i zatoa jas imam (int *)
        if(shared_memory == (void *) -1){
            perror("Neuspesno povrzuvanje na shared memory!");
            exit(1);
        }
        strcpy(shared_memory, naredba);
        printf("Naredbata e zapisana vo shared memory!");
        shmdt(shared_memory);
    }else{
        //MAPIRANA MEMORIJA
        int fd = open("dat.txt", O_RDWR | PROT_WRITE); //deka zapisuvam mora vaka
        if(fd == -1){
            perror("Ne moze da se otvori datotekata!");
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
            perror("Neuspesno mapiranje na datotekata");
            exit(1);
        }

        if(mapirani_podatoci == NULL){
            printf("Greska: Ne mozat da se prikacat vo adresen prostor!");
            exit(1);
        }

        strcpy(mapirani_podatoci, naredba);
        printf("Naredbata e zapisana vo mmap memorija!");
        munmap(mapirani_podatoci, memorija);
        close(fd);

        return 0;
    }    

}


//PROGRAMA 2
#include <stdio.h>
#define KEY 1234
#define memorija 1024
#define max_procesi 200
#define max_pati 6
char *shared_memory;
char *mmap_memory;

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("Nema dovolno argumenti na komandna linija!");
        exit(1);
    }


    char *naredba = argv[1];
    int znamence = 0;
    

    //SHARED MEMORIJA
    int shmid = shmget(KEY, memorija, 0666 | IPC_CREAT);
    
    if(shmid >= 0){
        shared_memory = (char *)shmat(shmid, NULL, 0); //shared mem attach, shmat vraka void i zatoa jas imam (int *)
        if(shared_memory == (void *) -1){
            perror("Neuspesno povrzuvanje na shared memory!");
            exit(1);
        }
    }else{
        //MAPIRANA MEMORIJA
        int fd = open("dat.txt", O_RDONLY);
        if(fd == -1){
            perror("Ne moze da se otvori datotekata!");
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
            perror("Neuspesno mapiranje na datotekata");
            exit(1);
        }

        if(mapirani_podatoci == NULL){
            printf("Greska: Ne mozat da se prikacat vo adresen prostor!");
            exit(1);
        }

        for(int p=0; p<max_procesi; p++){
            pid_t pid = fork();

            if(pid == 0){
                for(int  i=0; i<max_pati; i++){
                    if(shared_memory && strlen(shared_memory) > 0){
                        strcpy(naredba, shared_memory);
                        znamence = 1;
                        break;
                    }else if(mapirani_podatoci && strlen(mapirani_podatoci) > 0){
                        strcpy(naredba, mapirani_podatoci);
                        znamence=1;
                        break;
                    }
                    sleep(10);
                }

                if(znamence != 1){
                    printf("Ne e zapisano uste!");
                    if(shared_memory){
                        shmdt(shared_memory);
                    }
                    if(mapirani_podatoci){
                        munmap(mapirani_podatoci, memorija);
                    }
                    if(fd >= 0 ){
                        close(fd);
                    }
                    exit(1);
                }
                printf("Procesot ja izvrsuva naredbata!!!!!");

                char *args[10];
                execvp(args[0], naredba);
                perror("Neuspesno izvrsuvanje");

                if(shared_memory){
                        shmdt(shared_memory);
                    }
                    if(mapirani_podatoci){
                        munmap(mapirani_podatoci, memorija);
                    }
                    if(fd >= 0 ){
                        close(fd);
                    }
            }
        }
        return 0;
    }    
    
}