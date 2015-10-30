/*
                                                                                                             * nodes.h
 *
 *  Created on: Jul 8, 2015
 *      Author: Alan
 */

#ifndef SRC_NODES_NODES_H_
#define SRC_NODES_NODES_H_

/***********************************************/
/****** superclass and system includes  ********/

//#include "..\cross_compiler_defs.h"
#include "..\..\src\objects\objects.h"

/***********************************************/
/**** forward declarations and typedefs  *******/

extern const void * Node; // struct Node * new node = new(Node);
extern const void * NodeClass;

typedef enum state_t {
	STATE_UNKNOWN = -1,
	STATE_OFF     =  0,
	STATE_ON      =  1,
	STATE_VALUE
} state_t;

typedef enum id_t {
	ID_UNKNOWN = -1
} id_t;

typedef enum value_t {
	VALUE_UNKNOWN = 0
} value_t;

typedef enum nodeType_t {
	TYPE_UNKNOWN = -1
} nodeType_t;

typedef enum runCount_t {
	RUN_COUNT_UNKNOWN = -1
} runCount_t;

/***********************************************/
/************ protected includes  **************/

#include "nodes-private.h"


/***********************************************/
/****** application interface functions  *******/

// MUST be called before any new(Node) is created
void Node_init(void);

// returns the requested ID if the call is successful
id_t setID(const void * _self, const id_t _id);
id_t getID(const void * _self);

// returns the requested node type if the call is successful
nodeType_t setType(const void * _self, const nodeType_t _type);
nodeType_t getType(const void * _self);

// returns the requested state if the call is successful
state_t setState(const void * _self, const state_t _state);
state_t getState(const void * _self);

// returns the requested value if the call is successful
value_t setValue(const void * _self, const value_t _value);
value_t getValue(const void * _self);

// returns the number of times node or message should be processed
// TODO:  may want to remove these methods as being redundant
runCount_t setRunCount(const void * _self, const runCount_t runCount);
runCount_t getRunCount(const void * _self);

// returns the number of times a node or message should be processed
// return count is inclusive of most recent increment or decrement
// WARNING: runCount is a dynamic data member, should not be copied to nodes
runCount_t incrementRunCount(const void * _self);
runCount_t decrementRunCount(const void * _self);

// returns the pointer to the farthest linked node WITH a >0 run count
// will recursively traverse the links to the end of the chain
// place highest priority nodes as end of linked list for faster service
void * getPtrToSelfIfReadyToRun(const void * _self);

// returns void * to an additional chained/linked struct Node
// chained nodes can be used to pass messages, or hold additional data
void * setLinkedNode(const void * _self, const void * _linkedNode);
void * getLinkedNode(const void * _self);

// returns void * to an action message attached to parent node _self
// Each message is assumed to be associated with its one parent node
void * setLinkedMessage(const void * _self, const void * _linkedMessage);
void * getLinkedMessage(const void * _self);


#endif /* SRC_NODES_NODES_H_ */
