/*
0 * times-test-runner.c
 *
 *  Created on: August, 2015
 *      Author: Alan
 */


#include "..\unity\unity_fixture.h"
//#include "..\unity\unity.h"

TEST_GROUP_RUNNER(eventController)
{
	/**************************************/
	/*******  EventController_init  *******/

	RUN_TEST_CASE(eventController, create1);
	RUN_TEST_CASE(eventController, create2);
	/**/
	RUN_TEST_CASE(eventController, myEventList_IsNotNullAfterInit);
	RUN_TEST_CASE(eventController, myNodeList_IsNotNullAfterInit);
	RUN_TEST_CASE(eventController, myTime_IsNotNullAfterInit);

	/**************************************/
	/*****  EventController_addNode  ******/
	/**/
	RUN_TEST_CASE(eventController, eventController_addNode_addsNode);
	RUN_TEST_CASE(eventController, eventController_addNode_returnsNodePtr);
	RUN_TEST_CASE(eventController, eventController_addNull_returnsNull);

	/**************************************/
	/****  EventController_removeNode  ****/
	/**/
	RUN_TEST_CASE(eventController, eventController_removeNode_removesFirstNode);
	RUN_TEST_CASE(eventController, eventController_removeNode_removesSecondNode);
	RUN_TEST_CASE(eventController, eventController_removeNode_returnsNodePtr);
	RUN_TEST_CASE(eventController, eventController_removeNull_returnsNull);

	/********************************************/
	/****  EventController_incrementGetNode  ****/
	/**/
	RUN_TEST_CASE(eventController, eventController_incrementGetNode_FirstNode);
	RUN_TEST_CASE(eventController, eventController_incrementGetNode_SecondNode);
	RUN_TEST_CASE(eventController, eventController_incrementGetNode_NullOnEmpty);

	/**************************************/
	/*****  EventController_addEvent  *****/
	/**/
	RUN_TEST_CASE(eventController, eventController_addEvent_addsEvent);
	RUN_TEST_CASE(eventController, eventController_addEvent_returnsEventPtr);
	RUN_TEST_CASE(eventController, eventController_addEvent_NullreturnsNull);

	/**************************************/
	/*****  EventController_removeEvent  *****/
	/**/
	RUN_TEST_CASE(eventController, eventController_removeEvent_removesFirstEvent);
	RUN_TEST_CASE(eventController, eventController_removeEvent_removesSecondEvent);
	RUN_TEST_CASE(eventController, eventController_removeEvent_returnsEventPtr);
	RUN_TEST_CASE(eventController, eventController_removeEvent_NullreturnsNull);

	/********************************************/
	/****  EventController_incrementGetEvent  ****/
	/**/
	RUN_TEST_CASE(eventController, eventController_incrementGetEvent_FirstEvent);
	RUN_TEST_CASE(eventController, eventController_incrementGetEvent_SecondEvent);
	RUN_TEST_CASE(eventController, eventController_incrementGetEvent_NullOnEmpty);

	/********************************************/
	/**************  executeEvent  **************/
	/**/
	RUN_TEST_CASE(eventController, eventController_executeEvent_returnsNodePointer);
	RUN_TEST_CASE(eventController, eventController_executeEvent_copiesEventMessageToNode);
	RUN_TEST_CASE(eventController, eventController_executeEvent_returnsNullOnNullEvent);
	RUN_TEST_CASE(eventController, eventController_executeEvent_returnsNullOnMissingNode);
	RUN_TEST_CASE(eventController, eventController_executeEvent_returnsNullOnMissingNodeID);
	RUN_TEST_CASE(eventController, eventController_executeEvent_decrementsRunCountOnSuccess);
	RUN_TEST_CASE(eventController, eventController_executeEvent_RunCountUnchangedOnFailure);

	/********************************************/
	/*************  executeMessage  *************/
	/**/
	RUN_TEST_CASE(eventController, eventController_executeMessage_returnsTargetNodePointer);
	RUN_TEST_CASE(eventController, eventController_executeMessage_copiesMessageToNode);
	RUN_TEST_CASE(eventController, eventController_executeMessage_returnsNullOnNullMessage);
	RUN_TEST_CASE(eventController, eventController_executeMessage_returnsNullOnMissingNode);
	RUN_TEST_CASE(eventController, eventController_executeMessage_returnsNullOnMissingNodeID);
	RUN_TEST_CASE(eventController, eventController_executeMessage_decrementsRunCountOnSuccess);
	RUN_TEST_CASE(eventController, eventController_executeMessage_RunCountUnchangedOnFailure);


	/********************************************/
	/*******  processAllReadyNodeMessages  ******/
	/**/
	RUN_TEST_CASE(eventController, processAllReadyNodeMessages_returnsOneAfterProcessingMessage);
	RUN_TEST_CASE(eventController, processAllReadyNodeMessages_copiesMessageToNode);
	RUN_TEST_CASE(eventController, processAllReadyNodeMessages_returnsZeroRunsIfNothingReady);
	RUN_TEST_CASE(eventController, processAllReadyNodeMessages_returnsZeroRunsTargetNodeMissing);
	RUN_TEST_CASE(eventController, processAllReadyNodeMessages_returnsZeroRunsTargetMessageMissing);
	RUN_TEST_CASE(eventController, processAllReadyNodeMessages_returnsZeroOnMissingNodeID);

	/********************************************/
	/********  processAllReadyNodeEvents  *******/
	/**/
	RUN_TEST_CASE(eventController, processAllReadyEvents_returnsOneAfterProcessingEvents);
	RUN_TEST_CASE(eventController, processAllReadyEvents_copiesMessageToNode);
	RUN_TEST_CASE(eventController, processAllReadyEvents_returnsZeroIfNothingReady);
	RUN_TEST_CASE(eventController, processAllReadyEvents_returnsZeroOnTargetNodeMissing);
	RUN_TEST_CASE(eventController, processAllReadyEvents_returnsZeroOnNoEventMissing);
	RUN_TEST_CASE(eventController, processAllReadyEvents_returnsZeroOnMissingNodeID);

	/***************************************************/
	/*****  processAllReadyEventsAndNodeMessagges  *****/
	/**/
	RUN_TEST_CASE(eventController, processAllReadyEventsAndNodeMessagges_processesOneEvent);
	RUN_TEST_CASE(eventController, processAllReadyEventsAndNodeMessagges_processesOneNodeMessage);
	RUN_TEST_CASE(eventController, processAllReadyEventsAndNodeMessagges_processesOneEventandMessage);


	/********************************************/
	/**********  updateEventRunCounts  **********/
	/**/
	RUN_TEST_CASE(eventController, updateEventRunCounts_returnsOneAfterProcessingSingleEvent);
	RUN_TEST_CASE(eventController, updateEventRunCounts_returnsZeroIfNoTimeMatch);
	RUN_TEST_CASE(eventController, updateEventRunCounts_returnsZeroIfNoEvents);

	RUN_TEST_CASE(eventController, updateEventRunCounts_incrementTime_incrementsRunCount);

	//RUN_TEST_CASE(eventController, memory_leak);

	RUN_TEST_CASE(eventController, API_test);



}

