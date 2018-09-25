//
//  simplerCSVsorter.h
//  CSV Sorter
//
//  Created by Masood Karimi on 9/24/18.
//  Copyright © 2018 Masood Karimi. All rights reserved.
//

#ifndef simplerCSVsorter_h
#define simplerCSVsorter_h

#include <time.h>

#include <stdio.h>

typedef struct csventry {
    char** data;
    
} csventry;
#endif /* simplerCSVsorter_h */


int cmpString(char* str1, char* str2){
	int result = strcmp(str1, str2);
	if(result <= 0) return 1;
	else return 0;
}

int cmpInt(int firstInteger, int secondInteger){
	
	if(firstInteger <= secondInteger) return 1;
	else return 0;
}

int cmpDateTime(DateTime firstDate, DateTime secondDate ){
	return 0;
}

void merge(csvEntry* entryArr, int low, int mid, int high, int colID){
	csventry* mergeArr = malloc(sizeof(entryArr));
	int x = low;
	int y = mid + 1;
	int z = 0;

	while(x <= mid && y <= high) {
		if(numeric == 1){  //numeric
			if(cmpInt(entryArr[x].data[colID], entryArr[y].data[colID]) == 1){
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
			return 0;
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

void mergesort(csvEntry* entryArr, int low, int mid, int high, int colID){
	if(low<high){
		int mid = (high+low)/2;
		mergesort(entryArr, low, mid);
		mergesort(entryArr, mid+1, high);
		merge(entryArr, low, mid, high, colID);
	}

}
