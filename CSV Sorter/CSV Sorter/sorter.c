
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<limits.h>
#include "simpleCSVsorter.h"
int status;
int original;

void traverseDirectory(char * directory, char * field, char * outputDirectory) {
    int index=0;
    int processes = 0;
    struct dirent *de;
    DIR *dr = opendir(directory);
    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("NULL");
        return;
    }
    
    while ((de = readdir(dr)) != NULL) {
        struct stat path_stat;
        stat(de->d_name, &path_stat);
        if (strcmp(de->d_name, "..") == 0 || strcmp(de->d_name, ".") == 0) {
            continue;
        }
        
        char * absolute_path = malloc(strlen(directory) + strlen(de->d_name) + 2);
        strcpy(absolute_path, directory);
        strcat(absolute_path, "/");
        strcat(absolute_path, de->d_name);
        if (opendir(absolute_path) != NULL) {
            pid_t pid = fork();
            if (pid == 0) {
                printf("%d\n", getpid());
                //printf("Directories: %s\n", absolute_path);
                traverseDirectory(absolute_path, field, outputDirectory);
                _exit(1);
            } else {
                waitpid(pid, &status, 0);
                if (WIFEXITED(status)) {
                    processes += WEXITSTATUS(status);
                }
            }
        } else {
            pid_t pid = fork();
            if (pid == 0) {
                
                printf("%d\n", getpid());
                if(strstr(de->d_name, ".csv") != NULL && strstr(de->d_name, "sorted") == NULL) {
                    int val = sort(absolute_path, de->d_name, field, outputDirectory == NULL ? directory : outputDirectory);
                    _exit(1);
                } else {
                    _exit(1);
                }
            } else {
                waitpid(pid, &status, 0);
                if (WIFEXITED(status)) {
                    processes += WEXITSTATUS(status);
                }
            }
            
        }
    }
    closedir(dr);
    // _exit(0);
    
    
    if(getpid()==original){
        
        printf("\nTotal number of processes: %d\n",(processes));
    }
    exit(1 + processes);
    
}

int main(int argc, char* argv[])
{
    char cwd[PATH_MAX];
    
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        printf("ERROR: Current Directory throwing a cwd error.");
        exit(1);
    }
    char * directory = cwd;
    char * outputDirectory = NULL;
    if(argc < 3 || argc >7 || argc == 2 || argc == 4 || argc == 6){
        printf("ERROR: Incorrect imput, please enter valid number of arguments, %d\n", argc);
        return 1;
    }
    
    if(argc == 3){
        if(strcmp(argv[1], "-c") != 0){
            printf("Error: Incorrect input, missing \"-c\"\n");
            return 1;
        }
    }
    
    if(argc == 5){
        if(strcmp(argv[3],"-d") != 0){
            printf("Error: Incorrect input\n");
            return 1;
        }
        directory = malloc(strlen(argv[4]));
        strcpy(directory, argv[4]);
    }
    
    if(argc == 7){
        if(strcmp(argv[5], "-o") != 0){
            printf("Error: Incorrect input, missing \"-o\"\n");
            return 1;
        }
        outputDirectory = malloc(strlen(argv[6]));
        strcpy(outputDirectory, argv[6]);
    }
    original = getpid();
    printf("PIDS: \n");
    traverseDirectory(directory, argv[2], outputDirectory);
    
    
    return 0;
}
