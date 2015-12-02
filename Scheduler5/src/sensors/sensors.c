/*
 * sensors.c
 *
 *  Created on: Aug 31, 2015
 *      Author: alhansfo
 */

#include <stdlib.h>                    // for free() and malloc()
#include "sensors.h"
#include "sensors-private.h"           // safety include

#include "..\..\src\nodes\nodes.h"     // safety include
//#include "..\..\src\node_list\node-list.h"     // safety include
//#include "..\..\src\times\times.h"       // safety include
//#include "..\..\src\lists\lists.h"       // supports class registration
#include "..\..\src\objects\objects.h"     // safety include
#include "..\..\src\scheduler\scheduler.h" // safety include
#include "..\..\src\io\io.h"               // safety include
//#include "..\..\src\events\events.h" // safety include

/**************************************/
/***** Implementation Functions  ******/
static void * implement_Sensor_default_ctor(void * _self);
static void * implement_Sensor_default_dtor(      struct Sensor * _self);
static void * implement_Sensor_default_copy(      struct Sensor * _copyTo,
                                            const struct Sensor * _copyFrom);
static equal_t implement_Sensor_default_equal(const struct Sensor * _self,
		                                      const struct Sensor * _comparisonObject);
static void * implement_Sensor_default_config(struct Sensor * _self,
		                                      const struct Sensor * _master);
static puto_return_t implement_Sensor_default_puto(const struct Sensor * _self,
		                                                  FILE * _fp);

static void * implement_Sensor_default_writeDataToSensor(struct Sensor * _self);
static void * implement_Sensor_default_readDataFromSensor(struct Sensor * _self);

// only used by ctor and dtor
static void * implement_Sensor_default_clearAllValues(struct Sensor * _self);

static void * implement_Sensor_default_selectedDefaults(struct Sensor * _self);
static void * implement_Sensor_default_enablePower(struct Sensor * _self);
static void * implement_Sensor_default_alignAndConfig(struct Sensor * _self);
static void * implement_Sensor_default_startMeasurement(struct Sensor * _self);
static void * implement_Sensor_default_storeRawData(struct Sensor * _self);
static void * implement_Sensor_default_disablePower(struct Sensor * _self);
static void * implement_Sensor_default_processRawData(struct Sensor * _self);
static void * implement_Sensor_default_checkAlarms(struct Sensor * _self);

static void * implement_Sensor_callAlarmTriggered_CB(struct Sensor * _self);
static void * implement_Sensor_callReportReady_CB(struct Sensor * _self);

static void Sensor_checkAsyncFlag(struct Sensor * _self);

/*****************************/
/**** INITIALIZATIONS  *******/

const void * SensorClass = NULL;
const void * Sensor      = NULL;

// MUST be called before other event controller methods are called
void Sensor_init(void)
{
	// Insert the lines below for any derived subclasses
	//if (!Sensor)      {Sensor_init();}
	//if (!SensorClass) {Sensor_init();}

	if (!Node)      {Node_init();}
	if (!NodeClass) {Node_init();}

	//if (!IO)      {IO_init();}
	//if (!IOClass) {IO_init();}

	if (! SensorClass) {
		SensorClass = new(Class,  		// should be "Class"
						"SensorClass",	// should be "SomethingClass"
						Class,  		// should be "Class"
						sizeof(struct SensorClass),
						ctor, SensorClass_ctor,	//SomethingClass_ctor
						0);  // Terminator
	}
	if (! Sensor) {
		Sensor = new(SensorClass,		    // SomethingClass from above
					"Sensor",		        // name like "Something"
					Object,  			    // "superclass(Something)"
					sizeof(struct Sensor),  // size of self
					// Overloaded superclass functions
					// Remember to call superclass->method
					ctor,	Sensor_default_ctor,//Something_ctor
					dtor,   Sensor_default_dtor,
					copy,	Sensor_default_copy,
					equal,	Sensor_default_equal,
					config, Sensor_config,
					puto,	Sensor_default_puto,
					// New functions added in this class
					// Do not call superclass->method
					Sensor_writeDataToSensor,	Sensor_default_writeDataToSensor,
					Sensor_readDataFromSensor,	Sensor_default_readDataFromSensor,

					Sensor_loadDefaults,		Sensor_default_loadDefaults,
					Sensor_enablePower,			Sensor_default_enablePower,
					Sensor_alignAndConfig,		Sensor_default_alignAndConfig,
					Sensor_startMeasurement,	Sensor_default_startMeasurement,
					Sensor_storeRawData,		Sensor_default_storeRawData,
					Sensor_disablePower,		Sensor_default_disablePower,
					Sensor_processRawData,		Sensor_default_processRawData,
					Sensor_checkAlarms,			Sensor_default_checkSensorAlarms,

					0);	// Terminator
	}

	// requires #include "..\..\src\lists\lists.h" to support class registry
	//implement_Sensor_registerKeyClasses();

	return;
}

/*****************************/
/****** Constructors  ********/

void * Sensor_default_ctor(void * _self, va_list * app)
{
	// Add superclass class data members
	struct Sensor * self = super_ctor(Sensor, _self, app);
	if ( self == NULL ) { return NULL; } // failed to construct super

	// overwrite data members with new data
	// Only uncomment if all data members will be specified in new() command
	// ... this seems like an undue burden on the user.  Leave commented out
	// ... numerous unit tests will need to be adapted if uncommented
	//self->second = va_arg(* app, second_t);
	//self->minute = va_arg(* app, minute_t);

	// modify return if data members are individually initialized above

	return implement_Sensor_default_ctor(self);
}

void * SensorClass_ctor(void * _self, va_list *app)
{
	// Add superclass class data and methods
	struct SensorClass * self = super_ctor(SensorClass, _self, app);

	// Initialize new functions to default values
	typedef void (* voidf)();

	* (voidf *) & self->writeDataToSensor  = NULL;
	* (voidf *) & self->readDataFromSensor = NULL;
	* (voidf *) & self->loadDefaults       = NULL;
	* (voidf *) & self->enablePower        = NULL;
	* (voidf *) & self->alignAndConfig     = NULL;
	* (voidf *) & self->startMeasurement   = NULL;
	* (voidf *) & self->storeRawData       = NULL;
	* (voidf *) & self->disablePower       = NULL;
	* (voidf *) & self->processRawData     = NULL;
	* (voidf *) & self->checkAlarms        = NULL;

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

		if (selector == (voidf)Sensor_writeDataToSensor)
		      {* (voidf *) & self->writeDataToSensor = overloadedFunctionPtr;}

		if ( selector == (voidf)Sensor_readDataFromSensor )
		      { * (voidf *) & self->readDataFromSensor  = overloadedFunctionPtr; }

		if ( selector == (voidf)Sensor_loadDefaults )
		      { * (voidf *) & self->loadDefaults = overloadedFunctionPtr; }

		if ( selector == (voidf)Sensor_enablePower )
		      { * (voidf *) & self->enablePower = overloadedFunctionPtr; }

		if ( selector == (voidf)Sensor_alignAndConfig )
		      { * (voidf *) & self->alignAndConfig = overloadedFunctionPtr; }

		if ( selector == (voidf)Sensor_startMeasurement )
		      { * (voidf *) & self->startMeasurement = overloadedFunctionPtr; }

		if ( selector == (voidf)Sensor_storeRawData)
		      { * (voidf *) & self->storeRawData = overloadedFunctionPtr; }

		if ( selector == (voidf)Sensor_disablePower )
		      { * (voidf *) & self->disablePower = overloadedFunctionPtr; }

		if ( selector == (voidf)Sensor_processRawData)
		      { * (voidf *) & self->processRawData = overloadedFunctionPtr; }

		if ( selector == (voidf)Sensor_checkAlarms )
		      { * (voidf *) & self->checkAlarms = overloadedFunctionPtr; }

	#ifdef va_copy
		va_end(ap);
	#endif

	}  // end while

	return self;
}


/*****************************/
/**** Overloaded Methods  ****/

void * Sensor_default_dtor(void * _self)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct Sensor * self = cast(Sensor, _self);
	if( self == NULL ) {return NULL; } // fail

	// address local members first
	if ( implement_Sensor_default_dtor(self) == NULL) {return NULL; } // fail

	// call super method after local members are addressed
	// NOTE: classOf(self) calls into super will trigger looping
	if ( super_dtor(Sensor, _self) == NULL)   {return NULL; } // fail

	return _self;
}

void * Sensor_default_copy(void * _copyTo, const void * _copyFrom)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct Sensor * copyTo = cast(Sensor, _copyTo);
	if ( copyTo == NULL )   { return NULL; } //fail

	struct Sensor * copyFrom = (void *)_copyFrom;
	if ( copyFrom == NULL ) { return NULL; } // fail

	// WARNING: calling superclass in composite class is likely NOT correct
	//          complex classes should manage calls to superclass as needed
	//          within their implementation code

	// WARNING:  If Sensor is a complex class, do not call superclass_copy

	// call super method first
	// NOTE: classOf(copyTo) calls into super will trigger looping
	if (super_copy(Sensor, copyTo, copyFrom) == NULL )
		{ return NULL; } // fail

	// address local components
	return implement_Sensor_default_copy(copyTo, copyFrom);
}

