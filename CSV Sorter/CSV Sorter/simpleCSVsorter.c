
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>

pid_t arr[255];

int * currIndex;
int status;
int original;

void addPidToArray(pid_t pid) {
    int index = *currIndex;
    arr[index] = pid;
    *currIndex = index+1;
}

void traverseDirectory(char * directory) {
    int childCount=0;
    int * childIds=(int *)malloc(sizeof(int)*255);
    int childProcesses = 0;
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
        char * check = strchr(de->d_name, '.');
        char * absolute_path = malloc(strlen(directory) + strlen(de->d_name) + 2);
        strcpy(absolute_path, directory);
        strcat(absolute_path, "/");
        strcat(absolute_path, de->d_name);
        if (opendir(absolute_path) != NULL) {
            pid_t pid = fork();
            if (pid == 0) {
                printf("Directories: %s\n", absolute_path);
                traverseDirectory(absolute_path);
                _exit(2);
            } else {
                printf("Adding pid: %d\n", pid);
                childIds[childCount] = pid;
                childCount++;
                wait(&status);
            }
        }
    }
    closedir(dr);
    // _exit(0);
    int i;
    for (i=0;i<childCount;i++){
        printf("Pid: %d\n", childIds[i]);
        waitpid(childIds[i],&status,0);
        if (WIFEXITED(status)){
            childProcesses+=WEXITSTATUS(status);
        }else{
            printf("L\n");
        }
    }
    if(getpid()==original){
        printf("\nWe done.");
        //childProcesses=childProcesses>>8;
        printf("\nTotal number of processes: %d\n",(1+childProcesses));
    }
    exit(1+childProcesses);
}

int main(void)
{
    currIndex = malloc(sizeof(int*));
    *currIndex = 0;
    char *directory = "/ilab/users/mkk102/jay shah";
    struct dirent *de;  // Pointer for directory entry
    
    // opendir() returns a pointer of DIR type.
    DIR *dr = opendir(directory);
    int count = 0;
    original = getpid();
    if (dr == NULL)   // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory" );
        return 0;
    }
    traverseDirectory(directory);
    /* while ((de = readdir(dr)) != NULL) {
     struct stat path_stat;
     stat(de->d_name, &path_stat);
     if (strcmp(de->d_name, "..") == 0 || strcmp(de->d_name, ".") == 0) {
     continue;
     }
     char * check = strchr(de->d_name, '.');
     char * absolute_path = malloc(strlen(directory) + strlen(de->d_name) + 1);
     strcpy(absolute_path, directory);
     strcat(absolute_path, de->d_name);
     if (opendir(absolute_path) != NULL) {
     printf("Directories: %s\n", absolute_path);
     traverseDirectory(absolute_path);
     }
     }*/
    closedir(dr);
    
    
    if(getpid()!=original){
        printf("Exiting process.\n");
        _exit(2);
    } else {
        wait(&status);
    }
    printf("did i get here\n");
    int i;
    
    for (i = 0; i < *currIndex; i++) {
        printf("PID: %d\n", arr[i]);
    }
    return 0;
}
