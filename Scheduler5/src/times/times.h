/*
 * time.h
 *
 *  Created on: Aug 2, 2015
 *      Author: alhansfo
 */

#ifndef SRC_TIME_H_
#define SRC_TIME_H_

/***********************************************/
/****** superclass and system includes  ********/

//#include "..\cross_compiler_defs.h"
#include "..\..\src\objects\objects.h"

/***********************************************/
/**** forward declarations and typedefs  *******/

extern const void * Time; // struct Button * new myButton = new(Button);
extern const void * TimeClass;

typedef enum second_t {
	SECOND_UNKNOWN = -1, SECOND_0 = 0, SECOND_60 = 60
} second_t;

typedef enum minute_t {
	MINUTE_UNKNOWN = -1, MINUTE_0 = 0, MINUTE_60 = 60
} minute_t;

typedef enum hour_t {
HOUR_UNKNOWN = -1, HOUR_0 = 0, HOUR_12 = 12, HOUR_24 = 24
} hour_t;

typedef enum hourMode_t {
	HOUR_MODE_UNKNOWN = -1, HOUR_MODE_12, HOUR_MODE_24
} hourMode_t;

typedef enum AmPmMode_t {
	AM__PM_UNKNOWN = -1, AM_TIME, PM_TIME
} AmPm_t;

typedef enum dayOfWeek_t {
	DAY_OF_WEEK_UNKNOWN = -1,
	SUNDAY = 1, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY,
	EVERYDAY = 10, WEEKDAY, WEEKEND
} dayOfWeek_t;

typedef enum increment_t {
INCREMENT_UNKNOWN = -1, UPDATE_SECONDS, UPDATE_MINUTES
} increment_t;

typedef enum timeMatch_t {
MATCH_UNKNOWN = -1, MATCH_FAIL = 0, MATCH_PARTIAL, MATCH
} timeMatch_t;

typedef struct Time * (* time_cb_fnct)(struct Time *);
/***********************************************/
/************ protected includes  **************/
/***** must be after externs and typedefs  *****/

#include "times-private.h"


/***********************************************/
/****** application interface functions  *******/

// MUST be called before any new(Time) is created
void Time_init(void);

// returns second on success, otherwise SECOND_UNKNOWN
second_t setSecond(void * self, second_t _second);
second_t getSecond(void * self);

// returns minute on success, otherwise MINUTE_UNKNOWN
minute_t setMinute(void * _self, minute_t _minute);
minute_t getMinute(void * _self);

// returns hour on success, otherwise HOUR_UNKNOWN
hour_t setHour(void * _self, hour_t _hour);
hour_t getHour(void * _self);

// returns dayOfWeek on success, otherwise DAY_OF_WEEK_UNKNOWN
// can also return EVERYDAY, WEEKEND and WEEKDAY for events
dayOfWeek_t setDayOfWeek(void * _self, dayOfWeek_t _dayOfWeek);
dayOfWeek_t getDayOfWeek(void * _self);

// returns hourMode on success, otherwise HOUR_MODE_UNKNOWN
hourMode_t setHourMode(void * _self, hourMode_t _hourMode);
hourMode_t getHourMode(void * _self);

// returns AmPm on success, otherwise AM__PM_UNKNOWN
AmPm_t setAmPm(void * _self, AmPm_t _AmPm);
AmPm_t getAmPm(void * _self);

// returns incrementMode on success, otherwise INCREMENT_UNKNOWN
increment_t setTimeUpdateIncrement(void * _self, increment_t _mode);
increment_t getTimeUpdateIncrement(void * _self);

// set the timeUpdateIncrement to UPDATE_SECONDS, or UPDATE_MINUTES
// call the incrementTime() function every minute, or second, from ISR
// NOTE:  only "time" objects that change, should be incremented
//        examples are the master time object, or specific timers
//        "event style" time objects should typically NOT be incremented
struct Time * incrementTime(struct Time * _self);

// resets time to 12:00:00AM SUNDAY with timeUpdateIncrement = UPDATE_SECONDS
// if supported, date objects should be reset to JAN 1, 2000 within their code
void * resetTime(void * _self);

/* Typical time object use:
 * struct Time * masterTime = new(Time);
 * resetTime(masterTime);
 * set{Data Member}(myTimeEvent, desiredValue); // set actual time as needed
 * forever loop calls "incrementTime(masterTime);" once every second
 *
 * Typical event time object use:
 * struct Time * myTimeEvent = new(Time);
 * set{Data Member}(myTimeEvent, desiredValue);
 * call "get{Data Member}(myTimeEvent);" as needed to access data
 */

// returns MATCH
timeMatch_t checkTimeMatch(const void * _self, const void * _comparisonTime);

// returns current callback function on success
// can be used to create a callback chain that fires on changing seconds
time_cb_fnct setTime_OnSecondChange_cb(const void * _self, time_cb_fnct _cb);
time_cb_fnct getTime_OnSecondChange_cb(const void * _self);

// returns current callback function on success
// can be used to create a callback chain that fires on changing minutes
time_cb_fnct setTime_OnMinuteChange_cb(const void * _self, time_cb_fnct _cb);
time_cb_fnct getTime_OnMinuteChange_cb(const void * _self);

// returns current callback function on success
// can be used to create a callback chain that fires on changing hours
time_cb_fnct setTime_OnHourChange_cb(const void * _self, time_cb_fnct _cb);
time_cb_fnct getTime_OnHourChange_cb(const void * _self);


#endif /* SRC_TIME_H_ */
