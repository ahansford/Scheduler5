/*
 * nodes-test.c
 *
 *  Created on: July, 2015
 *      Author: Alan
 */


#include "..\..\src\cross_compiler_defs.h"
#include "..\unity\unity_fixture.h"
//#include "..\unity\unity.h"
#include "..\..\src\mocks\RuntimeErrorStub.h"

#include "..\..\src\times\times.h"
//#include "..\..\src\times\times-private.h"

/****************************************/
/*** Basic Object and Class Testing ****/
struct       Time *      myTest_Time;
const struct TimeClass * myTest_TimeClass_PTR;
const struct Class *     myTest_Time_class_PTR;

static void times_test_setTime(struct Time * _self,
					second_t      seconds,
		            minute_t      minute,
					hour_t        hour,
					hourMode_t	  hourMode,
					AmPm_t        AmPm,
					dayOfWeek_t   dayOfWeek
					);

struct Time * times_test_general_cb(struct Time * _self);
int times_test_cb_count = 0;

/****************************************/
/*********** Start of Testing ***********/
TEST_GROUP(times);

TEST_SETUP(times)
{
	RuntimeErrorStub_Reset();
	Time_init();
	myTest_Time  = new(Time);
	myTest_TimeClass_PTR  = classOf(myTest_Time);
	myTest_Time_class_PTR = Time;
}

TEST_TEAR_DOWN(times)
{
	myTest_Time = safeDelete(myTest_Time);
	RuntimeErrorStub_Reset();
}

/**************************************/
/**************  New  *****************/

TEST(times, myTestTime_IsNotNullOnCreate)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  myTest_Time);
}

TEST(times, Time_second_isUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(SECOND_UNKNOWN,  myTest_Time->second);
}

TEST(times, Time_minute_isUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(MINUTE_UNKNOWN,  myTest_Time->minute);
}

TEST(times, Time_hour_isUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(HOUR_UNKNOWN,  myTest_Time->hour);
}

TEST(times, Time_dayOfWeek_isUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(DAY_OF_WEEK_UNKNOWN,  myTest_Time->dayOfWeek);
}

TEST(times, Time_hourMode_isUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(HOUR_MODE_UNKNOWN,  myTest_Time->hourMode);
}

TEST(times, Time_AmPm_isUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(AM__PM_UNKNOWN,  myTest_Time->AmPm);
}


/**************************************/
/*************  Time  *****************/

TEST(times, init_Time_ClassReturnsNonNull)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  myTest_Time->_.class);
}

TEST(times, init_classOf_myTest_Time_is_Time)
{
	TEST_ASSERT_EQUAL_PTR(Time,  myTest_Time->_.class);
}

TEST(times, init_classOf_Time_is_TimeClass)
{
	TEST_ASSERT_EQUAL_PTR(TimeClass,  myTest_Time->_.class->_.class);
}

TEST(times, init_ctor_pointsTo_TimeClass_ctor)
{
	TEST_ASSERT_EQUAL_PTR(Time_ctor,  myTest_Time->_.class->ctor);
	TEST_ASSERT_EQUAL_PTR(TimeClass_ctor,  myTest_Time->_.class->_.class->ctor);
}

TEST(times, init_ctor_pointsTo_Time_ctor)
{
	TEST_ASSERT_EQUAL_PTR(Time_ctor,  myTest_Time->_.class->ctor);
}

TEST(times, init_dtor_pointsTo_Time_dtor)
{
	TEST_ASSERT_EQUAL_PTR(Time_dtor,  ((struct Class *)myTest_TimeClass_PTR)->dtor );
	TEST_ASSERT_EQUAL_PTR(Time_dtor,  myTest_Time->_.class->dtor);
}

TEST(times, init_copy_pointsTo_Time_copy)
{
	TEST_ASSERT_EQUAL_PTR(Time_copy,  myTest_Time->_.class->copy);
}

TEST(times, init_equal_pointsTo_Time_equal)
{
	TEST_ASSERT_EQUAL_PTR(Time_equal,  myTest_Time->_.class->equal);
}

TEST(times, init_config_pointsTo_Time_config)
{
	TEST_ASSERT_EQUAL_PTR(Time_config,  myTest_Time->_.class->config);
}

TEST(times, init_TimeClass_NameIs_TimeClass)
{
	TEST_ASSERT_EQUAL_STRING("TimeClass",  myTest_Time->_.class->_.class->className);
}

/**************************************/
/*******  TimeClass Methods  *********/

TEST(times, init_setSecond_pointsTo_Time_setSecond)
{
	TEST_ASSERT_EQUAL_PTR(Time_setSecond,  myTest_TimeClass_PTR->setSecond );
}

TEST(times, init_getSecond_pointsTo_Time_getSecond)
{
	TEST_ASSERT_EQUAL_PTR(Time_getSecond,  myTest_TimeClass_PTR->getSecond);
}

TEST(times, init_setMinute_pointsTo_Time_setMinute)
{
	TEST_ASSERT_EQUAL_PTR(Time_setMinute,  myTest_TimeClass_PTR->setMinute );
}

TEST(times, init_getMinute_pointsTo_Time_getMinute)
{
	TEST_ASSERT_EQUAL_PTR(Time_getMinute,  myTest_TimeClass_PTR->getMinute);
}

TEST(times, init_setHour_pointsTo_Time_setHour)
{
	TEST_ASSERT_EQUAL_PTR(Time_setHour,  myTest_TimeClass_PTR->setHour );
}

TEST(times, init_getHour_pointsTo_Time_getHour)
{
	TEST_ASSERT_EQUAL_PTR(Time_getHour,  myTest_TimeClass_PTR->getHour);
}

TEST(times, init_setDayOfWeek_pointsTo_Time_setDayOfWeek)
{
	TEST_ASSERT_EQUAL_PTR(Time_setDayOfWeek,  myTest_TimeClass_PTR->setDayOfWeek );
}

TEST(times, init_getDayOfWeek_pointsTo_Time_getDayOfWeek)
{
	TEST_ASSERT_EQUAL_PTR(Time_getDayOfWeek,  myTest_TimeClass_PTR->getDayOfWeek);
}

TEST(times, init_setHourMode_pointsTo_Time_setHourMode)
{
	TEST_ASSERT_EQUAL_PTR(Time_setHourMode,  myTest_TimeClass_PTR->setHourMode );
}

TEST(times, init_getHourMode_pointsTo_Time_getHourMode)
{
	TEST_ASSERT_EQUAL_PTR(Time_getHourMode,  myTest_TimeClass_PTR->getHourMode);
}

