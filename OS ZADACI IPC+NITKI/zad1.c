// Да се напише програма во C програмскиот јазик каде ќе се овозможи внесување на име на датотека како
// аргумент на програмата. Потоа, програмата треба да изврши мемориско мапирање на датотеката во
// адресниот простор. Датотеката е составена од N цели броеви. Главниот процес, треба да ја измине
// датотеката и да провери колку цели броеви има во (мемориски мапираната) датотека. Потоа, треба да
// креира 10 нитки, каде што секоја една од нитките ќе генерира случаен број од -10 до 2500, и ќе провери
// дали го има во датотеката. Доколку го има, бројот што е случајно генериран го сместува во низа, приота
// доколку некоја нитка повеќе пати го генерира истиот број, тогаш треба да се знае колку пати е секој
// еден број пронајден (генериран). Секоја нитка генерира 20 случајни броеви и потоа завршува. Откако ќе
// завршат сите нитки, главниот процес ги пренесува најдените броеви на дете процес, кое што тој го
// креира. Дете процесот е задолжено да отпечати на екран кои броеви биле пронајдени и колку пати во
// влезната датотека. 
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

#define NUM_THREADS 10 //brojj na nitki
#define MIN_RANDOM -10
#define MAX_RANDOM 2500
#define NUM_RANDOM 20
#define MAX_NUMBER 200


int *mapped_data; //pokazuvac kon memoriski mapirana datoteka
int num_Numbers; //broj na elementi vo datoteka
int found_numbers[MAX_NUMBER]; //niza na pronajdeni broevi
int found_count[MAX_NUMBER]; //kolku pati sekoj broj e pronajden


//KOLKU CELI BROEVI IMA VO DATOTEKATA - РАБОТА НА ГЛАВНИОТ ПРОЦЕС
int countNumbers(const char *filename){ //vraka cel broj bidejki e INT funkcija,  const bidejki nema da se menuva  nisto vo datotekata
    FILE *file = fopen(filename, "r"); // site raboti od datotekata ke se cuvaat vo file
    if(file == NULL){  //file e null ne moze da se otvori datotekata
        printf("Failed to open file\n");
        return -1;
    }

    int count = 0; //za kolku broevi ima
    int num; //tuka ke se zacuva sekoj broj sto ke go procitame od datoteka
    while(fscanf(file, "%d", &num) == 1){ //cita od kade, zapisuva kade, ako e 1 znaci uspesno e procitano
        count++;
    }

    fclose(file);
    return count; //vrakam kolku broevi ima
}

void *prebaraj(void *t){ //vraka pokazuvac poradi nitkite

    int id= *((int *)t); //so ova dobivam vrednost na nitkata
    int found_inThread[NUM_RANDOM]; //niza za broevi sto nitkata gi nasla vo datotekata

    //gi rasporeduva nitkite sekoja so svojot del da pocne, 
        for(int i = id * NUM_RANDOMS; i < id * NUM_RANDOMS + NUM_RANDOMS; i++) {//nitkata generria 20 slucajni broevi, da nema dupliranje
           
            int random_num = rand() % (MAX_RANDOM - MIN_RANDOM + 1) + MIN_RANDOM; //generiram random broj
            for(int j = 0; j < num_Numbers; j++) { //proveruva dali brojot postoi vo mapped
                if(mapped_data[j] == random_num) {
                    found_inThread[i] = random_num;ako brojot postoi go dodava vo nizata
                    break;
                }
            }
        }
        for(int i = 0; i < NUM_RANDOMS; i++) {
            if(found_inThread[i] != 0) {
                int found = 0;  //za dali postoi ili ne
    
                for(int j = 0; j < MAX_NUMBERS; j++) {
                    if (found_numbers[j] == found_inThread[i]) {//ako brojot prethodno e dodaden
                        found_count[j]++; //zgolemuva brojac za kolku pati
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    for (int j = 0; j < MAX_NUMBERS; j++) {
                        if (found_numbers[j] == 0) { //koga brojot e sosema nov za dodavanje
                            found_numbers[j] = found_inThread[i];
                            found_count[j] = 1;
                            break;
                        }
                    }
                }
            }
        }
        pthread_exit(NULL); //zavrsuva so null bidejki mora da vratat *
    
}


int main(int argc, char *argv[]) { //argc - broj na argumenti sto gi prima programata(so se ime na datoteka za izvrsuvanje), argv[] - niza sto sodrzi argumenti od komandna linija

    num_Numbers = countNumbers(argv[1]); //broi kollku broevi ima vo datoteka

    printf("Number of elements in the file: %d\n", num_Numbers); //gi pecati

    int fd = open(argv[1], O_RDONLY); //ja otvaram datotekata za citanje
    if (fd == -1) {  //ne uspealo otvaranjeto
        perror("Failed to open file");
        return 1;
    }

    int file_size = lseek(fd, 0, SEEK_END); //odime na kraj na fajlot za negovata golemina
    if (file_size == -1) { //nesto se slucilo greska
        perror("lseek failed");
        close(fd);
        return -1;
    }
    lseek(fd, 0, SEEK_SET); //go vraka kurserot na pocetokot

    mapped_data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(mapped_data == MAP_FAILED) { //akoo ne uspee pecati greska
        perror("Memory mapping failed");
        close(fd);
        return 1;
    }

    close(fd); //zatvoram fajl

    pthread_t nitki[NUM_THREADS]; //kreiram nitka
    int t[NUM_THREADS]; // id na nitkite

    for (int i = 0; i < NUM_THREADS; i++) {
        t[i] = i;
        pthread_create(&nitki[i], NULL, prebaraj, (void*)&t[i]); //startuvanje na nitka i ja povikuva funkcijata prebaruvaj
    }

    for (int i = 0; i < NUM_THREADS; i++) { //da zavrsat edna po edna nitka se cekaat
        pthread_join(nitki[i], NULL);
    }

    pid_t dete = fork(); //sozdavame nov proces dete

    if (dete == 0) { //ako sum vo deteto pecati mi gi najdenite broevi
        for (int i = 0; i < MAX_NUMBERS; i++) {
            if (found_numbers[i] != 0) {
                printf("Number: %d, Count: %d\n", found_numbers[i], found_count[i]);
            }
        }
        return 0;//deteto zavrsuva
    } else if (dete > 0){ //tuka veke sme roditel
        wait(NULL);
    } else { //ako ne uspee vrakame -1
        perror("Fork failed");
        return 1;
    }

    return 0;
}