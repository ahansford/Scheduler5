/*
0 * times-test-runner.c
 *
 *  Created on: August, 2015
 *      Author: Alan
 */


#include "..\unity\unity_fixture.h"
//#include "..\unity\unity.h"

TEST_GROUP_RUNNER(events)
{
	/**************************************/
	/**************  New  *****************/
	/**/
	RUN_TEST_CASE(events, myTestEvent_IsNotNullOnCreate);
	RUN_TEST_CASE(events, events_time_NotNullOnCreate);
	RUN_TEST_CASE(events, events_message_NotNullOnCreate);

	/**************************************/
	/*************  Events  ***************/
	/**/
	RUN_TEST_CASE(events, init_newEvent_ClassReturnsNonNull);
	RUN_TEST_CASE(events, init_classOf_myTest_Event_is_Event);
	RUN_TEST_CASE(events, init_classOf_Event_is_EventClass);
	RUN_TEST_CASE(events, init_ctor_pointsTo_EventClass_ctor);
	RUN_TEST_CASE(events, init_ctor_pointsTo_Event_ctor);
	RUN_TEST_CASE(events, init_dtor_pointsTo_Event_dtor);
	RUN_TEST_CASE(events, init_copy_pointsTo_Event_copy);
	RUN_TEST_CASE(events, init_equal_pointsTo_Event_equal);
	RUN_TEST_CASE(events, init_config_pointsTo_Event_config);

	RUN_TEST_CASE(events, init_EventClass_NameIs_EventClass);

	/**************************************/
	/*******  EventClass Methods  *********/
	/**/
	RUN_TEST_CASE(events, init_setTimePointer_pointsTo_Event_setTimePointer);
	RUN_TEST_CASE(events, init_getTimePointer_pointsTo_Event_getTimePointer);

	RUN_TEST_CASE(events, init_setMessagePointer_pointsTo_Event_setMessagePointer);
	RUN_TEST_CASE(events, init_getMessagePointer_pointsTo_Event_getMessagePointer);

	RUN_TEST_CASE(events, init_updateEventRunCount_pointsTo_Event_updateEventRunCount);
	RUN_TEST_CASE(events, init_setEventRunCount_pointsTo_Event_setEventRunCount);
	RUN_TEST_CASE(events, init_getEventRunCount_pointsTo_Event_getEventRunCount);

	//****  delete/Time_dtor  ****************
	/**/
	RUN_TEST_CASE(events, delete_returns_mySelfOnSuccess);
	RUN_TEST_CASE(events, delete_safeDelete_returnsNullOnSuccess);

	//****  set/get time  ****************
	/**/
	RUN_TEST_CASE(events, getTimePointer_NotNullOnCreate);
	RUN_TEST_CASE(events, getTimePointer_returns_specificValue);

	RUN_TEST_CASE(events, setTimePointer_returnsSpecificValue);
	RUN_TEST_CASE(events, setTimePointer_returnsNullOnNullPtr);
	RUN_TEST_CASE(events, setTimePointer_returnsSpecificValueOnSuccess);
	RUN_TEST_CASE(events, setTimePointer_canSetSpecificValue);

	//****  set/get message  ****************
	/**/
	RUN_TEST_CASE(events, getMessagePointer_NotNullOnCreate);
	RUN_TEST_CASE(events, getMessagePointer_returns_specificValue);

	RUN_TEST_CASE(events, setMessagePointer_returnsSpecificValue);
	RUN_TEST_CASE(events, setMessagePointer_returnsNullOnNullPtr);
	RUN_TEST_CASE(events, setMessagePointer_returnsSpecificValueOnSuccess);
	RUN_TEST_CASE(events, setMessagePointer_canSetSpecificValue);

	//****  updateEventRunCount  ****************
	/**/
	RUN_TEST_CASE(events, updateEventRunCount_CountIsZeroOnCreate);
	RUN_TEST_CASE(events, updateEventRunCount_CountIsZeroOnWrongTime);
	RUN_TEST_CASE(events, updateEventRunCount_CountIsOneOnCorrectTime);
	RUN_TEST_CASE(events, updateEventRunCount_CountIncrementsOnMultipleCorrectTimes);
	RUN_TEST_CASE(events, updateEventRunCount_DoesNotIncrementsOnWrongTime);
	RUN_TEST_CASE(events, updateEventRunCount_ReturnsSelfOnSuccess);

	//****  decrementEventRunCount  ****************
	/**/
	RUN_TEST_CASE(events, decrementEventRunCounter_ResetsCountToZeroFromUnknown);
	RUN_TEST_CASE(events, decrementEventRunCounter_HoldsAtZeroFromZero);
	RUN_TEST_CASE(events, decrementEventRunCounter_Decreases1ToZero);
	RUN_TEST_CASE(events, decrementEventRunCounter_ConvertsNegativesToZero);
	RUN_TEST_CASE(events, decrementEventRunCounter_100_to_99);
	RUN_TEST_CASE(events, decrementEventRunCounter_ReturnsSelfOnSuccess);

	//****  setEventRunCount/getEventRunCount  ****************
	/**/
	RUN_TEST_CASE(events, getEventRunCount_ZeroOnCreate);
	RUN_TEST_CASE(events, getEventRunCount_returns_specificValue);

	RUN_TEST_CASE(events, setEventRunCount_returnsSpecificValue);
	RUN_TEST_CASE(events, setEventRunCount_returnsNullOnNullPtr);
	RUN_TEST_CASE(events, setEventRunCount_returnsSpecificValueOnSuccess);
	RUN_TEST_CASE(events, setEventRunCount_canSetSpecificValue);

	//****  copy/Event_copy  ****************
	/**/
	RUN_TEST_CASE(events, copy_returnsSelfOnSuccess);
	RUN_TEST_CASE(events, copy_AllItemsCopiedToSelf);
	RUN_TEST_CASE(events, copy_returnsNullOnNullSelf);
	RUN_TEST_CASE(events, copy_returnsNullOnNullMaster);

	//****  equal/Event_equal  ********************
	/**/
	RUN_TEST_CASE(events, equal_self_Equals_self);
	RUN_TEST_CASE(events, equal_UnequalTimesReturn_Unequal);
	RUN_TEST_CASE(events, equal_UnequalMessagesReturn_Unequal);
	RUN_TEST_CASE(events, equal_NullReturns_Unequal);
	RUN_TEST_CASE(events, equal_CopiedItemReturns_Equal);

	/***********  config  **************/
	RUN_TEST_CASE(events, Config_copiesData);

	/***********  init  **************/
	RUN_TEST_CASE(events, CanCall_Event_Init_MulitpleTimes);
	//RUN_TEST_CASE(events, memory_leak);

}

