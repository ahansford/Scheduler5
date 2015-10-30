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
#include "..\..\src\events\events.h"

/****************************************/
/*** Basic Object and Class Testing *****/

struct       Event *      myTest_Event;
const struct EventClass * myTest_EventClass_PTR;

int i;

/****************************************/
/*********** Start of Testing ***********/
TEST_GROUP(events);

TEST_SETUP(events)
{
	RuntimeErrorStub_Reset();
	Event_init();

	myTest_Event  = new(Event);
	myTest_EventClass_PTR  = classOf(myTest_Event);
}

TEST_TEAR_DOWN(events)
{
	myTest_Event = safeDelete(myTest_Event);
	RuntimeErrorStub_Reset();
}

/**************************************/
/**************  New  *****************/

TEST(events, myTestEvent_IsNotNullOnCreate)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  myTest_Event);
}

TEST(events, events_time_NotNullOnCreate)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  myTest_Event->time);
}

TEST(events, events_message_NotNullOnCreate)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  myTest_Event->message);
}


/**************************************/
/*************  Events  *****************/

TEST(events, init_newEvent_ClassReturnsNonNull)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  myTest_Event->_.class);
}

TEST(events, init_classOf_myTest_Event_is_Event)
{
	TEST_ASSERT_EQUAL_PTR(Event,  myTest_Event->_.class);
}

TEST(events, init_classOf_Event_is_EventClass)
{
	TEST_ASSERT_EQUAL_PTR(EventClass,  myTest_Event->_.class->_.class);
}

TEST(events, init_ctor_pointsTo_EventClass_ctor)
{
	TEST_ASSERT_EQUAL_PTR(Event_ctor,  myTest_Event->_.class->ctor);
	TEST_ASSERT_EQUAL_PTR(EventClass_ctor,  myTest_Event->_.class->_.class->ctor);
}

TEST(events, init_ctor_pointsTo_Event_ctor)
{
	TEST_ASSERT_EQUAL_PTR(Event_ctor,  myTest_Event->_.class->ctor);
}

TEST(events, init_dtor_pointsTo_Event_dtor)
{
	TEST_ASSERT_EQUAL_PTR(Event_dtor,  ((struct Class *)myTest_EventClass_PTR)->dtor );
	TEST_ASSERT_EQUAL_PTR(Event_dtor,  myTest_Event->_.class->dtor);
}

TEST(events, init_copy_pointsTo_Event_copy)
{
	TEST_ASSERT_EQUAL_PTR(Event_copy,  myTest_Event->_.class->copy);
}

TEST(events, init_equal_pointsTo_Event_equal)
{
	TEST_ASSERT_EQUAL_PTR(Event_equal,  myTest_Event->_.class->equal);
}

TEST(events, init_config_pointsTo_Event_config)
{
	TEST_ASSERT_EQUAL_PTR(Event_config,  myTest_Event->_.class->config);
}

TEST(events, init_EventClass_NameIs_EventClass)
{
	TEST_ASSERT_EQUAL_STRING("EventClass",  myTest_Event->_.class->_.class->className);
}

/**************************************/
/********  TimeClass Methods  *********/

TEST(events, init_setTimePointer_pointsTo_Event_setTimePointer)
{
	TEST_ASSERT_EQUAL_PTR(Event_setTimePointer,  myTest_EventClass_PTR->setTimePointer );
}

TEST(events, init_getTimePointer_pointsTo_Event_getTimePointer)
{
	TEST_ASSERT_EQUAL_PTR(Event_getTimePointer,  myTest_EventClass_PTR->getTimePointer);
}

TEST(events, init_setMessagePointer_pointsTo_Event_setMessagePointer)
{
	TEST_ASSERT_EQUAL_PTR(Event_setMessagePointer,  myTest_EventClass_PTR->setMessagePointer );
}

TEST(events, init_getMessagePointer_pointsTo_Event_getMessagePointer)
{
	TEST_ASSERT_EQUAL_PTR(Event_getMessagePointer,  myTest_EventClass_PTR->getMessagePointer);
}

TEST(events, init_updateEventRunCount_pointsTo_Event_updateEventRunCount)
{
	TEST_ASSERT_EQUAL_PTR(Event_updateEventRunCount,  myTest_EventClass_PTR->updateEventRunCount);
}

TEST(events, init_setEventRunCount_pointsTo_Event_setEventRunCount)
{
	TEST_ASSERT_EQUAL_PTR(Event_setEventRunCount,  myTest_EventClass_PTR->setEventRunCount);
}