TEST(times, init_setAmPm_pointsTo_Time_setAmPm)
{
	TEST_ASSERT_EQUAL_PTR(Time_setAmPm,  myTest_TimeClass_PTR->setAmPm );
}

TEST(times, init_getAmPm_pointsTo_Time_getAmPm)
{
	TEST_ASSERT_EQUAL_PTR(Time_getAmPm,  myTest_TimeClass_PTR->getAmPm);
}

TEST(times, init_setTimeUpdateIncrement_pointsTo_Time_setTimeUpdateIncrement)
{
	TEST_ASSERT_EQUAL_PTR(Time_setTimeUpdateIncrement,  myTest_TimeClass_PTR->setTimeUpdateIncrement );
}

TEST(times, init_getTimeUpdateIncrement_pointsTo_Time_getTimeUpdateIncrement)
{
	TEST_ASSERT_EQUAL_PTR(Time_getTimeUpdateIncrement,  myTest_TimeClass_PTR->getTimeUpdateIncrement);
}

TEST(times, init_incrementTime_pointsTo_Time_incrementTime)
{
	TEST_ASSERT_EQUAL_PTR(Time_incrementTime,  myTest_TimeClass_PTR->incrementTime);
}

TEST(times, init_checkTimeMatch_pointsTo_Time_checkTimeMatch)
{
	TEST_ASSERT_EQUAL_PTR(Time_checkTimeMatch,  myTest_TimeClass_PTR->checkTimeMatch);
}


//****  delete/Time_dtor  ****************

TEST(times, delete_returns_SelfOnSuccess)
{
	TEST_ASSERT_EQUAL_PTR(myTest_Time,  delete(myTest_Time));
	myTest_Time = NULL;
}

TEST(times, delete_safeDelete_returnsNullOnSuccess)
{
	TEST_ASSERT_EQUAL_PTR(NULL,  safeDelete(myTest_Time));
	myTest_Time = NULL;
}

//****  Set and Get Second  ****************
/**/

TEST(times, getSecond_returns_returnsUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(SECOND_UNKNOWN,  getSecond(myTest_Time));
}

TEST(times, getSecond_returns_specificValue)
{
	myTest_Time->second = 4;
	TEST_ASSERT_EQUAL(4,  getSecond(myTest_Time));
}

TEST(times, setSecond_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(myTest_Time->second,  setSecond(myTest_Time, 5));
}

TEST(times, setSecond_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(SECOND_UNKNOWN,  setSecond(NULL, 5));
}

TEST(times, setSecond_returnsSpecificValueOnSuccess)
{
	TEST_ASSERT_EQUAL(5,  setSecond(myTest_Time, 5));
}

TEST(times, setSecond_canSetSpecificValue)
{
	setSecond(myTest_Time, 6);
	TEST_ASSERT_EQUAL(6,  myTest_Time->second);
}


//****  Set and Get Minute  ****************

TEST(times, getMinute_returns_returnsUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(MINUTE_UNKNOWN,  getMinute(myTest_Time));
}

TEST(times, getMinute_returns_specificValue)
{
	myTest_Time->minute = 4;
	TEST_ASSERT_EQUAL(4,  getMinute(myTest_Time));
}

TEST(times, setMinute_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(myTest_Time->minute,  setMinute(myTest_Time, 5));
}

TEST(times, setMinute_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(MINUTE_UNKNOWN,  setMinute(NULL, 5));
}

TEST(times, setMinute_returnsSpecificValueOnSuccess)
{
	TEST_ASSERT_EQUAL(5,  setMinute(myTest_Time, 5));
}

TEST(times, setMinute_canSetSpecificValue)
{
	setMinute(myTest_Time, 6);
	TEST_ASSERT_EQUAL(6,  myTest_Time->minute);
}


//****  Set/Get hour  ****************

TEST(times, getHour_returns_returnsUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(HOUR_UNKNOWN,  getHour(myTest_Time));
}

TEST(times, getHour_returns_specificValue)
{
	myTest_Time->hour = 4;
	TEST_ASSERT_EQUAL(4,  getHour(myTest_Time));
}

TEST(times, setHour_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(myTest_Time->hour,  setHour(myTest_Time, 5));
}

TEST(times, setHour_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(HOUR_UNKNOWN,  setHour(NULL, 5));
}

TEST(times, setHour_returnsSpecificValueOnSuccess)
{
	TEST_ASSERT_EQUAL(5,  setHour(myTest_Time, 5));
}

TEST(times, setHour_canSetSpecificValue)
{
	setHour(myTest_Time, 6);
	TEST_ASSERT_EQUAL(6,  myTest_Time->hour);
}

//****  Set/Get dayOfWeek  ****************
/**/

TEST(times, getDayOfWeek_returns_returnsUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(DAY_OF_WEEK_UNKNOWN,  getDayOfWeek(myTest_Time));
}

TEST(times, getDayOfWeek_returns_specificValue)
{
	myTest_Time->dayOfWeek = 4;
	TEST_ASSERT_EQUAL(4,  getDayOfWeek(myTest_Time));
}

TEST(times, setDayOfWeek_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(myTest_Time->dayOfWeek,  setDayOfWeek(myTest_Time, 5));
}

TEST(times, setDayOfWeek_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(DAY_OF_WEEK_UNKNOWN,  setDayOfWeek(NULL, 5));
}

TEST(times, setDayOfWeek_returnsSpecificValueOnSuccess)
{
	TEST_ASSERT_EQUAL(5,  setDayOfWeek(myTest_Time, 5));
}

TEST(times, setDayOfWeek_canSetSpecificValue)
{
	setDayOfWeek(myTest_Time, 6);
	TEST_ASSERT_EQUAL(6,  myTest_Time->dayOfWeek);
}

//****  Set/Get hourMode  ****************
/**/

TEST(times, getHourMode_returns_returnsUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(HOUR_MODE_UNKNOWN,  getHourMode(myTest_Time));
}

TEST(times, getHourMode_returns_specificValue)
{
	myTest_Time->hourMode = HOUR_MODE_12;
	TEST_ASSERT_EQUAL(HOUR_MODE_12,  getHourMode(myTest_Time));
}

TEST(times, setHourMode_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(myTest_Time->hourMode,  setHourMode(myTest_Time, HOUR_MODE_12));
}

TEST(times, setHourMode_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(HOUR_MODE_UNKNOWN,  setHourMode(NULL, HOUR_MODE_24));
}

