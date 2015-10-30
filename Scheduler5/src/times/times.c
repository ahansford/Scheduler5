/*
 * time.c
 *
 *  Created on: Aug 2, 2015
 *      Author: alhansfo
 */


#include "times.h"
#include "times-private.h"             // safety include
#include "..\..\src\lists\lists.h"     // supports class registration
#include "..\..\src\objects\objects.h" // safety include

/**************************************/
/***** Implementation Functions  ******/

static void *  implement_Time_dtor(       struct Time * _self);
void *         implement_Time_copy(       struct Time * _copyToTime,
                                    const struct Time * _copyFromTime);
equal_t        implement_Time_equal(const struct Time * _self,
		                            const struct Time * _comparisonObject);
static void * implement_Time_config(      struct Time * _self,
		                            const struct Time * _master);
puto_return_t   implement_Time_puto(const struct Time * _self, FILE * _fp);


static second_t implement_Time_getSecond(const struct Time * _self);
static second_t implement_Time_setSecond(struct Time * _self,
		                                 second_t      _second);

static minute_t implement_Time_getMinute(const struct Time * _self);
static minute_t implement_Time_setMinute(struct Time * _self,
		                                 minute_t      _minute);

static hour_t   implement_Time_getHour  (const struct Time * _self);
static hour_t   implement_Time_setHour  (struct Time * _self, hour_t   _hour);

static dayOfWeek_t implement_Time_getDayOfWeek(const struct Time * _self);
static dayOfWeek_t implement_Time_setDayOfWeek(struct Time * _self,
		                                       dayOfWeek_t   _dayOfWeek);

static hourMode_t implement_Time_getHourMode(const struct Time * _self);
static hourMode_t implement_Time_setHourMode(struct Time * _self,
		                                     hourMode_t    _hourMode);

static AmPm_t implement_Time_getAmPm(const struct Time * _self);
static AmPm_t implement_Time_setAmPm(struct Time * _self, AmPm_t _AmPm);

static increment_t implement_Time_getTimeUpdateIncrement(const struct Time * _self);
static increment_t implement_Time_setTimeUpdateIncrement(struct Time * _self,
		                                                 increment_t   _mode);

static timeMatch_t  implement_Time_checkTimeMatch(const struct Time * _self,
		                                          const struct Time * _comparisonTime);

static void *  implement_Time_incrementTime     (struct Time * _self);
static void *  implement_Time_incrementSecond   (struct Time * _self);
static void *  implement_Time_incrementMinute   (struct Time * _self);
static void *  implement_Time_incrementHour     (struct Time * _self);
static void *  implement_Time_incrementDayOfWeek(struct Time * _self);
static void *  implement_Time_convertTo12Hour   (struct Time * _self);
static void *  implement_Time_convertTo24Hour   (struct Time * _self);



/*****************************/
/**** INITIALIZATIONS  *******/

const void * TimeClass = NULL;
const void * Time      = NULL;

// MUST be called before any Node is created
void Time_init(void)
{
	// Insert the lines below for any derived subclasses
	//if (!Time)      {Time_init();}
	//if (!TimeClass) {Time_init();}

	if (! TimeClass) {
		TimeClass = new(Class,  		// should be "Class"
						"TimeClass",	// should be "SomethingClass"
						Class,  		// should be "Class"
						sizeof(struct TimeClass),
						ctor, TimeClass_ctor,	//SomethingClass_ctor
						0);  // Terminator
	}
	if (! Time) {
		Time = new(TimeClass,			// SomethingClass from above
				    "Time",				// name like "Something"
				    Object,  			// "superclass(Something)"
				    sizeof(struct Time),// size of self
					// Overloaded superclass functions
					// Remember to call superclass->method
				    ctor,	Time_ctor,//Something_ctor
					dtor,   Time_dtor,
					copy,	Time_copy,
					equal,	Time_equal,
					config, Time_config,
					puto,	Time_puto,
					// New functions added in this class
					// Do not call superclass->method
					setSecond,	Time_setSecond,
					getSecond,	Time_getSecond,

					setMinute,	Time_setMinute,
					getMinute,	Time_getMinute,

					setHour,	Time_setHour,
					getHour,	Time_getHour,

					setDayOfWeek, Time_setDayOfWeek,
					getDayOfWeek, Time_getDayOfWeek,

					setHourMode, Time_setHourMode,
					getHourMode, Time_getHourMode,

					setAmPm, Time_setAmPm,
					getAmPm, Time_getAmPm,

					setTimeUpdateIncrement, Time_setTimeUpdateIncrement,
					getTimeUpdateIncrement, Time_getTimeUpdateIncrement,

					incrementTime, Time_incrementTime,
					checkTimeMatch, Time_checkTimeMatch,
				    0);	// Terminator
	}

	// requires #include "..\..\src\lists\lists.h" to support class registry
	//implement_Time_registerKeyClasses();

	return;
}

/*****************************/
/****** Constructors  ********/

void * Time_ctor(void * _self, va_list * app)
{
	// Add superclass class data members
	struct Time * self = super_ctor(Time, _self, app);

	// set default values for any new data members
	self->second              = SECOND_UNKNOWN;
	self->minute              = MINUTE_UNKNOWN;
	self->hour                = HOUR_UNKNOWN;
	self->dayOfWeek           = DAY_OF_WEEK_UNKNOWN;
	self->hourMode            = HOUR_MODE_UNKNOWN;
	self->AmPm                = AM__PM_UNKNOWN;
	self->timeUpdateIncrement = INCREMENT_UNKNOWN;
	self->time_OnSecondChange_cb = Time_PostSecondCallback;  // default no-op
	self->time_OnMinuteChange_cb = Time_PostMinuteCallback;  // default no-op
	self->time_OnHourChange_cb   = Time_PostHourCallback;    // default no-op

	// overwrite data members with new data
	// Only uncomment if all data members will be specified in new() command
	// ... this seems like an undue burden on the user.  Leave commented out
	// ... numerous unit tests will need to be adapted if uncommented
	//self->second              = va_arg(* app, second_t);
	//self->minute              = va_arg(* app, minute_t);
	//self->hour                = va_arg(* app, hour_t);
	//self->dayOfWeek           = va_arg(* app, dayOfWeek_t);
	//self->hourMode            = va_arg(* app, hourMode_t);
	//self->AmPm                = va_arg(* app, AmPm_t);
	//self->timeUpdateIncrement = va_arg(* app, increment_t);

	return self;
}

