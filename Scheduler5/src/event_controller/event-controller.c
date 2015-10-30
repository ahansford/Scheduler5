/*
 * event-controller.c
 *
 *  Created on: Aug 15, 2015
 *      Author: alhansfo
 */


#include "event-controller.h"
#include "event-controller-private.h"            // safety include
#include "..\..\src\nodes\nodes.h"     // safety include
#include "..\..\src\node_list\node-list.h"     // safety include
#include "..\..\src\times\times.h"     // safety include
#include "..\..\src\lists\lists.h"     // supports class registration
#include "..\..\src\objects\objects.h" // safety include
#include "..\..\src\events\events.h" // safety include

/**************************************/
/***** Implementation Functions  ******/

void * implement_eventController_remove(void * _nodeList, void * _node);

// MUST be called before other event controller methods are called
increment_t EventController_init(increment_t _mode)
{
	if (!List)      { printf("EventController call to List_init()\n"); List_init(); }
	if (!ListClass) { printf("EventController call to List_init()\n"); List_init(); }

	if (!NodeList)      { printf("EventController call to List_init()\n"); NodeList_init(); }
	if (!NodeListClass) { printf("EventController call to List_init()\n"); NodeList_init(); }

	if (!Time)      { printf("EventController call to List_init()\n"); Time_init(); }
	if (!TimeClass) { printf("EventController call to List_init()\n"); Time_init(); }

	if ( !myEventList ) { myEventList = new(NodeList, eventArray); }
	if ( !myNodeList )  { myNodeList  = new(NodeList, nodeArray);  }
	if ( !myTime )      { myTime      = new(Time);     }

	// register updateEventRunCounts() to fire at incrementTime() interval
	resetTime(myTime);
	setTimeUpdateIncrement(myTime, _mode);

	increment_t returnMode = INCREMENT_UNKNOWN;
	if ( _mode == UPDATE_SECONDS) {
		setTime_OnSecondChange_cb(myTime, updateEventRunCounts);
		returnMode = _mode;
	}
	if ( _mode == UPDATE_MINUTES) {
		setTime_OnMinuteChange_cb(myTime, updateEventRunCounts);
		returnMode = _mode;
	}

	return returnMode;
}

void EventController_destroy(void)
{
	// remove all events and the nodelist
	//removeAllNodes(myEventList);
	myEventList = safeDelete(myEventList);

	// remove all nodes and the nodelist
	//removeAllNodes(myNodeList);
	myNodeList = safeDelete(myNodeList);

	// remove time
	myTime = safeDelete(myTime);

	return;
}

void EventController_removeAllAndReset(void)
{
	removeAllNodes(myEventList);
	removeAllNodes(myNodeList);
	resetTime(myTime);
	return;
}


int processAllReadyNodeMessages(void)
{
	int numberOfProcessedMessages = 0;

	// get count of nodes
	int localItemCount = getItemCount(myNodeList);

	// cycle through all ready to run messages
	int i;
	void * element = NULL;
	for ( i = 0; i < localItemCount; i++) {

		// get pointer to next element
		element = incrementGetReadyToRun(myNodeList);

		// test if ready to run
		element = getPtrToSelfIfReadyToRun(element);
		//printf("   after return pointer to self: %p with Message: %p\n", element, ((struct Node *)element)->linkedMessage);
		if (element != NULL ) {

			// ready to run message found
			// execute ready to run message
			if ( executeMessage(element) != NULL ) {
				// message successfully executed
				numberOfProcessedMessages++;
			}

		}
	}  // end for loop
	return numberOfProcessedMessages;
}

void * executeMessage(void * _node)
{
	// verify source node containing the message pointer
	struct Node * sourceNode = cast(Node, _node);
	if ( sourceNode == NULL ) { return NULL; } // fail

	// get message pointer
	struct Node * messagePointer = getLinkedMessage(sourceNode);
	if ( messagePointer == NULL ) { return NULL; } // fail

	// execute message now/immediate
	struct Node * targetNodePointer;

	targetNodePointer = executeEvent(messagePointer);

	// test for execution success
	if ( targetNodePointer == NULL ) { return NULL; }  // failed to execute

	// acknowledge successful execution of message
	decrementRunCount(sourceNode);
	return targetNodePointer;    // expected path
}

struct Time * updateEventRunCounts(struct Time * _time)
{
	struct Time * callbackTime = cast(Time, _time);

	int numberOfProcessedEvents = 0;

	// get count of events
	int localItemCount = getItemCount(myEventList);

