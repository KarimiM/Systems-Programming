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
#include <time.h>
#include <sys/stat.h>
#include "simplerCSVsorter.h"

csventry *columns;
csventry *entries;


int columnSize = 0;

int columnNumericIds[] = {2, 4, 5, 7, 8, 12, 13, 15, 18, 22, 23, 24, 25, 26, 27};

int isNumeric = 0;

void checkNumeric(int columnId) {
    if (columnId == 3) {
        isNumeric = 2;
        return;
    }
    for (int i = 0; i < 14; i++) {
        if (columnNumericIds[i] == columnId) {
            isNumeric = 1;
            return;
        }
    }
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

int cmpString(char* str1, char* str2){
    int result = strcmp(str1, str2);
    if(result <= 0) return 1;
    else return 0;
}

int cmpInt(int firstInteger, int secondInteger){
    
    if(firstInteger <= secondInteger) return 1;
    else return 0;
}

//int cmpDateTime(DateTime firstDate, DateTime secondDate ){
//	return 0;
//}

void merge(csventry* entryArr, int low, int mid, int high, int colID, int numeric){
    csventry* mergeArr = malloc(sizeof(entryArr));
    int x = low;
    int y = mid + 1;
    int z = 0;
    
    while(x <= mid && y <= high) {
        if(numeric == 1){  //numeric
            if(cmpInt(atoi(entryArr[x].data[colID]), atoi(entryArr[y].data[colID])) == 1){
                mergeArr[z++] = entryArr[x++];
            }
            else mergeArr[z++] = entryArr[y++];
        }
        else if(numeric == 0){  //string
            if(cmpString(entryArr[x].data[colID], entryArr[y].data[colID])== 1){
                mergeArr[z++] = entryArr[x++];
            }
            else mergeArr[z++] = entryArr[y++];
        }
        else{
            return;
        }
        while(x <= mid){
            mergeArr[z++] = entryArr[x++];
        }
        while(y <= high){
            mergeArr[z++] = entryArr[y++];
        }
        while(z >= 0){
            entryArr[low + z] = mergeArr[z];
            z = z-1;
        }
    }
}

void mergesorts(csventry* entryArr, int low, int high, int colID, int numeric){
    if(low<high){
        int mid = (high+low)/2;
        mergesorts(entryArr, low, mid, colID, numeric);
        mergesorts(entryArr, mid+1, high, colID, numeric);
        merge(entryArr, low, mid, high, colID, numeric);
    }
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
    checkNumeric(columnId);
    printf("Column Id: %d, isNumeric: %d\n", columnId, isNumeric);
    for (int i = 0; i < getSize(columns->data[columnSize - 1]); i++) {
        char c = columns->data[columnSize - 1][i];
        if (c == 13 || c == 10) {
            columns->data[columnSize - 1][i] = '\0';
        }
    }
    
    printf("Test: %s, Column Size: %d\n", columns->data[3], columnSize);
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
            entry.data[index] = malloc(tokSize + 1);
            strcpy(entry.data[index], tok);
            entry.data[index][tokSize] = '\0';
            ind += tokSize + 1;
            tok = getFirstValue(buffer, ind);
            index++;
        }
        entries[rows].data = entry.data;
        //printf("Name: %s, rows: %s, Address: %p\n", entries[rows].data[1], entries[rows].data
               //[2], &entries[rows]);
        rows++;
    }
    
    mergesorts(entries, 0, rows - 1, columnId, isNumeric);
    free(cols);
    free(columns);
    return 0;
    
}
