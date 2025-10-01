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
#define broj_nitki 10
#define slucani_broevi 20

int *mapirana_memorija;
int brojac = 0;

int niza_broevi[broj_nitki][slucani_broevi]; 
int kolku_pati[broj_nitki][slucani_broevi];

int golemina;

int *prebaraj(void *arg){
    int indeks = *(int *)arg;
    int lokalno_random[slucani_broevi];

    for(int i=0; i<slucani_broevi; i++){
        int random = rand() % 2501 -10;
        lokalno_random[i] = random;

        int lokalen_brojac =0; //kolku pati se pojavil brojot vo memorija

        for(int j=0; j<brojac; j++){
            if(mapirana_memorija[j] == random){
                lokalen_brojac++;
            }
        }
        niza_broevi[indeks][i]=random;
        kolku_pati[indeks][i] = lokalen_brojac;
    }
    return NULL;
}

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("Nema dovolno argumenti na komandna linija!");
        exit(1);
    }
    
    char *ime_datoteka = argv[1];

    int fd = open(ime_datoteka, O_RDONLY);
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
    
    //do tuka ja imam mapirano datotekata vo mmap memorija

    // int brojac = 0;
    // int zname_za_vo_broj = 0;
    // for(int i=0; i< golemina; i++){ //SE DVIZAM NIZ MAPIRANATA DATOTEKA
    //     if(isdigit(mapirana_memorija[i]) || mapirana_memorija[i] == '-' && i+1<golemina && isdigit(mapirana_memorija[i+1])){
    //         if(!zname_za_vo_broj){ //deka do sega ne sum bila vo broj
    //             brojac++; //najdov nov broj
    //             zname_za_vo_broj = 1; //vlegov vo brojot
    //         }
    //     }else{
    //         zname_za_vo_broj = 0; //ne sum vo brojot
    //     }
    // }
    // printf("Vo datotekata ima %d broevi!", brojac);


    brojac == golemina/sizeof(int);
    printf("Vo datotekata ima %d broevi", brojac);

    pthread_t threads[broj_nitki];
    int indeks_nitki[broj_nitki];

    for(int i=0; i<broj_nitki; i++){
        indeks_nitki[i] = i;
        pthread_create(&threads[i], NULL, prebaraj, &indeks_nitki[i]);
    }

    for(int i=0; i<broj_nitki; i++){
        pthread_join(threads[i], NULL);
    }

    pid_t dete = fork();
    if(dete<0){
        printf("Ne moze da se kreira deteto!");
    }else if(dete == 0){
        for(int i=0; i<broj_nitki; i++){
            for(int j=0; j<slucani_broevi; j++){
                printf("Rezultatite za broj od sekoja nitka se: %d", niza_broevi[i][j]);
                printf("Brojot se ima pojaveno %d pati", kolku_pati[i][j]);}
        }

        exit(0);
    }else{
        wait(NULL);
    }


    munmap(mapirana_memorija, golemina);
    close(fd);

    return 0;
}