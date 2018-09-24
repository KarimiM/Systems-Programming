//
//  simplerCSVsorter.c
//  CSV Sorter
//
//  Created by Masood Karimi on 9/24/18.
//  Copyright © 2018 Masood Karimi. All rights reserved.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "simplerCSVsorter.h"

csventry *columns;
csventry *entries;


int columnSize = 0;

long int findSize(char file_name[])
{
    // opening the file in read mode
    FILE* fp = fopen(file_name, "r");
    
    // checking if the file exist or not
    if (fp == NULL) {
        printf("File Not Found!\n");
        return -1;
    }
    
    fseek(fp, 0L, SEEK_END);
    
    // calculating the size of the file
    long int res = ftell(fp);
    
    // closing the file
    fclose(fp);
    
    return res;
}
int getSize(char *array)
{
    int size = 0;
    for (int i = 0; array[i] != '\0'; i++) {
        size++;
    }
    return size;
}


int checkForColumn(char* check)
{
    int sortColumn = 0;
    for(int i = 0; i < columnSize; i++){
        if(i == columnSize - 1 && strcmp(columns->data[i],check) != 0){
            return -1;
        }
        if(strcmp(columns->data[i],check) == 0){
            sortColumn = i;
            break;
        }
    }
    return sortColumn;
}

char* getFirstValue(char* entry, int startIndex)
{
    int endIndex = getSize(entry);
    for (int i = startIndex; i < endIndex; i++)
    {
        if (entry[i] == ',') {
            char* value = malloc(i - startIndex + 1);
            if (i - startIndex == 0) {
                value[0] = '\0';
                return value;
            }
            memcpy(value, &entry[startIndex], i - startIndex);
            value[i-startIndex] = '\0';
            return value;
        }
    }
    return NULL;
}

int main(int varc, char* argv[])
{
    char buffer[2500];
    long int size = findSize("/Users/masoodkarimi/Documents/GitHub/Systems-Programming/CSV Sorter/CSV Sorter/movie_metadata.csv");
    FILE* data = fopen("/Users/masoodkarimi/Documents/GitHub/Systems-Programming/CSV Sorter/CSV Sorter/movie_metadata.csv", "r");
    
    
    entries = malloc(size * 2);
    fgets(buffer, 2500, data);
    
    int actualSize = getSize(buffer);
    char** cols = malloc(actualSize * 2);
    int currIndex = 0;
    char* token = getFirstValue(buffer, currIndex);
    while(token != NULL) {
        cols[columnSize] = token;
        currIndex += getSize(token) + 1;
        token = getFirstValue(buffer, currIndex);
        columnSize++;
    }

    columns = malloc(sizeof(cols));
    columns->data = cols;
    int columnId = checkForColumn(argv[2]);
    if (columnId == -1) {
        printf("ERROR: Column in argument does not exist.");
        return 1;
    }
    for (int i = 0; i < getSize(columns->data[columnSize - 1]); i++) {
        char c = columns->data[columnSize - 1][i];
        if (c == 13 || c == 10) {
            columns->data[columnSize - 1][i] = '\0';
        }
    }
    
    printf("Test: %s, Column Size: %d\n", columns->data[27], columnSize);
    int rows = 0;
    while (fgets(buffer, 2500, data) != NULL)
    {
        int index = 0;
        actualSize = getSize(buffer);
        csventry entry;
        entry.data = malloc(actualSize * 2);
        int ind = 0;
        char* tok = getFirstValue(buffer, ind);
        while(tok != NULL) {
            int tokSize = getSize(tok);
            entry.data[index] = malloc(tokSize);
            strcpy(entry.data[index], tok);
            ind += tokSize + 1;
            tok = getFirstValue(buffer, ind);
            index++;
        }
        entries[rows].data = entry.data;
        printf("Name: %s, rows: %d, Address: %p\n", entries[rows].data[1], rows, &entries[rows]);
        rows++;
    }
    
    
    free(cols);
    free(columns);
    return 0;
    
}
