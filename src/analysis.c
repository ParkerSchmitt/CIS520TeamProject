#include <stdio.h>
#include <stdlib.h>

#include "dyn_array.h"
#include "processing_scheduling.h"

#define FCFS "FCFS"
#define P "P"
#define RR "RR"
#define SJF "SJF"


// Add and comment your analysis code in this function.
// THIS IS NOT FINISHED.
int main(int argc, char **argv) 
{
    if (argc < 3) 
    {
        printf("%s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
        return EXIT_FAILURE;
    }
    // argv1 is file, argv2 is algoirthm, algv3 is quantum optional
    dyn_array_t *loaded_pcb=  load_process_control_blocks(argv[1]);
    ScheduleResult_t *result = (ScheduleResult_t*) malloc(sizeof(ScheduleResult_t));

    if (loaded_pcb == NULL) {
        printf("%s <pcb file> not found \n", argv[0]);
        return EXIT_FAILURE; 
    }


if (strcmp(argv[2], FCFS) == 0) 
{
    first_come_first_serve(loaded_pcb, result);

} 
else if (strcmp(argv[2], P) == 0)
{
    priority(loaded_pcb, result);
  // do something else
}
/* more else if clauses */
else /* default: */
{
    printf("Can not find that alg\n");
    free(result);
    dyn_array_destroy(loaded_pcb);
    return EXIT_FAILURE; 

}
    
    printf("Average Turnaround time: %f\n", result->average_turnaround_time);
    printf("Average Waiting time: %f\n", result->average_waiting_time);
    printf("Total run time: %lu\n", result->total_run_time);

    free(result);
    dyn_array_destroy(loaded_pcb);

    
    return EXIT_SUCCESS;
}