equal_t Sensor_default_equal(const void * _self, const void * _comparisonObject)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct Sensor * self = cast(Sensor, _self);
	if (self == NULL )              { return OBJECT_UNEQUAL; } // fail

	struct Sensor * comparisonObject = cast(Sensor, _comparisonObject);
	if (comparisonObject == NULL )  { return OBJECT_UNEQUAL; } // fail

	// WARNING: calling superclass in composite class is likely NOT correct
	//          complex classes should manage calls to superclass as needed
	//          within their implementation code

	// WARNING:  if Sensor is a complex class, do not call superclass_equal

	// NOTE: classOf(self) calls into super will trigger looping
	if( super_equal(Sensor, self, comparisonObject) == OBJECT_UNEQUAL)
		{ return OBJECT_UNEQUAL; } // fail

	// address local components
	return implement_Sensor_default_equal(_self, _comparisonObject);
}

void * Sensor_config(const void * _self, const void * _master)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL )   { return NULL; }  // fail
	struct Sensor * master = cast(Sensor, _master);
	if ( master == NULL ) { return NULL; }  // fail
	return implement_Sensor_default_config(self, _master);  // expected path
}

puto_return_t Sensor_default_puto(const void * _self, FILE * _fp)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct Sensor * self = cast(Sensor, _self);
	if (self == NULL ) { return PUTO_ERROR; } // fail

	// the call to super_puto() is managed within implement_Button_puto()
	return implement_Sensor_default_puto(self, _fp);
}


/****************************************************************************/
/********  New functions for  class "SensorClass"  **************************/
/****************************************************************************/



/**************************************/
/*******  writeDataToSensor   *********/

void *  Sensor_writeDataToSensor(void * _self)
{
	const struct SensorClass * class = classOf( cast(Sensor, _self) );
	if ( class == NULL )                    { return NULL; } // fail
	if ( class->writeDataToSensor == NULL ) { return NULL; } // fail
	return class->writeDataToSensor(_self);
}

void * super_Sensor_writeDataToSensor(const void * _class, void * _self)
{
	// verify that SensorClass is in the superclass chain of _class
	if ( ! isOfSuper(SensorClass, _self) )       { return NULL; } // fail
	const struct SensorClass * superclass = super(_class);
	if ( superclass == NULL )                    { return NULL; } // fail
	if ( superclass->writeDataToSensor == NULL ) { return NULL; } // fail
	return superclass->writeDataToSensor(_self);
}

