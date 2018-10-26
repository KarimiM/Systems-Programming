//
//  simplerCSVsorter.h
//  CSV Sorter
//
//  Created by Masood Karimi on 9/24/18.
//  Copyright © 2018 Masood Karimi. All rights reserved.
//

#ifndef simplerCSVsorter_h
#define simplerCSVsorter_h

#include <stdio.h>
#include <ctype.h>

typedef struct csventry {
    char** data;
    int originalIndex;
    
} csventry;



int getSize(char *array)
{
    int size = 0;
    int i;
    for (i = 0; array[i] != '\0'; i++) {
        size++;
    }
    return size;
}

char *trimwhitespace(char *str)
{
    char *end;
    
    // Trim leading space
    while(isspace((unsigned char)*str)) str++;
    
    if(*str == 0)  // All spaces?
        return str;
    
    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    
    // Write new null terminator character
    end[1] = '\0';
    
    return str;
}

char* getFirstValue(char* entry, int startIndex)
{
    int endIndex = getSize(entry);
    int i;
    for (i = startIndex; i <= endIndex - 1; i++)
    {
        if (i == endIndex - 1) {
            char* value = malloc(i - startIndex);
            memcpy(value, &entry[startIndex], i - startIndex);
            value[i-startIndex] = '\0';
            return value;
            
        }
        if (entry[i] == ',' ) {
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

//#include "mergesort.c"
//int cmpInt(int firstInteger, int secondInteger);
//int cmpString(char* str1, char* str2);
//void merge(csventry* entryArr, int low, int mid, int high, int colID, int numeric);
//void mergesorts(csventry* entryArr, int low, int high, int colID, int numeric);
#endif /* simplerCSVsorter_h */
