#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define T_MAX 1024  //tamaño maximo de entrada
#define T_MAX_ARG 100  //Tamaño maximo de argumentos

void shell() {
    char input[T_MAX];
    char *args[T_MAX_ARG];
    char *temp;
    int i;
    pid_t pid; // Para almacenar el ID del proceso hijo
    int estado; // Para almacenar el estado de terminación del hijo

    while (1) {
        // 1. Mostrar el prompt de la shell
        printf("TestShell> ");
        fflush(stdout);

        // 2. Leer el argumento
        if (fgets(input, T_MAX, stdin) == NULL) {
            perror("Error leyendo la entrada");
            continue;
        }

        // Remover el salto de línea de la entrada
        input[strcspn(input, "\n")] = '\0';

        // Continuar si se presiona "enter" sin escribir un comando
        if (strlen(input) == 0) {
            continue;
        }

        // Guardar el comando y los argumentos
        i = 0;
        temp = strtok(input, " ");
        while (temp != NULL) {
            args[i] = temp;
            i++;
            temp = strtok(NULL, " ");
        }
        args[i] = NULL; // El último argumento debe ser NULL para execvp

        //Creamos un proceso hijo para ejecutar el comando
        pid = fork();

        if (pid == -1) {
            //Caso de error
            perror("Error al crear el proceso hijo");
        } else if (pid == 0) {
            // Proceso hijo: ejecutar el comando
            if (execvp(args[0], args) == -1) {
                perror("Error ejecutando el comando");
            }
            exit(EXIT_FAILURE); // Terminar el proceso hijo en caso de error
        } else {
            // Proceso padre: esperar a que el hijo termine
            wait(&estado); 
        }
    }
}

int main() {

    shell();
    return 0;
}
