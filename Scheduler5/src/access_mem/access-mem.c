/*
 * access-mem.c
 *
 *  Created on: Sep 30, 2015
 *      Author: Alan
 */

//#include <string.h>
#include "access-mem.h"
#include "access-mem-private.h"

#include "..\..\src\objects\objects.h"       // safety include
//#include "..\..\src\scheduler\scheduler.h"   // safety include
//#include "..\..\src\lists\lists.h"           // safety include

static void * implement_Access_MEM_ctor(void * _self);

static void * implement_Access_MEM_ctor(void * _self);
static void * implement_Access_MEM_dtor(        struct AccessMEM * _self);
static void * implement_Access_MEM_copy(        struct AccessMEM * _copyTo,
                                     const struct AccessMEM * _copyFrom);
static equal_t implement_Access_MEM_equal(const struct AccessMEM * _self,
		                             const struct AccessMEM * _comparisonObject);
static void * implement_Access_MEM_config(      struct AccessMEM * _self,
									 const struct AccessMEM * _master);
static puto_return_t implement_Access_MEM_puto(const struct AccessMEM * _self,
		                                                         FILE * _fp);

static void * implement_Access_MEM_addWriteCommandToSequence(struct AccessMEM * _self,
		                                                          access_data_t _value);
static void * implement_Access_MEM_processSequence(struct AccessMEM * _self);
//static void * implement_Access_MEM_xxxx(struct AccessMEM * _self);

static void Access_MEM_writeSingle    (void * _to, void * _from, int _writeCount);
static void Access_MEM_writeSequential(void * _to, void * _from, int _writeCount);
static void Access_MEM_readSingle     (void * _to, void * _from, int _readCount);
static void Access_MEM_readSequential (void * _to, void * _from, int _readCount);


/*****************************/
/**** INITIALIZATIONS  *******/

const void * AccessMEM		= NULL;
const void * AccessMEMClass = NULL;