TEST(events, init_getEventRunCount_pointsTo_Event_getEventRunCount)
{
	TEST_ASSERT_EQUAL_PTR(Event_getEventRunCount,  myTest_EventClass_PTR->getEventRunCount);
}


//****  delete/Event_dtor  ****************

TEST(events, delete_returns_mySelfOnSuccess)
{
	TEST_ASSERT_EQUAL_PTR(myTest_Event,  delete(myTest_Event));
	myTest_Event = NULL;

}

TEST(events, delete_safeDelete_returnsNullOnSuccess)
{
	TEST_ASSERT_EQUAL_PTR(NULL,  safeDelete(myTest_Event));
	myTest_Event = NULL;
}

//****  Set and Get time  ****************


TEST(events, getTimePointer_NotNullOnCreate)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  getTimePointer(myTest_Event));
}

TEST(events, getTimePointer_returns_specificValue)
{
	struct Time * oldTimePtr = myTest_Event->time;
	struct Time * localTimePtr = new(Time);
	myTest_Event->time = localTimePtr;
	TEST_ASSERT_EQUAL_PTR(localTimePtr,  getTimePointer(myTest_Event));
	oldTimePtr = safeDelete(oldTimePtr);
}

TEST(events, setTimePointer_returnsSpecificValue)
{
	struct Time * oldTimePtr = myTest_Event->time;
	struct Time * localTimePtr = new(Time);
	TEST_ASSERT_EQUAL_PTR(myTest_Event->time,  setTimePointer(myTest_Event, localTimePtr));
	oldTimePtr = safeDelete(oldTimePtr);
}

TEST(events, setTimePointer_returnsNullOnNullPtr)
{
	struct Time * localTimePtr = new(Time);
	TEST_ASSERT_EQUAL_PTR(NULL,  setTimePointer(NULL, localTimePtr));
	localTimePtr = safeDelete(localTimePtr);
}

TEST(events, setTimePointer_returnsSpecificValueOnSuccess)
{

	struct Time * oldTimePtr = myTest_Event->time;
	struct Time * localTimePtr = new(Time);
	TEST_ASSERT_EQUAL_PTR(localTimePtr,  setTimePointer(myTest_Event, localTimePtr));
	oldTimePtr = safeDelete(oldTimePtr);
}

TEST(events, setTimePointer_canSetSpecificValue)
{
	struct Time * oldTimePtr = myTest_Event->time;
	struct Time * localTimePtr = new(Time);
	setTimePointer(myTest_Event, localTimePtr);
	TEST_ASSERT_EQUAL_PTR(localTimePtr,  myTest_Event->time);
	oldTimePtr = safeDelete(oldTimePtr);
}


//****  Set and Get message  ****************
/**/
TEST(events, getMessagePointer_NotNullOnCreate)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  getTimePointer(myTest_Event));
}

TEST(events, getMessagePointer_returns_specificValue)
{
	struct Node * oldEventPtr = myTest_Event->message;
	struct Node * localEventPtr = new(Node);
	myTest_Event->message = localEventPtr;
	TEST_ASSERT_EQUAL_PTR(localEventPtr,  getMessagePointer(myTest_Event));
	oldEventPtr = safeDelete(oldEventPtr);
}


TEST(events, setMessagePointer_returnsSpecificValue)
{
	struct Node * oldEventPtr = myTest_Event->message;
	struct Node * localEventPtr = new(Node);
	TEST_ASSERT_EQUAL_PTR(myTest_Event->message,  setMessagePointer(myTest_Event, localEventPtr));
	oldEventPtr = safeDelete(oldEventPtr);
}

TEST(events, setMessagePointer_returnsNullOnNullPtr)
{
	struct Node * localEventPtr = new(Node);
	TEST_ASSERT_EQUAL_PTR(NULL,  setMessagePointer(NULL, localEventPtr));
	localEventPtr = safeDelete(localEventPtr);
}

TEST(events, setMessagePointer_returnsSpecificValueOnSuccess)
{
	struct Node * oldEventPtr = myTest_Event->message;
	struct Node * localEventPtr = new(Node);
	TEST_ASSERT_EQUAL_PTR(localEventPtr,  setMessagePointer(myTest_Event, localEventPtr));
	oldEventPtr = safeDelete(oldEventPtr);
}

TEST(events, setMessagePointer_canSetSpecificValue)
{
	struct Node * oldEventPtr = myTest_Event->message;
	struct Node * localEventPtr = new(Node);
	setMessagePointer(myTest_Event, localEventPtr);
	TEST_ASSERT_EQUAL_PTR(localEventPtr,  myTest_Event->message);
	oldEventPtr = safeDelete(oldEventPtr);
}

