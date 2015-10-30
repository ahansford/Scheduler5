/*
  * scheduler.c
 *
 *  Created on: Oct 18, 2014
 *      Author: Alan
 */

#include "..\cross_compiler_defs.h"
#include "..\mocks\RuntimeErrorStub.h"
#include "scheduler.h"

struct_task_t * tasksAddress;

scheduler_error_t scheduler_Create(struct_task_t * address)
{
	tasksAddress = address;

	scheduler_taskIndex_t taskIndex;
	for(taskIndex=0; taskIndex < SCHEDULER_MAX_TASKS; taskIndex++) {
		scheduler_DeleteTask(taskIndex);
	}
	return SCHEDULER_SUCCESS;
}


scheduler_taskIndex_t  scheduler_AddTask(void (* pTask)(void),
		                                 uint16_t delay,
		                                 uint16_t period)
{
	scheduler_taskIndex_t taskIndex = 0;

	if( findOpenIndex(&taskIndex) != SCHEDULER_SUCCESS ) {
		return SCHEDULER_MAX_TASKS;
	}

	tasksAddress[taskIndex].pTask         = pTask;
	tasksAddress[taskIndex].pTask_wPTR    = NULL;
	tasksAddress[taskIndex].objectPointer = NULL;
	tasksAddress[taskIndex].taskType      = SCHEDULER_TASK_TYPE_VOID;
	tasksAddress[taskIndex].delay         = delay;
	tasksAddress[taskIndex].period        = period;
	tasksAddress[taskIndex].runMe         = 0;
	return taskIndex;
}

scheduler_taskIndex_t  scheduler_AddTask_wPTR (void (* pTask)(void *),
											   void * _pointer,
		                                       uint16_t delay,
		                                       uint16_t period)
{
	scheduler_taskIndex_t taskIndex = 0;

	if( findOpenIndex(&taskIndex) != SCHEDULER_SUCCESS ) {
		return SCHEDULER_MAX_TASKS;
	}

	tasksAddress[taskIndex].pTask  = NULL;
	tasksAddress[taskIndex].pTask_wPTR  = pTask;
	tasksAddress[taskIndex].objectPointer = _pointer;
	tasksAddress[taskIndex].taskType = SCHEDULER_TASK_TYPE_VOID_PTR;
	tasksAddress[taskIndex].delay  = delay;
	tasksAddress[taskIndex].period = period;
	tasksAddress[taskIndex].runMe  = 0;
	return taskIndex;
}



scheduler_taskIndex_t  scheduler_AddBlockingTask (void (* pTask)(void),
		                                          uint16_t delay,
		                                          uint16_t period)
{
	scheduler_taskIndex_t taskIndex;
	taskIndex = scheduler_AddTask(pTask, delay, period);

	if( taskIndex != SCHEDULER_MAX_TASKS ) {

		tasksAddress[taskIndex].blocking = TRUE;
		return taskIndex;
	}

	return SCHEDULER_MAX_TASKS;
}


scheduler_error_t  scheduler_DeleteTask(scheduler_taskIndex_t taskIndex)
{
	if(!indexInRange(taskIndex)) {
		return SCHEDULER_ERROR_OUT_OF_BOUNDS_ACCESS;
	}

	tasksAddress[taskIndex].pTask      = NULL;
	tasksAddress[taskIndex].pTask_wPTR = NULL;
	tasksAddress[taskIndex].objectPointer   = NULL;
	tasksAddress[taskIndex].taskType   = SCHEDULER_TASK_TYPE_UNKNOWN;
	tasksAddress[taskIndex].delay      = 0;
 	tasksAddress[taskIndex].period     = 0;
	tasksAddress[taskIndex].runMe      = 0;
	tasksAddress[taskIndex].blocking   = 0;
	return SCHEDULER_SUCCESS;
}


