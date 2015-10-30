/*
 * nodes-controller.h
 *
 *  Created on: Jul 15, 2015
 *      Author: Alan
 */

#ifndef SRC_NODES_LIST_NODES_LIST_H_
#define SRC_NODES_LIST_NODES_LIST_H_

/***********************************************/
/****** superclass and system includes  ********/

#include "..\..\src\lists\lists.h"
#include "..\..\src\nodes\nodes.h"
#include "..\..\src\nodes_X10\nodes-X10.h"
//#include "..\cross_compiler_defs.h"

/***********************************************/
/**** forward declarations and typedefs  *******/

// Use form: struct Node * aNewNodeList = new(NodeList);
extern const void * NodeList;
extern const void * NodeListClass;


/***********************************************/
/************ protected includes  **************/

#include "node-list-private.h"


/***********************************************/
/****** application interface functions  *******/

// MUST be called before any NodeList is created
void 	NodeList_init(void);


containts_t containsNodeID(const void * _self, const id_t _id);

// returns the first available unused ID starting with zero
id_t    getAvailableNodeID(const void * _self);

// to add nodes use normal "add(listPtr, nodePointer);"
//id_t addNode(const void * _self, const void * _element, const id_t _id);

// returns the specified id on success, else it returns ID_UNKNOWN
id_t    removeAllNodesWithID(const void * _self, const id_t _id);

// removes and deletes all nodes
// does not remove the Nodelist itself
void * removeAllNodes(void * _self);

// returns pointer to the next node that is ready to run, otherwise NULL
// this is not overloaded method from List.h
void * incrementGetReadyToRun(const void * _self);



// returns the specified id on success, else it returns ID_U                                                                       NKNOWN
//id_t    setNodeID   (const void * _self, const id_t _id, id_t _newID);
//id_t	getNodeID   (const void * _self, const void * _nodePTR);

// returns the requested node type if the call is successful
//nodeType_t setNodeType(const void * _self, const id_t _id, const nodeType_t _type);
//nodeType_t getNodeType(const void * _self, const id_t _id);

// returns the requested state if the call is successful
//state_t setNodeState(const void *  _self, const id_t    _id, const state_t _state);
//state_t	getNodeState(const void * _self, const id_t _id);

// returns the requested value if the call is successful, VALUE_UNKNOWN = 0
//value_t	setNodeValue(const void * _self, const id_t _id, const value_t _value);
//value_t	getNodeValue(const void * _self, const id_t _id);

// configures a specified _id node to match the _masterID node
// the master nodeID can NOT be set to ID_UNKNOWN
// configured nodes with unknown IDs will be assigned the next available ID
// return of ID_UNKNOWN indicates failure
// return of original ID not equal to ID_UNKNOWN, indicates success
// superclass of node config() methods should not copy the masterID
id_t  nodeConfig  (const void * _self, const id_t _id, const id_t _masterID);

#endif /* SRC_NODES_LIST_NODES_LIST_H_ */
