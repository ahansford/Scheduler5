/*
 * event-controller.h
 *
 *  Created on: Aug 15, 2015
 *      Author: alhansfo
 */

#ifndef SRC_EVENT_CONTROLLER_EVENT_CONTROLLER_H_
#define SRC_EVENT_CONTROLLER_EVENT_CONTROLLER_H_

/***********************************************/
/****** superclass and system includes  ********/

//#include "..\cross_compiler_defs.h"
#include "..\..\src\objects\objects.h"
#include "..\..\src\nodes\nodes.h"     // supports class registration
#include "..\..\src\node_list\node-list.h"
#include "..\..\src\times\times.h"     // supports class registration
#include "..\..\src\events\events.h"     // supports class registration

/***********************************************/
/**** forward declarations and typedefs  *******/

//extern struct NodeList * myEventList;
//extern struct NodeList * myNodeList;
//extern struct Times *    myTime;


/***********************************************/
/************ protected includes  **************/
/***** must be after externs and typedefs  *****/

#include "event-controller-private.h"


/***********************************************/
/****** application interface functions  *******/

// MUST be called before other event controller methods are called
// _mode will is typically set to either UPDATE_SECONDS or UPDATE_MINUTES
// returns targeted mode on success, otherwise INCREMENT_UNKNOWN
increment_t EventController_init(increment_t _mode);

// deletes all events, nodes and time
// deletes all lists
// must call EventController_init() to re-initialize
void EventController_destroy(void);

// identifies and processes ready to run events and nodes
// returns the number of processed items
int processAllReadyEventsAndNodeMessages(void);


// adds a pre-configured event to the event controller
// returns event pointer on success, otherwise NULL
void * EventController_addEvent(void * _event);

// removes specified event from the event controller
// returns event pointer on success, otherwise NULL
void * EventController_removeEvent(void * _event);

// returns pointers to all events in succession, returns NULL on empty list
void * EventController_incrementGetEvent(void);

// adds a pre-configured node to the event controller
// returns node pointer on success, otherwise NULL
void * EventController_addNode(void * _node);

// removes specified node from the event controller
// returns node pointer on success, otherwise NULL
void * EventController_removeNode(void * _node);

// returns pointers to all nodes in succession, returns NULL on empty list
void * EventController_incrementGetNode(void);

void EventController_removeAllAndReset(void);


#endif /* SRC_EVENT_CONTROLLER_EVENT_CONTROLLER_H_ */
