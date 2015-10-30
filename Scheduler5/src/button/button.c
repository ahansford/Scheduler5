/*
 * event-controller.c
 *
 *  Created on: Aug 15, 2015
 *      Author: alhansfo
 */


#include "button.h"
#include "button-private.h"            // safety include
//#include "..\..\src\nodes\nodes.h"     // safety include
//#include "..\..\src\node_list\node-list.h"     // safety include
//#include "..\..\src\times\times.h"     // safety include
//#include "..\..\src\lists\lists.h"     // supports class registration
#include "..\..\src\objects\objects.h" // safety include
//#include "..\..\src\events\events.h" // safety include

/**************************************/
/***** Implementation Functions  ******/

static void *  implement_Button_dtor(       struct Button * _self);
void *         implement_Button_copy(       struct Button * _copyToButton,
                                      const struct Button * _copyFromButton);
equal_t        implement_Button_equal(const struct Button * _self,
		                              const struct Button * _comparisonObject);
static void * implement_Button_config(      struct Button * _self,
		                              const struct Button * _master);
puto_return_t   implement_Button_puto(const struct Button * _self, FILE * _fp);

static buttonState_t implement_Button_getButtonState(const struct Button * _self);
static buttonState_t implement_Button_setButtonState(      struct Button * _self,
		                                                     buttonState_t _buttonState);

static int implement_Button_getPreliminaryCount(const struct Button * _self);
static int implement_Button_setPreliminaryCount(      struct Button * _self,
		                                                          int _pressCount);

static int implement_Button_getPressCount(const struct Button * _self);
static int implement_Button_setPressCount(      struct Button * _self,
		                                                    int _pressCount);

static buttonPressType_t implement_Button_getPreliminaryType(const struct Button * _self);
static buttonPressType_t implement_Button_setPreliminaryType(struct Button * _self, buttonPressType_t _pressType);

static buttonPressType_t implement_Button_getPressType(const struct Button * _self);
static buttonPressType_t implement_Button_setPressType(      struct Button * _self,
		                                                   buttonPressType_t _pressTime);

static int implement_Button_getPressTime(const struct Button * _self);
static int implement_Button_setPressTime(      struct Button * _self,
		                                            int        _second);

static int implement_Button_getReleaseTime(const struct Button * _self);
static int implement_Button_setReleaseTime(      struct Button * _self,
		                                              int        _releaseTime);


/*****************************/
/**** INITIALIZATIONS  *******/

const void * ButtonClass = NULL;
const void * Button      = NULL;

int ticksPerSecond = 0;  // set using Button_init();

#ifndef __INT_MAX__
#define __INT_MAX__ 5000
#endif

//void * implement_eventController_remove(void * _nodeList, void * _node);

// MUST be called before other event controller methods are called
void Button_init(int _ticksPerSecond)
{
	// Insert the lines below for any derived subclasses
	//if (!Button)      {Button_init();}
	//if (!ButtonClass) {Button_init();}

	if (! ButtonClass) {
		ButtonClass = new(Class,  		// should be "Class"
						"ButtonClass",	// should be "SomethingClass"
						Class,  		// should be "Class"
						sizeof(struct ButtonClass),
						ctor, ButtonClass_ctor,	//SomethingClass_ctor
						0);  // Terminator
	}
	if (! Button) {
		Button = new(ButtonClass,			// SomethingClass from above
					"Button",				// name like "Something"
					Object,  			// "superclass(Something)"
					sizeof(struct Button),// size of self
					// Overloaded superclass functions
					// Remember to call superclass->method
					ctor,	Button_ctor,//Something_ctor
					dtor,   Button_dtor,
					copy,	Button_copy,
					equal,	Button_equal,
					config, Button_config,
					puto,	Button_puto,
					// New functions added in this class
					// Do not call superclass->method
					//setSecond,	Button_setSecond,
					//getSecond,	Button_getSecond,


					0);	// Terminator
	}

	// requires #include "..\..\src\lists\lists.h" to support class registry
	//implement_Button_registerKeyClasses();

	ticksPerSecond = _ticksPerSecond;

	return;
}


/*****************************/
/****** Constructors  ********/

