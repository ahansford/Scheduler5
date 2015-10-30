/*
 * nodes-test.c
 *
 *  Created on: July, 2015
 *      Author: Alan
 */


//#include "..\..\src\cross_compiler_defs.h"
#include "..\unity\unity_fixture.h"
//#include "..\unity\unity.h"
#include "..\..\src\mocks\RuntimeErrorStub.h"
#include "..\..\src\event_controller\event-controller.h"
#include "..\..\src\event_controller\event-controller-private.h"

/****************************************/
/*** Basic Object and Class Testing *****/

struct Event * myTest_Event;
struct Event * myTest_Event2;
struct Node *  myTest_Node;
struct Node *  myTest_Node2;
//struct Time *  myTest_Time;
struct Node *  myTest_Message;

int i;
int deleteIndex;

void EventController_deleteAllItems(void * _listPointer);

static void Event_controller_test_setTime(struct Time * _self,
					second_t      second,
		            minute_t      minute,
					hour_t        hour,
					hourMode_t	  hourMode,
					AmPm_t        AmPm,
					dayOfWeek_t   dayOfWeek );


/****************************************/
/*********** Start of Testing ***********/
TEST_GROUP(eventController);

TEST_SETUP(eventController)
{
	RuntimeErrorStub_Reset();
	Node_init();
	List_init();
	Event_init();
	Time_init();
	EventController_init(UPDATE_SECONDS);

	myTest_Event  = new(Event);
	myTest_Event2 = new(Event);

	myTest_Node   = new(Node);
	myTest_Node2  = new(Node);

	myTest_Message = new(Node);
}

TEST_TEAR_DOWN(eventController)
{
	myTest_Event  = safeDelete(myTest_Event);
	myTest_Event2 = safeDelete(myTest_Event2);

	myTest_Node  = safeDelete(myTest_Node);
	myTest_Node2 = safeDelete(myTest_Node2);

	myTest_Message = safeDelete(myTest_Message);

	EventController_removeAllAndReset();
	//EventController_destroy();
	RuntimeErrorStub_Reset();
}

/**************************************/
/**************  New  *****************/
/**/

TEST(eventController, create1)
{

}

TEST(eventController, create2)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  myEventList);
}



TEST(eventController, myEventList_IsNotNullAfterInit)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  myEventList);
}

TEST(eventController, myNodeList_IsNotNullAfterInit)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  myNodeList);
}

TEST(eventController, myTime_IsNotNullAfterInit)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  myTime);
}



/**************************************/
/*****  EventController_addNode  ******/
/**/
TEST(eventController, eventController_addNode_addsNode)
{
	EventController_addNode(myTest_Node);
	TEST_ASSERT_EQUAL(myTest_Node,  incrementGet(myNodeList));
	myTest_Node = NULL;
}

TEST(eventController, eventController_addNode_returnsNodePtr)
{
	TEST_ASSERT_EQUAL(myTest_Node,  EventController_addNode(myTest_Node));
	myTest_Node = NULL;
}

TEST(eventController, eventController_addNull_returnsNull)
{
	TEST_ASSERT_EQUAL(NULL,  EventController_addNode(NULL));
}

/**************************************/
/****  EventController_removeNode  ****/
/**/
TEST(eventController, eventController_removeNode_removesFirstNode)
{
	add(myNodeList, myTest_Node);
	add(myNodeList, myTest_Node2);
	EventController_removeNode(myTest_Node);
	TEST_ASSERT_EQUAL(myTest_Node2, incrementGet(myNodeList));
	myTest_Node  = NULL;
	myTest_Node2 = NULL;
}

TEST(eventController, eventController_removeNode_removesSecondNode)
{
	add(myNodeList, myTest_Node);
	add(myNodeList, myTest_Node2);
	EventController_removeNode(myTest_Node2);
	TEST_ASSERT_EQUAL(myTest_Node, incrementGet(myNodeList));
	myTest_Node  = NULL;
	myTest_Node2 = NULL;
}

TEST(eventController, eventController_removeNode_returnsNodePtr)
{
	add(myNodeList, myTest_Node);
	add(myNodeList, myTest_Node2);
	TEST_ASSERT_EQUAL(myTest_Node,  EventController_removeNode(myTest_Node));
	myTest_Node  = NULL;
	myTest_Node2 = NULL;
}

TEST(eventController, eventController_removeNull_returnsNull)
{
	add(myNodeList, myTest_Node);
	add(myNodeList, myTest_Node2);
	TEST_ASSERT_EQUAL(NULL,  EventController_removeNode(NULL));
	myTest_Node  = NULL;
	myTest_Node2 = NULL;
}

/********************************************/
/****  EventController_incrementGetNode  ****/
/**/
TEST(eventController, eventController_incrementGetNode_FirstNode)
{
	add(myNodeList, myTest_Node);
	TEST_ASSERT_EQUAL(myTest_Node,  EventController_incrementGetNode());
	myTest_Node  = NULL;
}

