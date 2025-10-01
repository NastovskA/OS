// Да се напише С програма за кодирање на дадена слика од страна на „N“ процеси. Секој процес како
// аргументи добива: <ime_na_datoteka_kade_e_slikata> <koj_del_od_slikata_go_kodira> < kolku pikseli imaime_na_datoteka_kade_e_slikata> <ime_na_datoteka_kade_e_slikata> <koj_del_od_slikata_go_kodira> < kolku pikseli imakoj_del_od_slikata_go_kodira> <ime_na_datoteka_kade_e_slikata> <koj_del_od_slikata_go_kodira> < kolku pikseli ima kolku pikseli ima
// toj del>. Сликата е запишана во текстуална датотека, така што секој знак е пиксел кој има код на боја од
// 0 до 255. Секој процес паралелно врши кодирање на сликата, притоа секој си знае кој дел е неков и
// колкаво парче (секој процес добива подеднакви големини на парчињата пиксели кои треба да ги
// кодираат). Кодирањето се прави така што се зема пиксел по пиксел и се проверува дали неговите
// соседни пиксели (околу него во матрицата) имаат иста вредност или се разликуваат најмногу за 1.
// Доколку е тој услов исполнет, тогаш на екран процесот ги печати координатите на пикселот и бојата со
// која се кодира. Сликата е со димензии 1096х1080. За комуникација помеѓу процесите да се користи
// мемориско мапирање. 

//PROGRAMA
#include <stdio.h>
#define sirocina 1096
#define visina 1080
#define golemina_slika visina*sirocina

int main(int argc, char *argv[]){

    if(argc < 4){
        printf("Nedovolen broj na argumenti!");
        exit(1);
    }

    char *ime_datoteka = argv[1];
    char *koj_del_od_slika = argv[2];
    int *kolku_pikseli = atoi(argv[3]);

    int fd = open(ime_datoteka, O_RDONLY);
    if(fd == -1){
        perror("ERROR");
        exit(1);
    } 

    int golemina = lseek(fd, 0, SEEK_END); //odi do kraj
    lseek(fd, 0, SEEK_SET); //vraka na pocetok

    if (golemina == 0){
        printf("Datotekata e prazna!");
        exit(1);
    }

    char *mapirani_podatoci = mmap(NULL, golemina, PROT_READ, MAP_SHARED,fd, 0); //void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
    if(mapirani_podatoci == MAP_FAILED){
        perror("ERROR");
        exit(1);
    }
    close(fd);

    int N;
    printf("Vnesi broj na procesi! ");
    scanf("%d", &N);
    
    int znak;
    

    for(int i=0; i<N; i++){
        pid_t pid = fork();

        if(pid == 0){ //DETE

        }

    }

    for(int i=0; i<; i++){
        wait(NULL);
    }
    return 0;

}