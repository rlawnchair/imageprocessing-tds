#include <stdio.h>
#include <stdlib.h>
#include <pnm.h>

#define PARAM 2

void usage(char *s){
    printf("Usage : %s <ims> <imd>\n",s);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    if(argc != PARAM+1)
        usage(argv[0]);

    return EXIT_SUCCESS;
}
