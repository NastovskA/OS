#include <stdio.h>
#define KEY 1234

//PROGRAMA 1
int main(int argc, int *argv[]){
    if(argc < 2){
        printf("Nema dovolno argumenti");
    }

    char *mapirana_memory;
    char *shared_memory;


    //KREIRANJE NA SHARE MEMORIJA ZA DA CUVA VO NEA FLAG, INDEKS, KRAJ
    int shmid = shmget(KEY, 1024, 0666 | IPC_CREAT);
    if(shmid < 0){
        perror("ERROR, ne moze da se kreira shared mem");
        exit(1);
    }

    shared_memory = (char *)shmat(shmid, NULL, 0); //shared mem attach, shmat vraka void i zatoa jas imam (int *)
    if(shared_memory == (char *) -1){
        perror("Neuspesno povrzuvanje na shared memory!");
        exit(1);
    }

    int fd = open("dat.txt", O_RDONLY);
    if(fd == -1){  
        perror("ERROR, ne moze da se otvori");
        exit(1);
    } 

    int golemina = lseek(fd, 0, SEEK_END); //odi do kraj
    lseek(fd, 0, SEEK_SET); //vraka na pocetok

    if (golemina == 0){
        printf("Datotekata e prazna!");
        exit(1);
    }

    mapirana_memory = mmap(NULL, golemina, PROT_READ, MAP_SHARED,fd, 0); //void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
    if(mapirana_memory == MAP_FAILED){
        perror("ERROR");
        exit(1);
    }
    close(fd);

    char ime_naredba[30];
    int argument = 0; //0-nema 1-ima
    int br_instanci;
    int indeks =0;

    while(1){
      if(  sscanf(&mapirana_memory[indeks], "%s %d %d", ime_naredba, argument, br_instanci) != 3 ){
        break;
      }
      sprintf(shared_memory, "%s %d %d", ime_naredba, argument, br_instanci);
      
      getchar();


      while(mapirana_memory[indeks] != '\n' && mapirana_memory[indeks] != '\0'){
        indeks++;
      }
      

      munmap(mapirana_memory, 1024);
      close(fd);
      shmdt(shared_memory);
      return 0;
    }


    
}


//PROGRAMA 2
int main(int argc, int *argv[]){
    if(argc < 2){
        printf("Nema dovolno argumenti");
    }

    char *mapirana_memory;
    char *shared_memory;


    //KREIRANJE NA SHARE MEMORIJA ZA DA CUVA VO NEA FLAG, INDEKS, KRAJ
    int shmid = shmget(KEY, 1024, 0666 | IPC_CREAT);
    if(shmid < 0){
        perror("ERROR, ne moze da se kreira shared mem");
        exit(1);
    }

    shared_memory = (char *)shmat(shmid, NULL, 0); //shared mem attach, shmat vraka void i zatoa jas imam (int *)
    if(shared_memory == (char *) -1){
        perror("Neuspesno povrzuvanje na shared memory!");
        exit(1);
    }

    int fd = open("dat.txt", O_RDONLY);
    if(fd == -1){
        perror("ERROR, ne moze da se otvori");
        exit(1);
    } 

    int golemina = lseek(fd, 0, SEEK_END); //odi do kraj
    lseek(fd, 0, SEEK_SET); //vraka na pocetok

    if (golemina == 0){
        printf("Datotekata e prazna!");
        exit(1);
    }

    mapirana_memory = mmap(NULL, golemina, PROT_READ, MAP_SHARED,fd, 0); //void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
    if(mapirana_memory == MAP_FAILED){
        perror("ERROR");
        exit(1);
    }


    char naredba[30];
    int argument;
    int instanci;

    sscanf(shared_memory, "%s %d %d", naredba, &argument, &instanci);

    for(int i=0; i<instanci;  i++){
        pid_t pid = fork();
        if(pid==0){

        }
    }
    for(int i = 0; i < instanci; i++) {
        wait(NULL);
    }

}