TEST(times, setHourMode_returnsSpecificValueOnSuccess)
{
	TEST_ASSERT_EQUAL(HOUR_MODE_24,  setHourMode(myTest_Time, HOUR_MODE_24));
}

TEST(times, setHourMode_canSetSpecificValue)
{
	setHourMode(myTest_Time, HOUR_MODE_24);
	TEST_ASSERT_EQUAL(HOUR_MODE_24,  myTest_Time->hourMode);
}

//+++++++++++++++ convert hour mode times

TEST(times, setHourMode_TwelveAMHourNoChangeAlready12H_12H)
{
	times_test_setTime(myTest_Time, SECOND_0, MINUTE_0, HOUR_12, HOUR_MODE_12, AM_TIME, SUNDAY);
	setHourMode(myTest_Time, HOUR_MODE_12);
	TEST_ASSERT_EQUAL(HOUR_12,  myTest_Time->hour);
}

TEST(times, setHourMode_ZeroToTwelveAM_withChangeTo12H)
{
	times_test_setTime(myTest_Time, SECOND_0, MINUTE_0, HOUR_0, HOUR_MODE_24, AM_TIME, SUNDAY);
	setHourMode(myTest_Time, HOUR_MODE_12);
	TEST_ASSERT_EQUAL(HOUR_12,  myTest_Time->hour);
}

TEST(times, setHourMode_PmSetAfterNoon_withChangeTo12H)
{
	times_test_setTime(myTest_Time, SECOND_0, MINUTE_0, HOUR_12, HOUR_MODE_24, AM__PM_UNKNOWN, SUNDAY);
	setHourMode(myTest_Time, HOUR_MODE_12);
	TEST_ASSERT_EQUAL(PM_TIME,  myTest_Time->AmPm);
}

TEST(times, setHourMode_AmSetBeforeNoon_withChangeTo12H)
{
	times_test_setTime(myTest_Time, SECOND_0, MINUTE_0, (HOUR_12 -1), HOUR_MODE_24, AM__PM_UNKNOWN, SUNDAY);
	setHourMode(myTest_Time, HOUR_MODE_12);
	TEST_ASSERT_EQUAL(AM_TIME,  myTest_Time->AmPm);
}

TEST(times, setHourMode_ZeroHourNoChangeAlready24H_24H)
{
	times_test_setTime(myTest_Time, SECOND_0, MINUTE_0, HOUR_0, HOUR_MODE_24, AM__PM_UNKNOWN, SUNDAY);
	setHourMode(myTest_Time, HOUR_MODE_24);
	TEST_ASSERT_EQUAL(HOUR_0,  myTest_Time->hour);
}

TEST(times, setHourMode_PmSetAt12_withChangeTo24H)
{
	times_test_setTime(myTest_Time, SECOND_0, MINUTE_0, HOUR_12, HOUR_MODE_12, PM_TIME, SUNDAY);
	setHourMode(myTest_Time, HOUR_MODE_24);
	TEST_ASSERT_EQUAL(PM_TIME,  myTest_Time->AmPm);
}

TEST(times, setHourMode_AmSetBefore12_withChangeTo24H)
{
	times_test_setTime(myTest_Time, SECOND_0, MINUTE_0, HOUR_12, HOUR_MODE_12, PM_TIME, SUNDAY);
	setHourMode(myTest_Time, HOUR_MODE_24);
	TEST_ASSERT_EQUAL(PM_TIME,  myTest_Time->AmPm);
}

TEST(times, setHourMode_TwelveAmToZero_withChangeTo24H)
{
	times_test_setTime(myTest_Time, SECOND_0, MINUTE_0, HOUR_12, HOUR_MODE_12, AM_TIME, SUNDAY);
	setHourMode(myTest_Time, HOUR_MODE_24);
	TEST_ASSERT_EQUAL(HOUR_0,  myTest_Time->hour);
}

TEST(times, setHourMode_Add12ForPm_withChangeTo24H)
{
	times_test_setTime(myTest_Time, SECOND_0, MINUTE_0, 5, HOUR_MODE_12, PM_TIME, SUNDAY);
	setHourMode(myTest_Time, HOUR_MODE_24);
	TEST_ASSERT_EQUAL( (HOUR_12 + 5) ,  myTest_Time->hour);
}


//****  Set/Get AmPm  ****************


TEST(times, getAmPm_returns_returnsUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(AM__PM_UNKNOWN,  getAmPm(myTest_Time));
}

TEST(times, getAmPm_returns_specificValue)
{
	myTest_Time->AmPm = AM_TIME;
	TEST_ASSERT_EQUAL(AM_TIME,  getAmPm(myTest_Time));
}

TEST(times, setAmPm_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(myTest_Time->AmPm,  setAmPm(myTest_Time, PM_TIME));
}

TEST(times, setAmPm_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(AM__PM_UNKNOWN,  setAmPm(NULL, PM_TIME));
}

TEST(times, setAmPm_returnsSpecificValueOnSuccess)
{
	TEST_ASSERT_EQUAL(PM_TIME,  setAmPm(myTest_Time, PM_TIME));
}

TEST(times, setAmPm_canSetSpecificValue)
{
	setAmPm(myTest_Time, AM_TIME);
	TEST_ASSERT_EQUAL(AM_TIME,  myTest_Time->AmPm);
}

//****  Set/Get AmPm  ****************

TEST(times, getTimeUpdateIncrement_returns_returnsUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(INCREMENT_UNKNOWN,  getTimeUpdateIncrement(myTest_Time));
}

TEST(times, getTimeUpdateIncrement_returns_specificValue)
{
	myTest_Time->timeUpdateIncrement = UPDATE_SECONDS;
	TEST_ASSERT_EQUAL(UPDATE_SECONDS,  getTimeUpdateIncrement(myTest_Time));
}

TEST(times, setTimeUpdateIncrement_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(myTest_Time->timeUpdateIncrement,
			        setTimeUpdateIncrement(myTest_Time, UPDATE_SECONDS));
}

TEST(times, setTimeUpdateIncrement_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(INCREMENT_UNKNOWN,
			        setTimeUpdateIncrement(NULL, UPDATE_SECONDS));
}

TEST(times, setTimeUpdateIncrement_returnsSpecificValueOnSuccess)
{
	TEST_ASSERT_EQUAL(UPDATE_SECONDS,
			        setTimeUpdateIncrement(myTest_Time, UPDATE_SECONDS));
}

