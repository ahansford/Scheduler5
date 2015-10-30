/*
 * times-test-runner.c
 *
 *  Created on: August, 2015
 *      Author: Alan
 */


#include "..\unity\unity_fixture.h"
//#include "..\unity\unity.h"

TEST_GROUP_RUNNER(times)
{
	/**************************************/
	/**************  New  *****************/
	/**/
	RUN_TEST_CASE(times, myTestTime_IsNotNullOnCreate);
	RUN_TEST_CASE(times, Time_second_isUnknownOnCreate);
	RUN_TEST_CASE(times, Time_minute_isUnknownOnCreate);
	RUN_TEST_CASE(times, Time_hour_isUnknownOnCreate);
	RUN_TEST_CASE(times, Time_dayOfWeek_isUnknownOnCreate);
	RUN_TEST_CASE(times, Time_hourMode_isUnknownOnCreate);
	RUN_TEST_CASE(times, Time_AmPm_isUnknownOnCreate);

	/**************************************/
	/*************  Time  *****************/
	/**/
	RUN_TEST_CASE(times, init_Time_ClassReturnsNonNull);
	RUN_TEST_CASE(times, init_classOf_myTest_Time_is_Time);
	RUN_TEST_CASE(times, init_classOf_Time_is_TimeClass);
	RUN_TEST_CASE(times, init_ctor_pointsTo_TimeClass_ctor);
	RUN_TEST_CASE(times, init_ctor_pointsTo_Time_ctor);
	RUN_TEST_CASE(times, init_dtor_pointsTo_Time_dtor);
	RUN_TEST_CASE(times, init_copy_pointsTo_Time_copy);
	RUN_TEST_CASE(times, init_equal_pointsTo_Time_equal);
	RUN_TEST_CASE(times, init_config_pointsTo_Time_config);

	RUN_TEST_CASE(times, init_TimeClass_NameIs_TimeClass);

	/**************************************/
	/*******  TimeClass Methods  *********/
	/**/
	RUN_TEST_CASE(times, init_setSecond_pointsTo_Time_setSecond);
	RUN_TEST_CASE(times, init_getSecond_pointsTo_Time_getSecond);

	RUN_TEST_CASE(times, init_setMinute_pointsTo_Time_setMinute);
	RUN_TEST_CASE(times, init_getMinute_pointsTo_Time_getMinute);

	RUN_TEST_CASE(times, init_setHour_pointsTo_Time_setHour);
	RUN_TEST_CASE(times, init_getHour_pointsTo_Time_getHour);

	RUN_TEST_CASE(times, init_setDayOfWeek_pointsTo_Time_setDayOfWeek);
	RUN_TEST_CASE(times, init_getDayOfWeek_pointsTo_Time_getDayOfWeek);

	RUN_TEST_CASE(times, init_setHourMode_pointsTo_Time_setHourMode);
	RUN_TEST_CASE(times, init_getHourMode_pointsTo_Time_getHourMode);

	RUN_TEST_CASE(times, init_setAmPm_pointsTo_Time_setAmPm);
	RUN_TEST_CASE(times, init_getAmPm_pointsTo_Time_getAmPm);

	RUN_TEST_CASE(times, init_setTimeUpdateIncrement_pointsTo_Time_setTimeUpdateIncrement);
	RUN_TEST_CASE(times, init_getTimeUpdateIncrement_pointsTo_Time_getTimeUpdateIncrement);

	RUN_TEST_CASE(times, init_incrementTime_pointsTo_Time_incrementTime);

	RUN_TEST_CASE(times, init_checkTimeMatch_pointsTo_Time_checkTimeMatch);

	//****  delete/Time_dtor  ****************
	/**/
	RUN_TEST_CASE(times, delete_returns_SelfOnSuccess);
	RUN_TEST_CASE(times, delete_safeDelete_returnsNullOnSuccess);

	//****  Set/Get Second  ****************
	/**/
	RUN_TEST_CASE(times, getSecond_returns_returnsUnknownOnCreate);
	RUN_TEST_CASE(times, getSecond_returns_specificValue);
	RUN_TEST_CASE(times, setSecond_returnsSpecificValue);
	RUN_TEST_CASE(times, setSecond_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(times, setSecond_returnsSpecificValueOnSuccess);
	RUN_TEST_CASE(times, setSecond_canSetSpecificValue);

	//****  Set/Get Minute  ****************
	/**/
	RUN_TEST_CASE(times, getMinute_returns_returnsUnknownOnCreate);
	RUN_TEST_CASE(times, getMinute_returns_specificValue);
	RUN_TEST_CASE(times, setMinute_returnsSpecificValue);
	RUN_TEST_CASE(times, setMinute_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(times, setMinute_returnsSpecificValueOnSuccess);
	RUN_TEST_CASE(times, setMinute_canSetSpecificValue);

	//****  Set/Get hour  ****************
	/**/
	RUN_TEST_CASE(times, getHour_returns_returnsUnknownOnCreate);
	RUN_TEST_CASE(times, getHour_returns_specificValue);
	RUN_TEST_CASE(times, setHour_returnsSpecificValue);
	RUN_TEST_CASE(times, setHour_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(times, setHour_returnsSpecificValueOnSuccess);
	RUN_TEST_CASE(times, setHour_canSetSpecificValue);

	//****  Set/Get dayOfWeek  ****************
	/**/
	RUN_TEST_CASE(times, getDayOfWeek_returns_returnsUnknownOnCreate);
	RUN_TEST_CASE(times, getDayOfWeek_returns_specificValue);
	RUN_TEST_CASE(times, setDayOfWeek_returnsSpecificValue);
	RUN_TEST_CASE(times, setDayOfWeek_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(times, setDayOfWeek_returnsSpecificValueOnSuccess);
	RUN_TEST_CASE(times, setDayOfWeek_canSetSpecificValue);

	//****  Set/Get hourMode  ****************
	/**/
	RUN_TEST_CASE(times, getHourMode_returns_returnsUnknownOnCreate);
	RUN_TEST_CASE(times, getHourMode_returns_specificValue);

	RUN_TEST_CASE(times, setHourMode_returnsSpecificValue);
	RUN_TEST_CASE(times, setHourMode_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(times, setHourMode_returnsSpecificValueOnSuccess);
	RUN_TEST_CASE(times, setHourMode_canSetSpecificValue);

	//+++++++++++++++ convert hour mode times
	/**/
	RUN_TEST_CASE(times, setHourMode_TwelveAMHourNoChangeAlready12H_12H);
	RUN_TEST_CASE(times, setHourMode_ZeroToTwelveAM_withChangeTo12H);
	RUN_TEST_CASE(times, setHourMode_PmSetAfterNoon_withChangeTo12H);
	RUN_TEST_CASE(times, setHourMode_AmSetBeforeNoon_withChangeTo12H);

	RUN_TEST_CASE(times, setHourMode_ZeroHourNoChangeAlready24H_24H);
	RUN_TEST_CASE(times, setHourMode_PmSetAt12_withChangeTo24H);
	RUN_TEST_CASE(times, setHourMode_AmSetBefore12_withChangeTo24H);
	RUN_TEST_CASE(times, setHourMode_TwelveAmToZero_withChangeTo24H);
	RUN_TEST_CASE(times, setHourMode_Add12ForPm_withChangeTo24H);

	//****  Set/Get AmPm  ****************
	/**/
	RUN_TEST_CASE(times, getAmPm_returns_returnsUnknownOnCreate);
	RUN_TEST_CASE(times, getAmPm_returns_specificValue);
	RUN_TEST_CASE(times, setAmPm_returnsSpecificValue);
	RUN_TEST_CASE(times, setAmPm_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(times, setAmPm_returnsSpecificValueOnSuccess);
	RUN_TEST_CASE(times, setAmPm_canSetSpecificValue);

	//****  Set/Get hourMode  ****************
	/**/
	RUN_TEST_CASE(times, getTimeUpdateIncrement_returns_returnsUnknownOnCreate);
	RUN_TEST_CASE(times, getTimeUpdateIncrement_returns_specificValue);
	RUN_TEST_CASE(times, setTimeUpdateIncrement_returnsSpecificValue);
	RUN_TEST_CASE(times, setTimeUpdateIncrement_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(times, setTimeUpdateIncrement_returnsSpecificValueOnSuccess);
	RUN_TEST_CASE(times, setTimeUpdateIncrement_canSetSpecificValue);

	//****  incrementTime  ****************
	/**/
	RUN_TEST_CASE(times, setTime_correctlyUpdatesValues);
	RUN_TEST_CASE(times, incrementTime_Seconds_ToZero_OnFirstCall);
	RUN_TEST_CASE(times, incrementTime_Seconds_ToZero_From59);
	RUN_TEST_CASE(times, incrementTime_Hour_Increments_From5959);
	RUN_TEST_CASE(times, incrementTime_AmPm_toPM_From115959AM);
	RUN_TEST_CASE(times, incrementTime_AmPm_toAM_From115959PM);
	RUN_TEST_CASE(times, incrementTime_DayIncrements_From125959PM);
	RUN_TEST_CASE(times, incrementTime_Day_toSunday_fromSaturday);
	RUN_TEST_CASE(times, incrementTime_is10secondsAfter10increments);
	RUN_TEST_CASE(times, incrementTime_is10minutesAfter10increments);
	RUN_TEST_CASE(times, incrementTime_is2DaysAfter2x24x60increments);

	//++++++++++
	RUN_TEST_CASE(times, incrementTime_IncrementsMinutesToZeroOnFirstCall);
	RUN_TEST_CASE(times, incrementTime_IncrementsMinutesToZeroFrom59);
	//++++++++++
	RUN_TEST_CASE(times, incrementTime_Hour_to_1_from115959);
	//++++++++++
	RUN_TEST_CASE(times, incrementTime_DayIncrements_From235959_24H);
	RUN_TEST_CASE(times, incrementTime_Hour_to_0_From235959_24H);
	RUN_TEST_CASE(times, incrementTime_Hour_to_13_From125959_24H);
	RUN_TEST_CASE(times, incrementTime_Day_toSunday_fromSaturday_24H);
	RUN_TEST_CASE(times, incrementTime_Day_toSunday_fromSaturdayWithAmSet_24H);
	//++++++++++

	RUN_TEST_CASE(times, incrementTime_resetTime_120000AM_SUNDAY);

	//****  copy/Time_copy  ****************
	/**/
	RUN_TEST_CASE(times, copy_returnsSelfOnSuccess);
	RUN_TEST_CASE(times, copy_AllItemsCopiedToSelf);
	RUN_TEST_CASE(times, copy_returnsNullOnNullSelf);
	RUN_TEST_CASE(times, copy_returnsNullOnNullMaster);

	//****  equal/Time_equal  ********************
	/**/
	RUN_TEST_CASE(times, myTest_Time_IsEqualTo_myTest_Time);
	RUN_TEST_CASE(times, equal_UnequalSecondsReturn_Unequal);
	RUN_TEST_CASE(times, equal_UnequalMinutesReturn_Unequal);
	RUN_TEST_CASE(times, equal_UnequalHoursReturn_Unequal);

	RUN_TEST_CASE(times, equal_UnequalDayOfWeekReturn_Unequal);
	RUN_TEST_CASE(times, equal_UnequalHourModeReturn_Unequal);
	RUN_TEST_CASE(times, equal_UnequalAmPmReturn_Unequal);
	RUN_TEST_CASE(times, equal_NullReturns_Null);
	RUN_TEST_CASE(times, equal_CopiedTimeReturnsEqual);

	//****  checkTimeMatch  ********************
	/**/
	RUN_TEST_CASE(times, match_self_matches_self);
	RUN_TEST_CASE(times, equal_CopiedTimesMatch);
	RUN_TEST_CASE(times, match_Defaults_DoNotMatch);
	RUN_TEST_CASE(times, match_HourMatch_returnsMatch);
	RUN_TEST_CASE(times, match_UnknownMaster_returnsUnknown);

	RUN_TEST_CASE(times, match_Monday_matches_Monday);
	RUN_TEST_CASE(times, match_Monday_matches_Weekday);
	RUN_TEST_CASE(times, match_Tuesday_matches_Weekday);
	RUN_TEST_CASE(times, match_Wednesday_matches_Weekday);
	RUN_TEST_CASE(times, match_Thursday_matches_Weekday);

	RUN_TEST_CASE(times, match_Monday_doesNotMatch_Weekend);
	RUN_TEST_CASE(times, match_Friday_matches_Weekday);
	RUN_TEST_CASE(times, match_Friday_doesNotMatch_Weekend);
	RUN_TEST_CASE(times, match_Sunday_matches_Weekend);
	RUN_TEST_CASE(times, match_Sunday_doesNotMatch_Weekday);
	RUN_TEST_CASE(times, match_Saturday_matches_Weekend);
	RUN_TEST_CASE(times, match_Saturday_doesNotMatch_Weekday);
	RUN_TEST_CASE(times, match_Sunday_matches_everyday);
	RUN_TEST_CASE(times, match_Friday_matches_everyday);
	RUN_TEST_CASE(times, match_Saturday_matches_everyday);
	RUN_TEST_CASE(times, match_NullReturns_Null);

	RUN_TEST_CASE(times, match_Second_01_Returns_NullOnCorrectMinute);
	RUN_TEST_CASE(times, match_Second_01_Returns_NullOnCorrectHour);
	RUN_TEST_CASE(times, match_Minute_01_Returns_NullOnCorrectHour);

	/***********  callback  **************/
	/**/
	RUN_TEST_CASE(times, getTime_OnSecondChange_cb_returns_Time_PostSecondCallback);
	RUN_TEST_CASE(times, getTime_OnMinuteChange_cb_returns_Time_PostMinuteCallback);
	RUN_TEST_CASE(times, getTime_OnHourChange_cb_returns_Time_PostHourCallback);

	RUN_TEST_CASE(times, setTime_OnSecondChange_cb_returns_new_cb);
	RUN_TEST_CASE(times, setTime_OnMinuteChange_cb_returns_new_cb);
	RUN_TEST_CASE(times, setTime_OnHourChange_cb_returns_new_cb);

	RUN_TEST_CASE(times, setTime_OnSecondChange_cb_sets_new_cb);
	RUN_TEST_CASE(times, setTime_OnMinuteChange_cb_sets_new_cb);
	RUN_TEST_CASE(times, setTime_OnHourChange_cb_sets_new_cb);


	/***********  config  **************/
	/**/
	//RUN_TEST_CASE(times, Config_copiesState);

	/***********  init  **************/
	RUN_TEST_CASE(times, CanCall_Time_Init_MulitpleTimes);
	//RUN_TEST_CASE(times, memory_leak);

}