void * TimeClass_ctor(void * _self, va_list *app)
{
	// Add superclass class data and methods
	struct TimeClass * self = super_ctor(TimeClass, _self, app);

	// Initialize new functions to default values
	typedef void (* voidf)();

	* (voidf *) & self->setSecond    = NULL;
	* (voidf *) & self->getSecond    = NULL;

	* (voidf *) & self->setMinute    = NULL;
	* (voidf *) & self->getMinute    = NULL;

	* (voidf *) & self->setHour      = NULL;
	* (voidf *) & self->getHour      = NULL;

	* (voidf *) & self->setHourMode  = NULL;
	* (voidf *) & self->getHourMode  = NULL;

	* (voidf *) & self->setAmPm      = NULL;
	* (voidf *) & self->getAmPm      = NULL;

	* (voidf *) & self->setDayOfWeek = NULL;
	* (voidf *) & self->getDayOfWeek = NULL;

	* (voidf *) & self->setTimeUpdateIncrement = NULL;
	* (voidf *) & self->getTimeUpdateIncrement = NULL;

	* (voidf *) & self->incrementTime = NULL;

	* (voidf *) & self->checkTimeMatch = NULL;


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
			{* (voidf *) & self->_._.class->dtor = overloadedFunctionPtr;}

		if (selector == (voidf) copy)
			{* (voidf *) & self->_._.class->copy = overloadedFunctionPtr;}

		if (selector == (voidf) equal)
			{* (voidf *) & self->_._.class->equal = overloadedFunctionPtr;}

		if (selector == (voidf) equal)
			{* (voidf *) & self->_._.class->config = overloadedFunctionPtr;}

		if (selector == (voidf) puto)
			{* (voidf *) & self->_._.class->puto = overloadedFunctionPtr;}



		if (selector == (voidf)setSecond)
			{* (voidf *) & self->setSecond = overloadedFunctionPtr;}
		if (selector == (voidf)getSecond)
			{* (voidf *) & self->getSecond = overloadedFunctionPtr;}

		if (selector == (voidf)setMinute)
		    {* (voidf *) & self->setMinute = overloadedFunctionPtr;}
		if (selector == (voidf)getMinute)
			{* (voidf *) & self->getMinute = overloadedFunctionPtr;}

		if (selector == (voidf)setMinute)
			{* (voidf *) & self->setMinute = overloadedFunctionPtr;}
		if (selector == (voidf)getMinute)
			{* (voidf *) & self->getMinute = overloadedFunctionPtr;}

		if (selector == (voidf)setHour)
			{* (voidf *) & self->setHour = overloadedFunctionPtr;}
		if (selector == (voidf)getHour)
			{* (voidf *) & self->getHour = overloadedFunctionPtr;}

		if (selector == (voidf)setHourMode)
			{* (voidf *) & self->setHourMode = overloadedFunctionPtr;}
		if (selector == (voidf)getHourMode)
			{* (voidf *) & self->getHourMode = overloadedFunctionPtr;}

		if (selector == (voidf)setAmPm)
			{* (voidf *) & self->setAmPm = overloadedFunctionPtr;}
		if (selector == (voidf)getAmPm)
			{* (voidf *) & self->getAmPm = overloadedFunctionPtr;}

		if (selector == (voidf)setDayOfWeek)
			{* (voidf *) & self->setDayOfWeek = overloadedFunctionPtr;}
		if (selector == (voidf)getDayOfWeek)
			{* (voidf *) & self->getDayOfWeek = overloadedFunctionPtr;}

		if (selector == (voidf)setTimeUpdateIncrement)
		  {* (voidf *) & self->setTimeUpdateIncrement = overloadedFunctionPtr;}
		if (selector == (voidf)getTimeUpdateIncrement)
		  {* (voidf *) & self->getTimeUpdateIncrement = overloadedFunctionPtr;}

		if (selector == (voidf)incrementTime)
			{* (voidf *) & self->incrementTime = overloadedFunctionPtr;}

		if (selector == (voidf)checkTimeMatch)
			{* (voidf *) & self->checkTimeMatch = overloadedFunctionPtr;}

	#ifdef va_copy
		va_end(ap);
	#endif

	}  // end while

	return self;
}


/*****************************/
/**** Overloaded Methods  ****/

void * Time_dtor(void * _self)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct Time * self = cast(Time, _self);
	if(self == NULL)                        {return NULL; } // fail

	// address local members first
	if ( implement_Time_dtor(self) == NULL) {return NULL; } // fail

	// call super method after local members are addressed
	// NOTE: classOf(self) calls into super will trigger looping
	if ( super_dtor(Time, _self) == NULL)   {return NULL; } // fail

	return _self;
}

void * Time_copy(void * _copyToTime, const void * _copyFromTime)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct Time * copyToTime = cast(Time, _copyToTime);
	if ( copyToTime == NULL )   { return NULL; } //fail

	struct Time * copyFromTime = (void *)_copyFromTime;
	if ( copyFromTime == NULL ) { return NULL; } // fail

	// WARNING: calling superclass in composite class is likely NOT correct
	//          complex classes should manage calls to superclass as needed
	//          within their implementation code

	// WARNING:  If Time is a complex class, do not call superclass_copy

	// call super method first
	// NOTE: classOf(copyToTime) calls into super will trigger looping
	if (super_copy(Time, copyToTime, copyFromTime) == NULL )
		{ return NULL; } // fail

	// address local components
	return implement_Time_copy(copyToTime, copyFromTime);
}

equal_t Time_equal(const void * _self, const void * _comparisonObject)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct Time * self = cast(Time, _self);
	if (self == NULL )              { return OBJECT_UNEQUAL; } // fail

	struct Time * comparisonObject = cast(Time, _comparisonObject);
	if (comparisonObject == NULL )  { return OBJECT_UNEQUAL; } // fail

	// WARNING: calling superclass in composite class is likely NOT correct
	//          complex classes should manage calls to superclass as needed
	//          within the implementation code

	// WARNING:  if Time is a complex class, do not call superclass_equal

	// NOTE: classOf(self) calls into super will trigger looping
	if( super_equal(Time, self, comparisonObject) == OBJECT_UNEQUAL)
		{ return OBJECT_UNEQUAL; } // fail

	// address local components
	return implement_Time_equal(_self, _comparisonObject);
}

void * Time_config(const void * _self, const void * _master)
{
	struct Time * self = cast(Time, _self);
	if ( self == NULL )   { return NULL; }       // fail
	struct Time * master = cast(Time, _master);
	if ( master == NULL ) { return NULL; }        // fail
	return implement_Time_config(self, _master);  // expected path
}

puto_return_t Time_puto(const void * _self, FILE * _fp)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct Time * self = cast(Time, _self);
	if (self == NULL ) { return PUTO_ERROR; } // fail

	// the call to super_puto() is managed within implement_Time_puto()
	return implement_Time_puto(self, _fp);
}



/****************************************************************************/
/********  New functions for  class " TimeClass"  ***************************/
/****************************************************************************/

/**************************************/
/***********  setSecond    *************/

