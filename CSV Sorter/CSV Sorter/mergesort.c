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
    int n1 = mid - low + 1;
    int n2 = high - mid;
    
    csventry L[n1];
    csventry R[n2];
    
    for (i = 0; i < n1; i++) {
        csventry check = entryArr[low + i];
        //L[i].data = malloc(sizeof(check.data));
        L[i].data = check.data;
        L[i].originalIndex = check.originalIndex;
    }
    
    for (j = 0; j < n2; j++) {
        csventry check = entryArr[mid + 1 + j];
        //R[j].data = malloc(sizeof(check.data));
        R[j].data = check.data;
        R[j].originalIndex = check.originalIndex;
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
