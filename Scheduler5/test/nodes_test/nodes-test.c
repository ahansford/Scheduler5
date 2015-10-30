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

#include "..\..\src\nodes\nodes.h"
//#include "..\..\src\nodes\nodes-private.h"

/****************************************/
/*** Basic Object and Class Testing ****/
struct       Node *      myTest_Node;
const struct NodeClass * myNode_Class;
const struct Node *      myNode_ptr;


/****************************************/
/*********** Start of Testing ***********/
TEST_GROUP(nodes);

TEST_SETUP(nodes)
{
	RuntimeErrorStub_Reset();
	Node_init();
	myTest_Node  = new(Node);

	setID(myTest_Node, ID_UNKNOWN);
	setState(myTest_Node, STATE_UNKNOWN);

	myNode_Class = classOf(myTest_Node);
	myNode_ptr      = (void *)Node;
}

TEST_TEAR_DOWN(nodes)
{
	myTest_Node = safeDelete(myTest_Node);
	RuntimeErrorStub_Reset();
}

TEST(nodes, myTestNodeIsNotNullOnCreate)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  myTest_Node);
}

TEST(nodes, NodeStateisUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(STATE_UNKNOWN,  myTest_Node->nodeState);
}

TEST(nodes, NodeID_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(ID_UNKNOWN, myTest_Node->nodeID);
}

TEST(nodes, GetID_returnsUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(ID_UNKNOWN, getID(myTest_Node));
}

/**************************************/
/************  Node  *****************/
TEST(nodes, init_Node_ClassReturnsNonNull)
{
	TEST_ASSERT_NOT_EQUAL(NULL, NodeClass);
}

TEST(nodes, init_myNode_ClassHasClassOf_Node)
{
	TEST_ASSERT_EQUAL(Node, myTest_Node->_.class);
	TEST_ASSERT_EQUAL(Node, classOf(myTest_Node));
	TEST_ASSERT_EQUAL(Object, super(classOf(myTest_Node)));
}

TEST(nodes, init_ctor_pointsTo_Node_ctor)
{
	TEST_ASSERT_EQUAL(Node_ctor, myTest_Node->_.class->ctor);
}

TEST(nodes, init_dtor_pointsTo_Node_dtor)
{
	TEST_ASSERT_EQUAL(Node_dtor, myTest_Node->_.class->dtor);
}

TEST(nodes, init_copy_pointsTo_Node_copy)
{
	TEST_ASSERT_EQUAL(Node_copy, myTest_Node->_.class->copy);
}

TEST(nodes, init_equal_pointsTo_Node_equal)
{
	TEST_ASSERT_EQUAL(Node_equal, myTest_Node->_.class->equal);
}

TEST(nodes, sizeOf_Node_is_32)
{
	TEST_ASSERT_EQUAL(32, myTest_Node->_.class->sizeOfThisClass);
}


/**************************************/
/**********  NodeClass  **************/

TEST(nodes, init_NodeClass_ctor_pointsTo_NodeClass_ctor)
{
	TEST_ASSERT_EQUAL(NodeClass_ctor, myTest_Node->_.class->_.class->ctor);
}

TEST(nodes, init_setID_pointsTo_Node_setID)
{
	TEST_ASSERT_EQUAL(Node_setID, myNode_Class->setID);
}

TEST(nodes, init_getID_pointsTo_Node_getID)
{
	TEST_ASSERT_EQUAL(Node_getID, myNode_Class->getID);
}

TEST(nodes, init_setType_pointsTo_Node_setType)
{
	TEST_ASSERT_EQUAL(Node_setType, myNode_Class->setType);
}

TEST(nodes, init_getType_pointsTo_Node_getType)
{
	TEST_ASSERT_EQUAL(Node_getType, myNode_Class->getType);
}

TEST(nodes, init_setState_pointsTo_Node_setState)
{
	TEST_ASSERT_EQUAL(Node_setState, myNode_Class->setState);
}

TEST(nodes, init_getValue_pointsTo_Node_getValue)
{
	TEST_ASSERT_EQUAL(Node_getValue, myNode_Class->getValue);
}

