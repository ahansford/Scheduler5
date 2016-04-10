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

struct List * ioSequenceList = NULL; // pointer to the List of sequences
struct AccessMEM * sequence  = NULL;       // pointer to the sequence currently being executed

// the internally managed IO state machine state used in IO_update()
io_update_state_t io_update_state = IO_UPDATE_UNKNOWN;


// MUST be called before other IO methods are called
void IO_init(struct List * _ioSequenceList)
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

	ioSequenceList = _ioSequenceList;

	// WARNING:  the IO state machine needs a time out check in
	// the IO_UPDATE_WAITING_COMMAND state in case the lower IO access
	// module does not execute as planned.  This should also include an
	// unwinding process to report the failure and attempted retrys.

	io_update_state = IO_UPDATE_IDLE;

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
	// TODO: needs to be removed from the listing, or converted to a list of
	//       lists where meme, I2C and SPI are covered
	//self->bufferPointer = va_arg(* app, io_data_t *);
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
	struct AccessMEM * self = cast(AccessMEM, _self);
	if( self == NULL ) { return NULL; } // fail

	struct List * localIoSequenceList = IO_getIoSequenceList(self);
	void * itemAddedToListPtr = add(localIoSequenceList, _ioSequence);
	//void * itemAddedToListPtr = add(ioSequenceList, self);

	// test for failure to add
	if ( itemAddedToListPtr == NULL ) { return NULL; } // fail

	return self; // success
}

void * IO_getIOSequenceFromList(void * _self)
{
	struct AccessMEM * self = cast(AccessMEM, _self);
	if( self == NULL ) { return NULL; } // fail
	struct List * localIoSequenceList = IO_getIoSequenceList(self);

	void * itemFromList = take(localIoSequenceList);
	return itemFromList;
}

void * IO_sequenceComplete_cb(void * _self)
{
	//TODO: when does this fire ??  who fires it ??
	io_update_state = IO_UPDATE_SEQUENCE_COMPLETE;
	return NULL;
}


/*********************/
/***** IO_update *****/

void * IO_update(void * _self)
{
	struct IO * self = cast(IO, _self);
	if (self == NULL ) { return NULL; }

	// TODO: io_state_t io_update_state = IO_getIoState(_self);
	// TODO: struct AccessMEM * sequence = cast( AccessMEM, IO_getSequencePtr(_self));
	// if (sequence == NULL ) {return NULL: }  // fail

	switch (io_update_state) {

	case IO_UPDATE_IDLE: {
		// check for a sequence to execute
		// only one sequence is manipulated at a time per I/O access method
		sequence = IO_getIOSequenceFromList(self);

		if (sequence != NULL ) {
			//sequence found, therefore transition to next state
			io_update_state = IO_UPDATE_EXECUTE_COMMAND;
			// TODO: IO_setSequencePtr(_self, sequence);
		}

		// otherwise, no sequence to transmit ... do nothing
		break;
	}

	case IO_UPDATE_EXECUTE_COMMAND: {

		// TODO:  IO_processSequence() may not be used anymore here
		// TODO:  Understand what replaces IO_processSequence()

		// set next transition to WAITING ... assumes the wait is needed
		// the IO_processSequence() method can override with COMPLETE if needed
		// immediate action drivers should override
		// override is accomplished by firing IO_sequenceComplete_cb()
		// delayed action drivers should fire callback after that delay
		io_update_state = IO_UPDATE_WAITING_COMMAND;

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
		io_update_state = IO_UPDATE_IDLE;

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
		io_update_state = IO_UPDATE_IDLE;
		break; }

	}  //  end switch

	// TODO: IO_setIoState(_self, io_update_state);
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
/*****  set and get bufferSize  *******/
/*
int IO_getBufferSize(const void * _self)
{
	const struct IO * self = cast(IO, _self);
	if ( self == NULL ) { return 0; }
	return self->bufferSize;
}

int IO_setBufferSize(void * _self, int _bufferSize)
{
	struct IO * self = cast(IO, _self);
	if ( self == NULL ) { return 0; }
	self->bufferSize = _bufferSize;
	return _bufferSize;
}
*/

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
	// WARNING:  data pointers are unique and should not be copied
	// WARNING:  buffer count May be unique and should not be copied
	////IO_setBufferPointer (_copyTo, IO_getBufferPointer(_copyFrom));
	////IO_setBufferSize(_copyTo, IO_getBufferSize(_copyFrom));
	IO_setActionDone_cb(_copyTo, IO_getActionDone_cb(_copyFrom));
	IO_setObjectPointer(_copyTo, IO_getObjectPointer(_copyFrom));
	return _copyTo;
}

static void * implement_IO_io_ctor(void * _self)
{
	// WARNING: buffer and size set externally where the size is known
	// WARNING: set in main ctor
	////IO_setBufferPointer(_self, xxx);
	////IO_setBufferSize(_self, xxx) );
	IO_setActionDone_cb(_self, NULL);
	IO_setObjectPointer(_self, NULL);
	return _self;
}

static void * implement_IO_io_dtor(struct IO * _self)
{
	//// WARNING:  delete/free the command buffer externally
	//IO_setBufferPointer (_self, NULL);
	//IO_setBufferSize    (_self, 0);
	IO_setActionDone_cb(_self, NULL);
	IO_setObjectPointer(_self, NULL);
	return _self;
}

static equal_t implement_IO_io_equal(const struct IO * _self,
		                             const struct IO * _comparisonObject)
{
	// Check each data member for congruence
	struct IO * self             = (void *)_self;
	struct IO * comparisonObject = (void *)_comparisonObject;

	// data pointers are unique and should not be included in the comparison
	////if( IO_getBufferPointer(self) != IO_getBufferPointer(comparisonObject) )
	////	{ return OBJECT_UNEQUAL; }

	////if( IO_getBufferSize(self) != IO_getBufferSize(comparisonObject) )
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



