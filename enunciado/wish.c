#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef struct PathNode
{
    char* path;
    struct PathNode* next;
} PathNode;

PathNode* mypath;

PathNode* createList(){

    PathNode* node1 = (PathNode*)malloc(sizeof(PathNode));
    node1->path= "./";
    
    PathNode* node2 = (PathNode*)malloc(sizeof(PathNode));
    node2->path= "/usr/bin/";
    
    
    PathNode* node3 = (PathNode*)malloc(sizeof(PathNode));
    node3->path= "/bin/";

    node1->next = node2;
    node2->next = node3;
    node3->next = NULL;

    return node1;

}


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

// Encontrar si el comand
char* isInPath(char* command) {
    char ruta[128];
    PathNode *head = mypath;

    while (head != NULL) {
        
        strcpy(ruta, head->path);
        strcat(ruta, command);
        if(access(ruta, X_OK) != -1){
            char *rutaFinal = malloc(strlen(ruta)+1);
            strcpy(rutaFinal, ruta);
            return rutaFinal;
        }
        head = head->next;
    }

    return NULL;
}


void newProcess(char* path, char* arr[]){
    int pid;
    pid = fork();
    //Proceso hijo
    if (pid == 0) {
        execv(path, arr);
        if (execv(path, arr) == -1) {
            fprintf(stderr, "An error has occurred\n");
            exit(0);
        }
    //Proceso Padre
    } else {
        wait(NULL);
    }
}

void cd(char* arr[]) {

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


    // ------------TEST 4------------ CD  Run.
        
    int ch = chdir(arr[1]);

    if(ch == -1) {
        printf("No se pudo acceder\n");
    }

    // char cwd[1024];
    // if (getcwd(cwd, sizeof(cwd)) != NULL) {
    //     printf("Directorio actual: %s\n", cwd);
    // } else {
    //     perror("getcwd");
    //     return;
    // }

}

// ------------TEST 7------------ Change of Path.


void setPath(char *arr[]) {

    PathNode* aux = NULL;
    
    if(arr[1] == NULL){
        mypath = NULL;
        return;
    }

    int i = 1;
    while (arr[i] != NULL)
    {
        
        PathNode* newNode = (PathNode*)malloc(sizeof(PathNode));

        char *copy = (char *)malloc(100 * sizeof(char));
        strcpy(copy, "./");
        strcat(copy, arr[i]);
        strcat(copy, "/");
        
        // Meto informacion
        newNode->path = copy;
        newNode->next = NULL;

        //Si es el primer nodo
        if (i == 1) {
            mypath = newNode;
            aux = newNode;
            i++;
            continue;
        }
        

        // Segunda o más
        aux->next = newNode;
        aux = newNode;

        i++;
    }
    
}

void printPath() {
    PathNode *head = mypath;

    while (head != NULL) {
        printf("%s\n", head->path);
        head = head->next;
    }
}


int searchChar(char *arr[]){
    for (int i = 0; arr[i] != NULL; i++)
    {
        if (strcmp(arr[i], ">") == 0){
            return i;

        }
    }
    return -1;
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

    if (strcmp(arr[0], "cd") == 0) {
        cd(arr);
        return;
    }

    if(strcmp(arr[0], "path") == 0){
        
        setPath(arr);
        return;
    }

    /*------------- TEST 8 ---------- Redirection with no output file specified.*/
    int pos = searchChar(arr);
    if (pos != -1 && arr[pos + 1] == NULL){
        fprintf(stderr, "An error has occurred\n");
        return;
    }

    
    if (strcmp(arr[0], "ls") == 0 && arr[1] != NULL && strcmp(arr[1], ">") != 0) {
        char *ruta = arr[1]; //ruta que deseamos verificar
        // Verificar si la ruta existe y si se puede acceder
        if (access(ruta, F_OK) == -1) {
            //* ------------TEST 3------------ ls with a bad directory name.        DONE
            fprintf(stderr, "ls: cannot access '%s': No such file or directory\n", ruta);
            return;
        } 
    }

    //* ------------TEST 5------------ Exit with other argument.         DONE    
    if (strcmp(arr[0], "exit") == 0 && arr[1] != NULL) {
        fprintf(stderr, "An error has occurred\n");
        return;
    } else if (strcmp(arr[0], "exit") == 0) {
        exit(0);
    } 

    //* ------------TEST 12------------ > without nothing before.         DONE
    if (strcmp(arr[0], ">") == 0 && arr[1] != NULL) {
        fprintf(stderr, "An error has occurred\n");
        return;
    }

    

    //Ejecucion de comandos
    char* path = isInPath(arr[0]);   
    
    if (path == NULL) {
        fprintf(stderr, "An error has occurred\n");
        return;
    } else {
        newProcess(path, arr);
    }
    

}



int main(int argc, char const *argv[]) {
    
    char str[100];
    mypath = createList();

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
            
            if (strchr(line, '&')) {
                printf("comando con &\n");
                parallelCommands(line);
                continue;
            }

            wishPerLine(line);

        }
        fclose(file);
        return 0;
    }

    
    

    return 0;
}


