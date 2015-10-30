/*
 * time.c
 *
 *  Created on: Aug 2, 2015
 *      Author: alhansfo
 */


#include "events.h"
#include "events-private.h"            // safety include
#include "..\..\src\nodes\nodes.h"     // safety include
#include "..\..\src\times\times.h"     // safety include
#include "..\..\src\lists\lists.h"     // supports class registration
#include "..\..\src\objects\objects.h" // safety include

/**************************************/
/***** Implementation Functions  ******/

static void *  implement_Event_dtor(       struct Event * _self);
void *         implement_Event_copy(       struct Event * _copyTo,
                                     const struct Event * _copyFrom);
equal_t        implement_Event_equal(const struct Event * _self,
		                             const struct Event * _comparisonObject);
static void * implement_Event_config(      struct Event * _self,
		                             const struct Event * _master);
puto_return_t   implement_Event_puto(const struct Event * _self, FILE * _fp);


static void * implement_Event_getTimePointer(struct Event * _self);
static void * implement_Event_setTimePointer(struct Event * _self,
		                                     void *         _timePTR);

static void * implement_Event_getMessagePointer(struct Event * _self);
static void * implement_Event_setMessagePointer(struct Event * _self,
		                                        void *         _messagePTR);

static void * implement_Event_updateEventRunCount(const struct Event * _self,
		                                     const void * _comparisonTime);
static void * implement_Event_decrementEventRunCounter(const void * _self);

static runCount_t implement_Event_setEventRunCount(const struct Event * _self,
		                                           const runCount_t _runCount);
static runCount_t implement_Event_getEventRunCount(const struct Event * _self);


/*****************************/
/**** INITIALIZATIONS  *******/

const void * EventClass = NULL;
const void * Event      = NULL;

// MUST be called before any Event is created
void Event_init(void)
{
	// Insert the lines below for any derived subclasses
	//if (!Event)      {Event_init();}
	//if (!EventClass) {Event_init();}

	if (!Node)      { Node_init(); }
	if (!NodeClass) { Node_init(); }

	if (!Time)      { Time_init(); }
	if (!TimeClass) { Time_init(); }

	if (! EventClass) {
		EventClass = new(Class,  		// should be "Class"
						"EventClass",	// should be "SomethingClass"
						Class,  		// should be "Class"
						sizeof(struct EventClass),
						ctor, EventClass_ctor,	//SomethingClass_ctor
						0);  // Terminator
	}
	if (! Event) {
		Event = new(EventClass,			// SomethingClass from above
				    "Event",				// name like "Something"
				    Object,  			// "superclass(Something)"
				    sizeof(struct Event),// size of self
					// Overloaded superclass functions
					// Remember to call superclass->method
				    ctor,	Event_ctor,//Something_ctor
					dtor,   Event_dtor,
					copy,	Event_copy,
					equal,	Event_equal,
					config, Event_config,
					puto,	Event_puto,
					// overloaded methods from Node


					// New functions added in this class
					// Do not call superclass->method
					setTimePointer,	Event_setTimePointer,
					getTimePointer,	Event_getTimePointer,

					setMessagePointer,	Event_setMessagePointer,
					getMessagePointer,	Event_getMessagePointer,

					updateEventRunCount,      Event_updateEventRunCount,
					decrementEventRunCounter, Event_decrementEventRunCounter,
					setEventRunCount,         Event_setEventRunCount,
					getEventRunCount,         Event_getEventRunCount,
				    0);	// Terminator
	}


	// requires #include "..\..\src\lists\lists.h" to support class registry
	//implement_Event_registerKeyClasses();

	return;
}

/*****************************/
/****** Constructors  ********/

void * Event_ctor(void * _self, va_list * app)
{
	// Add superclass class data members
	struct Event * self = super_ctor(Event, _self, app);

	// set default values for any new data members
	self->time    = NULL;
	self->message = NULL;

	// overwrite data members with new data
	// Only uncomment if all data members will be specified in new() command
	// ... this seems like an undue burden on the user.  Leave commented out
	// ... numerous unit tests will need to be adapted if uncommented
	//self->time    = va_arg(* app, void *);
	//self->message = va_arg(* app, void *);

	// create embedded structures

	self->time    = new(Time);
	if ( self->time == NULL ) { return NULL; } // fail

	// Add any time initializations here

	self->message = new(Node);
	if ( self->message == NULL ) { return NULL; } // fail

	// Add any time initializations here
	setRunCount(self->message, 0);

	return self;
}