second_t  setSecond(void * _self, second_t _second)
{
	const struct TimeClass * class = classOf( cast(Time, _self) );
	if ( class == NULL )            { return SECOND_UNKNOWN; } // fail
	if ( class->setSecond == NULL ) { return SECOND_UNKNOWN; } // fail
	return class->setSecond(_self, _second );
}

second_t super_setSecond(const void * _class, void * _self, second_t _second)
{
	// verify that TimeClass is in the superclass chain of _class
	if ( ! isOfSuper(TimeClass, _self) ) { return SECOND_UNKNOWN; } // fail
	const struct TimeClass * superclass = super(_class);
	if ( superclass == NULL )            { return SECOND_UNKNOWN; } // fail
	if ( superclass->setSecond == NULL ) { return SECOND_UNKNOWN; } // fail
	return superclass->setSecond(_self, _second );
}

second_t Time_setSecond(void * _self, second_t _second)
{
	struct Time * self = cast(Time, _self);
	if( self == NULL ) { return SECOND_UNKNOWN; } // fail
	return implement_Time_setSecond(self, _second );
}

/**************************************/
/***********  getSecond    *************/

second_t  getSecond(void * _self)
{
	const struct TimeClass * class = classOf( cast(Time, _self) );
	if ( class == NULL )            { return SECOND_UNKNOWN; } // fail
	if ( class->getSecond == NULL ) { return SECOND_UNKNOWN; } // fail
	return class->getSecond(_self);
}

second_t super_getSecond(const void * _class, void * _self)
{
	// verify that TimeClass is in the superclass chain of _class
	if ( ! isOfSuper(TimeClass, _self) ) { return SECOND_UNKNOWN; } // fail
	const struct TimeClass * superclass = super(_class);
	if ( superclass == NULL )            { return SECOND_UNKNOWN; } // fail
	if ( superclass->getSecond == NULL ) { return SECOND_UNKNOWN; } // fail
	return superclass->getSecond(_self);
}

second_t Time_getSecond(void * _self)
{
	struct Time * self = cast(Time, _self);
	if( self == NULL ) { return SECOND_UNKNOWN; } // fail
	return implement_Time_getSecond(self);
}

/**************************************/
/***********  setMinute    *************/

minute_t  setMinute(void * _self, minute_t _minute)
{
	const struct TimeClass * class = classOf( cast(Time, _self) );
	if ( class == NULL )            { return MINUTE_UNKNOWN; } // fail
	if ( class->setMinute == NULL ) { return MINUTE_UNKNOWN; } // fail
	return class->setMinute(_self, _minute );
}

minute_t super_setMinute(const void * _class, void * _self, minute_t _minute)
{
	// verify that TimeClass is in the superclass chain of _class
	if ( ! isOfSuper(TimeClass, _self) ) { return MINUTE_UNKNOWN; } // fail
	const struct TimeClass * superclass = super(_class);
	if ( superclass == NULL )            { return MINUTE_UNKNOWN; } // fail
	if ( superclass->setMinute == NULL ) { return MINUTE_UNKNOWN; } // fail
	return superclass->setMinute(_self, _minute );
}

minute_t Time_setMinute(void * _self, minute_t _minute)
{
	struct Time * self = cast(Time, _self);
	if( self == NULL ) { return MINUTE_UNKNOWN; } // fail
	return implement_Time_setMinute(self, _minute );
}

/**************************************/
/***********  getMinute    *************/

minute_t  getMinute(void * _self)
{
	const struct TimeClass * class = classOf( cast(Time, _self) );
	if ( class == NULL )            { return MINUTE_UNKNOWN; } // fail
	if ( class->getMinute == NULL ) { return MINUTE_UNKNOWN; } // fail
	return class->getMinute(_self);
}

minute_t super_getMinute(const void * _class, void * _self)
{
	// verify that TimeClass is in the superclass chain of _class
	if ( ! isOfSuper(TimeClass, _self) ) { return MINUTE_UNKNOWN; } // fail
	const struct TimeClass * superclass = super(_class);
	if ( superclass == NULL )            { return MINUTE_UNKNOWN; } // fail
	if ( superclass->getMinute == NULL ) { return MINUTE_UNKNOWN; } // fail
	return superclass->getMinute(_self);
}

minute_t Time_getMinute(void * _self)
{
	struct Time * self = cast(Time, _self);
	if( self == NULL ) { return MINUTE_UNKNOWN; } // fail
	return implement_Time_getMinute(self);
}


/**************************************/
/***********  setHour    *************/

hour_t  setHour(void * _self, hour_t _hour)
{
	const struct TimeClass * class = classOf( cast(Time, _self) );
	if ( class == NULL )          { return HOUR_UNKNOWN ; } // fail
	if ( class->setHour == NULL ) { return HOUR_UNKNOWN ; } // fail
	return class->setHour(_self, _hour );
}

hour_t super_setHour(const void * _class, void * _self, hour_t _hour)
{
	// verify that TimeClass is in the superclass chain of _class
	if ( ! isOfSuper(TimeClass, _self) ) { return HOUR_UNKNOWN ; } // fail
	const struct TimeClass * superclass = super(_class);
	if ( superclass == NULL )            { return HOUR_UNKNOWN ; } // fail
	if ( superclass->setHour == NULL )   { return HOUR_UNKNOWN ; } // fail
	return superclass->setHour(_self,  _hour );
}

hour_t Time_setHour(void * _self, hour_t _hour)
{
	struct Time * self = cast(Time, _self);
	if( self == NULL ) { return HOUR_UNKNOWN ; } // fail
	return implement_Time_setHour(self, _hour );
}


/**************************************/
/***********  getHour    *************/

hour_t  getHour(void * _self)
{
	const struct TimeClass * class = classOf( cast(Time, _self) );
	if ( class == NULL )          { return HOUR_UNKNOWN ; } // fail
	if ( class->getHour == NULL ) { return HOUR_UNKNOWN ; } // fail
	return class->getHour(_self);
}

hour_t super_getHour(const void * _class, void * _self)
{
	// verify that TimeClass is in the superclass chain of _class
	if ( ! isOfSuper(TimeClass, _self) ) { return HOUR_UNKNOWN ; } // fail
	const struct TimeClass * superclass = super(_class);
	if ( superclass == NULL )            { return HOUR_UNKNOWN ; } // fail
	if ( superclass->getHour == NULL )   { return HOUR_UNKNOWN ; } // fail
	return superclass->getHour(_self);
}

hour_t Time_getHour(void * _self)
{
	struct Time * self = cast(Time, _self);
	if( self == NULL ) { return HOUR_UNKNOWN ; } // fail
	return implement_Time_getHour(self);
}


/**************************************/
/***********  setDayOfWeek    *************/