TEST(eventController, eventController_incrementGetNode_SecondNode)
{
	add(myNodeList, myTest_Node);
	add(myNodeList, myTest_Node2);
	resetIndex(myNodeList);
	// resetIndex() will point at beginIndex ...
	// eventController_incrementGetNode() will initially point at beginIndex + 1
	TEST_ASSERT_EQUAL(myTest_Node2, EventController_incrementGetNode());
	TEST_ASSERT_EQUAL(myTest_Node,  EventController_incrementGetNode());
	myTest_Node  = NULL;
	myTest_Node2 = NULL;
}

TEST(eventController, eventController_incrementGetNode_NullOnEmpty)
{
	TEST_ASSERT_EQUAL(NULL,  EventController_incrementGetNode());
}

/***************************************/
/*****  EventController_addEvent  ******/

TEST(eventController, eventController_addEvent_addsEvent)
{
	EventController_addEvent(myTest_Event);
	TEST_ASSERT_EQUAL(myTest_Event,  incrementGet(myEventList));
	myTest_Event = NULL;  // prevents double free() of object
}

TEST(eventController, eventController_addEvent_returnsEventPtr)
{
	TEST_ASSERT_EQUAL(myTest_Event,  EventController_addEvent(myTest_Event));
	myTest_Event = NULL;  // prevents double free() of object
}

TEST(eventController, eventController_addEvent_NullreturnsNull)
{
	TEST_ASSERT_EQUAL(NULL,  EventController_addEvent(NULL));
}

/***************************************/
/****  EventController_removeEvent  ****/

TEST(eventController, eventController_removeEvent_removesFirstEvent)
{
	add(myEventList, myTest_Event);
	add(myEventList, myTest_Event2);
	EventController_removeEvent(myTest_Event);
	resetIndex(myEventList);
	TEST_ASSERT_EQUAL(myTest_Event2, incrementGet(myEventList));
	myTest_Event  = NULL;  // prevents double free() of object
	myTest_Event2 = NULL;  // prevents double free() of object
}

TEST(eventController, eventController_removeEvent_removesSecondEvent)
{
	add(myEventList, myTest_Event);
	add(myEventList, myTest_Event2);
	EventController_removeEvent(myTest_Event2);
	TEST_ASSERT_EQUAL(myTest_Event, incrementGet(myEventList));
	myTest_Event  = NULL;  // prevents double free() of object
	myTest_Event2 = NULL;  // prevents double free() of object
}

TEST(eventController, eventController_removeEvent_returnsEventPtr)
{
	add(myEventList, myTest_Event);
	add(myEventList, myTest_Event2);
	TEST_ASSERT_EQUAL(myTest_Event,  EventController_removeEvent(myTest_Event));
	myTest_Event  = NULL;  // prevents double free() of object double delete
	myTest_Event2 = NULL;  // prevents double free() of object
}

TEST(eventController, eventController_removeEvent_NullreturnsNull)
{
	add(myEventList, myTest_Event);
	add(myEventList, myTest_Event2);
	// TODO:  Issue with false values
	TEST_ASSERT_EQUAL_PTR((void *)(NULL),  EventController_removeEvent(NULL));
	myTest_Event  = NULL;  // prevents double free() of object
	myTest_Event2 = NULL;  // prevents double free() of object
}

/********************************************/
/****  EventController_incrementGetEvent  ****/
/**/
TEST(eventController, eventController_incrementGetEvent_FirstEvent)
{
	add(myEventList, myTest_Event);
	TEST_ASSERT_EQUAL(myTest_Event,  EventController_incrementGetEvent() );
	myTest_Event  = NULL;  // prevents double free() of object
}

TEST(eventController, eventController_incrementGetEvent_SecondEvent)
{
	add(myEventList, myTest_Event);
	add(myEventList, myTest_Event2);
	resetIndex(myNodeList);
	TEST_ASSERT_EQUAL(myTest_Event, EventController_incrementGetEvent() );
	TEST_ASSERT_EQUAL(myTest_Event2,  EventController_incrementGetEvent() );
	myTest_Event  = NULL;  // prevents double free() of object
	myTest_Event2 = NULL;  // prevents double free() of object
}

TEST(eventController, eventController_incrementGetEvent_NullOnEmpty)
{
	TEST_ASSERT_EQUAL(NULL,  EventController_incrementGetEvent());
}

/********************************************/
/**************  executeEvent  **************/


TEST(eventController, eventController_executeEvent_returnsNodePointer)
{
	setID(myTest_Event->message, 51);
	setValue(myTest_Event->message, 25);
	add(myEventList, myTest_Event);

	setID(myTest_Node, 51);
	add(myNodeList, myTest_Node);

	TEST_ASSERT_EQUAL(myTest_Node, executeEvent(myTest_Event) );
	myTest_Event  = NULL;  // prevents double free() of object
	myTest_Node   = NULL;  // prevents double free() of object
}

