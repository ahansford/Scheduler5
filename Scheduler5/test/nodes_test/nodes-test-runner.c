/*
 * nodes-test-runner.c
 *
 *  Created on: July, 2015
 *      Author: Alan
 */


#include "..\unity\unity_fixture.h"
//#include "..\unity\unity.h"

TEST_GROUP_RUNNER(nodes)
{
	/**/
	RUN_TEST_CASE(nodes, myTestNodeIsNotNullOnCreate);
	RUN_TEST_CASE(nodes, NodeStateisUnknownOnCreate);
	RUN_TEST_CASE(nodes, NodeID_UnknownOnCreate);
	RUN_TEST_CASE(nodes, GetID_returnsUnknownOnCreate);

	/**************************************/
	/************  Node  *****************/
	/**/
	RUN_TEST_CASE(nodes, init_Node_ClassReturnsNonNull);
	RUN_TEST_CASE(nodes, init_myNode_ClassHasClassOf_Node);
	RUN_TEST_CASE(nodes, init_ctor_pointsTo_Node_ctor);
	RUN_TEST_CASE(nodes, init_dtor_pointsTo_Node_dtor);
	RUN_TEST_CASE(nodes, init_copy_pointsTo_Node_copy);
	RUN_TEST_CASE(nodes, init_equal_pointsTo_Node_equal);
	RUN_TEST_CASE(nodes, sizeOf_Node_is_32);

	/**************************************/
	/**********  NodeClass  **************/
	/**/
	RUN_TEST_CASE(nodes, init_NodeClass_ctor_pointsTo_NodeClass_ctor);

	RUN_TEST_CASE(nodes, init_setID_pointsTo_Node_setID);
	RUN_TEST_CASE(nodes, init_getID_pointsTo_Node_getID);

	RUN_TEST_CASE(nodes, init_setType_pointsTo_Node_setType);
	RUN_TEST_CASE(nodes, init_getType_pointsTo_Node_getType);

	RUN_TEST_CASE(nodes, init_setState_pointsTo_Node_setState);
	RUN_TEST_CASE(nodes, init_getState_pointsTo_Node_getState);

	RUN_TEST_CASE(nodes, init_setValue_pointsTo_Node_setValue);
	RUN_TEST_CASE(nodes, init_getValue_pointsTo_Node_getValue);

	RUN_TEST_CASE(nodes, init_setRunCount_pointsTo_Node_setRunCount);
	RUN_TEST_CASE(nodes, init_getRunCount_pointsTo_Node_getRunCount);

	RUN_TEST_CASE(nodes, init_incrementRunCount_pointsTo_Node_incrementRunCount);
	RUN_TEST_CASE(nodes, init_decrementRunCount_pointsTo_Node_decrementRunCount);

	RUN_TEST_CASE(nodes, init_setLinkedNode_pointsTo_Node_setLinkedNode);
	RUN_TEST_CASE(nodes, init_getLinkedNode_pointsTo_Node_getLinkedNode);

	RUN_TEST_CASE(nodes, init_setLinkedMessage_pointsTo_Node_setLinkedMessage);
	RUN_TEST_CASE(nodes, init_getLinkedMessage_pointsTo_Node_getLinkedMessage);

	RUN_TEST_CASE(nodes, init_getPtrToSelfIfReadyToRun_pointsTo_Node_getPtrToSelfIfReadyToRun);

	RUN_TEST_CASE(nodes, init_NodeClass_NameIs_NodeClass);
	RUN_TEST_CASE(nodes, ClassOf_Node_pointsTo_NodeClass_OnCreate);
	RUN_TEST_CASE(nodes, sizeOf_NodeClass_is_108);

	//****  delete/Node_dtor  ****************
	/**/
	RUN_TEST_CASE(nodes, delete_returns_myTestNodeOnValidPtr);
	RUN_TEST_CASE(nodes, delete_safeDelete_returnsNullOnSuccess);

	//****  Node_copy  ****************
	/**/
	RUN_TEST_CASE(nodes, copy_returnsSelfOnSuccess);
	RUN_TEST_CASE(nodes, copy_AllItemsCopiedToSelf);
	RUN_TEST_CASE(nodes, copy_returnsNullOnNullSelf);
	RUN_TEST_CASE(nodes, copy_returnsNullOnNullTarget);
	RUN_TEST_CASE(nodes, copy_createsAndAddsLocalMessage);
	// trigger recursive calls to copy
	RUN_TEST_CASE(nodes, copy_createsAndAddsLinkedNode);
	RUN_TEST_CASE(nodes, copy_AddsChainOfLinkedNodes);

	//****  Node_equal  ********************
	/**/
	RUN_TEST_CASE(nodes, myTest_Node_IsEqualTo_myTest_Node);
	RUN_TEST_CASE(nodes, equal_returnsUnequalWithUnequalIDs);
	RUN_TEST_CASE(nodes, equal_UnequalTypesReturn_Unequal);
	RUN_TEST_CASE(nodes, equal_UnequalStatesReturn_Unequal);
	RUN_TEST_CASE(nodes, equal_UnequalValuesReturn_Unequal);
	// runCount is a dynamic data value and this test has been disabled
		//RUN_TEST_CASE(nodes, equal_UnequalRunCountsReturn_Unequal);
	RUN_TEST_CASE(nodes, equal_UnequalLinkedNodeReturn_Equal);
	RUN_TEST_CASE(nodes, equal_UnequalLinkedMessageReturn_Equal);


	RUN_TEST_CASE(nodes, equal_myTest_Node_NotEqual_Null);

	/***********  setID/getID  **************/
	/**/
	RUN_TEST_CASE(nodes, Node_setIDto21);
	RUN_TEST_CASE(nodes, class_setIDto21);
	RUN_TEST_CASE(nodes, setID_to21);

	/***********  setID/getID  **************/
	/**/
	RUN_TEST_CASE(nodes, getState_returns_UnknownOnCreate);
	RUN_TEST_CASE(nodes, setState_ON);
	RUN_TEST_CASE(nodes, setState_OFF);

	/***********  setType/getType  **************/
	/**/
	RUN_TEST_CASE(nodes, nodeType_UNKNOWN_onCreate);
	RUN_TEST_CASE(nodes, setType_ToSpecificType);
	RUN_TEST_CASE(nodes, setType_ReturnsTypeOnSuccess);
	RUN_TEST_CASE(nodes, getType_SuccessfullyReturnsType);

	/***********  setValue/getValue  **************/
	/**/
	RUN_TEST_CASE(nodes, nodeValue_UNKNOWN_onCreate);
	RUN_TEST_CASE(nodes, setValue_ToSpecificValue);
	RUN_TEST_CASE(nodes, getValue_ReturnsValueOnSuccess);
	RUN_TEST_CASE(nodes, getValue_SuccessfullyReturnsValue);

	/*******  setRunCount/getRunCount  *********/
	/**/
	RUN_TEST_CASE(nodes, runCount_UNKNOWN_onCreate);
	RUN_TEST_CASE(nodes, setRunCount_ToSpecificValue);
	RUN_TEST_CASE(nodes, getRunCount_ReturnsValueOnSuccess);
	RUN_TEST_CASE(nodes, getRunCount_SuccessfullyReturnsValue);

	/*******  incrementRunCount/decrementRunCount  *********/
	/**/
	RUN_TEST_CASE(nodes, incrementRunCount_setValueOneOnFirstCall);
	RUN_TEST_CASE(nodes, incrementRunCount_setValueTwoOnSecondCall);
	RUN_TEST_CASE(nodes, decrementRunCount_returnsZeroWithOriginalOfOne);
	RUN_TEST_CASE(nodes, decrementRunCount_returnsZeroIfOriginalyZero);

	/*******  setNodeLink/getNodeLink  *********/
	/**/
	RUN_TEST_CASE(nodes, nodeLink_UNKNOWN_onCreate);
	RUN_TEST_CASE(nodes, setNodeLink_ToSpecificValue);
	RUN_TEST_CASE(nodes, getNodeLink_ReturnsValueOnSuccess);
	RUN_TEST_CASE(nodes, getNodeLink_SuccessfullyReturnsValue);

	/*******  getPtrToSelfIfReadyToRun  *********/
	/**/
	RUN_TEST_CASE(nodes, incrementRunCount_returnsNullOnUnknown);
	RUN_TEST_CASE(nodes, incrementRunCount_returnsNullOnZero);
	RUN_TEST_CASE(nodes, incrementRunCount_returnsSelfOnOne);
	RUN_TEST_CASE(nodes, incrementRunCount_returnsSelfOnTwo);
	RUN_TEST_CASE(nodes, incrementRunCount_returnsSelfIfLinkedNotReady);
	RUN_TEST_CASE(nodes, incrementRunCount_returnsLinkedOnReady);
	RUN_TEST_CASE(nodes, incrementRunCount_returnsLinkedIfBothReady);
	RUN_TEST_CASE(nodes, incrementRunCount_returnsMiddleNodeOnReady);
	RUN_TEST_CASE(nodes, incrementRunCount_returnsNullAfterAcknowlwdge);

	/*******  setLinkedMessage/getLinkedMessage  *********/
	/**/
	RUN_TEST_CASE(nodes, linkedMessage_UNKNOWN_onCreate);
	RUN_TEST_CASE(nodes, setLinkedMessage_ToSpecificValue);
	RUN_TEST_CASE(nodes, getLinkedMessage_ReturnsValueOnSuccess);
	RUN_TEST_CASE(nodes, getLinkedMessage_SuccessfullyReturnsValue);

	// these tests trigger recursive calls to delete()
	/**/
	RUN_TEST_CASE(nodes, delete_SuccessfullyHandlesLinkedNode);
	RUN_TEST_CASE(nodes, delete_SuccessfullyHandlesChainedNodes);
	RUN_TEST_CASE(nodes, delete_SuccessfullyHandlesLinkedMessage);

	/***********  config  **************/
	/**/
	RUN_TEST_CASE(nodes, Config_copiesState);

	RUN_TEST_CASE(nodes, CanCall_Node_Init_MulitpleTimes);
	RUN_TEST_CASE(nodes, memory_leak);
}

