/*
 * nodes-list.c
 *
 *  Created on: Jul 15, 2015
 *      Author: Alan
 */


#include "..\..\src\objects\objects.h"	// safety include
#include "node-list.h"
#include "node-list-private.h"			// safety include
#include "..\..\src\nodes\nodes.h"		// safety include
//#include "..\..\src\nodes\nodes-private.h"


/*****************************/
/*** Support functions  ******/


static void * implement_NodeList_dtor(struct NodeList * _self);
static void * implement_NodeList_copy(struct NodeList * _copyToNodeList,
		                           const struct NodeList * _copyFromNodeList);
static equal_t       implement_NodeList_equal(const struct NodeList * _self,
		                                      const void * _comparisonObject);
static puto_return_t implement_NodeList_puto (const struct NodeList * _self,
		                                                       FILE * _fp);


static containts_t implement_NodeList_containsNodeID
                                           (const struct NodeList * _self,
										                 const id_t _id);

static id_t implement_NodeList_getAvailableNodeID
                                              (const struct NodeList * _self);

static id_t implement_NodeList_removeAllNodesWithID(struct NodeList * _self,
		                                            const id_t        _id);

static void * implement_NodeList_incrementGetReadyToRun(struct NodeList * _self);


/*
static id_t	implement_NodeList_getNodeID(struct NodeList * _self
		                             const void * _nodePTR);

static nodeType_t implement_NodeList_setNodeType(struct NodeList * _self,
		                      const id_t        _id,
					          const nodeType_t  _type);

static nodeType_t implement_NodeList_getNodeType(struct NodeList * _self,
		                                       const id_t _id);

static state_t implement_NodeList_setNodeState(struct NodeList * _self,
		                                  const id_t    _id,
					                      const state_t _state);
static state_t implement_NodeList_getNodeState(struct NodeList * _self,
		                                       const id_t        _id);

static value_t	implement_NodeList_setNodeValue(struct NodeList * _self,
		                                      const id_t _id,
										   const value_t _value);
static value_t	implement_NodeList_getNodeValue(struct NodeList * _self,
		                                      const id_t _id);


static id_t  implement_NodeList_nodeConfig(struct NodeList * _self, id_t _id, id_t _masterID);
*/

static void * getNodePointer  (const void * _self, const id_t _id);

static void implement_NodeList_registerKeyClasses(void);

/*****************************/
/**** INITIALIZATIONS  *******/

const void * NodeListClass = NULL;
const void * NodeList      = NULL;

// MUST be called before any NodeList is created
void NodeList_init(void)
{
	// call init for superclass if needed
	if (!List)      {List_init();}
	if (!ListClass) {List_init();}

	if (!Node)      {Node_init();}
	if (!NodeClass) {Node_init();}

	if (! NodeListClass) {
		NodeListClass = new(Class,  			// should be "Class"
						    "NodeListClass",	//  should be "SomethingClass"
						    Class,				// should be "Class"
						    sizeof(struct NodeListClass),  // size of self
						    ctor, NodeListClass_ctor,
						    0);  // Terminator
	}
	if (! NodeList) {
		NodeList = new(NodeListClass,			// SomethingClass from above
				    "NodeList",		  			// name like "Something"
				    List,						// "superclass(Something)"
				    sizeof(struct NodeList),	// size of self
					// Overloaded superclass functions
					// Remember to call superclass->method
				    ctor,     	NodeList_ctor,	//Something_ctor
					dtor,     	NodeList_dtor,	//Something_ctor
					copy,     	NodeList_copy,
					equal,    	NodeList_equal,
					puto,     	NodeList_puto,
					// New functions added in this class
					// Do not call superclass->method
					getAvailableNodeID, NodeList_getAvailableNodeID,
					//addNode, 			NodeList_addNode,
					removeAllNodesWithID, 		NodeList_removeAllNodesWithID,
					containsNodeID, 	NodeList_containsNodeID,

					incrementGetReadyToRun,    NodeList_incrementGetReadyToRun,
					//getNodeID,    NodeList_getNodeID,
					//setNodeType,  NodeList_setNodeType,
					//getNodeType,  NodeList_getNodeType,

					//setNodeState, NodeList_setNodeState,
					//getNodeState, NodeList_getNodeState,
					//setNodeValue, NodeList_setNodeValue,
					//getNodeValue, NodeList_getNodeValue,

					//nodeConfig,   NodeList_nodeConfig,
				    0); // terminator character
	}

	implement_NodeList_registerKeyClasses();

	return;
}


/*****************************/
/****** Constructors  ********/

void * NodeList_ctor(void * _self, va_list * app)
{
	// Add superclass class data members
	struct NodeList * self = super_ctor(NodeList, _self, app);

	// set default values for any new data members
	// self->nodeID    = ID_UNKNOWN;

	// Overwrite data member values with arg data
	// Only uncomment if all data members will be specified in new() command
	// ... this seems like an undue burden on the user.  Leave commented out
	// self->nodeID    = va_arg(* app, node_id_t);
	// self->nodeState = va_arg(* app, node_state_t);

	// NOTE: there are no new data members in NodeList
	return self;
}