TEST(eventController, eventController_executeEvent_copiesEventMessageToNode)
{
	setID(myTest_Event->message, 51);
	setValue(myTest_Event->message, 25);
	add(myEventList, myTest_Event);

	setID(myTest_Node, 51);
	add(myNodeList, myTest_Node);

	executeEvent(myTest_Event);

	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_Node, myTest_Event->message) );
	myTest_Event  = NULL;  // prevents double free() of object
	myTest_Node   = NULL;  // prevents double free() of object
}

TEST(eventController, eventController_executeEvent_returnsNullOnNullEvent)
{
	setID(myTest_Node, 51);
	add(myNodeList, myTest_Node);

	TEST_ASSERT_EQUAL(NULL, executeEvent(NULL) );
	myTest_Node   = NULL;  // prevents double free() of object
}

TEST(eventController, eventController_executeEvent_returnsNullOnMissingNode)
{
	setID(myTest_Event->message, 51);
	setValue(myTest_Event->message, 25);
	add(myEventList, myTest_Event);

	TEST_ASSERT_EQUAL(NULL, executeEvent(myTest_Event) );
	myTest_Event  = NULL;  // prevents double free() of object
}

TEST(eventController, eventController_executeEvent_returnsNullOnMissingNodeID)
{
	setID(myTest_Event->message, 51);
	setValue(myTest_Event->message, 25);
	add(myEventList, myTest_Event);

	setID(myTest_Node, 44);              // wrong ID
	add(myNodeList, myTest_Node);

	TEST_ASSERT_EQUAL(NULL, executeEvent(myTest_Event) );
	myTest_Event  = NULL;  // prevents double free() of object
	myTest_Node   = NULL;  // prevents double free() of object
}

TEST(eventController, eventController_executeEvent_decrementsRunCountOnSuccess)
{
	setID(myTest_Event->message, 51);
	setValue(myTest_Event->message, 25);
	setRunCount(myTest_Event->message, 1);
	add(myEventList, myTest_Event);

	setID(myTest_Node, 51);
	add(myNodeList, myTest_Node);

	executeEvent(myTest_Event);

	TEST_ASSERT_EQUAL(0, getRunCount(myTest_Event->message) );
	myTest_Event  = NULL;  // prevents double free() of object
	myTest_Node   = NULL;  // prevents double free() of object
}

TEST(eventController, eventController_executeEvent_RunCountUnchangedOnFailure)
{
	setID(myTest_Event->message, 51);
	setValue(myTest_Event->message, 25);
	setRunCount(myTest_Event->message, 5);
	add(myEventList, myTest_Event);

	setID(myTest_Node, 44);       // wrong nodeID
	add(myNodeList, myTest_Node);

	executeEvent(myTest_Event);

	TEST_ASSERT_EQUAL(5, getRunCount(myTest_Event->message) );
	myTest_Event  = NULL;  // prevents double free() of object
	myTest_Node   = NULL;  // prevents double free() of object
}

/********************************************/
/*************  executeMessage  *************/


TEST(eventController, eventController_executeMessage_returnsTargetNodePointer)
{
	setID   (myTest_Message, 44);
	setValue(myTest_Message, 23);

	setID           (myTest_Node, 51);
	setRunCount     (myTest_Node, 1);
	setLinkedMessage(myTest_Node, myTest_Message);
	add(myNodeList, myTest_Node);

	setID(myTest_Node2, 44);
	add(myNodeList, myTest_Node2);

	TEST_ASSERT_EQUAL(myTest_Node2, executeMessage(myTest_Node) );
	myTest_Node  = NULL;  // prevents double free() of object
	myTest_Node2  = NULL;  // prevents double free() of object
	myTest_Message  = NULL;  // prevents double free() of object
}

TEST(eventController, eventController_executeMessage_copiesMessageToNode)
{
	setID   (myTest_Message, 44);
	setValue(myTest_Message, 23);

	setID           (myTest_Node, 51);
	setRunCount     (myTest_Node, 1);
	setLinkedMessage(myTest_Node, myTest_Message);
	add(myNodeList, myTest_Node);

	setID(myTest_Node2, 44);
	add(myNodeList, myTest_Node2);

	executeMessage(myTest_Node);
	TEST_ASSERT_EQUAL(myTest_Message->nodeValue, myTest_Node2->nodeValue );
	myTest_Node  = NULL;  // prevents double free() of object
	myTest_Node2  = NULL;  // prevents double free() of object
	myTest_Message  = NULL;  // prevents double free() of object
}

TEST(eventController, eventController_executeMessage_returnsNullOnNullMessage)
{
	setID           (myTest_Node, 51);
	setRunCount     (myTest_Node, 1);
	setLinkedMessage(myTest_Node, NULL);
	add(myNodeList, myTest_Node);

	setID(myTest_Node2, 44);
	add(myNodeList, myTest_Node2);

	TEST_ASSERT_EQUAL(NULL, executeMessage(NULL) );
	myTest_Node  = NULL;  // prevents double free() of object
	myTest_Node2  = NULL;  // prevents double free() of object
}

