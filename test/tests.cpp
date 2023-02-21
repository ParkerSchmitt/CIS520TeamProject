#include <fcntl.h>
#include <stdio.h>
#include "gtest/gtest.h"
#include <pthread.h>
#include "../include/processing_scheduling.h"

// Using a C library requires extern "C" to prevent function managling
extern "C" 
{
#include <dyn_array.h>
}


#define NUM_PCB 30
#define QUANTUM 5 // Used for Robin Round for process as the run time limit

unsigned int score;
unsigned int total;

class GradeEnvironment : public testing::Environment 
{
    public:
        virtual void SetUp() 
        {
            score = 0;
            total = 210;
        }

        virtual void TearDown()
        {
            ::testing::Test::RecordProperty("points_given", score);
            ::testing::Test::RecordProperty("points_total", total);
            std::cout << "SCORE: " << score << '/' << total << std::endl;
        }
};
//If both params are null
TEST (first_come_first_serve, NullParams) {
    bool ret = first_come_first_serve(NULL, NULL);
    bool expected = false;
    EXPECT_EQ(ret,expected);
    if (ret == expected) {
        score = score + 10;
    }
}
//If queue param is null
TEST (first_come_first_serve, NullQueue) {
    ScheduleResult_t *result = new ScheduleResult_t;
    bool ret = first_come_first_serve(NULL, result);
    bool expected = false;
    EXPECT_EQ(ret,expected);
    if (ret == expected) {
        score = score + 10;
    }
}

//If result params are correct
TEST (first_come_first_serve, GoodParams) {
    dyn_array_t* array = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    ScheduleResult_t *result = new ScheduleResult_t;
    	ProcessControlBlock_t test[3] = {
			[0] = {24,0,0,false},
			[1] = {3,0,0,false},
			[2] = {3,0,0,false},
	};
	dyn_array_push_back(array,&test[0]);
    dyn_array_push_back(array,&test[1]);
	dyn_array_push_back(array,&test[2]);


    bool ret = first_come_first_serve(array, result);
    dyn_array_destroy(array);

    EXPECT_EQ(true,ret);
    //Average witing timee according to handout is (time to start process x1 + +sumoid(0,1, time to start process x2) +sumoif(0,2, time to start procexx xn) ... from x0 to xn-1)/n
    EXPECT_EQ((uint32_t) 17, result->average_waiting_time);
    //Average witing timee according to handout is (time to start process x1 + +sumoid(0,1, time to start process x2) +sumoif(0,2, time to start procexx xn) ... from x0 to xn)/n
    EXPECT_EQ((uint32_t) 27, result->average_turnaround_time);
    //Average total run time timee according to handout is sum of all times
    EXPECT_EQ((uint32_t) 30, result->total_run_time);
    if (ret == true) {
        score = score + 10;
    }
}

//If both params are null
TEST (shortest_job_first, NullParams) {
    bool ret = shortest_job_first(NULL, NULL);
    bool expected = false;
    EXPECT_EQ(ret,expected);
    if (ret == expected) {
        score = score + 10;
    }
}
//If queue param is null
TEST (shortest_job_first, NullQueue) {
    ScheduleResult_t *result = new ScheduleResult_t;
    bool ret = shortest_job_first(NULL, result);
    bool expected = false;
    EXPECT_EQ(ret,expected);
    if (ret == expected) {
        score = score + 10;
    }
}
//If result param is null
TEST (shortest_job_first, NullResult) {
    dyn_array_t* array = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    bool ret = shortest_job_first(array, NULL);
    dyn_array_destroy(array);
    bool expected = false;

    EXPECT_EQ(ret,expected);
    if (ret == expected) {
        score = score + 10;
    }
}

//If result params are correct
TEST (shortest_job_first, GoodParams) {
    dyn_array_t* array = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    ScheduleResult_t *result = new ScheduleResult_t;
    // Data from http://boron.physics.metu.edu.tr/ozdogan/OperatingSystems/week6/node3.html#:~:text=A%20pre%2Demptive%20SJF%20algorithm,to%20finish%20its%20CPU%20burst.&text=Nonpreemptive%20SJF%20scheduling%20would%20result,waiting%20time%20of%207.75%20milliseconds.
    	ProcessControlBlock_t test[4] = {
			[0] = {6,0,0,false},
			[1] = {8,0,0,false},
			[2] = {7,0,0,false},
			[3] = {3,0,0,false},
	};
	dyn_array_push_back(array,&test[0]);
    dyn_array_push_back(array,&test[1]);
	dyn_array_push_back(array,&test[2]);
	dyn_array_push_back(array,&test[3]);
	dyn_array_push_back(array,&test[4]);


    bool ret = first_come_first_serve(array, result);
    dyn_array_destroy(array);

    EXPECT_EQ(true,ret);
    //Average witing timee according to handout is (time to start process x1 + +sumoid(0,1, time to start process x2) +sumoif(0,2, time to start procexx xn) ... from x0 to xn-1)/n
    EXPECT_EQ((uint32_t) 7, result->average_waiting_time);
    //Average witing timee according to handout is (time to start process x1 + +sumoid(0,1, time to start process x2) +sumoif(0,2, time to start procexx xn) ... from x0 to xn)/n
    EXPECT_EQ((uint32_t) 13, result->average_turnaround_time);
    //Average total run time timee according to handout is sum of all times
    EXPECT_EQ((uint32_t) 28, result->total_run_time);
    if (ret == true) {
        score = score + 10;
    }
}

//If queue param is null
TEST (round_robin, NullQueue) {
    ScheduleResult_t *result = new ScheduleResult_t;
    bool ret = round_robin(NULL, result, 0);
    bool expected = false;
    EXPECT_EQ(ret,expected);
    if (ret == expected) {
        score = score + 10;
    }
}
//If result param is null
TEST (round_robin, NullResult) {
    dyn_array_t* array = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    bool ret = round_robin(array, NULL, 0);
    dyn_array_destroy(array);
    bool expected = false;

    EXPECT_EQ(ret,expected);
    if (ret == expected) {
        score = score + 10;
    }
}


TEST (load_process_control_blocks, NullParam) {
	dyn_array_t* pcb = load_process_control_blocks (NULL);
	ASSERT_EQ((dyn_array_t*)NULL, pcb);
	score+=5;
}

TEST (load_process_control_blocks, MissingFileParam) {
	dyn_array_t* pcb = load_process_control_blocks ("missingfile.bin");
	ASSERT_EQ((dyn_array_t*)NULL, pcb);
	score+=5;
}


TEST (load_process_control_blocks, GoodFile) {

    //We will create a temp file to read from because the build isn't situtated where the pcb.bin is and it's contents might change, anyways.
    FILE *fd;
    fd = fopen("test.bin", "wb");
    uint32_t vals = 4;

	uint32_t test_vals[12] = {
        15,5,3,
        32547,2,1,
        4251137,2,1,
        0,0,0
    };
    fwrite(&vals,sizeof(uint32_t),1,fd);
    fwrite(test_vals,sizeof(uint32_t),12,fd);
    fclose(fd);
    dyn_array_t* result = load_process_control_blocks ("test.bin");
   for (uint32_t i=0; i < dyn_array_size(result); i++) {
        ProcessControlBlock_t* val = (ProcessControlBlock_t*) dyn_array_at(result,i);
        ASSERT_EQ(val->remaining_burst_time, test_vals[i*3]);
        ASSERT_EQ(val->priority, test_vals[(i*3)+1]);
        ASSERT_EQ(val->arrival, test_vals[(i*3)+2]);
    }
	score+=5;
}




int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new GradeEnvironment);
    return RUN_ALL_TESTS();
}

