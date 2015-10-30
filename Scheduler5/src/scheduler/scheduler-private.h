/*
 * scheduler-private.h
 *
 *  Created on: Oct 20, 2014
 *      Author: Alan
 */

#ifndef SCHEDULER_PRIVATE_H_
#define SCHEDULER_PRIVATE_H_

#include "..\cross_compiler_defs.h"

typedef int8_t scheduler_taskIndex_t;

typedef struct {
	void       (* pTask)(void);                 // void function
	void       (* pTask_wPTR)(void * objectPointer); // function pointer with PTR
	void *	   objectPointer;
	scheduler_task_type_t taskType;				// void-void, or void-voidPTR
	uint16_t   delay;           // delay before cycles start
	uint16_t   period;	        // time between cycles
	uint8_t    runMe;           // task should be run
	bool       blocking;        // true = a blocking task
} struct_task_t;


/* Private helper functions */

scheduler_error_t findOpenIndex(scheduler_taskIndex_t * taskIndex);

bool indexInRange(scheduler_taskIndex_t taskIndex);
bool indexIsEmpty(scheduler_taskIndex_t taskIndex);
bool taskReadyToRun(scheduler_taskIndex_t taskIndex);

void decrementDelayCount(scheduler_taskIndex_t taskIndex);
void decrementRunMeCount(scheduler_taskIndex_t taskIndex);
void resetDelayCounter(scheduler_taskIndex_t taskIndex);
void addRunMeCycle(scheduler_taskIndex_t taskIndex);
void deleteTask(scheduler_taskIndex_t taskIndex);
bool taskIsPeriodic(scheduler_taskIndex_t taskIndex);
bool delayCountDownComplete(scheduler_taskIndex_t taskIndex);
bool taskAtThisIndex(scheduler_taskIndex_t taskIndex);

#endif /* SCHEDULER_PRIVATE_H_ */