TEST(eventController, eventController_executeMessage_returnsNullOnMissingNode)
{
	setID   (myTest_Message, 44);
	setValue(myTest_Message, 23);

	setID           (myTest_Node, 51);
	setRunCount     (myTest_Node, 1);
	setLinkedMessage(myTest_Node, myTest_Message);
	add(myNodeList, myTest_Node);

	TEST_ASSERT_EQUAL(NULL, executeMessage(myTest_Node) );
	myTest_Node  = NULL;  // prevents double free() of object
	myTest_Message  = NULL;  // prevents double free() of object
}

TEST(eventController, eventController_executeMessage_returnsNullOnMissingNodeID)
{
	setID   (myTest_Message, 44);
	setValue(myTest_Message, 23);

	setID           (myTest_Node, 51);
	setRunCount     (myTest_Node, 1);
	setLinkedMessage(myTest_Node, myTest_Message);
	add(myNodeList, myTest_Node);

	setID(myTest_Node2, 65);          // wrong nodeID
	add(myNodeList, myTest_Node2);

	TEST_ASSERT_EQUAL(NULL, executeMessage(myTest_Node) );
	myTest_Node  = NULL;  // prevents double free() of object
	myTest_Node2  = NULL;  // prevents double free() of object
	myTest_Message  = NULL;  // prevents double free() of object
}

TEST(eventController, eventController_executeMessage_decrementsRunCountOnSuccess)
{
	setID   (myTest_Message, 44);
	setValue(myTest_Message, 23);

	setID           (myTest_Node, 51);
	setRunCount     (myTest_Node, 1);
	setLinkedMessage(myTest_Node, myTest_Message);
	add(myNodeList, myTest_Node);

	setID(myTest_Node2, 44);
	add(myNodeList, myTest_Node2);

	executeMessage(myTest_Node);

	TEST_ASSERT_EQUAL(0, getRunCount(myTest_Node) );
	myTest_Node  = NULL;  // prevents double free() of object
	myTest_Node2  = NULL;  // prevents double free() of object
	myTest_Message  = NULL;  // prevents double free() of object
}

TEST(eventController, eventController_executeMessage_RunCountUnchangedOnFailure)
{
	setID   (myTest_Message, 44);
	setValue(myTest_Message, 23);

	setID           (myTest_Node, 51);
	setRunCount     (myTest_Node, 6);
	setLinkedMessage(myTest_Node, myTest_Message);
	add(myNodeList, myTest_Node);

	// target node
	setID(myTest_Node2, 65);          // wrong nodeID
	add(myNodeList, myTest_Node2);

	executeMessage(myTest_Node);

	TEST_ASSERT_EQUAL(6, getRunCount(myTest_Node) );
	myTest_Node  = NULL;  // prevents double free() of object
	myTest_Node2  = NULL;  // prevents double free() of object
	myTest_Message  = NULL;  // prevents double free() of object
}

/********************************************/
/*******  processAllReadyNodeMessages  ******/


TEST(eventController, processAllReadyNodeMessages_returnsOneAfterProcessingMessage)
{
	setID   (myTest_Message, 44);
	setValue(myTest_Message, 23);

	setID           (myTest_Node, 51);
	setRunCount     (myTest_Node, 1);
	setLinkedMessage(myTest_Node, myTest_Message);
	add(myNodeList, myTest_Node);

	// target node
	setID(myTest_Node2, 44);
	add(myNodeList, myTest_Node2);

	TEST_ASSERT_EQUAL(1, processAllReadyNodeMessages() );
	myTest_Node  = NULL;  // prevents double free() of object
	myTest_Node2  = NULL;  // prevents double free() of object
	myTest_Message  = NULL;  // prevents double free() of object
}

TEST(eventController, processAllReadyNodeMessages_copiesMessageToNode)
{
	setID   (myTest_Message, 44);
	setValue(myTest_Message, 23);

	setID           (myTest_Node, 51);
	setRunCount     (myTest_Node, 1);
	setLinkedMessage(myTest_Node, myTest_Message);
	add(myNodeList, myTest_Node);

	// target node
	setID(myTest_Node2, 44);
	add(myNodeList, myTest_Node2);

	processAllReadyNodeMessages();
	TEST_ASSERT_EQUAL(myTest_Message->nodeValue, myTest_Node2->nodeValue);
	myTest_Node  = NULL;  // prevents double free() of object
	myTest_Node2  = NULL;  // prevents double free() of object
	myTest_Message  = NULL;  // prevents double free() of object
}

