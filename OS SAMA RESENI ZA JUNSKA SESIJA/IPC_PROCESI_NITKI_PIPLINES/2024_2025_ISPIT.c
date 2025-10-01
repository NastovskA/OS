// Да се напише програма во C програмскиот јазик за работа со процеси, нитки и IPC. Програмата како аргументи од командна
// линија добива клуч за Shared memory и име на датотека за мемориско мапирање. Процесот креира дете процес кои двата
// комуницираат преку Shared memory. Дете процесот мемориски ја мапира датотеката чија содржина треба да се пребара со 26 нитки.
// Дете процесот ги креира 26те нитки, притоа секоја нитка пребарува по една буква (во мапираната датотека) од ASCII табелата (без
// разлика дали е мала или голема), така што првата нитка ја пребарува буквата „а“ втората буквата „b“ и т.н. Откако ќе завршат со
// пребарување нитките, дете процесот, преку Shared memory му кажува на родител процесот дека нитките завршиле и резултатот од
// пребарувањето се запишува во shared memory. Родител процесот чека нитките на детето да завршат со извршување а потоа
// резултатот од пребарувањето, родител процесот го печати на екран. 

//procesi nitki i ipc
#define broj_nitki 26
#include <stdio.h>

int *mapirana_memory;
char *shared_memory;
char *ime_datoteka;
int fd;
int rezultati[broj_nitki];
int golemina;

void *prebaraj(void *arg){
    int indeks = *(int*)arg; //koja nitka ja zimam

    int brojac = 0;

    char *mem = (char *)mapirana_memory; //ova e pokazuvac za vo mapirana memorija

    char c;
   for(int i=0; i<golemina; i++){
        char c = mem[i];
        if(indeks == 0 && c == 'a' || c == 'A'){ brojac++;}
        else if(indeks == 1 && c =='b' || c =='B') {brojac++;}
        else if (indeks == 2 && c =='c' || c =='C') {brojac++;}
        else if (indeks == 3 && c =='d' || c =='D') {brojac++;}
        else if (indeks == 4 && c =='e' || c =='E') {brojac++;}
        else if (indeks == 5 && c =='f' || c =='F') {brojac++;}
        else if (indeks == 6 && c =='g' || c =='G') {brojac++;}
        else if (indeks == 7 && c =='h' || c =='H') {brojac++;}
        else if (indeks == 8 && c =='i' || c =='I') {brojac++;}
        else if (indeks == 9 && c =='j' || c =='J') {brojac++;}
        else if (indeks == 10 && c =='k' || c =='K') {brojac++;}
        else if (indeks == 11 && c =='l' || c =='L') {brojac++;}
        else if (indeks == 12 && c =='m' || c =='M') {brojac++;}
        else if (indeks == 13 && c =='n' || c =='N') {brojac++;}
        else if (indeks == 14 && c =='o' || c =='O') {brojac++;}
        else if (indeks == 15 && c =='p' || c =='P') {brojac++;}
        else if (indeks == 16 && c =='q' || c =='Q') {brojac++;}
        else if (indeks == 17 && c =='r' || c =='R') {brojac++;}
        else if (indeks == 18 && c =='s' || c =='S') {brojac++;}
        else if (indeks == 19 && c =='t' || c =='T') {brojac++;}
        else if (indeks == 20 && c =='u' || c =='U') {brojac++;}
        else if (indeks == 21 && c =='v' || c =='V') {brojac++;}
        else if (indeks == 22 && c =='w' || c =='W') {brojac++;}
        else if (indeks == 23 && c =='x' || c =='X') {brojac++;}
        else if (indeks == 24 && c =='y' || c =='Y') {brojac++;}
        else if (indeks == 25 && c =='z' || c =='Z') {brojac++;}
    }
    rezultati[indeks] = brojac;
    return NULL;
}

int main(int argc, char *argv[]){
    if (argc < 3){
        printf("Nemate dovolno argumenti na komandna linija!");
        exit(1);
    }

    int kluc = atoi(argv[1]); //za broevi
    ime_datoteka = argv[2];
    int flag = 0; //0 zavrseno 1 zafateno
    int rezultat;

    pid_t dete;

    int shmid = shmget(kluc, sizeof(int), 0666 | IPC_CREAT);
    if(shmid < 0){
        perror("ERROR, ne moze da se postavi shared memorijata!");
        exit(1);
    }

    shared_memory = (char *)shmat(shmid, NULL, 0); //shared mem attach, shmat vraka void i zatoa jas imam (int *)
    if(shared_memory == (void *) -1){
        perror("Neuspesno povrzuvanje na shared memory!");
        exit(1);
    }

    shared_memory[0] = flag;

    dete = fork();
    if(dete < 0){
        printf("Nema vospostavuvanje na dete proces!");
        exit(1);
    }else if (dete == 0){ //sum vo deteto
        
        fd = open(ime_datoteka, O_RDONLY);
        if (fd == -1){ 
            perror("ERROR, ne moze da se otvori");
            exit(1);
        }
        golemina = lseek(fd, 0, SEEK_END); // odi do kraj
        lseek(fd, 0, SEEK_SET);                // vraka na pocetok

        if (golemina == 0){
            printf("Datotekata e prazna!");
            exit(1);
        }
        mapirana_memory = mmap(NULL, golemina, PROT_READ, MAP_SHARED, fd, 0); // void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
        if (mapirana_memory == MAP_FAILED){
            perror("ERROR");
            exit(1);
        }
        pthread_t threads[broj_nitki]; //ja cuva ID na sekoja nitka
        int indeks_nitka[broj_nitki]; //preku ova ja prakam nitkata na funkcijata

        for(int i=0; i<broj_nitki; i++){
            indeks_nitka[i] = i;
            pthread_create(&threads[i], NULL, prebaraj, &indeks_nitka[i]); //kreira nova nitka, sekoe prebaraj dobiva razlicen indeks 
            //so &indeks_nitka se praka na funkcijata prebaraj
        }

        for(int i=0; i<broj_nitki; i++){
            pthread_join(threads[i], NULL); //sekoja nitka cekam da zavrsi
        }

        shared_memory[0] = 1; //tuka signaliziram deka e gotovo
        memcpy(&shared_memory[1], rezultati, sizeof(rezultati)); //so ova od lokacija 1 vo shared memorijata si zapisuvam vredosti na nizata 
    }
    wait(NULL);

    while (shared_memory[0] != 1) {
    sleep(1); // чека додека дете процесот не заврши
    }

    int *rezultati_od_deteproces_citam_od_shar_mem = (int *)&shared_memory[1]; //pokazuvac za da gi zemam int vrednostite od nizata vo shared memorijata
    for(int i=0; i<broj_nitki; i++){
        printf("Rezultatite za sekoja bukva se: %d", rezultati_od_deteproces_citam_od_shar_mem[i]);
    }
    shmdt(shared_memory);
    shmctl(shmid, IPC_RMID, NULL);
    munmap(mapirana_memory, golemina);
return 0;
}





