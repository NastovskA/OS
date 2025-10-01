// Да се напише програма во C програмскиот јазик за броење на зборови во датотека.
// Програмата добива име на влезна датотека како аргумент од командна линија. Процесот чита
// по 128 знаци од влезната датотека, притоа на секое второ читање ги праќа податоците до друг
// процес кој ќе го врши броењето на зборовите (скокнува по едно читање т.е. ги праќа вторите 128
// знаци, па четвртите 128 знаци и т.н.). Како метод на комуникација помеѓу двата процеси да се
// користи концепт на цевки, додека пак за броење на зборовите може да се користи наредбата
// „wc -w“ од Shell.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>


int main(int argc, char *argv[]){

    if(argc < 2){
        fprintf("Nema dovolno adrugmenti od komandna linija!");
        exit(1);
    }

    char *ime_datoteka = argv[1];
    pid_t dete1;
    int pipe1[2];
    pipe(pipe1);

    if (pipe(pipe1) == -1) {
        perror("pipe1 error");
        exit(1);
    }
    //0-cita
    //1- zapisuva
    FILE *file_open = fopen(ime_datoteka, "r");
    if(file_open == NULL){
        perror("Greska pri otvaranje!");
        exit(1);
    }

    dete1 = fork();
    if(dete1>0){ //jas sum vo roditelot - glavniot proces
        close(pipe1[0]);
        dup2(pipe1[1], 1);
        close(pipe1[1]);

        //printf sega odi na pipe
        //scanf si ostanuva kaj so si e od datoteka

        int c;
        int flag = 0;
        int end = flag+128;
        int iteracija = 1;

        while(( c = fgetc (file_open)) != EOF){ //FGETC -cita eden znak od datoteka izbrana
            //vo C mi stoi karakterot
            flag++;

            if(iteracija % 2 == 0){
                printf("%c", c);
            }

            if(flag+1 == end){
                flag = end;
                end = flag + 128;
                iteracija++;
            }
            
        } 
        fclose(file_open);
        exit(1);   
    }else if(dete1 == 0){
        close(pipe1[1]);
        dup2[pipe1[0], 0];
        close(pipe1[0]);
        //scanf mi e od pipe
        //printf si ide kaj saka

        int c;
        int brojac= 0;
        int in_word = 0; //deka ne sum vo zbor;

        while((c=getchar()) != EOF){ //GETCHAR = GETC -cita od vlez
            if(!isspace(c)){
                if(!in_word){
                    brojac++;
                    in_word=1;
                }
            }else{
                in_word = 0; 
            }
        }
        printf("Vkupno zborovi ima %d", brojac);
        
        // execlp("wc", "wc", "-w", NULL);
        // perror("Neuspesno izvresno!");
        exit(1);
    }

    return 0;
}