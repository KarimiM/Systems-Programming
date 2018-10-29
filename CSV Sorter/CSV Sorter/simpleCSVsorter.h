//
//  simplerCSVsorter.h
//  CSV Sorter
//
//  Created by Masood Karimi on 9/24/18.
//  Copyright © 2018 Masood Karimi. All rights reserved.
//

#ifndef simpleCSVsorter_h
#define simpleCSVsorter_h


typedef struct csventry {
    char** data;
    int originalIndex;
    
} csventry;

int sort(char * fileDirectory, char * fileName, char * field, char * outputDirectory);

#endif /* simpleCSVsorter_h */
