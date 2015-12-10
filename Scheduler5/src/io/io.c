/*
 * io.c
 *
 *  Created on: Sep 30, 2015
 *      Author: Alan
 */

//#include <string.h>
#include "io.h"
#include "io-private.h"                        // safety include
//#include "..\..\src\nodes\nodes.h"           // safety include
//#include "..\..\src\node_list\node-list.h"   // safety include
//#include "..\..\src\times\times.h"           // safety include
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


static void * implement_IO_io_addWriteValue(struct IO * _self,
		                                    io_data_t   _value);
static void * implement_IO_io_processSequence(struct IO * _self);
static void * implement_IO_io_xxxx(struct IO * _self);

static void IO_io_writeSingle    (void * _to, void * _from, int _writeCount);
static void IO_io_writeSequential(void * _to, void * _from, int _writeCount);
static void IO_io_readSingle     (void * _to, void * _from, int _readCount);
static void IO_io_readSequential (void * _to, void * _from, int _readCount);


/*****************************/
/**** INITIALIZATIONS  *******/

const void * IOClass = NULL;
const void * IO      = NULL;

struct List * ioSequenceList = NULL; // pointer to the List of sequences
struct IO *   sequence;  // pointer to the sequence currently being executed

// the internally managed IO state machine state used in IO_update()
io_update_state_t io_update_state;


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
					IO_addWriteCommandToSequence, IO_io_addWriteValue,
					IO_processSequence,		      IO_io_processSequence,
					IO_xxxx,		              IO_io_xxxx,

					0);	// Terminator
	}

	ioSequenceList = _ioSequenceList;

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
	self->bufferPointer = va_arg(* app, io_data_t *);
	//self->minute = va_arg(* app, minute_t);

	// modify return if data members are individually initialized above

	return implement_IO_io_ctor(self);
}

