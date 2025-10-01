//OVAA SEGA JA RESAVAMM NE MI E CELA TREBA DA POPRAVAM

#include <stdio.h>          
#include <stdlib.h>         
#include <unistd.h>     //za pid_t     
#include <sys/types.h>      
#include <time.h>           
#include <string.h>  
#include <fcntl.h>          // za open()
#include <sys/stat.h>       // za struct stat

#define Golemina 1000
#define broj_nitki 10
#define broj_procesi 10
int niza[Golemina];

void *prebaraj(void *arg){
    int indeks = *(int*)arg; //koja nitka ja zimam
    int start, end;
    int brojac = 0; //mora ova da mi e lokalno za da ne mi dojde do race condition
        start = indeks*100;
        end = (indeks+1)*100;
        brojac=0;

        for(int i=start; i < end; i++){
            if(niza[i] == 0){
                brojac++;
            }
        }
        printf("Za nitka so indeks %d, imame deka ja pronasla 0, %d pati!", indeks, brojac);
    
    return NULL;
}

int main(int argc, char *argv[]){

    if(argc < 1){
        printf("Nema dovolno argumenti na komandna linija!");
        exit(1);
    }

    pid_t pid = fork();
    srand(time(NULL));

    int vreme = 0;

    if(pid < 0){
        printf("Nieden od procesite ne raboti!");
        exit(1);
    }else if(pid > 0){ //RODITEL
        for(int i=0; i<Golemina; i++){
            niza[i] = rand() % 1000; //tuka generiram broj od 0 do 999
        }

        for(int i=0; i<2; i++){ //sozdavam 2 deza
            pid_t dete = fork();
            if(dete > 0 || dete < 0 ){
                break;
            }else if (dete == 0 && i==0){
                for(int j=0; j<broj_procesi; j++){
                    pid_t pomosen_proc = fork();
                    if(pomosen_proc < 0 || pomosen_proc > 0){
                        break;

                    }else{
                        int start = j*100;
                        int end = (j+1)*100;
                        int brojac =0;
                        for(int k=start; k<end; k++){
                            if(niza[k] == 0) brojac++;
                        }
                        printf("Proces so id %d go najde brojot %d pati", j, brojac);
                    }
                }
                for(int j=0; j<broj_procesi; j++){
                    wait(NULL);
                }
                exit(0);
            }else if(dete == 0 && i == 1){
                pthread_t threads[broj_nitki]; //ja cuva ID na sekoja nitka
                int indeks_nitka[broj_nitki]; //preku ova ja prakam nitkata na funkcijata

                for(int k=0; k<broj_nitki; k++){
                    indeks_nitka[k] = k;
                    pthread_create(&threads[k], NULL, prebaraj, &indeks_nitka[k]); //kreira nova nitka, sekoe prebaraj dobiva razlicen indeks 
                    //so &indeks_nitka se praka na funkcijata prebaraj
                }

                for(int k=0; k<broj_nitki; k++){
                    pthread_join(threads[k], NULL); //sekoja nitka cekam da zavrsi
                }
                exit(0);
            }
            
        }

        
    }

}