TEST(eventController, processAllReadyNodeMessages_returnsZeroRunsIfNothingReady)
{
	setID   (myTest_Message, 44);
	setValue(myTest_Message, 23);

	setID           (myTest_Node, 51);
	setRunCount     (myTest_Node, 0);                // nothing ready
	setLinkedMessage(myTest_Node, myTest_Message);
	add(myNodeList, myTest_Node);

	// target node
	setID(myTest_Node2, 44);
	add(myNodeList, myTest_Node2);

	TEST_ASSERT_EQUAL(0, processAllReadyNodeMessages() );
	myTest_Node  = NULL;  // prevents double free() of object
	myTest_Node2  = NULL;  // prevents double free() of object
	myTest_Message  = NULL;  // prevents double free() of object
}

TEST(eventController, processAllReadyNodeMessages_returnsZeroRunsTargetNodeMissing)
{
	setID   (myTest_Message, 44);
	setValue(myTest_Message, 23);

	setID           (myTest_Node, 51);
	setRunCount     (myTest_Node, 1);
	setLinkedMessage(myTest_Node, myTest_Message);
	add(myNodeList, myTest_Node);

	// target node missing

	TEST_ASSERT_EQUAL(0, processAllReadyNodeMessages() );
	myTest_Node  = NULL;  // prevents double free() of object
	myTest_Message  = NULL;  // prevents double free() of object
}

TEST(eventController, processAllReadyNodeMessages_returnsZeroRunsTargetMessageMissing)
{
	// missing message

	// primary Node
	setID           (myTest_Node, 51);
	setRunCount     (myTest_Node, 1);
	setLinkedMessage(myTest_Node, myTest_Message);
	add(myNodeList, myTest_Node);

	// target node
	setID(myTest_Node2, 65);
	add(myNodeList, myTest_Node2);

	TEST_ASSERT_EQUAL(0, processAllReadyNodeMessages() );
	myTest_Node  = NULL;  // prevents double free() of object
	myTest_Node2  = NULL;  // prevents double free() of object
	myTest_Message  = NULL;  // prevents double free() of object
}

TEST(eventController, processAllReadyNodeMessages_returnsZeroOnMissingNodeID)
{
	setID   (myTest_Message, 44);
	setValue(myTest_Message, 23);

	setID           (myTest_Node, 51);
	setRunCount     (myTest_Node, 1);
	setLinkedMessage(myTest_Node, myTest_Message);
	add(myNodeList, myTest_Node);

	// target node
	setID(myTest_Node2, 65);        // wrong Node
	add(myNodeList, myTest_Node2);

	TEST_ASSERT_EQUAL(0, processAllReadyNodeMessages() );
	myTest_Node  = NULL;  // prevents double free() of object
	myTest_Node2  = NULL;  // prevents double free() of object
	myTest_Message  = NULL;  // prevents double free() of object
}

/********************************************/
/********  processAllReadyNodeEvents  *******/


TEST(eventController, processAllReadyEvents_returnsOneAfterProcessingEvents)
{
	// event
	setID      (myTest_Event->message, 44);
	setValue   (myTest_Event->message, 23);
	setRunCount(myTest_Event->message, 1);
	add(myEventList, myTest_Event);

	// target node
	setID   (myTest_Node, 44);
	setValue(myTest_Node, -5);
	add(myNodeList, myTest_Node);

	TEST_ASSERT_EQUAL(1, processAllReadyEvents() );
	myTest_Event  = NULL;  // prevents double free() of object
	myTest_Node   = NULL;  // prevents double free() of object

}

TEST(eventController, processAllReadyEvents_copiesMessageToNode)
{
	// event
	setID      (myTest_Event->message, 44);
	setValue   (myTest_Event->message, 23);
	setRunCount(myTest_Event->message, 1);
	add(myEventList, myTest_Event);

	// target node
	setID   (myTest_Node, 44);
	setValue(myTest_Node, -5);
	add(myNodeList, myTest_Node);

	processAllReadyEvents();
	TEST_ASSERT_EQUAL(myTest_Event->message->nodeValue, myTest_Node->nodeValue);
	myTest_Event  = NULL;  // prevents double free() of object
	myTest_Node   = NULL;  // prevents double free() of object
}

TEST(eventController, processAllReadyEvents_returnsZeroIfNothingReady)
{
	// event
	setID      (myTest_Event->message, 44);
	setValue   (myTest_Event->message, 23);
	setRunCount(myTest_Event->message, 0);  // nothing ready
	add(myEventList, myTest_Event);

	// target node
	setID   (myTest_Node, 44);
	setValue(myTest_Node, -5);
	add(myNodeList, myTest_Node);

	TEST_ASSERT_EQUAL(0, processAllReadyEvents() );
	myTest_Event  = NULL;  // prevents double free() of object
	myTest_Node   = NULL;  // prevents double free() of object
}

TEST(eventController, processAllReadyEvents_returnsZeroOnTargetNodeMissing)
{
	// event
	setID      (myTest_Event->message, 44);
	setValue   (myTest_Event->message, 23);
	setRunCount(myTest_Event->message, 1);
	add(myEventList, myTest_Event);

	// target node missing

	TEST_ASSERT_EQUAL(0, processAllReadyEvents() );
	myTest_Event  = NULL;  // prevents double free() of object
}