TEST(times, setTimeUpdateIncrement_canSetSpecificValue)
{
	setTimeUpdateIncrement(myTest_Time, UPDATE_SECONDS);
	TEST_ASSERT_EQUAL(UPDATE_SECONDS,  myTest_Time->timeUpdateIncrement);
}

//****  incrementTime  ****************

TEST(times, setTime_correctlyUpdatesValues)
{
	times_test_setTime(myTest_Time, SECOND_0, MINUTE_0, HOUR_0, HOUR_MODE_12, AM_TIME, SUNDAY);
	TEST_ASSERT_EQUAL(SECOND_0,     getSecond(myTest_Time));
	TEST_ASSERT_EQUAL(MINUTE_0,     getMinute(myTest_Time));
	TEST_ASSERT_EQUAL(HOUR_0,       getHour(myTest_Time));
	TEST_ASSERT_EQUAL(HOUR_MODE_12, getHourMode(myTest_Time));
	TEST_ASSERT_EQUAL(AM_TIME,      getAmPm(myTest_Time));
	TEST_ASSERT_EQUAL(SUNDAY,       getDayOfWeek(myTest_Time));
}

TEST(times, incrementTime_Seconds_ToZero_OnFirstCall)
{
	setTimeUpdateIncrement(myTest_Time, UPDATE_SECONDS);
	incrementTime(myTest_Time);
	TEST_ASSERT_EQUAL(SECOND_0, getSecond(myTest_Time));
}

TEST(times, incrementTime_Seconds_ToZero_From59)
{
	setTimeUpdateIncrement(myTest_Time, UPDATE_SECONDS);
	times_test_setTime(myTest_Time, 59, MINUTE_0, HOUR_0, HOUR_MODE_12, AM_TIME, SUNDAY);
	incrementTime(myTest_Time);
	TEST_ASSERT_EQUAL(SECOND_0, getSecond(myTest_Time));
}

TEST(times, incrementTime_Hour_Increments_From5959)
{
	setTimeUpdateIncrement(myTest_Time, UPDATE_SECONDS);
	times_test_setTime(myTest_Time, 59, 59, HOUR_0, HOUR_MODE_12, AM_TIME, SUNDAY);
	incrementTime(myTest_Time);
	TEST_ASSERT_EQUAL((HOUR_0 + 1), getHour(myTest_Time));
}

TEST(times, incrementTime_AmPm_toPM_From115959AM)
{
	setTimeUpdateIncrement(myTest_Time, UPDATE_SECONDS);
	times_test_setTime(myTest_Time, 59, 59, (HOUR_12 - 1), HOUR_MODE_12, AM_TIME, SUNDAY);
	incrementTime(myTest_Time);
	TEST_ASSERT_EQUAL(PM_TIME, getAmPm(myTest_Time));
}

TEST(times, incrementTime_AmPm_toAM_From115959PM)
{
	setTimeUpdateIncrement(myTest_Time, UPDATE_SECONDS);
	times_test_setTime(myTest_Time, 59, 59, (HOUR_12 - 1), HOUR_MODE_12, PM_TIME, SUNDAY);
	incrementTime(myTest_Time);
	TEST_ASSERT_EQUAL(AM_TIME, getAmPm(myTest_Time));
}

TEST(times, incrementTime_DayIncrements_From125959PM)
{
	setTimeUpdateIncrement(myTest_Time, UPDATE_SECONDS);
	times_test_setTime(myTest_Time, 59, 59, (HOUR_12 - 1), HOUR_MODE_12, PM_TIME, SUNDAY);
	incrementTime(myTest_Time);
	TEST_ASSERT_EQUAL((SUNDAY + 1), getDayOfWeek(myTest_Time));
}

TEST(times, incrementTime_Day_toSunday_fromSaturday)
{
	setTimeUpdateIncrement(myTest_Time, UPDATE_SECONDS);
	times_test_setTime(myTest_Time, 59, 59, (HOUR_12 - 1), HOUR_MODE_12, PM_TIME, SATURDAY);
	incrementTime(myTest_Time);
	TEST_ASSERT_EQUAL(SUNDAY, getDayOfWeek(myTest_Time));
}

TEST(times, incrementTime_is10secondsAfter10increments)
{
	int i;
	resetTime(myTest_Time);
	setTimeUpdateIncrement(myTest_Time, UPDATE_SECONDS);
	for ( i = 0; i < 10; i++) { incrementTime(myTest_Time); }
	TEST_ASSERT_EQUAL(10, getSecond(myTest_Time));
}

TEST(times, incrementTime_is10minutesAfter10increments)
{
	int i;
	resetTime(myTest_Time);
	setTimeUpdateIncrement(myTest_Time, UPDATE_MINUTES);
	for ( i = 0; i < 10; i++) { incrementTime(myTest_Time); }
	TEST_ASSERT_EQUAL(10, getMinute(myTest_Time));
}

TEST(times, incrementTime_is2DaysAfter2x24x60increments)
{
	int i;
	resetTime(myTest_Time);
	setTimeUpdateIncrement(myTest_Time, UPDATE_MINUTES);
	for ( i = 0; i < (48*60); i++) { incrementTime(myTest_Time); }
	TEST_ASSERT_EQUAL(SUNDAY+2, getDayOfWeek(myTest_Time));
}


//++++++++++

TEST(times, incrementTime_IncrementsMinutesToZeroOnFirstCall)
{
	setTimeUpdateIncrement(myTest_Time, UPDATE_MINUTES);
	incrementTime(myTest_Time);
	TEST_ASSERT_EQUAL(MINUTE_0, getMinute(myTest_Time));
}

TEST(times, incrementTime_IncrementsMinutesToZeroFrom59)
{
	setTimeUpdateIncrement(myTest_Time, UPDATE_MINUTES);
	times_test_setTime(myTest_Time, SECOND_0, 59, HOUR_0+1, HOUR_MODE_12, AM_TIME, SUNDAY);
	incrementTime(myTest_Time);
	TEST_ASSERT_EQUAL(MINUTE_0, getMinute(myTest_Time));
}

//++++++++++

TEST(times, incrementTime_Hour_to_1_from115959)
{
	setTimeUpdateIncrement(myTest_Time, UPDATE_MINUTES);
	times_test_setTime(myTest_Time, 59, 59, HOUR_12, HOUR_MODE_12, AM_TIME, SUNDAY);
	incrementTime(myTest_Time);
	TEST_ASSERT_EQUAL((HOUR_0 + 1), getHour(myTest_Time));
}

//++++++++++

