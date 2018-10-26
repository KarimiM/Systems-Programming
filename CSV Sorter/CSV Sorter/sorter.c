
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>

int status;
int original;

void traverseDirectory(char * directory) {
    int index=0;
    int * children=(int *)malloc(sizeof(int)*255);
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
                printf("Directories: %s\n", absolute_path);
                traverseDirectory(absolute_path);
                _exit(2);
            } else {
                children[index++] = pid;
                wait(&status);
            }
        } else {
            if(strstr(de->d_name, ".csv") != NULL) {
                printf("Found csv file!\n");
            }
        }
    }
    closedir(dr);
    // _exit(0);
    int i;
    for (i=0;i<index;i++){
        waitpid(children[i],&status,0);
        if (WIFEXITED(status)){
            processes+=WEXITSTATUS(status);
        }
    }
    if(getpid()==original){
        printf("\nTotal number of processes: %d\n",(1+processes));
    }
    exit(1+processes);
}

int main(void)
{
    char *directory = "/ilab/users/mkk102/jay shah";
    int count = 0;
    original = getpid();
    traverseDirectory(directory);
    
    
    return 0;
}