//****  updateEventRunCount  ****************

TEST(events, updateEventRunCount_CountIsZeroOnCreate)
{
	struct Node * myMessage = getMessagePointer(myTest_Event);
	TEST_ASSERT_EQUAL(0,  getRunCount(myMessage));
}

TEST(events, updateEventRunCount_CountIsZeroOnWrongTime)
{
	struct Time * eventTime = getTimePointer(myTest_Event);
	setHour(eventTime, 2);

	struct Time * masterTime = new(Time);
	setHour(masterTime, 1);

	updateEventRunCount(myTest_Event, masterTime);

	struct Node * myMessage = getMessagePointer(myTest_Event);
	TEST_ASSERT_EQUAL(0,  getRunCount(myMessage));
	masterTime = safeDelete(masterTime);

}

TEST(events, updateEventRunCount_CountIsOneOnCorrectTime)
{
	struct Time * eventTime = getTimePointer(myTest_Event);
	setHour(eventTime, 2);

	struct Time * masterTime = new(Time);
	setHour(masterTime, 2);

	updateEventRunCount(myTest_Event, masterTime);

	struct Node * myMessage = getMessagePointer(myTest_Event);
	TEST_ASSERT_EQUAL(1,  getRunCount(myMessage));
	masterTime = safeDelete(masterTime);
}

TEST(events, updateEventRunCount_CountIncrementsOnMultipleCorrectTimes)
{
	struct Time * eventTime = getTimePointer(myTest_Event);
	setHour(eventTime, 2);

	struct Time * masterTime = new(Time);
	setHour(masterTime, 2);
	updateEventRunCount(myTest_Event, masterTime);
	updateEventRunCount(myTest_Event, masterTime);

	struct Node * myMessage = getMessagePointer(myTest_Event);
	TEST_ASSERT_EQUAL(2,  getRunCount(myMessage));
	masterTime = safeDelete(masterTime);
}

TEST(events, updateEventRunCount_DoesNotIncrementsOnWrongTime)
{
	struct Time * eventTime = getTimePointer(myTest_Event);
	setHour(eventTime, 2);

	struct Time * masterTime = new(Time);
	setHour(masterTime, 3);
	updateEventRunCount(myTest_Event, masterTime);

	struct Node * myMessage = getMessagePointer(myTest_Event);
	TEST_ASSERT_EQUAL(0,  getRunCount(myMessage));
	masterTime = safeDelete(masterTime);
}

TEST(events, updateEventRunCount_ReturnsSelfOnSuccess)
{
	struct Time * masterTime = new(Time);
	setHour(masterTime, 3);
	TEST_ASSERT_EQUAL_PTR(myTest_Event, updateEventRunCount(myTest_Event, masterTime));
	masterTime = safeDelete(masterTime);
}


//****  decrementEventRunCount  ****************

TEST(events, decrementEventRunCounter_ResetsCountToZeroFromUnknown)
{
	decrementEventRunCounter(myTest_Event);
	struct Node * myMessagePTR = getMessagePointer(myTest_Event);
	TEST_ASSERT_EQUAL(0,  getRunCount( myMessagePTR ));
}

TEST(events, decrementEventRunCounter_HoldsAtZeroFromZero)
{
	decrementEventRunCounter(myTest_Event);
	decrementEventRunCounter(myTest_Event);
	struct Node * myMessagePTR = getMessagePointer(myTest_Event);
	TEST_ASSERT_EQUAL(0,  getRunCount( myMessagePTR ));
}

TEST(events, decrementEventRunCounter_Decreases1ToZero)
{

	struct Node * myMessagePTR = getMessagePointer(myTest_Event);
	myMessagePTR->runCount = 1;
	decrementEventRunCounter(myTest_Event);
	TEST_ASSERT_EQUAL(0,  getRunCount( myMessagePTR ));
}

TEST(events, decrementEventRunCounter_ConvertsNegativesToZero)
{
	struct Node * myMessagePTR = getMessagePointer(myTest_Event);
	myMessagePTR->runCount = -51;
	decrementEventRunCounter(myTest_Event);
	TEST_ASSERT_EQUAL(0,  getRunCount( myMessagePTR ));
}

TEST(events, decrementEventRunCounter_100_to_99)
{
	struct Node * myMessagePTR = getMessagePointer(myTest_Event);
	myMessagePTR->runCount = 100;
	decrementEventRunCounter(myTest_Event);
	TEST_ASSERT_EQUAL(99,  getRunCount( myMessagePTR ));
}

