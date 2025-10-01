//SO SIGNALIZACIJA 
// PROGRAMA 1
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


#define sinhronizacija_memory 1
#define KEY_memory 128
#define memory 1024
#define signal 0
char *mapirana_memory;

void otkluci(char *kluc, char *tekst, int p)
{
    int k = strlen(kluc);
    int t = strlen(tekst);
    int i, sum, z = 0;
    for (i = 0; i < t; i++)
    {
        if (p == 0)
        {
            sum = kluc[z] + tekst[i];
            if (sum > 255)
            {
                sum -= 255;
            }
        }
        else
        {
            sum = kluc[z] - tekst[i];
            if (sum < 0)
            {
                sum += 255;
            }
        }
        tekst[i] = sum;
        i++;
        z++;
        if (z == k)
            z = 0;
    }
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        printf("Nema dovolno argumenti na komandna linija!");
        exit(1);
    }

    char *ime_datoteka = argv[1];

    int fd = open(ime_datoteka, O_RDONLY);
    if (fd == -1)
    {
        perror("ERROR, ne moze da se otvori");
        exit(1);
    }

    int golemina = lseek(fd, 0, SEEK_END); // odi do kraj
    lseek(fd, 0, SEEK_SET);                // vraka na pocetok

    if (golemina == 0)
    {
        printf("Datotekata e prazna!");
        exit(1);
    }
    //1024+128+1 - realna golemina na datotekata

    mapirana_memory = mmap(NULL, golemina, PROT_READ, MAP_SHARED, fd, 0); // void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
    if (mapirana_memory == MAP_FAILED)
    {
        perror("ERROR");
        exit(1);
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        printf("Ne moze da rabotis nitu so dete nitu so roditel!");
        exit(1);
    }
    else if (pid > 0)
    { // Roditelot

        int kluc_fd = open("kluc.out", O_RDWR);
        char orginalen_kluc[KEY_memory];
        read(kluc_fd, orginalen_kluc, KEY_memory);
        close(kluc_fd);

        while(1){
            if(mapirana_memory[signal] == 1){
                char *kluc_od_dete = mapirana_memory + 1; //mora da pocne od +1 bidejki na 0 mi e staven bitot za signalizacija
                char *tekst = mapirana_memory + 1 + KEY_memory; //go zimam klucot pa posle mozam da pristapam do teksot 

                if(strcmp (orginalen_kluc, kluc_od_dete) == 0){
                    printf("Isti kluc! ");
                    otkluci(orginalen_kluc, tekst, 1); //otsifriraj
                    mapirana_memory[signal] = 2;

                    sleep(10);
                    otkluci(orginalen_kluc,tekst, 0); //sifriraj
                    mapirana_memory[signal] = 0;
                    break;
                }else{
                    printf("Gresen kluc");
                    mapirana_memory[signal] = 0;
                }
            }
            wait(NULL);
            munmap(mapirana_memory, memory);
            close(fd);
        }

    }else if(pid == 0){ //dete prices
        for(int obid=1; obid<=3; obid++){
            printf("Dete cita od tastatura, vnesi kluc: ");
            char kluc[KEY_memory];
            fgets(kluc, KEY_memory, stdin); //STDIN citaj od standarden vlez - tastatura (scanf)
            kluc[strcspn(kluc, "\n")] = 0; //na ovoj nacin ja zimam samo sodrzinata bez \n

            memcpy(mapirana_memory, kluc, strlen(kluc) + 1);
            mapirana_memory[signal] = 1; //mu signaliziram

            if(mapirana_memory[signal] == 2){
                printf("Uspesno otklucena!");
                break;
            }else{
                printf("Obidi se pak");
            }
        }
        exit(1);
    }
}
