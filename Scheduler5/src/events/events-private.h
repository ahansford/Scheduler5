/*
 * time-private.h
 *
 *  Created on: Aug 2, 2015
 *      Author: alhansfo
 */

#ifndef SRC_EVENTS_EVENTS_PRIVATE_H_
#define SRC_EVENTS_EVENTS_PRIVATE_H_

/***********************************/
/*** class and object structures ***/

struct Event {
	const struct Object _;	// should the superclass
	struct Time * time;
	struct Node * message;
};


struct EventClass {
	const struct Class	_;	// should be superclass: Class or "somethingClass"

	void *    	(*setTimePointer)(void * _self, void * _timePTR);
	void *    	(*getTimePointer)(void * _self);

	void *    	(*setMessagePointer)(void * _self, void * _messagePTR);
	void *    	(*getMessagePointer)(void * _self);

	void *    	 (*updateEventRunCount)(const void * _self,
			                            const void * _comparisonTime);
	void *  (*decrementEventRunCounter)(const void * _self);

	runCount_t  (*setEventRunCount)(const void * _self, const runCount_t _runCount);
	runCount_t  (*getEventRunCount)(const void * _self);
	runCount_t  (*decrementEventRunCount)(const void * _self);
};

/*****************************/
/****** Constructors  ********/

void * EventClass_ctor(void * _self, va_list * app);
void * Event_ctor     (void * _self, va_list * app);

/***********************************/
/****** Overload Functions  ********/

void *      Event_dtor  (      void * _self);
void *      Event_copy  (      void * _copyTo, const void * _copyFrom);
equal_t     Event_equal (const void * _self, const void * _comparisonObject);
void *      Event_config(const void * _self, const void * _master);
puto_return_t Event_puto(const void * _self, FILE * _fp);


/**********************************/
/**********  new methods  *********/

void *    super_setTimePointer(const void * _class,
		                       void *_self, void *_timePTR);
void *    Event_setTimePointer(void *_self, void *_timePTR);

void *    super_getTimePointer(const void * _class, void *_self);
void *    Event_getTimePointer(void *_self);

void *    super_setMessagePointer(const void * _class,
		                          void *_self, void *_messagePTR);
void *    Event_setMessagePointer(void *_self, void *_messagePTR);

void *    super_getMessagePointer(const void * _class, void *_self);
void *    Event_getMessagePointer(                     void *_self);


void * super_updateEventRunCount(const void * _class,
		                         const void * _self,
								 const void * _comparisonTime);
void * Event_updateEventRunCount(const void * _self,
		                         const void * _comparisonTime);

void * super_decrementEventRunCounter(const void * _class,
		                              const void * _self);
void * Event_decrementEventRunCounter(const void * _self);



runCount_t super_setEventRunCount(const void * _class, const void * _self, const runCount_t _runCount);
runCount_t Event_setEventRunCount(const void * _self, const runCount_t _runCount);

runCount_t super_getEventRunCount(const void * _class, const void * _self);
runCount_t Event_getEventRunCount(                     const void * _self);


#endif /* SRC_EVENTS_EVENTS_PRIVATE_H_ */
