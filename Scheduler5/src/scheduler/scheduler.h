/*
 * scheduler.h
 *
 *  Created on: Oct 18, 2014
 *      Author: Alan
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "..\cross_compiler_defs.h"


typedef enum scheduler_error_t { SCHEDULER_SUCCESS = 0,
	   	   	   	   	   	   	     SCHEDULER_ERROR_TOO_MANY_TASKS,
	   	   	   	   	             SCHEDULER_ERROR_TASK_NOT_ADDED,
	   	   	   	   	   	   	     SCHEDULER_ERROR_OUT_OF_BOUNDS_ACCESS
}scheduler_error_t;

typedef enum scheduler_task_type_t { SCHEDULER_TASK_TYPE_UNKNOWN = 0,
								     SCHEDULER_TASK_TYPE_VOID,
								     SCHEDULER_TASK_TYPE_VOID_PTR
}scheduler_task_type_t;

#define SCHEDULER_MAX_TASKS 5


#include "scheduler-private.h"


scheduler_error_t  scheduler_Create(struct_task_t * address);
scheduler_error_t  scheduler_DeleteTask(scheduler_taskIndex_t taskIndex);
scheduler_error_t  scheduler_UpdateTasks(void);
scheduler_error_t  scheduler_DispatchTasks(void);

scheduler_taskIndex_t  scheduler_AddTask (void (* pTask)(void),
		                                  uint16_t delay,
		                                  uint16_t period);

scheduler_taskIndex_t  scheduler_AddTask_wPTR (void (* pTask)(void*_pointer),
											   void * _pointer,
		                                       uint16_t delay,
		                                       uint16_t period);

scheduler_taskIndex_t  scheduler_AddBlockingTask (void (* pTask)(void),
		                                          uint16_t delay,
		                                          uint16_t period);



#endif   /*  SCHEDULER_H_  */