void * Button_ctor(void * _self, va_list * app)
{
	// Add superclass class data members
	struct Button * self = super_ctor(Button, _self, app);

	// set default values for any new data members
	Button_reset(self);

	// set default for the onReportReady callback, but only set it once
	// Button_reset() is used multiple times during program execution
	// setting the callback chain should not be managed inside Button_reset()
	self->button_OnReportReady_cb = Button_PostOnReportReadyCallback; // no-op

	// overwrite data members with new data
	// Only uncomment if all data members will be specified in new() command
	// ... this seems like an undue burden on the user.  Leave commented out
	// ... numerous unit tests will need to be adapted if uncommented
	//self->second = va_arg(* app, second_t);
	//self->minute = va_arg(* app, minute_t);

	return self;
}

void * ButtonClass_ctor(void * _self, va_list *app)
{
	// Add superclass class data and methods
	struct ButtonClass * self = super_ctor(ButtonClass, _self, app);

	// Initialize new functions to default values
	typedef void (* voidf)();

	//* (voidf *) & self->setSecond    = NULL;
	//* (voidf *) & self->getSecond    = NULL;


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

		// use form below for any new overloadable functions.
		// if (selector == (voidf)setSecond)
		// 	{* (voidf *) & self->setSecond = overloadedFunctionPtr;}

	#ifdef va_copy
		va_end(ap);
	#endif

	}  // end while

	return self;
}


/*****************************/
/**** Overloaded Methods  ****/

void * Button_dtor(void * _self)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct Button * self = cast(Button, _self);
	if(self == NULL)                         {return NULL; } // fail

	// address local members first
	if ( implement_Button_dtor(self) == NULL) {return NULL; } // fail

	// call super method after local members are addressed
	// NOTE: classOf(self) calls into super will trigger looping
	if ( super_dtor(Button, _self) == NULL)   {return NULL; } // fail

	return _self;
}

void * Button_copy(void * _copyToButton, const void * _copyFromButton)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct Button * copyToButton = cast(Button, _copyToButton);
	if ( copyToButton == NULL )   { return NULL; } //fail

	struct Button * copyFromButton = (void *)_copyFromButton;
	if ( copyFromButton == NULL ) { return NULL; } // fail

	// WARNING: calling superclass in composite class is likely NOT correct
	//          complex classes should manage calls to superclass as needed
	//          within their implementation code

	// WARNING:  If Button is a complex class, do not call superclass_copy

	// call super method first
	// NOTE: classOf(copyToButton) calls into super will trigger looping
	if (super_copy(Button, copyToButton, copyFromButton) == NULL )
		{ return NULL; } // fail

	// address local components
	return implement_Button_copy(copyToButton, copyFromButton);
}

equal_t Button_equal(const void * _self, const void * _comparisonObject)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct Button * self = cast(Button, _self);
	if (self == NULL )              { return OBJECT_UNEQUAL; } // fail

	struct Button * comparisonObject = cast(Button, _comparisonObject);
	if (comparisonObject == NULL )  { return OBJECT_UNEQUAL; } // fail

	// WARNING: calling superclass in composite class is likely NOT correct
	//          complex classes should manage calls to superclass as needed
	//          within the implementation code

	// WARNING:  if Button is a complex class, do not call superclass_equal

	// NOTE: classOf(self) calls into super will trigger looping
	if( super_equal(Button, self, comparisonObject) == OBJECT_UNEQUAL)
		{ return OBJECT_UNEQUAL; } // fail

	// address local components
	return implement_Button_equal(_self, _comparisonObject);
}

void * Button_config(const void * _self, const void * _master)
{
	struct Button * self = cast(Button, _self);
	if ( self == NULL )   { return NULL; }          // fail
	struct Button * master = cast(Button, _master);
	if ( master == NULL ) { return NULL; }          // fail
	return implement_Button_config(self, _master);  // expected path
}

puto_return_t Button_puto(const void * _self, FILE * _fp)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct Button * self = cast(Button, _self);
	if (self == NULL ) { return PUTO_ERROR; } // fail

	// the call to super_puto() is managed within implement_Button_puto()
	return implement_Button_puto(self, _fp);
}


/****************************************************************************/
/********  New functions for  class "ButtonClass"  **************************/
/****************************************************************************/

