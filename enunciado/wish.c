#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char *mypath[] = {
  "./",
  "/usr/bin/",
  "/bin/",
  NULL
};

int argCount(char *line){
    char *linetmp = strdup(line); // Creamos una copia de la cadena para no modificar la original
    char *token;
    int i = 0;

    // Usamos strsep para dividir la cadena en tokens utilizando el espacio como delimitador
    while ((token = strsep(&linetmp, " ")) != NULL) {
        if (*token != '\0') { // Ignoramos tokens vacíos
            i++;
        }
    }

    free(linetmp); // Liberamos la memoria de la copia de la cadena
    return i;
    
}

void wishPerLine(char *line){
    int counterArgs= argCount(line);

    char *arr[counterArgs+1];

    //Quitar el ultimo caracter y hacer que termine en null
    line[strcspn(line, "\n")] = '\0';

    char *palabra;
    char delimitador[] = " ";
    palabra = strtok(line, delimitador);
    int i = 0;
    while (palabra != NULL) {
        
        //Meterle al arreglo cada palabra
        arr[i] = palabra;
            
        palabra = strtok(NULL, delimitador);
        i++;

    }
    arr[i] = NULL;
    //* ------------TEST 1------------ No arguments are passed to cd.            DONE         
    if (strcmp(arr[0], "cd") == 0 && arr[1] == NULL) {
        fprintf(stderr, "An error has occurred\n");
        return;
    }

    //* ------------TEST 2------------ Two arg is passed to cd.                  DONE
    if (strcmp(arr[0], "cd") == 0 && arr[1] != NULL && arr[2] != NULL) {
        fprintf(stderr, "An error has occurred\n");
        return;
    }

    // ------------TEST 3/4 ------------ One arg is passed to cd and don´t exist.
    if (strcmp(arr[0], "ls") == 0 && arr[1] != NULL) {
        char *ruta = arr[1]; //ruta que deseamos verificar
        // Verificar si la ruta existe y si se puede acceder
        if (access(ruta, F_OK) != -1) {
            // ------------TEST 4------------ Input to run misc. commands.
            printf("La ruta %s existe.\n", ruta);
            //TODO: Crear proceso y cambiar de directorio
        } else {
            //* ------------TEST 3------------ ls with a bad directory name.        DONE
            fprintf(stderr, "ls: cannot access '/no/such/file': No such file or directory\n");
        }
    }

    //* ------------TEST 5------------ Exit with other argument.         DONE    
    if (strcmp(arr[0], "exit") == 0 && arr[1] != NULL) {
        fprintf(stderr, "An error has occurred\n");
        return;
    }


    // ------------TEST 6------------ Try run shield without path.

    //* ------------TEST 12------------ > without nothing before.         DONE
    if (strcmp(arr[0], ">") == 0 && arr[1] != NULL) {
        fprintf(stderr, "An error has occurred\n");
        return;
    }


      

    

}



int main(int argc, char const *argv[]) {
    
    char str[100];
    // Batch File, read text file and execute commands

    //Leer argumentos por consola
    if (argc == 1) {
        while (1) {
            // Wait for input 
            printf ("wish> ");
            //Recibir comando por consola
            fgets (str, sizeof(str), stdin);

            wishPerLine(str);

        }
        
        
    //Leer argumentos de archivo de texto
    } else if(argc == 2){
        FILE *file;
        file = fopen(argv[1], "r");
        //* ------------TEST 14------------ File not found.         DONE
        if (file == NULL) {
            fprintf(stderr, "An error has occurred\n");
            return 1;
        }
        char line[100];
        //While seeing all lines
        int a;
        while (fgets(line, sizeof(line), file)) {
            
            wishPerLine(line);

        }
        fclose(file);
        return 0;
    }

    /**
    while (1) {

        // Wait for input 
        printf ("wish> ");
        //Recibir comando por consola
        fgets (str, sizeof(str), stdin);
        //Quitar el ultimo caracter y hacer que termine en null
        str[strcspn(str, "\n")] = '\0';

        
        //inicializar un arreglo de strings
        char *arr[20];

        char *palabra;
        char delimitador[] = " ";
        palabra = strtok(str, delimitador);
        int i = 0;
        while (palabra != NULL) {
            
            //Meterle al arreglo cada palabra
            arr[i] = palabra;
            
            palabra = strtok(NULL, delimitador);
            printf("while: %s\n", arr[i]);
            i++;

        }
        arr[i] = NULL;

        

        // ------------TEST 1------------ No arguments are passed to cd.
        printf("%s\n", str);                     
        if (strcmp(arr[0], "cd") == 0 && arr[1] == NULL) {
            fprintf(stderr, "An error has occurred\n");
            exit(0);
        }

        // ------------TEST 2------------ Two arg is passed to cd.
        if (strcmp(arr[0], "cd") == 0 && arr[1] != NULL && arr[2] != NULL) {
            fprintf(stderr, "An error has occurred\n");
            return 0;
        }

        // ------------TEST 3/4 ------------ One arg is passed to cd and don´t exist.
        if (strcmp(arr[0], "cd") == 0 && arr[1] != NULL) {
            char *ruta = arr[1]; //ruta que deseamos verificar
            // Verificar si la ruta existe y si se puede acceder
            if (access(ruta, F_OK) != -1) {
                // ------------TEST 4------------ One arg is passed to cd and exist.
                printf("La ruta %s existe.\n", ruta);
                //TODO: Crear proceso y cambiar de directorio
            } else {
                // ------------TEST 3------------ One arg is passed to cd and don´t exist.
                fprintf(stderr, "ls: cannot access '/no/such/file': No such file or directory\n");
            }
        }

        // ------------TEST 5------------ Exit with other argument.
        if (strcmp(arr[0], "exit") == 0 && arr[1] != NULL) {
            printf("\n");
            fprintf(stderr, "An error has occurred\n");
            return 0;
        }


        // ------------TEST 6------------ Try run shield without path.



        






    }
    */

    return 0;
}


