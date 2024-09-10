Consola de Comandos en C

Este proyecto consiste en una implementación de una consola de comandos (shell) en C, desarrollada como parte del Proyecto 1 de la asignatura de Sistemas Operativos, Segundo Semestre 2024. Esta shell es capaz de ejecutar comandos en procesos concurrentes, gestionar tuberías (\textit{pipes}), mantener una lista de comandos favoritos de forma persistente, y crear recordatorios con temporizadores.

 Características

- **Ejecución de comandos**: Soporte para ejecutar comandos estándar de Unix/Linux.
- **Soporte para pipes**: Capacidad de conectar comandos mediante tuberías (`|`).
- **Comandos favoritos**: Almacenamiento, visualización, eliminación y ejecución de comandos favoritos.
- **Recordatorios**: Sistema de recordatorios basado en temporizadores.

 Requisitos

- **Compilador GCC**: El código está escrito en C y requiere un compilador compatible como `gcc`.
- **Sistema operativo**: Probado en distribuciones Linux.

 Instalación

1. Clona el repositorio en tu máquina local:
    ```bash
    git clone https://github.com/sebargith/Consola-de-comandos.git
    ```
2. Navega al directorio del proyecto:
    ```bash
    cd Consola-de-comandos
    ```
3. Compila el archivo fuente `shell.c`:
    ```bash
    gcc -o shell shell.c
    ```

 Uso

Para iniciar la consola de comandos, ejecuta el binario generado:

```bash
./Shell

Ejecución de Comandos Básicos
Puedes ejecutar cualquier comando de Unix/Linux desde el prompt, por ejemplo:
TestShell> ls -l

Soporte para Pipes
La consola soporta la ejecución de comandos conectados por tuberías  lo que permite usar la salida de un comando como entrada de otro. Ejemplo:

TestShell> ps -aux | sort -nr -k 4 | head -10

Comandos Favoritos
La shell incluye una serie de comandos personalizados para manejar una lista de comandos favoritos. A continuación se describen estos comandos:

Crear archivo de favoritos:
TestShell> favs crear ruta/favoritos.txt

Crea un archivo de favoritos en la ruta especificada para guardar los comandos favoritos de manera persistente.

Mostrar favoritos:

TestShell> favs mostrar

Muestra la lista de comandos favoritos almacenados.

Eliminar favoritos:

TestShell> favs eliminar num1,num2

Elimina los comandos favoritos asociados a los números especificados.

Buscar favoritos:

TestShell> favs buscar nombre_comando

Busca un comando en la lista de favoritos que contenga el nombre parcial o completo.

Ejecutar comando favorito:

TestShell> favs num ejecutar

Ejecuta el comando favorito con el número asociado.

Guardar comandos favoritos:

TestShell> favs guardar

Guarda los comandos favoritos en el archivo especificado.

Cargar comandos favoritos:

TestShell> favs cargar

Carga los comandos favoritos desde el archivo previamente creado.

Recordatorios
La consola también te permite crear recordatorios que se activan después de un tiempo específico. El formato del comando es:

TestShell> set recordatorio <tiempo> "mensaje"

Ejemplo:

TestShell> set recordatorio 10 "Hacer una pausa activa"

Esto mostrará el mensaje después de 10 segundos.

Salir de la Shell
Para salir de la consola, simplemente ingresa:

TestShell> exit

Estructura del Código
El archivo shell.c contiene la implementación de todas las funcionalidades. Las funciones principales incluyen:

ejecutar_comando(): Ejecuta un comando en un proceso hijo.
manejar_pipes(): Maneja la ejecución de comandos conectados por pipes.
manejar_favs(): Administra la funcionalidad de favoritos.
set_recordatorio(): Configura un recordatorio con temporizador.