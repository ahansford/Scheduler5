/*
 * LedDriverTest.c
 *
 *  Created on: Oct 11, 2014
 *      Author: Alan
 */

#include "..\unity\unity_fixture.h"
#include "..\unity\unity.h"
#include "..\..\src\mocks\RuntimeErrorStub.h"
#include "..\..\src\scheduler\scheduler.h"
#include "..\..\src\scheduler\scheduler-private.h"

uint16_t Funct1RunCount;
uint16_t Funct2RunCount;
uint16_t Funct3RunCount;
uint16_t Funct4RunCount;
uint16_t Funct5RunCount;
uint16_t Funct6RunCount;

uint16_t Funct7RunCount;

void	Funct1(void)  { Funct1RunCount++; return; }
void	Funct2(void)  { Funct2RunCount++; return; }
void	Funct3(void)  { Funct3RunCount++; return; }
void	Funct4(void)  { Funct4RunCount++; return; }
void	Funct5(void)  { Funct5RunCount++; return; }
void	Funct6(void)  { Funct6RunCount++; return; }

void	Funct7(void * _pointer)  { Funct7RunCount++; return; }

struct_task_t testTASKS[SCHEDULER_MAX_TASKS];

uint8_t i;

TEST_GROUP(scheduler);

TEST_SETUP(scheduler)
{
	Funct1RunCount = 0;
	Funct2RunCount = 0;
	Funct3RunCount = 0;
	Funct4RunCount = 0;
	Funct5RunCount = 0;
	Funct6RunCount = 0;
	Funct7RunCount = 0;

	RuntimeErrorStub_Reset();
	scheduler_Create(testTASKS);
}

TEST_TEAR_DOWN(scheduler)
{
}

TEST(scheduler, pTaskIsNullOnCreate)
{
	for(i=0; i < SCHEDULER_MAX_TASKS; i++) {
		TEST_ASSERT_EQUAL(NULL, testTASKS[0].pTask);
	}
}

TEST(scheduler, pTaskWithPointerIsNullOnCreate)
{
	for(i=0; i < SCHEDULER_MAX_TASKS; i++) {
		TEST_ASSERT_EQUAL(NULL, testTASKS[0].pTask_wPTR);
	}
}

TEST(scheduler, taskTypeIsNullOnCreate)
{
	for(i=0; i < SCHEDULER_MAX_TASKS; i++) {
		TEST_ASSERT_EQUAL(SCHEDULER_TASK_TYPE_UNKNOWN, testTASKS[0].taskType);
	}
}

TEST(scheduler, DelayIsNullOnCreate)
{
	for(i=0; i < SCHEDULER_MAX_TASKS; i++) {
		TEST_ASSERT_EQUAL(0, testTASKS[0].delay);
	}
}

TEST(scheduler, PeriodIsNullOnCreate)
{
	for(i=0; i < SCHEDULER_MAX_TASKS; i++) {
		TEST_ASSERT_EQUAL(0, testTASKS[0].period);
	}
}

TEST(scheduler, runMeIsNullOnCreate)
{
	for(i=0; i < SCHEDULER_MAX_TASKS; i++) {
		TEST_ASSERT_EQUAL(0, testTASKS[0].period);
	}
}

TEST(scheduler, AddTaskAddsCooperativeTask)
{
	scheduler_AddTask(&Funct1, 3, 2);
	TEST_ASSERT_EQUAL(&Funct1, testTASKS[0].pTask);
	TEST_ASSERT_EQUAL(NULL, testTASKS[0].pTask_wPTR);
	TEST_ASSERT_EQUAL(NULL, testTASKS[0].objectPointer);
	TEST_ASSERT_EQUAL(SCHEDULER_TASK_TYPE_VOID, testTASKS[0].taskType);
	TEST_ASSERT_EQUAL(3, testTASKS[0].delay);
	TEST_ASSERT_EQUAL(2, testTASKS[0].period);
	TEST_ASSERT_EQUAL(0, testTASKS[0].runMe);

}

