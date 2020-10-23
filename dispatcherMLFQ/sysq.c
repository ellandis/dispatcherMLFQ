#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/wait.h>
#include "sysq.h"



//processes
struct job_Node
{
    int Arrival_Time; //arrival time
    int Priority; //priority
    int Processor_Time; //processor time
    pid_t pid;
    enum job_state state;
};
//FCFS
struct System_Process_Que0{
    jnode **sysqlist;
    int front,rear,size;
    int capacity;
};

//initialize System Process Que
SPQ* Create_SysProQ(){
    SPQ *que = malloc(sizeof(SPQ));
    if (!que)
    { return NULL; }
    que->front = que->rear = -1;
    que->capacity = 2;
    que->size = 0;
    que->sysqlist = malloc(que->capacity * sizeof(jnode));
    return que;
}
jnode* create_jnode(){
    jnode* node = malloc(sizeof(jnode));
    assert(node != NULL);
    node->Arrival_Time = -1;
    node->Priority = -1;
    node->Processor_Time = -1;
    node->pid = 0;
    node->state = ready;
    return node;
}

//push process job
void SysProQ_Enqueue(SPQ *que, jnode *data){
    //if(SysProQ_IsFull(que)){
    if(que->size == que->capacity - 1){
        Double_SPQ(que);
    }
    que->rear = (que->rear + 1) % que->capacity;
    que->sysqlist[que->rear] = data;
    que->size++;
    if(que->front == -1){
        que->front = que->rear;
    }
}

//may not need to return jnode*
jnode* SysProQ_Dequeue(SPQ* que){
    jnode *data;
    if(SysProQ_IsEmpty(que)){
        printf("Que is Empty");
    }
    else{
        data = que->sysqlist[que->front];
        if(que->front == que->rear){
            que->front = que->rear = -1; 
        }
        else{
            que->front = (que->front + 1) % que->capacity;
        }
    }
    que->size--;
    return data;
}

int SysProQ_IsFull(SPQ* que){
    return ((que->rear + 1) % que->capacity == que->front); 
    //return que->size == que->capacity;
    //return que->rear == que->capacity - 1;
}
int SysProQ_IsEmpty(SPQ* que){
    return (que->front == -1);
}
int SPQ_size(SPQ* que){
    //return (que->capacity - que->front + que->rear + 1) % que->capacity;
    return que->size;
}

void Delete_SPQ(SPQ* que){
    if(que){
        if(que->sysqlist){
            free(que->sysqlist);
        }
        free(que);
        //printf("Que is Destroyed\n");
    }
}
void Display_SPQ(SPQ* que){
    int k = 0;
    int i = que->front;
    if (SysProQ_IsEmpty(que))
    //if(que->size == 0)
    {
        printf("Que is Empty\n");
    }
    else{
        while(k < SPQ_size(que)){
            printf("#%d que[%d]\t AT-> %d\t Pr-> %d\t PT-> %d\n",
            i,
            que->sysqlist[i]->Priority, 
            que->sysqlist[i]->Arrival_Time, 
            que->sysqlist[i]->Priority,
            que->sysqlist[i]->Processor_Time);
            i = (i + 1) % que->capacity;
            k++;
        }
        printf("\nsize = %d, cap = %d\n",que->size,que->capacity);
    }
}
jnode* SysProQ_Front(SPQ* que){
    return que->sysqlist[que->front];
}
void Double_SPQ(SPQ* que){
    int oldFront = que->front;
    int oldCap = que->capacity;
    que->capacity *= 2;
    SPQ *newArr = Create_SysProQ();
    newArr->sysqlist = malloc(que->capacity * sizeof(jnode));
    assert(newArr != 0);
    for (int i = 0; i < que->size; i++) {
        newArr->sysqlist[i] = que->sysqlist[(oldFront++) % oldCap];
    }
    free(que->sysqlist);
    que->sysqlist = newArr->sysqlist;
    que->front = 0;
}

void Shrink_SPQ(SPQ* que){
    que->capacity /= 2;
    if(que->capacity < 2){
        que->capacity = 2;
    }
    que->sysqlist = realloc(que->sysqlist, que->capacity * sizeof(int));
}



void Job_Placement(SPQ* dis_list, SPQ* sysQ,SPQ* us1,SPQ* us2,SPQ* us3){
    int i = 0;
    while (i < dis_list->size)
    {
        switch (dis_list->sysqlist[i]->Priority)
        {
        case 0:
            SysProQ_Enqueue(sysQ, dis_list->sysqlist[i]);
            break;
        case 1:
            SysProQ_Enqueue(us1, dis_list->sysqlist[i]);
            break;
        case 2:
            SysProQ_Enqueue(us2, dis_list->sysqlist[i]);
            break;
        case 3:
            SysProQ_Enqueue(us3, dis_list->sysqlist[i]);
            break;
        }
        i++;
    }
    return;
}


SPQ* Job_arrival_time_insert(SPQ* list, int x) {
    SPQ* fly = Create_SysProQ();
    for (int i = 0; i < list->size; i++) {
        if (list->sysqlist[i]->Arrival_Time == x) {
            SysProQ_Enqueue(fly, list->sysqlist[i]);
        } 
    }
    return fly; 
} 