void * EventClass_ctor(void * _self, va_list *app)
{
	// Add superclass class data and methods
	struct EventClass * self = super_ctor(EventClass, _self, app);

	// Initialize new functions to default values
	typedef void (* voidf)();

	* (voidf *) & self->setTimePointer    = NULL;
	* (voidf *) & self->getTimePointer    = NULL;

	* (voidf *) & self->setMessagePointer = NULL;
	* (voidf *) & self->getMessagePointer = NULL;

	* (voidf *) & self->updateEventRunCount = NULL;
	* (voidf *) & self->decrementEventRunCounter = NULL;

	* (voidf *) & self->setEventRunCount    = NULL;
	* (voidf *) & self->getEventRunCount    = NULL;


	// Update any overloaded method function pointers
	// using style ... , methodSelector, overloadedFunctionPtr,
	voidf selector;

	#ifdef va_copy
		va_list ap;
		va_copy(ap, * app);
	#else
		va_list ap = * app;
	#endif

	// if non-NULL selector then grab next method pointer from arg list
	while ( (selector = va_arg(ap, voidf)) ) {
		voidf overloadedFunctionPtr  = va_arg(ap, voidf);

	// assign methods newly added to Events

	if (selector == (voidf)setTimePointer)
		{* (voidf *) & self->setTimePointer = overloadedFunctionPtr;}
	if (selector == (voidf)getTimePointer)
		{* (voidf *) & self->getTimePointer = overloadedFunctionPtr;}

	if (selector == (voidf)setMessagePointer)
		{* (voidf *) & self->setMessagePointer = overloadedFunctionPtr;}
	if (selector == (voidf)getMessagePointer)
		{* (voidf *) & self->getMessagePointer = overloadedFunctionPtr;}

	if (selector == (voidf)updateEventRunCount)
	  {* (voidf *) & self->updateEventRunCount = overloadedFunctionPtr;}
	if (selector == (voidf)decrementEventRunCounter)
	  {* (voidf *) & self->decrementEventRunCounter = overloadedFunctionPtr;}

	if (selector == (voidf)setEventRunCount)
	  {* (voidf *) & self->setEventRunCount = overloadedFunctionPtr;}
	if (selector == (voidf)getEventRunCount)
	  {* (voidf *) & self->getEventRunCount = overloadedFunctionPtr;}

	#ifdef va_copy
		va_end(ap);
	#endif

	}  // end while

	return self;
}


/*****************************/
/**** Overloaded Methods  ****/

void * Event_dtor(void * _self)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct Event * self = cast(Event, _self);
	if(self == NULL)                         {return NULL; } // fail

	// address local members first
	if ( implement_Event_dtor(self) == NULL) {return NULL; } // fail

	// call super method after local members are addressed
	// NOTE: classOf(self) calls into super will trigger looping
	if ( super_dtor(Event, _self) == NULL)   {return NULL; } // fail

	return _self;
}

void * Event_copy(void * _copyToEvent, const void * _copyFromEvent)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct Event * copyToEvent = cast(Event, _copyToEvent);
	if ( copyToEvent == NULL )   { return NULL; } //fail

	struct Event * copyFromEvent = (void *)_copyFromEvent;
	if ( copyFromEvent == NULL ) { return NULL; } // fail

	// WARNING: calling superclass in composite class is likely NOT correct
	//          complex classes should manage calls to superclass as needed
	//          within their implementation code

	// WARNING:  If Event is a complex class, do not call superclass_copy

	// call super method first
	// NOTE: classOf(copyToEvent) calls into super will trigger looping
	if (super_copy(Event, copyToEvent, copyFromEvent) == NULL )
		{ return NULL; } // fail

	// address local components
	return implement_Event_copy(copyToEvent, copyFromEvent);
}

