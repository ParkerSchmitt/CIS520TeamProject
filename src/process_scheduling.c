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
    UNUSED(ready_queue);
    UNUSED(result);
    return false;
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
    if (!ready_queue || !result || quantum == 0) return false;

    // the number of pcbs in ready_queue
    size_t n = dyn_array_size(ready_queue);
    // waiting time, burst time, turnaround time
    int wt[n], bt[n], tat[n];

    // save the pcbs initial burst times into bt array
    for (size_t i = 0 ; i < n ; i++) {
        bt[i] = ((ProcessControlBlock_t*) dyn_array_at(ready_queue,i))->remaining_burst_time;
    }

    uint32_t total_time = 0;

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

        if (done) break;
    }

    // calculate the total waiting time and turnaround time
    int total_wt = 0, total_tat = 0;
    for (size_t i = 0; i < n; i++) {
        total_wt = total_wt + wt[i];

        tat[i] = bt[i] + wt[i];
        total_tat = total_tat + tat[i];
    }

    result->average_waiting_time = (float)total_wt / (float)n;
    result->average_turnaround_time = ceilf(((float)total_tat / (float)n) * 1000) / 1000;
    result->total_run_time = total_time;

    return true;
}

/**
 * The only thign stored in the .bin file should be the burst times
 **/
 
dyn_array_t *load_process_control_blocks(const char *input_file) 
{
    UNUSED(input_file);
    return NULL;
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;
}
