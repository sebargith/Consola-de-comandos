#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define T_MAX 1024  // Tamaño máximo de entrada
#define T_MAX_ARG 100  // Tamaño máximo de argumentos
#define MAX_FAVS 100  // Máximo número de comandos favoritos

// Estructura para almacenar comandos favoritos
typedef struct {
    char comando[T_MAX];
    int id;
} Favorito;

Favorito favoritos[MAX_FAVS];
int num_favoritos = 0;
int fav_id = 1;
char archivo_favoritos[T_MAX] = "";

// Prototipos de funciones
void agregar_favorito(char *comando);
void mostrar_favoritos();
void eliminar_favoritos(char *ids);
void buscar_favoritos(char *cmd);
void ejecutar_favorito(int id);
void manejar_favs(char *input);
void ejecutar_comando(char **args);
void manejar_pipes(char *input);
void shell();
void cargar_favoritos();
void guardar_favoritos();

// Implementación de las funciones
void agregar_favorito(char *comando) {
    for (int i = 0; i < num_favoritos; i++) {
        if (strcmp(favoritos[i].comando, comando) == 0) {
            return;
        }
    }
    strcpy(favoritos[num_favoritos].comando, comando);
    favoritos[num_favoritos].id = fav_id++;
    num_favoritos++;
}

void mostrar_favoritos() {
    printf("Lista de comandos favoritos:\n");
    for (int i = 0; i < num_favoritos; i++) {
        printf("%d: %s\n", favoritos[i].id, favoritos[i].comando);
    }
}

void eliminar_favoritos(char *ids) {
    int id;
    char *token = strtok(ids, ",");
    while (token != NULL) {
        id = atoi(token);
        for (int i = 0; i < num_favoritos; i++) {
            if (favoritos[i].id == id) {
                for (int j = i; j < num_favoritos - 1; j++) {
                    favoritos[j] = favoritos[j + 1];
                }
                num_favoritos--;
                break;
            }
        }
        token = strtok(NULL, ",");
    }
}

void buscar_favoritos(char *cmd) {
    printf("Resultados de la búsqueda:\n");
    for (int i = 0; i < num_favoritos; i++) {
        if (strstr(favoritos[i].comando, cmd) != NULL) {
            printf("%d: %s\n", favoritos[i].id, favoritos[i].comando);
        }
    }
}

void ejecutar_favorito(int id) {
    for (int i = 0; i < num_favoritos; i++) {
        if (favoritos[i].id == id) {
            char *args[T_MAX_ARG];
            int j = 0;
            char *temp = strtok(favoritos[i].comando, " ");
            while (temp != NULL) {
                args[j] = temp;
                j++;
                temp = strtok(NULL, " ");
            }
            args[j] = NULL;
            ejecutar_comando(args);
        }
    }
}

void manejar_favs(char *input) {
    if (strncmp(input, "favs crear ", 11) == 0) {
        strcpy(archivo_favoritos, input + 11);
        FILE *file = fopen(archivo_favoritos, "w");
        if (file == NULL) {
            perror("Error creando archivo de favoritos");
        } else {
            printf("Archivo de favoritos creado: %s\n", archivo_favoritos);
            fclose(file);
        }
    } else if (strcmp(input, "favs mostrar") == 0) {
        mostrar_favoritos();
    } else if (strncmp(input, "favs eliminar ", 14) == 0) {
        eliminar_favoritos(input + 14);
    } else if (strncmp(input, "favs buscar ", 12) == 0) {
        buscar_favoritos(input + 12);
    } else if (strcmp(input, "favs borrar") == 0) {
        num_favoritos = 0;
    } else if (strncmp(input, "favs ", 5) == 0 && strstr(input, " ejecutar") != NULL) {
        int id = atoi(input + 5);
        ejecutar_favorito(id);
    } else if (strcmp(input, "favs cargar") == 0) {
        cargar_favoritos();
    } else if (strcmp(input, "favs guardar") == 0) {
        guardar_favoritos();
    }
}

