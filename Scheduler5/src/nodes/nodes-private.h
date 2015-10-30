/*
 * nodes-private.h
 *
 *  Created on: Jul 8, 2015
 *      Author: Alan
 */

#ifndef SRC_NODES_NODES_PRIVATE_H_
#define SRC_NODES_NODES_PRIVATE_H_

/***********************************/
/*** class and object structures ***/

struct Node {
	const struct Object _;	// should be of Object
	id_t			nodeID;
	nodeType_t		nodeType;
	state_t			nodeState;
	value_t			nodeValue;
	runCount_t		runCount;
	struct Node *	linkedNode;
	struct Node *	linkedMessage;
};

struct NodeClass {
	const struct Class 	_;	// should be superclass: Class or "somethingClass"
	id_t  		(* setID)    	(const void * _self, const id_t _id);
	id_t  		(* getID)    	(const void * _self);

	nodeType_t  (* setType)  	(const void * _self, const nodeType_t _type);
	nodeType_t  (* getType)  	(const void * _self);

	state_t  	(* setState) 	(const void * _self, const state_t _state);
	state_t  	(* getState) 	(const void * _self);

	value_t  	(* setValue) 	(const void * _self, const state_t _value);

	value_t  	(* getValue) 	(const void * _self);

	runCount_t	(*setRunCount)(const void * _self, const runCount_t runCount);
	runCount_t	(*getRunCount)(const void * _self);

	runCount_t	(*incrementRunCount)(const void * _self);
	runCount_t	(*decrementRunCount)(const void * _self);

	void *    	(*setLinkedNode)(const void * _self, const void * _linkedNode);
	void *    	(*getLinkedNode)(const void * _self);

	void *    	(*setLinkedMessage)(const void * _self, const void * _linkedMessage);
	void *    	(*getLinkedMessage)(const void * _self);

	void * 		(*getPtrToSelfIfReadyToRun)(const void * _self);

};


/*****************************/
/****** Constructors  ********/

void * NodeClass_ctor(void * _self, va_list * app);
void * 	    Node_ctor(void * _self, va_list * app);

/***********************************/
/****** Overload Functions  ********/

// overloaded Object methods
void *        Node_dtor  (      void * _self);
void *        Node_copy  (      void * _self, const void * _copyFromObject);
equal_t       Node_equal (const void * _self, const void * _comparisonObject);
void *        Node_config(const void * _self, const void * _master);
puto_return_t Node_puto  (const void * _self, FILE * _fp);


/**********************************/
/***********  new methods  ********/
/**********************************/

id_t super_setID(const void * _class, const void * _self, const id_t _id);
id_t  Node_setID(                     const void * _self, const id_t _id);

id_t super_getID(const void *   _class, const void * _self);
id_t  Node_getID(                       const void * _self);

nodeType_t super_setType(const void * _class,
		                 const void * _self, const nodeType_t _type);
nodeType_t  Node_setType(const void * _self, const nodeType_t _type);

nodeType_t super_getType(const void * _class, const void * _self);
nodeType_t  Node_getType(                     const void * _self);

state_t super_setState(const void * _class,
		               const void * _self, const state_t _state);
state_t  Node_setState(const void * _self, const state_t _state);

state_t super_getState(const void * _class, const void * _state);
state_t  Node_getState(                     const void * _state);

value_t super_setValue(const void * _class,
		               const void * _self, const state_t _value);
value_t  Node_setValue(const void * _self, const state_t _value);

value_t super_getValue(const void * _class, const void * _self);
value_t  Node_getValue(                     const void * _self);

runCount_t super_setRunCount(const void * _class,
		                     const void * _self, const runCount_t runCount);
runCount_t  Node_setRunCount(const void * _self, const runCount_t runCount);

runCount_t super_getRunCount(const void * _class, const void * _self);
runCount_t  Node_getRunCount(                     const void * _self);

runCount_t super_incrementRunCount(const void * _class, const void * _self);
runCount_t  Node_incrementRunCount(                     const void * _self);

runCount_t super_decrementRunCount(const void * _class, const void * _self);
runCount_t  Node_decrementRunCount(                     const void * _self);


void * super_setLinkedNode(const void * _class, const void * _self, const void * _linkedNode);
void * Node_setLinkedNode (                     const void * _self, const void * _linkedNode);

void * super_getLinkedNode(const void * _class, const void * _self);
void *  Node_getLinkedNode(                     const void * _self);


void * super_getPtrToSelfIfReadyToRun(const void * _class, const void * _self);
void * Node_getPtrToSelfIfReadyToRun (                     const void * _self);


void * super_setLinkedMessage(const void * _class, const void * _self, const void * _linkedMessage);
void * Node_setLinkedMessage(                      const void * _self, const void * _linkedMessage);

void * super_getLinkedMessage(const void * _class, const void * _self);
void *  Node_getLinkedMessage(                     const void * _self);

#endif /* SRC_NODES_NODES_PRIVATE_H_ */
