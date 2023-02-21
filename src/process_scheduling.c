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
        uint32_t min = 0;
        for (size_t i=1; i < dyn_array_size(ready_queue); i++) {
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

    result->average_waiting_time = (waiting)/(total_processes);
    result->average_turnaround_time = (turnaround)/total_processes;
    result->total_run_time = total_time;


    return true;

}

bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;   
}

bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;   
}

bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    UNUSED(quantum);
    return false;
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
    UNUSED(ready_queue);
    UNUSED(result);
    return false;
}