void * NodeListClass_ctor(void * _self, va_list *app)
{
	// Add superclass class data and methods
	struct NodeListClass * self = super_ctor(NodeListClass, _self, app);

	// Initialize new function pointers to NULL
	typedef void (* voidf)();
	* (voidf *) & self->getAvailableNodeID     = NULL;
	* (voidf *) & self->removeAllNodesWithID   = NULL;
	* (voidf *) & self->containsNodeID         = NULL;
	* (voidf *) & self->incrementGetReadyToRun = NULL;
	//* (voidf *) & self->getNodeType    = NULL;
	//* (voidf *) & self->setNodeState   = NULL;
	//* (voidf *) & self->getNodeState   = NULL;
	//* (voidf *) & self->setNodeValue   = NULL;
	//* (voidf *) & self->getNodeValue   = NULL;
	//* (voidf *) & self->setNodeID      = NULL;
	//* (voidf *) & self->getNodeID      = NULL;
	//* (voidf *) & self->nodeConfig     = NULL;

	// Update any overloaded method function pointers
	// using style ... , methodSelector, overloadedFunctionPtr,
	voidf selector;

	#ifdef va_copy
		va_list ap;
		va_copy(ap, * app);
	#else
		va_list ap = * app;
	#endif

	// if non-NULL selector then grab next method pointer from arg list
	while ( (selector = va_arg(ap, voidf)) ) {

		voidf overloadedFunctionPtr  = va_arg(ap, voidf);

		// ObjectClass overloads:
		if (selector == (voidf) dtor)
			{* (voidf *) & self->_._.dtor = overloadedFunctionPtr; }

		if (selector == (voidf) copy)
			{* (voidf *) & self->_._.copy = overloadedFunctionPtr; }

		if (selector == (voidf) equal)
			{* (voidf *) & self->_._.equal = overloadedFunctionPtr; }

		// TODO: Update config as appropriate for Lists

		if (selector == (voidf) puto)
			{* (voidf *) & self->_._.puto = overloadedFunctionPtr; }

		// ListClass overloads:


		// New NodeListClass assignments:
		if (selector == (voidf) getAvailableNodeID)
			{* (voidf *) & self->getAvailableNodeID = overloadedFunctionPtr;}

		if (selector == (voidf) removeAllNodesWithID)
			{* (voidf *) & self->removeAllNodesWithID = overloadedFunctionPtr;}

		if (selector == (voidf) containsNodeID)
			{* (voidf *) & self->containsNodeID = overloadedFunctionPtr;}

		if (selector == (voidf) incrementGetReadyToRun)
			{* (voidf *) & self->incrementGetReadyToRun    = overloadedFunctionPtr;}
/*
		if (selector == (voidf) getNodeID)
			{* (voidf *) & self->getNodeID    = overloadedFunctionPtr;}

		if (selector == (voidf) setNodeType)
			{* (voidf *) & self->setNodeType = overloadedFunctionPtr;}

		if (selector == (voidf) getNodeType)
			{* (voidf *) & self->getNodeType = overloadedFunctionPtr;}

		if (selector == (voidf) setNodeState)
			{* (voidf *) & self->setNodeState = overloadedFunctionPtr;}

		if (selector == (voidf) getNodeState)
			{* (voidf *) & self->getNodeState = overloadedFunctionPtr;}

		if (selector == (voidf) setNodeValue)
			{* (voidf *) & self->setNodeValue = overloadedFunctionPtr;}

		if (selector == (voidf) getNodeValue)
			{* (voidf *) & self->getNodeValue = overloadedFunctionPtr;}

		if (selector == (voidf) nodeConfig)
			{* (voidf *) & self->nodeConfig    = overloadedFunctionPtr;}
*/
	#ifdef va_copy
		va_end(ap);
	#endif

	}  // end while

	return self;
}


/*****************************/
/**** Overloaded Methods  ****/

void * NodeList_dtor(void * _self)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	//void * localPtr = _self;
	//printf("     _self in NodeList_dtor: %p", _self);
	struct NodeList * self = cast(NodeList, _self);
	if ( self != _self) { printf("     _self: %p, does not match cast to self: %p in NodeList_dtor\n", _self, self); }

	if(self == NULL) {return NULL; } // fail

	// address local data members first
	//removeAllNodes(self);
	if ( implement_NodeList_dtor(self) == NULL) {return NULL; } // fail

	// call super method after local members are addressed
	// NOTE: classOf(self) calls into super will trigger looping
	if ( super_dtor(NodeList, _self) == NULL)   {return NULL; } // fail

	return _self;
}