TEST(eventController, processAllReadyEvents_returnsZeroOnNoEventMissing)
{
	// event missing

	// target node
	setID   (myTest_Node, 44);
	setValue(myTest_Node, -5);
	add(myNodeList, myTest_Node);

	TEST_ASSERT_EQUAL(0, processAllReadyEvents() );
	myTest_Node   = NULL;  // prevents double free() of object
}

TEST(eventController, processAllReadyEvents_returnsZeroOnMissingNodeID)
{
	// event
	setID      (myTest_Event->message, 44);
	setValue   (myTest_Event->message, 23);
	setRunCount(myTest_Event->message, 1);
	add(myEventList, myTest_Event);

	// target node
	setID   (myTest_Node, 67);  // wrong nodeID
	setValue(myTest_Node, -5);
	add(myNodeList, myTest_Node);

	TEST_ASSERT_EQUAL(0, processAllReadyEvents() );
	myTest_Event  = NULL;  // prevents double free() of object
	myTest_Node   = NULL;  // prevents double free() of object
}

/*************************************************/
/****  processAllReadyEventsAndNodeMessagges  ****/

TEST(eventController, processAllReadyEventsAndNodeMessagges_processesOneEvent)
{
	// event message
	setID      (myTest_Event->message, 44);
	setValue   (myTest_Event->message, 23);
	setRunCount(myTest_Event->message, 0);

	// set event trigger time to 00:01
	resetTime(myTest_Event->time);
	setSecond(myTest_Event->time, 1);

	// add event
	add(myEventList, myTest_Event);

	// add target node
	setID   (myTest_Node, 44);  // matching nodeID
	setValue(myTest_Node, -5);
	add(myNodeList, myTest_Node);

	// set master time to 00:00
	resetTime(myTime);
	incrementTime(myTime);
	TEST_ASSERT_EQUAL(1,  processAllReadyEventsAndNodeMessages() );
	TEST_ASSERT_EQUAL(23, getValue(myTest_Node) );
	myTest_Event  = NULL;  // prevents double free() of object
	myTest_Node   = NULL;  // prevents double free() of object
}

TEST(eventController, processAllReadyEventsAndNodeMessagges_processesOneNodeMessage)
{
	// configure linked node message for immediate execution
	setID      (myTest_Message, 77);
	setValue   (myTest_Message, 23);

	// configure source node
	setID           (myTest_Node, 44);
	setValue        (myTest_Node, -5);
	setRunCount     (myTest_Node, 1);
	setLinkedMessage(myTest_Node, myTest_Message);

	add(myNodeList, myTest_Node);

	// add target node
	setID   (myTest_Node2, 77);  // matching nodeID for immediate message
	setValue(myTest_Node2, -6);
	add(myNodeList, myTest_Node2);

	updateEventRunCounts(NULL);
	TEST_ASSERT_EQUAL(1,  processAllReadyEventsAndNodeMessages() );
	TEST_ASSERT_EQUAL(23, getValue(myTest_Node2) );
	//myTest_Event  = NULL;  // prevents double free() of object
	myTest_Node  = NULL;  // prevents double free() of object
	myTest_Node2  = NULL;  // prevents double free() of object
	myTest_Message  = NULL;  // prevents double free() of object
}

TEST(eventController, processAllReadyEventsAndNodeMessagges_processesOneEventandMessage)
{
	// configure linked node message for immediate execution
	setID      (myTest_Message, 77);
	setValue   (myTest_Message, 23);

	// configure source node
	setID           (myTest_Node, 44);
	setValue        (myTest_Node, -5);
	setRunCount     (myTest_Node, 1);
	setLinkedMessage(myTest_Node, myTest_Message);

	add(myNodeList, myTest_Node);

	// add target node
	setID   (myTest_Node2, 77);  // matching nodeID for immediate message
	setValue(myTest_Node2, -6);
	add     (myNodeList, myTest_Node2);


	// configure event message
	setID           (myTest_Event->message, 44);
	setValue        (myTest_Event->message, -4);

	// configure event time trigger to 00:01
	resetTime             (myTest_Event->time);
	setSecond             (myTest_Event->time, 1);

	add(myEventList, myTest_Event);

	// set master time to 00:00
	resetTime             (myTime);
	TEST_ASSERT_EQUAL(MATCH_FAIL,  checkTimeMatch(myTest_Event->time, myTime) );

	// set master time to 00:01
	incrementTime         (myTime);
	TEST_ASSERT_EQUAL(MATCH,  checkTimeMatch(myTest_Event->time, myTime) );

	TEST_ASSERT_EQUAL(2,  processAllReadyEventsAndNodeMessages() );
	TEST_ASSERT_EQUAL(0,  getRunCount(myTest_Event->message) );      // event ran
	TEST_ASSERT_EQUAL(0,  getRunCount(myTest_Node) );                // message ran

	TEST_ASSERT_EQUAL(-4, getValue(myTest_Node) );
	TEST_ASSERT_EQUAL(23, getValue(myTest_Node2) );
	myTest_Event  = NULL;  // prevents double free() of object
	myTest_Node   = NULL;  // prevents double free() of object
	myTest_Node2  = NULL;  // prevents double free() of object
	myTest_Message  = NULL;  // prevents double free() of object
}