	// cycle through all events
	int i;
	void * event = NULL;
	void * message = NULL;
	void * time = NULL;
	timeMatch_t timeMatch;
	for ( i = 0; i < localItemCount; i++) {

		// get pointer to next event
		event = incrementGet(myEventList);

		// get message pointer
		message = getMessagePointer(event);

		// get time pointer
		time = getTimePointer(event);

		// test if times match
		timeMatch = checkTimeMatch(time, myTime);

		if ( timeMatch == MATCH ) {

			// time match found
			incrementRunCount(message);

			numberOfProcessedEvents++;

		}
	}  // end for loop
	return callbackTime;
}

int processAllReadyEventsAndNodeMessages(void)
{
	int numberOfProcessedItems = 0;
	int countOnLastPass = 0;

	do {

		// update the count of executions from the last pass
		countOnLastPass = numberOfProcessedItems;

		// check for and execute any events or node messages
		//numberOfProcessedItems += processAllReadyNodeMessages();
		numberOfProcessedItems += processAllReadyEvents();
		numberOfProcessedItems += processAllReadyNodeMessages();

		// break from loop if last pass was clean with no additional executions
	} while (numberOfProcessedItems != countOnLastPass);

	return numberOfProcessedItems;
}

int processAllReadyEvents(void)
{
	int numberOfProcessedEvents = 0;

	// get count of nodes
	int localItemCount = getItemCount(myEventList);

	// cycle through all ready to run messages
	int i;
	void * event = NULL;
	void * message = NULL;
	void * targetNode = NULL;
	for ( i = 0; i < localItemCount; i++) {

		// get pointer to next event
		event = incrementGet(myEventList);

		// get message pointer
		message = getMessagePointer(event);

		// test if ready to run
		message = getPtrToSelfIfReadyToRun(message);

		if ( message != NULL ) {

			// ready to run message found
			// execute ready to run message
			targetNode = executeEvent(message);

			// increment count on execution success
			if ( targetNode != NULL ) { numberOfProcessedEvents++; }

		}
	}  // end for loop
	return numberOfProcessedEvents;
}

void * executeEvent(void * _event)
{
	// get pointer to the message node
	struct Node * eventMessagePointer;
	struct Class * class = (void *)classOf(_event);
	if (class == Event) {
		eventMessagePointer = getMessagePointer(_event);
	}
	else {
		eventMessagePointer = _event;
	}

	// get nodeID
	id_t eventID = getID(eventMessagePointer);

	// verify that nodeID is present in myNodeList
	if ( containsNodeID(myNodeList, eventID) == DOES_NOT_CONTAIN )
		{ return NULL; } // fail

	// get pointer to the node from myNodeList
	// WARNING: assumes that nodeID values are unique
	struct Node * nodePointer = NULL;
	struct Node * element;
	id_t nodeID;
	int localItemCount = getItemCount(myNodeList);
	int i;
	for ( i = 0; i < localItemCount; i++ ) {
		element = incrementGet(myNodeList);
		nodeID = getID(element);
		if ( nodeID == eventID ) {
			// message and node IDs match
			nodePointer = element;
		}
	}
	if ( nodePointer == NULL ) {return NULL; } // mismatched IDs

	// EXECUTION: copy event state to the node
	if ( nodePointer != copy(nodePointer, eventMessagePointer))
		{ return NULL; } // failed to execute event

	// acknowledge successful execution of event
	decrementRunCount(eventMessagePointer);
	return nodePointer;
}

void * EventController_addEvent(void * _event)
{
	struct Event * event = cast(Event, _event);
	if ( event == NULL ) { return NULL; } // fail
	return add(myEventList, event);       // expected path
}

void * EventController_removeEvent(void * _event)
{
	struct Event * event = cast(Event, _event);
	if ( event == NULL ) { return NULL; } // fail
	return implement_eventController_remove(myEventList, event);
}

void * EventController_incrementGetEvent(void)
{
	return incrementGet(myEventList);
}

void * EventController_addNode(void * _node)
{
	struct Node * node = cast(Node, _node);
	if ( node == NULL ) { return NULL; } // fail
	return add(myNodeList, node);       // expected path
}

void * EventController_removeNode(void * _node)
{
	struct Node * node = cast(Node, _node);
	if ( node == NULL ) { return NULL; } // fail
	return implement_eventController_remove(myNodeList, node);
}

void * EventController_incrementGetNode(void)
{
	return incrementGet(myNodeList);
}

void * implement_eventController_remove(void * _nodeList, void * _node)
{
	int localItemCount = getItemCount(_nodeList);
	int i;
	void * element;
	void * returnValue = NULL;

	resetIndex(_nodeList);
	for ( i = 0; i < localItemCount; i++) {
		element =  take(_nodeList);
		//element =  delete( take(_nodeList) );
		if ( element == _node ) {
			// success: node found, do not add back
			returnValue = delete(element);
		}
		else {
			add(_nodeList, element);
		}
	}

	// index may not be within the begin end bounds after a take()
	// execute resetIndex()
	resetIndex(_nodeList);

	return returnValue;
}
