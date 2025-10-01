// Да се напише програма во C програмскиот јазик каде ќе се овозможи внесување на низа од цели броеви
// како аргументи на програмата. Потоа, програмата треба да дозволи внесување на непознат број на цели
// броеви за пребарување во низата. За секој внесен број, програмата креира нова нитка, преку која го
// пребарува тој број во низата и печати на екран колку пати се појавува тој број и колку има броеви во
// низата кои што се делители на тој број. Печатењето на екран не го прават нитките што го прават
// пребарувањето туку тие резултатот го препраќаат до друг процес преку мемориско мапирање.
// Внесувањето на броевите за пребарување прекинува кога ќе се внесе од тастатура нешто различно од
// цел број.

#define golemina_niza 100
#define broj_nitki 1
#include <stdio.h>
int broj_argumenti;
int niza[golemina_niza];
int broj;
int *mapirana_memorija;
int golemina;

void *prebaraj(void *arg){
    int indeks = *(int*)arg;
    int brojac_pojavuvanja = 0;
    int brojac_deliteli = 0;
    for(int i=0; i<broj_argumenti; i++){
        if(broj == niza[i]){
            brojac_pojavuvanja++;
        }
        if(broj%niza[i] == 0){
            brojac_deliteli++;
        }
    }
    mapirana_memorija[0] = broj; //ovoj broj go baram
    mapirana_memorija[1] = brojac_pojavuvanja;
    mapirana_memorija[2] = brojac_deliteli;
    
    return NULL;
}

int main(int argc, char *argv[]){
    
    if(argc < 2){
        printf("Nemate dovolno argumenti na komandna linija!");
        exit(1);
    }
    broj_argumenti = argc - 1;
    
    for(int i=0; i<broj_argumenti; i++){
        niza[i] = atoi(argv[i+1]);
    }


    int fd = open("dat.txt", O_RDONLY);
    if(fd == -1){
        perror("ERROR!");
        exit(1);
    }

    golemina = lseek(fd, 0, SEEK_END); // odi do kraj 
    lseek(fd, 0, SEEK_SET);  //vrati na pocetok

    if(golemina == 0){
        printf("Datotekata e prazna!");
        exit(1);
    }

    mapirana_memorija = mmap(NULL, golemina, PROT_READ, MAP_SHARED, fd, 0);
    if(mapirana_memorija == MAP_FAILED){
        perror("ERROR");
        exit(1);
    }


    int niza_vneseni_breovi;
    while(scanf("%d", broj) == 1){
        //vo broj mi se naoga vneseniot broj 
        pthread_t threads[broj_nitki];
        int indeks_nitki[broj_nitki];

        for(int i=0; i<broj_nitki; i++){
            indeks_nitki[i] = i;
            pthread_create(&threads[i], NULL, prebaraj, &broj); //brojot sto go kreiram go prakam na nitkite kako argument
        }

        for(int i=0; i<broj_nitki; i++){
            pthread_join(threads[i], NULL);
        }

        printf("Brojot %d e pronaden %d pati i ima vkupno deliteli %d", mapirana_memorija[0], mapirana_memorija[1], mapirana_memorija[2]);

    }
    munmap(mapirana_memorija);
    close(fd);
    return 0;
}
