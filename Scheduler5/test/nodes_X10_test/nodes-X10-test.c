/*
 * nodes-X10-test.c
 *
 *  Created on: Feb 24, 2015
 *      Author: Alan
 */


#include "..\..\src\cross_compiler_defs.h"
#include "..\unity\unity_fixture.h"
//#include "..\unity\unity.h"
#include "..\..\src\mocks\RuntimeErrorStub.h"

#include "..\..\src\nodes_X10\nodes-X10.h"
//#include "..\..\src\nodes_X10\nodes-X10-private.h"
//#include "..\..\src\nodes\nodes.h"
//#include "..\..\src\nodes\nodes-private.h"

/****************************************/
/*** Basic Object and Class Testing ****/
struct       Node_X10 *      myTest_Node;
const struct NodeClass_X10 * myNode_Class;
//const struct Node_X10 *      myNode_Ptr;


/****************************************/
/*********** Start of Testing ***********/
TEST_GROUP(nodesX10);

TEST_SETUP(nodesX10)
{
	RuntimeErrorStub_Reset();
	Node_X10_init();

	myTest_Node  = new(Node_X10);
	setID       (myTest_Node, ID_UNKNOWN);
	setState    (myTest_Node, STATE_UNKNOWN);
	setHouseCode(myTest_Node, X10_HOUSE_CODE_UNKNOWN);
	setUnitCode (myTest_Node, X10_UNIT_CODE_UNKNOWN);

	myNode_Class = classOf(myTest_Node);
	//myNode_Ptr   = (void *)Node_X10;
}

TEST_TEAR_DOWN(nodesX10)
{
	myTest_Node = safeDelete(myTest_Node);
	RuntimeErrorStub_Reset();
}

TEST(nodesX10, myTestNodeIsNotNullOnCreate)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  myTest_Node);
}

TEST(nodesX10, NodeStateisUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(STATE_UNKNOWN,  getState(myTest_Node));
}

TEST(nodesX10, NodeID_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(ID_UNKNOWN, getID(myTest_Node));
}

TEST(nodesX10, GetID_returnsUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(ID_UNKNOWN, getID(myTest_Node));
}
/**/
/**************************************/
/************  Node  *****************/

TEST(nodesX10, init_NodeClass_Returns_NonNull)
{
	TEST_ASSERT_NOT_EQUAL(NULL, NodeClass);
}

TEST(nodesX10, init_myNodeClass_hasClassOf_Node)
{
	//TEST_ASSERT_EQUAL(Node_X10, myTestNode->_.class);
	TEST_ASSERT_EQUAL(Node_X10, classOf(myTest_Node));
}

TEST(nodesX10, init_ctor_pointsTo_Node_ctor)
{
	TEST_ASSERT_EQUAL(Node_X10_ctor, (myTest_Node)->_._.class->ctor);
	//TEST_ASSERT_EQUAL(NodeX10_ctor, myNode_Class->_.ctor);

}
/**/
/**************************************/
/**********  NodeClass  **************/

TEST(nodesX10, init_NodeClass_ctor_pointsTo_NodeClass_ctor)
{
	TEST_ASSERT_EQUAL_PTR(NodeClass_X10_ctor, myTest_Node->_._.class->_.class->ctor);
	TEST_ASSERT_EQUAL_PTR(NodeClass_X10_ctor, myNode_Class->_._._.class->ctor);
}

TEST(nodesX10, init_getState_pointsTo_Node_getState)
{
	TEST_ASSERT_EQUAL_PTR(Node_getState, myNode_Class->_.getState);
}

TEST(nodesX10, init_setID_pointsTo_Node_setID)
{
	TEST_ASSERT_EQUAL_PTR(Node_setID, myNode_Class->_.setID);
}

TEST(nodesX10, init_getID_pointsTo_Node_getID)
{
	TEST_ASSERT_EQUAL_PTR(Node_getID, myNode_Class->_.getID);
}

TEST(nodesX10, init_NodeClass_NameIs_NodeClass_X10)
{
	TEST_ASSERT_EQUAL_STRING("NodeClass_X10", myTest_Node->_._.class->_.class->className);
	TEST_ASSERT_EQUAL_STRING("NodeClass_X10", myNode_Class->_._._.class->className);
}