// MUST be called before other access methods are called
void Access_init(void)
{
	// Insert the lines below for any derived subclasses
	//if (!ACCESS_MEM)      {ACCESS_MEM_init();}
	//if (!ACCESS_MEMClass) {ACCESS_MEM_init();}

	if (!AccessMEMClass) {
		 AccessMEMClass = new(Class,  		// should be "Class"
						  "AccessMEMClass",	// should be "SomethingClass"
						  Class,  		    // should be "Class"
						  sizeof(struct AccessMEMClass), // size of self
						  ctor, Access_MEMClass_ctor,	 // SomethingClass_ctor
						  0);  // Terminator
	}
	if (!AccessMEM) {
		 AccessMEM = new(AccessMEMClass,	   // SomethingClass from above
					 "AccessMEM",			   // name like "Something"
					 Object,  			       // "superclass(Something)"
					 sizeof(struct AccessMEM), // size of self
					 // Overloaded superclass functions
					 // Remember to call superclass->method
					 ctor,	 Access_MEM_ctor,  //Something_ctor
					 dtor,   Access_MEM_dtor,
					 copy,	 Access_MEM_copy,
					 equal,  Access_MEM_equal,
					 config, Access_MEM_config,
					 puto,	 Access_MEM_puto,
					 // New functions added in this class
					 // Do not call superclass->method
					 Access_addWriteCommandToSequence, Access_MEM_addWriteCommandToSequence,
					 Access_processSequence,		   Access_MEM_processSequence,
					 //ACCESS_xxxx,		               ACCESS_MEM_xxxx,

					 0);	// Terminator
	}

	// requires #include "..\..\src\lists\lists.h" to support class registry
	//implement_Access_MEM_registerKeyClasses();

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

	// TODO: created the command buffer once if pointer is NULL
	// if buffer is created in this constructor, then its deletion
	// must be kept separate from externally generated list buffers
	self->bufferPointer = va_arg(* app, access_data_t *);

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
	//* (voidf *) & self->Access_xxxx                      = NULL;

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
		/**/
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

		if (selector == (voidf)Access_addWriteCommandToSequence)
		    {* (voidf *) & self->Access_addWriteCommandToSequence = overloadedFunctionPtr;}

		if ( selector == (voidf)Access_processSequence )
		    { * (voidf *) & self->Access_processSequence  = overloadedFunctionPtr; }
/*
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
	if (self == NULL)                             {return NULL; } // fail

	// WARNING:  The command buffer should be deleted/freed externally

	// address local members first
	if ( implement_Access_MEM_dtor(self) == NULL) {return NULL; } // fail

	// call super method after local members are addressed
	// NOTE: classOf(self) calls into super will trigger looping
	if ( super_dtor(AccessMEM, _self) == NULL)    {return NULL; } // fail

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

	// WARNING:  If AcessMEM is a complex class, do not call superclass_copy

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
	if (self == NULL )             { return OBJECT_UNEQUAL; } // fail

	struct AccessMEM * comparisonObject = cast(AccessMEM, _comparisonObject);
	if (comparisonObject == NULL ) { return OBJECT_UNEQUAL; } // fail

	// WARNING: calling superclass in composite class is likely NOT correct
	//          complex classes should manage calls to superclass as needed
	//          within the implementation code

	// WARNING:  if AccessMEM is a complex class, do not call superclass_equal

	// NOTE: classOf(self) calls into super will trigger looping
	if( super_equal(AccessMEM, self, comparisonObject) == OBJECT_UNEQUAL)
		{ return OBJECT_UNEQUAL; } // fail

	// address local components
	return implement_Access_MEM_equal(_self, _comparisonObject);
}

void * Access_MEM_config(const void * _self, const void * _master)
{
	struct AccessMEM * self = cast(AccessMEM, _self);
	if ( self == NULL )   { return NULL; } // fail
	struct AccessMEM * master = cast(AccessMEM, _master);
	if ( master == NULL ) { return NULL; } // fail
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
/********  New functions for  class "AccessMEMClass"  ***********************/
/****************************************************************************/


/*************************************************/
/*****  ACCESS_addWriteCommandToSequence    ******/

void *  Access_addWriteCommandToSequence(void * _self, access_data_t _value)
{
	const struct AccessMEMClass * class = classOf( cast(AccessMEM, _self) );
	if ( class == NULL )
		{ return NULL; } // fail
	if ( class->Access_addWriteCommandToSequence == NULL )
		{ return NULL; } // fail
	return class->Access_addWriteCommandToSequence(_self, _value);
}

void * super_Access_addWriteCommandToSequence(const void *  _class,
		                                            void *  _self,
											  access_data_t _value)
{
	// verify that ACCESS_MEMClass is in the superclass chain of _class
	if ( ! isOfSuper(AccessMEM, _self) ) { return NULL; } // fail
	const struct AccessMEMClass * superclass = super(_class);
	if ( superclass == NULL )            { return NULL; } // fail
	if ( superclass->Access_addWriteCommandToSequence == NULL )
		                                 { return NULL; } // fail
	return superclass->Access_addWriteCommandToSequence(_self, _value);
}

void * Access_MEM_addWriteCommandToSequence(void * _self, access_data_t _value)
{
	struct AccessMEM * self = cast(AccessMEM, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Access_MEM_addWriteCommandToSequence(self, _value);
}

/********************************************/
/********  Access_processSequence    ********/

void *  Access_processSequence(void * _self)
{
	const struct AccessMEMClass * class = classOf( cast(AccessMEM, _self) );
	if ( class == NULL )                         { return NULL; } // fail
	if ( class->Access_processSequence == NULL ) { return NULL; } // fail
	return class->Access_processSequence(_self);
}

void * super_Access_processSequence(const void * _class, void * _self)
{
	// verify that AccessMEMClass is in the superclass chain of _class
	if ( ! isOfSuper(AccessMEMClass, _self) )         { return NULL; } // fail
	const struct AccessMEMClass * superclass = super(_class);
	if ( superclass == NULL )                         { return NULL; } // fail
	if ( superclass->Access_processSequence == NULL ) { return NULL; } // fail
	return superclass->Access_processSequence(_self);
}

void * Access_MEM_processSequence(void * _self)
{
	struct AccessMEM * self = cast(AccessMEM, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Access_MEM_processSequence(self);
}

/*************************************/
/***********  Access_xxxx    *************/
/*
void * Access_xxxx(void * _self)
{
	const struct AccessMEMClass * class = classOf( cast(MEM, _self) );
	if ( class == NULL )              { return NULL; } // fail
	if ( class->Access_xxxx == NULL ) { return NULL; } // fail
	return class->Access_xxxx(_self);
}

void * super_Access_xxxx(const void * _class, void * _self)
{
	// verify that AccessMEMClass is in the superclass chain of _class
	if ( ! isOfSuper(AccessMEMClass, _self) ) { return NULL; } // fail
	const struct AccessMEMClass * superclass = super(_class);
	if ( superclass == NULL )                 { return NULL; } // fail
	if ( superclass->Access_xxxx == NULL )    { return NULL; } // fail
	return superclass->Access_xxxx(_self);
}

void * Access_MEM_xxxx(void * _self)
{
	struct AccessMEM * self = cast(AccessMEM, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Access_MEM_xxxx(self);
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

	// check for count request that is larger than buffer
	int bufferSize = Access_getBufferSize(self);
	if ( _readCount > bufferSize ) { _readCount = 0; }  // fail

	self->readCount = _readCount;
	return _readCount;
}

/*************************************/
/*****  set and get writeCount  ******/

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
/*******  set and get bufferSize  ********/

int Access_getBufferSize(const void * _self)
{
	struct AccessMEM * self = cast(AccessMEM, _self);
	if ( self == NULL ) { return 0; }
	return self->bufferSize;
}

int Access_setBufferSize(void * _self, int _size)
{
	struct AccessMEM * self = cast(AccessMEM, _self);
	if ( self == NULL ) { return 0; }
	self->bufferSize = _size;
	return _size;
}

/************************************************/
/*******  set and get actionComplete_cb  ********/

access_cb_fnct Access_getActionDone_cb(const void * _self)
{
	const struct AccessMEM * self = cast(AccessMEM, _self);
	if ( self == NULL ) { return NULL; }
	return self->actionDone_cb;
}

access_cb_fnct Access_setActionDone_cb(void * _self, access_cb_fnct _cb)
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
/*****  set and get hardwareConfig  *******/

void * Access_getHardwareConfig(const void * _self)
{
	const struct AccessMEM * self = cast(AccessMEM, _self);
	if ( self == NULL ) { return NULL; }
	return self->hardwareConfig;
}

void * Access_setHardwareConfig(void * _self, void * _hardwareConfig)
{
	struct AccessMEM * self = cast(AccessMEM, _self);
	if ( self == NULL ) { return NULL; }
	self->hardwareConfig = _hardwareConfig;
	return _hardwareConfig;
}

/*****************************************/
/*******  implementation methods  ********/

static void * implement_Access_MEM_copy(struct AccessMEM * _copyTo, const struct AccessMEM * _copyFrom)
{
	// copy master data members, except for PTRs and dynamic values

	Access_setAddress       (_copyTo, Access_getAddress(_copyFrom));
	Access_setIOAction      (_copyTo, Access_getIOAction(_copyFrom));
	Access_setReadCount     (_copyTo, Access_getReadCount(_copyFrom));
	Access_setWriteCount    (_copyTo, Access_getWriteCount(_copyFrom));
	// data pointers are unique and should not be copied
	//Access_setBufferPointer(_copyTo, Access_getBufferPointer(_copyFrom));
	// buffer count May be unique and should not be copied
	//Access_setBufferSize(_copyTo, Access_getBufferSize(_copyFrom));
	Access_setActionDone_cb(_copyTo, Access_getActionDone_cb(_copyFrom));
	Access_setObjectPointer(_copyTo, Access_getObjectPointer(_copyFrom));
	Access_setHardwareConfig(_copyTo, Access_getHardwareConfig(_copyFrom));
	return _copyTo;

	//TODO: add test coverage for hardware pointer
}

static void * implement_Access_MEM_ctor(void * _self)
{
	Access_setAddress        (_self, NULL);
	Access_setIOAction       (_self, ACCESS_ACTION_UNKNOWN);
	Access_setReadCount      (_self, 0);
	Access_setWriteCount     (_self, 0);
	//Access_setBufferPointer(_self, bufferPointer);  // WARNING: set in main ctor
	Access_setBufferSize     (_self, 0);
	Access_setActionDone_cb  (_self, NULL);
	Access_setObjectPointer  (_self, NULL);
	Access_setHardwareConfig (_self, NULL);
	return _self;
}

static void * implement_Access_MEM_dtor(struct AccessMEM * _self)
{
	Access_setAddress      (_self, NULL);
	Access_setIOAction     (_self, ACCESS_ACTION_UNKNOWN);
	Access_setReadCount    (_self, 0);
	Access_setWriteCount   (_self, 0);
	Access_clearCommandBuffer(_self);
	// WARNING: TODO:  delete/free the command buffer externally
	Access_setBufferPointer (_self, NULL);
	Access_setBufferSize    (_self, 0);
	Access_setActionDone_cb (_self, NULL);
	Access_setObjectPointer (_self, NULL);
	Access_setHardwareConfig(_self, NULL);
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

	if( Access_getReadCount(self) != Access_getReadCount(comparisonObject) )
		{ return OBJECT_UNEQUAL; }

	if( Access_getWriteCount(self) != Access_getWriteCount(comparisonObject) )
		{ return OBJECT_UNEQUAL; }

	// data pointers are unique and should not be included in the comparison
	//if( Access_MEM_getBufferPointer(self) != Access_MEM_getBufferPointer(comparisonObject) )
	//	{ return OBJECT_UNEQUAL; }

	// buffer sizes are unique and should not be included in the comparison
	//if( Access_getBufferSize(self) != Access_getBufferSize(comparisonObject) )
	//	{ return OBJECT_UNEQUAL; }

	if( Access_getActionDone_cb(self) != Access_getActionDone_cb(comparisonObject) )
		{ return OBJECT_UNEQUAL; }

	if( Access_getObjectPointer(self) != Access_getObjectPointer(comparisonObject) )
		{ return OBJECT_UNEQUAL; }

	if( Access_getHardwareConfig(self) != Access_getHardwareConfig(comparisonObject) )
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

void * Access_clearCommandBuffer(void * _self)
{
	access_data_t * bufferPointer = Access_getBufferPointer(_self);
	if ( bufferPointer == NULL ) { return NULL; } // fail
	int i;
	for( i = 0; i < Access_getBufferSize(_self); i++ ) {
		bufferPointer[i] = (access_data_t)0;
	}
	Access_setWriteCount(_self, 0);
	Access_setReadCount(_self, 0);
	return _self;
}

static void * implement_Access_MEM_addWriteCommandToSequence(struct AccessMEM * _self, access_data_t _value)
{
	access_data_t * bufferPointer = Access_getBufferPointer(_self);
	if ( bufferPointer == NULL ) { return NULL; } // fail

	int writeCount = Access_getWriteCount(_self);
	if ( writeCount == Access_getBufferSize(_self) ) { return NULL; } // fail full

	// Add value to buffer and increment the writeCount
	bufferPointer[writeCount] = _value;
	writeCount++;
	Access_setWriteCount(_self, writeCount);

	return _self;
}

static void * implement_Access_MEM_processSequence(struct AccessMEM * _self)
{
	// local scope of data members does not support non-blocking operation
	// complete sequences are likely executed as a block
	// reentrant support is not likely needed

	access_read_write_t ioAction      = Access_getIOAction(_self);
	void *              address       = Access_getAddress(_self);
	int                 writeCount    = Access_getWriteCount(_self);
	int                 readCount     = Access_getReadCount(_self);
	access_data_t *     bufferAddress = Access_getBufferPointer(_self);
	int                 bufferSize    = Access_getBufferSize(_self);

	if ( address == NULL )        { return NULL; }
	if ( bufferAddress == NULL )  { return NULL; }
	if ( readCount > bufferSize ) { return NULL; }

	switch (ioAction) {

	case ACCESS_WRITE_SINGLE: {
		if (writeCount == 0 ) { return NULL; }
		Access_MEM_writeSingle(address, bufferAddress, writeCount);
		Access_sequenceComplete_cb(_self);
		break;
	}

	case ACCESS_WRITE_SEQUENTIAL: {
		if (writeCount == 0 ) { return NULL; }
		Access_MEM_writeSequential(address, bufferAddress, writeCount);
		Access_sequenceComplete_cb(_self);
		break;
	}

	case ACCESS_READ_SINGLE: {
		if (readCount == 0 ) { return NULL; }
		Access_MEM_readSingle(bufferAddress, address, readCount);
		Access_sequenceComplete_cb(_self);
		break;
	}

	case ACCESS_READ_SEQUENTIAL: {
		if (readCount == 0 ) { return NULL; }
		Access_MEM_readSequential(bufferAddress, address, readCount);
		Access_sequenceComplete_cb(_self);
		break;
	}

	case ACCESS_WRITE_READ_SINGLE: {
		if (writeCount == 0 ) { return NULL; }
		if (readCount == 0 )  { return NULL; }
		Access_MEM_writeSingle(address, bufferAddress, writeCount);
		Access_MEM_readSingle(bufferAddress, address, readCount);
		Access_sequenceComplete_cb(_self);
		break;
	}

	case ACCESS_WRITE_READ_SEQUENTIAL: {
		if (writeCount == 0 ) { return NULL; }
		if (readCount == 0 )  { return NULL; }
		Access_MEM_writeSequential(address, bufferAddress, writeCount);
		Access_MEM_readSequential(bufferAddress, address, readCount);
		Access_sequenceComplete_cb(_self);
		break;
	}

	default: {
		return NULL;
		break; }

	}// end switch

	// WARNING: do not fire Access_sequenceComplete_cb() here.
	// The individual sequence cases should each fire when needed.
	return _self;
}

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


void * Access_sequenceComplete_cb(struct AccessMEM * _self)
{
	//Goal:  call the action complete callback in the higher level module

	// verify the Access object pointer
	struct AccessMEM * self = cast(AccessMEM, _self);
	if(self == NULL) {return NULL; } // fail

	// verify the action complete pointer was set
	void * (* IO_CB_FNT)(void *) = self->actionDone_cb;
	void * IO_object = self->objectPointer;
	if(IO_CB_FNT == NULL) {return NULL; } // fail

	// fire the action complete callback
	IO_CB_FNT(IO_object);

	return self;
}

void * Access_sequenceIsValid(struct AccessMEM * _self)
{
	access_read_write_t ioAction      = Access_getIOAction(_self);
	void *              address       = Access_getAddress(_self);
	int                 writeCount    = Access_getWriteCount(_self);
	int                 readCount     = Access_getReadCount(_self);
	access_data_t *     bufferAddress = Access_getBufferPointer(_self);
	int                 bufferSize    = Access_getBufferSize(_self);

	if ( ioAction <= ACCESS_ACTION_UNKNOWN ) { printf("ioAction unknown-\n"); return NULL; }
	if ( ioAction > ACCESS_READ_SEQUENTIAL ) { printf("ioAction unknown+\n"); return NULL; }

	if ( ioAction >= ACCESS_WRITE_SINGLE ) {
			if ( address == NULL )       { printf("address NULL\n"); return NULL; }
			if ( bufferAddress == NULL ) { printf("bufferAddress NULL"); return NULL; }
			if ( writeCount > bufferSize ){ printf("writeCount larger than bufferSize\n"); return NULL; }
	}

	if ( ioAction >= ACCESS_WRITE_READ_SINGLE ) {
		if ( readCount > bufferSize )    { printf("readCount larger than bufferSize\n"); return NULL; }
	}

	return _self;
}

/*
static void * implement_Access_MEM_xxxx(struct AccessMEM * _self)
{
	// Update with actual code in
	return NULL;  // remove this fail when actual code is added
}
*/