TEST(nodes, init_setValue_pointsTo_Node_setValue)
{
	TEST_ASSERT_EQUAL(Node_setValue, myNode_Class->setValue);
}

TEST(nodes, init_getState_pointsTo_Node_getState)
{
	TEST_ASSERT_EQUAL(Node_getState, myNode_Class->getState);
}

TEST(nodes, init_setRunCount_pointsTo_Node_setRunCount)
{
	TEST_ASSERT_EQUAL(Node_setRunCount, myNode_Class->setRunCount);
}

TEST(nodes, init_getRunCount_pointsTo_Node_getRunCount)
{
	TEST_ASSERT_EQUAL(Node_getRunCount, myNode_Class->getRunCount);
}

TEST(nodes, init_incrementRunCount_pointsTo_Node_incrementRunCount)
{
	TEST_ASSERT_EQUAL(Node_incrementRunCount, myNode_Class->incrementRunCount);
}

TEST(nodes, init_decrementRunCount_pointsTo_Node_decrementRunCount)
{
	TEST_ASSERT_EQUAL(Node_decrementRunCount, myNode_Class->decrementRunCount);
}

TEST(nodes, init_setLinkedNode_pointsTo_Node_setLinkedNode)
{
	TEST_ASSERT_EQUAL(Node_setLinkedNode, myNode_Class->setLinkedNode);
}

TEST(nodes, init_getLinkedNode_pointsTo_Node_getLinkedNode)
{
	TEST_ASSERT_EQUAL(Node_getLinkedNode, myNode_Class->getLinkedNode);
}


TEST(nodes, init_setLinkedMessage_pointsTo_Node_setLinkedMessage)
{
	TEST_ASSERT_EQUAL(Node_setLinkedMessage, myNode_Class->setLinkedMessage);
}

TEST(nodes, init_getLinkedMessage_pointsTo_Node_getLinkedMessage)
{
	TEST_ASSERT_EQUAL(Node_getLinkedMessage, myNode_Class->getLinkedMessage);
}


TEST(nodes, init_getPtrToSelfIfReadyToRun_pointsTo_Node_getPtrToSelfIfReadyToRun)
{
	TEST_ASSERT_EQUAL(Node_getPtrToSelfIfReadyToRun, myNode_Class->getPtrToSelfIfReadyToRun);
}

TEST(nodes, init_NodeClass_NameIs_NodeClass)
{
	TEST_ASSERT_EQUAL_STRING("NodeClass", myTest_Node->_.class->_.class->className);
}

TEST(nodes, ClassOf_Node_pointsTo_NodeClass_OnCreate)
{
	TEST_ASSERT_EQUAL_PTR(NodeClass, classOf(classOf(myTest_Node)));
}

TEST(nodes, sizeOf_NodeClass_is_108)
{
	TEST_ASSERT_EQUAL(108, myTest_Node->_.class->_.class->sizeOfThisClass);
}


//****  delete/Node_dtor  ****************

TEST(nodes, delete_returns_myTestNodeOnValidPtr)
{
	struct Node * localNode = new(Node);
	TEST_ASSERT_EQUAL_PTR(localNode, delete(localNode));
	localNode = NULL;
}

TEST(nodes, delete_safeDelete_returnsNullOnSuccess)
{
	struct Node * localNode = new(Node);
	localNode = safeDelete(localNode);
	TEST_ASSERT_EQUAL_PTR(NULL, localNode);
	localNode = NULL;
}

//****  Node_copy  ********************

TEST(nodes, copy_returnsSelfOnSuccess)
{
	struct Node * copyFromNode = new(Node);
	TEST_ASSERT_EQUAL(myTest_Node, copy(myTest_Node, copyFromNode));
	copyFromNode = safeDelete(copyFromNode);
}

