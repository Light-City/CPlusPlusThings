//
// Created by light on 19-12-12.
//
#include <stdio.h>
#include <string.h>
#include <malloc.h>
int main() {
    char *str = (char *) malloc(7);
    strcpy(str, "toptal");
    printf("char array = \"%s\" @ %u\n", str, str);

    str = (char *) realloc(str, 11);
    strcat(str, ".com");
    printf("char array = \"%s\" @ %u\n", str, str);

    free(str);

    return(0);
}