/**************************************/
/****  set and get buttonState    *****/

buttonState_t Button_getButtonState(const void  * _self)
{
	const struct ButtonClass * class = classOf( cast(Button, _self) );
	if ( class == NULL ) { return BUTTON_STATE_UNKNOWN ; } // fail
	return implement_Button_getButtonState(_self);
}

buttonState_t Button_setButtonState(void * _self, buttonState_t _buttonState)
{
	const struct ButtonClass * class = classOf( cast(Button, _self) );
	if ( class == NULL ) { return BUTTON_STATE_UNKNOWN ; } // fail
	return implement_Button_setButtonState(_self, _buttonState);
}

/**************************************/
/***  set and get preliminaryCount ****/

int Button_getPreliminaryCount(const void  * _self)
{
	const struct ButtonClass * class = classOf( cast(Button, _self) );
	if ( class == NULL ) { return 0 ; } // fail
	return implement_Button_getPreliminaryCount(_self);
}

int Button_setPreliminaryCount(void * _self, int _pressCount)
{
	const struct ButtonClass * class = classOf( cast(Button, _self) );
	if ( class == NULL ) { return 0 ; } // fail
	return implement_Button_setPreliminaryCount(_self, _pressCount);
}

/**************************************/
/******  set and get pressCount *******/

int Button_getPressCount(const void  * _self)
{
	const struct ButtonClass * class = classOf( cast(Button, _self) );
	if ( class == NULL ) { return 0 ; } // fail
	return implement_Button_getPressCount(_self);
}

int Button_setPressCount(void * _self, int _pressCount)
{
	const struct ButtonClass * class = classOf( cast(Button, _self) );
	if ( class == NULL ) { return 0 ; } // fail
	return implement_Button_setPressCount(_self, _pressCount);
}

/**************************************/
/***  set and get preliminaryType  ****/

buttonPressType_t Button_getPreliminaryType(const void  * _self)
{
	const struct ButtonClass * class = classOf( cast(Button, _self) );
	if ( class == NULL ) { return BUTTON_PRESS_TYPE_UNKNOWN ; } // fail
	return implement_Button_getPreliminaryType(_self);
}

buttonPressType_t Button_setPreliminaryType(void * _self, buttonPressType_t _pressType)
{
	const struct ButtonClass * class = classOf( cast(Button, _self) );
	if ( class == NULL ) { return BUTTON_PRESS_TYPE_UNKNOWN ; } // fail
	return implement_Button_setPreliminaryType(_self, _pressType);
}

/**************************************/
/*******  set and get pressType  ******/

buttonPressType_t Button_getPressType(const void  * _self)
{
	const struct ButtonClass * class = classOf( cast(Button, _self) );
	if ( class == NULL ) { return BUTTON_PRESS_TYPE_UNKNOWN ; } // fail
	return implement_Button_getPressType(_self);
}

buttonPressType_t Button_setPressType(void * _self, buttonPressType_t _pressType)
{
	const struct ButtonClass * class = classOf( cast(Button, _self) );
	if ( class == NULL ) { return BUTTON_PRESS_TYPE_UNKNOWN ; } // fail
	return implement_Button_setPressType(_self, _pressType);
}

/**************************************/
/*******  set and get pressTime *******/

int Button_getPressTime(const void  * _self)
{
	const struct ButtonClass * class = classOf( cast(Button, _self) );
	if ( class == NULL ) { return BUTTON_TIME_UNKNOWN ; } // fail
	return implement_Button_getPressTime(_self);
}

int Button_setPressTime(void * _self, int _pressTime)
{
	const struct ButtonClass * class = classOf( cast(Button, _self) );
	if ( class == NULL ) { return BUTTON_TIME_UNKNOWN ; } // fail
	return implement_Button_setPressTime(_self, _pressTime);
}

/**************************************/
/******  set and get releaseTime ******/

int Button_getReleaseTime(const void  * _self)
{
	const struct ButtonClass * class = classOf( cast(Button, _self) );
	if ( class == NULL ) { return BUTTON_TIME_UNKNOWN ; } // fail
	return implement_Button_getReleaseTime(_self);
}

