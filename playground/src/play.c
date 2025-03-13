#include "core/core.h"
#include "core/job.h"


void workerThread2(void *data){
    while(TRUE){
        printf("Worker thread is running %d\n",(int)data);
        Sleep(1000);
    }
}

void main(){
    struct Boss boss = {0};
    bossCreate(&boss);
    bossHireWorker(&boss);
    bossAssignJob(&boss,(struct Job){.func = workerThread2, .data = (void *)1});
    printf("test\n");
    while(TRUE){
        Sleep(1000);
    }
    return;
}