TEST(times, incrementTime_DayIncrements_From235959_24H)
{
	setTimeUpdateIncrement(myTest_Time, UPDATE_SECONDS);
	times_test_setTime(myTest_Time, 59, 59, (HOUR_24 - 1), HOUR_MODE_24, AM__PM_UNKNOWN, SUNDAY);
	incrementTime(myTest_Time);
	TEST_ASSERT_EQUAL((SUNDAY + 1), getDayOfWeek(myTest_Time));
}

TEST(times, incrementTime_Hour_to_0_From235959_24H)
{
	setTimeUpdateIncrement(myTest_Time, UPDATE_SECONDS);
	times_test_setTime(myTest_Time, 59, 59, (HOUR_24 - 1), HOUR_MODE_24, AM__PM_UNKNOWN, SUNDAY);
	incrementTime(myTest_Time);
	TEST_ASSERT_EQUAL(HOUR_0, getHour(myTest_Time));
}

TEST(times, incrementTime_Hour_to_13_From125959_24H)
{
	setTimeUpdateIncrement(myTest_Time, UPDATE_SECONDS);
	times_test_setTime(myTest_Time, 59, 59, HOUR_12, HOUR_MODE_24, AM__PM_UNKNOWN, SUNDAY);
	incrementTime(myTest_Time);
	TEST_ASSERT_EQUAL(13, getHour(myTest_Time));
}

TEST(times, incrementTime_Day_toSunday_fromSaturday_24H)
{
	setTimeUpdateIncrement(myTest_Time, UPDATE_SECONDS);
	times_test_setTime(myTest_Time, 59, 59, 23, HOUR_MODE_24, AM__PM_UNKNOWN, SATURDAY);
	incrementTime(myTest_Time);
	TEST_ASSERT_EQUAL(SUNDAY, getDayOfWeek(myTest_Time));
}

TEST(times, incrementTime_Day_toSunday_fromSaturdayWithAmSet_24H)
{
	setTimeUpdateIncrement(myTest_Time, UPDATE_SECONDS);
	times_test_setTime(myTest_Time, 59, 59, 23, HOUR_MODE_24, AM_TIME, SATURDAY);
	incrementTime(myTest_Time);
	TEST_ASSERT_EQUAL(SUNDAY, getDayOfWeek(myTest_Time));
}

TEST(times, incrementTime_resetTime_120000AM_SUNDAY)
{
	resetTime(myTest_Time);
	TEST_ASSERT_EQUAL(SECOND_0,     getSecond(myTest_Time));
	TEST_ASSERT_EQUAL(MINUTE_0,     getMinute(myTest_Time));
	TEST_ASSERT_EQUAL(HOUR_12,       getHour(myTest_Time));
	TEST_ASSERT_EQUAL(HOUR_MODE_12, getHourMode(myTest_Time));
	TEST_ASSERT_EQUAL(AM_TIME,      getAmPm(myTest_Time));
	TEST_ASSERT_EQUAL(SUNDAY,       getDayOfWeek(myTest_Time));
}

//****  copy/Time_copy  ****************

TEST(times, copy_returnsSelfOnSuccess)
{
	struct Time * masterTime = new(Time);
	setSecond   (masterTime, 1);
	TEST_ASSERT_EQUAL_PTR(myTest_Time, copy(myTest_Time, masterTime));
	masterTime = safeDelete(masterTime);
}

TEST(times, copy_AllItemsCopiedToSelf)
{
	struct Time * masterTime = new(Time);
	setSecond   (masterTime, 1);
	setMinute   (masterTime, 2);
	setHour     (masterTime, 3);
	setDayOfWeek(masterTime, 4);
	setHourMode (masterTime, HOUR_MODE_24);
	setAmPm     (masterTime, AM_TIME);
	copy(myTest_Time, masterTime);
	TEST_ASSERT_EQUAL(1,             myTest_Time->second);
	TEST_ASSERT_EQUAL(2,             myTest_Time->minute);
	TEST_ASSERT_EQUAL(3,             myTest_Time->hour);
	TEST_ASSERT_EQUAL(4,             myTest_Time->dayOfWeek);
	TEST_ASSERT_EQUAL(HOUR_MODE_24,  myTest_Time->hourMode);
	TEST_ASSERT_EQUAL(AM_TIME,       myTest_Time->AmPm);
	masterTime = safeDelete(masterTime);
}

TEST(times, copy_returnsNullOnNullSelf)
{
	struct Time * masterTime = new(Time);
	setSecond (masterTime, 1);
	TEST_ASSERT_EQUAL_PTR(NULL, copy(NULL, masterTime)  );
	masterTime = safeDelete(masterTime);
}

TEST(times, copy_returnsNullOnNullMaster)
{
	struct Time * masterTime = new(Time);
	setSecond (masterTime, 1);
	TEST_ASSERT_EQUAL_PTR(NULL, copy(myTest_Time, NULL) );
	masterTime = safeDelete(masterTime);
}

//****  equal/Time_equal  ********************


TEST(times, myTest_Time_IsEqualTo_myTest_Time)
{
	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_Time, myTest_Time));
}

TEST(times, equal_UnequalSecondsReturn_Unequal)
{
	struct Time * masterTime = new(Time);
	setSecond (masterTime, 1);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Time, masterTime) );
	masterTime = safeDelete(masterTime);
}

TEST(times, equal_UnequalMinutesReturn_Unequal)
{
	struct Time * masterTime = new(Time);
	setMinute(masterTime, 1);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Time, masterTime) );
	masterTime = safeDelete(masterTime);
}

TEST(times, equal_UnequalHoursReturn_Unequal)
{
	struct Time * masterTime = new(Time);
	setHour(masterTime, 1);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Time, masterTime) );
	masterTime = safeDelete(masterTime);
}

TEST(times, equal_UnequalDayOfWeekReturn_Unequal)
{
	struct Time * masterTime = new(Time);
	setDayOfWeek(masterTime, 1);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Time, masterTime) );
	masterTime = safeDelete(masterTime);
}

TEST(times, equal_UnequalHourModeReturn_Unequal)
{
	struct Time * masterTime = new(Time);
	setHourMode(masterTime, HOUR_MODE_12);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Time, masterTime) );
	masterTime = safeDelete(masterTime);
}

TEST(times, equal_UnequalAmPmReturn_Unequal)
{
	struct Time * masterTime = new(Time);
	setAmPm(masterTime, PM_TIME);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Time, masterTime) );
	masterTime = safeDelete(masterTime);
}