TEST(events, decrementEventRunCounter_ReturnsSelfOnSuccess)
{
	TEST_ASSERT_EQUAL_PTR(myTest_Event, decrementEventRunCounter(myTest_Event));
}


//****  setEventRunCount/getEventRunCount  ****************


TEST(events, getEventRunCount_ZeroOnCreate)
{
	TEST_ASSERT_EQUAL(0,  getEventRunCount(myTest_Event));
}

TEST(events, getEventRunCount_returns_specificValue)
{
	myTest_Event->message->runCount = 7;
	TEST_ASSERT_EQUAL(7,  getEventRunCount(myTest_Event));
}

TEST(events, setEventRunCount_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(myTest_Event->message->runCount,  setEventRunCount(myTest_Event, 5));
}

TEST(events, setEventRunCount_returnsNullOnNullPtr)
{
	TEST_ASSERT_EQUAL(RUN_COUNT_UNKNOWN,  setEventRunCount(NULL, 5));
}

TEST(events, setEventRunCount_returnsSpecificValueOnSuccess)
{
	TEST_ASSERT_EQUAL(5,  setEventRunCount(myTest_Event, 5));
}

TEST(events, setEventRunCount_canSetSpecificValue)
{
	setEventRunCount(myTest_Event, 6);
	TEST_ASSERT_EQUAL(6,  myTest_Event->message->runCount);
}


//****  copy/Event_copy  ****************

TEST(events, copy_returnsSelfOnSuccess)
{
	struct Event * masterEvent    = new(Event);
	struct Time  * masterTime     = getTimePointer(masterEvent);
		setSecond (masterTime, 1);
	TEST_ASSERT_EQUAL_PTR(myTest_Event, copy(myTest_Event, masterEvent));
	masterEvent = safeDelete(masterEvent);
}

TEST(events, copy_AllItemsCopiedToSelf)
{
	struct Event * masterEvent = new(Event);

	struct Time  * masterTime  = getTimePointer(masterEvent);
		setSecond             (masterTime, 1);
		setMinute             (masterTime, 2);
		setHour               (masterTime, 3);
		setHourMode           (masterTime, HOUR_MODE_24);
		setAmPm               (masterTime, AM_TIME);
		setDayOfWeek          (masterTime, 4);
		setTimeUpdateIncrement(masterTime, UPDATE_SECONDS);

	struct Node  * masterMessage = getMessagePointer(masterEvent);
		setID   (masterMessage, 1);
		setType (masterMessage, 2);
		setState(masterMessage, 3);
		setValue(masterMessage, 4);

	copy(myTest_Event, masterEvent);

	struct Time  * myTimePTR = getTimePointer(myTest_Event);
	TEST_ASSERT_EQUAL(1,              getSecond             (myTimePTR) );
	TEST_ASSERT_EQUAL(2,              getMinute             (myTimePTR) );
	TEST_ASSERT_EQUAL(3,              getHour               (myTimePTR) );
	TEST_ASSERT_EQUAL(HOUR_MODE_24,   getHourMode           (myTimePTR) );
	TEST_ASSERT_EQUAL(AM_TIME,        getAmPm               (myTimePTR) );
	TEST_ASSERT_EQUAL(4,              getDayOfWeek          (myTimePTR) );
	TEST_ASSERT_EQUAL(UPDATE_SECONDS, getTimeUpdateIncrement(myTimePTR) );

	struct Time  * myMessagePTR = getMessagePointer(myTest_Event);
	TEST_ASSERT_EQUAL(1, getID   (myMessagePTR) );
	TEST_ASSERT_EQUAL(2, getType (myMessagePTR) );
	TEST_ASSERT_EQUAL(3, getState(myMessagePTR) );
	TEST_ASSERT_EQUAL(4, getValue(myMessagePTR) );

	masterEvent = safeDelete(masterEvent);
}

TEST(events, copy_returnsNullOnNullSelf)
{
	struct Event * masterEvent    = new(Event);
	struct Time  * masterTime     = getTimePointer(masterEvent);
	setSecond(masterTime, 1);
	TEST_ASSERT_EQUAL_PTR(NULL, copy(NULL, masterTime)  );
	masterEvent = safeDelete(masterEvent);
}

