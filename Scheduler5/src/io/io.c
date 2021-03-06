/*
 * io.c
 *
 *  Created on: Sep 30, 2015
 *      Author: Alan
 */

//#include <string.h>
#include "io.h"
#include "io-private.h"                        // safety include
#include "..\..\src\lists\lists.h"             // safety include
#include "..\..\src\objects\objects.h"         // safety include
//#include "..\..\src\scheduler\scheduler.h"   // safety include
#include "..\..\src\access_mem\access-mem.h"


static void * implement_IO_io_ctor(void * _self);
static void * implement_IO_io_dtor(        struct IO * _self);
static void * implement_IO_io_copy(        struct IO * _copyTo,
                                     const struct IO * _copyFrom);
static equal_t implement_IO_io_equal(const struct IO * _self,
		                             const struct IO * _comparisonObject);
static void * implement_IO_io_config(      struct IO * _self,
									 const struct IO * _master);
static puto_return_t implement_IO_io_puto(const struct IO * _self, FILE * _fp);

static void * implement_IO_io_xxxx(struct IO * _self);


/*****************************/
/**** INITIALIZATIONS  *******/

const void * IOClass = NULL;
const void * IO      = NULL;

// MUST be called before other IO methods are called
void IO_init(void)
{
	// Insert the lines below for any derived subclasses
	//if (!IO)      {IO_init();}
	//if (!IOClass) {IO_init();}

	// IO uses a List to manage sequences before they are executed
	if (!List)      {List_init();}
	if (!ListClass) {List_init();}

	if (! IOClass) {
		IOClass = new(Class,  		// should be "Class"
						"IOClass",	// should be "SomethingClass"
						Class,  		// should be "Class"
						sizeof(struct IOClass),
						ctor, IOClass_ctor,	//SomethingClass_ctor
						0);  // Terminator
	}
	if (! IO) {
		IO = new(IOClass,			// SomethingClass from above
					"IO",				// name like "Something"
					Object,  			// "superclass(Something)"
					sizeof(struct IO),// size of self
					// Overloaded superclass functions
					// Remember to call superclass->method
					ctor,	IO_io_ctor,//Something_ctor
					dtor,   IO_io_dtor,
					copy,	IO_io_copy,
					equal,	IO_io_equal,
					config, IO_io_config,
					puto,	IO_io_puto,
					// New functions added in this class
					// Do not call superclass->method
					//xxxx, IO_io_xxxx,

					0);	// Terminator
	}

	// WARNING:  TODO: the IO state machine needs a time out check in
	// the IO_UPDATE_WAITING_COMMAND state in case the lower IO access
	// module does not execute as planned.  This should also include an
	// unwinding process to report the failure and attempted re-trys.


	// requires #include "..\..\src\lists\lists.h" to support class registry
	//implement_Sensor_registerKeyClasses();

	return;
}

/*****************************/
/****** Constructors  ********/

void * IO_io_ctor(void * _self, va_list * app)
{
	// Add superclass class data members
	struct IO * self = super_ctor(IO, _self, app);
	if ( self == NULL ) { return NULL; } // failed to construct super

	// overwrite data members with new data
	// Only uncomment if all data members will be specified in new() command
	// ... this seems like an undue burden on the user.  Leave commented out
	// ... numerous unit tests will need to be adapted if uncommented
	//self->minute = va_arg(* app, minute_t);
	IO_setIoSequenceList(self, va_arg(* app, void *));

	// modify return if data members are individually initialized above

	return implement_IO_io_ctor(self);
}

void * IOClass_ctor(void * _self, va_list *app)
{
	// Add superclass class data and methods
	struct IOClass * self = super_ctor(IOClass, _self, app);

	// Initialize new functions to default values
	typedef void (* voidf)();

	//* (voidf *) & self->IO_xxxx            = NULL;

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

		if (selector == (voidf) dtor)
			{* (voidf *) & self->_._.class->dtor= overloadedFunctionPtr; }

		if (selector == (voidf) copy)
			{* (voidf *) & self->_._.class->copy = overloadedFunctionPtr;}

		if (selector == (voidf) equal)
			{* (voidf *) & self->_._.class->equal = overloadedFunctionPtr;}

		if (selector == (voidf) equal)
			{* (voidf *) & self->_._.class->config = overloadedFunctionPtr;}

		if (selector == (voidf) puto)
			{* (voidf *) & self->_._.class->puto = overloadedFunctionPtr;}


		// use form below to overload any new functions
/*
		if ( selector == (voidf)IO_xxxx )
		    { * (voidf *) & self->IO_xxxx = overloadedFunctionPtr; }
*/
	#ifdef va_copy
		va_end(ap);
	#endif

	}  // end while

	return self;
}


