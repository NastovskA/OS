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


#define MAX 100
#define MAX_WORD_DOLZINA 50

char *mapped_data;
int Broj_zborovi;
int rezultantna_niza[MAX];
char zborovi_koi_ke_gi_prebaruvame[MAX][MAX_WORD_DOLZINA];

void *prebaraj(void *arg){

    int indeks = *(int *)arg;
    char *zbor = zborovi_koi_ke_gi_prebaruvame[indeks];
    int brojac=0;

    char *pocetokot = mapped_data;
    char *sleden;

    while(sleden = strchr(pocetokot, ' ') != NULL){ //go deli tekstot na prazni mesta
        *sleden='\0'; //prazniot prostor so null za da raboti strchr
        if(strcmp(pocetokot,zbor) == 0){ //gi sporeduvame pocetniot so zborot
            brojac++;
        }
        pocetokot=sleden+1;
    }
    if(strcmp(pocetokot, zbor) == 0){ //za posledniot zbor
        brojac++
    }
    rezultantna_niza[indeks] = brojac;
}


int main(int argc, char *argv[])
{

    if(argc < 2){ //ime na datoteka + 1 
        printf("Nevaliden broj na argumenti");
        return EXIT_FAILURE;
    }

    printf("Vnesi broj na zborovi sto sakas da gi prebaras: ");
    scanf("%d", &Broj_zborovi); // Внеси број на зборови


    
    int fd = open(argv[1], O_RDONLY); // Отвори датотека во режим за читање
    if (fd == -1) {
        perror("Ne mozam da ja otvoram datotekata");
        return 1;
    }

    int golemina_na_datoteka = lseek(fd, 0, SEEK_END); //ja zimam goleminata na datotekata (kon datotekata, pomestuvanje=0, od kade da pocne pomestuvanjeto)
    if(golemina_na_datoteka == -1){
        perror("lseek failed");
        close(fd);
        return -1;
    }
    lseek(fd,0,SEEK_SET);


    mapped_data = mmap(NULL, golemina_na_datoteka, PROT_READ, MAP_PRIVATE, fd, 0); //cel tekst od datoteka kako niza od karakteri
    if (mapped_data == MAP_FAILED) {
        perror("Ne mozam da ja mapiram datotekata");
        close(fd);
        return 1;
    }

    pthread_t threads[MAX];

    for (int i = 0; i < Broj_zborovi; i++)
    {
        printf("Vnesi go zborot koj sakas da go prebarash: ");
        scanf("%s", zborovi_koi_ke_gi_prebaruvame[i]);

        
        pthread_create(&threads[i], NULL, prebaraj, (void*)&i); // mesto indeks, brojot go vnesuvam
    }

    for (int i = 0; i < Broj_zborovi; i++) {
        pthread_join(threads[i], NULL); // Чекај нитките
    }
    //nitkata treba da ispecati kolku pati go nasla zborot

    for (int i = 0; i < Broj_zborovi; i++) {
        printf("Zbor: %s, naiden: %d pati\n",zborovi_koi_ke_gi_prebaruvame[i], rezultantna_niza[i]); // Испечатете резултати
    }

    // Извлечи вкупно најдени зборови
    int vkupno_najdeni = 0;
    for (int i = 0; i < Broj_zborovi; i++) {
        vkupno_najdeni += rezultantna_niza[i];
    }
    printf("Vkupno najdeni zborovi: %d\n", vkupno_najdeni); // Испечати вкупно најдени зборови

    
    munmap(mapped_data, golemina_na_datoteka);
    close(fd);

    return 0;
}