equal_t Event_equal(const void * _self, const void * _comparisonObject)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct Event * self = cast(Event, _self);
	if (self == NULL )              { return OBJECT_UNEQUAL; } // fail

	struct Event * comparisonObject = cast(Event, _comparisonObject);
	if (comparisonObject == NULL )  { return OBJECT_UNEQUAL; } // fail

	// WARNING: calling superclass in composite class is likely NOT correct
	//          complex classes should manage calls to superclass as needed
	//          within the implementation code

	// WARNING:  if Event is a complex class, do not call superclass_equal

	// NOTE: classOf(self) calls into super will trigger looping
	if( super_equal(Event, self, comparisonObject) == OBJECT_UNEQUAL)
		{ return OBJECT_UNEQUAL; } // fail

	// address local components
	return implement_Event_equal(_self, _comparisonObject);
}

void * Event_config(const void * _self, const void * _master)
{
	struct Event * self = cast(Event, _self);
	if ( self == NULL )   { return NULL; }       // fail
	struct Event * master = cast(Event, _master);
	if ( master == NULL ) { return NULL; }        // fail
	return implement_Event_config(self, _master);  // expected path
}

puto_return_t Event_puto(const void * _self, FILE * _fp)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct Event * self = cast(Event, _self);
	if (self == NULL ) { return PUTO_ERROR; } // fail

	// the call to super_puto() is managed within implement_Event_puto()
	return implement_Event_puto(self, _fp);
}


/****************************************************************************/
/********  New functions for  class "EventClass"  ***************************/
/****************************************************************************/

/**************************************/
/**********  setTimePointer    ********/
/**/
void *  setTimePointer(void *_self, void *_timePTR)
{
	const struct EventClass * class = classOf( cast(Event, _self) );
	if ( class == NULL )                 { return NULL; } // fail
	if ( class->setTimePointer == NULL ) { return NULL; } // fail
	return class->setTimePointer(_self, _timePTR);
}

void * super_setTimePointer(const void * _class, void *_self, void *_timePTR)
{
	// verify that EventClass is in the superclass chain of _class
	if ( ! isOfSuper(EventClass, _self) )     { return NULL; } // fail
	const struct EventClass * superclass = super(_class);
	if ( superclass == NULL )                 { return NULL; } // fail
	if ( superclass->setTimePointer == NULL ) { return NULL; } // fail
	return superclass->setTimePointer(_self, _timePTR);
}