int Button_setReleaseTime(void * _self, int _releaseTime)
{
	const struct ButtonClass * class = classOf( cast(Button, _self) );
	if ( class == NULL ) { return BUTTON_TIME_UNKNOWN ; } // fail
	return implement_Button_setReleaseTime(_self, _releaseTime);
}


/*************************************************/
/*********** Implementation Procedures ***********/

static void * implement_Button_dtor(struct Button * _self)
{
	_self->buttonState      = BUTTON_STATE_UNKNOWN;
	_self->preliminaryCount = 0;
	_self->pressCount       = 0;
	_self->preliminaryType  = BUTTON_PRESS_TYPE_UNKNOWN;
	_self->pressType        = BUTTON_PRESS_TYPE_UNKNOWN;
	_self->pressTime        = BUTTON_TIME_UNKNOWN;
	_self->releaseTime      = BUTTON_TIME_UNKNOWN;
	_self->button_OnReportReady_cb = NULL;
	return _self;
}

void * implement_Button_copy(      struct Button * _copyToButton,
						     const struct Button * _copyFromMaster)
{
	// copy master data members, unless the value is UNKNOWN

	if ( implement_Button_getButtonState(_copyFromMaster) !=
			                                           BUTTON_STATE_UNKNOWN )
		{ implement_Button_setButtonState(_copyToButton,
				implement_Button_getButtonState(_copyFromMaster)); }

	if ( implement_Button_getPreliminaryCount(_copyFromMaster) != 0 )
		{ implement_Button_setPreliminaryCount(_copyToButton,
				implement_Button_getPreliminaryCount(_copyFromMaster)); }

	if ( implement_Button_getPressCount(_copyFromMaster) != 0 )
		{ implement_Button_setPressCount(_copyToButton,
				implement_Button_getPressCount(_copyFromMaster)); }

	if ( implement_Button_getPreliminaryType(_copyFromMaster) !=
			                                      BUTTON_PRESS_TYPE_UNKNOWN )
		{ implement_Button_setPreliminaryType(_copyToButton,
				implement_Button_getPreliminaryType(_copyFromMaster)); }

	if ( implement_Button_getPressType(_copyFromMaster) !=
			                                      BUTTON_PRESS_TYPE_UNKNOWN )
		{ implement_Button_setPressType(_copyToButton,
				implement_Button_getPressType(_copyFromMaster)); }

	if ( implement_Button_getPressTime(_copyFromMaster) !=
			                                            BUTTON_TIME_UNKNOWN )
		{ implement_Button_setPressTime(_copyToButton,
				implement_Button_getPressTime(_copyFromMaster)); }

	if ( implement_Button_getReleaseTime(_copyFromMaster) !=
			                                            BUTTON_TIME_UNKNOWN )
		{ implement_Button_setReleaseTime(_copyToButton,
				implement_Button_getReleaseTime(_copyFromMaster)); }

	return _copyToButton;
}

equal_t implement_Button_equal(const struct Button * _self,
		                     const struct Button * _comparisonObject)
{
	// Check each data member for congruence
	struct Button * self            = (void *)_self;
	struct Button* comparisonObject = (void *)_comparisonObject;

	if( implement_Button_getButtonState(self) !=
			               implement_Button_getButtonState(comparisonObject) )
		{return OBJECT_UNEQUAL;}

	if( implement_Button_getPressCount(self) !=
			                implement_Button_getPressCount(comparisonObject) )
		{return OBJECT_UNEQUAL;}

	if( implement_Button_getPressType(self) !=
			                 implement_Button_getPressType(comparisonObject) )
		{return OBJECT_UNEQUAL;}

	if( implement_Button_getPressTime(self) !=
                             implement_Button_getPressTime(comparisonObject) )
		{return OBJECT_UNEQUAL;}

	if( implement_Button_getReleaseTime(self) !=
			               implement_Button_getReleaseTime(comparisonObject) )
		{return OBJECT_UNEQUAL;}

	return OBJECT_EQUAL;
}

static void * implement_Button_config(      struct Button * _self,
		                              const struct Button * _master)
{
	return copy(_self, _master);
}