/*****************************/
/**** Overloaded Methods  ****/

void * IO_io_dtor(void * _self)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct IO * self = cast(IO, _self);
	if(self == NULL)                         {return NULL; } // fail

	// WARNING:  The command buffer should be deleted/freed externally

	// address local members first
	if ( implement_IO_io_dtor(self) == NULL) {return NULL; } // fail

	// call super method after local members are addressed
	// NOTE: classOf(self) calls into super will trigger looping
	if ( super_dtor(IO, _self) == NULL)      {return NULL; } // fail

	return _self;
}

void * IO_io_copy(void * _copyTo, const void * _copyFrom)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct IO * copyTo = cast(IO, _copyTo);
	if ( copyTo == NULL )   { return NULL; } //fail

	struct IO * copyFrom = cast(IO, _copyFrom);
	if ( copyFrom == NULL ) { return NULL; } // fail

	// WARNING: calling superclass in composite class is likely NOT correct
	//          complex classes should manage calls to superclass as needed
	//          within their implementation code

	// WARNING:  If IO is a complex class, do not call superclass_copy

	// call super method first
	// NOTE: classOf(copyToIO) calls into super will trigger looping
	if (super_copy(IO, copyTo, copyFrom) == NULL )
		{ return NULL; } // fail

	// address local components
	return implement_IO_io_copy(copyTo, copyFrom);
}

equal_t IO_io_equal(const void * _self, const void * _comparisonObject)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct IO * self = cast(IO, _self);
	if (self == NULL )              { return OBJECT_UNEQUAL; } // fail

	struct IO * comparisonObject = cast(IO, _comparisonObject);
	if (comparisonObject == NULL )  { return OBJECT_UNEQUAL; } // fail

	// WARNING: calling superclass in composite class is likely NOT correct
	//          complex classes should manage calls to superclass as needed
	//          within the implementation code

	// WARNING:  if IO is a complex class, do not call superclass_equal

	// NOTE: classOf(self) calls into super will trigger looping
	if( super_equal(IO, self, comparisonObject) == OBJECT_UNEQUAL)
		{ return OBJECT_UNEQUAL; } // fail

	// address local components
	return implement_IO_io_equal(_self, _comparisonObject);
}

void * IO_io_config(const void * _self, const void * _master)
{
	struct IO * self = cast(IO, _self);
	if ( self == NULL )   { return NULL; }          // fail
	struct IO * master = cast(IO, _master);
	if ( master == NULL ) { return NULL; }          // fail
	return implement_IO_io_config(self, _master);  // expected path
}

puto_return_t IO_io_puto(const void * _self, FILE * _fp)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct IO * self = cast(IO, _self);
	if (self == NULL ) { return PUTO_ERROR; } // fail

	// the call to super_puto() is managed within implement_Button_puto()
	return implement_IO_io_puto(self, _fp);
}


/****************************************************************************/
/********  New functions for  class "IOClass"  ******************************/
/****************************************************************************/

/*************************************/
/***********  IO_xxxx    *************/

void *  IO_xxxx(void * _self)
{
	const struct IOClass * class = classOf( cast(IO, _self) );
	if ( class == NULL )          { return NULL; } // fail
	if ( class->IO_xxxx == NULL ) { return NULL; } // fail
	return class->IO_xxxx(_self);
}

void * super_IO_xxxx(const void * _class, void * _self)
{
	// verify that IOClass is in the superclass chain of _class
	if ( ! isOfSuper(IOClass, _self) ) { return NULL; } // fail
	const struct IOClass * superclass = super(_class);
	if ( superclass == NULL )          { return NULL; } // fail
	if ( superclass->IO_xxxx == NULL ) { return NULL; } // fail
	return superclass->IO_xxxx(_self);
}

