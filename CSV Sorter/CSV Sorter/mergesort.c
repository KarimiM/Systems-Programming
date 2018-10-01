#include <stdio.h>
#include <stdlib.h>
#include "simpleCSVsorter.h"
//#include <string.h>


int cmpString(char* str1, char* str2){
    int result = strcmp(str1, str2);
    if(result <= 0) return 1;
    else return 0;
}

int cmpInt(int firstInteger, int secondInteger){
    
    if(firstInteger <= secondInteger) return 1;
    else return 0;
}

void merge(csventry* entryArr, int low, int mid, int high, int colID, int numeric){
    int i, j, k;
    int leftSize = mid - low + 1;
    int rightSize = high - mid;
    
    csventry left[n1];
    csventry right[n2];
    
    for (i = 0; i < leftSize; i++) {
        csventry check = entryArr[low + i];
        //L[i].data = malloc(sizeof(check.data));
        left[i].data = check.data;
        left[i].originalIndex = check.originalIndex;
    }
    
    for (j = 0; j < rightSize; j++) {
        csventry check = entryArr[mid + 1 + j];
        //R[j].data = malloc(sizeof(check.data));
        right[j].data = check.data;
        right[j].originalIndex = check.originalIndex;
    }
    
    i=0;
    j=0;
    k=low;
    while (i < leftSize && j < rightSize) {
        if(numeric == 1){
            int one = atoi(left[i].data[colID]);
            int two = atoi(right[j].data[colID]);
            if (one <= two) {
                entryArr[k] = left[i];
                i++;
            } else {
                entryArr[k] = right[j];
                j++;
            }
        }
        else if(numeric == 0){
            char* one = left[i].data[colID];
            char* two = right[j].data[colID];
            if(cmpString(one, two) == 1){
                entryArr[k] = left[i];
                i++;
            }
            else{
                entryArr[k] = right[j];
                j++;
            }
        }
        k++;
    }
    while (i < leftSize) {
        entryArr[k] = left[i];
        i++;
        k++;
    }
    while (j < rightSize) {
        entryArr[k] = right[j];
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
