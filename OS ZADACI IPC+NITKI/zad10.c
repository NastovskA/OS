// Да се напише програма во C која како аргумент од командна линија добива имиња на влезни текстуални
// датотеки. Програмата треба да го најде поединечниот и апсолутниот просек на должина на низа
// најблиску поголема од 100 бајти во која ќе нема пресекување на низа од букви. Поединечниот просек се
// прави за секоја датотека одделно, додека пак апсолутниот просек се преметува како просек од
// поединечните просеци од секоја датотека. Поединечните просеци се прават така што се земаат 100 по
// 100 бајти од влезната датотека (додека не стигне до крај на датотеката) и проверува дали на крајот од
// 100-те бајти има букви или не. Доколку 100-те бајти завршуваат на знак различен од бува, тогаш се
// земаат 100 бајти дека се доволни за тој дел, но доколку 100-те бајти завршуваат на буква, тогаш на тие
// 100 бајти се додаваат останатите букви додека не се стигне до знак различен од буква. На крај од
// датотеката се прави просек од должините детектирани за целата датотека. Читањето да се прави преку мемориско мапирање на датотеките.


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

#define BLOCK 100

int presmetaj_dolzina(char *datoteka, int golemina){
    int i=BLOCK;
    while(i<golemina && isalpha(datoteka[i])){
        i++;
    }
    return i;
}

int main(int argc, char *argv[]){

    if (argc < 2) {
        printf("Nevaliden broj na argumenti\n");
        return -1;
    }

    for(int i=1; i<argc; i++){
        pid_t dete = fork();

        if(dete == 0){

            
            if(dete < 0){
                perror("FORK FAILED");
            }else if(dete == 0){


                int fd = open(datoteka, O_RDONLY); //ja otvaram datotekata za citanje
                if (fd == -1) {  //ne uspealo otvaranjeto
                    perror("Failed to open file");
                    return 1;
                }
            
                int golemina_na_datoteka = lseek(fd, 0, SEEK_END); //ja zimam goleminata na datotekata (kon datotekata, pomestuvanje=0, od kade da pocne pomestuvanjeto)
                if(golemina_na_datoteka == -1){
                    perror("lseek failed");
                    close(fd);
                    return -1;
                }
                lseek(fd,0,SEEK_SET);
            
                // null - adresa kade sakame da se mapira,  kolku bajti da mapirame,   zastita na memorija,  tip na mapiranje,  otvorena datoteka fd,  offset od pocetok na datoteka 0
                char *datoteka = mmap(NULL, golemina_na_datoteka, PROT_READ, MAP_PRIVATE, fd, 0); //mapiraj ja datotekata vo memorija
                if(datoteka == MAP_FAILED) { //akoo ne uspee pecati greska
                    perror("Memory mapping failed");
                    close(fd);
                    return -1;
                }
                
                int vukpna_dolzina = 0; 
                int blok_broj =0 ;
                for(int j=0; j<golemina_na_datoteka; j++){
                    int dolzina = presmetaj_dolzina(datoteka+j, golemina_na_datoteka-j);
                    vukpna_dolzina+=dolzina;
                    blok_broj++;
                }

                munmap(datoteka,golemina_na_datoteka); //ПРЕКИН СО МАПИРАЊЕ 
                close(fd); //ЗАТВАРА ДАТОТЕКА
            
            if(blok_broj>0){
                printf("Datotekata %s ima poedinecen prosek: %.2f\n", argv[i], (float)vkupno_dolzina / blok_broj);
            }

            }
        }

    }

    printf("Apsolutniot prosek e: %.2f\n", apsoluten_prosek / (argc - 1));

    return 0;
}