scheduler_error_t scheduler_UpdateTasks()
{
	scheduler_taskIndex_t taskIndex;
	for (taskIndex = 0; taskIndex < SCHEDULER_MAX_TASKS; taskIndex++) {

		//  For every task in the task list
		if (taskAtThisIndex(taskIndex)) {

			if ( delayCountDownComplete(taskIndex) ) {

				addRunMeCycle(taskIndex);  // schedule to run

				if( taskIsPeriodic(taskIndex) ) {

					resetDelayCounter(taskIndex);  // repeating tasks
				}
				else
				{
					deleteTask(taskIndex);  // non-repeating tasks
				}
			}
			else {
				decrementDelayCount(taskIndex); // not ready to run yet
			}
		}
	}
	return SCHEDULER_SUCCESS;
}


scheduler_error_t	scheduler_DispatchTasks(void)
{
	scheduler_taskIndex_t taskIndex;
	for(taskIndex = 0; taskIndex < SCHEDULER_MAX_TASKS; taskIndex++) {
		if ((taskAtThisIndex(taskIndex)) && (taskReadyToRun(taskIndex))) {

			// function call to the task
			if (tasksAddress[taskIndex].taskType == SCHEDULER_TASK_TYPE_VOID )
				{ (*tasksAddress[taskIndex].pTask)(); }
			if (tasksAddress[taskIndex].taskType == SCHEDULER_TASK_TYPE_VOID_PTR )
				{ (*tasksAddress[taskIndex].pTask_wPTR)( tasksAddress[taskIndex].objectPointer ); }

			decrementRunMeCount(taskIndex);

			if(tasksAddress[taskIndex].period == 0) {
				scheduler_DeleteTask(taskIndex);
			}
		}
	}
	return SCHEDULER_SUCCESS;
}

bool indexInRange(scheduler_taskIndex_t taskIndex)
{
	if ((taskIndex < 0) || (taskIndex >= SCHEDULER_MAX_TASKS))
		{
		RuntimeError("scheduler: Out of bounds index attempt", taskIndex, __FILE__, __LINE__);
		return FALSE;
		}
	else
		{ return TRUE; }
}

bool indexIsEmpty(scheduler_taskIndex_t taskIndex)
{
	if ( (tasksAddress[taskIndex].pTask == NULL) && (tasksAddress[taskIndex].pTask_wPTR == NULL) )
		{ return 1; }
	else
		{ return 0;}
}

bool taskReadyToRun(scheduler_taskIndex_t  taskIndex)
{
	return (bool)((tasksAddress[taskIndex].runMe > 0) && indexInRange(taskIndex));
}

scheduler_error_t findOpenIndex(scheduler_taskIndex_t * taskIndex)
{
	scheduler_taskIndex_t localIndex = 0;
	while (!indexIsEmpty(localIndex) && indexInRange(localIndex)) {
		localIndex++;
	}

	if(!indexInRange(localIndex)) {
		RuntimeError("scheduler: Out of bounds index add", localIndex, __FILE__, __LINE__);
		* taskIndex = SCHEDULER_MAX_TASKS;
		return SCHEDULER_ERROR_TOO_MANY_TASKS;
	}

	* taskIndex = localIndex;
	return SCHEDULER_SUCCESS;
}

void decrementDelayCount(scheduler_taskIndex_t taskIndex)
{
	tasksAddress[taskIndex].delay--;
	return;
}

void decrementRunMeCount(scheduler_taskIndex_t taskIndex)
{
	tasksAddress[taskIndex].runMe--;
	return;
}

void resetDelayCounter(scheduler_taskIndex_t taskIndex)
{
	tasksAddress[taskIndex].delay = (tasksAddress[taskIndex].period - 1);
	return;
}

bool taskIsPeriodic(scheduler_taskIndex_t taskIndex)
{
	return (tasksAddress[taskIndex].period != 0);
}

void addRunMeCycle(scheduler_taskIndex_t taskIndex)
{
	tasksAddress[taskIndex].runMe++;
	return;
}

void deleteTask(scheduler_taskIndex_t taskIndex)
{
	scheduler_DeleteTask(taskIndex);
	return;
}

bool delayCountDownComplete(scheduler_taskIndex_t taskIndex)
{
	return tasksAddress[taskIndex].delay == 0;
}

bool taskAtThisIndex(scheduler_taskIndex_t taskIndex)
{
	return !indexIsEmpty(taskIndex);
}
