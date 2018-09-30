//
//  test.c
//  CSV Sorter
//
//  Created by Masood Karimi on 9/30/18.
//  Copyright © 2018 Masood Karimi. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int varc, char* argv[])
{
    char *line = NULL;
    size_t size;
    if (getline(&line, &size, stdin) == -1) {
        printf("No line\n");
    } else {
        printf("%s\n", line);
    }
    return 0;
}