void * NodeList_copy( void * _copyToNodeList, const void * _copyFromNodeList)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct NodeList * self = cast(NodeList, _copyToNodeList);
	if (self == NULL )          { return NULL; } //fail

	struct NodeList * copyFromObject = (void *)_copyFromNodeList;
	if (copyFromObject == NULL ) { return NULL; } // fail

	// WARNING: calling superclass in composite class is likely NOT correct
	//          complex classes should manage calls to superclass as needed
	//          within the implementation code

	// WARNING:  NodeList is a complex class, do not call superclass_copy

	// call super method first
	// NOTE: classOf(self) calls into super will trigger looping
	//if (super_copy(NodeList, self, copyFromObject) == NULL )
	//	{ return NULL; } // fail

	// address local components
	return implement_NodeList_copy(self, _copyFromNodeList);
}

equal_t NodeList_equal(const void * _self, const void * _comparisonObject)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct NodeList * self = cast(NodeList, _self);
	if (self == NULL )              { return OBJECT_UNEQUAL; } // fail

	struct NodeList * comparisonObject = cast(NodeList, _comparisonObject);
	if (comparisonObject == NULL )  { return OBJECT_UNEQUAL; } // fail

	// WARNING: calling superclass in composite class is likely NOT correct
	//          complex classes should manage calls to superclass as needed
	//          within the implementation code

	// WARNING:  NodeList is a complex class, do not call superclass_equal

	// NOTE: classOf(self) calls into super will trigger looping
	//if( super_equal(NodeList, self, comparisonObject) == OBJECT_UNEQUAL)
    //	{ return OBJECT_UNEQUAL; } // fail

	// address local components
	return implement_NodeList_equal(_self, _comparisonObject);
}

puto_return_t NodeList_puto(const void * _self, FILE * _fp)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct NodeList * self = cast(NodeList, _self);
	if (self == NULL )         { return PUTO_ERROR; } // fail

	// the call to super_puto() is managed within implement_List_puto()
	return implement_NodeList_puto(self, _fp);
}


/****************************************************************************/
/********  New functions for  class "NodeListClass"  ************************/
/****************************************************************************/

/**************************************/
/********  getAvailableNodeID  ********/

id_t getAvailableNodeID(const void * _self)
{
	const struct NodeListClass * class = classOf( cast(NodeList, _self) );
	if (class == NULL)                     { return ID_UNKNOWN; } // fail
	if (class->getAvailableNodeID == NULL) { return ID_UNKNOWN; } // fail
	return class->getAvailableNodeID(_self);
}

id_t super_getAvailableNodeID(const void * _class, const void * _self)
{
	// verify that NodeListClass is in the superclass chain of _class
	if ( ! isOfSuper(NodeListClass, _self) )
		{ return ID_UNKNOWN; } // fail
	const struct NodeListClass * superclass = super(_class);
	if ( superclass == NULL )
		{ return ID_UNKNOWN; }// fail
	if ( superclass->getAvailableNodeID == NULL )
		{ return ID_UNKNOWN; }// fail
	return superclass->getAvailableNodeID(_self);
}

id_t NodeList_getAvailableNodeID( const void * _self)
{
	// return first unused ID starting with zero
	struct List * self = cast(NodeList, _self);
	if( self == NULL ) { return ID_UNKNOWN; } // fail
	return implement_NodeList_getAvailableNodeID(_self);
}

/**************************************/
/*************  addNode  **************/
/*

id_t addNode(const void * _self, const void * _element, const id_t _id)
{
	return ID_UNKNOWN;
}

id_t super_addNode(const void * _class, const void * _self,
									const void * _element, const id_t _id)
{
	return ID_UNKNOWN;
}

id_t NodeList_addNode(const void * _self,
									const void * _element, const id_t _id)
{
	return ID_UNKNOWN;
}
*/

/**************************************/
/***********  removeNode  *************/

id_t removeAllNodesWithID(const void * _self, const id_t _id)
{
	const struct NodeListClass * class = classOf( cast(NodeList, _self) );
	if (class == NULL)                       { return ID_UNKNOWN; } // fail
	if (class->removeAllNodesWithID == NULL) { return ID_UNKNOWN; } // fail
	return class->removeAllNodesWithID(_self, _id);
}

id_t super_removeAllNodesWithID(const void * _class,
		                        const void * _self,
								const id_t   _id)
{
	// verify that NodeListClass is in the superclass chain of _class
	if ( ! isOfSuper(NodeListClass, _self) )
		{ return ID_UNKNOWN; } // fail
	const struct NodeListClass * superclass = super(_class);
	if ( superclass == NULL )
		{ return ID_UNKNOWN; } // fail
	if ( superclass->removeAllNodesWithID == NULL )
		{ return ID_UNKNOWN; } // fail
	return superclass->removeAllNodesWithID(_self, _id);
}

id_t NodeList_removeAllNodesWithID(const void * _self, const id_t _id)
{
	// remove from the list the node with the specified ID
	struct NodeList * self = cast(NodeList, _self);
	if( self == NULL ) { return ID_UNKNOWN; } // fail
	return implement_NodeList_removeAllNodesWithID(self, _id);
}


/**************************************/
/***********  containsNodeID  *********/

containts_t containsNodeID(const void * _self,  const id_t _id)
{
	const struct NodeListClass * class = classOf( cast(NodeList, _self) );
	if (class == NULL)                 { return DOES_NOT_CONTAIN; } // fail
	if (class->containsNodeID == NULL) { return DOES_NOT_CONTAIN; } // fail
	return class->containsNodeID(_self, _id);
}