void * Event_setTimePointer(void *_self, void *_timePTR)
{
	struct Event * self = cast(Event, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Event_setTimePointer(self, _timePTR);
}


/**************************************/
/**********  getTimePointer    ********/

void * getTimePointer(void *_self)
{
	const struct EventClass * class = classOf( cast(Event, _self) );
	if ( class == NULL )                 { return NULL; } // fail
	if ( class->getTimePointer == NULL ) { return NULL; } // fail
	return class->getTimePointer(_self);
}

void * super_getTimePointer(const void * _class, void *_self)
{
	// verify that EventClass is in the superclass chain of _class
	if ( ! isOfSuper(EventClass, _self) )     { return NULL; } // fail
	const struct EventClass * superclass = super(_class);
	if ( superclass == NULL )                 { return NULL; } // fail
	if ( superclass->getTimePointer == NULL ) { return NULL; } // fail
	return superclass->getTimePointer(_self);
}

void * Event_getTimePointer(void *_self)
{
	struct Event * self = cast(Event, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Event_getTimePointer(self);
}


/**************************************/
/**********  setMessagePointer    ********/

void *  setMessagePointer(void *_self, void * _messagePTR)
{
	const struct EventClass * class = classOf( cast(Event, _self) );
	if ( class == NULL )                    { return NULL; } // fail
	if ( class->setMessagePointer == NULL ) { return NULL; } // fail
	return class->setMessagePointer(_self, _messagePTR);
}

void * super_setMessagePointer(const void * _class,
		                       void *_self, void * _messagePTR)
{
	// verify that EventClass is in the superclass chain of _class
	if ( ! isOfSuper(EventClass, _self) )        { return NULL; } // fail
	const struct EventClass * superclass = super(_class);
	if ( superclass == NULL )                    { return NULL; } // fail
	if ( superclass->setMessagePointer == NULL ) { return NULL; } // fail
	return superclass->setMessagePointer(_self, _messagePTR);
}

void * Event_setMessagePointer(void *_self, void * _messagePTR)
{
	struct Event * self = cast(Event, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Event_setMessagePointer(self, _messagePTR);
}


/**************************************/
/**********  getMessagePointer    ********/

void *  getMessagePointer(void *_self)
{
	const struct EventClass * class = classOf( cast(Event, _self) );
	if ( class == NULL )           { return NULL; } // fail
	if ( class->getMessagePointer == NULL ) { return NULL; } // fail
	return class->getMessagePointer(_self);
}

void * super_getMessagePointer(const void * _class, void *_self)
{
	// verify that EventClass is in the superclass chain of _class
	if ( ! isOfSuper(EventClass, _self) )        { return NULL; } // fail
	const struct EventClass * superclass = super(_class);
	if ( superclass == NULL )                    { return NULL; } // fail
	if ( superclass->getMessagePointer == NULL ) { return NULL; } // fail
	return superclass->getMessagePointer(_self);
}

void * Event_getMessagePointer(void *_self)
{
	struct Event * self = cast(Event, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Event_getMessagePointer(self);
}

/**************************************/
/***********  updateRunCount    *************/

void * updateEventRunCount(const void * _self, const void * _comparisonTime)
{
	const struct EventClass * class = classOf( cast(Event, _self) );
	if ( class == NULL )                 { return NULL; } // fail
	if ( class->updateEventRunCount == NULL ) { return NULL; } // fail
	return class->updateEventRunCount(_self, _comparisonTime);
}

void * super_updateEventRunCount(const void * _class,
		                    const void * _self,  const void * _comparisonTime)
{
	// verify that EventClass is in the superclass chain of _class
	if ( ! isOfSuper(EventClass, _self) )     { return NULL; } // fail
	const struct EventClass * superclass = super(_class);
	if ( superclass == NULL )                 { return NULL; } // fail
	if ( superclass->updateEventRunCount == NULL ) { return NULL; } // fail
	return superclass->updateEventRunCount(_self, _comparisonTime);
}

void * Event_updateEventRunCount(const void * _self,  const void * _comparisonTime)
{
	struct Event * self = cast(Event, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Event_updateEventRunCount(self, _comparisonTime);
}


/*************************************/
/******  setEventRunCount    *********/

runCount_t  setEventRunCount(const void * _self, const runCount_t _runCount)
{
	const struct EventClass * class = classOf( cast(Event, _self) );
	if ( class == NULL )                   { return RUN_COUNT_UNKNOWN;} // fail
	if ( class->setEventRunCount == NULL ) { return RUN_COUNT_UNKNOWN;} // fail
	return class->setEventRunCount(_self, _runCount);
}

runCount_t super_setEventRunCount(const void * _class,
								  const void * _self,
								  const runCount_t _runCount)
{
	// verify that EventClass is in the superclass chain of _class
	if ( ! isOfSuper(EventClass, _self) )
		{ return RUN_COUNT_UNKNOWN; } // fail
	const struct EventClass * superclass = super(_class);
	if ( superclass == NULL )
		{ return RUN_COUNT_UNKNOWN; } // fail
	if ( superclass->setEventRunCount == NULL )
		{ return RUN_COUNT_UNKNOWN; } // fail
	return superclass->setEventRunCount(_self, _runCount);
}

runCount_t Event_setEventRunCount(const void *     _self,
		                          const runCount_t _runCount)
{
	struct Event * self = cast(Event, _self);
	if( self == NULL ) { return RUN_COUNT_UNKNOWN; } // fail
	return implement_Event_setEventRunCount(self, _runCount);
}


/*************************************/
/******  getEventRunCount    *********/

runCount_t  getEventRunCount(const void * _self)
{
	const struct EventClass * class = classOf( cast(Event, _self) );
	if ( class == NULL )                   { return RUN_COUNT_UNKNOWN;} // fail
	if ( class->getEventRunCount == NULL ) { return RUN_COUNT_UNKNOWN;} // fail
	return class->getEventRunCount(_self);
}

runCount_t super_getEventRunCount(const void * _class, const void * _self)
{
	// verify that EventClass is in the superclass chain of _class
	if ( ! isOfSuper(EventClass, _self) )
		{ return RUN_COUNT_UNKNOWN; } // fail
	const struct EventClass * superclass = super(_class);
	if ( superclass == NULL )
		{ return RUN_COUNT_UNKNOWN; } // fail
	if ( superclass->getEventRunCount == NULL )
			{ return RUN_COUNT_UNKNOWN; } // fail
	return superclass->getEventRunCount(_self);
}

runCount_t Event_getEventRunCount(const void * _self)
{
	struct Event * self = cast(Event, _self);
	if( self == NULL ) { return RUN_COUNT_UNKNOWN; } // fail
	return implement_Event_getEventRunCount(self);
}


/**************************************/
/****  decrementEventRunCounter    ****/

void *  decrementEventRunCounter(const void * _self)
{
	const struct EventClass * class = classOf( cast(Event, _self) );
	if ( class == NULL )                           { return NULL; } // fail
	if ( class->decrementEventRunCounter == NULL ) { return NULL; } // fail
	return class->decrementEventRunCounter(_self);
}

void * super_decrementEventRunCounter(const void * _class, const void * _self)
{
	// verify that EventClass is in the superclass chain of _class
	if ( ! isOfSuper(EventClass, _self) )               { return NULL;} // fail
	const struct EventClass * superclass = super(_class);
	if ( superclass == NULL )                           { return NULL;} // fail
	if ( superclass->decrementEventRunCounter == NULL ) { return NULL;} // fail
	return superclass->decrementEventRunCounter(_self);
}

void * Event_decrementEventRunCounter(const void * _self)
{
	struct Event * self = cast(Event, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Event_decrementEventRunCounter(self);
}






/*************************************************/
/*********** Implementation Procedures ***********/

static void * implement_Event_dtor(struct Event * _self)
{
	_self->time    = safeDelete(_self->time);
	if ( _self->time != NULL ) { printf("failed to delete time member in Event_dtor\n"); }

	_self->message = safeDelete(_self->message);
	if ( _self->message != NULL ) { printf("failed to delete message member in Event_dtor\n"); }
	return _self;
}

void * implement_Event_copy(      struct Event * _copyTo,
						   const struct Event * _copyFrom)
{
	// determine struct Time pointers
	struct Time * copyFromTime  = (void *)_copyFrom->time;
	struct Time * copyToTime    = _copyTo->time;

	// determine struct Message pointers
	struct Node * copyFromMessage =  (void *)_copyFrom->message;
	struct Time * copyToMessage    = (void *)_copyTo->message;

	// copy Time and Node data
	if ( copy(copyToTime, copyFromTime)       == NULL ) {return NULL;} // fail
	if ( copy(copyToMessage, copyFromMessage) == NULL ) {return NULL;} // fail

	return _copyTo;
}

equal_t implement_Event_equal(const struct Event * _self,
		                     const struct Event * _comparisonObject)
{
	// get pointers to Time objects
	struct Time * selfTime       = (void *)_self->time;
	struct Time * comparisonTime = (void *)_comparisonObject->time;
	if ( equal(selfTime,  comparisonTime ) == OBJECT_UNEQUAL)
		{ return OBJECT_UNEQUAL; }


	// get pointers to Time objects
	struct Time * selfMessage       = (void *)_self->message;
	struct Time * comparisonMessage = (void *)_comparisonObject->message;
	if ( equal(selfMessage,  comparisonMessage ) == OBJECT_UNEQUAL)
		{ return OBJECT_UNEQUAL; }

	return OBJECT_EQUAL;
}

puto_return_t implement_Event_puto(const struct Event * _self, FILE * _fp)
{
	puto_return_t printReturnCode = PUTO_SUCCESS;

	struct Event * self = (void *)cast(Event, _self);

	// print local data members
	fprintf(_fp, "\n  Local data members managed in Event:\n");

	if (PUTO_ERROR == fprintf(_fp, "  Event time:    %p\n",
			self->time ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Event message: %p\n",
			self->message ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	fprintf(_fp, "\n  New methods added in Event:\n");

	if (PUTO_ERROR == fprintf(_fp, "  Event_setTimePointer:      %p\n",
			implement_Event_setTimePointer ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Event_getTimePointer:      %p\n",
			implement_Event_getTimePointer ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Event_setMessagePointer:   %p\n",
			implement_Event_setMessagePointer ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Event_getMessagePointer:   %p\n",
			implement_Event_getMessagePointer ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Event_updateEventRunCount: %p\n",
			implement_Event_updateEventRunCount ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Event_setEventRunCount:    %p\n",
			implement_Event_setEventRunCount ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Event_getEventRunCount:    %p\n",
			implement_Event_getEventRunCount))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	fprintf(_fp, "\n  Local Event methods:\n" );

	if (PUTO_ERROR == fprintf(_fp, "  Event_ctor:      %p\n", Event_ctor ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Event_dtor:      %p\n", Event_dtor ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Event_copy:      %p\n", Event_copy ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Event_equal:     %p\n", Event_equal ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Event_config:    %p\n", Event_config ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Event_puto:      %p\n", Event_puto ))
			{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  EventClass_ctor: %p\n\n", EventClass_ctor))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	fprintf(_fp, "Printing Event class:\n" );
	if (printReturnCode == Class_puto(Event, _fp))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	// print superclass data
	fprintf(_fp, "Printing the %s superclass data for Event:\n",
			((struct Object * )self)->class->superclass->className );
	// WARNING: classOf(self) calls into super will trigger looping
	if (printReturnCode == super_puto(Event, self, _fp))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	fprintf(_fp, "\nPrinting EventClass class:\n" );
	if (printReturnCode == puto(EventClass, _fp))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	return printReturnCode;
}

static void * implement_Event_config(      struct Event * _self,
		                             const struct Event * _master)
{
	return copy(_self, _master);
}

static void * implement_Event_setTimePointer(struct Event * _self, void *_timePTR)
{
	_self->time = _timePTR;
	return implement_Event_getTimePointer(_self);
}

static void * implement_Event_getTimePointer(struct Event * _self)
{
	return (void *)_self->time;
}

static void * implement_Event_setMessagePointer(struct Event * _self, void * _messagePTR)
{
	_self->message = _messagePTR;
	return implement_Event_getMessagePointer(_self);
}

static void * implement_Event_getMessagePointer(struct Event *_self)
{
	return (void *)_self->message;
}

static void * implement_Event_updateEventRunCount(const struct Event * _self,
		                                     const void * _comparisonTime)
{
	struct Node * myMessagePTR = getMessagePointer((void *)_self);
	runCount_t currentRunCount = getRunCount( myMessagePTR );

	struct Time * myTimePTR = getTimePointer((void *)_self);
	timeMatch_t timeMatch   = checkTimeMatch(myTimePTR, _comparisonTime);

	if ( timeMatch == MATCH) {
		currentRunCount++;
		setRunCount(  myMessagePTR, currentRunCount  );
	}
	return (void *)_self;
}

static void * implement_Event_decrementEventRunCounter(const void * _self)
{
	struct Node * myMessagePTR = getMessagePointer((void *)_self);
	runCount_t currentRunCount = getRunCount( myMessagePTR );
	if (currentRunCount > 0) {
		currentRunCount--;
	}
	else {
		currentRunCount = 0;
	}
	setRunCount(myMessagePTR, currentRunCount);
	return (void *)_self;  // remove this fail
}

static runCount_t implement_Event_setEventRunCount(const struct Event * _self, const runCount_t _runCount)
{
	struct Node * myMessagePTR = getMessagePointer((void *)_self);
	setRunCount(myMessagePTR, _runCount);
	return _runCount;
}

static runCount_t implement_Event_getEventRunCount(const struct Event * _self)
{
	struct Node * myMessagePTR = getMessagePointer((void *)_self);
	return getRunCount(myMessagePTR);
}



/*
void * resetTime(void * _self)
{
	struct Time * self = cast(Time, _self);
	self->second              = 0;
	self->minute              = 0;
	self->hour                = 12;
	self->hourMode            = HOUR_MODE_12;
	self->AmPm                = AM_TIME;
	self->dayOfWeek           = SUNDAY;
	self->timeUpdateIncrement = UPDATE_SECONDS;
	return _self;
}
*/

