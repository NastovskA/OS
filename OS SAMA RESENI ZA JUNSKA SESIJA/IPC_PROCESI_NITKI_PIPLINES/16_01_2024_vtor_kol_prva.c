//PROGRAMA 1
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
char *shared_memory; //bidejki zapisuvam tekst a ne brojki

int main(int argc, char *argv[]){
    if(argc != 3){
        printf("Nema dovolno argumenti na komandna linija!");
        exit(1);
    }

    char *ime_datoteka = argv[1];
    int kolku_pati = atoi(argv[2]); //bidejki ne e adresa tuku si e obicna vrednost ne mi treba int *kolku_pati

    FILE *file_open = fopen(ime_datoteka, "r"); //se otvara datoteka koja e pratena kako argument

    if( file_open == NULL){
        perror("Ne moze da se otvori datotekata!");
        return 1;
    }

    fseek(file_open, 0, SEEK_END); //ODI DO KRAJ NA DATOTEKATA 
    int golemina = ftell(file_open); //VAKA SE VRAKA VKUPNA GOLEMINA NA DATOTEKA
    rewind(file_open); //pointer na pocetok od datoteka

    if(golemina == -1){
        perror("ERROR");
        close(file_open);
        exit(1);
    }

    int shmid = shmget(KEY, golemina+100, 0666 | IPC_CREAT); //ovie 100 gi dodavam za prvata linija sto treeba da go zapisam brojot na izvrsuvanja
    if(shmid < 0){
        perror("Neuspesno mapiranje!");
        exit(1);   
    }

    shared_memory = (char *)shmat(shmid, NULL, 0); //shared mem attach, shmat vraka void i zatoa jas imam (int *)
    if(shared_memory == (char *) -1){
        perror("Neuspesno povrzuvanje na shared memory!");
        exit(1);
    }

    //ZAPISUVA PRVO KOLKU_PATI
    sprintf(shared_memory,"//%d\n", kolku_pati); //bidejki kolku_pati e int vrednost zatoa koristime sprINTf
    //funkcijata sprintf() raboti kako printf()
    int offset= strlen(shared_memory);
    
    char red[1024];
    while(fgets(red, sizeof(red), file_open)){ //se cita linija i se smestuva vo red
        strcpy(shared_memory+offset, red);
        offset += strlen(red);
    }
    

    fclose(file_open);
    stmdt(shared_memory);
    return 1;
}


// PROGRAMA 2
#include <stdio.h>          
#include <stdlib.h>         
#include <unistd.h>         
#include <sys/types.h>      
#include <sys/ipc.h>        
#include <sys/shm.h>        
#include <pthread.h>        
#include <time.h>           
#include <string.h>  

#define KEY 12345
#define File_C "datoteka.c"
#define Pom_za_izvrsuvanje "./datoteka"
char *shared_memory; //bidejki zimam tekst a ne broevi


int main(int argc, char *argv[]){

    int shmid = shmget(KEY, 0, 0666 | IPC_CREAT); //ovie 100 gi dodavam za prvata linija sto treeba da go zapisam brojot na izvrsuvanja
    if(shmid < 0){
        perror("Neuspesno mapiranje!");
        exit(1);   
    }

    shared_memory = (char *)shmat(shmid, NULL, 0); //shared mem attach, shmat vraka void i zatoa jas imam (int *)
    if(shared_memory == (char *) -1){
        perror("Neuspesno povrzuvanje na shared memory!");
        exit(1);
    }   

    int broj_izvrsuvanja;
    sscanf(shared_memory, "//%d\n", &broj_izvrsuvanja);

    char *kod_start = strchr(shared_memory, '\n');
    if(!kod_start){
        fprintf(stderr, "Kodot ne e validen!");
        exit(1);
    }

    FILE *file_za_zapisuvanje = fopen(File_C, "w");
    if(!file_za_zapisuvanje){
        perror("Ne moze da se otvori!");
        exit(1);
    }

    fputs(kod_start+1, file_za_zapisuvanje); // +1 za da otide vo nov red
    fclose(file_za_zapisuvanje);

    char komanda[1024]; //ova mi e linijata od komandna linija  gcc datoteka.c -o datoteka

    sprintf(komanda, "gcc %s -o %s", File_C, Pom_za_izvrsuvanje); //sprintf POLNI promenliva komandna
    if(system(komanda) != 0){
        fprintf(stderr, "ERROR");
        exit(1);
    }


    for (int i=0; i<broj_izvrsuvanja; i++){
        pid_t pid = fork();
        if(pid == 0){
            execl(Pom_za_izvrsuvanje, Pom_za_izvrsuvanje, NULL); //EXECL jja izvrsuva programata, execl(path_do_programa, ime_vo_argv0, NULL);
            perror("ERROR");
            exit(1);
        }
    }

    for(int i=0; i<broj_izvrsuvanja; i++){ //gi cekam site deca
        wait(NULL);
    }
    return 0;
}