dayOfWeek_t  setDayOfWeek(void * _self, dayOfWeek_t _dayOfWeek)
{
	const struct TimeClass * class = classOf( cast(Time, _self) );
	if ( class == NULL )               { return HOUR_UNKNOWN ; } // fail
	if ( class->setDayOfWeek == NULL ) { return HOUR_UNKNOWN ; } // fail
	return class->setDayOfWeek(_self, _dayOfWeek );
}

dayOfWeek_t super_setDayOfWeek(const void * _class,
		                             void * _self, dayOfWeek_t _dayOfWeek)
{
	// verify that TimeClass is in the superclass chain of _class
	if ( ! isOfSuper(TimeClass, _self) )    { return HOUR_UNKNOWN ; } // fail
	const struct TimeClass * superclass = super(_class);
	if ( superclass == NULL )               { return HOUR_UNKNOWN ; } // fail
	if ( superclass->setDayOfWeek == NULL ) { return HOUR_UNKNOWN ; } // fail
	return superclass->setDayOfWeek(_self, _dayOfWeek );
}

dayOfWeek_t Time_setDayOfWeek(void * _self, dayOfWeek_t _dayOfWeek)
{
	struct Time * self = cast(Time, _self);
	if( self == NULL ) { return HOUR_UNKNOWN ; } // fail
	return implement_Time_setDayOfWeek(self, _dayOfWeek );
}

/**************************************/
/***********  getDayOfWeek    *************/

dayOfWeek_t  getDayOfWeek(void * _self)
{
	const struct TimeClass * class = classOf( cast(Time, _self) );
	if ( class == NULL )               { return HOUR_UNKNOWN ; } // fail
	if ( class->getDayOfWeek == NULL ) { return HOUR_UNKNOWN ; } // fail
	return class->getDayOfWeek(_self);
}

dayOfWeek_t super_getDayOfWeek(const void * _class, void * _self)
{
	// verify that TimeClass is in the superclass chain of _class
	if ( ! isOfSuper(TimeClass, _self) )    { return HOUR_UNKNOWN ; } // fail
	const struct TimeClass * superclass = super(_class);
	if ( superclass == NULL )               { return HOUR_UNKNOWN ; } // fail
	if ( superclass->getDayOfWeek == NULL ) { return HOUR_UNKNOWN ; } // fail
	return superclass->getDayOfWeek(_self);
}

dayOfWeek_t Time_getDayOfWeek(void * _self)
{
	struct Time * self = cast(Time, _self);
	if( self == NULL ) { return HOUR_UNKNOWN ; } // fail
	return implement_Time_getDayOfWeek(self);
}

/**************************************/
/***********  setHourMode    *************/

hourMode_t  setHourMode(void * _self, hourMode_t _hourMode)
{
	const struct TimeClass * class = classOf( cast(Time, _self) );
	if ( class == NULL )              { return HOUR_MODE_UNKNOWN ; } // fail
	if ( class->setHourMode == NULL ) { return HOUR_MODE_UNKNOWN ; } // fail
	return class->setHourMode(_self, _hourMode );
}

hourMode_t super_setHourMode(const void * _class,
		                     void * _self, hourMode_t _hourMode)
{
	// verify that TimeClass is in the superclass chain of _class
	if ( ! isOfSuper(TimeClass, _self) )
		{ return HOUR_MODE_UNKNOWN ; } // fail
	const struct TimeClass * superclass = super(_class);
	if ( superclass == NULL )
		{ return HOUR_MODE_UNKNOWN ; } // fail
	if ( superclass->setHourMode == NULL )
		{ return HOUR_MODE_UNKNOWN ; } // fail
	return superclass->setHourMode(_self, _hourMode );
}

hourMode_t Time_setHourMode(void * _self, hourMode_t _hourMode)
{
	struct Time * self = cast(Time, _self);
	if( self == NULL ) { return HOUR_MODE_UNKNOWN ; } // fail
	return implement_Time_setHourMode(self, _hourMode );
}


/**************************************/
/***********  getHourMode    *************/

hourMode_t  getHourMode(void * _self)
{
	const struct TimeClass * class = classOf( cast(Time, _self) );
	if ( class == NULL )              { return HOUR_MODE_UNKNOWN ; } // fail
	if ( class->getHourMode == NULL ) { return HOUR_MODE_UNKNOWN ; } // fail
	return class->getHourMode(_self);
}

hourMode_t super_getHourMode(const void * _class, void * _self)
{
	// verify that TimeClass is in the superclass chain of _class
	if ( ! isOfSuper(TimeClass, _self) )
		{ return HOUR_MODE_UNKNOWN ; } // fail
	const struct TimeClass * superclass = super(_class);
	if ( superclass == NULL )
		{ return HOUR_MODE_UNKNOWN ; } // fail
	if ( superclass->getHourMode == NULL )
		{ return HOUR_MODE_UNKNOWN ; } // fail
	return superclass->getHourMode(_self);
}

hourMode_t Time_getHourMode(void * _self)
{
	struct Time * self = cast(Time, _self);
	if( self == NULL ) { return HOUR_MODE_UNKNOWN ; } // fail
	return implement_Time_getHourMode(self);
}


/**************************************/
/***********  setAmPm    *************/

AmPm_t  setAmPm(void * _self, AmPm_t _AmPm)
{
	const struct TimeClass * class = classOf( cast(Time, _self) );
	if ( class == NULL )           { return AM__PM_UNKNOWN ; } // fail
	if ( class->setAmPm == NULL ) { return AM__PM_UNKNOWN ; } // fail
	return class->setAmPm(_self, _AmPm );
}

AmPm_t super_setAmPm(const void * _class, void * _self, AmPm_t _AmPm)
{
	// verify that TimeClass is in the superclass chain of _class
	if ( ! isOfSuper(TimeClass, _self) )
		{ return AM__PM_UNKNOWN ; } // fail
	const struct TimeClass * superclass = super(_class);
	if ( superclass == NULL )
		{ return AM__PM_UNKNOWN ; } // fail
	if ( superclass->setAmPm == NULL )
		{ return AM__PM_UNKNOWN ; } // fail
	return superclass->setAmPm(_self, _AmPm);
}

AmPm_t Time_setAmPm(void * _self, AmPm_t _AmPm)
{
	struct Time * self = cast(Time, _self);
	if( self == NULL ) { return AM__PM_UNKNOWN ; } // fail
	return implement_Time_setAmPm(self, _AmPm);
}


/**************************************/
/***********  getAmPm    *************/

AmPm_t  getAmPm(void * _self)
{
	const struct TimeClass * class = classOf( cast(Time, _self) );
	if ( class == NULL )           { return AM__PM_UNKNOWN ; } // fail
	if ( class->getAmPm == NULL ) { return AM__PM_UNKNOWN ; } // fail
	return class->getAmPm(_self);
}