TEST(nodesX10, ClassOf_Node_pointsTo_NodeClass_X10_OnCreate)
{
	TEST_ASSERT_EQUAL_PTR(NodeClass_X10, classOf(classOf(myTest_Node)));
}
/**/

//****  Node_X10_copy  ********************

TEST(nodesX10, copy_pointsToNewOnSuccess)
{
	struct Node_X10 * copyToNode = new(Node_X10);
	TEST_ASSERT_EQUAL_PTR(myTest_Node, copy(myTest_Node, copyToNode));
	copyToNode = safeDelete(copyToNode);
}

TEST(nodesX10, copy_returnsNullOnNullSelf)
{
	struct Node_X10 * copyToNode = new(Node);
	TEST_ASSERT_EQUAL_PTR(NULL, copy(NULL, copyToNode));
	copyToNode = safeDelete(copyToNode);
}

TEST(nodesX10, copy_returnsNullOnNullTarget)
{
	TEST_ASSERT_EQUAL_PTR(NULL, copy(myTest_Node, NULL));
}
/**/
//****  Node_X10_equal  ********************

TEST(nodesX10, equal_myTestNode_equalTo_MyTest_Node)
{
	TEST_ASSERT_EQUAL(1, equal(myTest_Node, myTest_Node));
}

TEST(nodesX10, equal_myTest_Node_NotEqual_OtherNode)
{
	struct Node * otherNode = new(Node);
	setState(myTest_Node, STATE_ON);
	setState(otherNode,   STATE_OFF);
	TEST_ASSERT_EQUAL(FALSE, equal(myTest_Node, otherNode));
	otherNode = safeDelete(otherNode);
}

TEST(nodesX10, equal_myTest_Node_NotEqual_Null)
{
	TEST_ASSERT_EQUAL(FALSE, equal(myTest_Node, NULL));
}

TEST(nodesX10, equal_myTest_Node_NotEqual_changedID)
{
	struct Node_X10 * copyToNode = new(Node_X10);
	setID(myTest_Node,        45);
	setState(myTest_Node,     STATE_ON);
	setHouseCode(myTest_Node, 10);
	setUnitCode(myTest_Node,  11);
	TEST_ASSERT_EQUAL_PTR(myTest_Node, copy(myTest_Node, copyToNode));
	copy(myTest_Node, copyToNode);
	setID(copyToNode, 53);
	TEST_ASSERT_EQUAL(FALSE, equal(NULL, copyToNode));
	copyToNode = safeDelete(copyToNode);
}

TEST(nodesX10, equal_myTest_Node_NotEqual_ChangedHouseCode)
{
	struct Node_X10 * copyToNode = new(Node_X10);
	setID       (copyToNode, 45);
	setState    (copyToNode, STATE_ON);
	setHouseCode(copyToNode, 10);
	setUnitCode (copyToNode, 11);

	copy(myTest_Node, copyToNode);
	setHouseCode(copyToNode, 44);
	TEST_ASSERT_EQUAL(FALSE, equal(NULL, copyToNode));
	copyToNode = safeDelete(copyToNode);
}

/**/

/**************************************/
/***********  Behaviors  **************/

TEST(nodesX10, Node_setIDto21)
{
	TEST_ASSERT_EQUAL(21, Node_setID(myTest_Node, 21));
}

TEST(nodesX10, class_setIDto21)
{
	const struct NodeClass * class = classOf( cast(Node, myTest_Node) );
	TEST_ASSERT_EQUAL(21, class->setID(myTest_Node, 21));
}

TEST(nodesX10, setID_to21)
{
	setID(myTest_Node, -1);
	TEST_ASSERT_EQUAL(-1, getID(myTest_Node));

	setID(myTest_Node, 21);
	TEST_ASSERT_EQUAL(21, getID(myTest_Node));
}

/***********  setState  **************/
TEST(nodesX10, setState_returns_NODE_ON)
{
	TEST_ASSERT_EQUAL(STATE_ON, setState(myTest_Node, STATE_ON));
}

TEST(nodesX10, setState_returns_NODE_OFF)
{
	TEST_ASSERT_EQUAL(STATE_OFF, setState(myTest_Node, STATE_OFF));
}

TEST(nodesX10, setState_NULL_returns_NODE_OFF)
{
	TEST_ASSERT_EQUAL(STATE_UNKNOWN, setState(NULL, STATE_OFF));
}

