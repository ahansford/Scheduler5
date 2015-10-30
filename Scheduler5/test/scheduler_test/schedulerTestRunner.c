/*
 * LedDriverTestRunner.c
 *
 *  Created on: Oct 11, 2014
 *      Author: Alan
 */


#include "..\unity\unity_fixture.h"
#include "..\unity\unity.h"

TEST_GROUP_RUNNER(scheduler)
{
	RUN_TEST_CASE(scheduler, pTaskIsNullOnCreate);
	RUN_TEST_CASE(scheduler, pTaskWithPointerIsNullOnCreate);
	RUN_TEST_CASE(scheduler, taskTypeIsNullOnCreate);
	RUN_TEST_CASE(scheduler, DelayIsNullOnCreate);
	RUN_TEST_CASE(scheduler, PeriodIsNullOnCreate);
	RUN_TEST_CASE(scheduler, runMeIsNullOnCreate);
	RUN_TEST_CASE(scheduler, AddTaskAddsCooperativeTask);
	RUN_TEST_CASE(scheduler, AddTaskWithPointerAddsCooperativeTask);
	RUN_TEST_CASE(scheduler, AddTaskAddsPreemptiveTask);
	RUN_TEST_CASE(scheduler, DeleteSingleTask);
	RUN_TEST_CASE(scheduler, AddTwoTasks);
	RUN_TEST_CASE(scheduler, AddingASingleTaskReturnsZeroIndex);
	RUN_TEST_CASE(scheduler, AddingSecondTaskReturnsIndexOne);
	RUN_TEST_CASE(scheduler, AddingTooManyTasksReturnsMaxIndex);
	RUN_TEST_CASE(scheduler, DeletingNegativeIndexTriggeresError);
	RUN_TEST_CASE(scheduler, UpdateTaskIncrementsSingleRunMe);
	RUN_TEST_CASE(scheduler, UpdateTaskIncrementsMulitpleRunMe);
	RUN_TEST_CASE(scheduler, DispatcherRunsReadyTasks);
	RUN_TEST_CASE(scheduler, DispatcherRunsReadyTasksWithPointer);
	RUN_TEST_CASE(scheduler, DispatcherRunsMultipleRunMeCounts);
	RUN_TEST_CASE(scheduler, DispatcherDoesNotRunTasksBeforeUpdating);
	RUN_TEST_CASE(scheduler, TaskWithZeroPeriodRunsOnce);

	RUN_TEST_CASE(scheduler, Private_FindOpenIndexReturnsZeroOnFirstTask);
	RUN_TEST_CASE(scheduler, Private_FindOpenIndexReturnsOneOnSecondTask);
	RUN_TEST_CASE(scheduler, Private_FindOpenIndexReturnsMaxOnSixthTask);
	RUN_TEST_CASE(scheduler, Private_ZeroIndexEmptyAfterCreate);
	RUN_TEST_CASE(scheduler, Private_NumTwoIndexIsEmptyAfterTwoTaskAdds);
	RUN_TEST_CASE(scheduler, Private_ZeroIndexInRangeAfterCreate);
	RUN_TEST_CASE(scheduler, Private_MaxMinusOneIndexInRangeAfterCreate);
	RUN_TEST_CASE(scheduler, Private_MaxIndexNotInRangeAfterCreate);
	RUN_TEST_CASE(scheduler, Private_ZeroDelayTaskReadyToRunAfterUpdate);
	RUN_TEST_CASE(scheduler, Private_DecrementDelayCountAfterUpdate);
	RUN_TEST_CASE(scheduler, Private_DecrementRunCountAfterUpdate);
	RUN_TEST_CASE(scheduler, Private_ResetDelayCounterAfterRunForPerioicTasks);
	RUN_TEST_CASE(scheduler, Private_AddRunMeCycleAfterCreate);
	RUN_TEST_CASE(scheduler, Private_DeleteNonPeriodicTask);
	RUN_TEST_CASE(scheduler, Private_DetermineIfTaskIsPeriodic);
	RUN_TEST_CASE(scheduler, Private_ConfirmDelayCountDownIsComplete);
	RUN_TEST_CASE(scheduler, Private_ConfirmTaskIsAtThisIndex);

	RUN_TEST_CASE(scheduler, nothing);

}
