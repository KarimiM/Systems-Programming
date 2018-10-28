
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
#include "simpleCSVsorter.h"
#include "mergesort.c"

const char *columnTitles[] = { "color", "director_name", "num_critic_for_reviews", "duration", "director_facebook_likes", "actor_3_facebook_likes", "actor_2_name", "actor_1_facebook_likes", "gross", "genres", "actor_1_name", "movie_title", "num_voted_users", "cast_total_facebook_likes", "actor_3_name", "facenumber_in_poster", "plot_keywords", "movie_imdb_link", "num_user_for_reviews", "language", "country", "content_rating", "budget", "title_year", "actor_2_facebook_likes", "imdb_score", "aspect_ratio", "movie_facebook_likes" };


const int columnNumericIds[] = {2, 4, 5, 7, 8, 12, 13, 15, 18, 22, 23, 24, 25, 26, 27};

void checkNumeric(char *columnName, int * isNumeric) {
    if (strcmp(columnName,"duration") == 0 || strcmp(columnName, "movie_facebook_likes") == 0) {
        *isNumeric = 1;
        return;
    }
    int i;
    for (i = 0; i < 28; i++) {
        if (strcmp(columnName,columnTitles[i]) == 0) {
            
            int j;
            for (j = 0; j < 14; j++) {
                if (columnNumericIds[j] == i) {
                    *isNumeric = 1;
                }
            }
            break;
        }
    }
}

int checkForColumn(char* check, int columnSize, csventry *columns)
{
    int i;
    int exists = 0;
    for (i = 0; i < 28; i++) {
        if (strcmp(check, columnTitles[i]) == 0) {
            exists = 1;
            break;
        }
    }
    if (exists == 0) {
        return -1;
    }
    for(i = 0; i < columnSize; i++){
        printf("Column: %s\n", columns->data[i]);
        if(strcmp(trimwhitespace(columns->data[i]),check) == 0){
            return i;
        }
    }
    return -1;
}

void printCSV(char * fileName, char * outputDirectory, char * field, csventry* entries, int rows, csventry* columnArr, char** originalData){
    char add[] = "sorted";
    char * ptr = fileName;
    int i;
    for (i = 0; i < strlen(fileName); i++) {
        if (fileName[i] == '.') {
            fileName[i] = '\0';
            break;
        }
    }
    char * newFile = malloc(strlen(fileName) + strlen(add) + strlen(field) + 7);
    
    strcpy(newFile, fileName);
    strcat(newFile, "-");
    strcat(newFile, add);
    strcat(newFile, "-");
    strcat(newFile, field);
    strcat(newFile, ".csv");
    
    char * absolute_path = malloc(strlen(outputDirectory) + strlen(newFile) + 2);
    strcpy(absolute_path, outputDirectory);
    strcat(absolute_path, newFile);
    FILE * fp;
    fp = fopen(absolute_path, "w");
    if (fp == NULL) {
        puts("FP IS NULL");
    }
    fprintf(fp, "%s", originalData[0]);
    for (i = 0; i < rows; i++) {
        int index = entries[i].originalIndex;
        fprintf(fp, "%s", originalData[index + 1]);
    }
    fclose(fp);
}

int sort(char * fileDirectory, char * fileName, char * field, char * outputDirectory)
{
    FILE * fp;
    
    csventry *columns;
    
    csventry *entries;
    
    char** originalData;
    
    int dataSize = 1000;
    
    int dataIndex = 0;
    
    int columnSize = 0;
    
    int arraySize = 1000;
    
    int currSize = 0;
    
    int * isNumeric = calloc(1, sizeof(int*));
    fp = fopen(fileDirectory, "r");
    if (fp == NULL) {
        fprintf( stderr, "Error, CSV File cant open!\n");
        //puts("ERROR, CSV File cant open!");
        return 1;
    }
    originalData = malloc(dataSize * sizeof(char*));
    entries = malloc(arraySize);
    char *buffer = NULL;
    size_t size;
    if (getline(&buffer, &size, fp) == -1) {
        fprintf(stderr, "No input\n");
        //puts("No input\n");
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
    char* bufferCopy = malloc(size);
    strncpy(bufferCopy, buffer, size);
    originalData[dataIndex++] = bufferCopy;
    columns = malloc(sizeof(cols));
    columns->data = cols;
    int columnId = checkForColumn(field, columnSize, columns);
    if (columnId == -1) {
        fprintf(stderr, "ERROR: Column in argument does not exist."\n);
        //puts("ERROR: Column in argument does not exist.");
        return 1;
    }
    checkNumeric(field, isNumeric);
    int i;
    for (i = 0; i < getSize(columns->data[columnSize - 1]); i++) {
        //replace whitespaces at end of certain column names
        char c = columns->data[columnSize - 1][i];
        if (c == 13 || c == 10) {
            columns->data[columnSize - 1][i] = '\0';
        }
    }
    int rows = 0;
    while (getline(&buffer, &size, fp) != -1)
    {
        char* buffCopy = malloc(size);
        strncpy(buffCopy, buffer, size);
        if (dataIndex >= dataSize - 100) {
            dataSize *= 2;
            originalData = realloc(originalData, dataSize * sizeof(char*));
        }
        originalData[dataIndex++] = buffCopy;
        int index = 0;
        actualSize = getSize(buffer);
        csventry entry;
        entry.data = malloc(actualSize * 2);
        currSize += actualSize * 2;
        int ind = 0;
        int columnCount = 0;
        char* tok = getFirstValue(buffer, ind);
        while(tok != NULL) {
            columnCount++;
            int tokSize = getSize(tok);
            entry.data[index] = malloc(tokSize + 1);
            strcpy(entry.data[index], tok);
            entry.data[index][tokSize] = '\0';
            ind += tokSize + 1;
            tok = getFirstValue(buffer, ind);
            index++;
        }
        if (columnCount != columnSize) {
            fprintf(stderr, "Error, not enough columns");
            //puts("ERROR:");
            //Data column size (%d) does not match data header size (%d).\n", columnCount, columnSize);
            exit(1);
            
        }
        if (currSize > arraySize) {
            arraySize *= 2;
            csventry *adjustSize = malloc(arraySize);
            memcpy(adjustSize, entries, arraySize / 2);
            free(entries);
            entries = adjustSize;
        }
        entries[rows].data = entry.data;
        entries[rows].originalIndex = rows;
        rows++;
    }
    mergesorts(entries, 0, rows - 1, columnId, isNumeric);
    printCSV(fileName, outputDirectory, field, entries, rows, columns, originalData);
    free(cols);
    free(columns);
    free(entries);
    free(isNumeric);
    return 0;
    
}