TEST(nodes, copy_AllItemsCopiedToSelf)
{
	struct Node * copyFromNode = new(Node);
	setID           (copyFromNode, 45);
	setType         (copyFromNode, 451);
	setState        (copyFromNode, STATE_ON);
	setValue        (copyFromNode, 678);
	setRunCount     (copyFromNode, 800);
	setLinkedNode   (copyFromNode, NULL);
	setLinkedMessage(copyFromNode, NULL);

	copy(myTest_Node, copyFromNode);

	// ID is not copied
	//TEST_ASSERT_EQUAL(45,        getID      (myTest_Node) );
	TEST_ASSERT_EQUAL(451,       getType    (myTest_Node) );
	TEST_ASSERT_EQUAL(STATE_ON,  getState   (myTest_Node) );
	TEST_ASSERT_EQUAL(678,       getValue   (myTest_Node) );
	// runCount is not copied
	//TEST_ASSERT_EQUAL(800,       getRunCount(myTest_Node) );
	TEST_ASSERT_EQUAL(NULL,      getLinkedNode(myTest_Node) );
	TEST_ASSERT_EQUAL(NULL,      getLinkedMessage(myTest_Node) );
	copyFromNode = safeDelete(copyFromNode);
}

TEST(nodes, copy_returnsNullOnNullSelf)
{
	struct Node * copyFromNode = new(Node);
	setID(copyFromNode, 45);
	setState(copyFromNode, STATE_ON);
	TEST_ASSERT_EQUAL(NULL, copy(NULL,         copyFromNode));
	TEST_ASSERT_EQUAL(NULL, copy(copyFromNode, NULL        ));
	copyFromNode = safeDelete(copyFromNode);
}

TEST(nodes, copy_returnsNullOnNullTarget)
{
	TEST_ASSERT_EQUAL(NULL, copy(myTest_Node, NULL));
}

TEST(nodes, copy_createsAndAddsLocalMessage)
{
	struct Node * localMessage = new(Node);
	setValue(localMessage, 199);

	struct Node * copyFromNode = new(Node);
	setLinkedNode   (copyFromNode, NULL);
	setLinkedMessage(copyFromNode, localMessage);

	copy(myTest_Node, copyFromNode);

	//  the message pointers will not match since they are separate objects
	//  the values of the original message should be copied to the new linked message
	TEST_ASSERT_EQUAL(getValue(localMessage), getValue(getLinkedMessage(myTest_Node)) );
	//localMessage = safeDelete(localMessage);
	copyFromNode = safeDelete(copyFromNode);
}

TEST(nodes, copy_createsAndAddsLinkedNode)
{
	struct Node * localLinkedNode = new(Node);
	setValue(localLinkedNode, 199);

	struct Node * copyFromNode = new(Node);
	setValue     (copyFromNode, 678);
	setLinkedNode(copyFromNode, localLinkedNode);

	copy(myTest_Node, copyFromNode);

	TEST_ASSERT_EQUAL(678,          getValue        (myTest_Node) );

	//  the linked node pointers will not match since they are separate objects
	//  the values of the original node should be copied to the new linked node
	TEST_ASSERT_EQUAL(getValue(localLinkedNode), getValue(getLinkedNode(myTest_Node)) );
	//localLinkedNode = safeDelete(localLinkedNode);
	copyFromNode    = safeDelete(copyFromNode);
}

TEST(nodes, copy_AddsChainOfLinkedNodes)
{
	struct Node * localLinkedNode1 = new(Node);    // last node
	setValue(localLinkedNode1, 1);

	struct Node * localLinkedNode2 = new(Node);    // middle node
	setValue(localLinkedNode2, 2);
	setLinkedNode(localLinkedNode2, localLinkedNode1);

	struct Node * copyFromNode = new(Node);        // root node
	setValue        (copyFromNode, 3);
	setLinkedNode(copyFromNode, localLinkedNode2);
	copy(myTest_Node, copyFromNode);

	TEST_ASSERT_EQUAL(3,          getValue        (myTest_Node) ); // root test

	//  the linked node pointers will not match since they are separate objects
	//  the values of the original node should be copied to the new linked node
	TEST_ASSERT_EQUAL(2, getValue(getLinkedNode(myTest_Node)) );  // middle test
	TEST_ASSERT_EQUAL(1, getValue(getLinkedNode(getLinkedNode(myTest_Node))) );
	copyFromNode     = safeDelete(copyFromNode);
}

