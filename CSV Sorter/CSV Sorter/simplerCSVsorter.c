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

int arraySize = 2000;

int currSize = 0;

const char *columnTitles[] = { "color", "director_name", "num_critic_for_reviews", "duration", "director_facebook_likes", "actor_3_facebook_likes", "actor_2_name", "actor_1_facebook_likes", "gross", "genres", "actor_1_name", "movie_title", "num_voted_users", "cast_total_facebook_likes", "actor_3_name", "facenumber_in_poster", "plot_keywords", "movie_imdb_link", "num_user_for_reviews", "language", "country", "content_rating", "budget", "title_year", "actor_2_facebook_likes", "imdb_score", "aspect_ratio", "movie_facebook_likes" };

int isNumeric = 0;

void checkNumeric(char *columnName) {
    if (strcmp(columnName,"duration") == 0) {
        isNumeric = 2;
        return;
    }
    for (int i = 0; i < 28; i++) {
        if (strcmp(columnName,columnTitles[i]) == 0) {
            printf("I: %d\n", i);
            for (int j = 0; j < 14; j++) {
                if (columnNumericIds[j] == i) {
                    isNumeric = 1;
                }
            }
            break;
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
    int i, j, k;
    int n1 = mid - low + 1;
    int n2 = high - mid;
    
    csventry L[n1];
    csventry R[n2];
    
    for (i = 0; i < n1; i++) {
        csventry check = entryArr[low + i];
        //L[i].data = malloc(sizeof(check.data));
        L[i].data = check.data;
    }
    
    for (j = 0; j < n2; j++) {
        csventry check = entryArr[mid + 1 + j];
        //R[j].data = malloc(sizeof(check.data));
        R[j].data = check.data;
    }
    
    i=0;
    j=0;
    k=low;
    while (i < n1 && j < n2) {
        if(numeric == 1){
            int one = atoi(L[i].data[colID]);
            int two = atoi(R[j].data[colID]);
            if (one <= two) {
                entryArr[k] = L[i];
                i++;
            } else {
                entryArr[k] = R[j];
                j++;
            }
        }
        else if(numeric == 0){
            char* one = L[i].data[colID];
            char* two = R[j].data[colID];
            if(cmpString(one, two) == 1){
                entryArr[k] = L[i];
                i++;
            }
            else{
                entryArr[k] = R[j];
                j++;
            }
        }
        k++;
    }
    while (i < n1) {
        entryArr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        entryArr[k] = R[j];
        j++;
        k++;
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
    
    entries = malloc(arraySize);
    char *buffer = NULL;
    size_t size;
    if (getline(&buffer, &size, stdin) == -1) {
        printf("No line\n");
        return 0;
    }
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
    checkNumeric(argv[2]);
    printf("Column: %s, Column Id: %d, isNumeric: %d\n", argv[2], columnId, isNumeric);
    for (int i = 0; i < getSize(columns->data[columnSize - 1]); i++) {
        //replace whitespaces at end of certain column names
        char c = columns->data[columnSize - 1][i];
        if (c == 13 || c == 10) {
            columns->data[columnSize - 1][i] = '\0';
        }
    }
    int rows = 0;
    while (getline(&buffer, &size, stdin) != -1)
    {
        int index = 0;
        actualSize = getSize(buffer);
        csventry entry;
        entry.data = malloc(actualSize * 2);
        currSize += actualSize * 2;
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
        if (currSize > arraySize) {
            arraySize *= 2;
            csventry *adjustSize = malloc(arraySize);
            memcpy(adjustSize, entries, arraySize / 2);
            free(entries);
            entries = adjustSize;
        }
        entries[rows].data = entry.data;
        //printf("Name: %s, rows: %s, Address: %p\n", entries[rows].data[1], entries[rows].data
               //[2], &entries[rows]);
        rows++;
    }
    
    for (int i = 0; i < rows; i++) {
        printf("Facebook likes: %s\n", entries[i].data[columnId]);
    }
    mergesorts(entries, 0, rows - 1, columnId, isNumeric);
    printf("------------\n");
    for (int i = 0; i < rows; i++) {
        printf("Facebook likes: %s\n", entries[i].data[columnId]);
    }
    free(cols);
    free(columns);
    return 0;
    
}
