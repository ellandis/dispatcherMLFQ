#ifndef __SYSQ_H__
#define __SYSQ_H__
#include <stdio.h>
//queue implemented as a Binary Heap

typedef struct System_Process_Que0 SPQ;
typedef struct job_Node jnode;
enum job_state{ready,waiting, complete};


extern SPQ* Create_SysProQ();
jnode* create_jnode();
extern void SysProQ_Enqueue(SPQ*, jnode*);
extern jnode* SysProQ_Dequeue(SPQ*);

extern int SPQ_size(SPQ*);
extern int SysProQ_IsFull(SPQ*);
extern int SysProQ_IsEmpty(SPQ*);

extern void Display_SPQ(SPQ*);
extern void Delete_SPQ(SPQ*);
extern jnode* SysProQ_Front(SPQ*);
extern void Double_SPQ(SPQ*);
extern void Shrink_SPQ(SPQ*);
extern void fix(SPQ*);


extern void Job_Placement(SPQ*,SPQ*,SPQ*,SPQ*,SPQ*);
extern SPQ* Job_arrival_time_insert(SPQ* list, int x);


#endif