TEST(times, equal_NullReturns_Null)
{
	struct Time * masterTime = new(Time);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Time, NULL));
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(NULL, masterTime ));
	masterTime = safeDelete(masterTime);
}

TEST(times, equal_CopiedTimeReturnsEqual)
{
	struct Time * masterTime = new(Time);
	setSecond   (masterTime, 1);
	setMinute   (masterTime, 2);
	setHour     (masterTime, 3);
	setDayOfWeek(masterTime, 4);
	setHourMode (masterTime, HOUR_MODE_24);
	setAmPm     (masterTime, AM_TIME);
	copy(myTest_Time, masterTime);
	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_Time, masterTime));
	masterTime = safeDelete(masterTime);
}


//****  checkTimeMatch  ********************

TEST(times, match_self_matches_self)
{
	setMinute(myTest_Time, 2);
	TEST_ASSERT_EQUAL(MATCH, checkTimeMatch(myTest_Time, myTest_Time));
}

TEST(times, equal_CopiedTimesMatch)
{
	struct Time * masterTime = new(Time);
	setSecond   (masterTime, 1);
	setMinute   (masterTime, 2);
	setHour     (masterTime, 3);
	setDayOfWeek(masterTime, 4);
	setHourMode (masterTime, HOUR_MODE_24);
	setAmPm     (masterTime, AM_TIME);
	copy(myTest_Time, masterTime);
	TEST_ASSERT_EQUAL(MATCH, checkTimeMatch(myTest_Time, masterTime));
	masterTime = safeDelete(masterTime);
}

TEST(times, match_Defaults_DoNotMatch)
{
	TEST_ASSERT_EQUAL(MATCH_UNKNOWN, checkTimeMatch(myTest_Time, myTest_Time));
}

TEST(times, match_HourMatch_returnsMatch)
{
	struct Time * masterTime = new(Time);
	setHour     (masterTime, 3);

	setSecond   (myTest_Time, 0);  // seconds must be unknown or 00 for match
	setMinute   (myTest_Time, 0);  // minutes must be unknown or 00 for match
	setHour     (myTest_Time, 3);
	setDayOfWeek(myTest_Time, 4);
	setHourMode (myTest_Time, HOUR_MODE_24);
	setAmPm     (myTest_Time, AM_TIME);
	TEST_ASSERT_EQUAL(MATCH, checkTimeMatch(myTest_Time, masterTime));
	masterTime = safeDelete(masterTime);
}

TEST(times, match_UnknownMaster_returnsUnknown)
{
	struct Time * masterTime = new(Time);
	setSecond   (myTest_Time, 1);
	setMinute   (myTest_Time, 2);
	setHour     (myTest_Time, 3);
	setDayOfWeek(myTest_Time, 4);
	setHourMode (myTest_Time, HOUR_MODE_24);
	setAmPm     (myTest_Time, AM_TIME);
	TEST_ASSERT_EQUAL(MATCH_UNKNOWN, checkTimeMatch(myTest_Time, masterTime));
	masterTime = safeDelete(masterTime);
}

TEST(times, match_Monday_matches_Monday)
{
	struct Time * masterTime = new(Time);
	setDayOfWeek(masterTime, MONDAY);
	setDayOfWeek(myTest_Time, MONDAY);
	TEST_ASSERT_EQUAL(MATCH, checkTimeMatch(myTest_Time, masterTime));
	masterTime = safeDelete(masterTime);
}

TEST(times, match_Monday_matches_Weekday)
{
	struct Time * masterTime = new(Time);
	setDayOfWeek(masterTime, WEEKDAY);
	setDayOfWeek(myTest_Time, MONDAY);
	TEST_ASSERT_EQUAL(MATCH, checkTimeMatch(myTest_Time, masterTime));
	masterTime = safeDelete(masterTime);
}

TEST(times, match_Tuesday_matches_Weekday)
{
	struct Time * masterTime = new(Time);
	setDayOfWeek(masterTime, WEEKDAY);
	setDayOfWeek(myTest_Time, TUESDAY);
	TEST_ASSERT_EQUAL(MATCH, checkTimeMatch(myTest_Time, masterTime));
	masterTime = safeDelete(masterTime);
}

TEST(times, match_Wednesday_matches_Weekday)
{
	struct Time * masterTime = new(Time);
	setDayOfWeek(masterTime, WEEKDAY);
	setDayOfWeek(myTest_Time, WEDNESDAY);
	TEST_ASSERT_EQUAL(MATCH, checkTimeMatch(myTest_Time, masterTime));
	masterTime = safeDelete(masterTime);
}

TEST(times, match_Thursday_matches_Weekday)
{
	struct Time * masterTime = new(Time);
	setDayOfWeek(masterTime, WEEKDAY);
	setDayOfWeek(myTest_Time, THURSDAY);
	TEST_ASSERT_EQUAL(MATCH, checkTimeMatch(myTest_Time, masterTime));
	masterTime = safeDelete(masterTime);
}

TEST(times, match_Monday_doesNotMatch_Weekend)
{
	struct Time * masterTime = new(Time);
	setDayOfWeek(masterTime, WEEKEND);
	setDayOfWeek(myTest_Time, MONDAY);
	TEST_ASSERT_EQUAL(MATCH_FAIL, checkTimeMatch(myTest_Time, masterTime));
	masterTime = safeDelete(masterTime);
}

TEST(times, match_Friday_matches_Weekday)
{
	struct Time * masterTime = new(Time);
	setDayOfWeek(masterTime, WEEKDAY);
	setDayOfWeek(myTest_Time, FRIDAY);
	TEST_ASSERT_EQUAL(MATCH, checkTimeMatch(myTest_Time, masterTime));
	masterTime = safeDelete(masterTime);
}

TEST(times, match_Friday_doesNotMatch_Weekend)
{
	struct Time * masterTime = new(Time);
	setDayOfWeek(masterTime, WEEKEND);
	setDayOfWeek(myTest_Time, FRIDAY);
	TEST_ASSERT_EQUAL(MATCH_FAIL, checkTimeMatch(myTest_Time, masterTime));
	masterTime = safeDelete(masterTime);
}

TEST(times, match_Sunday_matches_Weekend)
{
	struct Time * masterTime = new(Time);
	setDayOfWeek(masterTime, WEEKEND);
	setDayOfWeek(myTest_Time, SUNDAY);
	TEST_ASSERT_EQUAL(MATCH, checkTimeMatch(myTest_Time, masterTime));
	masterTime = safeDelete(masterTime);
}

