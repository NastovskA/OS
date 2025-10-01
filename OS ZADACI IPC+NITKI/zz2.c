// Да се напише програма во C програмскиот јазик каде ќе се овозможи внесување на низа од цели броеви
// како аргументи на програмата. Потоа, програмата треба да дозволи внесување на непознат број на цели
// броеви за пребарување во низата. За секој внесен број, програмата креира нова нитка, преку која го
// пребарува тој број во низата и печати на екран колку пати се појавува тој број и колку има броеви во
// низата кои што се делители на тој број. Печатењето на екран не го прават нитките што го прават
// пребарувањето туку тие резултатот го препраќаат до друг процес преку мемориско мапирање.
// Внесувањето на броевите за пребарување прекинува кога ќе се внесе од тастатура нешто различно од
// цел број. 

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

int niza_broevi[100];
int *mapped_data; //ja zimam nizata od broevi
int dolzina_na_niza;

void *prebaraj(void *arg){


    int broj_za_prebaruvanje = *(int *)arg;
    int brojac_za_pojavuvanje=0;
    int brojac_za_deliteli=0;

    for(int j=0; j<dolzina_na_niza; j++){
        if(mapped_data[j] == broj_za_prebaruvanje){
            brojac_za_pojavuvanje++;
        } 
        if(broj_za_prebaruvanje % mapped_data[j] == 0){
            brojac_za_deliteli++:
        }
    }

   mapped_data[0]=broj_za_prebaruvanje;
   mapped_data[1] = brojac_za_pojavuvanje;
   mapped_data[2] = brojac_za_deliteli;

}


int main(int argc, char *argv[]){ //broj na argumenti preku tastatura,   nizata na komandna linija

    if(argc < 2){ //niza + 1 
        printf("Nevaliden broj na argumenti");
        return EXIT_FAILURE;
    }

    dolzina_na_niza = argc - 1;

    printf("Elementi od nizata: ");
    for(int i=1; i<argc; i++){
        mapped_data[i-1] = (atoi[i]);
    }

    int fd=open("dat.txt",O_RDWR | O_CREAT |O_TRUNCATE,0x666); //kreirame
        // datoteka koja kje ni bide dostapna preku mmap
        if(fd==-1){
         printf("Ne uspea fd");
         return EXIT_FAILURE;
        }
    
    mapped_data=mmap(NULL,4000,PROT_READ,|PROT_WRITE,MAP_SHARED,fd,0);
     if(mapped_data==MAP_FAILED){
     printf("Ne uspea mmap");
     return -1;
    }

while(true){

    int broj;
    printf("Vnesi element sto sakas za prebaruvanje vo nizata: ");
    scanf("%d", broj);

    pthread_t threads;
    pthread_create(&threads, NULL, prebaraj, &broj);//mesto indeks, brojot go vnesuvam
    pthread_join(threads, NULL);

    printf("Brojot %d e najden %d pati i ima %d deliteli", mapped_data[0], mapped_data[1], mapped_data[2]);

    munmap(mapped_data);
    close(fd);

    return 0;
  
}
}