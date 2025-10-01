//SO PRINTF I SCANF

/*Да се напише C програма за комуникација помеѓу три процеси. Процесите 
комуницираат преку pipe комуникација, така што првиот процес комуницира 
со вториот процес преку еден pipe, додека пак вториот комуницира со третиот
процес преку втор pipe. Првиот процес чита цели броеви од тастатура и број
по број ги испраќа на вториот процес. Вториот процес, ги чита броевите, 
број по број, пресметува просек на моментално пристигнатите броеви, така
што за секој нареден број се наново пресметува просекот вклучувајќи го 
и новиот број. Просекот на дотогаш внесените броеви го испраќа на третиот
процес и чека нов број. Третиот процес, ги чита просек по просек и на 
екран го печати првиот просек, а секој нареден просек печати колава е 
разликата од претходниот просек.*/

/*Пример: Прв процес чита 12, втор процес пресметува 12, трет процес печати 12
Потоа, прв процес чита 8, втор процес пресметува 10, трет процес печати 10-12=-2
Потоа, прв процес чита 5, втор процес пресметува 25/3 = 6, трет процес печати 6-10=-4 
И така натаму*/

#include <stdio.h>
#include <unistd.h>  // за pipe, fork, dup2
#include <sys/types.h>
#include <stdlib.h>  // за exit

int main(){
    int pipe1[2], pipe2[2]; //imam 2 cefki, i dvete mi trebaat za zapisuvanje/citanje
    pipe(pipe1);
    pipe(pipe2);
    //pipe1[0] - cita od pip
    //pip1[1] - zapisuva vo pipe 

    pid_t dete1;
    pid_t dete2;

    dete1=fork();
    if(dete1>0){ //Sum vo roditelot - glavniot proces

        close(pipe1[0]);
        dup2(pipe1[1], 1); //od standarden vlez da zimam podatoci i da idat na pipe
        close(pipe1[1]);
        int broj;
        printf("Vnesi broj od tastatura! ");
        while(scanf("%d", &broj) == 1){
            printf("%d\n", broj); //printf sega izleguva na pipe1
        }
        exit(0);

    }else if(dete1==0){
        dete2 = fork();
        if(dete2 > 0){ //proces 2 koj e roditel na proces 3

            close(pipe1[1]);
            dup2(pipe1[0], 0);
            close(pipe1[0]);
            //scanf mi e nasocen kon pipe1

            close(pipe2[0]);
            dup2(pipe2[1],1);
            close(pipe2[1]);
            //tuka veke printf mi e nasocen kon pipe2;

            int broj;
            int suma = 0;
            int brojac = 0;
            while(scanf("%d", &broj) == 1){
                brojac++;
                suma+=broj;
                int prosek = suma/brojac;

                printf("%d", prosek);
            }
            exit(0);
        }else if(dete2 == 0){//ova znaci deka e proces 3 koj e dete na proces 2
            //0-cita
            //1-zapisuva

            close(pipe2[1]);
            dup2(pipe2[0], 0);
            close(pipe2[0]);
            //ovde scanf mi doaga od pipe2

            //printf si ide na ekran

            int broj;
            int prethodnik;
            int flag = 0;
            int razlika = 0;

            while(scanf("%d", &broj) == 1){
                if(flag == 0){
                    printf("%d", broj);
                    flag = 1;
                }else{
                    razlika = broj - prethodnik;
                    printf("%d", razlika);
                }
                prethodnik = broj;
            }
            exit(0);
        }
        
    }

    return 0;
}