containts_t super_containsNodeID(const void * _class,
						  const void * _self,
						  const id_t   _id)
{
	// verify that NodeListClass is in the superclass chain of _class
	if ( ! isOfSuper(NodeListClass, _self) )
		{ return DOES_NOT_CONTAIN; } // fail
	const struct NodeListClass * superclass = super(_class);
	if ( superclass == NULL )
		{ return DOES_NOT_CONTAIN; } // fail
	if ( superclass->containsNodeID == NULL )
		{ return DOES_NOT_CONTAIN; } // fail
	return superclass->containsNodeID(_self, _id);
}

containts_t NodeList_containsNodeID(const void * _self, const id_t _id)
{
	// remove from the list the node with the specified ID
	struct NodeList * self = cast(NodeList, _self);
	if( self == NULL ) { return DOES_NOT_CONTAIN; } // fail
	return implement_NodeList_containsNodeID(self, _id);
}


/**************************************/
/***********  getIncrementReadyToRun  **************/

void * incrementGetReadyToRun(const void * _self)
{
	const struct NodeListClass * class = classOf( cast(NodeList, _self) );
	if (class == NULL)            { return NULL; } // fail
	if (class->incrementGetReadyToRun == NULL) { return NULL; } // fail
	return class->incrementGetReadyToRun(_self);
}

void * super_incrementGetReadyToRun(const void * _class, const void * _self)
{
	// verify that NodeListClass is in the superclass chain of _class
	if ( ! isOfSuper(NodeListClass, _self) ) { return NULL; } // fail
	const struct NodeListClass * superclass = super(_class);
	if ( superclass == NULL )                { return NULL; } // fail
	if ( superclass->incrementGetReadyToRun == NULL )     { return NULL; } // fail
	return superclass->incrementGetReadyToRun(_self);
}

