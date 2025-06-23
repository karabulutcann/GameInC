#include "stdio.h"
#include <stdlib.h>
#include <time.h>


int randomInRange(int min, int max) {
    srand(time(NULL));
    return min + rand() % (max - min + 1);
}

int main(){
    
    srand(time(NULL) * time(NULL)); 

    printf("%d",rand());
    return 0;
}