TEST(scheduler, AddTaskWithPointerAddsCooperativeTask)
{
	scheduler_AddTask_wPTR(&Funct7, (void *)55, 3, 2);
	TEST_ASSERT_EQUAL(NULL, testTASKS[0].pTask);
	TEST_ASSERT_EQUAL(&Funct7, testTASKS[0].pTask_wPTR);
	TEST_ASSERT_EQUAL((void *)55, testTASKS[0].objectPointer);
	TEST_ASSERT_EQUAL(SCHEDULER_TASK_TYPE_VOID_PTR, testTASKS[0].taskType);
	TEST_ASSERT_EQUAL(3, testTASKS[0].delay);
	TEST_ASSERT_EQUAL(2, testTASKS[0].period);
	TEST_ASSERT_EQUAL(0, testTASKS[0].runMe);

}

TEST(scheduler, AddTaskAddsPreemptiveTask)
{
	scheduler_AddBlockingTask(&Funct1, 3, 2 );
	TEST_ASSERT_EQUAL(&Funct1, testTASKS[0].pTask);
	TEST_ASSERT_EQUAL(3, testTASKS[0].delay);
	TEST_ASSERT_EQUAL(2, testTASKS[0].period);
	TEST_ASSERT_EQUAL(0, testTASKS[0].runMe);
	TEST_ASSERT_EQUAL(1, testTASKS[0].blocking);
}

TEST(scheduler, DeleteSingleTask)
{
	uint8_t taskIndex;
	taskIndex = scheduler_AddBlockingTask(&Funct1, 3, 2);
	scheduler_DeleteTask(taskIndex);
	TEST_ASSERT_EQUAL(0, testTASKS[taskIndex].pTask);
	TEST_ASSERT_EQUAL(0, testTASKS[taskIndex].delay);
	TEST_ASSERT_EQUAL(0, testTASKS[taskIndex].period);
	TEST_ASSERT_EQUAL(0, testTASKS[taskIndex].runMe);
	TEST_ASSERT_EQUAL(0, testTASKS[0].blocking);
}

TEST(scheduler, AddTwoTasks)
{
	uint8_t taskIndex;
	scheduler_AddTask(&Funct1, 9, 9);
	taskIndex = scheduler_AddBlockingTask(&Funct2, 3, 2);
	TEST_ASSERT_EQUAL(&Funct2, testTASKS[taskIndex].pTask);
	TEST_ASSERT_EQUAL(3, testTASKS[taskIndex].delay);
	TEST_ASSERT_EQUAL(2, testTASKS[taskIndex].period);
	TEST_ASSERT_EQUAL(0, testTASKS[taskIndex].runMe);
	TEST_ASSERT_EQUAL(TRUE, testTASKS[taskIndex].blocking);
}

TEST(scheduler, AddingASingleTaskReturnsZeroIndex)
{
	TEST_ASSERT_EQUAL(0, scheduler_AddTask(&Funct1, 9, 9));
}

TEST(scheduler, AddingSecondTaskReturnsIndexOne)
{
	scheduler_AddTask(&Funct1, 9, 9);
	TEST_ASSERT_EQUAL(1, scheduler_AddTask(&Funct2, 9, 9));
}

TEST(scheduler, AddingTooManyTasksReturnsMaxIndex)
{
	scheduler_AddTask(&Funct1, 9, 9);
	scheduler_AddTask(&Funct2, 9, 9);
	scheduler_AddTask(&Funct3, 9, 9);
	scheduler_AddTask(&Funct4, 9, 9);
	scheduler_AddTask(&Funct5, 9, 9);
	TEST_ASSERT_EQUAL(SCHEDULER_MAX_TASKS, scheduler_AddTask(&Funct6, 9, 9));
}

TEST(scheduler, DeletingNegativeIndexTriggeresError)
{
	scheduler_DeleteTask(-1);
	TEST_ASSERT_EQUAL_STRING("scheduler: Out of bounds index attempt",
			RuntimeErrorStub_GetLastError());
	TEST_ASSERT_EQUAL(-1, RuntimeErrorStub_GetLastParameter());
}

TEST(scheduler, DeletingTooLargeIndexTriggeresError)
{
	scheduler_DeleteTask(SCHEDULER_MAX_TASKS+1);
	TEST_ASSERT_EQUAL_STRING("scheduler: Out of bounds index attempt",
			RuntimeErrorStub_GetLastError());
	TEST_ASSERT_EQUAL((SCHEDULER_MAX_TASKS + 1),
			RuntimeErrorStub_GetLastParameter());
}