void cargar_favoritos() {
    FILE *file = fopen(archivo_favoritos, "r");
    if (file == NULL) {
        perror("Error al cargar favoritos");
        return;
    }
    num_favoritos = 0;
    while (fscanf(file, "%d %[^\n]", &favoritos[num_favoritos].id, favoritos[num_favoritos].comando) != EOF) {
        if (favoritos[num_favoritos].id >= fav_id) {
            fav_id = favoritos[num_favoritos].id + 1;
        }
        num_favoritos++;
    }
    printf("Favoritos cargados desde el archivo: %s\n", archivo_favoritos);
    fclose(file);
}

void guardar_favoritos() {
    FILE *file = fopen(archivo_favoritos, "w");
    if (file == NULL) {
        perror("Error al guardar favoritos");
        return;
    }
    for (int i = 0; i < num_favoritos; i++) {
        fprintf(file, "%d %s\n", favoritos[i].id, favoritos[i].comando);
    }
    printf("Favoritos guardados en el archivo: %s\n", archivo_favoritos);
    fclose(file);
}

void ejecutar_comando(char **args) {
    if (execvp(args[0], args) == -1) {
        perror("Error ejecutando el comando");
    }
    exit(EXIT_FAILURE); // Terminar el proceso hijo en caso de error
}

void manejar_pipes(char *input) {
    int fd[2];
    pid_t pid;
    int estado;
    char *comandos[10];
    char *args[T_MAX_ARG];
    int i = 0;

    // Dividir la entrada en comandos usando el delimitador '|'
    comandos[i] = strtok(input, "|");
    while (comandos[i] != NULL) {
        i++;
        comandos[i] = strtok(NULL, "|");
    }

    int num_comandos = i;
    int in_fd = 0; // File descriptor para la entrada

    for (int j = 0; j < num_comandos; j++) {
        // Crear un pipe
        pipe(fd);
        pid = fork();
        if (pid == -1) {
            perror("Error al crear el proceso hijo");
        } else if (pid == 0) {
            // Redirigir la entrada estándar
            dup2(in_fd, STDIN_FILENO);
            // Redirigir la salida estándar al pipe, excepto para el último comando
            if (j < num_comandos - 1) {
                dup2(fd[1], STDOUT_FILENO);
            }
            close(fd[0]);

            // Parsear los argumentos
            int k = 0;
            args[k] = strtok(comandos[j], " ");
            while (args[k] != NULL) {
                k++;
                args[k] = strtok(NULL, " ");
            }

            // Ejecutar el comando
            ejecutar_comando(args);
        } else {
            // Proceso padre: esperar a que el hijo termine
            wait(&estado);
            close(fd[1]);
            // Guardar el extremo de lectura del pipe
            in_fd = fd[0];
        }
    }
}

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

        // Comprobar si el comando es "exit"
        if (strcmp(input, "exit") == 0) {
            printf("Saliendo de la shell...\n");
            break;
        }

        // Comprobar si el comando es un recordatorio
        if (strncmp(input, "set recordatorio ", 17) == 0) {
            int tiempo;
            char mensaje[T_MAX];

            // Parsear el comando de recordatorio
            sscanf(input + 17, "%d \"%[^\"]\"", &tiempo, mensaje);

            pid = fork();
            if (pid == -1) {
                perror("Error al crear el proceso hijo");
            } else if (pid == 0) {
                // Proceso hijo: esperar el tiempo especificado y mostrar el mensaje
                sleep(tiempo);
                printf("\nRecordatorio: %s\n", mensaje);
                exit(EXIT_SUCCESS);
            } else {
                // Proceso padre: continuar
                continue;
            }
        }

        // Comprobar si el comando es un comando de favoritos
        if (strncmp(input, "favs", 4) == 0) {
            manejar_favs(input);
            continue;
        }

        // Comprobar si hay pipes en el comando
        if (strchr(input, '|') != NULL) {
            manejar_pipes(input);
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

        // Agregar el comando a los favoritos, si no es un comando de favoritos
        agregar_favorito(input);

        // Crear un proceso hijo para ejecutar el comando
        pid = fork();

        if (pid == -1) {
            // Caso de error
            perror("Error al crear el proceso hijo");
        } else if (pid == 0) {
            // Proceso hijo: ejecutar el comando
            ejecutar_comando(args);
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