TEST(times, match_Sunday_doesNotMatch_Weekday)
{
	struct Time * masterTime = new(Time);
	setDayOfWeek(masterTime, WEEKDAY);
	setDayOfWeek(myTest_Time, SUNDAY);
	TEST_ASSERT_EQUAL(MATCH_FAIL, checkTimeMatch(myTest_Time, masterTime));
	masterTime = safeDelete(masterTime);
}

TEST(times, match_Saturday_matches_Weekend)
{
	struct Time * masterTime = new(Time);
	setDayOfWeek(masterTime, WEEKEND);
	setDayOfWeek(myTest_Time, SATURDAY);
	TEST_ASSERT_EQUAL(MATCH, checkTimeMatch(myTest_Time, masterTime));
	masterTime = safeDelete(masterTime);
}

TEST(times, match_Saturday_doesNotMatch_Weekday)
{
	struct Time * masterTime = new(Time);
	setDayOfWeek(masterTime, WEEKDAY);
	setDayOfWeek(myTest_Time, SATURDAY);
	TEST_ASSERT_EQUAL(MATCH_FAIL, checkTimeMatch(myTest_Time, masterTime));
	masterTime = safeDelete(masterTime);
}

TEST(times, match_Sunday_matches_everyday)
{
	struct Time * masterTime = new(Time);
	setDayOfWeek(masterTime, EVERYDAY);
	setDayOfWeek(myTest_Time, SUNDAY);
	TEST_ASSERT_EQUAL(MATCH, checkTimeMatch(myTest_Time, masterTime));
	masterTime = safeDelete(masterTime);
}

TEST(times, match_Friday_matches_everyday)
{
	struct Time * masterTime = new(Time);
	setDayOfWeek(masterTime, EVERYDAY);
	setDayOfWeek(myTest_Time, FRIDAY);
	TEST_ASSERT_EQUAL(MATCH, checkTimeMatch(myTest_Time, masterTime));
	masterTime = safeDelete(masterTime);
}

TEST(times, match_Saturday_matches_everyday)
{
	struct Time * masterTime = new(Time);
	setDayOfWeek(masterTime, EVERYDAY);
	setDayOfWeek(myTest_Time, SATURDAY);
	TEST_ASSERT_EQUAL(MATCH, checkTimeMatch(myTest_Time, masterTime));
	masterTime = safeDelete(masterTime);
}

TEST(times, match_NullReturns_Null)
{
	struct Time * masterTime = new(Time);
	setDayOfWeek(masterTime, EVERYDAY);
	setDayOfWeek(myTest_Time, SATURDAY);
	TEST_ASSERT_EQUAL(MATCH_FAIL, checkTimeMatch(NULL, masterTime));
	TEST_ASSERT_EQUAL(MATCH_FAIL, checkTimeMatch(myTest_Time, NULL));
	masterTime = safeDelete(masterTime);
}

TEST(times, match_Second_01_Returns_NullOnCorrectMinute)
{
	//set master time to 01 minutes
	struct Time * masterTime = new(Time);
	setSecond    (masterTime, SECOND_UNKNOWN);
	setMinute    (masterTime, 1);
	setHour      (masterTime, HOUR_UNKNOWN);
	setDayOfWeek (masterTime, DAY_OF_WEEK_UNKNOWN);
	setHourMode  (masterTime, HOUR_MODE_24);
	setAmPm      (masterTime, AM__PM_UNKNOWN);

	// set test time to 00:59 minutes:seconds
	resetTime(myTest_Time);
	setTimeUpdateIncrement(myTest_Time, UPDATE_SECONDS);
	setSecond(myTest_Time, 59);
	TEST_ASSERT_EQUAL(MATCH_FAIL, checkTimeMatch(myTest_Time, masterTime));

	// set test time to 01:00 minutes:seconds
	incrementTime(myTest_Time);
	TEST_ASSERT_EQUAL(MATCH, checkTimeMatch(myTest_Time, masterTime));

	// set test time to 01:01 minutes:seconds
	incrementTime(myTest_Time);
	TEST_ASSERT_EQUAL(MATCH_FAIL, checkTimeMatch(myTest_Time, masterTime));
	masterTime = safeDelete(masterTime);
}

TEST(times, match_Second_01_Returns_NullOnCorrectHour)
{
	//set master time to 01 hour
	struct Time * masterTime = new(Time);
	setSecond    (masterTime, SECOND_UNKNOWN);
	setMinute    (masterTime, MINUTE_UNKNOWN);
	setHour      (masterTime, 1);
	setDayOfWeek (masterTime, DAY_OF_WEEK_UNKNOWN);
	setHourMode  (masterTime, HOUR_MODE_24);
	setAmPm      (masterTime, AM__PM_UNKNOWN);

	// set test time to 00:59:59 hours:minutes:seconds
	resetTime(myTest_Time);
	setTimeUpdateIncrement(myTest_Time, UPDATE_SECONDS);
	setSecond(myTest_Time, 59);
	setMinute(myTest_Time, 59);
	TEST_ASSERT_EQUAL(MATCH_FAIL, checkTimeMatch(myTest_Time, masterTime));

	// set test time to 01:00:00 hours:minutes:seconds
	incrementTime(myTest_Time);
	TEST_ASSERT_EQUAL(MATCH, checkTimeMatch(myTest_Time, masterTime));

	// set test time to 01:00:01 hours:minutes:seconds
	incrementTime(myTest_Time);
	TEST_ASSERT_EQUAL(MATCH_FAIL, checkTimeMatch(myTest_Time, masterTime));
	masterTime = safeDelete(masterTime);
}


TEST(times, match_Minute_01_Returns_NullOnCorrectHour)
{
	//set master time to 01 minutes
	struct Time * masterTime = new(Time);
	setSecond    (masterTime, SECOND_UNKNOWN);
	setMinute    (masterTime, MINUTE_UNKNOWN);
	setHour      (masterTime, 1);
	setDayOfWeek (masterTime, DAY_OF_WEEK_UNKNOWN);
	setHourMode  (masterTime, HOUR_MODE_24);
	setAmPm      (masterTime, AM__PM_UNKNOWN);

	// set test time to 59:59 minutes:seconds
	resetTime(myTest_Time);
	setTimeUpdateIncrement(myTest_Time, UPDATE_SECONDS);
	setSecond(myTest_Time, 59);
	setMinute(myTest_Time, 59);
	TEST_ASSERT_EQUAL(MATCH_FAIL, checkTimeMatch(myTest_Time, masterTime));

	// set test time to 01:00 minutes:seconds
	incrementTime(myTest_Time);
	TEST_ASSERT_EQUAL(MATCH, checkTimeMatch(myTest_Time, masterTime));

	// set test time to 01:01 minutes:seconds
	incrementTime(myTest_Time);
	TEST_ASSERT_EQUAL(MATCH_FAIL, checkTimeMatch(myTest_Time, masterTime));
	masterTime = safeDelete(masterTime);
}

