//
//  mergesort.c
//  CSV Sorter
//
//  Created by Masood Karimi on 9/24/18.
//  Copyright © 2018 Masood Karimi. All rights reserved.
//

#include <stdio.h>
#include <date.h>

int cmpString(char* str1, char* str2){

}

int cmpInt(int firstInteger, int secondInteger){

}

int cmpDateTime(DateTime firstDate, DateTime secondDate ){
	
}

void merge(csvEntry* entryArr, int low, int mid, int high){

}

void mergesort(csvEntry* entryArr, int low, int mid, int high){
	if(low<high){
		int mid = (high+low)/2;
		mergesort(entryArr, low, mid);
		mergesort(entryArr, mid+1, high);
		merge(entryArr, low, mid, high);
	}

}