//****  Node_equal  ********************

TEST(nodes, myTest_Node_IsEqualTo_myTest_Node)
{
	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_Node, myTest_Node));
}

TEST(nodes, equal_returnsUnequalWithUnequalIDs)
{
	//WARNING:  nodes and messages should have unique IDs
	struct Node * otherNode = new(Node);
	setID(myTest_Node, 4);
	setID(otherNode,   5);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Node, otherNode));
	otherNode = safeDelete(otherNode);
}

TEST(nodes, equal_UnequalTypesReturn_Unequal)
{
	struct Node * otherNode = new(Node);
	setType(myTest_Node, 14);
	setType(otherNode,   15);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Node, otherNode));
	otherNode = safeDelete(otherNode);
}

TEST(nodes, equal_UnequalStatesReturn_Unequal)
{
	struct Node * otherNode = new(Node);
	setState(myTest_Node, STATE_ON);
	setState(otherNode,   STATE_OFF);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Node, otherNode));
	otherNode = safeDelete(otherNode);
}

TEST(nodes, equal_UnequalValuesReturn_Unequal)
{
	struct Node * otherNode = new(Node);
	setValue(myTest_Node, 457);
	setValue(otherNode,   460);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Node, otherNode));
	otherNode = safeDelete(otherNode);
}

TEST(nodes, equal_UnequalRunCountsReturn_Unequal)
{
	// runCount is a dynamic data value and this test has been disabled
	struct Node * otherNode = new(Node);
	setRunCount(myTest_Node, 459);
	setRunCount(otherNode,   461);
	//TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Node, otherNode));
	otherNode = safeDelete(otherNode);
}

TEST(nodes, equal_UnequalLinkedNodeReturn_Equal)
{
	struct Node * otherNode = new(Node);
	struct Node * link1 = new(Node);
	struct Node * link2 = new(Node);

	setLinkedNode(myTest_Node, link1);
	setLinkedNode(otherNode,   link2);
	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_Node, otherNode));
	otherNode = safeDelete(otherNode);
}

TEST(nodes, equal_UnequalLinkedMessageReturn_Equal)
{
	struct Node * otherNode = new(Node);
	struct Node * link1 = new(Node);
	struct Node * link2 = new(Node);

	setLinkedMessage(myTest_Node, link1);
	setLinkedMessage(otherNode,   link2);
	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_Node, otherNode));
	otherNode = safeDelete(otherNode);
}

TEST(nodes, equal_myTest_Node_NotEqual_Null)
{
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Node, NULL));
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(NULL, myTest_Node));
}

/***********  setID/getID  **************/

TEST(nodes, Node_setIDto21)
{
	TEST_ASSERT_EQUAL(21, Node_setID(myTest_Node, 21));
}

TEST(nodes, class_setIDto21)
{
	const struct NodeClass * class = classOf( cast(Node, myTest_Node) );
	TEST_ASSERT_EQUAL(21, class->setID(myTest_Node, 21));
}

TEST(nodes, setID_to21)
{
	setID(myTest_Node, -1);
	TEST_ASSERT_EQUAL(-1, getID(myTest_Node));

	setID(myTest_Node, 21);
	TEST_ASSERT_EQUAL(21, getID(myTest_Node));
}

/***********  setID/getID  **************/

TEST(nodes, getState_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(STATE_UNKNOWN, getState(myTest_Node));
}

TEST(nodes, setState_ON )
{
	setState(myTest_Node, STATE_OFF);
	TEST_ASSERT_EQUAL(STATE_ON, setState(myTest_Node, STATE_ON));
}

TEST(nodes, setState_OFF)
{
	setState(myTest_Node, STATE_ON);
	TEST_ASSERT_EQUAL(STATE_OFF, setState(myTest_Node, STATE_OFF));
}

/***********  setType/getType  **************/

TEST(nodes, nodeType_UNKNOWN_onCreate)
{
	TEST_ASSERT_EQUAL(TYPE_UNKNOWN, getType(myTest_Node));
}

