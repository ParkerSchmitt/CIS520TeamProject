#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dyn_array.h"
#include "processing_scheduling.h"


// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

// private function
void virtual_cpu(ProcessControlBlock_t *process_control_block) 
{
    // decrement the burst time of the pcb
    --process_control_block->remaining_burst_time;
}

bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{

    if (ready_queue == NULL) {
        return false;
    }

    int total_time = 0;
    int total_processes = dyn_array_size(ready_queue);
    int turnaround = 0;
    int waiting = 0;

    while (dyn_array_size(ready_queue) > 0) {

        int index = 0;
        uint32_t min = 0xFFFFFFFF;
        for (size_t i=0; i < dyn_array_size(ready_queue); i++) {
            if (min > ((ProcessControlBlock_t*) dyn_array_at(ready_queue,i))->arrival) {
                index = i;
                min = ((ProcessControlBlock_t*) dyn_array_at(ready_queue,i))->arrival;
            } //new min
        }
        ProcessControlBlock_t* p = (ProcessControlBlock_t*) dyn_array_at(ready_queue,index);
        if (p->remaining_burst_time > 0) {
            virtual_cpu(p);
            total_time++;
        } else {
            dyn_array_erase(ready_queue,index);
            if (dyn_array_size(ready_queue) > 0) {
                waiting += total_time;
            }
            turnaround += total_time;
        }
    }

    result->average_waiting_time =  (waiting)/(total_processes);
    result->average_turnaround_time = (turnaround)/total_processes;
    result->total_run_time = total_time;


    return true;

}

bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    if (ready_queue == NULL) {
        return false;
    }
    int total_time = 0;
    int total_processes = dyn_array_size(ready_queue);
    int turnaround = 0;
    int waiting = 0;
    while (dyn_array_size(ready_queue) > 0) {

        size_t index = 0;
        uint32_t min = 0xFFFFFFFF;
        
        for (size_t i=0; i < dyn_array_size(ready_queue); i++) {
            if (min >= ((ProcessControlBlock_t*) dyn_array_at(ready_queue,i))->remaining_burst_time) {

                //Two things same value different things, we do FCFS 
                if (index != i && ((ProcessControlBlock_t*) dyn_array_at(ready_queue,i))->remaining_burst_time == min) {
                    //If the old indexes arrival is greater, we take the new one
                    if ( ((ProcessControlBlock_t*) dyn_array_at(ready_queue,index))->arrival > ((ProcessControlBlock_t*) dyn_array_at(ready_queue,i))->arrival) {
                        index = i;
                        min = ((ProcessControlBlock_t*) dyn_array_at(ready_queue,i))->remaining_burst_time;
                    }
                } 
                // Its less than and a new value
                else {
                    index = i;
                    min = ((ProcessControlBlock_t*) dyn_array_at(ready_queue,i))->remaining_burst_time;
                }

            } //new min
        }
        ProcessControlBlock_t* p = (ProcessControlBlock_t*) dyn_array_at(ready_queue,index);
        while (p->remaining_burst_time > 0) {
            virtual_cpu(p);
            total_time++;
        } 

            dyn_array_erase(ready_queue,index);

           if (dyn_array_size(ready_queue) > 0) {
                printf("it has had to wait: %d\n", total_time);
                waiting += total_time;
            }
            turnaround += total_time;

        
    }

    result->average_waiting_time =  (waiting)/(total_processes);
    result->average_turnaround_time = (turnaround)/total_processes;
    result->total_run_time = total_time;


    return true;
}

bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    if (ready_queue == NULL) {
        return false;
    }
    int total_time = 0;
    int total_processes = dyn_array_size(ready_queue);
    int turnaround = 0;
    int waiting = 0;
    while (dyn_array_size(ready_queue) > 0) {

        int index = 0;
        uint32_t max = 0;
        
        for (size_t i=0; i < dyn_array_size(ready_queue); i++) {
            if (max < ((ProcessControlBlock_t*) dyn_array_at(ready_queue,i))->priority) {
                index = i;
                max = ((ProcessControlBlock_t*) dyn_array_at(ready_queue,i))->priority;

            } //new max
        }
        ProcessControlBlock_t* p = (ProcessControlBlock_t*) dyn_array_at(ready_queue,index);
        if (p->remaining_burst_time > 0) {
            virtual_cpu(p);
            
            total_time++;
        } else {

            dyn_array_erase(ready_queue,index);

           if (dyn_array_size(ready_queue) > 0) {
                waiting += total_time;
            }
            turnaround += total_time;

        }
    }

    result->average_waiting_time =  ((float) waiting)/((float) total_processes);
    result->average_turnaround_time = (turnaround)/total_processes;
    result->total_run_time = total_time;


    return true;
}

int compare(const void * a, const void * b)
{
    return ( *(int*)a - *(int*)b );
}

bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) 
{
    if (!ready_queue || !result || quantum == 0) return false;

    // the number of pcbs in ready_queue
    size_t n = dyn_array_size(ready_queue);

    // sort ready_queue by arrival time
    dyn_array_sort(ready_queue,compare);

    // waiting time, burst time, turnaround time
    int wt[n], bt[n], tat[n];

    // save the pcbs initial burst times into bt array
    for (size_t i = 0 ; i < n ; i++) {
        bt[i] = ((ProcessControlBlock_t*) dyn_array_at(ready_queue,i))->remaining_burst_time;
    }

    uint32_t total_time = 0;
    bool first_loop = true;

    while (1)
    {
        bool done = true;

        // iterate through ready_queue
        for (size_t i = 0; i < n; i++)
        {
            ProcessControlBlock_t* pcb = (ProcessControlBlock_t*) dyn_array_at(ready_queue,i);
            if (pcb->remaining_burst_time > 0)
            {
                done = false;
                if(first_loop) pcb->started = true;
                if(pcb->remaining_burst_time > quantum)
                {
                    total_time += quantum;
                    for(size_t cnt = 0; cnt < quantum; cnt++) virtual_cpu(pcb);
                }
                else
                {
                    total_time = total_time + pcb->remaining_burst_time;
                    wt[i] = total_time - bt[i];
                    pcb->remaining_burst_time = 0;
                }
            }
        }
        first_loop = false;
        if (done) break;
    }

    // calculate the total waiting time and turnaround time
    int total_wt = 0, total_tat = 0;
    for (size_t i = 0; i < n; i++) {
        total_wt = total_wt + wt[i];

        tat[i] = bt[i] + wt[i];
        total_tat = total_tat + tat[i];
    }

    result->average_waiting_time = ceilf(((float)total_wt / (float)n) * 1000) / 1000;
    result->average_turnaround_time = ceilf(((float)total_tat / (float)n) * 1000) / 1000;
    result->total_run_time = total_time;

    return true;
}

/**
 * The only thign stored in the .bin file should be the burst times
 **/

dyn_array_t *load_process_control_blocks(const char *input_file) 
{
    if (input_file == NULL || *input_file == 0) {
        return NULL;
    }

    FILE *fd = fopen(input_file, "rb");
    if (fd == NULL)
    {
        return NULL;
    }

	uint32_t N;
    fread(&N,sizeof(uint32_t),1,fd);
    uint32_t values[3];
    ProcessControlBlock_t parsedData[N];

    //0= burst, 1= priority 2= arrival
    for (size_t i=0; i < (size_t) N; i ++) {
        fread(values,sizeof(uint32_t),3,fd);
        parsedData[i].remaining_burst_time = values[0];
        parsedData[i].priority = values[1];
        parsedData[i].arrival = values[2];
    }



    return dyn_array_import(parsedData, N, sizeof(ProcessControlBlock_t), NULL);

}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
if (ready_queue == NULL) {
        return false;
    }
    int total_time = 0;
    int total_processes = dyn_array_size(ready_queue);
    int turnaround = 0;
    int waiting = 0;
    while (dyn_array_size(ready_queue) > 0) {

        size_t index = 0;
        uint32_t min = 0xFFFFFFFF;
        
        for (size_t i=0; i < dyn_array_size(ready_queue); i++) {
            if (min > ((ProcessControlBlock_t*) dyn_array_at(ready_queue,i))->remaining_burst_time) {

              //Two things same value different things, we do FCFS 
                if (index != i && ((ProcessControlBlock_t*) dyn_array_at(ready_queue,i))->remaining_burst_time == min) {
                    //If the old indexes arrival is greater, we take the new one
                    if ( ((ProcessControlBlock_t*) dyn_array_at(ready_queue,index))->arrival > ((ProcessControlBlock_t*) dyn_array_at(ready_queue,i))->arrival) {
                        index = i;
                        min = ((ProcessControlBlock_t*) dyn_array_at(ready_queue,i))->remaining_burst_time;
                    }
                } 
                // Its less than and a new value
                else {
                    index = i;
                    min = ((ProcessControlBlock_t*) dyn_array_at(ready_queue,i))->remaining_burst_time;
                }


            } //new min
        }
        ProcessControlBlock_t* p = (ProcessControlBlock_t*) dyn_array_at(ready_queue,index);
        if (p->remaining_burst_time > 0) {
            virtual_cpu(p);
            
            total_time++;
        } else {

            dyn_array_erase(ready_queue,index);

           if (dyn_array_size(ready_queue) > 0) {
                printf("it has had to wait: %d\n", total_time);
                waiting += total_time;
            }
            turnaround += total_time;

        }
    }

    result->average_waiting_time =  (waiting)/(total_processes);
    result->average_turnaround_time = (turnaround)/total_processes;
    result->total_run_time = total_time;


    return true;
}
