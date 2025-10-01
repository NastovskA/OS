// Да се напише програма во С која што ќе креира К нитки (К се внесува од тастатура), кои што ќе
// пребаруваат зборови во датотека. Главната нитка ги креира К-те нитки и притоа на секоја една од
// нитките и кажува кој збор да го пребарува. Тоа значи дека пред да се креира нитката, треба од тастатура
// да се внесе зборот и истиот да и се прати на нитката за пребарување. Секоја нитка го пребарува својот
// збор во датотеката и печати на екран колку пати го нашла зборот. Изминувањето на датотеката да се
// прави преку мемориско мапирање на истата, притоа, секоја една од нитките и кажува на главната нитка
// колку пати го има пронајдено зборот. Оваа информација, нитките ја споделуваат со главната нитка
// преку заедничка меморија. Главната нитка, откако ќе завршат сите останати нитки, на екран печати
// колку вкупно зборови биле најдени (од сите К зборови) во датотеката. 

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


//brojot na nitki se vnesuva od tastatura
typedef struct 
{
    char zbor[20]; //go cuvame zborot sto treba da go prebaram
    int brojac_za_najden_zbor; //brojac kolku pati sum go nasla
    char *datoteka; //pokazuvac kon sodrzina na datoteka
    size_t golemina_na_podatoci; //goleminata na datotekata
}info_t; //strukturata info_t 


void *prebaraj(void *t){

    info_t *info = (info_t *)t;
    char *pokazuvac = info->datoteka; //pokazuva kon samiot start na datotekata
    int broj=0; //lokalen brojac
    int dolzinaNaZbor= strlen(info->zbor); //dolzina na zborot sto se bara

    for(int i=0; i<= info-> golemina_na_podatoci-dolzinaNaZbor; i++){ //iteriram niz sodrzinata na datotekata
        if(strncmp(pokazuvac+i, info->zbor, dolzinaNaZbor) == 0){ //strncmp sporeduva 2 stringa  (od kade pocnuva, zborot sto ni treba, kolku karakteri sporeduva)
            broj++;
        }
    }
info->broj = broj; //go zacuvuvam rezultatot
pthread_exit(NULL);
}


int main(int argc, char *argv[]){

    if(argc < 2){
        printf("Nevaliden broj na argumenti");
        return EXIT_FAILURE;
    }

    char datoteka[100];
    int k;

    printf("Vnesete ime na datoteka: ");
    scanf("%s", datoteka);

    printf("Vnesete broj na nitki: ");
    scanf("%s", k);

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
    close(fd);

    pthread_t nitki[k]; //niza od nitki
    info_t t[k]; //niza od strukturi za informacii

    for(int i=0; i<k; i++){

        printf("Vnesete zbor: ");
        scanf("%s", t[i].zbor);

        t[i].datoteka=datoteka;
        t[i].golemina_na_podatoci=golemina_na_datoteka;
        t[i].brojac_za_najden_zbor=0;

        pthread_create(&nitki[i], NULL, prebaraj, (void*)&t[i]); //kreiranje na nova nitka (kade ke ja zacuvam,  null, prebaraj-funkcija za nitkata,  argument sto go prosleduvame)
        //sekoja nitka dobiva razlicen zbor na prebaruvanje preku t
    }

    int vkupno=0;

    for(int i=0; i<k; i++){

        pthread_join(nitki[i], NULL);
        printf("Zborot '%s' e najden %d pati.\n", t[i].zbor, t[i].brojac_za_najden_zbor);
        vkupno += t[i].brojac_za_najden_zbor;
    }
    printf("Vkupno zborovi najdeni: %d\n", vkupno);

    munmap(datoteka,golemina_na_datoteka); //ПРЕКИН СО МАПИРАЊЕ 
    close(fd); //ЗАТВАРА ДАТОТЕКА

    return 0;
}