void * Sensor_default_writeDataToSensor(void * _self)
{
	struct Sensor * self = cast(Sensor, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Sensor_default_writeDataToSensor(self);
}

/**************************************/
/******  readDataFromSensor    ********/

void *  Sensor_readDataFromSensor (void * _self)
{
	const struct SensorClass * class = classOf( cast(Sensor, _self) );
	if ( class == NULL )                      { return NULL; } // fail
	if ( class->readDataFromSensor  == NULL ) { return NULL; } // fail
	return class->readDataFromSensor (_self);
}

void * super_Sensor_default_readDataFromSensor(const void * _class, void * _self)
{
	// verify that SensorClass is in the superclass chain of _class
	if ( ! isOfSuper(SensorClass, _self) )         { return NULL; } // fail
	const struct SensorClass * superclass = super(_class);
	if ( superclass == NULL )                      { return NULL; } // fail
	if ( superclass->readDataFromSensor  == NULL ) { return NULL; } // fail
	return superclass->readDataFromSensor(_self);
}

void * Sensor_default_readDataFromSensor(void * _self)
{
	struct Sensor * self = cast(Sensor, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Sensor_default_readDataFromSensor(self);
}


/**************************************/
/******  Sensor_loadDefaults    *******/

void *  Sensor_loadDefaults(void * _self)
{
	const struct SensorClass * class = classOf( cast(Sensor, _self) );
	if ( class == NULL )               { return NULL; } // fail
	if ( class->loadDefaults == NULL ) { return NULL; } // fail
	return class->loadDefaults(_self);
}

void * super_Sensor_loadDefaults(const void * _class, void * _self)
{
	// verify that SensorClass is in the superclass chain of _class
	if ( ! isOfSuper(SensorClass, _self) )  { return NULL; } // fail
	const struct SensorClass * superclass = super(_class);
	if ( superclass == NULL )               { return NULL; } // fail
	if ( superclass->loadDefaults == NULL ) { return NULL; } // fail
	return superclass->loadDefaults(_self);
}

void * Sensor_default_loadDefaults(void * _self)
{
	struct Sensor * self = cast(Sensor, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Sensor_default_selectedDefaults(self);
}


/*************************************/
/******  Sensor_EnablePower   ********/

void * Sensor_enablePower(void * _self)
{
	const struct SensorClass * class = classOf( cast(Sensor, _self) );
	if ( class == NULL )              { return NULL; } // fail
	if ( class->enablePower == NULL ) { return NULL; } // fail
	return class->enablePower(_self);
}

void * super_Sensor_enablePower(const void * _class, void * _self)
{
	// verify that SensorClass is in the superclass chain of _class
	if ( ! isOfSuper(SensorClass, _self) ) { return NULL; } // fail
	const struct SensorClass * superclass = super(_class);
	if ( superclass == NULL )              { return NULL; } // fail
	if ( superclass->enablePower == NULL ) { return NULL; } // fail
	return superclass->enablePower(_self);
}

void * Sensor_default_enablePower (void * _self)
{
	struct Sensor * self = cast(Sensor, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Sensor_default_enablePower(self);
}


/*************************************/
/********  alignAndConfig   ************/

void *  Sensor_alignAndConfig(void * _self)
{
	const struct SensorClass * class = classOf( cast(Sensor, _self) );
	if ( class == NULL )                 { return NULL; } // fail
	if ( class->alignAndConfig == NULL ) { return NULL; } // fail
	return class->alignAndConfig(_self);
}

void * super_Sensor_alignAndConfig(const void * _class, void * _self)
{
	// verify that SensorClass is in the superclass chain of _class
	if ( ! isOfSuper(SensorClass, _self) )   { return NULL; } // fail
	const struct SensorClass * superclass = super(_class);
	if ( superclass == NULL )                { return NULL; } // fail
	if ( superclass->alignAndConfig== NULL ) { return NULL; } // fail
	return superclass->alignAndConfig(_self);
}

void * Sensor_default_alignAndConfig(void * _self)
{
	struct Sensor * self = cast(Sensor, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Sensor_default_alignAndConfig(self);
}

/**************************************/
/********  startMeasurement   *********/

void *  Sensor_startMeasurement(void * _self)
{
	const struct SensorClass * class = classOf( cast(Sensor, _self) );
	if ( class == NULL )                   { return NULL; } // fail
	if ( class->startMeasurement == NULL ) { return NULL; } // fail
	return class->startMeasurement(_self);
}

void * super_Sensor_startMeasurement(const void * _class, void * _self)
{
	// verify that SensorClass is in the superclass chain of _class
	if ( ! isOfSuper(SensorClass, _self) )      { return NULL; } // fail
	const struct SensorClass * superclass = super(_class);
	if ( superclass == NULL )                   { return NULL; } // fail
	if ( superclass->startMeasurement == NULL ) { return NULL; } // fail
	return superclass->startMeasurement(_self);
}

void * Sensor_default_startMeasurement(void * _self)
{
	struct Sensor * self = cast(Sensor, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Sensor_default_startMeasurement(self);
}
/**/
/**************************************/
/*******  storeRawData   *********/
/**/
void *  Sensor_storeRawData(void * _self)
{
	const struct SensorClass * class = classOf( cast(Sensor, _self) );
	if ( class == NULL )               { return NULL; } // fail
	if ( class->storeRawData == NULL ) { return NULL; } // fail
	return class->storeRawData(_self);
}

void * super_Sensor_storeRawData(const void * _class, void * _self)
{
	// verify that SensorClass is in the superclass chain of _class
	if ( ! isOfSuper(SensorClass, _self) )  { return NULL; } // fail
	const struct SensorClass * superclass = super(_class);
	if ( superclass == NULL )               { return NULL; } // fail
	if ( superclass->storeRawData == NULL ) { return NULL; } // fail
	return superclass->storeRawData(_self);
}

void * Sensor_default_storeRawData(void * _self)
{
	struct Sensor * self = cast(Sensor, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Sensor_default_storeRawData(self);
}

/**************************************/
/*******  disablePower   *********/

void *  Sensor_disablePower(void * _self)
{
	const struct SensorClass * class = classOf( cast(Sensor, _self) );
	if ( class == NULL )               { return NULL; } // fail
	if ( class->disablePower == NULL ) { return NULL; } // fail
	return class->disablePower(_self);
}

void * super_Sensor_disablePower(const void * _class, void * _self)
{
	// verify that SensorClass is in the superclass chain of _class
	if ( ! isOfSuper(SensorClass, _self) )  { return NULL; } // fail
	const struct SensorClass * superclass = super(_class);
	if ( superclass == NULL )               { return NULL; } // fail
	if ( superclass->disablePower == NULL ) { return NULL; } // fail
	return superclass->disablePower(_self);
}

void * Sensor_default_disablePower(void * _self)
{
	struct Sensor * self = cast(Sensor, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Sensor_default_disablePower(self);
}

/**************************************/
/*******  processRawSensorData   *********/

void *  Sensor_processRawData(void * _self)
{
	const struct SensorClass * class = classOf( cast(Sensor, _self) );
	if ( class == NULL )                 { return NULL; } // fail
	if ( class->processRawData == NULL ) { return NULL; } // fail
	return class->processRawData(_self);
}

void * super_Sensor_processRawData(const void * _class, void * _self)
{
	// verify that SensorClass is in the superclass chain of _class
	if ( ! isOfSuper(SensorClass, _self) )    { return NULL; } // fail
	const struct SensorClass * superclass = super(_class);
	if ( superclass == NULL )                 { return NULL; } // fail
	if ( superclass->processRawData == NULL ) { return NULL; } // fail
	return superclass->processRawData(_self);
}

void * Sensor_default_processRawData(void * _self)
{
	struct Sensor * self = cast(Sensor, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Sensor_default_processRawData(self);
}

/**************************************/
/*******  checkSensorAlarms   *********/

void *  Sensor_checkAlarms(void * _self)
{
	const struct SensorClass * class = classOf( cast(Sensor, _self) );
	if ( class == NULL )              { return NULL; } // fail
	if ( class->checkAlarms == NULL ) { return NULL; } // fail
	return class->checkAlarms(_self);
}

void * super_Sensor_checkSensorAlarms(const void * _class, void * _self)
{
	// verify that SensorClass is in the superclass chain of _class
	if ( ! isOfSuper(SensorClass, _self) ) { return NULL; } // fail
	const struct SensorClass * superclass = super(_class);
	if ( superclass == NULL )              { return NULL; } // fail
	if ( superclass->checkAlarms == NULL ) { return NULL; } // fail
	return superclass->checkAlarms(_self);
}

void * Sensor_default_checkSensorAlarms(void * _self)
{
	struct Sensor * self = cast(Sensor, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Sensor_default_checkAlarms(self);
}



/*************************************************/
/*********** Implementation Procedures ***********/

static void * implement_Sensor_default_ctor(void * _self)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return NULL; }  // fail

	// complete initialization of values is only used for create and destroy
	implement_Sensor_default_clearAllValues(self);

	// create IO access structure and its data objects
	// allocate memory for the command buffer and assign the pointer
	command_t * commandBufferPTR =
			(command_t *)malloc(sizeof(command_t)*SENSOR_DEFAULT_MAX_COMMANDS);
	if ( commandBufferPTR == NULL ) { return NULL; }  // fail

	// WARNING TODO:  internal IO management List needed for IO_init() should
	//                ... be created external to Sensor.  This weakness should
	//                ... be corrected
	// create new IO access structure object
	struct IO * ioStructPointer = new(SENSOR_DEFAULT_IO_TYPE, commandBufferPTR);
	Sensor_setIoStructPointer(_self, ioStructPointer);

	// set the IO address to NULL for safety
	// Ordinary drivers will select a non-NULL value
	// can be overwritten externally for testing
	ioStructPointer->address = SENSOR_DEFAULT_ADDRESS;

	// generate raw data object
	struct Node * rawDataPointer = new(Node);
	if ( rawDataPointer == NULL ) { return NULL; }  // fail
	Sensor_setRawDataPointer(_self, rawDataPointer);

	// generate final data object
	struct Node * finalDataPointer = new(Node);
	if ( finalDataPointer == NULL ) { return NULL; }  // fail
	Sensor_setFinalDataPointer(_self, finalDataPointer);

	// assign two linked nodes to alarm data pointer
	// lowerPrimaryLimit is used for simple above/below tests
	// upperSecondaryLimit is added for between/outside tests
	struct Node * lowerPrimaryNodePTR = new(Node);
	if ( lowerPrimaryNodePTR == NULL ) { return NULL; }  // fail
	Sensor_setAlarmLevelsPointer(self, lowerPrimaryNodePTR);

	// link the secondary node to the primary
	struct Node * upperSecondaryNodePTR = new(Node);
	if ( upperSecondaryNodePTR == NULL ) { return NULL; }  // fail
	setLinkedNode(lowerPrimaryNodePTR, upperSecondaryNodePTR);

	return self;  // expected path
}

static void * implement_Sensor_default_dtor(struct Sensor * _self)
{
	/**/
	// delete strict IO access object and set pointer to NULL
	struct IO * ioStructPointer = Sensor_getIoStructPointer(_self);
	if ( ioStructPointer != NULL ) {
		delete(ioStructPointer);
		Sensor_setIoStructPointer(_self, NULL);
		if ( Sensor_getIoStructPointer(_self) != NULL )
			{ return NULL; } // fail
	}

	// delete raw data pointer object
	struct Node * rawDataPtr = Sensor_getRawDataPointer(_self);
	if ( rawDataPtr != NULL ) {
		Sensor_setRawDataPointer(_self, safeDelete(rawDataPtr));
		if ( Sensor_getRawDataPointer(_self) != NULL )
			{ return NULL; } // fail
	}

	// delete final data object
	struct Node * finalDataPtr = Sensor_getFinalDataPointer(_self);
	if ( finalDataPtr != NULL ) {
		Sensor_setFinalDataPointer(_self, safeDelete(finalDataPtr));
		if ( Sensor_getFinalDataPointer(_self) != NULL )
			{ return NULL; } // fail
	}

	// delete alarm level data objects
	struct Node * alarmLevelsPtr = Sensor_getAlarmLevelsPointer(_self);
	if ( alarmLevelsPtr != NULL ) {
		Sensor_setAlarmLevelsPointer(_self, safeDelete(alarmLevelsPtr));
		if ( Sensor_getAlarmLevelsPointer(_self) != NULL )
			{ return NULL; } // fail
	}

	implement_Sensor_default_clearAllValues(_self);
	return _self;  // expected path
}

static void * implement_Sensor_default_copy(      struct Sensor * _copyTo,
						     const struct Sensor * _copyFromMaster)
{
	// copy master data members, except for PTRs and dynamic
	// PTRs and dynamic values are unique and should not be copied

	// Dynamic value
	//Sensor_setSensorState(_copyTo,
	//		Sensor_getSensorState(_copyFromMaster));

	//Sensor_setMiniState(_copyTo,
	//		Sensor_getMiniState(_copyFromMaster));

	//Sensor_setAsyncFlag(_copyTo,
	//		Sensor_getAsyncFlag(_copyFromMaster));

	Sensor_setPowerUpDelayTicks(_copyTo,
			Sensor_getEnablePowerDelayTicks(_copyFromMaster));

	Sensor_setAlignConfigDelayTicks(_copyTo,
			Sensor_getAlignConfigDelayTicks(_copyFromMaster));

	Sensor_setMeasurementDelayTicks(_copyTo,
			Sensor_getMeasurementDelayTicks(_copyFromMaster));

	// Pointer values
	//Sensor_setRawDataPointer(_copyTo,
	//		Sensor_getRawDataPointer(_copyFromMaster));

	//Sensor_setFinalDataPointer(_copyTo,
	//		Sensor_getFinalDataPointer(_copyFromMaster));

	//Sensor_setAlarmLevelsPointer(_copyTo,
	//		Sensor_getAlarmLevelsPointer(_copyFromMaster));

	Node_setValue(Sensor_getAlarmLevelsPointer(_copyTo),
			Node_getValue(Sensor_getAlarmLevelsPointer(_copyFromMaster)));

	Node_setValue(getLinkedNode(Sensor_getAlarmLevelsPointer(_copyTo)),
			Node_getValue(getLinkedNode(Sensor_getAlarmLevelsPointer(_copyFromMaster))));

	Sensor_setAlarmState(_copyTo,
			Sensor_getAlarmState(_copyFromMaster));

	Sensor_setNormalState(_copyTo,
			Sensor_getNormalState(_copyFromMaster));

	Sensor_setOnReportReady_cb(_copyTo,
			Sensor_getOnReportReady_cb(_copyFromMaster));

	Sensor_setOnAlarmTriggered_cb(_copyTo,
			Sensor_getOnAlarmTriggered_cb(_copyFromMaster));

	// Pointer value
	//Sensor_setIoStructPointer(_copyTo,
	//		Sensor_getIoStructPointer(_copyFromMaster));

	// copy IO structure values
	copy(Sensor_getIoStructPointer(_copyTo),
			Sensor_getIoStructPointer(_copyFromMaster));

	return _copyTo;
}

static equal_t implement_Sensor_default_equal(const struct Sensor * _self,
		                       const struct Sensor * _comparisonObject)
{
	// Check each data member for congruence
	struct Sensor * self            = (void *)_self;
	struct Sensor * comparisonObject = (void *)_comparisonObject;

	// sensorState minState and asyncFlag are dynamic and should not be
	// ... included in comparison

	//if( Sensor_getSensorState(self) !=
	//		               Sensor_getSensorState(comparisonObject) )
	//	{return OBJECT_UNEQUAL;}

	//if( Sensor_getMiniState(self) !=
	//		               Sensor_getMiniState(comparisonObject) )
	//	{return OBJECT_UNEQUAL;}

	//if( Sensor_getAsyncFlag(self) !=
	//		               Sensor_getAsyncFlag(comparisonObject) )
	//	{return OBJECT_UNEQUAL;}

	if( Sensor_getEnablePowerDelayTicks(self) !=
			               Sensor_getEnablePowerDelayTicks(comparisonObject) )
		{return OBJECT_UNEQUAL;}

	if( Sensor_getAlignConfigDelayTicks(self) !=
			               Sensor_getAlignConfigDelayTicks(comparisonObject) )
		{return OBJECT_UNEQUAL;}

	if( Sensor_getMeasurementDelayTicks(self) !=
			               Sensor_getMeasurementDelayTicks(comparisonObject) )
		{return OBJECT_UNEQUAL;}

	// data pointers are unique and should not be included in the comparison
	// data values are unique for separate sensors and should not be compared
	/*

	if( Sensor_getRawDataPointer(self) !=
			               Sensor_getRawDataPointer(comparisonObject) )
		{return OBJECT_UNEQUAL;}

	if( Sensor_getFinalDataPointer(self) !=
			               Sensor_getFinalDataPointer(comparisonObject) )
		{return OBJECT_UNEQUAL;}

	if( Sensor_getAlarmLevelsPointer(self) !=
			               Sensor_getAlarmLevelsPointer(comparisonObject) )
		{return OBJECT_UNEQUAL;}
	 */

	if( Node_getValue(Sensor_getAlarmLevelsPointer(self)) !=
			Node_getValue(Sensor_getAlarmLevelsPointer(comparisonObject)) )
			{return OBJECT_UNEQUAL;}

	if( Node_getValue(Node_getLinkedNode(Sensor_getAlarmLevelsPointer(self))) !=
			Node_getValue(Node_getLinkedNode(Sensor_getAlarmLevelsPointer(comparisonObject))) )
			{return OBJECT_UNEQUAL;}

	if( Sensor_getAlarmState(self) !=
			               Sensor_getAlarmState(comparisonObject) )
		{return OBJECT_UNEQUAL;}

	if( Sensor_getNormalState(self) !=
			               Sensor_getNormalState(comparisonObject) )
		{return OBJECT_UNEQUAL;}

	if( Sensor_getOnReportReady_cb(self) !=
			               Sensor_getOnReportReady_cb(comparisonObject) )
		{return OBJECT_UNEQUAL;}

	if( Sensor_getOnAlarmTriggered_cb(self) !=
			               Sensor_getOnAlarmTriggered_cb(comparisonObject) )
		{return OBJECT_UNEQUAL;}

	/* pointer value is not directly compared
	if( Sensor_getIoStructPointer(self) !=
			               Sensor_getIoStructPointer(comparisonObject) )
		{return OBJECT_UNEQUAL;}
	*/

	if( equal(Sensor_getIoStructPointer(self),
		Sensor_getIoStructPointer(comparisonObject)) == OBJECT_UNEQUAL )
		{return OBJECT_UNEQUAL;}

	// all data members are congruent
	return OBJECT_EQUAL;
}

static void * implement_Sensor_default_config(      struct Sensor * _self,
		                              const struct Sensor * _master)
{
	return copy(_self, _master);
}

puto_return_t implement_Sensor_default_puto(const struct Sensor * _self, FILE * _fp)
{
	puto_return_t printReturnCode = PUTO_SUCCESS;

	struct Sensor * self = (void *)cast(Sensor, _self);

	// print local data members
	fprintf(_fp, "\n  Local data members managed in Sensor:\n");

	if (PUTO_ERROR == fprintf(_fp, "  Sensor sensorState:           %i\n",
			Sensor_getSensorState(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Sensor minState:              %i\n",
			Sensor_getMiniState(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Sensor asyncFlag:             %i\n",
			Sensor_getAsyncFlag(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Sensor powerUpDelayTicks:     %i\n",
			Sensor_getEnablePowerDelayTicks(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Sensor alignConfigDelayTicks: %i\n",
			Sensor_getAlignConfigDelayTicks(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Sensor measurementDelayTicks: %i\n",
			Sensor_getMeasurementDelayTicks(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Sensor rawDataPointer:        %p\n",
			Sensor_getRawDataPointer(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Sensor finalDataPointer:      %p\n",
			Sensor_getFinalDataPointer(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Sensor alarmLevelsPointer:    %p\n",
			Sensor_getAlarmLevelsPointer(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Sensor LowerPrimaryAlarmLevel:   %i\n",
			Sensor_getLowerPrimaryAlarmLevel(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Sensor UpperSecondaryAlarmLevel: %i\n",
			Sensor_getUpperSecondaryAlarmLevel(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Sensor alarmState:            %i\n",
			Sensor_getAlarmState(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Sensor normalState:           %i\n",
			Sensor_getNormalState(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Sensor Sensor_onReportReady_cb:    %p\n",
			Sensor_getOnReportReady_cb(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Sensor Sensor_onAlarmTriggered_cb: %p\n",
			Sensor_getOnAlarmTriggered_cb(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Sensor IoStructPointer:            %p\n",
			Sensor_getIoStructPointer(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	fprintf(_fp, "\n  New methods added in Sensor:\n");

	//fprintf(_fp, "    ... no methods were added in XXXXX:\n");

	if (PUTO_ERROR == fprintf(_fp, "  writeDataToSensor:    %p\n",
			implement_Sensor_default_writeDataToSensor ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  readDataFromSensor:   %p\n",
			implement_Sensor_default_readDataFromSensor ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  loadDefaults:         %p\n",
			implement_Sensor_default_selectedDefaults ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  enablePower:          %p\n",
			implement_Sensor_default_enablePower ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  alignAndConfig:       %p\n",
			implement_Sensor_default_alignAndConfig ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  startMeasurement:     %p\n",
			implement_Sensor_default_startMeasurement ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  storeRawSensorData:   %p\n",
			implement_Sensor_default_storeRawData ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  disableSensorPower:   %p\n",
			implement_Sensor_default_disablePower ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  processRawSensorData: %p\n",
			implement_Sensor_default_processRawData ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  checkAlarms:          %p\n",
			implement_Sensor_default_processRawData ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	fprintf(_fp, "\n  Local Sensor methods:\n" );

	if (PUTO_ERROR == fprintf(_fp, "  Sensor_ctor:      %p\n", Sensor_default_ctor ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Sensor_dtor:      %p\n", Sensor_default_dtor ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Sensor_copy:      %p\n", Sensor_default_copy ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Sensor_equal:     %p\n", Sensor_default_equal ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Sensor_config:    %p\n", Sensor_config ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Sensor_puto:      %p\n", Sensor_default_puto ))
			{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  SensorClass_ctor: %p\n\n",
															SensorClass_ctor))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	fprintf(_fp, "Printing Sensor class:\n" );
	if (printReturnCode == Class_puto(Sensor, _fp))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	// print superclass data
	fprintf(_fp, "Printing the %s superclass data for Sensor:\n",
			((struct Object * )self)->class->superclass->className );
	// WARNING: classOf(self) calls into super will trigger looping
	if (printReturnCode == super_puto(Sensor, self, _fp))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	fprintf(_fp, "\nPrinting SensorClass class:\n" );
	if (printReturnCode == puto(SensorClass, _fp))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	return printReturnCode;
}

/**************************************/
/****  set and get sensorState    *****/

sensorState_t Sensor_getSensorState(const void * _self)
{
	const struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return SENSOR_STATE_UNKNOWN; }
	return self->sensorState;
}

sensorState_t Sensor_setSensorState(void * _self, sensorState_t _sensorState)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return SENSOR_STATE_UNKNOWN; }
	self->sensorState = _sensorState;
	return _sensorState;
}

/**************************************/
/*****  set and get miniState    ******/

miniState_t Sensor_getMiniState(const void  * _self)
{
	const struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return SENSOR_MINI_STATE_UNKNOWN; }
	return self->miniState;
}

miniState_t Sensor_setMiniState(void * _self, miniState_t _miniState)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return SENSOR_MINI_STATE_UNKNOWN; }
	self->miniState = _miniState;
	return _miniState;
}

/**************************************/
/*****  set and get AsyncFlag    ******/

sensorAsyncFlag_t Sensor_getAsyncFlag(const void  * _self)
{
	const struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return SENSOR_ASYNC_FLAG_UNKNOWN; }
	return self->asyncFlag;
}

sensorAsyncFlag_t Sensor_setAsyncFlag(void * _self,
		                              sensorAsyncFlag_t _asyncFlag)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return SENSOR_ASYNC_FLAG_UNKNOWN; }
	self->asyncFlag = _asyncFlag;
	return _asyncFlag;
}

/********************************************/
/****  set and get powerUpDelayTicks    *****/

int Sensor_getEnablePowerDelayTicks(const void * _self)
{
	const struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return SENSOR_DELAY_TICKS_UNKNOWN; }
	return self->powerUpDelayTicks;
}

int Sensor_setPowerUpDelayTicks(void * _self, int _delayTicks)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return SENSOR_DELAY_TICKS_UNKNOWN; }
	self->powerUpDelayTicks = _delayTicks;
	return _delayTicks;
}

/********************************************/
/****  set and get AlignConfig    *****/

int Sensor_getAlignConfigDelayTicks(const void * _self)
{
	const struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return SENSOR_DELAY_TICKS_UNKNOWN; }
	return self->configDelayTicks;
}

int Sensor_setAlignConfigDelayTicks(void * _self, int _delayTicks)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return SENSOR_DELAY_TICKS_UNKNOWN; }
	self->configDelayTicks = _delayTicks;
	return _delayTicks;
}

/********************************************/
/****  set and get measurmentDelayTicks  ****/

int Sensor_getMeasurementDelayTicks(const void * _self)
{
	const struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return SENSOR_DELAY_TICKS_UNKNOWN; }
	return self->measurementDelayTicks;
}

int Sensor_setMeasurementDelayTicks(void * _self, int _delayTicks)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return SENSOR_DELAY_TICKS_UNKNOWN; }
	self->measurementDelayTicks = _delayTicks;
	return _delayTicks;
}

/********************************************/
/*****  set and get RawDataPointer    *******/

void * Sensor_getRawDataPointer(const void * _self)
{
	const struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return NULL; }
	return self->rawDataPointer;
}

void * Sensor_setRawDataPointer(void * _self, void * _rawDataPointer)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return NULL; }
	self->rawDataPointer = _rawDataPointer;
	return _rawDataPointer;
}

/********************************************/
/****  set and get FinalDataPointer    *****/

void * Sensor_getFinalDataPointer(const void * _self)
{
	const struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return NULL; }
	return self->finalDataPointer;
}

void * Sensor_setFinalDataPointer(void * _self, void * _finalDataPointer)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return NULL; }
	self->finalDataPointer = _finalDataPointer;
	return _finalDataPointer;
}

/********************************************/
/****  set and get AlarmLevelsPointer    *****/

void * Sensor_getAlarmLevelsPointer(const void * _self)
{
	const struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return NULL; }
	return self->alarmLevelsPointer;
}

void * Sensor_setAlarmLevelsPointer(void * _self, void * _alarmLevelsPointer)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return NULL; }
	self->alarmLevelsPointer = _alarmLevelsPointer;
	return _alarmLevelsPointer;
}

/*************************************************/
/****  set and get alarm threshold values    *****/

value_t Sensor_getLowerPrimaryAlarmLevel(const void * _self)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return VALUE_UNKNOWN; }  // fail

	struct Node * localPrimaryAlarmLevelPtr = Sensor_getAlarmLevelsPointer(self);
	if (localPrimaryAlarmLevelPtr == NULL ) { return VALUE_UNKNOWN; } // fail

	return getValue(localPrimaryAlarmLevelPtr); // expected path
}

