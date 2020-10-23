/* Landis Eian ellandis 11530445 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#include <sys/wait.h>

#include "sysq.h"
#include "sysq.c"
#define MAXJOBS 1000
#define QUANTUM 1


void terminateProcess();
void suspendProcess(); //user jnode
void startProcess();
void restartProcess();





int main(int argc, char *argv[])
{
    FILE* fp;
    fp = fopen(argv[1], "r");
    //creating a dispatch que to hold all jobs then placing them into the correct queue via priority order
    SPQ* dislist = Create_SysProQ();
     SPQ* sysq = Create_SysProQ();
     SPQ* user1 = Create_SysProQ();
     SPQ* user2 = Create_SysProQ();
     SPQ* user3 = Create_SysProQ();
    
    char buff[10],*tok;
    int x,y,z,currClock =0;
    
    //read input file
    if(argc < 2){
		perror("Missing file:");
	}
    while ( fgets(buff,10,fp)  != NULL)
    {
        // get numbers from sting
        tok = strtok(buff,", ");
        for (int i = 0; i < 3; i++)
        {
            if (i == 0)
            {
                x = atoi(tok);
            }
            else if (i == 1)
            {
                y = atoi(tok);
            }
            else
            {
                z = atoi(tok);
            }
            tok = strtok(NULL,", ");
        }
        
        //detailing each jnode read from file
        jnode *jnode = create_jnode();
        jnode->Arrival_Time = x;
        jnode->Priority = y;
        jnode->Processor_Time = z;
        //placing each jnode jnode in the dispatcher list
        SysProQ_Enqueue(dislist,jnode);
        
    }

    //Display_SPQ(dislist);
    
   //should i build the Sys ques here or in function
    //Job_Placement(dislist,sysq,user1,user2,user3);

    //displaying ques to view correct order
    //Display_SPQ(sysq); 
    // Display_SPQ(user1);
    // Display_SPQ(user2);
    // Display_SPQ(user3);
    // Display_SPQ(sysq);
    
    int num = 0;
    jnode *currently_running = 0;
    int Flag = 0;
    while (currently_running || num != SPQ_size(dislist)) {
        SPQ *curr_procs = Job_arrival_time_insert(dislist, currClock); 
        num += SPQ_size(curr_procs);
        int i;
        for (i = 0; i < SPQ_size(curr_procs); i++) {
            jnode *curr_proc = (jnode *)curr_procs->sysqlist[i];
            if(curr_proc->Priority == 0) {
                SysProQ_Enqueue(sysq, curr_proc);
            }
            if(curr_proc->Priority == 1)
                SysProQ_Enqueue(user1, curr_proc);

            if(curr_proc->Priority == 2){
                SysProQ_Enqueue(user2,curr_proc);
            }
            if(curr_proc->Priority == 3){
                SysProQ_Enqueue(user3, curr_proc);
            }
        }
        if (currently_running && currently_running->Processor_Time == 0) {
            terminateProcess(currently_running);
            if (Flag) {
                Flag = 0;
            }
            currently_running = 0;
        } 
      
        // if there are system processes waiting to be run and one is not already running 
        if (SPQ_size(sysq) > 0 && !Flag) {
            // preempt
            if (currently_running) {
                suspendProcess(currently_running);
                if(currently_running->Priority == 0) {
                SysProQ_Enqueue(sysq, currently_running);
                }
                if(currently_running->Priority == 1)
                    SysProQ_Enqueue(user1, currently_running);

                if(currently_running->Priority == 2){
                    SysProQ_Enqueue(user2,currently_running);
                }
                if(currently_running->Priority == 3){
                    SysProQ_Enqueue(user3, currently_running);
                }
                
            }
            jnode *sys_proc = SysProQ_Dequeue(sysq);
            startProcess(sys_proc);
            currently_running = sys_proc;   
            Flag = 1;       
        }
        // if sys SPQ is empty but last sys jnode running
        else if (Flag) {
            ;
        }
        // if no sys jnode is running and 1st Priority SPQ has some things in it 
        else if (SPQ_size(user1) > 0) {
            if (currently_running) {
                suspendProcess(currently_running);
                if(currently_running->Priority == 0) {
                SysProQ_Enqueue(sysq, currently_running);
                }
                if(currently_running->Priority == 1)
                    SysProQ_Enqueue(user1, currently_running);

                if(currently_running->Priority == 2){
                    SysProQ_Enqueue(user2,currently_running);
                }
                if(currently_running->Priority == 3){
                    SysProQ_Enqueue(user3, currently_running);
                }
            }
            jnode *one_proc = SysProQ_Dequeue(user1);
            if (one_proc->state == ready) {
                startProcess(one_proc);
            } else {
                restartProcess(one_proc);
            }
            currently_running = one_proc;
            Flag = 0;
        }
        else if (SPQ_size(user2) > 0) {
            if (currently_running) {
                suspendProcess(currently_running);
                if(currently_running->Priority == 0) {
                SysProQ_Enqueue(sysq, currently_running);
                }
                if(currently_running->Priority == 1)
                    SysProQ_Enqueue(user1, currently_running);

                if(currently_running->Priority == 2){
                    SysProQ_Enqueue(user2,currently_running);
                }
                if(currently_running->Priority == 3){
                    SysProQ_Enqueue(user3, currently_running);
                }
            }
            jnode *two_proc = SysProQ_Dequeue(user2);
            if (two_proc->state == ready) {
                startProcess(two_proc);
            } else {
                restartProcess(two_proc);
            }
            currently_running = two_proc;
            Flag = 0;
        }
        else if (SPQ_size(user3) > 0) {
            if (currently_running) {
                suspendProcess(currently_running);
                if(currently_running->Priority == 0) {
                SysProQ_Enqueue(sysq, currently_running);
                }
                if(currently_running->Priority == 1)
                    SysProQ_Enqueue(user1, currently_running);

                if(currently_running->Priority == 2){
                    SysProQ_Enqueue(user2,currently_running);
                }
                if(currently_running->Priority == 3){
                    SysProQ_Enqueue(user3, currently_running);
                }
            }
            jnode *three_proc = SysProQ_Dequeue(user3);
            if (three_proc->state == ready) {
                startProcess(three_proc);
            } else {
                restartProcess(three_proc);
            }
            currently_running = three_proc;
            Flag = 0;
        }
        // decrement>Processor_Time
        if (currently_running) {
            currently_running->Processor_Time--; 
        }
        currClock++;
        free(curr_procs);
        sleep(QUANTUM);
    }




    return 0;
}





void startProcess(jnode *p) {
    pid_t child_pid = fork();
    if (child_pid == 0) {
        char* args[] = {"./process","10",NULL};
        execvp(args[0], args);
        //free(args);    
    } else {
        p->pid = child_pid;
    }
}
void terminateProcess(jnode*p) {
    kill(p->pid, SIGINT);
    waitpid(p->pid, NULL, WUNTRACED);
}
void suspendProcess(jnode*p) {
    kill(p->pid, SIGTSTP);
    waitpid(p->pid, NULL, WUNTRACED);
    if (p->Priority != 3) {
        p->Priority++;
    }
    p->state = waiting;
}
void restartProcess(jnode*p) {
    kill(p->pid, SIGCONT);
}