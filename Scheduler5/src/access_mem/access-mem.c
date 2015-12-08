/*
 * io.c
 *
 *  Created on: Sep 30, 2015
 *      Author: Alan
 */

//#include <string.h>
#include "access-mem.h"

#include "..\..\src\lists\lists.h"             // safety include
#include "..\..\src\objects\objects.h"         // safety include
#include "access-mem-private.h"                // safety include
//#include "..\..\src\scheduler\scheduler.h"   // safety include

static void * implement_Access_MEM_ctor(void * _self);
static void * implement_Access_MEM_addWriteCommandToSequence(struct AccessMEM * _self, access_data_t _value);

static void * implement_Access_MEM_ctor(void * _self);
static void * implement_Access_MEM_dtor(        struct AccessMEM * _self);
static void * implement_Access_MEM_copy(        struct AccessMEM * _copyTo,
                                     const struct AccessMEM * _copyFrom);
static equal_t implement_Access_MEM_equal(const struct AccessMEM * _self,
		                             const struct AccessMEM * _comparisonObject);
static void * implement_Access_MEM_config(      struct AccessMEM * _self,
									 const struct AccessMEM * _master);
static puto_return_t implement_Access_MEM_puto(const struct AccessMEM * _self, FILE * _fp);


static void * implement_Access_MEM_addWriteValue(struct AccessMEM * _self,
		                                    access_data_t   _value);

//static void * implement_Access_MEM_processSequence(struct AccessMEM * _self);
//static void * implement_Access_MEM_xxxx(struct AccessMEM * _self);

static void Access_MEM_writeSingle    (void * _to, void * _from, int _writeCount);
static void Access_MEM_writeSequential(void * _to, void * _from, int _writeCount);
static void Access_MEM_readSingle     (void * _to, void * _from, int _readCount);
static void Access_MEM_readSequential (void * _to, void * _from, int _readCount);


/*****************************/
/**** INITIALIZATIONS  *******/

const void * AccessMEM		= NULL;
const void * AccessMEMClass = NULL;

//struct List * ioSequenceList = NULL; // pointer to the List of sequence
//struct IO *   sequence;  // pointer to the sequence currently being executed

// the internally managed IO state machine state used in IO_update()
//io_update_state_t io_update_state;


// MUST be called before other IO methods are called
void Access_init(void)
{
	// Insert the lines below for any derived subclasses
	//if (!ACCESS_MEM)      {ACCESS_MEM_init();}
	//if (!ACCESS_MEMClass) {ACCESS_MEM_init();}

	// IO uses a List to manage sequences before they are executed
	if (!List)      {List_init();}
	if (!ListClass) {List_init();}

	if (! AccessMEMClass) {
		AccessMEMClass = new(Class,  		// should be "Class"
						"AccessMEMClass",	// should be "SomethingClass"
						Class,  		// should be "Class"
						sizeof(struct AccessMEMClass), //TODO: fix
						ctor, Access_MEMClass_ctor,	//SomethingClass_ctor
						0);  // Terminator
	}
	if (! AccessMEM) {
		AccessMEM = new(AccessMEMClass,			// SomethingClass from above
					"AccessMEM",				// name like "Something"
					Object,  			// "superclass(Something)"
					sizeof(struct AccessMEM),// size of self
					// Overloaded superclass functions
					// Remember to call superclass->method
					ctor,	Access_MEM_ctor,//Something_ctor
					dtor,   Access_MEM_dtor,
					copy,	Access_MEM_copy,
					equal,  Access_MEM_equal,
					config, Access_MEM_config,
					puto,	Access_MEM_puto,
					// New functions added in this class
					// Do not call superclass->method
					Access_addWriteCommandToSequence, Access_MEM_addWriteCommandToSequence,
					//ACCESS_processSequence,		    ACCESS_MEM_processSequence,
					//ACCESS_xxxx,		                ACCESS_MEM_xxxx,

					0);	// Terminator
	}

	//ioSequenceList = _ioSequenceList;

	// requires #include "..\..\src\lists\lists.h" to support class registry
	//implement_Sensor_registerKeyClasses();

	return;
}

/*****************************/
/****** Constructors  ********/

void * Access_MEM_ctor(void * _self, va_list * app)
{
	// Add superclass class data members
	struct AccessMEM * self = super_ctor(AccessMEM, _self, app);
	if ( self == NULL ) { return NULL; } // failed to construct super

	// overwrite data members with new data
	// Only uncomment if all data members will be specified in new() command
	// ... this seems like an undue burden on the user.  Leave commented out
	// ... numerous unit tests will need to be adapted if uncommented

	// TODO: created the single needed buffer once if missing
	//self->bufferPointer = va_arg(* app, io_data_t *);

	//self->minute = va_arg(* app, minute_t);

	// modify return if data members are individually initialized above

	return implement_Access_MEM_ctor(self);
}