void * Sensor_setLowerPrimaryAlarmLevel(void * _self, value_t _value)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return VALUE_UNKNOWN; }  // fail

	struct Node * localPrimaryAlarmLevelPtr = Sensor_getAlarmLevelsPointer(self);

	// verify that the alarm node is present, create and link one if not
	if (localPrimaryAlarmLevelPtr == NULL ) {
		struct Node * tempNodePtr= new(Node);
		if ( tempNodePtr == NULL ) { return NULL; } // failed to create node
		Sensor_setAlarmLevelsPointer(_self, tempNodePtr);
	}

	setValue(localPrimaryAlarmLevelPtr, _value);
	setState(localPrimaryAlarmLevelPtr, STATE_VALUE);
	return _self;
}

value_t Sensor_getUpperSecondaryAlarmLevel(const void * _self)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL )                       { return VALUE_UNKNOWN; }  // fail

	struct Node * localPrimaryAlarmLevelPtr = Sensor_getAlarmLevelsPointer(self);
	if (localPrimaryAlarmLevelPtr == NULL )   { return VALUE_UNKNOWN; }  // fail

	struct Node * localSecondaryAlarmLevelPtr;
	localSecondaryAlarmLevelPtr = getLinkedNode(localPrimaryAlarmLevelPtr);
	if (localSecondaryAlarmLevelPtr == NULL ) { return VALUE_UNKNOWN; }  // fail

	return getValue(localSecondaryAlarmLevelPtr); // expected path
}

