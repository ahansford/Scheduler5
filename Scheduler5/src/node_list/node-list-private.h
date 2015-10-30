/*
 * nodes-list-private.h
 *
 *  Created on: Jul 15, 2015
 *      Author: Alan
 */

#ifndef SRC_NODES_LIST_NODES_LIST_PRIVATE_H_
#define SRC_NODES_LIST_NODES_LIST_PRIVATE_H_

/***********************************/
/*** class and object structures ***/

struct NodeList {
	const struct List _;	// should the superclass
	//id_t		NodeListID;
};


struct NodeListClass {
	const struct ListClass 	_;
	containts_t (* containsNodeID)    (const void * _self,  const id_t _id);
	id_t    	(* getAvailableNodeID)(const void * _self);
	id_t    	(* removeAllNodesWithID) (const void * _self,  const id_t _id);
	void *		(* incrementGetReadyToRun) (const void * _self);


	//id_t		(* getNodeID)   (const void * _self, const void * _nodePTR);


	//nodeType_t 	(* setNodeType) (const void * _self, const id_t _id, const nodeType_t _type);
	//nodeType_t 	(* getNodeType) (const void * _self, const id_t _id);


	//state_t		(* setNodeState)(const void * _self, const id_t _id, const state_t _state);
	//state_t		(* getNodeState)(const void * _self, state_t _state);
	//value_t		(* setNodeValue)(const void * _self, const id_t _id, const value_t _value);
	//value_t 	(* getNodeValue)(const void * _self, const id_t _id);

	id_t		(* nodeConfig)	(const void * _self,
			                     const id_t _id,
								 const id_t _masterID);
	//void *		(* )
};


/*****************************/
/****** Constructors  ********/

void * NodeListClass_ctor(void * _self, va_list * app);
void * NodeList_ctor     (void * _self, va_list * app);

/***********************************/
/****** Overload Functions  ********/
void *        NodeList_dtor (      void * _self);
void *        NodeList_copy (      void * _copyToNodeList,
			                 const void * _copyFromNodeList);
equal_t       NodeList_equal(const void * _self,
		                     const void * _comparisonObject);
puto_return_t NodeList_puto (const void * _self, FILE * _fp);


/**********************************/
/**********  new methods  *********/

id_t    super_getAvailableNodeID(const void * _class, const void * _self);
id_t NodeList_getAvailableNodeID(                     const void * _self);

containts_t    super_containsNodeID(const void * _class,
		                            const void * _self, const id_t _id);
containts_t NodeList_containsNodeID(const void * _self, const id_t _id);

id_t    super_removeAllNodesWithID(const void * _class,
		                           const void * _self, const id_t _id);
id_t NodeList_removeAllNodesWithID(const void * _self, const id_t _id);

void *    super_incrementGetReadyToRun(const void * _class, const void * _self);
void * NodeList_incrementGetReadyToRun(                     const void * _self);

//id_t    super_getNodeID(const void * _class,  const void * _self, const void * _nodePTR);
//id_t NodeList_getNodeID(const void * _self, const void * _nodePTR);

//nodeType_t    super_setNodeType(const void * _class, const void * _self, const id_t _id, const nodeType_t _type);
//nodeType_t NodeList_setNodeType(const void * _self, const id_t _id, const nodeType_t _type);


/*
nodeType_t    super_getNodeType(const void * _class, const void * _self, const id_t _id);
nodeType_t NodeList_getNodeType(const void * _self, const id_t _id);

state_t    super_setNodeState(const void * _class, const void * _self, const id_t _id, const state_t _state);
state_t NodeList_setNodeState(const void * _self, const id_t _id, const state_t _state);

state_t    super_getNodeState(const void * _class, const void * _self, const id_t _id);
state_t NodeList_getNodeState(const void * _self, const id_t _id);

value_t    super_setNodeValue(const void * _class, const void * _self, const id_t _id, const value_t _value);
value_t NodeList_setNodeValue(const void * _self, const id_t _id, const value_t _value);

value_t    super_getNodeValue(const void * _class, const void * _self, const id_t _id);
value_t NodeList_getNodeValue(const void * _self, const id_t _id);

id_t    super_nodeConfig(const void * _class, const void * _self, const id_t _id, const id_t _masterID);
id_t NodeList_nodeConfig(const void * _self, const id_t _id, const id_t _masterID);
*/
#endif /* SRC_NODES_LIST_NODES_LIST_PRIVATE_H_ */
