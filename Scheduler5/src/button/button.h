/*
 * event-controller.h
 *
 *  Created on: Aug 15, 2015
 *      Author: alhansfo
 */

#ifndef SRC_BUTTON_BUTTON_H_
#define SRC_BUTTON_BUTTON_H_

/***********************************************/
/****** superclass and system includes  ********/

//#include "..\cross_compiler_defs.h"
#include "..\..\src\objects\objects.h"
//#include "..\..\src\nodes\nodes.h"     // supports class registration
//#include "..\..\src\node_list\node-list.h"
//#include "..\..\src\times\times.h"     // supports class registration
//#include "..\..\src\events\events.h"     // supports class registration

/***********************************************/
/**** forward declarations and typedefs  *******/

extern const void * Button;
extern const void * ButtonClass;

typedef enum buttonState_t {
	BUTTON_STATE_UNKNOWN = -1, BUTTON_RESET, BUTTON_IDLE, BUTTON_PRESSED,
	BUTTON_WAITING_RELEASE,	BUTTON_RELEASED, BUTTON_WAITING, BUTTON_REPORT
} buttonState_t;

typedef enum buttonPressType_t {
	BUTTON_PRESS_TYPE_UNKNOWN = -1,
	BUTTON_SHORT_PRESS, BUTTON_LONG_PRESS, BUTTON_FIVE_SECONDS
} buttonPressType_t;

#define BUTTON_TIME_UNKNOWN (-1)

typedef enum buttonReportStatus_t {
	BUTTON_REPORT_NOT_READY = 0, BUTTON_REPORT_IS_READY = 1
} buttonReportStatus_t;

typedef struct Button * (* button_cb_fnct)(struct Button *);

/***********************************************/
/************ protected includes  **************/
/***** must be after externs and typedefs  *****/

#include "button-private.h"


/***********************************************/
/****** application interface functions  *******/

// MUST be called before other methods are called
// ticks per second must be set when calling initializing
// pressType is segregated as less then 1 second, one second to
// just less than 5 seconds, and 5 seconds or longer
// returns self on success, otherwise NULL
void Button_init (int _ticksPerSecond);

// start resets the data members to their defaults and waits press event
// returns self on success, otherwise NULL
void * Button_start(void * _self);

// press and release callbacks are called by hardware drivers
// timeStamp is in tick counts
// returns self on success, otherwise NULL
void * Button_pressCallback  (void * _self, int _timeStamp);
void * Button_releaseCallback(void * _self, int _timeStamp);

// reportCallback is fired after the wait interval has expired
// the wait interval allows multiple press events to be counted/accumulated
// once the wait interval expires, no further counts are accumulated
// the hold off event should be set by the Button_releaseCallback() call
// the scheduler will call Button_reportCallback() to complete the button event
// asynchronous test of button state can be checked via Button_reportReady()
void * Button_reportCallback (void * _self);

// use Button_reportReady() to test for button press/count cycle complete
// once button report is ready, the press type and count can be read
buttonReportStatus_t Button_reportReady(const void * _self);
buttonPressType_t    Button_getPressType (const void * _self);
int                  Button_getPressCount(const void * _self);

// returns current callback function on success
// can be used to create a callback chain that fires on valid button events
button_cb_fnct  Button_setButtonReport_cb(const void * _self, button_cb_fnct _cb);
button_cb_fnct  Button_getButtonReport_cb(const void * _self);;;;;;;



#endif /* SRC_BUTTON_BUTTON_H_ */