void * Sensor_setUpperSecondaryAlarmLevel(void * _self, value_t _value)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return VALUE_UNKNOWN; }  // fail

	struct Node * tempNodePtr;
	struct Node * localPrimaryAlarmLevelPtr = Sensor_getAlarmLevelsPointer(self);

	// verify that the alarm node is present, create and link one if not
	if (localPrimaryAlarmLevelPtr == NULL ) {
		tempNodePtr= new(Node);
		if ( tempNodePtr == NULL ) { return NULL; } // failed to create node
		Sensor_setAlarmLevelsPointer(_self, tempNodePtr);
	}

	struct Node * localSecondaryAlarmLevelPtr;
	localSecondaryAlarmLevelPtr = getLinkedNode(localPrimaryAlarmLevelPtr);

	// verify that the alarm node is present, create and link one if not
	if (localSecondaryAlarmLevelPtr == NULL ) {
		tempNodePtr= new(Node);
		if ( tempNodePtr == NULL ) { return NULL; } // failed to create node
		Sensor_setAlarmLevelsPointer(_self, tempNodePtr);
	}

	setValue(localSecondaryAlarmLevelPtr, _value);
	setState(localSecondaryAlarmLevelPtr, STATE_VALUE);
	return _self;
}


/********************************************/
/****  set and get AlarmState    *****/

alarmType_t Sensor_getAlarmState(const void * _self)
{
	const struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return ALARM_TYPE_UNKNOWN; }
	return self->alarmState;
}

alarmType_t Sensor_setAlarmState(void * _self, alarmType_t _alarmState)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return ALARM_TYPE_UNKNOWN; }
	self->alarmState = _alarmState;
	return _alarmState;
}

/*********************************************/
/*****  set and get NormalState    *****/

alarmType_t Sensor_getNormalState(const void * _self)
{
	const struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return ALARM_TYPE_UNKNOWN; }
	return self->normalState;
}

alarmType_t Sensor_setNormalState(void * _self, alarmType_t _normalState)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return ALARM_TYPE_UNKNOWN; }
	self->normalState = _normalState;
	return _normalState;
}

/***********************************************/
/****  set and get onReportReady callback  *****/

sensor_cb_fnct Sensor_setOnReportReady_cb(const void * _self, sensor_cb_fnct _cb)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return NULL; }
	self->Sensor_onReportReady_cb = _cb;
	return _cb;
}

sensor_cb_fnct Sensor_getOnReportReady_cb(const void * _self)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return NULL; }
	return self->Sensor_onReportReady_cb;
}

struct Sensor *  Sensor_emptyReportReadyCallback(struct Sensor * _self)
{
	return _self;
}

/***********************************************/
/****  set and get onAlarmTriggered callback  *****/

sensor_cb_fnct Sensor_setOnAlarmTriggered_cb(const void * _self, sensor_cb_fnct _cb)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return NULL; }
	self->Sensor_onAlarmTriggered_cb = _cb;
	return _cb;
}

sensor_cb_fnct Sensor_getOnAlarmTriggered_cb(const void * _self)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return NULL; }
	return self->Sensor_onAlarmTriggered_cb;
}

struct Sensor *  Sensor_emptyAlarmTriggeredCallback(struct Sensor * _self)
{
	return _self;
}


/********************************************/
/*****  set and get ioStructPointer    *******/
/**/
void * Sensor_getIoStructPointer(const void * _self)
{
	const struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return NULL; }
	return self->ioStructPtr;
}

void * Sensor_setIoStructPointer(void * _self, void * _ioStructPtr)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return NULL; }
	self->ioStructPtr = _ioStructPtr;
	return _ioStructPtr;
}


/*************************************/
/****    sensor state machine    *****/

void Sensor_update(void * _self)
{
	// trap for any asynchronous flags, like start or begin measurement
	Sensor_checkAsyncFlag(_self);


	switch (Sensor_getSensorState(_self)) {

	case SENSOR_STATE_UNKNOWN: {
		// do nothing ... wait for sensor to be enabled
		break;
	}

	case SENSOR_START_DATA_DEFAULTS: {    // <<<--- Sensor_start()
		// load sensor access structure defaults
		Sensor_loadDefaults(_self);
		Sensor_transitionState(_self, SENSOR_UNPOWERED_IDLE);
		break;
	}

	case SENSOR_UNPOWERED_IDLE: {
		// do nothing
		break;
	}

	case SENSOR_ENABLE_POWER: {    // <<<--- Sensor_measureAndProcess()
		//  WARNING:  must include state machine transition in update method
		//  serial communication probably required
		Sensor_enablePower(_self);
		break;
	}

	case SENSOR_WAITING_POWER: {
		// do nothing
		// transition is via Sensor_postEnablePower()
		break;
	}

	case SENSOR_ALIGN_CONFIG: {    // <<<--- Sensor_postEnablePower()
		//  WARNING:  must include state machine transition in update method
		//  serial communication probably required
		Sensor_alignAndConfig(_self);
		break;
	}

	case SENSOR_WAITING_CONFIG: {
		// do nothing
		// transition is via Sensor_postAlignAndConfig()
		break;
	}

	case SENSOR_READY_IDLE: {
		// do nothing
		break;
	}

	case SENSOR_START_MEASUREMENT: {    // <<<--- Sensor_measureAndProcess(), <<<--- Sensor_postAlignAndConfig()
		//  WARNING:  must include state machine transition in update method
		//  serial communication probably required
		Sensor_startMeasurement(_self);
		break;
	}

	case SENSOR_WAITING_MEASUREMENT: {
		// do nothing
		// transition is via Sensor_postStartMeasurement()
		break;
	}

	case SENSOR_GET_RAW_DATA: {    // <<<--- Sensor_postStartMeasurement()
		//  WARNING:  must include state machine transition in update method
		//  serial communication probably required
		Sensor_storeRawData(_self);
		break;
	}

	case SENSOR_DISABLE_POWER: {
		//  WARNING:  must include state machine transition in update method
		//  serial communication probably required
		Sensor_disablePower(_self);
		break;
	}

	case SENSOR_PROCESS_RAW_DATA: {
		//  WARNING:  must include state machine transition in update method
		//  serial communication probably required
		Sensor_processRawData(_self);
		Sensor_transitionState(_self, SENSOR_CHECK_ALARMS);
		break;
	}

	case SENSOR_CHECK_ALARMS: {
		Sensor_checkAlarms(_self);

		// fire alarm triggered callback if needed
		if ( Sensor_getAlarmState(_self) == ALARM_TRIGGERED )
			{ implement_Sensor_callAlarmTriggered_CB(_self); }

		// regardless of checkAlarms() outcome, transition to next state
		Sensor_transitionState(_self, SENSOR_REPORT);

		// fire the report ready callback only once
		implement_Sensor_callReportReady_CB(_self);
		break;
	}

	case SENSOR_REPORT: {
		// increment asynchronous flag to allow subsequent measurement request
		Sensor_setAsyncFlag(_self, SENSOR_ASYNC_MEASURE_DONE);

		// hold in this state until a new measurement is requested
		break;
	}

	default: { break; }

	} // end of switch

	return;
}