void * NodeList_incrementGetReadyToRun(const void * _self)
{
	struct NodeList * self = cast(NodeList, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_NodeList_incrementGetReadyToRun(self);
}


/**************************************/
/***********  getNodeID  **************/
/*
id_t getNodeID(const void * _self, const void * _nodePTR)
{
	const struct NodeListClass * class = classOf( cast(NodeList, _self) );
	if (class == NULL)            { return ID_UNKNOWN; } // fail
	if (class->getNodeID == NULL) { return ID_UNKNOWN; } // fail
	return class->getNodeID(_self, _nodePTR);
}

id_t super_getNodeID(const void * _class,
					 const void * _self,
					 const void * _nodePTR)
{
	// verify that NodeListClass is in the superclass chain of _class
	if ( ! isOfSuper(NodeListClass, _self) ) { return ID_UNKNOWN; } // fail
	const struct NodeListClass * superclass = super(_class);
	if ( superclass == NULL )                { return ID_UNKNOWN; } // fail
	if ( superclass->getNodeID == NULL )     { return ID_UNKNOWN; } // fail
	return superclass->getNodeID(_self, _nodePTR);
}

id_t NodeList_getNodeID(const void * _self, const void * _nodePTR)
{
	struct NodeList * self = cast(NodeList, _self);
	if( self == NULL ) { return ID_UNKNOWN; } // fail
	return implement_NodeList_getNodeID(self, _nodePTR);
}
*/

/**************************************/
/***********  setNodeType  **************/
/*
nodeType_t setNodeType(const void * _self, const id_t _id, nodeType_t _type)
{
	const struct NodeListClass * class = classOf( cast(NodeList, _self) );
	if (class == NULL)              { return TYPE_UNKNOWN; } // fail
	if (class->setNodeType == NULL) { return TYPE_UNKNOWN; } // fail
	return class->setNodeType(_self, _id, _type);
}

nodeType_t super_setNodeType(const void * _class,
					         const void * _self,
							 const id_t   _id,
							 nodeType_t   _newType)
{
	// verify that NodeListClass is in the superclass chain of _class
	if ( ! isOfSuper(NodeListClass, _self) ) { return ID_UNKNOWN; } // fail
	const struct NodeListClass * superclass = super(_class);
	if ( superclass == NULL )              { return TYPE_UNKNOWN; } // fail
	if ( superclass->setNodeType == NULL ) { return TYPE_UNKNOWN; } // fail
	return superclass->setNodeType(_self, _id, _newType);
}

nodeType_t NodeList_setNodeType(const void * _self,
		                        const id_t   _id,
								nodeType_t   _type)
{
	struct NodeList * self = cast(NodeList, _self);
	if( self == NULL ) { return TYPE_UNKNOWN; } // fail
	return implement_NodeList_setNodeType(self, _id, _type);
}
*/

/**************************************/
/***********  getNodeType  **************/
/*
nodeType_t getNodeType(const void * _self, id_t _id)
{
	const struct NodeListClass * class = classOf( cast(NodeList, _self) );
	if (class == NULL)              { return TYPE_UNKNOWN; } // fail
	if (class->getNodeType == NULL) { return TYPE_UNKNOWN; } // fail
	return class->getNodeType(_self, _id);
}

nodeType_t super_getNodeType(const void * _class,
		                     const void * _self,
							 const id_t   _id)
{
	// verify that NodeListClass is in the superclass chain of _class
	if ( ! isOfSuper(NodeListClass, _self) ) { return ID_UNKNOWN; } // fail
	const struct NodeListClass * superclass = super(_class);
	if ( superclass == NULL )              { return TYPE_UNKNOWN; } // fail
	if ( superclass->getNodeType == NULL ) { return TYPE_UNKNOWN; } // fail
	return superclass->getNodeType(_self, _id);
}

nodeType_t NodeList_getNodeType(const void * _self, id_t _id)
{
	struct NodeList * self = cast(NodeList, _self);
	if( self == NULL ) { return TYPE_UNKNOWN; } // fail
	return implement_NodeList_getNodeType(self, _id);
}

*/

/**************************************/
/**********  setNodeState  ************/
/*
state_t setNodeState(const void * _self, const id_t _id, const state_t _state)
{
	const struct NodeListClass * class = classOf( cast(NodeList, _self) );
	if (class == NULL)               { return STATE_UNKNOWN; } // fail
	if (class->setNodeState == NULL) { return STATE_UNKNOWN; } // fail
	return class->setNodeState(_self, _id, _state);
}

state_t super_setNodeState(const void * _class,
		const void * _self, const id_t _id, const state_t _state)
{
	// verify that ListClass is in the superclass chain of _class
	if ( ! isOfSuper(ListClass, _self) )    { return STATE_UNKNOWN; } // fail
	const struct NodeListClass * superclass = super(_class);
	if ( superclass == NULL )               { return STATE_UNKNOWN; } // fail
	if ( superclass->setNodeState == NULL ) { return STATE_UNKNOWN; } // fail
	return superclass->setNodeState(_self, _id, _state);
}

state_t NodeList_setNodeState(const void *  _self,
							  const id_t    _id,
							  const state_t _state)
{
	struct NodeList * self = cast(NodeList, _self);
	if( self == NULL ) { return TYPE_UNKNOWN; } // fail
	return implement_NodeList_setNodeState(self, _id, _state);
}
*/

/**************************************/
/**********  getNodeState  ************/
/*
state_t getNodeState(const void * _self, const id_t _id)
{
	const struct NodeListClass * class = classOf( cast(NodeList, _self) );
	if (class == NULL)               { return STATE_UNKNOWN; } // fail
	if (class->getNodeState == NULL) { return STATE_UNKNOWN; } // fail
	return class->getNodeState(_self, _id);
}

state_t super_getNodeState(const void * _class,
						   const void * _self,
						   const id_t _id)
{
	// verify that ListClass is in the superclass chain of _class
	if ( ! isOfSuper(ListClass, _self) )    { return STATE_UNKNOWN; } // fail
	const struct NodeListClass * superclass = super(_class);
	if ( superclass == NULL )               { return STATE_UNKNOWN; } // fail
	if ( superclass->getNodeState == NULL ) { return STATE_UNKNOWN; } // fail
	return superclass->getNodeState(_self, _id);
}

state_t NodeList_getNodeState(const void * _self, const id_t _id)
{
	struct NodeList * self = cast(NodeList, _self);
	if( self == NULL ) { return STATE_UNKNOWN; } // fail
	return implement_NodeList_getNodeState(self, _id);
}

*/
/**************************************/
/**********  setNodeValue  ************/
/*
value_t setNodeValue(const void * _self,
		             const id_t _id,
					 const value_t _value)
{
	const struct NodeListClass * class = classOf( cast(NodeList, _self) );
	if (class == NULL)               { return VALUE_UNKNOWN; } // fail
	if (class->setNodeValue == NULL) { return VALUE_UNKNOWN; } // fail
	return class->setNodeValue(_self, _id, _value);
}

value_t super_setNodeValue(const void * _class,
		const void * _self, const id_t _id, const value_t _value)
{
	// verify that ListClass is in the superclass chain of _class
	if ( ! isOfSuper(ListClass, _self) )    { return VALUE_UNKNOWN; } // fail
	const struct NodeListClass * superclass = super(_class);
	if ( superclass == NULL )               { return VALUE_UNKNOWN; } // fail
	if ( superclass->setNodeValue == NULL ) { return VALUE_UNKNOWN; } // fail
	return superclass->setNodeValue(_self, _id, _value);
}

value_t NodeList_setNodeValue(const void * _self,
							  const id_t _id,
							  const value_t _value)
{
	struct NodeList * self = cast(NodeList, _self);
	if( self == NULL ) { return VALUE_UNKNOWN; } // fail
	return implement_NodeList_setNodeValue(self, _id, _value);
}
*/

/**************************************/
/**********  getNodeValue  ************/
/*
value_t getNodeValue(const void * _self, const id_t _id)
{
	const struct NodeListClass * class = classOf( cast(NodeList, _self) );
	if (class == NULL)               { return VALUE_UNKNOWN; } // fail
	if (class->getNodeValue == NULL) { return VALUE_UNKNOWN; } // fail
	return class->getNodeValue(_self, _id);
}

value_t super_getNodeValue(const void * _class,
						   const void * _self,
						   const id_t _id)
{
	// verify that ListClass is in the superclass chain of _class
	if ( ! isOfSuper(ListClass, _self) )    { return VALUE_UNKNOWN; } // fail
	const struct NodeListClass * superclass = super(_class);
	if ( superclass == NULL )               { return VALUE_UNKNOWN; } // fail
	if ( superclass->getNodeValue == NULL ) { return VALUE_UNKNOWN; } // fail
	return superclass->getNodeValue(_self, _id);
}

value_t NodeList_getNodeValue(const void * _self, const id_t _id)
{
	struct NodeList * self = cast(NodeList, _self);
	if( self == NULL ) { return ID_UNKNOWN; } // fail
	return implement_NodeList_getNodeValue(self, _id);
}
*/


/**************************************/
/**********  nodeConfig  **************/
/*
id_t nodeConfig(const void * _self, const id_t _id, const id_t _masterID)
{
	const struct NodeListClass * class = classOf( cast(NodeList, _self) );
	if (class == NULL)             { return ID_UNKNOWN; } // fail
	if (class->nodeConfig == NULL) { return ID_UNKNOWN; } // fail
	return class->nodeConfig(_self, _id, _masterID);
}

id_t super_nodeConfig(const void * _class,
						const void * _self,
						const id_t _id,
						const id_t _masterID)
{
	// verify that ListClass is in the superclass chain of _class
	if ( ! isOfSuper(ListClass, _self) )  { return VALUE_UNKNOWN; } // fail
	const struct NodeListClass * superclass = super(_class);
	if ( superclass == NULL )             { return VALUE_UNKNOWN; } // fail
	if ( superclass->nodeConfig == NULL ) { return VALUE_UNKNOWN; } // fail
	return superclass->nodeConfig(_self, _id, _masterID);
}

id_t NodeList_nodeConfig(const void * _self,
		                 const id_t   _id,
						 const id_t   _masterID)
{
	struct NodeList * self = cast(NodeList, _self);
	if( self == NULL ) { return ID_UNKNOWN; } // fail
	return implement_NodeList_nodeConfig(self, _id, _masterID);
}
*/

/**************************************/
/********  support methods  ************/

static void * implement_NodeList_dtor(struct NodeList * _self)
{
	//printf("    item count before removing nodes in implement_NodeList_dtor: %i\n", getItemCount(_self));
	removeAllNodes(_self);
	//printf("    item count after  removing nodes in implement_NodeList_dtor: %i\n", getItemCount(_self));

	return (void *)_self;
}

void * implement_NodeList_copy(      struct NodeList * _copyToNodeList,
		                       const struct NodeList * _copyFromNodeList)
{
	// create copyFrom and copyTo node pointers
	struct Node * originalElement = NULL;
	struct Node * copyToElement   = NULL;

	// use resetIndex() to reset currentIndex pointer and test for empty list
	if ( resetIndex(_copyFromNodeList) == NULL ) { return NULL; } // fail

	// loop through the copyFrom list to copy each node and add to copyTo list
	int i;
	int localItemCount;
	localItemCount = getItemCount(_copyFromNodeList);
	for (i = 0; i < localItemCount; i++) {

		// get original node
		originalElement= incrementGet(_copyFromNodeList);
		if ( originalElement == NULL )
			{ return NULL; } // fail: no original

		// allocate memory for new node
		copyToElement = new(Node);
		if  ( copyToElement == NULL )
			{ return NULL; } // fail new node create

		// copy original node content to new node
		if ( copy(copyToElement, originalElement) == NULL )
			{ return NULL; } // fail to add node to copyTo list

		// add new node to new list
		if ( add(_copyToNodeList, copyToElement) == _copyToNodeList )
			{ return NULL; } // fail to add new node

	} // end of for loop

	return _copyToNodeList;
}

equal_t implement_NodeList_equal(const struct NodeList * _self,
								 const void * _comparisonObject)
{
	struct Node * testElementPTR        = NULL;
	struct Node * comparisonElementPTR  = NULL;
	equal_t returnCode;
	equal_t innerLoopReturnCode;

	// loop through the test and comparison lists looking for matching nodeIDs
	int i, j;
	int testItemCount, comparisonItemCount;
	testItemCount       = getItemCount(_self);
	comparisonItemCount = getItemCount(_comparisonObject);
	returnCode          = OBJECT_EQUAL;

	// loop through the comparison list items: outer loop
	for (i = 0; i < comparisonItemCount; i++) {

		// get a pointer to an item in the master list
		comparisonElementPTR = incrementGet(_comparisonObject);

		// determine the ID of this item from the master list
		//comparisonElementID  = getID(comparisonElementPTR);

		// loop through the test list items:  inner loop
		innerLoopReturnCode = OBJECT_UNEQUAL;
		for ( j = 0; j < testItemCount; j++) {

			// get node pointer from the test list
			testElementPTR= incrementGet(_self);

			// check for matching nodeID in _self list
			if ( equal(testElementPTR, comparisonElementPTR ) == OBJECT_EQUAL )
				{ innerLoopReturnCode = OBJECT_EQUAL; } // matching node found

		}  // end of inner for-loop

		// check for failure to find matching node in the inner loop
		if (innerLoopReturnCode == OBJECT_UNEQUAL)
			{ returnCode = OBJECT_UNEQUAL; } // fail

	}  // end of outer for-loop

	return returnCode;
}

puto_return_t implement_NodeList_puto
								(const struct NodeList * _self, FILE * _fp)
{
	puto_return_t printReturnCode = PUTO_SUCCESS;

	struct NodeList * self = cast(NodeList, _self);

	// print local data members
	fprintf(_fp, "\n Local data members managed in NodeList:\n");

	fprintf(_fp, "  ... no data members are stored in NodeList\n");

	/* if (PUTO_ERROR == fprintf(_fp, "  Node ID:    %i\n",
							  implement_getID(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected */

	fprintf(_fp, "\n New methods added in NodeListClass:\n");

	if (PUTO_ERROR == fprintf(_fp, " containsNodeID:     %p\n",
			containsNodeID ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, " getAvailableNodeID: %p\n",
			getAvailableNodeID ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	/* if (PUTO_ERROR == fprintf(_fp, " addNode:              %p\n", addNode ))
		{ printReturnCode = PUTO_ERROR;  } // error detected  */

	if (PUTO_ERROR == fprintf(_fp, " removeAllNodesWithID: %p\n", removeAllNodesWithID ))
		{ printReturnCode = PUTO_ERROR;  } // error detected
/*
	if (PUTO_ERROR == fprintf(_fp, " setNodeID:          %p\n", setNodeID ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, " getNodeID:          %p\n", getNodeID ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, " setNodeType:        %p\n", setNodeType ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, " getNodeType:        %p\n", getNodeType ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, " setNodeState:       %p\n", setNodeState ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, " getNodeState:       %p\n", getNodeState ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, " setNodeValue:       %p\n", setNodeValue ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, " getNodeValue:       %p\n", getNodeValue ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, " nodeConfig:         %p\n", nodeConfig ))
		{ printReturnCode = PUTO_ERROR;  } // error detected
*/
	fprintf(_fp, "\n Overloaded Class methods in NodeListClass:\n" );
// ********************
	if (PUTO_ERROR == fprintf(_fp,
		" NodeList_ctor:      %p\n", NodeList_ctor ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp,
		" NodeList_dtor:      %p\n", NodeList_dtor ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp,
		" NodeList_copy:      %p\n", NodeList_copy ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp,
		" NodeList_equal:     %p\n", NodeList_equal ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp,
		" NodeList_puto:      %p\n",  NodeList_puto))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp,
		" NodeListClass_ctor: %p\n", NodeListClass_ctor))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	fprintf(_fp, "\n" );

	// print superclass data
	fprintf(_fp, "%s class data (the superclass  for NodeList):\n",
			     ((struct Object * )self)->class->superclass->className );
	// WARNING: classOf(self) calls into super will trigger looping
	if (printReturnCode == super_puto(NodeList, self, _fp))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	fprintf(_fp, "NodeList class data:\n" );
	if (printReturnCode == Class_puto(NodeList, _fp))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	fprintf(_fp, "\nNodeListClass class:\n" );
	if (printReturnCode == puto(NodeListClass, _fp))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	return printReturnCode;
}

static containts_t implement_NodeList_containsNodeID
                               (const struct NodeList * _self, const id_t _id)
{
	struct Node * elementPTR;
	elementPTR = getNodePointer(_self, _id);
	return contains(_self, elementPTR);
}

static id_t implement_NodeList_getAvailableNodeID
                                               (const struct NodeList * _self)
{
	#ifndef MAX_NODE_COUNT
		#define MAX_NODE_COUNT 5000
	#endif

	id_t nodeID = ID_UNKNOWN;
	do {
		nodeID++;

		if (nodeID >= MAX_NODE_COUNT)
				{ nodeID = ID_UNKNOWN; break; } // looping protection
	} while ( containsNodeID(_self,  nodeID) );

	return nodeID;
}

static id_t implement_NodeList_removeAllNodesWithID
                                     (struct NodeList * _self, const id_t _id)
{
	// check every item in the list for an ID match
	int localItemCount = getItemCount(_self);
	struct Node * element = NULL;
	int i;
	id_t returnID = ID_UNKNOWN;

	for (i = 0; i < localItemCount; i++)
	{
		element = List_takeFirst(_self);
		if (element->nodeID == _id)
		{
			// This is the element to remove
			// set returnValue, but do NOT add back to list
			returnID = _id;
		}
		else
		{
			// this is NOT the correct element, ADD it back to list
			add(_self, element);
		}
	}  // end for loop

	// WARNING: index may not be within the begin end bounds after a take()
	// resetIndex(_self);
	return returnID;
}

static void * implement_NodeList_incrementGetReadyToRun(struct NodeList * _self)
{
	struct List * self = (void *)_self;
	int itemCount = getItemCount(self);
	if(itemCount <= 0) {return NULL;} // empty list

	// check every item in the list for a run count greater then zero
	int i;
	struct Node * element = NULL;
	for ( i = 0; i < itemCount; i++ ) {
		element = incrementGet(self);
		if ( getRunCount(element) > 0 ) { return element;}  // success
	}
	return NULL;  // nothing ready to run
}

/*
static id_t	implement_NodeList_getNodeID(struct NodeList * _self,
		                             const void * _nodePTR)
{
	struct Node * nodePTR = (void *)_nodePTR;
	return getID(nodePTR);
}

static nodeType_t implement_NodeList_setNodeType(struct NodeList * _self,
		                      const id_t        _id,
					          const nodeType_t  _type)
{
	struct Node * element = getNodePointer(_self, _id);
	return setType(element, _type);
}

static nodeType_t implement_NodeList_getNodeType(struct NodeList * _self,
		                                       const id_t _id)
{
	struct Node * element = getNodePointer(_self, _id);
	return getType(element);
}

static state_t implement_NodeList_setNodeState(struct NodeList * _self,
		                                  const id_t    _id,
					                      const state_t _state)
{
	struct Node * element = getNodePointer(_self, _id);
	return setState(element, _state);
}

static state_t implement_NodeList_getNodeState
                                     (struct NodeList * _self, const id_t _id)
{
	struct Node * element = getNodePointer(_self, _id);
	return getState(element);
}

static value_t	implement_NodeList_setNodeValue(struct NodeList * _self,
		                                      const id_t _id,
										   const value_t _value)
{
	struct Node * element = getNodePointer(_self, _id);
	return setValue(element, _value);
}

static value_t	implement_NodeList_getNodeValue(struct NodeList * _self,
		                                      const id_t _id)
{
	struct Node * element = getNodePointer(_self, _id);
	return getValue(element);
}

static id_t implement_NodeList_nodeConfig
                           (struct NodeList * _self, id_t _id, id_t _masterID)
{
	struct Node * copyToPTR  = getNodePointer(_self, _id);
	struct Node * masterPTR  = getNodePointer(_self, _masterID);
	id_t          originalID = _id;

	// test for inadvertent config of master node, or two unknown IDs
	if ( getID(copyToPTR) == getID(masterPTR) )
		{ return ID_UNKNOWN ; } // fail

	// execute basic node configuration
	struct Node * configReturnPointer = config(copyToPTR, masterPTR);

	// regardless of success or failure of config(), avoid reuse of nodeIDs
	// update the configured node IDs BEFORE determining return action

	// configured node ID should NOT match the master ID
	// reassign the original ID if needed
	if ( getID(copyToPTR) == getID(masterPTR) )
		{ setID(copyToPTR, originalID ); }

	// if the original nodeID happened to be UNKNOWN, assign a new ID
	if ( getID(copyToPTR) == ID_UNKNOWN )
		{ setID(copyToPTR, getAvailableNodeID(_self) ); }

	//check the return result of node config(), return ID_UNKNOWN on fail
	if ( configReturnPointer == NULL ) {
		return ID_UNKNOWN; }    // fail
	else {
		return getID( copyToPTR ); }  // expected path

	return ID_UNKNOWN; // fail safe
}
*/

static void * getNodePointer(const void * _self, const id_t _id)
{
	struct List * self = (void *)_self;
	struct Node * element = NULL;
	int i;

	int itemCount = getItemCount(self);
	if(itemCount <= 0) {return NULL;} // empty list

	// check every item in the list for an ID match
	for (i = 0; i < itemCount; i++) {
		element = take(self);
		add(self, element);
		if (element->nodeID == _id) { return element;}
	}

	return NULL;  // no match found

}

static void implement_NodeList_registerKeyClasses(void)
{
	// registers Object, Class, List and ListClass
	if ( classRegistry == NULL ) { classRegistry = new(List, registryArray); }

	if ( !List_contains(classRegistry,  NodeList) )
		{ List_add(classRegistry, NodeList); }

	if ( !List_contains(classRegistry,  NodeListClass) )
		{ List_add(classRegistry, NodeListClass); }
	return;
 }

void * removeAllNodes(void * _self)
{
	//struct NodeList * self = cast(NodeList, _self);
	//if ( self == NULL ) { return NULL; } // fail

	//printf("  entering removeAllNodes  ");
	// remove and delete all nodes
	int localItemCount;
	int i;
	void * localNodePtr;
	localItemCount = getItemCount(_self);
	for ( i = 0; i < localItemCount; i++ ) {
		localNodePtr = take(_self);
		delete( localNodePtr );
		//printf("  itemCount: %i of %i, _self: %p, localNodePtr: %p", (i+1), localItemCount, _self, localNodePtr);
	}
	//printf("\n");
	return _self;  // expected path
}
