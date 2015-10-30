/*
 * time.h
 *
 *  Created on: Aug 2, 2015
 *      Author: alhansfo
 */

#ifndef SRC_EVENTS_H_
#define SRC_EVENTS_H_

/***********************************************/
/****** superclass and system includes  ********/

//#include "..\cross_compiler_defs.h"
#include "..\..\src\objects\objects.h"
#include "..\..\src\nodes\nodes.h"     // supports class registration
#include "..\..\src\times\times.h"     // supports class registration

/***********************************************/
/**** forward declarations and typedefs  *******/

extern const void * Event; // struct Event * new myEvent = new(Event);
extern const void * EventClass;


/***********************************************/
/************ protected includes  **************/
/***** must be after externs and typedefs  *****/

#include "events-private.h"


/***********************************************/
/****** application interface functions  *******/

// MUST be called before any new(Event) is created
void Event_init(void);


// returns pointer to "struct Time" on success, otherwise NULL
// ... time data members may be incomplete and UNKNOWN
// set is disallowed because the pointer is only set in the new() method
void *    	setTimePointer(void * _self, void * _timePTR);
void *    	getTimePointer(void * _self);

// returns pointer to "struct Node" on success, otherwise NULL
// ... message data members may be incomplete and UNKNOWN
// set is disallowed because the pointer is only set in the new() method
void *    	setMessagePointer(void * _self, void * _messagePTR);
void *    	getMessagePointer(void * _self);

// increments the event run count if event time matches the master time
// returns self on success
void * updateEventRunCount(const void * _self, const void * _comparisonTime);

// decrements the event run counter; returns self on success
// can be used in a closed loop acknowledgment for processed events
void * decrementEventRunCounter(const void * _self);

// directly accesses the event run counter.
// setting of the run counter should normally be avoided
// the updateEventRunCount(), with time matching, auto-increments
runCount_t setEventRunCount(const void * _self, const runCount_t _runCount);
runCount_t getEventRunCount(const void * _self);


#endif /* SRC_EVENTS_H_ */