/*************************************************/
/*******  asynchronous management methods   ******/

void * Sensor_start(void * _self)
{
	// set asynchronous start flag for the state machine
	// initializes sensor access structure data values

	// does not power the sensor
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return NULL; }  // fail

	// return without taking action if the sensor has already been started
	if ( Sensor_getSensorState(self) >=  SENSOR_START_DATA_DEFAULTS ) {
		return self;
	}

	// set the asynchronous flag indicating that a sensor start is needed
	Sensor_setAsyncFlag(self, SENSOR_ASYNC_START_REQUEST);
	return self;
}

void * Sensor_measureAndProcess(void * _self)
{
	// set asynchronous start measurement flag for the state machine
	// aligns, enables power, collects and processes data

	// validate sensor access structure
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return NULL; }  // fail

	// trigger measurement if one is not already in process
	if ( Sensor_getAsyncFlag(_self) != SENSOR_ASYNC_MEASURE_IN_PROCESS ) {
			Sensor_setAsyncFlag(self, SENSOR_ASYNC_MEASURE_REQUEST);
	}

	return self;
}

void * Sensor_stopAndRemovePower(void * _self)
{
	// asynchronous trigger for the state machine
	// assumes that the measurement is complete
	// will halt sensor operation immediately regardless of status

	// validate sensor access structure
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return NULL; }  // fail

	// disable power if sensor has been previously powered
	if ( Sensor_getSensorState(self) >=  SENSOR_WAITING_POWER ) {
		Sensor_disablePower(_self);
	}

	// set main state as uppowered  idle if previously running
	if ( Sensor_getSensorState(self) >  SENSOR_UNPOWERED_IDLE ) {
		Sensor_transitionState(_self, SENSOR_UNPOWERED_IDLE);
	}

	// TODO: add protection against previously registered async communication
	//       ... there may be communication sequences already registered in
	//       ... the IO driver.  These comm sequences will execute when
	//       ... possible and will likely assume that the sensor is powered.
	//       ... Need to unwind these calls without hanging the system.

	return self;
}

static void Sensor_checkAsyncFlag(struct Sensor * _self)
{
	switch (Sensor_getAsyncFlag(_self)) {

	case SENSOR_ASYNC_FLAG_UNKNOWN: {
		// do nothing
		break; }

	case SENSOR_ASYNC_START_REQUEST: {
		// start sensor state machine if not already started
		if ( Sensor_getSensorState(_self) < SENSOR_START_DATA_DEFAULTS ) {
			Sensor_transitionState(_self, SENSOR_START_DATA_DEFAULTS);
		}

		// increment asynchronous flag to block further start requests
		Sensor_setAsyncFlag(_self, SENSOR_ASYNC_START_DONE);
		break;
	}

	case SENSOR_ASYNC_START_DONE: {
		// do nothing
		break;
	}

	case SENSOR_ASYNC_MEASURE_REQUEST: {
		// increment asynchronous flag to block further measurement requests
		Sensor_setAsyncFlag(_self, SENSOR_ASYNC_MEASURE_IN_PROCESS);

		// action based on status of main sensor state machine
		sensorState_t currentSensorState = Sensor_getSensorState(_self);

		// start sensor if needed
		if ( currentSensorState < SENSOR_UNPOWERED_IDLE ) {
			Sensor_transitionState(_self, SENSOR_START_DATA_DEFAULTS);
			break;
		}

		// sensor has been started previously ...
		// measure now if ready, otherwise apply power before proceeding
		if ( currentSensorState >= SENSOR_READY_IDLE ) {
			Sensor_transitionState(_self, SENSOR_START_MEASUREMENT);
		}
		else {
			Sensor_transitionState(_self, SENSOR_ENABLE_POWER);
		}

		break;
	}

	case SENSOR_ASYNC_MEASURE_IN_PROCESS: {
		// do nothing
		break;
	}
	case SENSOR_ASYNC_MEASURE_DONE: {
		// do nothing
		break;
	}

	default: { break; }

	}  // end switch
	return;
}

/*******************************************/
/*****    external module interfaces   *****/

sensor_cb_fnct Sensor_armDelayedCallback(void *         _self,
		                                 sensor_cb_fnct _callback,
										 int            _ticksOfDelay)
{
	if ( _ticksOfDelay <= SENSOR_DELAY_TICKS_ZERO )
		{ return NULL; }  // no delay needed

	// arm callback task on scheduler
	// NOTE:  the callback should be of "void functionName(void * _pointer)"
	scheduler_AddTask_wPTR((void (*)(void *))_callback,
			               (void *)_self,
						   _ticksOfDelay,
						   0);

	return _callback;
}

static void * implement_Sensor_default_writeDataToSensor(struct Sensor * _self)
{
	// get struct IO pointer in the correct IO type ... SENSOR_xxxx_IO_TYPE
	struct SENSOR_DEFAULT_IO_TYPE * localIoStructPtr =
	                                    Sensor_getIoStructPointer(_self);
	if ( localIoStructPtr == NULL ) { return NULL; }  // fail no IO struct

	// get command buffer pointer
	command_t * commandBufferPTR = Sensor_getIoCommandBufPointer(_self);
	if ( commandBufferPTR == NULL ) { return NULL; }  // fail no cmd buffer

	// do not allow default sensor to attempt reads/writes to a NULL address
	// comm address is usually set externally once just after new(Sensor)
	void * address = IO_getAddress(localIoStructPtr);
	if ( address == NULL ) { return NULL; }  // fail

	// set for sequential writes to successive locations starting with "address"
	// default sensor is a simple memory access module and assumes sequential
	IO_setIOAction(localIoStructPtr, IO_WRITE_SEQUENTIAL);

	// add the command sequence to the IO list for processing when possible
	if ( IO_addIOSequenceToList(localIoStructPtr) == localIoStructPtr) {
		return _self;  // expected return path
	}
	else {
		return NULL;  // fail in IO
	}
	return NULL; // fail
}

static void * implement_Sensor_default_readDataFromSensor (struct Sensor * _self)
{
	// get struct IO pointer in the correct IO type ... SENSOR_xxxx_IO_TYPE
	struct SENSOR_DEFAULT_IO_TYPE * localIoStructPtr =
										Sensor_getIoStructPointer(_self);
	if ( localIoStructPtr == NULL ) { return NULL; }  // fail no IO struct

	// get command buffer pointer
	command_t * commandBufferPTR = Sensor_getIoCommandBufPointer(_self);
	if ( commandBufferPTR == NULL ) { return NULL; }  // fail no cmd buffer

	// do not allow default sensor to attempt reads/writes to a NULL address
	// comm address is usually set externally once just after new(Sensor)
	void * address = IO_getAddress(localIoStructPtr);
	if ( address == NULL ) { return NULL; }  // fail

	// set for sequential writes to successive locations starting with "address"
	// default sensor is a simple memory access module and assumes sequential
	IO_setIOAction(localIoStructPtr, IO_READ_SEQUENTIAL);

	// add the command sequence to the IO list for processing when possible
	if ( IO_addIOSequenceToList(localIoStructPtr) == localIoStructPtr) {
		return _self;  // expected return path
	}
	else {
		return NULL;  // fail in IO
	}
	return NULL; // fail
}


/********************************************/
/*******    state machine methods   *********/

