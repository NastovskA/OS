//ISTA ZADACA KAKO PIPE1
//SAMO KORISTIME READ I WRITE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int pipe1[2]; // pipe помеѓу родител и процес 2
    int pipe2[2]; // pipe помеѓу процес 2 и процес 3

    if (pipe(pipe1) == -1) {
        perror("pipe1 error");
        exit(1);
    }
    if (pipe(pipe2) == -1) {
        perror("pipe2 error");
        exit(1);
    }

    pid_t pid1 = fork();
    if (pid1 > 0) {
        // Родителски процес - чита од тастатура и праќа во pipe1
        close(pipe1[0]); // Затвори читање од pipe1 - не ни треба
        close(pipe2[0]); // Не користи pipe2
        close(pipe2[1]); // Не користи pipe2

        int broj;
        printf("Внеси цели броеви (Ctrl+D за крај):\n");
        while (scanf("%d", &broj) == 1) {
            if (write(pipe1[1], &broj, sizeof(int)) != sizeof(int)) {
                perror("Грешка при пишување во pipe1");
                break;
            }
        }
        close(pipe1[1]); // Затвори го pipe1 кога завршуваш
        wait(NULL); // Чекај детето
        exit(0);

    } else if (pid1 == 0) {
        pid_t pid2 = fork();
        if (pid2 > 0) {
            // Процес 2 - чита од pipe1, пресметува просек, праќа во pipe2
            close(pipe1[1]); // Затвори пишување во pipe1
            close(pipe2[0]); // Затвори читање од pipe2

            int broj;
            int suma = 0;
            int brojac = 0;

            while (read(pipe1[0], &broj, sizeof(int)) == sizeof(int)) {
                brojac++;
                suma += broj;
                int prosek = suma / brojac;

                if (write(pipe2[1], &prosek, sizeof(int)) != sizeof(int)) {
                    perror("Грешка при пишување во pipe2");
                    break;
                }
            }

            close(pipe1[0]);
            close(pipe2[1]);
            wait(NULL); // Чекај детето
            exit(0);

        } else if (pid2 == 0) {
            // Процес 3 - чита од pipe2 и печати на екран
            close(pipe2[1]); // Затвори пишување во pipe2
            close(pipe1[0]); // Не користи pipe1
            close(pipe1[1]); // Не користи pipe1

            int broj;
            int prethoden_prosek = 0;
            int prvi_prosek = 1;

            while (read(pipe2[0], &broj, sizeof(int)) == sizeof(int)) {
                if (prvi_prosek) {
                    printf("%d\n", broj);
                    prvi_prosek = 0;
                } else {
                    int razlika = broj - prethoden_prosek;
                    printf("%d\n", razlika);
                }
                prethoden_prosek = broj;
            }
            close(pipe2[0]);
            exit(0);
        }
    }
    return 0;
}
