#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <command>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct timeval start, end;
    int status;

    // Obtiene el tiempo de inicio
    gettimeofday(&start, NULL);

    // Crea un nuevo proceso
    int pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Error al hacer fork()\n");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Proceso hijo
        execvp(argv[1], &argv[1]);
        // Si execvp retorna, ha habido un error
        fprintf(stderr, "Error al ejecutar el comando %s\n", argv[1]);
        
    } else {
        //El proceso padre espera a que el proceso hijo termine
        wait(&status);

        // Obtiene el tiempo de finalización
        gettimeofday(&end, NULL);

        // Calcula el tiempo transcurrido en segundos y microsegundos
        double segundos_transcurridos = end.tv_sec - start.tv_sec;
        double ms_transcurridos = end.tv_usec - start.tv_usec;

        // Calcula el tiempo total en segundos
        double total_time = segundos_transcurridos + ms_transcurridos / 1000000;

        // Imprime el tiempo transcurrido
        printf("Elapsed time: %.5f\n", total_time);
    }

    return 0;
}