static void * implement_Sensor_default_clearAllValues(struct Sensor * _self)
{
	// WARNING:  Should only be executed when the sensor access structure is
	//           initiated.  Otherwise, middleware-specific items like
	//           the callbacks function pointers would be inadvertently reset.
	Sensor_setSensorState          (_self, SENSOR_STATE_UNKNOWN);
	Sensor_setMiniState            (_self, SENSOR_MINI_STATE_UNKNOWN);
	Sensor_setAsyncFlag            (_self, SENSOR_ASYNC_FLAG_UNKNOWN);
	Sensor_setPowerUpDelayTicks    (_self, SENSOR_DELAY_TICKS_UNKNOWN);
	Sensor_setAlignConfigDelayTicks(_self, SENSOR_DELAY_TICKS_UNKNOWN);
	Sensor_setMeasurementDelayTicks(_self, SENSOR_DELAY_TICKS_UNKNOWN);

	// WARNING: DO NOT overwrite pointers outside of ctor and dtor.
	//          The individual buffers or Nodes are created in the ctor.

	Sensor_setRawDataPointer       (_self, NULL);
	Sensor_setFinalDataPointer     (_self, NULL);
	Sensor_setAlarmLevelsPointer   (_self, NULL);


	Sensor_setAlarmState           (_self, ALARM_TYPE_UNKNOWN);
	Sensor_setNormalState          (_self, ALARM_TYPE_UNKNOWN);
	Sensor_setOnReportReady_cb     (_self, Sensor_emptyReportReadyCallback);
	Sensor_setOnAlarmTriggered_cb  (_self, Sensor_emptyAlarmTriggeredCallback);
	Sensor_setIoStructPointer      (_self, NULL);
	return _self;
}

static void * implement_Sensor_default_selectedDefaults(struct Sensor * _self)
{
	Sensor_setPowerUpDelayTicks    (_self, SENSOR_DEFAULT_POWER_UP_DELAY_TICKS);
	Sensor_setAlignConfigDelayTicks(_self, SENSOR_DEFAULT_CONFIG_DELAY_TICKS);
	Sensor_setMeasurementDelayTicks(_self, SENSOR_DEFAULT_MEASUREMENT_DELAY_TICKS);

	// Do not modify object pointers.  These are managed in ctor and dtor.
	//Sensor_setRawDataPointer       (_self, NULL);
	//Sensor_setFinalDataPointer     (_self, NULL);
	//Sensor_setAlarmLevelsPointer   (_self, NULL);

	Sensor_setAlarmState(_self, ALARM_TYPE_UNKNOWN);

	// Do not modify normal alarm state.  This are managed by middleware.
	//Sensor_setNormalState(_self, ALARM_TYPE_UNKNOWN);

	// Do not modify object pointers.  These are managed by middleware.
	//Sensor_setOnReportReady_cb   (_self, Sensor_emptyReportReadyCallback);
	//Sensor_setOnAlarmTriggered_cb(_self, Sensor_emptyAlarmTriggeredCallback);
	//Sensor_setIoStructPointer      (_self, NULL);
	return _self;
}

static void * implement_Sensor_default_enablePower(struct Sensor * _self)
{
	//  Example power enable process:
	//  miniState 0: load command buffer and add write sequence to IO manager
	//  miniState 1: wait for IO manager callback fire to increment miniState
	//  miniState 2: arm scheduler task with power delay + transition main SM

	miniState_t localMiniState = Sensor_getMiniState(_self);
	switch (localMiniState) {

	case SENSOR_MINI_STATE_START_0: {
		// Example:  create write/read sequence to enable power an I2C sensor
		//           ... this command example only will take no action if
		//           ... address is set to NULL

		// get pointer to the struct IO
		struct SENSOR_DEFAULT_IO_TYPE * localIoStructPtr =
		                                    Sensor_getIoStructPointer(_self);
		if ( localIoStructPtr == NULL ) { return NULL; }  // fail

		//  get pointer to the command buffer
		command_t * commandBufferPTR = Sensor_getIoCommandBufPointer(_self);
		if ( commandBufferPTR == NULL ) { return NULL; }  // fail

		// comm address is usually set externally once, just after new(Sensor)
		// IO_setAddress(localIoStructPtr, 0x40);  // I2C example address
		// a NULL address will fail to write in IO

		// clear the command buffer
		IO_clearCommandSequences(localIoStructPtr);

		// add register and data values to command buffer
		IO_addWriteCommandToSequence(localIoStructPtr, 0x03);
		IO_addWriteCommandToSequence(localIoStructPtr, 0x41);

		// set communication complete callback to Sensor_incrementMiniState()
		IO_set_actionDone_cb(localIoStructPtr,
										(io_cb_fnct)Sensor_incrementMiniState);
		IO_setObjectPointer(localIoStructPtr, _self);

		// transmit the IO object to IO manager via Sensor_writeDataToSensor()
		// NOTE: might place write operation inside a NULL test to return error
		Sensor_writeDataToSensor(_self);

		// transition to a waiting miniState
		Sensor_incrementMiniState(_self);
		break;
	}

	case SENSOR_MINI_STATE_1: {
		// a wait state that takes no action
		// transition to next miniState is usually through io_cb_fnct callback
		// Sensor_incrementMiniState(_self);
		break;
	}

	case SENSOR_MINI_STATE_2: {
		// last mini-state
		// all device communication is complete
		// arm scheduler task to fire, OR automatically transition on 0 delay
		int delayTicks = Sensor_getEnablePowerDelayTicks(_self);
		if ( delayTicks > 0 ) {
			Sensor_armDelayedCallback(_self,
								      (sensor_cb_fnct)Sensor_postEnablePower,
								      delayTicks);
			Sensor_transitionState(_self, SENSOR_WAITING_POWER);
		} else {
			// no callback is needed
			Sensor_transitionState(_self, SENSOR_ALIGN_CONFIG);
		}
		break;
	}

	default: { break; }

	} // end switch

	return _self;
}

void Sensor_postEnablePower(void * _self)
{
	// asynchronous callback from the scheduler
	// ready to transition to next state
	Sensor_transitionState(_self, SENSOR_ALIGN_CONFIG);
	return;
}


static void * implement_Sensor_default_alignAndConfig(struct Sensor * _self)
{
	// add any sensor specific code here

	// set callback to fire, OTHERWISE automatically transition
	int delayTicks = Sensor_getAlignConfigDelayTicks(_self);
	if ( delayTicks > 0 ) {
		Sensor_armDelayedCallback(_self,
							      (sensor_cb_fnct)Sensor_postAlignAndConfig,
							      delayTicks);
		Sensor_transitionState(_self, SENSOR_WAITING_CONFIG);
	} else {
		// no callback is needed
		Sensor_transitionState(_self, SENSOR_START_MEASUREMENT);
	}
	return _self;
}

void Sensor_postAlignAndConfig(void * _self)
{
	// asynchronous callback from the scheduler
	// ready to transition to next state
	Sensor_transitionState(_self, SENSOR_START_MEASUREMENT);
	return;
}

static void * implement_Sensor_default_startMeasurement(struct Sensor * _self)
{
	// add any sensor specific code here

	// set callback to fire, OTHERWISE automatically transition
	int delayTicks = Sensor_getMeasurementDelayTicks(_self);
	if ( delayTicks > 0 ) {
		Sensor_armDelayedCallback(_self,
							      (sensor_cb_fnct)Sensor_postStartMeasurement,
							      delayTicks);
		Sensor_transitionState(_self, SENSOR_WAITING_MEASUREMENT);
	} else {
		// no callback is needed
		Sensor_transitionState(_self, SENSOR_GET_RAW_DATA);
	}
	return _self;
}

void Sensor_postStartMeasurement(void * _self)
{
	// asynchronous callback from the scheduler
	// ready to transition to next state
	Sensor_transitionState(_self, SENSOR_GET_RAW_DATA);
	return;
}