AmPm_t super_getAmPm(const void * _class, void * _self)
{
	// verify that TimeClass is in the superclass chain of _class
	if ( ! isOfSuper(TimeClass, _self) ) { return AM__PM_UNKNOWN ; } // fail
	const struct TimeClass * superclass = super(_class);
	if ( superclass == NULL )            { return AM__PM_UNKNOWN ; } // fail
	if ( superclass->getAmPm == NULL )   { return AM__PM_UNKNOWN ; } // fail
	return superclass->getAmPm(_self);
}

AmPm_t Time_getAmPm(void * _self)
{
	struct Time * self = cast(Time, _self);
	if( self == NULL ) { return AM__PM_UNKNOWN ; } // fail
	return implement_Time_getAmPm(self);
}


/**************************************/
/*****  setTimeUpdateIncrement    *****/

increment_t  setTimeUpdateIncrement(void * _self, increment_t _mode)
{
	const struct TimeClass * class = classOf( cast(Time, _self) );
	if ( class == NULL )            { return INCREMENT_UNKNOWN; } // fail
	if ( class->setTimeUpdateIncrement == NULL )
									{ return INCREMENT_UNKNOWN; } // fail
	return class->setTimeUpdateIncrement(_self, _mode );
}

increment_t super_setTimeUpdateIncrement(const void * _class,
		                                 void * _self, increment_t _mode)
{
	// verify that TimeClass is in the superclass chain of _class
	if ( ! isOfSuper(TimeClass, _self) )
		{ return INCREMENT_UNKNOWN; } // fail
	const struct TimeClass * superclass = super(_class);
	if ( superclass == NULL )
		{ return INCREMENT_UNKNOWN; } // fail
	if ( superclass->setTimeUpdateIncrement == NULL )
    	{ return INCREMENT_UNKNOWN; } // fail
	return superclass->setTimeUpdateIncrement(_self, _mode );
}

increment_t Time_setTimeUpdateIncrement(void * _self, increment_t _mode)
{
	struct Time * self = cast(Time, _self);
	if( self == NULL ) { return INCREMENT_UNKNOWN; } // fail
	return implement_Time_setTimeUpdateIncrement(self, _mode );
}


/**************************************/
/*****  getTimeUpdateIncrement    *****/

increment_t  getTimeUpdateIncrement(void * _self)
{
	const struct TimeClass * class = classOf( cast(Time, _self) );
	if ( class == NULL )            { return INCREMENT_UNKNOWN; } // fail
	if ( class->getTimeUpdateIncrement == NULL )
									{ return INCREMENT_UNKNOWN; } // fail
	return class->getTimeUpdateIncrement(_self);
}

increment_t super_getTimeUpdateIncrement(const void * _class, void * _self)
{
	// verify that TimeClass is in the superclass chain of _class
	if ( ! isOfSuper(TimeClass, _self) )
		{ return INCREMENT_UNKNOWN; } // fail
	const struct TimeClass * superclass = super(_class);
	if ( superclass == NULL )
		{ return INCREMENT_UNKNOWN; } // fail
	if ( superclass->getTimeUpdateIncrement == NULL )
    	{ return INCREMENT_UNKNOWN; } // fail
	return superclass->getTimeUpdateIncrement(_self);
}

increment_t Time_getTimeUpdateIncrement(void * _self)
{
	struct Time * self = cast(Time, _self);
	if( self == NULL ) { return INCREMENT_UNKNOWN; } // fail
	return implement_Time_getTimeUpdateIncrement(self);
}

/**************************************/
/***********  incrementTime    *************/

struct Time *  incrementTime(struct Time * _self)
{
	const struct TimeClass * class = classOf( cast(Time, _self) );
	if ( class == NULL )                { return NULL; } // fail
	if ( class->incrementTime == NULL ) { return NULL; } // fail
	return class->incrementTime(_self );
}

void * super_incrementTime(const void * _class, void * _self)
{
	// verify that TimeClass is in the superclass chain of _class
	if ( ! isOfSuper(TimeClass, _self) )     { return NULL; } // fail
	const struct TimeClass * superclass = super(_class);
	if ( superclass == NULL )                { return NULL; } // fail
	if ( superclass->incrementTime == NULL ) { return NULL; } // fail
	return superclass->incrementTime(_self );
}

