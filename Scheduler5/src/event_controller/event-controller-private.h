/*
 * event-controller-private.h
 *
 *  Created on: Aug 15, 2015
 *      Author: alhansfo
 */

#ifndef SRC_EVENT_CONTROLLER_EVENT_CONTROLLER_PRIVATE_H_
#define SRC_EVENT_CONTROLLER_EVENT_CONTROLLER_PRIVATE_H_

struct NodeList * myEventList;
struct NodeList * myNodeList;
struct Time *     myTime;

#ifndef MIN_LIST_BUFFER_SIZE
#define MIN_LIST_BUFFER_SIZE 4
#endif

void * nodeArray[MIN_LIST_BUFFER_SIZE];
void * eventArray[MIN_LIST_BUFFER_SIZE];

// executes a single message immediately
// behavior is generally from the source node to the target node
// returns targeted node pointer, which can be used to trigger node H/W update
// otherwise returns NULL on failure
// ASSUMES that there is a unique nodeID value for every node
void * executeMessage(void * _node);

// executes a single event
// behavior is generally from the event to the node
// returns targeted node pointer, which can be used to trigger node H/W update
// otherwise returns NULL on failure
// ASSUMES that there is a unique nodeID value for every node
void * executeEvent(void * _event);

// identifies and processes ready to run events within myEventList
int processAllReadyEvents(void);

// identifies and processes ready to run messages within myNodeList
int processAllReadyNodeMessages(void);

// called on every time increment
// updates runCount for events matching current time
// typically registered as a callback with the master Time structure
// ... setTimeUpdateIncrement(myTime, UPDATE_SECONDS);
// ... using setTime_OnSecondChange_cb(myTime, updateEventRunCounts);
// ... updateEventRunCounts() would not otherwise be called
struct Time * updateEventRunCounts(struct Time * _time);


#endif /* SRC_EVENT_CONTROLLER_EVENT_CONTROLLER_PRIVATE_H_ */