static void * implement_Sensor_default_storeRawData(struct Sensor * _self)
{
	// add any sensor specific code here

	// store raw data
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL)             { return NULL; }  // fail

	struct Node * localRawDataPtr = Sensor_getRawDataPointer  (_self);
	if ( localRawDataPtr == NULL ) { return NULL; }  // fail

	//  Example power enable process:
	//  miniState 0: load command buffer and add write sequence to IO manager
	//  miniState 1: wait while IO manager reads data back into the buffer
	//  miniState 2: transfer data into the rawDataBuffer + transition state

	miniState_t localMiniState = Sensor_getMiniState(_self);
	switch (localMiniState) {

	case SENSOR_MINI_STATE_START_0: {
		// Example:  create write/read sequence to enable power an I2C sensor
		//           ... this command example only will take no action if
		//           ... address is set to NULL

		// get pointer to the struct IO
		struct SENSOR_DEFAULT_IO_TYPE * localIoStructPtr =
											Sensor_getIoStructPointer(_self);
		if ( localIoStructPtr == NULL ) { return NULL; }  // fail

		//  get pointer to the command buffer
		command_t * commandBufferPTR = Sensor_getIoCommandBufPointer(_self);
		if ( commandBufferPTR == NULL ) { return NULL; }  // fail

		// comm address is usually set externally once, just after new(Sensor)
		// IO_setAddress(localIoStructPtr, 0x40);  // I2C example address
		// a NULL address will fail to write in IO

		// clear the command buffer
		IO_clearCommandSequences(localIoStructPtr);

		// add register/data values to command buffer to trigger read operation
		// in example code no values are written to buffer
		// ... unit test can externally set a test value
		// ... write commands are not needed in example
		//IO_addWriteCommandToSequence(localIoStructPtr, 0x03);
		//IO_addWriteCommandToSequence(localIoStructPtr, 0x41);

		// load the number of bytes to read back into the readCount "1" here
		// the write counts are automatically handled
		IO_setReadCount(localIoStructPtr, 1);

		// set communication complete callback as Sensor_incrementMiniState()
		IO_set_actionDone_cb(localIoStructPtr,
										(io_cb_fnct)Sensor_incrementMiniState);
		IO_setObjectPointer(localIoStructPtr, _self);

		// transmit the IO object to IO manager via Sensor_writeDataToSensor()
		// NOTE: might test for NULL return errors
		Sensor_readDataFromSensor(_self);

		// transition to a waiting miniState
		Sensor_incrementMiniState(_self);
		break;
	}

	case SENSOR_MINI_STATE_1: {
		// a wait state that takes no action
		// transition to next miniState is usually through io_cb_fnct callback
		// Sensor_incrementMiniState(_self);
		break;
	}

	case SENSOR_MINI_STATE_2: {
		// last mini-state
		// all device communication is complete

		// transfer data to the rawDataBuffer
		// get pointer to the buffer in the struct IO object
		struct SENSOR_DEFAULT_IO_TYPE * localIoStructPtr =
											Sensor_getIoStructPointer(_self);
		if ( localIoStructPtr == NULL )  { return NULL; }  // fail
		io_data_t * localBufferPointer = IO_getBufferPointer(localIoStructPtr);
		if ( localBufferPointer == NULL ) { return NULL; }  // fail

		// get the raw buffer pointer in the sensor structure
		struct Node * localRawDataPtr = Sensor_getRawDataPointer(_self);
		if ( localRawDataPtr == NULL )   { return NULL; }

		// copy data from IO buffer to sensor raw data buffer
		// WARNING:  the sensor data is held in a Node structure while the
		//           ... IO buffer is an array of char ... be careful
		//           ... processing multiple sequential read values
		setValue(localRawDataPtr, localBufferPointer[0]);


		// transition to next state
		Sensor_transitionState(_self, SENSOR_DISABLE_POWER);
		break;
	}

	default: { break; }

	} // end switch


	Sensor_transitionState(_self, SENSOR_DISABLE_POWER);
	return _self;
}

void * implement_Sensor_default_disablePower(struct Sensor * _self)
{
	enum { AUTOMATICALY_DIABLE_POWER, MANUALLY_DISABLE_POWER };
	int mode = AUTOMATICALY_DIABLE_POWER;
	//int mode = MANUALLY_DISABLE_POWER;

	if ( mode == AUTOMATICALY_DIABLE_POWER ) {
		Sensor_transitionState(_self, SENSOR_PROCESS_RAW_DATA);
		return Sensor_default_sendDisablePowerCommands(_self);
	}

	if ( mode == MANUALLY_DISABLE_POWER ) {
		Sensor_transitionState(_self, SENSOR_PROCESS_RAW_DATA);
		return _self;
	}

	return NULL;  // unexpected failing path
}

void * Sensor_default_sendDisablePowerCommands(void * _self)
{
	// add any sensor specific code here

	return _self;
}

static void * implement_Sensor_default_processRawData(struct Sensor * _self)
{
	struct Node * localRawDataPtr = Sensor_getRawDataPointer(_self);
	if ( localRawDataPtr == NULL )   { return NULL; }
	struct Node * localFinalDataPtr = Sensor_getFinalDataPointer(_self);
	if ( localFinalDataPtr == NULL ) { return NULL; }

	// add correct processing code here
	// default case simply copies the raw data to the final data area
	setValue(localFinalDataPtr, getValue(localRawDataPtr) );
	return _self;
}

static void * implement_Sensor_default_checkAlarms(struct Sensor * _self)
{
	// set alarm state as unknown prior to passing the valid data checks
	Sensor_setAlarmState( _self, ALARM_TYPE_UNKNOWN );

	// test for an active/valid normal alarm type
	// see alarmType_t enumeration for details
	alarmType_t normalAlarmType = Sensor_getNormalState(_self);
	if ( normalAlarmType < ALARM_BELOW )      { return _self; } // fail

	// test for a valid lower/primary threshold value

	// get pointer to primary threshold Node
	struct Node * lowerPrimaryAlarmPtr = Sensor_getAlarmLevelsPointer(_self);
	if ( lowerPrimaryAlarmPtr == NULL )       { return _self; } // fail

	// test for valid primary threshold value
	state_t lowerThresholdState = getState(lowerPrimaryAlarmPtr);
	if ( lowerThresholdState != STATE_VALUE ) { return _self; } // fail
	value_t lowerValue = getValue(lowerPrimaryAlarmPtr);

	// test for a valid upper/secondary threshold value if needed for dual test
	// see alarmType_t enumeration for details
	value_t upperValue;
	upperValue = lowerValue; // default value
	if ( normalAlarmType >= ALARM_BETWEEN ) {
		// upper value is needed for dual threshold check

		// get pointer to upper secondary threshold Node
		struct Node * upperSecondaryAlarmPtr = lowerPrimaryAlarmPtr->linkedNode;
		if ( upperSecondaryAlarmPtr == NULL ) { return _self; } // fail

		// test for valid secondary threshold value
		state_t upperThresholdState = getState(upperSecondaryAlarmPtr);
		if ( upperThresholdState != STATE_VALUE ) { return _self; }// fail
		upperValue = getValue(upperSecondaryAlarmPtr);
	}

	// all data validity checks pass

	// set default alarm state to NONE pending threshold tests
	Sensor_setAlarmState( _self, ALARM_NONE );

	// get final data value for comparison
	struct Node * localFinalDataPtr = Sensor_getFinalDataPointer(_self);
	value_t finalValue = getValue(localFinalDataPtr);

	// test value against alarm thresholds
	switch (normalAlarmType) {

	case ALARM_BELOW: {
		if ( finalValue < lowerValue )
			{ Sensor_setAlarmState( _self, ALARM_TRIGGERED ); }
		break;
	}

	case ALARM_BELOW_OR_EQUAL: {
		if ( finalValue <= lowerValue )
			{ Sensor_setAlarmState( _self, ALARM_TRIGGERED ); }
		break;
	}

	case ALARM_EQUAL: {
		if ( finalValue == lowerValue )
			{ Sensor_setAlarmState( _self, ALARM_TRIGGERED ); }
		break;
	}

	case ALARM_ABOVE: {
		if ( finalValue > lowerValue )
			{ Sensor_setAlarmState( _self, ALARM_TRIGGERED ); }
		break;
	}

	case ALARM_ABOVE_OR_EQUAL: {
		if ( finalValue >= lowerValue )
			{ Sensor_setAlarmState( _self, ALARM_TRIGGERED ); }
		break;
	}

	case ALARM_BETWEEN: {
		if ( (finalValue >= lowerValue) && (finalValue <= upperValue) )
			{ Sensor_setAlarmState( _self, ALARM_TRIGGERED ); }
		break;
	}

	case ALARM_OUTSIDE: {
		if ( (finalValue < lowerValue) || (finalValue > upperValue) )
			{ Sensor_setAlarmState( _self, ALARM_TRIGGERED ); }
		break;
	}

	case ALARM_TYPE_UNKNOWN:
	case ALARM_TRIGGERED:
	case ALARM_NONE:
	default: { break; }
	}

	return _self;
}

static void * implement_Sensor_callAlarmTriggered_CB(struct Sensor * _self)
{
	_self->Sensor_onAlarmTriggered_cb(_self);
	return _self;
}

static void * implement_Sensor_callReportReady_CB(struct Sensor * _self)
{
	_self->Sensor_onReportReady_cb(_self);
	return _self;
}

sensorReportStatus_t Sensor_reportReady(const void * _self)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return SENSOR_REPORT_NOT_READY; }  // fail

	// test for valid/completed process
	if ( Sensor_getSensorState(self) != SENSOR_REPORT )
		{ return SENSOR_REPORT_NOT_READY; }
	else
		{ return SENSOR_REPORT_IS_READY; }
}


/*************************************************/

void Sensor_transitionState(struct Sensor * _self, sensorState_t _state)
{
	Sensor_setSensorState(_self, _state);
	// always reset the miniState when transitioning the main state
	Sensor_resetMiniState(_self);
	return;
}

void * Sensor_resetMiniState(struct Sensor * _self)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return NULL; }  // fail
	Sensor_setMiniState(self, SENSOR_MINI_STATE_START_0);
	return self;
}

void * Sensor_incrementMiniState(struct Sensor * _self)
{
	struct Sensor * self = cast(Sensor, _self);
	if ( self == NULL ) { return NULL; }  // fail
	miniState_t localMiniState = Sensor_getMiniState(self);
	localMiniState++;
	Sensor_setMiniState(self, localMiniState);
	return self;
}

void * Sensor_getIoCommandBufPointer( void * _self)
{
	struct Sensor * self = cast(Sensor, _self);
	struct SENSOR_DEFAULT_IO_TYPE * localIoStructPtr =
										Sensor_getIoStructPointer(self);
	if ( localIoStructPtr == NULL ) {
		Sensor_incrementMiniState(self);  // fail
		return NULL;
	}

	//  get command buffer pointer
	command_t * commandBufferPTR = IO_getBufferPointer(localIoStructPtr);
	if ( commandBufferPTR == NULL ) {
		Sensor_incrementMiniState(self);   // fail
		return NULL;
	}
	return commandBufferPTR;
}