void * Time_incrementTime(void * _self)
{
	// remove from the list the node with the specified ID
	struct Time * self = cast(Time, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Time_incrementTime(self );

}


/**************************************/
/********  checkTimeMatch    **********/

timeMatch_t   checkTimeMatch(const void * _self, const void * _comparisonTime)
{
	const struct TimeClass * class = classOf( cast(Time, _self) );
	if ( class == NULL )                 { return MATCH_FAIL; } // fail
	if ( class->checkTimeMatch == NULL ) { return MATCH_FAIL; } // fail
	return class->checkTimeMatch(_self, _comparisonTime);
}

timeMatch_t  super_checkTimeMatch(const void * _class, const void * _self, const void * _comparisonTime)
{
	// verify that TimeClass is in the superclass chain of _class
	if ( ! isOfSuper(TimeClass, _self))       { return MATCH_FAIL; } // fail
	const struct TimeClass * superclass = super(_class);
	if ( superclass == NULL )                 { return MATCH_FAIL; } // fail
	if ( superclass->checkTimeMatch == NULL ) { return MATCH_FAIL; } // fail
	return superclass->checkTimeMatch(_self, _comparisonTime);
}

timeMatch_t  Time_checkTimeMatch(const void * _self, const void * _comparisonTime)
{
	// remove from the list the node with the specified ID
	struct Time * self = cast(Time, _self);
	if( self == NULL )           { return MATCH_FAIL; } // fail
	struct Time * comparisonTime = cast(Time, _comparisonTime);
	if( comparisonTime == NULL ) { return MATCH_FAIL; } // fail
	return implement_Time_checkTimeMatch(self, comparisonTime);
}


/*************************************************/
/*********** Implementation Procedures ***********/

static void * implement_Time_dtor(struct Time * _self)
{
	_self->second              = SECOND_UNKNOWN;
	_self->minute              = MINUTE_UNKNOWN;
	_self->hour                = HOUR_UNKNOWN;
	_self->hourMode            = HOUR_MODE_UNKNOWN;
	_self->dayOfWeek           = DAY_OF_WEEK_UNKNOWN;
	_self->AmPm                = AM__PM_UNKNOWN;
	_self->timeUpdateIncrement = INCREMENT_UNKNOWN;
	_self->time_OnSecondChange_cb = NULL;
	_self->time_OnMinuteChange_cb = NULL;
	_self->time_OnHourChange_cb   = NULL;
	return _self;

}

void * implement_Time_copy(      struct Time * _copyToNode,
						   const struct Time * _copyFromMaster)
{
	// copy master data members, unless the value is UNKNOWN

	if ( implement_Time_getSecond(_copyFromMaster) != SECOND_UNKNOWN )
		{ implement_Time_setSecond(_copyToNode,
				implement_Time_getSecond(_copyFromMaster)); }

	if ( implement_Time_getMinute(_copyFromMaster) != MINUTE_UNKNOWN )
		{ implement_Time_setMinute(_copyToNode,
				implement_Time_getMinute(_copyFromMaster)); }

	if ( implement_Time_getHour(_copyFromMaster) != HOUR_UNKNOWN )
		{ implement_Time_setHour(_copyToNode,
				implement_Time_getHour(_copyFromMaster)); }

	if ( implement_Time_getHourMode(_copyFromMaster) != HOUR_MODE_UNKNOWN )
		{ implement_Time_setHourMode(_copyToNode,
				implement_Time_getHourMode(_copyFromMaster)); }

	if ( implement_Time_getAmPm(_copyFromMaster) != AM__PM_UNKNOWN )
		{ implement_Time_setAmPm(_copyToNode,
				implement_Time_getAmPm(_copyFromMaster)); }

	if ( implement_Time_getDayOfWeek(_copyFromMaster) != DAY_OF_WEEK_UNKNOWN )
		{ implement_Time_setDayOfWeek(_copyToNode,
				implement_Time_getDayOfWeek(_copyFromMaster)); }

	if ( implement_Time_getTimeUpdateIncrement(_copyFromMaster) != INCREMENT_UNKNOWN )
		{ implement_Time_setTimeUpdateIncrement(_copyToNode,
				implement_Time_getTimeUpdateIncrement(_copyFromMaster)); }

	return _copyToNode;
}

equal_t implement_Time_equal(const struct Time * _self,
		                     const struct Time * _comparisonObject)
{
	// Check each data member for congruence
	struct Time * self             = (void *)_self;
	struct Time * comparisonObject = (void *)_comparisonObject;

	if( getSecond(self) != getSecond(comparisonObject) )
		{return OBJECT_UNEQUAL;}

	if( getMinute(self)  != getMinute(comparisonObject) )
		{return OBJECT_UNEQUAL;}

	if( getHour(self) != getHour(comparisonObject) )
		{return OBJECT_UNEQUAL;}

	if( getHourMode(self) != getHourMode(comparisonObject) )
		{return OBJECT_UNEQUAL;}

	if( getAmPm(self) != getAmPm(comparisonObject) )
		{return OBJECT_UNEQUAL;}

	if( getDayOfWeek(self) != getDayOfWeek(comparisonObject) )
		{return OBJECT_UNEQUAL;}

	if( getTimeUpdateIncrement(self) !=
			                        getTimeUpdateIncrement(comparisonObject) )
			{return OBJECT_UNEQUAL;}

	return OBJECT_EQUAL;

}

puto_return_t implement_Time_puto(const struct Time * _self, FILE * _fp)
{
	puto_return_t printReturnCode = PUTO_SUCCESS;

	struct Time * self = (void *)cast(Time, _self);

	// print local data members
	fprintf(_fp, "\n  Local data members managed in Time:\n");

	if (PUTO_ERROR == fprintf(_fp, "  Time second:              %i\n",
			implement_Time_getSecond(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time minute:              %i\n",
			implement_Time_getMinute(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time Hour:                %i\n",
			implement_Time_getHour(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time hourMode:            %i\n",
			implement_Time_getHourMode(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time AmPm:                %i\n",
			implement_Time_getAmPm(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time dayOfWeek:           %i\n",
			implement_Time_getDayOfWeek(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time timeUpdateIncrement: %i\n",
			implement_Time_getTimeUpdateIncrement(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	fprintf(_fp, "\n  New methods added in Time:\n");

	if (PUTO_ERROR == fprintf(_fp, "  Time_setSecond:               %p\n",
			implement_Time_setSecond ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time_getSecond:               %p\n",
			implement_Time_getSecond ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time_setMinute:               %p\n",
			implement_Time_setMinute ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time_getMinute:               %p\n",
			implement_Time_getMinute ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time_setHour:                 %p\n",
			implement_Time_setHour ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time_getHour:                 %p\n",
			implement_Time_getHour ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time_setHourMode:             %p\n",
			Time_setHourMode ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time_getHourMode:             %p\n",
			Time_getHourMode ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time_setAmPm:                 %p\n",
			Time_setAmPm ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time_getAmPm:                 %p\n",
			Time_getAmPm ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time_setDayOfWeek:            %p\n",
			Time_setDayOfWeek ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time_getDayOfWeek:            %p\n",
			Time_getDayOfWeek ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time_setTimeUpdateIncrement:  %p\n",
			Time_setTimeUpdateIncrement ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time_getTimeUpdateIncrement:  %p\n",
			Time_getTimeUpdateIncrement ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time_incrementTime:           %p\n",
			Time_incrementTime ))
		{ printReturnCode = PUTO_ERROR;  } // error detected


	fprintf(_fp, "\n  Local Time methods:\n" );

	if (PUTO_ERROR == fprintf(_fp, "  Time_ctor:      %p\n", Time_ctor ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time_dtor:      %p\n", Time_dtor ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time_copy:      %p\n", Time_copy ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time_equal:     %p\n", Time_equal ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time_config:    %p\n", Time_config ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Time_puto:      %p\n", Time_puto ))
			{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  TimeClass_ctor: %p\n\n", TimeClass_ctor))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	fprintf(_fp, "Printing Time class:\n" );
	if (printReturnCode == Class_puto(Time, _fp))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	// print superclass data
	fprintf(_fp, "Printing the %s superclass data for Time:\n",
			((struct Object * )self)->class->superclass->className );
	// WARNING: classOf(self) calls into super will trigger looping
	if (printReturnCode == super_puto(Time, self, _fp))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	fprintf(_fp, "\nPrinting TimeClass class:\n" );
	if (printReturnCode == puto(TimeClass, _fp))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	return printReturnCode;
}



static second_t implement_Time_setSecond(struct Time * _self,
		                                 second_t      _second)
{
	_self->second = _second;
	return _second;
}

static second_t implement_Time_getSecond(const struct Time * _self)
{
	return _self->second;
}

static minute_t implement_Time_setMinute(struct Time * _self,
		                                 minute_t      _minute)
{
	_self->minute = _minute;
	return _minute;
}

static minute_t implement_Time_getMinute(const struct Time * _self)
{
	return _self->minute;
}

static hour_t implement_Time_setHour(struct Time * _self, hour_t _hour)
{
	_self->hour = _hour ;
	return _hour;
}

static hour_t implement_Time_getHour(const struct Time * _self)
{
	return _self->hour;
}

static dayOfWeek_t implement_Time_setDayOfWeek(struct Time * _self,
		                                       dayOfWeek_t   _dayOfWeek)
{
	_self->dayOfWeek = _dayOfWeek ;
	return _dayOfWeek;
}

static dayOfWeek_t implement_Time_getDayOfWeek(const struct Time * _self)
{
	return _self->dayOfWeek;
}

static hourMode_t implement_Time_setHourMode(struct Time * _self,
		                                     hourMode_t    _hourMode)
{
	if( _hourMode == HOUR_MODE_12 )
		{ implement_Time_convertTo12Hour(_self); }

	if( _hourMode == HOUR_MODE_24 )
		{ implement_Time_convertTo24Hour(_self); }

	// hourMode data member update is handled inside functions above
	// _self->hourMode = _hourMode;

	return implement_Time_getHourMode(_self);
}

static hourMode_t implement_Time_getHourMode(const struct Time * _self)
{
	return _self->hourMode;
}

static AmPm_t implement_Time_setAmPm(struct Time * _self, AmPm_t _AmPm)
{
	_self->AmPm = _AmPm ;
	return _AmPm;
}

static AmPm_t implement_Time_getAmPm(const struct Time * _self)
{
	return _self->AmPm;
}

static increment_t implement_Time_setTimeUpdateIncrement(struct Time * _self,
		                                                 increment_t   _mode)
{
	_self->timeUpdateIncrement = _mode;
	return _mode;
}

static increment_t implement_Time_getTimeUpdateIncrement(const struct Time * _self)
{
	return _self->timeUpdateIncrement;
}

static void * implement_Time_incrementTime(struct Time * _self)
{
	if (_self->timeUpdateIncrement == UPDATE_SECONDS)
		{ return implement_Time_incrementSecond(_self); }

	if (_self->timeUpdateIncrement == UPDATE_MINUTES)
		{ return implement_Time_incrementMinute(_self); }

	return NULL;  // fail
}

static timeMatch_t implement_Time_checkTimeMatch(const struct Time * _self,
		                                   const struct Time * _comparisonTime)
{
	// convert to 24 hour mode for easier comparisons
	hourMode_t originalSelfMode = getHourMode((struct Time *)_self);
	if (originalSelfMode ==  HOUR_MODE_12)
	 { implement_Time_setHourMode( (struct Time *)_self, HOUR_MODE_24); }

	hourMode_t originalComparisonTimeMode = getHourMode((struct Time *)_comparisonTime);
	if (originalComparisonTimeMode ==  HOUR_MODE_12)
		 { implement_Time_setHourMode( (struct Time *)_comparisonTime, HOUR_MODE_24); }

	// set flag to trap no valid master comparison values
	typedef enum validMatch_t {VALID_TEST, INVALID_TEST } validMatch_t ;
	validMatch_t validMatch  = INVALID_TEST;

	// set return value to MATCH
	timeMatch_t returnValue = MATCH;

	// find any data member that FAIL to match

	second_t selfSecond       = getSecond((struct Time *)_self);
	second_t comparisonSecond = getSecond((struct Time *)_comparisonTime);
	if( comparisonSecond != SECOND_UNKNOWN ) {
		validMatch  = VALID_TEST;
		if ( selfSecond != comparisonSecond ) {returnValue = MATCH_FAIL;}
	}

	minute_t selfMinute       = getMinute((struct Time *)_self);
	minute_t comparisonMinute = getMinute((struct Time *)_comparisonTime);
	if( comparisonMinute != MINUTE_UNKNOWN ) {
		validMatch  = VALID_TEST;

		// test for minute match
		if ( selfMinute != comparisonMinute ) {returnValue = MATCH_FAIL;}

		// prevent firing multiple times if seconds are unknown
		if ( comparisonSecond == SECOND_UNKNOWN ) {
			if ( selfSecond > SECOND_0 ) { returnValue = MATCH_FAIL; }
		}
	}

	hour_t selfHour       = getHour((struct Time *)_self);
	hour_t comparisonHour = getHour((struct Time *)_comparisonTime);
	if( comparisonHour != HOUR_UNKNOWN ) {
		validMatch  = VALID_TEST;

		// test for hour match
		if ( selfHour != comparisonHour ) {returnValue = MATCH_FAIL;}

		// prevent firing multiple times if seconds are unknown
		if ( comparisonSecond == SECOND_UNKNOWN ) {
			if ( selfSecond > SECOND_0 ) { returnValue = MATCH_FAIL; }
		}

		// prevent firing multiple times if minutes are unknown
		if ( comparisonMinute == MINUTE_UNKNOWN ) {
			if ( selfMinute > MINUTE_0 ) { returnValue = MATCH_FAIL; }
		}

	}

	dayOfWeek_t selfDay       = getDayOfWeek((struct Time *)_self);
	dayOfWeek_t comparisonDay = getDayOfWeek((struct Time *)_comparisonTime);

	switch (comparisonDay) {
	case EVERYDAY: {
		validMatch  = VALID_TEST;
		if ( selfDay == DAY_OF_WEEK_UNKNOWN )
			{returnValue = MATCH_FAIL; }
		break;
		}

	case WEEKDAY: {
		validMatch  = VALID_TEST;
		if ( !((selfDay >= MONDAY) && (selfDay <= FRIDAY)) )
			{ returnValue = MATCH_FAIL; }
		break;
		}

	case WEEKEND: {
		validMatch  = VALID_TEST;
		if ( (selfDay != SATURDAY) && (selfDay != SUNDAY) )
			{ returnValue = MATCH_FAIL; }
		break;
		}

	case SUNDAY:
	case MONDAY:
	case TUESDAY:
	case WEDNESDAY:
	case THURSDAY:
	case FRIDAY:
	case SATURDAY: {
		validMatch  = VALID_TEST;
		if ( selfDay != comparisonDay )
			{ returnValue = MATCH_FAIL; }
		break;
		}

	case DAY_OF_WEEK_UNKNOWN:
	default: {
		// no action to be taken
		}
	}  // end of switch

	// return to 12 hour mode if needed
	if (originalSelfMode ==  HOUR_MODE_12)
		 { implement_Time_setHourMode( (struct Time *)_self, HOUR_MODE_12); }

	if (originalComparisonTimeMode ==  HOUR_MODE_12)
		 { implement_Time_setHourMode( (struct Time *)_comparisonTime, HOUR_MODE_12); }

	if ( validMatch == INVALID_TEST ) { return MATCH_UNKNOWN; }  // fail

	// test is valid, return the combined data member match status
	return returnValue;
}

static void * implement_Time_incrementSecond(struct Time * _self)
{
	second_t second = implement_Time_getSecond(_self);
	second++;

	if( second < SECOND_0 ) { second = SECOND_0; }

	if ( second >= (SECOND_60 - 1) ) {
		second = SECOND_0;
		implement_Time_incrementMinute(_self);
	}
	implement_Time_setSecond(_self, second);
	_self->time_OnSecondChange_cb(_self);
	return _self;
}

static void * implement_Time_incrementMinute(struct Time * _self)
{
	minute_t minute = implement_Time_getMinute(_self);
	minute++;

	if( minute < MINUTE_0 ) { minute = MINUTE_0; }

	if ( minute >= (MINUTE_60 - 1) ) {
		minute = MINUTE_0;
		implement_Time_incrementHour(_self);
	}
	implement_Time_setMinute(_self,  minute);
	_self->time_OnMinuteChange_cb(_self);
	return _self;
}

static void * implement_Time_incrementHour(struct Time * _self)
{
	hour_t hour = implement_Time_getHour(_self);
	hour++;

	if( hour < HOUR_0 ) {hour = HOUR_0;}

	if( getHourMode(_self) ==  HOUR_MODE_24) {  // in 24 hour mode
		// in HOUR_MODE_24 ... check for past end of day
		if (hour >= HOUR_24 ) {
			// past end of day, reset hour to zero, increment dayOfWeek
			hour = HOUR_0;
			implement_Time_incrementDayOfWeek(_self);
		}
	} // end 24 Hour mode processing

	else {  // in 12 hour mode

		// check for past 1PM
		if ( hour == 13 ) { hour = 1;}

		// check for noon or midnight transition
		if (hour == 12) {

			// check for AM/PM ... increment dayOfWeek on midnight transition
			if (getAmPm(_self) == AM_TIME) {
				// Noon to PM transition
				setAmPm(_self, PM_TIME);
			}
			else {
				// midnight to AM transition
				setAmPm(_self, AM_TIME);
				implement_Time_incrementDayOfWeek(_self);
			} // end AmPm check

		} // end if (hour == 12)

	}  // end 12 hour mode processing

	implement_Time_setHour(_self, hour);
	_self->time_OnHourChange_cb(_self);
	return _self;
}

static void * implement_Time_incrementDayOfWeek(struct Time * _self)
{
	dayOfWeek_t dayOfWeek = implement_Time_getDayOfWeek(_self);
	dayOfWeek++;

	if( dayOfWeek < SUNDAY ) { dayOfWeek = SUNDAY; }

	if ( dayOfWeek > (SATURDAY) ) { dayOfWeek = SUNDAY; }

	implement_Time_setDayOfWeek(_self,  dayOfWeek);
	return _self;
}

static void * implement_Time_convertTo12Hour(struct Time * _self)
{
	if (implement_Time_getHourMode(_self) == HOUR_MODE_12 )
		{ return _self; } // already in 12 Hour mode

	// in 24 hour mode ... need to convert to 12 hour mode
	hour_t originalHour = implement_Time_getHour(_self);
	hour_t returnHour = originalHour;

	AmPm_t AmPm;

	// set AmPm
	if ( originalHour >= HOUR_12 ) {
		AmPm = PM_TIME;
	}
	else {
		AmPm = AM_TIME;
	}

	// check for zero hour
	if (originalHour == HOUR_0 ) { returnHour = HOUR_12; }

	// convert higher hours down to 12
	if (originalHour >= HOUR_12 + 1 ) { returnHour = originalHour - HOUR_12; }

	implement_Time_setHour(_self, returnHour);
	implement_Time_setAmPm(_self, AmPm);
	_self->hourMode = HOUR_MODE_12;
	return _self;
}

static void * implement_Time_convertTo24Hour(struct Time * _self)
{
	if (implement_Time_getHourMode(_self) == HOUR_MODE_24 )
		{ return _self; } // already in 24 Hour mode

	// in 12 hour mode ... need to convert to 24 hour mode
	hour_t originalHour = implement_Time_getHour(_self);
	hour_t returnHour = originalHour;

	AmPm_t AmPm  = implement_Time_getAmPm(_self);

	// check for zero hour
	if ((originalHour == HOUR_12 ) && (AmPm == AM_TIME)) {
		returnHour = HOUR_0;
	}

	// set AmPm
	// NOTE:  12 hour time mode assumes that AmPm is NOT unknown
	//        care should be taken when initializing times
	if ( AmPm == PM_TIME ) {
		returnHour = originalHour + HOUR_12;
	}

	implement_Time_setHour(_self, returnHour);
	implement_Time_setAmPm(_self, AmPm);
	_self->hourMode = HOUR_MODE_24;
	return _self;
}


static void * implement_Time_config(      struct Time * _self,
		                            const struct Time * _master)
{
	return copy(_self, _master);
}

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

struct Time * Time_PostSecondCallback(struct Time * _time)
{
	//printf("Time_PostSecondCallback\n");
	return _time;
}

struct Time * Time_PostMinuteCallback(struct Time * _time)
{
	//printf("Time_PostMinuteCallback\n");
	return _time;
}

struct Time * Time_PostHourCallback(struct Time * _time)
{
	//printf("Time_PostHourCallback\n");
	return _time;
}

time_cb_fnct setTime_OnSecondChange_cb(const void * _self, time_cb_fnct _cb)
{
	struct Time * self = cast(Time, _self);
	if ( self == NULL ) { return NULL; }  // fail
	if ( _cb  == NULL ) { return NULL; }  // fail
	self->time_OnSecondChange_cb = _cb;
	return _cb;
}

time_cb_fnct getTime_OnSecondChange_cb(const void * _self)
{
	struct Time * self = cast(Time, _self);
	if ( self == NULL ) { return NULL; }  // fail
	return self->time_OnSecondChange_cb;
}

time_cb_fnct setTime_OnMinuteChange_cb(const void * _self, time_cb_fnct _cb)
{
	struct Time * self = cast(Time, _self);
	if ( self == NULL ) { return NULL; }  // fail
	if ( _cb  == NULL ) { return NULL; }  // fail
	self->time_OnMinuteChange_cb = _cb;
	return _cb;
}

time_cb_fnct getTime_OnMinuteChange_cb(const void * _self)
{
	struct Time * self = cast(Time, _self);
	if ( self == NULL ) { return NULL; }  // fail
	return self->time_OnMinuteChange_cb;
}

time_cb_fnct setTime_OnHourChange_cb(const void * _self, time_cb_fnct _cb)
{
	struct Time * self = cast(Time, _self);
	if ( self == NULL ) { return NULL; }  // fail
	if ( _cb  == NULL ) { return NULL; }  // fail
	self->time_OnHourChange_cb = _cb;
	return _cb;
}

time_cb_fnct getTime_OnHourChange_cb(const void * _self)
{
	struct Time * self = cast(Time, _self);
	if ( self == NULL ) { return NULL; }  // fail
	return self->time_OnHourChange_cb;
}