/***********  callback  **************/

TEST(times, getTime_OnSecondChange_cb_returns_Time_PostSecondCallback)
{
	TEST_ASSERT_EQUAL(Time_PostSecondCallback, getTime_OnSecondChange_cb(myTest_Time));
}

TEST(times, getTime_OnMinuteChange_cb_returns_Time_PostMinuteCallback)
{
	TEST_ASSERT_EQUAL(Time_PostMinuteCallback, getTime_OnMinuteChange_cb(myTest_Time));
}

TEST(times, getTime_OnHourChange_cb_returns_Time_PostHourCallback)
{
	TEST_ASSERT_EQUAL(Time_PostHourCallback, getTime_OnHourChange_cb(myTest_Time));
}


TEST(times, setTime_OnSecondChange_cb_returns_new_cb)
{
	TEST_ASSERT_EQUAL(times_test_general_cb,
			    setTime_OnSecondChange_cb(myTest_Time, times_test_general_cb));
}

TEST(times, setTime_OnMinuteChange_cb_returns_new_cb)
{
	TEST_ASSERT_EQUAL(times_test_general_cb,
			    setTime_OnMinuteChange_cb(myTest_Time, times_test_general_cb));
}

TEST(times, setTime_OnHourChange_cb_returns_new_cb)
{
	TEST_ASSERT_EQUAL(times_test_general_cb,
			    setTime_OnHourChange_cb(myTest_Time, times_test_general_cb));
}

TEST(times, setTime_OnSecondChange_cb_sets_new_cb)
{
	times_test_cb_count = 0;
	setTime_OnSecondChange_cb(myTest_Time, times_test_general_cb);
	TEST_ASSERT_EQUAL(times_test_general_cb, myTest_Time->time_OnSecondChange_cb);
	myTest_Time->time_OnSecondChange_cb(myTest_Time);
	TEST_ASSERT_EQUAL(1, times_test_cb_count);
}

TEST(times, setTime_OnMinuteChange_cb_sets_new_cb)
{
	times_test_cb_count = 0;
	setTime_OnMinuteChange_cb(myTest_Time, times_test_general_cb);
	TEST_ASSERT_EQUAL(times_test_general_cb, myTest_Time->time_OnMinuteChange_cb);
	myTest_Time->time_OnMinuteChange_cb(myTest_Time);
	TEST_ASSERT_EQUAL(1, times_test_cb_count);
}

TEST(times, setTime_OnHourChange_cb_sets_new_cb)
{
	times_test_cb_count = 0;
	setTime_OnHourChange_cb(myTest_Time, times_test_general_cb);
	TEST_ASSERT_EQUAL(times_test_general_cb, myTest_Time->time_OnHourChange_cb);
	//myTest_Time->time_OnHourChange_cb(myTest_Time);
	resetTime(myTest_Time);
	setSecond    (myTest_Time, 59);
	setMinute    (myTest_Time, 59);
	setHour      (myTest_Time, 1);
	incrementTime(myTest_Time);  // will fire hour, minutes and seconds callbacks
	incrementTime(myTest_Time);  // will fire only the seconds callback
	TEST_ASSERT_EQUAL(1, times_test_cb_count);  // should only increment one time for the hour cb
}


/***********  config  **************/

TEST(times, Config_copiesState)
{
	struct Time * masterTime = new(Time);
	setHour(masterTime, 5);
	setAmPm(masterTime, PM_TIME);
	config(myTest_Time, masterTime);
	TEST_ASSERT_EQUAL(5,       getHour(myTest_Time));
	TEST_ASSERT_EQUAL(PM_TIME, getAmPm(myTest_Time));
}

/***********  init  **************/

TEST(times, CanCall_Time_Init_MulitpleTimes)
{
	const void * originalTime = Time;
	const void * originalTimeClass = TimeClass;
	Time_init();
	Time_init();
	Time_init();

	// Verify that the class pointers are unchanged
	TEST_ASSERT_EQUAL_PTR(Time, originalTime);
	TEST_ASSERT_EQUAL_PTR(TimeClass, originalTimeClass);

	char outputFilename[] = "times_puto.txt";
	FILE * fp = fopen(outputFilename, "a");
	puto(myTest_Time, fp);
	fclose(fp);
}

TEST(times, memory_leak)
{
	// new structure location will move based on other processes
	// create and destroy a large number of objects
	// then look for the base address within an additional 10 new objects


	int i;

	// get original object location
	struct Event * newItem = new(Time);
	struct Event * originalPointer = newItem;
	delete(newItem);

	// create and destroy a large number of objects to cause a memory leak

	for (i = 0; i < 1000; i++) {
		newItem = new(Time);
		//printf("newItem: %p, i: %i\n", newItem, i);
		delete(newItem);
	}

	// check for original object location within a small number of additional objects
	//printf("\nOriginal newItem: %p\n", originalPointer);
	int originalPtrCount = 0;
	for (i = 0; i < 100; i++) {
		newItem = new(Time);
		//printf("newItem: %p, i: %i\n", newItem, i);
		delete(newItem);
		if (originalPointer == newItem) { originalPtrCount++; }
		newItem = NULL;
	}
	printf("\nTimes-memory-leak-test, originalPtrCount: %i, out of %i trials\n", originalPtrCount, 100);
	TEST_ASSERT_TRUE(originalPtrCount);
}



//****  Support Methods  ****************

static void times_test_setTime(struct Time * _self,
					second_t      second,
		            minute_t      minute,
					hour_t        hour,
					hourMode_t	  hourMode,
					AmPm_t        AmPm,
					dayOfWeek_t   dayOfWeek )
{
	_self->second    = second;
	_self->minute    = minute;
	_self->hour      = hour;
	_self->dayOfWeek = dayOfWeek;
	_self->hourMode  = hourMode;
	_self->AmPm      = AmPm;
	_self->dayOfWeek = dayOfWeek;
	return;
}

struct Time * times_test_general_cb(struct Time * _self)
{
	times_test_cb_count++;
	//printf("  XXX times_test_general_cb\n");
	return _self;
}