TEST(nodes, setType_ToSpecificType)
{
	setType(myTest_Node, 236);
	TEST_ASSERT_EQUAL(236, myTest_Node->nodeType);
}

TEST(nodes, setType_ReturnsTypeOnSuccess)
{
	TEST_ASSERT_EQUAL(456, setType(myTest_Node, 456));
}

TEST(nodes, getType_SuccessfullyReturnsType)
{
	setType(myTest_Node, 237);
	TEST_ASSERT_EQUAL(237, getType(myTest_Node));
}

/***********  setValue/getValue  **************/

TEST(nodes, nodeValue_UNKNOWN_onCreate)
{
	TEST_ASSERT_EQUAL(VALUE_UNKNOWN, getValue(myTest_Node));
}

TEST(nodes, setValue_ToSpecificValue)
{
	setValue(myTest_Node, 236);
	TEST_ASSERT_EQUAL(236, myTest_Node->nodeValue);
}

TEST(nodes, getValue_ReturnsValueOnSuccess)
{
	TEST_ASSERT_EQUAL(456, setValue(myTest_Node, 456));
}

TEST(nodes, getValue_SuccessfullyReturnsValue)
{
	setValue(myTest_Node, 237);
	TEST_ASSERT_EQUAL(237, getValue(myTest_Node));
}


/***********  setRunCount/getRunCount  **************/

TEST(nodes, runCount_UNKNOWN_onCreate)
{
	TEST_ASSERT_EQUAL(RUN_COUNT_UNKNOWN, getRunCount(myTest_Node));
}

TEST(nodes, setRunCount_ToSpecificValue)
{
	setRunCount(myTest_Node, 236);
	TEST_ASSERT_EQUAL(236, myTest_Node->runCount);
}

TEST(nodes, getRunCount_ReturnsValueOnSuccess)
{
	TEST_ASSERT_EQUAL(456, setRunCount(myTest_Node, 456));
}

TEST(nodes, getRunCount_SuccessfullyReturnsValue)
{
	setRunCount(myTest_Node, 237);
	TEST_ASSERT_EQUAL(237, getRunCount(myTest_Node));
}

/*******  incrementRunCount/decrementRunCount  *********/

TEST(nodes, incrementRunCount_setValueOneOnFirstCall)
{
	incrementRunCount(myTest_Node);
	TEST_ASSERT_EQUAL(1, getRunCount(myTest_Node));
}

TEST(nodes, incrementRunCount_setValueTwoOnSecondCall)
{
	incrementRunCount(myTest_Node);
	incrementRunCount(myTest_Node);
	TEST_ASSERT_EQUAL(2, getRunCount(myTest_Node));
}

TEST(nodes, decrementRunCount_returnsZeroWithOriginalOfOne)
{
	incrementRunCount(myTest_Node);
	TEST_ASSERT_EQUAL(0, decrementRunCount(myTest_Node));
}

TEST(nodes, decrementRunCount_returnsZeroIfOriginalyZero)
{
	incrementRunCount(myTest_Node);
	decrementRunCount(myTest_Node);
	TEST_ASSERT_EQUAL(0, decrementRunCount(myTest_Node));
}

/*******  setNodeLink/getNodeLink  *********/

TEST(nodes, nodeLink_UNKNOWN_onCreate)
{
	TEST_ASSERT_EQUAL_PTR(NULL, myTest_Node->linkedNode);
}

TEST(nodes, setNodeLink_ToSpecificValue)
{
	struct Node * localLinkNode = new(Node);
	setLinkedNode(myTest_Node, localLinkNode );
	TEST_ASSERT_EQUAL_PTR( localLinkNode, myTest_Node->linkedNode );
}

TEST(nodes, getNodeLink_ReturnsValueOnSuccess)
{
	struct Node * localLinkNode = new(Node);
	TEST_ASSERT_EQUAL_PTR( localLinkNode, setLinkedNode(myTest_Node, localLinkNode) );
}

