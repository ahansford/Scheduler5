/*
65
 * time-private.h
 *
 *  Created on: Aug 2, 2015
 *      Author: alhansfo
 */

#ifndef SRC_TIMES_TIMES_PRIVATE_H_
#define SRC_TIMES_TIMES_PRIVATE_H_

/***********************************/
/*** class and object structures ***/

struct Time {
	const struct Object _;	// should the superclass
	second_t	second;
	minute_t	minute;
	hour_t		hour;
	hourMode_t	hourMode;
	AmPm_t		AmPm;
	dayOfWeek_t	dayOfWeek;
	increment_t timeUpdateIncrement;
	struct Time * (*time_OnSecondChange_cb)(struct Time * _time);
	struct Time * (*time_OnMinuteChange_cb)(struct Time * _time);
	struct Time * (*time_OnHourChange_cb)  (struct Time * _time);
};


struct TimeClass {
	const struct Class	_;	// should be superclass: Class or "somethingClass"

	second_t    	(*setSecond)(void * _self, second_t _second);
	second_t    	(*getSecond)(void * _self);

	minute_t    	(*setMinute)(void * _self, minute_t _minute);
	minute_t    	(*getMinute)(void * _self);

	hour_t    		(*setHour)(void * _self, hour_t _hour);
	hour_t    		(*getHour)(void * _self);

	hourMode_t    	(*setHourMode)(void * _self, hourMode_t _hourMode);
	hourMode_t    	(*getHourMode)(void * _self);

	AmPm_t    		(*setAmPm)(void * _self, AmPm_t _AmPm);
	AmPm_t    		(*getAmPm)(void * _self);

	dayOfWeek_t    	(*setDayOfWeek)(void * _self, dayOfWeek_t _dayOfWeek);
	dayOfWeek_t    	(*getDayOfWeek)(void * _self);

	increment_t    	(*setTimeUpdateIncrement)(void * _self, increment_t _mode);
	increment_t    	(*getTimeUpdateIncrement)(void * _self);

	void *          (*incrementTime)(void * _self);

	timeMatch_t     (*checkTimeMatch)(const void * _self,
			                          const void * _comparisonTime);
};

/*****************************/
/****** Constructors  ********/

void * TimeClass_ctor(void * _self, va_list * app);
void * Time_ctor     (void * _self, va_list * app);

/***********************************/
/****** Overload Functions  ********/

void *      Time_dtor  (      void * _self);
void *      Time_copy  (      void * _copyToTime, const void * _copyFromTime);
equal_t     Time_equal (const void * _self, const void * _comparisonObject);
void *      Time_config(const void * _self, const void * _master);
puto_return_t Time_puto(const void * _self, FILE * _fp);


/**********************************/
/**********  new methods  *********/


second_t super_setSecond(const void * _class, void * self, second_t _second);
second_t  Time_setSecond(                     void * self, second_t _second);

second_t super_getSecond(const void * _class, void * self);
second_t  Time_getSecond(                     void * self);

minute_t super_setMinute(const void * _class, void * _self, minute_t _minute);
minute_t  Time_setMinute(                     void * _self, minute_t _minute);

minute_t super_getMinute(const void * _class, void * _self);
minute_t  Time_getMinute(                     void * _self);

hour_t super_setHour(const void * _class, void * _self, hour_t _hour);
hour_t  Time_setHour(                     void * _self, hour_t _hour);

hour_t super_getHour(const void * _class, void * _self);
hour_t  Time_getHour(                     void * _self);

dayOfWeek_t super_setDayOfWeek(const void * _class,
		                       void * _self, dayOfWeek_t _dayOfWeek);
dayOfWeek_t  Time_setDayOfWeek(void * _self, dayOfWeek_t _dayOfWeek);

dayOfWeek_t super_getDayOfWeek(const void * _class, void * _self);
dayOfWeek_t  Time_getDayOfWeek(                     void * _self);

hourMode_t super_setHourMode(const void * _class,
		                     void * _self, hourMode_t _hourMode);
hourMode_t  Time_setHourMode(void * _self, hourMode_t _hourMode);

hourMode_t super_getHourMode(const void * _class, void * _self);
hourMode_t  Time_getHourMode(                     void * _self);

AmPm_t super_setAmPm(const void * _class, void * _self, AmPm_t _AmPm);
AmPm_t  Time_setAmPm(                     void * _self, AmPm_t _AmPm);

AmPm_t super_getAmPm(const void * _class, void * _self);
AmPm_t  Time_getAmPm(                     void * _self);

increment_t super_setTimeUpdateIncrement(const void * _class,
		                                  void * _self, increment_t _mode);
increment_t  Time_setTimeUpdateIncrement (void * _self, increment_t _mode);

increment_t super_getTimeUpdateIncrement(const void * _class, void * _self);
increment_t  Time_getTimeUpdateIncrement(                     void * _self);

void * super_incrementTime(const void * _class, void * _self);
void *  Time_incrementTime(                     void * _self);


timeMatch_t super_checkTimeMatch(const void * _class, const void * _self, const void * _comparisonTime);
timeMatch_t  Time_checkTimeMatch(                     const void * _self, const void * _comparisonTime);

struct Time * Time_PostSecondCallback(struct Time * _time);
struct Time * Time_PostMinuteCallback(struct Time * _time);
struct Time * Time_PostHourCallback  (struct Time * _time);

#endif /* SRC_TIMES_TIMES_PRIVATE_H_ */
