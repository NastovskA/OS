// Да се напишат две програми во C кои работат со процеси и нитки. Првата програма се повикува од
// конзола и притоа како аргумент (од командна линија) се испраќа број „N“ кој кажува кој број треба да
// се пребара. Потоа, првата програма ја стартува втората програма како аргумент од командна линија ѝ ја
// испраќа вредноста „N“ за пребарување. Двата процеси креираат по 5 нитки кои ја пребаруваат
// вредноста N во мемориски мапирана датотека (со име „dat.txt“) во која има 100 цели броеви. Секоја од
// нитките на двата процеси генерира случаен индекс од низата од 100 цели броеви и проверува дали е на
// таа позиција во датотеката бројот „N“. Доколку е, тогаш ја менува неговата вредност со зголемување за
// 1 кај првиот процес и намалување за 1 кај вториот процес (каде што ќе го пронајде „N“ поставува
// вредност „N+1“ или „N-1“ соодветно) и продолжува понатаму. Пребарувањето трае 5 секунди (и за
// двата процеси), потоа на крај двата процеси печатат колку пати ја пронашле вредноста „N“ вкупно од
// сите 5 нитки. 

//PROGRAMA 1
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
#define KOLKU_BROEVI 100
int N;
int *mapped_data;

void *prebaruvaj(void *arg){
    int id = *(int *)arg;
    int brojac=0;

    for(int i=0; i<5; i++){
    int random_indeks = rand() % 100;
   // pthread_mutex_lock(&mutex);
    if(mapped_data[random_indeks] == N){
        mapped_data[random_indeks] = N+1;
        brojac++;
    }
   // pthread_mutex_unlock(&mutex);
    }
    printf("Nitkata go najde brojot %d, %d pati! ", N, brojac);
    
}


int main(int argc, char *argv[]){

    N = atoi(argv[1]); //koj broj go prebaruvam
    
    if(argc < 2){ //ime na datoteka + 1 
        printf("Nevaliden broj na argumenti");
        return EXIT_FAILURE;
    }

    pid_t dete = fork();

    if(dete < 0){
        perror("Ne moze da se kreira proces");
        return EXIT_FAILURE;
    }
    
    if(dete == 0){
        execl("./programa2", "programa2", argv[1], NULL);  // Startuvanje na programata 2
        perror("execl failed");
        return EXIT_FAILURE;
    }
 

    int fd = open("dat.txt", O_RDONLY); // Отвори датотека во режим за читање
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


    mapped_data = mmap(NULL, golemina_na_datoteka, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped_data == MAP_FAILED) {
        perror("Ne mozam da ja mapiram datotekata");
        close(fd);
        return 1;
    }

    pthread_t threads[5]; //inicijalizacija
    int indeks_nitka[5]; //argument na prebaraj
    for(int i=0; i<5; i++){
        indeks_nitka[i] = i;
        pthread_create(&threads[i], NULL, prebaraj, &indeks_nitka[i]);//kreiram nitka,    ,    ,  id na nitka
    }


    //site nitki da zavrsat pred da prodolzam ponatamu
    for(int i=0; i<5; i++){
        pthread_join(threads[i], NULL);
    }

    munmap(mapped_data, golemina_na_datoteka);  
    close(fd);  

    wait(NULL); //cekanje za 2 proces

    return 0;

}

//PROGRAMA 2
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
#define KOLKU_BROEVI 100
int *mapped_data;


void *prebaruvaj(void *arg){
    int id = *(int *)arg;
    int brojac=0;
    for(int i=0; i<5; i++){
    int random_indeks = rand() % 100;
    
    if(mapped_data[random_indeks] == N){
        mapped_data[random_indeks] = N-1;
        brojac++;
    }
    }
    printf("Nitkata go najde brojot %d, %d pati! ", N, brojac);
    
}

int main(int argc, char *argv[]){

    if(argc < 2) {
        printf("Nevaliden broj na argumenti\n");
        return EXIT_FAILURE;
    }

    int N = atoi(argv[1]);

    int fd = open("dat.txt", O_RDWR);  // Отвори ја датотеката
    if (fd == -1) {
        perror("Ne mozam da ja otvoram datotekata");
        return 1;
    }

    int golemina_na_datoteka = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    mapped_data = mmap(NULL, golemina_na_datoteka, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped_data == MAP_FAILED) {
        perror("Ne mozam da ja mapiram datotekata");
        close(fd);
        return 1;
    }

    pthread_t threads[5];  // 5 нитки
    int indeks_nitka[5]; //argument na prebaraj
    for(int i=0; i<5; i++){
        indeks_nitka[i] = i;
        pthread_create(&threads[i], NULL, prebaraj, &indeks_nitka[i]);//kreiram nitka,    ,    ,  id na nitka
    }

    for(int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    munmap(mapped_data, golemina_na_datoteka);  // Освободување на меморија
    close(fd);  // Затворање на датотеката
    return 0;

}