TEST(nodes, getNodeLink_SuccessfullyReturnsValue)
{
	struct Node * localLinkNode = new(Node);
	setLinkedNode(myTest_Node, localLinkNode);
	TEST_ASSERT_EQUAL_PTR( localLinkNode, getLinkedNode(myTest_Node) );
}

/*******  getPtrToSelfIfReadyToRun  *********/

TEST(nodes, incrementRunCount_returnsNullOnUnknown)
{
	TEST_ASSERT_EQUAL_PTR(NULL, getPtrToSelfIfReadyToRun(myTest_Node));
}

TEST(nodes, incrementRunCount_returnsNullOnZero)
{
	setRunCount(myTest_Node, 0);
	TEST_ASSERT_EQUAL_PTR(NULL, getPtrToSelfIfReadyToRun(myTest_Node));
}

TEST(nodes, incrementRunCount_returnsSelfOnOne)
{
	incrementRunCount(myTest_Node);
	TEST_ASSERT_EQUAL_PTR(myTest_Node, Node_getPtrToSelfIfReadyToRun(myTest_Node));
}

TEST(nodes, incrementRunCount_returnsSelfOnTwo)
{
	incrementRunCount(myTest_Node);
	incrementRunCount(myTest_Node);
	TEST_ASSERT_EQUAL_PTR(myTest_Node, getPtrToSelfIfReadyToRun(myTest_Node));
}

TEST(nodes, incrementRunCount_returnsSelfIfLinkedNotReady)
{
	struct Node * localLinkedNode = new(Node);
	setLinkedNode(myTest_Node, localLinkedNode);
	incrementRunCount(myTest_Node);
	TEST_ASSERT_EQUAL_PTR(myTest_Node, getPtrToSelfIfReadyToRun(myTest_Node));
	//localLinkedNode = safeDelete(localLinkedNode);
}

TEST(nodes, incrementRunCount_returnsLinkedOnReady)
{
	struct Node * localLinkedNode = new(Node);
	setLinkedNode(myTest_Node, localLinkedNode);
	incrementRunCount(localLinkedNode);
	TEST_ASSERT_EQUAL_PTR(localLinkedNode, getPtrToSelfIfReadyToRun(myTest_Node));
	//localLinkedNode = safeDelete(localLinkedNode);
}

TEST(nodes, incrementRunCount_returnsLinkedIfBothReady)
{
	struct Node * localLinkedNode = new(Node);
	setLinkedNode(myTest_Node, localLinkedNode);
	incrementRunCount(localLinkedNode);
	incrementRunCount(myTest_Node);
	TEST_ASSERT_EQUAL_PTR(localLinkedNode, getPtrToSelfIfReadyToRun(myTest_Node));
	//localLinkedNode = safeDelete(localLinkedNode);
}

TEST(nodes, incrementRunCount_returnsMiddleNodeOnReady)
{
	struct Node * farthestLinkedNode = new(Node);
	struct Node * middleLinkedNode = new(Node);
	setLinkedNode(middleLinkedNode, farthestLinkedNode);
	setLinkedNode(myTest_Node, middleLinkedNode);
	incrementRunCount(middleLinkedNode);
	TEST_ASSERT_EQUAL_PTR(middleLinkedNode, getPtrToSelfIfReadyToRun(myTest_Node));
}

TEST(nodes, incrementRunCount_returnsNullAfterAcknowlwdge)
{
	struct Node * farthestLinkedNode = new(Node);
	struct Node * middleLinkedNode = new(Node);
	setLinkedNode(middleLinkedNode, farthestLinkedNode);
	setLinkedNode(myTest_Node, middleLinkedNode);
	incrementRunCount(middleLinkedNode);
	decrementRunCount( getPtrToSelfIfReadyToRun(myTest_Node) );
	TEST_ASSERT_EQUAL_PTR(NULL, getPtrToSelfIfReadyToRun(myTest_Node));
}

/*******  setLinkedMessage/getLinkedMessage  *********/

TEST(nodes, linkedMessage_UNKNOWN_onCreate)
{
	TEST_ASSERT_EQUAL_PTR(NULL, myTest_Node->linkedMessage);
}

