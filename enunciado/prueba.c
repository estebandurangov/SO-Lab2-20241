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
            //fprintf(stderr, "An error has occurred\n");
            exit(0);
        }
    //Proceso Padre
    } else {
        wait(NULL);
    }
}

void newProcessWithFile(char* path, char* arr[], char *archivoSalida){
    
    int pid;
    pid = fork();
    //Proceso hijo
    if (pid == 0) {

        freopen(archivoSalida, "w", stdout);

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
        fprintf(stderr, "An error has occurred1\n");
        return;
    }

    //* ------------TEST 2------------ Two arg is passed to cd.                  DONE
    if (strcmp(arr[0], "cd") == 0 && arr[1] != NULL && arr[2] != NULL) {
        fprintf(stderr, "An error has occurred2\n");
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

int searchMultipleChar(char *arr[]){
    int j = 0;
    for (int i = 0; arr[i] != NULL; i++)
    {
        if (strcmp(arr[i], ">") == 0){
            j++;
        }
    }
    return j;
    
}

int searchCharFromString(char str[]){
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '&') {
            return 1; //retorna 1 si el caracter está.
        }
    }
    return 0;
}

void parallelCommands(char *line){

    //Quitar el ultimo caracter y hacer que termine en null
    line[strcspn(line, "\n")] = '\0';

    char *newLine;
    char delimitador[] = "&";
    newLine = strtok(line, delimitador);
    newLine[strcspn(newLine, "\n")] = '\0';
    while (newLine != NULL) {
        printf("newLine: %s\n", newLine);
        wishPerLine(newLine);
        newLine = strtok(NULL, delimitador);
    }
    
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
    
    //* ------------TEST 16------------ 
    if(strcmp(arr[0], "&") == 0 && arr[1] == NULL){
        return;
    }

    if (strcmp(arr[0], "cd") == 0) {
        cd(arr);
        return;
    }

    if(strcmp(arr[0], "path") == 0){
        
        setPath(arr);
        return;
    }

    // ------------TEST 10------------ Multiple  Más de un >      
    int multi = searchMultipleChar(arr);
    if (multi > 1){
        fprintf(stderr, "An error has occurred10\n");

        return;
    }




    //*------------- TEST 8 ---------- Redirection with no output file specified.
    int pos = searchChar(arr);
    
    if (pos != -1 ){
        if (arr[pos + 1] == NULL)
        {
            fprintf(stderr, "An error has occurred8\n");
            return;
        } 

        //* ------------TEST 12------------ > without nothing before.         DONE 
        if (strcmp(arr[0], ">") == 0 && arr[1] != NULL) {
            fprintf(stderr, "An error has occurred12\n");
            return;
        }

        //*------------- TEST 9 ----------  Redirection with multiple output files
        if (((counterArgs-1) - pos) > 1 ){
            fprintf(stderr, "An error has occurred9\n");
            return;
        }

        // ------------TEST 11------------ Normal redirection> 

        char* path = isInPath(arr[0]);   
        // ------------TEST 6------------ without path         DONE
        if (path == NULL) {
            fprintf(stderr, "An error has occurred6\n");
            return;
        } else {
            arr[pos] = NULL;
            newProcessWithFile(path, arr, arr[pos + 1]);
            return;
        }

        
    } 
    

    
    if (strcmp(arr[0], "ls") == 0 && arr[1] != NULL && pos != -1) {
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
        fprintf(stderr, "An error has occurred5\n");
        return;
    } else if (strcmp(arr[0], "exit") == 0) {
        exit(0);
    } 

    

    

    //Ejecucion de comandos
    char* path = isInPath(arr[0]);   
    // ------------TEST 6------------ without path         DONE
    if (path == NULL) {
        fprintf(stderr, "An error has occurred6\n");
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
    } else if(argc >= 2){
        FILE *file;
        char line[100];
        file = fopen(argv[1], "r");
        //* ------------TEST 14------------ File not found.         DONE
        if (file == NULL) {
            fprintf(stderr, "An error has occurred14\n");
            return 1;
        }

        //* ------------TEST 13------------     Input file is valid and empty. But shell is invoked in batch mode with 2 files (with same file 13.in used twice)*
        if (fgets(line, sizeof(line), file) == NULL ) {
            fprintf(stderr, "An error has occurred13\n");
            exit (1);
        }
   
        //While seeing all lines
        
        do  {

            int aux = searchCharFromString(line);
            
            if (aux == 1) {
                
                parallelCommands(line);
                
                continue;
            }

            wishPerLine(line);
            
        } while (fgets(line, sizeof(line), file));
        fclose(file);
        return 0;
    }

    return 0;
}


