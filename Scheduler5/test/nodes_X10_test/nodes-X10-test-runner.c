/*
 * nodes-X10-test-runner.c
 *
 *  Created on: Feb 24, 2015
 *      Author: Alan
 */


#include "..\unity\unity_fixture.h"
//#include "..\unity\unity.h"

TEST_GROUP_RUNNER(nodesX10)
{

	RUN_TEST_CASE(nodesX10, myTestNodeIsNotNullOnCreate);
	RUN_TEST_CASE(nodesX10, NodeStateisUnknownOnCreate);
	RUN_TEST_CASE(nodesX10, NodeID_UnknownOnCreate);
	RUN_TEST_CASE(nodesX10, GetID_returnsUnknownOnCreate);
	/* */
	/**************************************/
	/************  Node  *****************/

	RUN_TEST_CASE(nodesX10, init_NodeClass_Returns_NonNull);
	RUN_TEST_CASE(nodesX10, init_myNodeClass_hasClassOf_Node);
	RUN_TEST_CASE(nodesX10, init_ctor_pointsTo_Node_ctor);
	/**/
	/**************************************/
	/**********  NodeClass  **************/

	RUN_TEST_CASE(nodesX10, init_NodeClass_ctor_pointsTo_NodeClass_ctor);
	RUN_TEST_CASE(nodesX10, init_getState_pointsTo_Node_getState);
	RUN_TEST_CASE(nodesX10, init_setID_pointsTo_Node_setID);
	RUN_TEST_CASE(nodesX10, init_getID_pointsTo_Node_getID);
	RUN_TEST_CASE(nodesX10, init_NodeClass_NameIs_NodeClass_X10);
	RUN_TEST_CASE(nodesX10, ClassOf_Node_pointsTo_NodeClass_X10_OnCreate);
	/**/

	//****  Node_X10_copy  ****************
	/**/
	RUN_TEST_CASE(nodesX10, copy_pointsToNewOnSuccess);
	RUN_TEST_CASE(nodesX10, copy_returnsNullOnNullSelf);
	RUN_TEST_CASE(nodesX10, copy_returnsNullOnNullTarget);

	//****  Node_X10_equal  ********************
	/**/
	RUN_TEST_CASE(nodesX10, equal_myTestNode_equalTo_MyTest_Node);
	RUN_TEST_CASE(nodesX10, equal_myTest_Node_NotEqual_OtherNode);
	RUN_TEST_CASE(nodesX10, equal_myTest_Node_NotEqual_Null);
	RUN_TEST_CASE(nodesX10, equal_myTest_Node_NotEqual_changedID);
	RUN_TEST_CASE(nodesX10, equal_myTest_Node_NotEqual_ChangedHouseCode);

	/**************************************/
	/***********  Behaviors  **************/

	RUN_TEST_CASE(nodesX10, Node_setIDto21);
	RUN_TEST_CASE(nodesX10, class_setIDto21);
	RUN_TEST_CASE(nodesX10, setID_to21);


	/***********  setState  **************/
	RUN_TEST_CASE(nodesX10, setState_returns_NODE_ON);
	RUN_TEST_CASE(nodesX10, setState_returns_NODE_OFF);
	RUN_TEST_CASE(nodesX10, setState_NULL_returns_NODE_OFF);

	/***********  getState  **************/
	RUN_TEST_CASE(nodesX10, getState_returns_UnknownOnCreate);
	RUN_TEST_CASE(nodesX10, getState_returns_NODE_ON);
	RUN_TEST_CASE(nodesX10, getState_returns_NODE_OFF);
	RUN_TEST_CASE(nodesX10, getState_NULL_returns_UnknownOn);

	/***********  setUnitCode  **************/
	RUN_TEST_CASE(nodesX10, getUnitCode_returns_UNKNOWN_onCreate);
	RUN_TEST_CASE(nodesX10, setUnitCode_returns_1);
	RUN_TEST_CASE(nodesX10, setUnitCode_NULL_returns_X10_UNIT_CODE_UNKNOWN);

	/***********  getUnitCode  **************/
	RUN_TEST_CASE(nodesX10, getUnitCode_returns_UNKNOWN_onCreate);
	RUN_TEST_CASE(nodesX10, getUnitCode_returns_3);
	RUN_TEST_CASE(nodesX10, getUnitCode_NULL_returns_X10_UNIT_CODE_UNKNOWN);

	/***********  config  **************/
	RUN_TEST_CASE(nodesX10, Config_CopiesMasterDataToTarget);

	RUN_TEST_CASE(nodesX10, Config_returnSelfOnSuccess);


	RUN_TEST_CASE(nodesX10, CanCall_Node_Init_MulitpleTimes);
	/**/
}