void * IOClass_ctor(void * _self, va_list *app)
{
	// Add superclass class data and methods
	struct IOClass * self = super_ctor(IOClass, _self, app);

	// Initialize new functions to default values
	typedef void (* voidf)();

	* (voidf *) & self->IO_addWriteValue   = NULL;
	* (voidf *) & self->IO_processSequence = NULL;
	* (voidf *) & self->IO_xxxx            = NULL;

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

		if (selector == (voidf)IO_addWriteCommandToSequence)
		    {* (voidf *) & self->IO_addWriteValue = overloadedFunctionPtr;}

		if ( selector == (voidf)IO_processSequence )
		    { * (voidf *) & self->IO_processSequence  = overloadedFunctionPtr; }

		if ( selector == (voidf)IO_xxxx )
		    { * (voidf *) & self->IO_xxxx = overloadedFunctionPtr; }

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


/*************************************************/
/***********  IO_addWriteSequence    *************/

void *  IO_addWriteCommandToSequence(void * _self, io_data_t _value)
{
	const struct IOClass * class = classOf( cast(IO, _self) );
	if ( class == NULL )                   { return NULL; } // fail
	if ( class->IO_addWriteValue == NULL ) { return NULL; } // fail
	return class->IO_addWriteValue(_self, _value);
}

void * super_IO_io_addWriteValue(const void * _class, void * _self, io_data_t _value)
{
	// verify that IOClass is in the superclass chain of _class
	if ( ! isOfSuper(IOClass, _self) )          { return NULL; } // fail
	const struct IOClass * superclass = super(_class);
	if ( superclass == NULL )                   { return NULL; } // fail
	if ( superclass->IO_addWriteValue == NULL ) { return NULL; } // fail
	return superclass->IO_addWriteValue(_self, _value);
}

void * IO_io_addWriteValue(void * _self, io_data_t _value)
{
	struct IO * self = cast(IO, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_IO_io_addWriteValue(self, _value);
}

/********************************************/
/*********  IO_processSequence    ***********/

void *  IO_processSequence(void * _self)
{
	const struct IOClass * class = classOf( cast(IO, _self) );
	if ( class == NULL )                     {
		printf("\nFAIL8: IO_processSequence ... class is NULL");
		return NULL; } // fail
	if ( class->IO_processSequence == NULL ) {
		printf("\nFAIL9: IO_processSequence ... class->IO_processSequence is NULL");
		return NULL;
	} // fail
	return class->IO_processSequence(_self);
}

void * super_IO_processSequence(const void * _class, void * _self)
{
	// verify that IOClass is in the superclass chain of _class
	if ( ! isOfSuper(IOClass, _self) )           { return NULL; } // fail
	const struct IOClass * superclass = super(_class);
	if ( superclass == NULL )                     { return NULL; } // fail
	if ( superclass->IO_processSequence == NULL ) { return NULL; } // fail
	return superclass->IO_processSequence(_self);
}

void * IO_io_processSequence(void * _self)
{
	struct IO * self = cast(IO, _self);
	if( self == NULL ) { printf("\nFAIL9: IO_io_processSequence ... self is NULL"); return NULL; } // fail
	return implement_IO_io_processSequence(self);
}

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

void * IO_addIOSequenceToList(void * _self)
{
	struct AccessMEM * self = cast(AccessMEM, _self);
	if( self == NULL ) { printf("\nFAIL5: IO_addIOSequenceToList NULL self\n"); return NULL; } // fail
	void * itemAddedToListPtr = add(ioSequenceList, self);
	if ( itemAddedToListPtr == NULL ) {
		printf("\nFAIL6: IO_addIOSequenceToList add operation\n");
	}
	return _self;
}

void * IO_getActionFromList(void)
{
	return take(ioSequenceList);
}

void IO_sequenceComplete_cb(void)
{
	io_update_state = IO_UPDATE_SEQUENCE_COMPLETE;
	return;
}

/*********************/
/***** IO_update *****/

void IO_update(void)
{
	printf("\nIO_update ... io_update_state: %i", io_update_state);

	switch (io_update_state) {

	case IO_UPDATE_UNKNOWN: {
		io_update_state = IO_UPDATE_IDLE;
		break;
	}

	case IO_UPDATE_IDLE: {
		// check for a sequence to execute
		// only one sequence is manipulated at a time
		sequence = IO_getActionFromList();
		printf("\nIO_update ... sequence: %p", sequence);
		if (sequence != NULL ) {
			//sequence found, therefore transition to next state
			io_update_state = IO_UPDATE_EXECUTE_COMMAND;
		}
		// no sequence to transmit ... do nothing
		break;
	}

	case IO_UPDATE_EXECUTE_COMMAND: {
		// set next transition to WAITING ... assumes the wait is needed
		// the IO_processSequence() method can override with COMPLETE if needed
		// immediate action drivers should override
		// override is accomplished by firing IO_sequenceComplete_cb()
		// delayed action drivers should fire callback after that delay
		io_update_state = IO_UPDATE_WAITING_COMMAND;

		// sends i/o sequence instructions to the respective driver
		// IO_processSequence() must manage any failures itself
		// the state will automatically transition to COMPLETE
		printf("\nIO_update ... IO_processSequence ... sequence:%p", sequence);
		Access_processSequence(sequence);
		break;
	}

	case IO_UPDATE_WAITING_COMMAND: {
		// do nothing
		// wait for IO_commandExecuteComplete_cb() callback to transition state
		break;
	}

	case IO_UPDATE_SEQUENCE_COMPLETE: {
		// transition to IDLE on next call, regardless of callback status
		io_update_state = IO_UPDATE_IDLE;

		//sequence processing is complete, fire the sequence callback
		io_cb_fnct callbackFunctionPointer = IO_getActionDone_cb(sequence);
		if ( callbackFunctionPointer != NULL ) {
			sequence->actionDone_cb(sequence->objectPointer);
		}

		break;
	}

	default: { break; }
	}  //  end switch

	return;
}


/****************************************************************************/
/********  data access methods  *********************************************/
/****************************************************************************/

/***********************************/
/*****  set and get address    *****/

void * IO_getAddress(const void * _self)
{
	const struct IO * self = cast(IO, _self);
	if ( self == NULL ) { return NULL; }
	return self->address;
}

void* IO_setAddress(void * _self, void * _address)
{
	struct IO * self = cast(IO, _self);
	if ( self == NULL ) { return NULL; }
	self->address = _address;
	return _address;
}

/************************************/
/*****  set and get ioAction    *****/

io_read_write_t IO_getIOAction(const void * _self)
{
	const struct IO * self = cast(IO, _self);
	if ( self == NULL ) { return IO_ACTION_UNKNOWN; }
	return self->ioAction;
}

io_read_write_t IO_setIOAction(void * _self, io_read_write_t _ioAction)
{
	struct IO * self = cast(IO, _self);
	if ( self == NULL ) { return IO_ACTION_UNKNOWN; }
	self->ioAction = _ioAction;
	return _ioAction;
}

/*************************************/
/*****  set and get readCount  *******/

int IO_getReadCount(const void * _self)
{
	const struct IO * self = cast(IO, _self);
	if ( self == NULL ) { return 0; }
	return self->readCount;
}

int IO_setReadCount(void * _self, int _readCount)
{
	struct IO * self = cast(IO, _self);
	if ( self == NULL ) { return 0; }
	self->readCount = _readCount;
	return _readCount;
}

/*************************************/
/*****  set and get writeCount  *******/

int IO_getWriteCount(const void * _self)
{
	const struct IO * self = cast(IO, _self);
	if ( self == NULL ) { return 0; }
	return self->writeCount;
}

int IO_setWriteCount(void * _self, int _writeCount)
{
	struct IO * self = cast(IO, _self);
	if ( self == NULL ) { return 0; }
	self->writeCount = _writeCount;
	return _writeCount;
}

/*****************************************/
/*****  set and get bufferPointer  *******/

void * IO_getBufferPointer(const void * _self)
{
	const struct IO * self = cast(IO, _self);
	if ( self == NULL ) { return NULL; }
	return self->bufferPointer;
}

void * IO_setBufferPointer(void * _self, void * _bufferPointer)
{
	struct IO * self = cast(IO, _self);
	if ( self == NULL ) { return NULL; }
	self->bufferPointer = _bufferPointer;
	return _bufferPointer;
}

/*****************************************/
/*****  set and get bufferSize  *******/

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

	IO_setAddress       (_copyTo, IO_getAddress(_copyFrom));
	IO_setIOAction      (_copyTo, IO_getIOAction(_copyFrom));
	IO_setReadCount     (_copyTo, IO_getReadCount(_copyFrom));
	IO_setWriteCount    (_copyTo, IO_getWriteCount(_copyFrom));
	// data pointers are unique and should not be copied
	//IO_setBufferPointer (_copyTo, IO_getBufferPointer(_copyFrom));
	// buffer count May be unique and should not be copied
	//IO_setBufferSize(_copyTo, IO_getBufferSize(_copyFrom));
	IO_setActionDone_cb(_copyTo, IO_getActionDone_cb(_copyFrom));
	IO_setObjectPointer (_copyTo, IO_getObjectPointer(_copyFrom));
	return _copyTo;
}

static void * implement_IO_io_ctor(void * _self)
{
	IO_setAddress       (_self, NULL);
	IO_setIOAction      (_self, IO_ACTION_UNKNOWN);
	IO_setReadCount     (_self, 0);
	IO_setWriteCount    (_self, 0);
	//IO_setBufferPointer (_self, bufferPointer);  // WARNING: set in main ctor
	void * localBufferPointer = IO_getBufferPointer(_self);
	IO_setBufferSize(_self, \
			sizeof(localBufferPointer)/sizeof(localBufferPointer[0]) );
	IO_setActionDone_cb(_self, NULL);
	IO_setObjectPointer (_self, NULL);
	return _self;
}

static void * implement_IO_io_dtor(struct IO * _self)
{
	IO_setAddress       (_self, NULL);
	IO_setIOAction      (_self, IO_ACTION_UNKNOWN);
	IO_setReadCount     (_self, 0);
	IO_setWriteCount    (_self, 0);
	// WANRING:  delete/free the command buffer externally
	IO_setBufferPointer (_self, NULL);
	IO_setBufferSize    (_self, 0);
	IO_setActionDone_cb(_self, NULL);
	IO_setObjectPointer (_self, NULL);
	return _self;
}

static equal_t implement_IO_io_equal(const struct IO * _self,
		                             const struct IO * _comparisonObject)
{
	// Check each data member for congruence
	struct IO * self             = (void *)_self;
	struct IO * comparisonObject = (void *)_comparisonObject;

	if( IO_getAddress(self) != IO_getAddress(comparisonObject) )
		{ return OBJECT_UNEQUAL; }

	if( IO_getIOAction(self) != IO_getIOAction(comparisonObject) )
		{ return OBJECT_UNEQUAL; }

	if( IO_getAddress(self) != IO_getAddress(comparisonObject) )
		{ return OBJECT_UNEQUAL; }

	if( IO_getReadCount(self) != IO_getReadCount(comparisonObject) )
		{ return OBJECT_UNEQUAL; }

	if( IO_getWriteCount(self) != IO_getWriteCount(comparisonObject) )
		{ return OBJECT_UNEQUAL; }

	// data pointers are unique and should not be included in the comparison
	//if( IO_getBufferPointer(self) != IO_getBufferPointer(comparisonObject) )
	//	{ return OBJECT_UNEQUAL; }

	if( IO_getBufferSize(self) != IO_getBufferSize(comparisonObject) )
		{ return OBJECT_UNEQUAL; }

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
/*
void * IO_clearCommandBuffer(void * _self)
{
	io_data_t * bufferPointer = IO_getBufferPointer(_self);
	if ( bufferPointer == NULL ) { return NULL; } // fail
	IO_setWriteCount(_self, 0);
	IO_setReadCount(_self, 0);
	return _self;
}
*/
static void * implement_IO_io_addWriteValue(struct IO * _self, io_data_t _value)
{
	io_data_t * bufferPointer = IO_getBufferPointer(_self);
	if ( bufferPointer == NULL ) { return NULL; } // fail

	int writeCount = IO_getWriteCount(_self);
	if ( writeCount == IO_getBufferSize(_self) ) { return NULL; }  // fail full

	// Add value to buffer and increment the writeCount
	bufferPointer[writeCount] = _value;
	writeCount++;
	IO_setWriteCount(_self, writeCount);

	return _self;  // remove this fail
}

static void * implement_IO_io_processSequence(struct IO * _self)
{
	// local scope of data members does not support non-blocking operation
	// complete sequences are likely executed as a block
	// reentrant support is not likely needed

	io_read_write_t ioAction      = Access_getIOAction(_self);
	void *          address       = Access_getAddress(_self);
	int             writeCount    = Access_getWriteCount(_self);
	int             readCount     = Access_getReadCount(_self);
	io_data_t *     bufferAddress = Access_getBufferPointer(_self);

	printf("\nimplement_IO_io_processSequence ioAction: %i", ioAction);

	/*
	switch (ioAction) {

	case IO_ACTION_UNKNOWN: {
		break;
	}

	case IO_WRITE_SINGLE: {
		IO_io_writeSingle(address, bufferAddress, writeCount);
		break;
	}

	case IO_WRITE_SEQUENTIAL: {
		IO_io_writeSequential(address, bufferAddress, writeCount);
		break;
	}

	case IO_READ_SINGLE: {
		IO_io_readSingle(bufferAddress, address, readCount);
		break;
	}

	case IO_READ_SEQUENTIAL: {
		IO_io_readSequential(bufferAddress, address, readCount);
		break;
	}

	case IO_WRITE_READ_SINGLE: {
		IO_io_writeSingle(address, bufferAddress, writeCount);
		IO_io_readSingle(bufferAddress, address, readCount);
		break;
	}

	case IO_WRITE_READ_SEQUENTIAL: {
		IO_io_writeSequential(address, bufferAddress, writeCount);
		IO_io_readSequential(bufferAddress, address, readCount);
		break;
	}

	default: { break; }

	}// end switch
	*/

	// fire the sequence complete callback since transfer activity is complete
	IO_sequenceComplete_cb();
	return _self;  // remove this fail
}

static void IO_io_writeSingle(void * _to, void * _from, int _writeCount)
{
	int i;
	io_data_t * to = _to;
	io_data_t * from = _from;

	for ( i = 0; i < _writeCount; i++ ) {
		*to = *from;
		from++;
	}
	return;
}

static void IO_io_writeSequential(void * _to, void * _from, int _writeCount)
{
	int i;
	io_data_t * to = _to;
	io_data_t * from = _from;

	for ( i = 0; i < _writeCount; i++ ) {
		*to = *from;
		to++;
		from++;
	}
	return;
}

static void IO_io_readSingle(void * _to, void * _from, int _readCount)
{
	int i;
	io_data_t * to = _to;
	io_data_t * from = _from;

	for ( i = 0; i < _readCount; i++ ) {
		*to = *from;
		to++;
	}
	return;
}

static void IO_io_readSequential(void * _to, void * _from, int _readCount)
{
	int i;
	io_data_t * to = _to;
	io_data_t * from = _from;


	for ( i = 0; i < _readCount; i++ ) {
		*to = *from;
		to++;
		from++;
	}
	return;
}


static void * implement_IO_io_xxxx(struct IO * _self)
{
	// Update with actual code in
	return NULL;  // remove this fail when actual code is added
}
