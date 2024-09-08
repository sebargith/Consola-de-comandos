#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define T_MAX 1024  //tamaño maximo de entrada
#define T_MAX_ARG 100  //Tamaño maximo de argumentos

void shell() {
    char input[T_MAX];
    char *args[T_MAX_ARG];
    char *temp;
    int i;

    while (1) {
        //Mostramos el propmt de la shell
        printf("TestShell> ");
        fflush(stdout);

        //Leemos el argumento
        if (fgets(input, T_MAX, stdin) == NULL) {
            perror("Error leyendo la entrada");
            continue;
        }

        //Salto de linea
        input[strcspn(input, "\n")] = '\0';

        //Para continuar en caso de tener enter como entrada
        if (strlen(input) == 0) {
            continue;
        }

        // Guardamos el argumento
        i = 0;
        temp = strtok(input, " ");
        while (temp != NULL) {
            args[i] = temp;
            i++;
            temp = strtok(NULL, " ");
        }
        args[i] = NULL;

        // Procesamos el argumento de forma inicial devolviendo la entrada
        printf("Comando: %s\n", args[0]);
        for (int j = 1; j < i; j++) {
            printf("Argumento %d: %s\n", j, args[j]);
        }
    }
}

int main() {

    shell();

    return 0;
}