TEST(scheduler, UpdateTaskIncrementsSingleRunMe)
{
	scheduler_taskIndex_t taskIndex;
	taskIndex = scheduler_AddTask(&Funct1, 0, 1);
	scheduler_UpdateTasks();
	TEST_ASSERT_EQUAL(1, testTASKS[taskIndex].runMe);
}

TEST(scheduler, UpdateTaskIncrementsMulitpleRunMe)
{
	scheduler_AddTask(&Funct1, 0, 1);
	scheduler_AddTask(&Funct2, 0, 1);
	scheduler_AddTask(&Funct3, 0, 1);
	scheduler_AddTask(&Funct4, 0, 1);
	scheduler_AddTask(&Funct5, 0, 1);

	scheduler_UpdateTasks();

	TEST_ASSERT_EQUAL(1, testTASKS[0].runMe);
	TEST_ASSERT_EQUAL(1, testTASKS[1].runMe);
	TEST_ASSERT_EQUAL(1, testTASKS[2].runMe);
	TEST_ASSERT_EQUAL(1, testTASKS[3].runMe);
	TEST_ASSERT_EQUAL(1, testTASKS[4].runMe);
}

TEST(scheduler, DispatcherRunsReadyTasks)
{
	scheduler_AddTask(&Funct1, 0, 1);
	scheduler_UpdateTasks();
	scheduler_DispatchTasks();
	TEST_ASSERT_EQUAL(1, Funct1RunCount);
}

TEST(scheduler, DispatcherRunsReadyTasksWithPointer)
{
	scheduler_AddTask_wPTR(&Funct7, (void *)44, 0, 1);
	scheduler_UpdateTasks();

	scheduler_DispatchTasks();

	TEST_ASSERT_EQUAL(1, Funct7RunCount);
}

TEST(scheduler, DispatcherRunsMultipleRunMeCounts)
{
	scheduler_AddTask(&Funct1, 0, 1);
	scheduler_UpdateTasks();
	scheduler_UpdateTasks();
	scheduler_DispatchTasks();
	scheduler_DispatchTasks();
	scheduler_DispatchTasks();
	TEST_ASSERT_EQUAL(2, Funct1RunCount);
}

TEST(scheduler, DispatcherDoesNotRunTasksBeforeUpdating)
{
	scheduler_AddTask(&Funct1, 0, 1);
	scheduler_DispatchTasks();
	TEST_ASSERT_EQUAL(0, Funct1RunCount);
}

TEST(scheduler, TaskWithZeroPeriodRunsOnce)
{
	scheduler_AddTask(&Funct1, 0, 1);
	scheduler_UpdateTasks();
	scheduler_DispatchTasks();
	scheduler_DispatchTasks();
	TEST_ASSERT_EQUAL(1, Funct1RunCount);
}

TEST(scheduler, Private_FindOpenIndexReturnsZeroOnFirstTask)
{
	scheduler_taskIndex_t taskIndex;
	scheduler_error_t error;

	error = findOpenIndex(&taskIndex);
	TEST_ASSERT_EQUAL(0, taskIndex);
	TEST_ASSERT_EQUAL(SCHEDULER_SUCCESS, error);
}

TEST(scheduler, Private_FindOpenIndexReturnsOneOnSecondTask)
{
	scheduler_taskIndex_t taskIndex;
	scheduler_error_t error;

	scheduler_AddTask(&Funct1, 0, 0);
	error = findOpenIndex(&taskIndex);
	TEST_ASSERT_EQUAL(1, taskIndex);
	TEST_ASSERT_EQUAL(SCHEDULER_SUCCESS, error);
}