/********************************************/
/**********  updateEventRunCounts  **********/

TEST(eventController, updateEventRunCounts_returnsOneAfterProcessingSingleEvent)
{
	// event
	struct Node * eventMessagePointer = myTest_Event->message;
	struct Time * eventTimePointer    = myTest_Event->time;


	setID      (eventMessagePointer, 44);
	setValue   (eventMessagePointer, 23);
	setRunCount(eventMessagePointer, 0);
	Event_controller_test_setTime((struct Time *)eventTimePointer, SECOND_0, MINUTE_0, HOUR_0,
									  HOUR_24, AM_TIME, SUNDAY);

	add(myEventList, myTest_Event);

	// target node
	setID   (myTest_Node, 44);  // wrong nodeID
	setValue(myTest_Node, -5);
	add(myNodeList, myTest_Node);

	// Time
	Event_controller_test_setTime(myTime, SECOND_0, MINUTE_0, HOUR_0,
								  HOUR_24, AM_TIME, SUNDAY);

	updateEventRunCounts(NULL);

	TEST_ASSERT_EQUAL(1, myTest_Event->message->runCount );
	myTest_Event  = NULL;  // prevents double free() of object
	myTest_Node   = NULL;  // prevents double free() of object
}

TEST(eventController, updateEventRunCounts_returnsZeroIfNoTimeMatch)
{
	// event
	struct Time * eventTimePointer    = myTest_Event->time;
	Event_controller_test_setTime(eventTimePointer, SECOND_0+1, MINUTE_0, HOUR_0,
									  HOUR_24, AM_TIME, SUNDAY);
	add(myEventList, myTest_Event);

	// target node
	add(myNodeList, myTest_Node);

	// Time
	Event_controller_test_setTime(myTime, SECOND_0, MINUTE_0, HOUR_0,
								  HOUR_24, AM_TIME, SUNDAY);
	updateEventRunCounts(NULL);

	TEST_ASSERT_EQUAL(0, myTest_Event->message->runCount );
	myTest_Event  = NULL;  // prevents double free() of object
	myTest_Node   = NULL;  // prevents double free() of object
}

TEST(eventController, updateEventRunCounts_returnsZeroIfNoEvents)
{
	// missing event

	// target node
	add(myNodeList, myTest_Node);

	// Time
	Event_controller_test_setTime(myTime, SECOND_0, MINUTE_0, HOUR_0,
								  HOUR_24, AM_TIME, SUNDAY);

	updateEventRunCounts(NULL);

	TEST_ASSERT_EQUAL(0, myTest_Event->message->runCount );
	myTest_Node   = NULL;  // prevents double free() of object
}

TEST(eventController, updateEventRunCounts_incrementTime_incrementsRunCount)
{
	// event
	struct Node * eventMessagePointer = myTest_Event->message;
	setID        (eventMessagePointer, 44);
	setValue     (eventMessagePointer, 23);
	setRunCount  (eventMessagePointer, 0);

	// set eventTime to 1 second
	struct Time * eventTimePointer = myTest_Event->time;
	resetTime(eventTimePointer);
	eventTimePointer->second++;
	TEST_ASSERT_EQUAL(1, eventTimePointer->second );

	add(myEventList, myTest_Event);

	// target node
	setID   (myTest_Node, 44);
	setValue(myTest_Node, -5);
	add(myNodeList, myTest_Node);

	// set master time to 0 seconds
	resetTime(myTime);

	// show that wrong time does not increment runCount
	updateEventRunCounts(NULL);
	TEST_ASSERT_EQUAL(0, myTest_Event->message->runCount );

	// increment master time
	TEST_ASSERT_EQUAL(0, getSecond(myTime) );
	incrementTime(myTime);
	TEST_ASSERT_EQUAL(1, myTest_Event->message->runCount );

	// process all events to show that run count is decremented
	processAllReadyEvents();
	TEST_ASSERT_EQUAL(0, myTest_Event->message->runCount );
	myTest_Event  = NULL;  // prevents double free() of object
	myTest_Node   = NULL;  // prevents double free() of object
}