void * IO_io_xxxx(void * _self)
{
	struct IO * self = cast(IO, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_IO_io_xxxx(self);
}

/*************************************************************/
/***** store and get struct IO objects from ioActionList *****/

void * IO_addIOSequenceToList(void * _self, void * _ioSequence)
{
	struct IO * self = cast(IO, _self);
	if( self == NULL )        { return NULL; } // fail
	if( _ioSequence == NULL ) { return NULL; } // fail

	struct List * localIoSequenceList = IO_getIoSequenceList(self);
	void * itemAddedToListPtr = add(localIoSequenceList, _ioSequence);

	// test for failure to add
	if ( itemAddedToListPtr == NULL ) { return NULL; } // fail

	return _ioSequence; // success
}

void * IO_getIOSequenceFromList(void * _self)
{
	struct IO * self = cast(IO, _self);
	if( self == NULL ) { return NULL; } // fail
	struct List * localIoSequenceList = IO_getIoSequenceList(self);

	void * itemFromList = take(localIoSequenceList);
	return itemFromList;
}

void * IO_sequenceComplete_cb(void * _self)
{
	//TODO: when does this fire ??  who fires it ??
	//io_update_state = IO_UPDATE_SEQUENCE_COMPLETE;
	IO_setIoState(_self, IO_UPDATE_SEQUENCE_COMPLETE);
	return NULL;
}


/*********************/
/***** IO_update *****/

void * IO_update(void * _self)
{
	struct IO * self = cast(IO, _self);
	if (self == NULL ) { return NULL; }

	struct AccessMEM * sequence = IO_getCurrentSequence(self);
	io_update_state_t io_update_state2 = IO_getIoState(self);

	switch (io_update_state2) {

	case IO_UPDATE_IDLE: {
		// check for a sequence to execute
		// only one sequence is manipulated at a time per I/O access method
		sequence = IO_getIOSequenceFromList(self);
		IO_setCurrentSequence(self, sequence);

		if (sequence != NULL ) {
			//sequence found, therefore transition to next state
			IO_setIoState(_self, IO_UPDATE_EXECUTE_COMMAND);
		}

		// otherwise, no sequence to transmit ... do nothing
		break;
	}

	case IO_UPDATE_EXECUTE_COMMAND: {

		// set next transition to WAITING ... assumes the wait is needed
		// the IO_processSequence() method can override with COMPLETE if needed
		// immediate action drivers should override
		// override is accomplished by firing IO_sequenceComplete_cb()
		// delayed action drivers should fire callback after that delay
		IO_setIoState(_self, IO_UPDATE_WAITING_COMMAND);

		// sends i/o sequence instructions to the respective driver
		// IO_processSequence() must manage any failures itself
		// the state will automatically transition to COMPLETE
		Access_processSequence(sequence);
		break;
	}

	case IO_UPDATE_WAITING_COMMAND: {
		// do nothing
		// wait for IO_commandExecuteComplete_cb() callback to transition state
		// WARNING:  Need watch dog timer to catch failed comm execution
		break;
	}

	case IO_UPDATE_SEQUENCE_COMPLETE: {
		// transition to IDLE on next call, regardless of callback status
		IO_setIoState(_self, IO_UPDATE_IDLE);

		// sequence processing is complete, fire sequence completed callback
		// WARNING: the callback should only set a variable or flag,
		// otherwise the processing call chain will not be able to unwind.
		io_cb_fnct localActionDone_cb = IO_getActionDone_cb(sequence);
		void *     localObjectPtr     = IO_getObjectPointer(sequence);
		if ( localActionDone_cb != NULL ) {
			localActionDone_cb(localObjectPtr);
		}

		break;
	}

	default: {
		IO_setIoState(_self, IO_UPDATE_IDLE);
		break; }

	}  //  end switch

	return self;
}


/****************************************************************************/
/********  data access methods  *********************************************/
/****************************************************************************/

/*****************************************/
/*****  set and get ioSequenceList  *******/

void * IO_getIoSequenceList(const void * _self)
{
	const struct IO * self = cast(IO, _self);
	if ( self == NULL ) { return NULL; }
	return self->ioSequenceList;
}

void * IO_setIoSequenceList(void * _self, void * _ioSequenceList)
{
	struct IO * self = cast(IO, _self);
	if ( self == NULL ) { return NULL; }
	self->ioSequenceList = _ioSequenceList;
	return _ioSequenceList;
}

/*****************************************/
/*****  set and get currentSequence  *******/

void * IO_getCurrentSequence(const void * _self)
{
	const struct IO * self = cast(IO, _self);
	if ( self == NULL ) { return 0; }
	return self->currentSequence;
}

void * IO_setCurrentSequence(void * _self, void * _currentSequence)
{
	struct IO * self = cast(IO, _self);
	if ( self == NULL ) { return 0; }
	self->currentSequence = _currentSequence;
	return _currentSequence;
}

/*****************************************/
/*****  set and get ioState  *******/

int IO_getIoState(const void * _self)
{
	const struct IO * self = cast(IO, _self);
	if ( self == NULL ) { return 0; }
	return self->ioState;
}

int IO_setIoState(void * _self, int _ioState)
{
	struct IO * self = cast(IO, _self);
	if ( self == NULL ) { return 0; }
	self->ioState = _ioState;
	return _ioState;
}

/************************************************/
/*****  set and get ActionComplete_cb  *******/

io_cb_fnct IO_getActionDone_cb(const void * _self)
{
	const struct IO * self = cast(IO, _self);
	if ( self == NULL ) { return NULL; }
	return self->actionDone_cb;
}

io_cb_fnct IO_setActionDone_cb(void * _self, io_cb_fnct _cb)
{
	struct IO * self = cast(IO, _self);
	if ( self == NULL ) { return NULL; }
	self->actionDone_cb = _cb;
	return _cb;
}

/*****************************************/
/*****  set and get objectPointer  *******/

void * IO_getObjectPointer(const void * _self)
{
	const struct IO * self = cast(IO, _self);
	if ( self == NULL ) { return NULL; }
	return self->objectPointer;
}

void * IO_setObjectPointer(void * _self, void * _objectPointer)
{
	struct IO * self = cast(IO, _self);
	if ( self == NULL ) { return NULL; }
	self->objectPointer = _objectPointer;
	return _objectPointer;
}

/*****************************************/
/*******  implementation methods  ********/

static void * implement_IO_io_copy(struct IO * _copyTo, const struct IO * _copyFrom)
{
	// copy master data members, except for PTRs and dynamic values
	// WARNING:  sequence List pointer is unique and will not be copied
	////IO_setIoSequenceList (_copyTo, IO_getIoSequenceList(_copyFrom));
	////IO_setCurrentSequence(_copyTo, IO_getCurrentSequence(_copyFrom));
	////IO_setIoState        (_copyTo, IO_getIoState(_copyFrom));
	IO_setActionDone_cb  (_copyTo, IO_getActionDone_cb(_copyFrom));
	IO_setObjectPointer  (_copyTo, IO_getObjectPointer(_copyFrom));
	return _copyTo;
}

static void * implement_IO_io_ctor(void * _self)
{
	// WARNING:  sequence List pointer is set in base ctor
	////IO_setIoSequenceList(_self, xxx) );
	IO_setCurrentSequence(_self, NULL);
	IO_setIoState        (_self, IO_UPDATE_UNKNOWN);
	IO_setActionDone_cb  (_self, NULL);
	IO_setObjectPointer  (_self, NULL);

	// insure the list of sequences is empty
	struct List * localListPtr = IO_getIoSequenceList(_self);
	int localItemCount = getItemCount(localListPtr);
	int i;
	for (i = 0; i < localItemCount; i++) { take(localListPtr); }
	return _self;
}

static void * implement_IO_io_dtor(struct IO * _self)
{
	//// WARNING:  delete/free the sequence List externally
	IO_setIoSequenceList (_self, NULL);
	IO_setCurrentSequence(_self, NULL);
	IO_setIoState        (_self, IO_UPDATE_UNKNOWN);
	IO_setActionDone_cb  (_self, NULL);
	IO_setObjectPointer  (_self, NULL);
	return _self;
}

static equal_t implement_IO_io_equal(const struct IO * _self,
		                             const struct IO * _comparisonObject)
{
	// Check each data member for congruence
	struct IO * self             = (void *)_self;
	struct IO * comparisonObject = (void *)_comparisonObject;

	// data pointers are unique and should not be included in the comparison
	////if( IO_getIoSequenceList(self) != IO_getIoSequenceList(comparisonObject) )
	////	{ return OBJECT_UNEQUAL; }

	////if( IO_getCurrentSequence(self) != IO_getCurrentSequence(comparisonObject) )
	////	{ return OBJECT_UNEQUAL; }

	////if( IO_getIoState(self) != IO_getIoState(comparisonObject) )
	////	{ return OBJECT_UNEQUAL; }

	if( IO_getActionDone_cb(self) != IO_getActionDone_cb(comparisonObject) )
		{ return OBJECT_UNEQUAL; }

	if( IO_getObjectPointer(self) != IO_getObjectPointer(comparisonObject) )
		{ return OBJECT_UNEQUAL; }

	// all data members are congruent
	return OBJECT_EQUAL;
}

static void * implement_IO_io_config(      struct IO * _self,
									 const struct IO * _master)
{
	return copy(_self, _master);
}

static puto_return_t implement_IO_io_puto(const struct IO * _self, FILE * _fp)
{
	// TODO: Add puto code in IO
	return 0;
}


static void * implement_IO_io_xxxx(struct IO * _self)
{
	// Update with actual code in
	return NULL;  // remove this fail when actual code is added
}