void * Access_MEMClass_ctor(void * _self, va_list *app)
{
	// Add superclass class data and methods
	struct AccessMEMClass * self = super_ctor(AccessMEMClass, _self, app);

	// Initialize new functions to default values
	typedef void (* voidf)();

	* (voidf *) & self->Access_addWriteCommandToSequence = NULL;
	* (voidf *) & self->Access_processSequence           = NULL;
	* (voidf *) & self->Access_xxxx                      = NULL;

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
/*
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
*/

		// use form below to overload any new functions

		if (selector == (voidf)Access_addWriteCommandToSequence)
		    {* (voidf *) & self->Access_addWriteCommandToSequence = overloadedFunctionPtr;}
/*
		if ( selector == (voidf)Access_processSequence )
		    { * (voidf *) & self->Access_processSequence  = overloadedFunctionPtr; }

		if ( selector == (voidf)Access_xxxx )
		    { * (voidf *) & self->Access_xxxx = overloadedFunctionPtr; }
*/
	#ifdef va_copy
		va_end(ap);
	#endif

	}  // end while

	return self;
}


/*****************************/
/**** Overloaded Methods  ****/

void * Access_MEM_dtor(void * _self)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct AccessMEM * self = cast(AccessMEM, _self);
	if(self == NULL)                         {return NULL; } // fail

	// WARNING:  The command buffer should be deleted/freed externally

	// address local members first
	if ( implement_Access_MEM_dtor(self) == NULL) {return NULL; } // fail

	// call super method after local members are addressed
	// NOTE: classOf(self) calls into super will trigger looping
	if ( super_dtor(AccessMEM, _self) == NULL)      {return NULL; } // fail

	return _self;
}

void * Access_MEM_copy(void * _copyTo, const void * _copyFrom)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct AccessMEM * copyTo = cast(AccessMEM, _copyTo);
	if ( copyTo == NULL )   { return NULL; } //fail

	struct AccessMEM * copyFrom = cast(AccessMEM, _copyFrom);
	if ( copyFrom == NULL ) { return NULL; } // fail

	// WARNING: calling superclass in composite class is likely NOT correct
	//          complex classes should manage calls to superclass as needed
	//          within their implementation code

	// WARNING:  If IO is a complex class, do not call superclass_copy

	// call super method first
	// NOTE: classOf(copyToIO) calls into super will trigger looping
	if (super_copy(AccessMEM, copyTo, copyFrom) == NULL )
		{ return NULL; } // fail

	// address local components
	return implement_Access_MEM_copy(copyTo, copyFrom);
}

equal_t Access_MEM_equal(const void * _self, const void * _comparisonObject)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct AccessMEM * self = cast(AccessMEM, _self);
	if (self == NULL )            { return OBJECT_UNEQUAL; } // fail

	struct AccessMEM * comparisonObject = cast(AccessMEM, _comparisonObject);
	if (comparisonObject == NULL ) { return OBJECT_UNEQUAL; } // fail

	// WARNING: calling superclass in composite class is likely NOT correct
	//          complex classes should manage calls to superclass as needed
	//          within the implementation code

	// WARNING:  if IO is a complex class, do not call superclass_equal

	// NOTE: classOf(self) calls into super will trigger looping
	if( super_equal(AccessMEM, self, comparisonObject) == OBJECT_UNEQUAL)
		{ return OBJECT_UNEQUAL; } // fail

	// address local components
	return implement_Access_MEM_equal(_self, _comparisonObject);
}

void * Access_MEM_config(const void * _self, const void * _master)
{
	struct AccessMEM * self = cast(AccessMEM, _self);
	if ( self == NULL )   { return NULL; }          // fail
	struct AccessMEM * master = cast(AccessMEM, _master);
	if ( master == NULL ) { return NULL; }          // fail
	return implement_Access_MEM_config(self, _master);  // expected path
}

puto_return_t Access_MEM_puto(const void * _self, FILE * _fp)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct AccessMEM * self = cast(AccessMEM, _self);
	if (self == NULL ) { return PUTO_ERROR; } // fail

	// the call to super_puto() is managed within implement_Button_puto()
	return implement_Access_MEM_puto(self, _fp);
}


/****************************************************************************/
/********  New functions for  class "IOClass"  ******************************/
/****************************************************************************/