TEST(events, copy_returnsNullOnNullMaster)
{
	struct Event * masterEvent    = new(Event);
	struct Time  * masterTime     = getTimePointer(masterEvent);
	setSecond(masterTime, 1);
	TEST_ASSERT_EQUAL_PTR(NULL, copy(myTest_Event, NULL) );
	masterEvent = safeDelete(masterEvent);
}

//****  equal/Time_equal  ********************

/**/
TEST(events, equal_self_Equals_self)
{
	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_Event, myTest_Event));
}

TEST(events, equal_UnequalTimesReturn_Unequal)
{
	struct Event * masterEvent = new(Event);
	struct Time  * masterTime  = getTimePointer(masterEvent);
	setSecond(masterTime, 1);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Event, masterEvent) );
	masterEvent = safeDelete(masterEvent);
}

TEST(events, equal_UnequalMessagesReturn_Unequal)
{
	struct Event * masterEvent   = new(Event);
	struct Node  * masterMessage = getMessagePointer(masterEvent);
	setID(masterMessage, 1);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Event, masterEvent) );
	masterEvent = safeDelete(masterEvent);
}

TEST(events, equal_NullReturns_Unequal)
{
	struct Event * masterEvent = new(Event);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Event, NULL));
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(NULL, masterEvent ));
	masterEvent = safeDelete(masterEvent);
}

TEST(events, equal_CopiedItemReturns_Equal)
{
	struct Event * masterEvent    = new(Event);

	struct Time  * masterTime     = getTimePointer(masterEvent);
		setSecond             (masterTime, 1);
		setMinute             (masterTime, 2);
		setHour               (masterTime, 3);
		setHourMode           (masterTime, HOUR_MODE_24);
		setAmPm               (masterTime, AM_TIME);
		setDayOfWeek          (masterTime, 4);
		setTimeUpdateIncrement(masterTime, UPDATE_SECONDS);

	struct Node  * masterMessage  = getMessagePointer(masterEvent);
		setID   (masterMessage, 1);
		setType (masterMessage, 2);
		setState(masterMessage, 3);
		setValue(masterMessage, 4);

	copy(myTest_Event, masterEvent);
	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_Event, masterEvent));
	masterEvent = safeDelete(masterEvent);
}


/***********  config  **************/
/**/
TEST(events, Config_copiesData)
{
	struct Event * masterEvent    = new(Event);
	struct Time  * masterTime     = getTimePointer(masterEvent);
	setSecond(masterTime, 3);
	config(myTest_Event, masterEvent);
	TEST_ASSERT_EQUAL(3, getSecond(masterTime));
	masterEvent = safeDelete(masterEvent);
}

/***********  init  **************/
/**/
TEST(events, CanCall_Event_Init_MulitpleTimes)
{
	const void * originalEvent      = Event;
	const void * originalEventClass = EventClass;
	Event_init();
	Event_init();
	Event_init();

	// Verify that the class pointers are unchanged
	TEST_ASSERT_EQUAL_PTR(Event,      originalEvent);
	TEST_ASSERT_EQUAL_PTR(EventClass, originalEventClass);

	char outputFilename[] = "events_puto.txt";
	FILE * fp = fopen(outputFilename, "a");
	puto(myTest_Event, fp);
	fclose(fp);
}

TEST(events, memory_leak)
{
	// new structure location will move based on other processes
	// create and destroy a large number of objects
	// then look for the base address within an additional 10 new objects

	// get original object location
	struct Event * newItem = new(Event);
	struct Event * originalPointer = newItem;

	//printf("\nEvents-memory-leak-test, originalPointer: %p, OriginalTimePtr: %p, originalMessageNodePtr: %p\n",
	//			originalPointer, originalPointer->time, originalPointer->message);

	newItem = safeDelete(newItem);
	// create and destroy a large number of objects to cause a memory leak
	for (i = 0; i < 1000; i++) {
		newItem = new(Event);
		printf("newItem: %p, timePtr: %p, messagePtr: %p, i: %i\n", newItem, newItem->time, newItem->message,  i);
		delete(newItem);
	}

	// check for original object location within a small number of additional objects
	int originalPtrCount = 0;
	for (i = 0; i < 100; i++) {
		newItem = new(Event);
		printf("newItem: %p, timePtr: %p, messagePtr: %p, i: %i\n", newItem, newItem->time, newItem->message,  i);
		delete(newItem);
		if (originalPointer == newItem) { originalPtrCount++; }
	}

	printf("\nEvents-memory-leak-test, originalPtrCount: %i, out of %i trials\n", originalPtrCount, 100);
	TEST_ASSERT_TRUE(originalPtrCount);
}