puto_return_t implement_Button_puto(const struct Button * _self, FILE * _fp)
{
	puto_return_t printReturnCode = PUTO_SUCCESS;

	struct Button * self = (void *)cast(Button, _self);

	// print local data members
	fprintf(_fp, "\n  Local data members managed in Button:\n");

	if (PUTO_ERROR == fprintf(_fp, "  Button buttonState:      %i\n",
			implement_Button_getButtonState(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Button preliminaryCount: %i\n",
			implement_Button_getPreliminaryCount(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Button pressCount:       %i\n",
			implement_Button_getPressCount(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Button preliminaryType:  %i\n",
			implement_Button_getPreliminaryType(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Button pressType:        %i\n",
			implement_Button_getPressType(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Button pressTime:        %i\n",
			implement_Button_getPressTime(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Button releaseTime:      %i\n",
			implement_Button_getReleaseTime(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected


	fprintf(_fp, "\n  New methods added in Button:\n");

	fprintf(_fp, "    ... no methods were added in Button:\n");
/*
	if (PUTO_ERROR == fprintf(_fp, "  Time_setSecond:               %p\n",
			implement_Time_setSecond ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

*/

	fprintf(_fp, "\n  Local Time methods:\n" );

	if (PUTO_ERROR == fprintf(_fp, "  Button_ctor:      %p\n", Button_ctor ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Button_dtor:      %p\n", Button_dtor ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Button_copy:      %p\n", Button_copy ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Button_equal:     %p\n", Button_equal ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Button_config:    %p\n", Button_config ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Button_puto:      %p\n", Button_puto ))
			{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  ButtonClass_ctor: %p\n\n",
			                                                ButtonClass_ctor))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	fprintf(_fp, "Printing Button class:\n" );
	if (printReturnCode == Class_puto(Button, _fp))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	// print superclass data
	fprintf(_fp, "Printing the %s superclass data for Button:\n",
			((struct Object * )self)->class->superclass->className );
	// WARNING: classOf(self) calls into super will trigger looping
	if (printReturnCode == super_puto(Button, self, _fp))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	fprintf(_fp, "\nPrinting ButtonClass class:\n" );
	if (printReturnCode == puto(ButtonClass, _fp))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	return printReturnCode;
}

static buttonState_t implement_Button_getButtonState(
		                                          const struct Button * _self)
{
	return _self->buttonState;
}

static buttonState_t implement_Button_setButtonState(struct Button * _self,
		                                           buttonState_t _buttonState)
{
	_self->buttonState = _buttonState;
	return _buttonState;
}

static int implement_Button_getPreliminaryCount(const struct Button * _self)
{
	struct Button * self = cast(Button, _self);
	return self->preliminaryCount;
}

static int implement_Button_setPreliminaryCount(struct Button * _self, int _pressCount)
{
	struct Button * self = cast(Button, _self);
	self->preliminaryCount = _pressCount;
	return _pressCount;
}

static int implement_Button_getPressCount(const struct Button * _self)
{
	return _self->pressCount;
}

static int implement_Button_setPressCount(struct Button * _self,
		                                                      int _pressCount)
{
	_self->pressCount = _pressCount;
	return _pressCount;
}

static buttonPressType_t implement_Button_getPreliminaryType(const struct Button * _self)
{
	struct Button * self = cast(Button, _self);
	return self->preliminaryType;
}

static buttonPressType_t implement_Button_setPreliminaryType(struct Button * _self,
		                                         buttonPressType_t _pressType)
{
	struct Button * self = cast(Button, _self);
	self->preliminaryType = _pressType;
	return _pressType;
}

static buttonPressType_t implement_Button_getPressType(
		                                          const struct Button * _self)
{
	return _self->pressType;
}

static buttonPressType_t implement_Button_setPressType(struct Button * _self,
		                                        buttonPressType_t _pressType)
{
	_self->pressType = _pressType;
	return _pressType;
}

static int implement_Button_getPressTime(const struct Button * _self)
{
	return _self->pressTime;
}

static int implement_Button_setPressTime(struct Button * _self,
		                                                       int _pressTime)
{
	_self->pressTime = _pressTime;
	return _pressTime;
}

static int implement_Button_getReleaseTime(const struct Button * _self)
{
	return _self->releaseTime;
}

static int implement_Button_setReleaseTime(struct Button * _self,
		                                                     int _releaseTime)
{
	_self->releaseTime = _releaseTime;
	return _releaseTime;
}

void Button_updateState(void * _self, int _timeStamp)
{
	switch (implement_Button_getButtonState(_self)) {

	case BUTTON_STATE_UNKNOWN: {
		// do nothing ... wait for button to be enabled
		break;
	}

	case BUTTON_RESET: {
		Button_reset(_self);
		Button_setButtonState(_self, BUTTON_IDLE);
		Button_updateState(_self, BUTTON_TIME_UNKNOWN);
		break;
	}

	case BUTTON_IDLE: {
		// do nothing
		break;
	}

	case BUTTON_PRESSED: {
		Button_setPressTime(_self, _timeStamp);
		Button_setButtonState(_self, BUTTON_WAITING_RELEASE);
		implement_Button_setPreliminaryType(_self, BUTTON_STATE_UNKNOWN);
		Button_updateState(_self, _timeStamp);
		break;
	}

	case BUTTON_WAITING_RELEASE: {
		break;
	}

	case BUTTON_RELEASED: {
		Button_setReleaseTime(_self, _timeStamp);

		// determine press length in terms of time stamp ticks
		int pressLength;
		int pressTime   = Button_getPressTime  (_self);
		int releaseTime = _timeStamp;
		if ( pressTime < releaseTime )
			{ pressLength = releaseTime - pressTime; }
		else
			{ pressLength = __INT_MAX__ - pressTime + releaseTime; }

		// assign preliminaryType
		if      ( pressLength >= (5 * ticksPerSecond) ) // 5 sec trap
			{ implement_Button_setPreliminaryType(_self, BUTTON_FIVE_SECONDS);}
		else if ( pressLength >= (1 * ticksPerSecond) ) // 1 sec trap
			{ implement_Button_setPreliminaryType(_self, BUTTON_LONG_PRESS);  }
		else                                            // sub 1 second
			{ implement_Button_setPreliminaryType(_self, BUTTON_SHORT_PRESS); }

		// assign preliminaryCount
		implement_Button_setPreliminaryCount(_self,
				        ( implement_Button_getPreliminaryCount(_self) + 1 ) );

		// report 5-sec press immediately, otherwise allow time for next press
		if ( implement_Button_getPreliminaryType(_self) == BUTTON_FIVE_SECONDS)
			{ Button_setButtonState(_self, BUTTON_REPORT); }
		else
			{ Button_setButtonState(_self, BUTTON_WAITING); }

		Button_updateState(_self, _timeStamp);
		break;
	}

	case BUTTON_WAITING: {
		// do nothing
		// state is incremented by callback from scheduler
		break;
	}

	case BUTTON_REPORT: {
		// assign pressType
		// if type is unknown, then assign the preliminary type
		// if types are mismatched, then reset the button and ignore all prior
		if ( implement_Button_getPressType(_self) !=
				                 implement_Button_getPreliminaryType(_self)) {

			// types do not match
			if ( implement_Button_getPressType(_self) ==
					                              BUTTON_PRESS_TYPE_UNKNOWN) {

				// original type is unknown, therefore update type and count
				implement_Button_setPressType(_self,
						         implement_Button_getPreliminaryType(_self) );
				implement_Button_setPressCount(_self,
								implement_Button_getPreliminaryCount(_self) );

				// fire the button event ready callback
				// methods monitoring the callback chain pick off messages
				(Button_getButtonReport_cb(_self))(_self);
				break;

			}
			else {

				// types mismatch and are not unknown, therefore trigger reset
				Button_setButtonState(_self, BUTTON_RESET);
				Button_updateState(_self, _timeStamp);
				break;
			}

		}
		else { break; } // types already match, no action is needed

		}

		break;


	default: {}
	}

	return;
}

void * Button_start(void * _self)
{
	struct Button * self = cast(Button, _self);
	if ( self == NULL ) { return NULL; }  // fail
	Button_setButtonState(_self, BUTTON_RESET);
	Button_updateState(_self, BUTTON_TIME_UNKNOWN);
	return self;
}

void * Button_reset(void * _self)
{
	struct Button * self = cast(Button, _self);
	if ( self == NULL ) { return NULL; }  // fail
	self->buttonState      = BUTTON_STATE_UNKNOWN;
	self->preliminaryCount = 0;
	self->pressCount       = 0;
	self->preliminaryType  = BUTTON_PRESS_TYPE_UNKNOWN;
	self->pressType        = BUTTON_PRESS_TYPE_UNKNOWN;
	self->pressTime        = BUTTON_TIME_UNKNOWN;
	self->releaseTime      = BUTTON_TIME_UNKNOWN;
	return self;  // expected path
}

void * Button_pressCallback(void * _self, int _timeStamp)
{
	struct Button * self = cast(Button, _self);
	if ( self == NULL )   { return NULL; }  // fail
	if ( _timeStamp < 0 ) { return NULL; }  // fail

	// test for initial button press
	if ( implement_Button_getButtonState(self) == BUTTON_IDLE ) {
		Button_setButtonState(self, BUTTON_PRESSED);
		Button_updateState   (self, _timeStamp);
		return self;  // expected path
	}

	// test for second or larger button press
	if ( implement_Button_getButtonState(self) == BUTTON_WAITING ) {
		Button_setButtonState(self, BUTTON_PRESSED);
		Button_updateState   (self, _timeStamp);
		return self;  // expected path
	}

	// invalid state machine sequence
	Button_setButtonState(self, BUTTON_RESET);
	Button_updateState   (self, BUTTON_PRESS_TYPE_UNKNOWN);
	return NULL;
}

void * Button_releaseCallback(void * _self, int _timeStamp)
{
	struct Button * self = cast(Button, _self);
	if ( self == NULL )   { return NULL; }  // fail
	if ( _timeStamp < 0 ) { return NULL; }  // fail


	if ( implement_Button_getButtonState(self) == BUTTON_WAITING_RELEASE ) {
		Button_setButtonState(self, BUTTON_RELEASED);
		Button_updateState   (self, _timeStamp);
		return self;  // expected path
	}

	// invalid state machine sequence
	Button_setButtonState(self, BUTTON_IDLE);
	Button_updateState   (self, BUTTON_PRESS_TYPE_UNKNOWN);
	return NULL;  // fail
}

void * Button_reportCallback(void * _self)
{
	struct Button * self = cast(Button, _self);
	if ( self == NULL ) { return NULL; }  // fail

	// ready to advance to next report state where pressCount is finalized
	if ( implement_Button_getButtonState(self) == BUTTON_WAITING ) {
		Button_setButtonState(self, BUTTON_REPORT);
		Button_updateState   (self, BUTTON_TIME_UNKNOWN);
		return self;  // expected path
	}

	// remain in current state, take no action
	if (implement_Button_getButtonState(self) == BUTTON_REPORT ) {
		return self;  // expected path
	}

	// invalid state machine sequence
	Button_setButtonState(self, BUTTON_IDLE);
	Button_updateState   (self, BUTTON_TIME_UNKNOWN);
	return NULL;  // fail
}

buttonReportStatus_t Button_reportReady(const void * _self)
{
	struct Button * self = cast(Button, _self);
	if ( self == NULL ) { return BUTTON_REPORT_NOT_READY; }  // fail

	// test for valid/completed button event
	if ( Button_getButtonState(self) != BUTTON_REPORT )
		{ return BUTTON_REPORT_NOT_READY; }
	else
		{ return BUTTON_REPORT_IS_READY; }
}

button_cb_fnct Button_setButtonReport_cb(const void * _self, button_cb_fnct _cb)
{
	struct Button * self = cast(Button, _self);
	if ( self == NULL ) { return NULL; }  // fail
	if ( _cb  == NULL ) { return NULL; }  // fail
	self->button_OnReportReady_cb = _cb;
	return _cb;
}

button_cb_fnct Button_getButtonReport_cb(const void * _self)
{
	struct Button * self = cast(Button, _self);
	if ( self == NULL ) { return NULL; }  // fail
	return self->button_OnReportReady_cb;
}

struct Button * Button_PostOnReportReadyCallback(struct Button * _self)
{
	struct Button * self = cast(Button, _self);
	if ( self == NULL ) { return NULL; }  // fail
	//printf("Button_PostOnReportReadyCallback\n");
	return self;
}