TEST(nodes, setLinkedMessage_ToSpecificValue)
{
	struct Node * messageNode = new(Node);
	setLinkedMessage(myTest_Node, messageNode );
	TEST_ASSERT_EQUAL_PTR( messageNode, myTest_Node->linkedMessage );
	messageNode = NULL;
}

TEST(nodes, getLinkedMessage_ReturnsValueOnSuccess)
{
	struct Node * messageNode = new(Node);
	TEST_ASSERT_EQUAL_PTR( messageNode, setLinkedMessage(myTest_Node, messageNode ) );
	messageNode = NULL;
}

TEST(nodes, getLinkedMessage_SuccessfullyReturnsValue)
{
	struct Node * messageNode = new(Node);
	setLinkedMessage(myTest_Node, messageNode );
	TEST_ASSERT_EQUAL_PTR( messageNode, getLinkedMessage(myTest_Node) );
	messageNode = NULL;
}

TEST(nodes, delete_SuccessfullyHandlesLinkedNode)
{
	struct Node * localLinkedNode1 = new(Node);           // last node
	setLinkedNode(myTest_Node, localLinkedNode1);

	TEST_ASSERT_EQUAL(myTest_Node, delete(myTest_Node) ); // root test
	myTest_Node = NULL;
}

TEST(nodes, delete_SuccessfullyHandlesChainedNodes)
{
	struct Node * localLinkedNode1 = new(Node);           // last node

	struct Node * localLinkedNode2 = new(Node);           // middle node
	setLinkedMessage(localLinkedNode2, localLinkedNode1);

	setLinkedNode(myTest_Node, localLinkedNode2);

	TEST_ASSERT_EQUAL(myTest_Node, delete(myTest_Node) ); // root test
	myTest_Node = NULL;
}

TEST(nodes, delete_SuccessfullyHandlesLinkedMessage)
{
	struct Node * localLinkedMessage = new(Node);         // last node
	setLinkedMessage(myTest_Node, localLinkedMessage);

	TEST_ASSERT_EQUAL(myTest_Node, delete(myTest_Node) ); // root test
	myTest_Node = NULL;
}


/***********  config  **************/

TEST(nodes, Config_copiesState)
{
	struct Node * master = new(Node);
	setState(master, STATE_ON);
	config(myTest_Node, master);
	TEST_ASSERT_EQUAL(STATE_ON, getState(myTest_Node));
	master = safeDelete(master);
}

/**************************************/
TEST(nodes, CanCall_Node_Init_MulitpleTimes)
{
	const void * originalNode = Node;
	const void * originalNodeClass = NodeClass;
	Node_init();
	Node_init();
	Node_init();

	// Verify that the class pointers are unchanged
	TEST_ASSERT_EQUAL_PTR(Node, originalNode);
	TEST_ASSERT_EQUAL_PTR(NodeClass, originalNodeClass);

	char outputFilename[] = "nodes_puto.txt";
	FILE * fp = fopen(outputFilename, "a");
	puto(myTest_Node, fp);
	fclose(fp);
}

TEST(nodes, memory_leak)
{
	// new structure location will move based on other processes
	// create and destroy a large number of objects
	// then look for the base address within an additional 10 new objects

	// get original object location
	struct Event * newItem = new(Node);
	struct Event * originalPointer = newItem;
	delete(newItem);

	// create and destroy a large number of objects to cause a memory leak
	int i;
	for (i = 0; i < 1000; i++) {
		newItem = new(Node);
		//printf("newItem: %p, i: %i\n", newItem, i);
		delete(newItem);
	}
	//printf("Original newItem: %p\n", originalPointer);
	// check for original object location within a small number of additional objects
	int originalPtrCount = 0;
	for (i = 0; i < 100; i++) {
		newItem = new(Node);
		//printf("newItem: %p, i: %i\n", newItem, i);
		delete(newItem);
		if (originalPointer == newItem) { originalPtrCount++; }
	}
	//printf("\nNodes-memory-leak-test, originalPtrCount: %i, out of %i trials\n", originalPtrCount, 100);
	TEST_ASSERT_TRUE(originalPtrCount);
	newItem = NULL;
}