TEST(scheduler, Private_FindOpenIndexReturnsMaxOnSixthTask)
{
	scheduler_taskIndex_t taskIndex;
	scheduler_error_t error;

	scheduler_AddTask(&Funct1, 0, 0);
	scheduler_AddTask(&Funct2, 0, 0);
	scheduler_AddTask(&Funct3, 0, 0);
	scheduler_AddTask(&Funct4, 0, 0);
	scheduler_AddTask(&Funct5, 0, 0);
	error = findOpenIndex(&taskIndex);
	TEST_ASSERT_EQUAL(SCHEDULER_MAX_TASKS, taskIndex);
	TEST_ASSERT_EQUAL(SCHEDULER_ERROR_TOO_MANY_TASKS, error);
}

TEST(scheduler, Private_ZeroIndexEmptyAfterCreate)
{
	TEST_ASSERT_EQUAL(1, indexIsEmpty(0));
}

TEST(scheduler, Private_NumTwoIndexIsEmptyAfterTwoTaskAdds)
{
	scheduler_AddTask(&Funct1, 0, 0);
	scheduler_AddTask(&Funct2, 0, 0);
	TEST_ASSERT_EQUAL(0, indexIsEmpty(0));
	TEST_ASSERT_EQUAL(0, indexIsEmpty(1));
	TEST_ASSERT_EQUAL(1, indexIsEmpty(2));
}

TEST(scheduler, Private_ZeroIndexInRangeAfterCreate)
{
	TEST_ASSERT_EQUAL(1, indexInRange(0));
}

TEST(scheduler, Private_MaxMinusOneIndexInRangeAfterCreate)
{
	TEST_ASSERT_EQUAL(1, indexInRange(SCHEDULER_MAX_TASKS-1));
}

TEST(scheduler, Private_MaxIndexNotInRangeAfterCreate)
{
	TEST_ASSERT_EQUAL(0, indexInRange(SCHEDULER_MAX_TASKS));
}

TEST(scheduler, Private_ZeroDelayTaskReadyToRunAfterUpdate)
{
	scheduler_AddTask(&Funct1, 0, 1);
	scheduler_UpdateTasks();
	TEST_ASSERT_EQUAL(1, taskReadyToRun(0));
}

TEST(scheduler, Private_DecrementDelayCountAfterUpdate)
{
	scheduler_AddTask(&Funct1, 10, 100);
	decrementDelayCount(0);
	TEST_ASSERT_EQUAL(9, testTASKS[0].delay);
}

TEST(scheduler, Private_DecrementRunCountAfterUpdate)
{
	scheduler_AddTask(&Funct1, 0, 1);
	scheduler_UpdateTasks();
	TEST_ASSERT_EQUAL(1, testTASKS[0].runMe);
	scheduler_DispatchTasks();
	TEST_ASSERT_EQUAL(0, testTASKS[0].runMe);
}

TEST(scheduler, Private_ResetDelayCounterAfterRunForPerioicTasks)
{
	uint16_t period = 10;

	scheduler_AddTask(&Funct1, 0, period);
	resetDelayCounter(0);
	TEST_ASSERT_EQUAL((period - 1), testTASKS[0].delay);
}

TEST(scheduler, Private_AddRunMeCycleAfterCreate)
{
	addRunMeCycle(0);
	TEST_ASSERT_EQUAL(1, testTASKS[0].runMe);
}

TEST(scheduler, Private_DeleteNonPeriodicTask)
{
	scheduler_AddTask(&Funct1, 0, 1);
	TEST_ASSERT_EQUAL(0, indexIsEmpty(0));
	deleteTask(0);
	TEST_ASSERT_EQUAL(1, indexIsEmpty(0));
}

TEST(scheduler, Private_DetermineIfTaskIsPeriodic)
{
	scheduler_AddTask(&Funct1, 0, 1);
	TEST_ASSERT_EQUAL(1, taskIsPeriodic(0));
}

TEST(scheduler, Private_ConfirmDelayCountDownIsComplete)
{
	scheduler_AddTask(&Funct1, 0, 2);
	scheduler_UpdateTasks();
	scheduler_UpdateTasks();
	TEST_ASSERT_EQUAL(1, delayCountDownComplete(0));
}

TEST(scheduler, Private_ConfirmTaskIsAtThisIndex)
{
	scheduler_AddTask(&Funct1, 0, 2);
	TEST_ASSERT_EQUAL(1, taskAtThisIndex(0));
}

TEST(scheduler, nothing)
{

}

