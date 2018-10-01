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

typedef struct csventry {
    char** data;
    int originalIndex;
    
} csventry;



char* getFirstValue(char* entry, int startIndex)
{
    int endIndex = getSize(entry);
    int i;
    for (i = startIndex; i <= endIndex - 1; i++)
    {
        if (i == endIndex - 2) {
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