/***********  getState  **************/
TEST(nodesX10, getState_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(STATE_UNKNOWN, getState(myTest_Node));
}

TEST(nodesX10, getState_returns_NODE_ON)
{
	setState(myTest_Node, STATE_ON);
	TEST_ASSERT_EQUAL(STATE_ON, getState(myTest_Node));
}

TEST(nodesX10, getState_returns_NODE_OFF)
{
	setState(myTest_Node, STATE_OFF);
	TEST_ASSERT_EQUAL(STATE_OFF, getState(myTest_Node));
}

TEST(nodesX10, getState_NULL_returns_UnknownOn)
{
	TEST_ASSERT_EQUAL(STATE_UNKNOWN, getState(NULL));
}


/***********  setUnitCode  **************/
TEST(nodesX10, setUnitCode_returns_1)
{
	TEST_ASSERT_EQUAL(1, setUnitCode(myTest_Node, 1));
}

TEST(nodesX10, setUnitCode_NULL_returns_X10_UNIT_CODE_UNKNOWN)
{
	TEST_ASSERT_EQUAL(X10_UNIT_CODE_UNKNOWN, setUnitCode(NULL, 2));
}


/***********  getUnitCode  **************/
TEST(nodesX10, getUnitCode_returns_UNKNOWN_onCreate)
{
	TEST_ASSERT_EQUAL(X10_UNIT_CODE_UNKNOWN, getUnitCode(myTest_Node));
}

TEST(nodesX10, getUnitCode_returns_3)
{
	setUnitCode(myTest_Node, 3);
	TEST_ASSERT_EQUAL(3, getUnitCode(myTest_Node));
}

TEST(nodesX10, getUnitCode_NULL_returns_X10_UNIT_CODE_UNKNOWN)
{
	setUnitCode(myTest_Node, 4);
	TEST_ASSERT_EQUAL(X10_UNIT_CODE_UNKNOWN, getUnitCode(NULL));
}

/***********  config  **************/

TEST(nodesX10, Config_CopiesMasterDataToTarget)
{
	struct Node_X10 * master = new(Node_X10);
	setID(master, 1);
	setType(master, 2);
	setState(master, 3);
	setValue(master, 4);
	setHouseCode(master, 5);
	setUnitCode(master, 6);
	config(myTest_Node, master);

	TEST_ASSERT_EQUAL(1, getID(myTest_Node));
	TEST_ASSERT_EQUAL(2, getType(myTest_Node));
	TEST_ASSERT_EQUAL(3, getState(myTest_Node));
	TEST_ASSERT_EQUAL(4, getValue(myTest_Node));
	TEST_ASSERT_EQUAL(5, getHouseCode(myTest_Node));
	TEST_ASSERT_EQUAL(6, getUnitCode(myTest_Node));
	master = safeDelete(master);
}

TEST(nodesX10, Config_returnSelfOnSuccess)
{
	struct Node_X10 * master = new(Node_X10);
	config(myTest_Node, master);
	TEST_ASSERT_EQUAL(myTest_Node, config(myTest_Node, master));
	master = safeDelete(master);
}

/***********  X10_init  **************/
TEST(nodesX10, CanCall_Node_Init_MulitpleTimes)
{
	const void * originalNode_X10      = Node_X10;
	const void * originalNodeClass_X10 = NodeClass_X10;
	Node_X10_init();
	Node_X10_init();
	Node_X10_init();

	// Verify that behaviors are unchanged
	setState(myTest_Node, STATE_ON);
	TEST_ASSERT_EQUAL(STATE_ON, getState(myTest_Node));
	setState(myTest_Node, STATE_OFF);
	TEST_ASSERT_EQUAL(STATE_OFF, getState(myTest_Node));
	setState(myTest_Node, STATE_ON);
	TEST_ASSERT_EQUAL(STATE_ON, getState(myTest_Node));

	// Verify that the class pointers are unchanged
	TEST_ASSERT_EQUAL_PTR(Node_X10, originalNode_X10);
	TEST_ASSERT_EQUAL_PTR(NodeClass_X10, originalNodeClass_X10);

	setHouseCode(myTest_Node, 3);
	setUnitCode(myTest_Node, 4);
	setID(myTest_Node, 5);
	char outputFilename[] = "nodes_X10_puto.txt";
	FILE * fp = fopen(outputFilename, "a");
	puto(myTest_Node, fp);
}
/**/