TEST(eventController, memory_leak)
{
	// new structure location will move based on other processes
	// create and destroy a large number of objects
	// then look for the base address within an additional 10 new objects

	struct Event * new_EventItem = new(Event);
	struct Event * originalEventPointer = new_EventItem;
	EventController_addEvent(new_EventItem);
	//new_EventItem = safeDelete(new_EventItem);

	struct Node * new_NodeItem = new(Node);
	struct Node * originalNodePointer = new_NodeItem;
	EventController_addNode(new_NodeItem);
	//new_NodeItem = safeDelete(new_NodeItem);

	//printf("\nEventController-memory-leak-test, originalNodePointer: %p, originalNodePtrCount: %p\n", originalEventPointer, originalNodePointer);

	removeAllNodes(myEventList);
	removeAllNodes(myNodeList);

	// create and destroy a large number of objects to cause a memory leak
	printf("\n");
	int j;
	for (j = 0; j < 1000; j++) {

		new_EventItem = new(Event);
		EventController_addEvent(new_EventItem);

		new_NodeItem = new(Node);
		EventController_addNode(new_NodeItem);

		//printf("new_EventItem: %p, new_NodeItem: %p, i: %i\n", new_EventItem, new_NodeItem, j);

		//delete(take(myEventList));
		//delete(take(myNodeList));

		//removeAllNodes(myEventList);
		//removeAllNodes(myNodeList);

		EventController_deleteAllItems(myEventList);
		EventController_deleteAllItems(myNodeList);

	}

	//printf("\nnew_EventItem target: %p, new_NodeItem target: %p\n", originalEventPointer, originalNodePointer);

	// check for original object location within a small number of additional objects
	int originalEventPtrCount = 0;
	int originalNodePtrCount = 0;
	for (j = 0; j < 100; j++) {

		new_EventItem = new(Event);
		EventController_addEvent(new_EventItem);

		new_NodeItem = new(Node);
		EventController_addNode(new_NodeItem);

		//printf("new_EventItem: %p, new_NodeItem: %p, i: %i\n", new_EventItem, new_NodeItem, j);

		if (originalEventPointer == new_EventItem) { originalEventPtrCount++; }
		if (originalNodePointer  == new_NodeItem)  { originalNodePtrCount++; }

		//delete(take(myEventList));
		//delete(take(myNodeList));

		//removeAllNodes(myEventList);
		//removeAllNodes(myNodeList);

		EventController_deleteAllItems(myEventList);
		EventController_deleteAllItems(myNodeList);

		}

	//printf("\nEventController-memory-leak-test, originalEventPtrCount: %i, originalNodePtrCount: %i, i: %i\n", originalEventPtrCount, originalNodePtrCount, 100);

	TEST_ASSERT_TRUE(originalEventPtrCount);
	TEST_ASSERT_TRUE(originalNodePtrCount);

	originalEventPointer = NULL;
	originalNodePointer = NULL;
	new_NodeItem  = NULL;
	new_NodeItem  = NULL;
}

TEST(eventController, API_test)
{
	// event
	struct Node * eventMessagePointer = getMessagePointer(myTest_Event);
	setID        (eventMessagePointer, 44);
	setValue     (eventMessagePointer, 23);
	setRunCount  (eventMessagePointer, 0);

	// set eventTime to 1 second
	struct Time * eventTimePointer = getTimePointer(myTest_Event);
	resetTime    (eventTimePointer);
	incrementTime(eventTimePointer);  // set to 01 seconds

	EventController_addEvent(myTest_Event);

	// target node
	setID   (myTest_Node, 44);
	setValue(myTest_Node, -5);
	EventController_addNode(myTest_Node);

	// set master time to 0 seconds
	resetTime(myTime);

	// register updateEventRunCounts() to fire in second change CB from TIME
	setTime_OnSecondChange_cb(myTime, updateEventRunCounts);

	// show that wrong time does not increment runCount at time of 00:00
	TEST_ASSERT_EQUAL(0, processAllReadyEventsAndNodeMessages() );

	// increment time to 00:01 minutes:seconds increments runCount
	incrementTime(myTime);
	TEST_ASSERT_EQUAL(1, processAllReadyEventsAndNodeMessages() );

	// increment master time to 00:02 second, does not increment runCount
	incrementTime(myTime);
	TEST_ASSERT_EQUAL(0, processAllReadyEventsAndNodeMessages() );

	// increment master time to 03 second, resulting in no action
	incrementTime(myTime);
	TEST_ASSERT_EQUAL(0, processAllReadyEventsAndNodeMessages() );

	myTest_Event  = NULL;  // prevents double free() of object
	myTest_Node   = NULL;  // prevents double free() of object
}

/********************************************/
/********************************************/
/********************************************/

void EventController_deleteAllItems(void * _listPointer)
{
	void * localPTR;
	int localItemCount = getItemCount(_listPointer);
	for ( deleteIndex = 0; deleteIndex < localItemCount; deleteIndex++) {
		// WARNING:  deleting non-objects is not allowed
		localPTR = take(_listPointer);
		//printf("deleting pointer: %p\n", localPTR);
		delete(localPTR);
	}
	return;
}

static void Event_controller_test_setTime(struct Time * _self,
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