/*************************************************/
/***********  ACCESS_addWriteCommandToSequence    *************/

void *  Access_addWriteCommandToSequence(void * _self, access_data_t _value)
{
	const struct AccessMEMClass * class = classOf( cast(AccessMEM, _self) );
	if ( class == NULL )                   { return NULL; } // fail
	//if ( class-> == NULL ) { return NULL; } // fail
	//return class-(_self, _value);
	return NULL;
}

void * super_Access_addWriteCommandToSequence(const void * _class, void * _self, access_data_t _value)
{
	// verify that ACCESS_MEMClass is in the superclass chain of _class
	if ( ! isOfSuper(AccessMEM, _self) ) { return NULL; } // fail
	const struct AccessMEMClass * superclass = super(_class);
	if ( superclass == NULL )            { return NULL; } // fail
	//if ( superclass->Access_addWriteCommandToSequence == NULL ) { return NULL; } // fail
	//return superclass->Access_addWriteCommandToSequence(_self, _value);
	return NULL;
}

void * Access_MEM_addWriteCommandToSequence(void * _self, access_data_t _value)
{
	struct AccessMEM * self = cast(AccessMEM, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Access_MEM_addWriteCommandToSequence(self, _value);
}

/********************************************/
/*********  IO_processSequence    ***********/
/*
void *  IO_processSequence(void * _self)
{
	const struct AccessMEMClass * class = classOf( cast(AccessMEM, _self) );
	if ( class == NULL )                      { return NULL; } // fail
	if ( class->IO_processSequence == NULL ) { return NULL; } // fail
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
	if( self == NULL ) { return NULL; } // fail
	return implement_IO_io_processSequence(self);
}
*/
/*************************************/
/***********  IO_xxxx    *************/
/*
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
*/
/*************************************************************/
/***** store and get struct IO objects from ioActionList *****/
/*
void * IO_addIOSequenceToList(void * _self)
{
	struct IO * self = cast(IO, _self);
	if( self == NULL ) { return NULL; } // fail
	void * itemAddedToListPtr = add(ioSequenceList, self);
	if ( itemAddedToListPtr == NULL ) {
		printf("\nFAIL: IO_addIOSequenceToList add operation\n");
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
*/
/*********************/
/***** IO_update *****/
/*
void IO_update(void)
{
	switch (io_update_state) {

	case IO_UPDATE_UNKNOWN: {
		io_update_state = IO_UPDATE_IDLE;
		break;
	}

	case IO_UPDATE_IDLE: {
		// check for a sequence to execute
		// only one sequence is manipulated at a time
		sequence = IO_getActionFromList();
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
		IO_processSequence(sequence);
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
		io_cb_fnct callbackFunctionPointer = IO_get_actionDone_cb(sequence);
		if ( callbackFunctionPointer != NULL ) {
			sequence->actionDone_cb(sequence->objectPointer);
		}

		break;
	}

	default: { break; }
	}  //  end switch

	return;
}

*/
/****************************************************************************/
/********  data access methods  *********************************************/
/****************************************************************************/

/***********************************/
/*****  set and get address    *****/

void * Access_getAddress(const void * _self)
{
	const struct AccessMEM * self = cast(AccessMEM, _self);
	if ( self == NULL ) { return NULL; }
	return self->address;
}

void * Access_setAddress(void * _self, void * _address)
{
	struct AccessMEM * self = cast(AccessMEM, _self);
	if ( self == NULL ) { return NULL; }
	self->address = _address;
	return _address;
}

/************************************/
/*****  set and get ioAction    *****/

access_read_write_t Access_getIOAction(const void * _self)
{
	const struct AccessMEM * self = cast(AccessMEM, _self);
	if ( self == NULL ) { return ACCESS_ACTION_UNKNOWN; }
	return self->ioAction;
}

access_read_write_t Access_setIOAction(void * _self, access_read_write_t _ioAction)
{
	struct AccessMEM * self = cast(AccessMEM, _self);
	if ( self == NULL ) { return ACCESS_ACTION_UNKNOWN; }
	self->ioAction = _ioAction;
	return _ioAction;
}

/*************************************/
/*****  set and get readCount  *******/

int Access_getReadCount(const void * _self)
{
	const struct AccessMEM * self = cast(AccessMEM, _self);
	if ( self == NULL ) { return 0; }
	return self->readCount;
}

int Access_setReadCount(void * _self, int _readCount)
{
	struct AccessMEM * self = cast(AccessMEM, _self);
	if ( self == NULL ) { return 0; }
	self->readCount = _readCount;
	return _readCount;
}

/*************************************/
/*****  set and get writeCount  *******/

int Access_getWriteCount(const void * _self)
{
	const struct AccessMEM * self = cast(AccessMEM, _self);
	if ( self == NULL ) { return 0; }
	return self->writeCount;
}

int Access_setWriteCount(void * _self, int _writeCount)
{
	struct AccessMEM * self = cast(AccessMEM, _self);
	if ( self == NULL ) { return 0; }
	self->writeCount = _writeCount;
	return _writeCount;
}

/*****************************************/
/*****  set and get bufferPointer  *******/
/**/
void * Access_getBufferPointer(const void * _self)
{
	const struct AccessMEM * self = cast(AccessMEM, _self);
	if ( self == NULL ) { return NULL; }
	return self->bufferPointer;
}

void * Access_setBufferPointer(void * _self, void * _bufferPointer)
{
	struct AccessMEM * self = cast(AccessMEM, _self);
	if ( self == NULL ) { return NULL; }
	self->bufferPointer = _bufferPointer;
	return _bufferPointer;
}

/*****************************************/
/*****  set and get bufferSize  *******/

int Access_getBufferSize(const void * _self)
{
	const struct AccessMEM * self = cast(AccessMEM, _self);
	if ( self == NULL ) { return 0; }
	return self->bufferSize;
}

int Access_setBufferSize(void * _self, int _bufferSize)
{
	struct AccessMEM * self = cast(AccessMEM, _self);
	if ( self == NULL ) { return 0; }
	self->bufferSize = _bufferSize;
	return _bufferSize;
}


/************************************************/
/*****  set and get IO_actionComplete_cb  *******/

access_cb_fnct Access_get_actionDone_cb(const void * _self)
{
	const struct AccessMEM * self = cast(AccessMEM, _self);
	if ( self == NULL ) { return NULL; }
	return self->actionDone_cb;
}

access_cb_fnct Access_set_actionDone_cb(void * _self, access_cb_fnct _cb)
{
	struct AccessMEM * self = cast(AccessMEM, _self);
	if ( self == NULL ) { return NULL; }
	self->actionDone_cb = _cb;
	return _cb;
}

/*****************************************/
/*****  set and get objectPointer  *******/

void * Access_getObjectPointer(const void * _self)
{
	const struct AccessMEM * self = cast(AccessMEM, _self);
	if ( self == NULL ) { return NULL; }
	return self->objectPointer;
}

void * Access_setObjectPointer(void * _self, void * _objectPointer)
{
	struct AccessMEM * self = cast(AccessMEM, _self);
	if ( self == NULL ) { return NULL; }
	self->objectPointer = _objectPointer;
	return _objectPointer;
}

/*****************************************/
/*******  implementation methods  ********/
/*
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
	IO_set_actionDone_cb(_copyTo, IO_get_actionDone_cb(_copyFrom));
	IO_setObjectPointer (_copyTo, IO_getObjectPointer(_copyFrom));
	return _copyTo;
}
*/
static void * implement_Access_MEM_ctor(void * _self)
{
	Access_setAddress       (_self, NULL);
	Access_setIOAction      (_self, ACCESS_ACTION_UNKNOWN);
	Access_setReadCount     (_self, 0);
	Access_setWriteCount    (_self, 0);
	//Access_setBufferPointer (_self, bufferPointer);  // WARNING: set in main ctor
	void * localBufferPointer = Access_getBufferPointer(_self);
	Access_setBufferSize(_self, \
			sizeof(localBufferPointer)/sizeof(localBufferPointer[0]) );
	Access_set_actionDone_cb(_self, NULL);
	Access_setObjectPointer (_self, NULL);
	return _self;
}

static void * implement_Access_MEM_dtor(struct AccessMEM * _self)
{
	Access_setAddress       (_self, NULL);
	Access_setIOAction      (_self, ACCESS_ACTION_UNKNOWN);
	Access_setReadCount     (_self, 0);
	Access_setWriteCount    (_self, 0);
	// WARNING:  delete/free the command buffer externally
	Access_setBufferPointer (_self, NULL);
	Access_setBufferSize    (_self, 0);
	Access_set_actionDone_cb(_self, NULL);
	Access_setObjectPointer (_self, NULL);
	return _self;
}

static equal_t implement_Access_MEM_equal(const struct AccessMEM * _self,
		                             const struct AccessMEM * _comparisonObject)
{
	// Check each data member for congruence
	struct AccessMEM * self             = (void *)_self;
	struct AccessMEM * comparisonObject = (void *)_comparisonObject;

	if( Access_getAddress(self) != Access_getAddress(comparisonObject) )
		{ return OBJECT_UNEQUAL; }

	if( Access_getIOAction(self) != Access_getIOAction(comparisonObject) )
		{ return OBJECT_UNEQUAL; }

	if( Access_getAddress(self) != Access_getAddress(comparisonObject) )
		{ return OBJECT_UNEQUAL; }

	if( Access_getReadCount(self) != Access_getReadCount(comparisonObject) )
		{ return OBJECT_UNEQUAL; }

	if( Access_getWriteCount(self) != Access_getWriteCount(comparisonObject) )
		{ return OBJECT_UNEQUAL; }

	// data pointers are unique and should not be included in the comparison
	//if( Access_MEM_getBufferPointer(self) != Access_MEM_getBufferPointer(comparisonObject) )
	//	{ return OBJECT_UNEQUAL; }

	if( Access_getBufferSize(self) != Access_getBufferSize(comparisonObject) )
		{ return OBJECT_UNEQUAL; }

	if( Access_get_actionDone_cb(self) != Access_get_actionDone_cb(comparisonObject) )
		{ return OBJECT_UNEQUAL; }

	if( Access_getObjectPointer(self) != Access_getObjectPointer(comparisonObject) )
		{ return OBJECT_UNEQUAL; }

	// all data members are congruent
	return OBJECT_EQUAL;
}

static void * implement_Access_MEM_config(      struct AccessMEM * _self,
									 const struct AccessMEM * _master)
{
	return copy(_self, _master);
}

static puto_return_t implement_Access_MEM_puto(const struct AccessMEM * _self, FILE * _fp)
{
	// TODO: Add puto code in IO
	return 0;
}

void * implement_Access_MEM_clearCommandSequences(void * _self)
{
	access_data_t * bufferPointer = Access_getBufferPointer(_self);
	if ( bufferPointer == NULL ) { return NULL; } // fail
	Access_setWriteCount(_self, 0);
	Access_setReadCount(_self, 0);
	return _self;
}


static void * implement_Access_MEM_addWriteCommandToSequence(struct AccessMEM * _self, access_data_t _value)
//static void * implement_IO_io_addWriteValue(struct IO * _self, io_data_t _value)
{
	access_data_t * bufferPointer = Access_getBufferPointer(_self);
	if ( bufferPointer == NULL ) { return NULL; } // fail

	int writeCount = Access_getWriteCount(_self);
	if ( writeCount == Access_getBufferSize(_self) ) { return NULL; }  // fail full

	// Add value to buffer and increment the writeCount
	bufferPointer[writeCount] = _value;
	writeCount++;
	Access_setWriteCount(_self, writeCount);

	return _self;  // remove this fail
}
/*
static void * implement_IO_io_processSequence(struct IO * _self)
{
	// local scope of data members does not support non-blocking operation
	// complete sequences are likely executed as a block
	// reentrant support is not likely needed

	io_read_write_t ioAction      = IO_getIOAction(_self);
	void *          address       = IO_getAddress(_self);
	int             writeCount    = IO_getWriteCount(_self);
	int             readCount     = IO_getReadCount(_self);
	io_data_t *     bufferAddress = IO_getBufferPointer(_self);

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

	// fire the sequence complete callback since transfer activity is complete
	IO_sequenceComplete_cb();
	return _self;  // remove this fail
}
*/
static void Access_MEM_writeSingle(void * _to, void * _from, int _writeCount)
{
	int i;
	access_data_t * to = _to;
	access_data_t * from = _from;

	for ( i = 0; i < _writeCount; i++ ) {
		*to = *from;
		from++;
	}
	return;
}

static void Access_MEM_writeSequential(void * _to, void * _from, int _writeCount)
{
	int i;
	access_data_t * to = _to;
	access_data_t * from = _from;

	for ( i = 0; i < _writeCount; i++ ) {
		*to = *from;
		to++;
		from++;
	}
	return;
}

static void Access_MEM_readSingle(void * _to, void * _from, int _readCount)
{
	int i;
	access_data_t * to = _to;
	access_data_t * from = _from;

	for ( i = 0; i < _readCount; i++ ) {
		*to = *from;
		to++;
	}
	return;
}

static void Access_MEM_readSequential(void * _to, void * _from, int _readCount)
{
	int i;
	access_data_t * to = _to;
	access_data_t * from = _from;


	for ( i = 0; i < _readCount; i++ ) {
		*to = *from;
		to++;
		from++;
	}
	return;
}

/*
static void * implement_Access_MEM_xxxx(struct IO * _self)
{
	// Update with actual code in
	return NULL;  // remove this fail when actual code is added
}
*/
