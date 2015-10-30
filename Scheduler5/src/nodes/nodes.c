/*
 * nodes.c
 *
 *  Created on: Jul 8, 2015
 *      Author: Alan
 */


#include "nodes.h"
#include "nodes-private.h"             // safety include
#include "..\..\src\lists\lists.h"     // supports class registration
#include "..\..\src\objects\objects.h" // safety include

/**************************************/
/***** Implementation Functions  ******/

static void * implement_Node_dtor(struct Node *_self);

void * implement_Node_copy(      struct Node * _copyToNode,
						   const struct Node * _copyFromMaster);

int implement_Node_equal(const void * _self, const void * _comparisonObject);

puto_return_t implement_Node_puto(const void * _self, FILE * _fp);

id_t implement_Node_setID(      struct Node * _self, id_t _id);
id_t implement_Node_getID(const struct Node * _self);

nodeType_t implement_Node_setType(      struct Node *    _self,
		                                const nodeType_t _type);
nodeType_t implement_Node_getType(const struct Node *    _self);

state_t implement_Node_setState(      struct Node * _self,
		                              const state_t _state);
state_t implement_Node_getState(const struct Node * _self);

value_t implement_Node_setValue(      struct Node * _self,
		                              const value_t _value);
value_t implement_Node_getValue(const struct Node * _self);

static runCount_t implement_Node_setRunCount(struct Node *       _self,
		                                     const runCount_t    _runCount);
static runCount_t implement_Node_getRunCount(const struct Node * _self);

static runCount_t implement_Node_incrementRunCount(struct Node * _self);
static runCount_t implement_Node_decrementRunCount(struct Node * _self);

static void * implement_Node_setLinkedNode(struct Node *       _self,
		                                   const void *        _linkedNode);
static void * implement_Node_getLinkedNode(const struct Node * _self);

static void * implement_Node_getPtrToSelfIfReadyToRun
                                                  (const struct Node * _self);

static void * implement_Node_setLinkedMessage(struct Node * _self,
		                                      const void * _linkedMessage);
static void * implement_Node_getLinkedMessage(const struct Node * _self);

void * implement_Node_config(      struct Node * _self,
		                     const struct Node * _master);

static void implement_Node_registerKeyClasses(void);


/*****************************/
/**** INITIALIZATIONS  *******/

const void * NodeClass;
const void * Node;

// MUST be called before any Node is created
void Node_init(void)
{
	// Insert the lines below for any derived subclasses
	//if (!Node)      {Node_init();}
	//if (!NodeClass) {Node_init();}

	if (! NodeClass) {
		NodeClass = new(Class,  		// should be "Class"
						"NodeClass",	// should be "SomethingClass"
						Class,  		// should be "Class"
						sizeof(struct NodeClass),
						ctor, NodeClass_ctor,	//SomethingClass_ctor
						0);  // Terminator
	}
	if (! Node) {
		Node = new(NodeClass,			// SomethingClass from above
				    "Node",				// name like "Something"
				    Object,  			// "superclass(Something)"
				    sizeof(struct Node),// size of self
					// Overloaded superclass functions
					// Remember to call superclass->method
				    ctor,     Node_ctor,//Something_ctor
					dtor,     Node_dtor,
					copy,     Node_copy,
					equal,    Node_equal,
					config,   Node_config,
					puto,     Node_puto,
					// New functions added in this class
					// Do not call superclass->method
					setID,    Node_setID,
					getID,    Node_getID,
					setType,  Node_setType,
					getType,  Node_getType,
					setState, Node_setState,
					getState, Node_getState,
					setValue, Node_setValue,
					getValue, Node_getValue,
					setRunCount, Node_setRunCount,
					getRunCount, Node_getRunCount,

					incrementRunCount, Node_incrementRunCount,
					decrementRunCount, Node_decrementRunCount,

					setLinkedNode, Node_setLinkedNode,
					getLinkedNode, Node_getLinkedNode,

					setLinkedMessage, Node_setLinkedMessage,
					getLinkedMessage, Node_getLinkedMessage,

					getPtrToSelfIfReadyToRun, Node_getPtrToSelfIfReadyToRun,

				    0);	// Terminator
	}


	// requires #include "..\..\src\lists\lists.h" to support class registry
	implement_Node_registerKeyClasses();

	return;
}

/*****************************/
/****** Constructors  ********/

void * Node_ctor(void * _self, va_list * app)
{
	// Add superclass class data members
	struct Node * self = super_ctor(Node, _self, app);

	// set default values for any new data members
	self->nodeID        = ID_UNKNOWN;
	self->nodeType      = TYPE_UNKNOWN;
	self->nodeState     = STATE_UNKNOWN;
	self->nodeValue     = VALUE_UNKNOWN;
	self->runCount      = RUN_COUNT_UNKNOWN;
	self->linkedNode    = NULL;
	self->linkedMessage = NULL;

	// overwrite data members with new data
	// Only uncomment if all data members will be specified in new() command
	// ... this seems like an undue burden on the user.  Leave commented out
	//self->nodeID    = va_arg(* app, id_t);
	//self->nodeState = va_arg(* app, nodeType_t);
	//self->nodeState = va_arg(* app, state_t);
	//self->nodeState = va_arg(* app, value_t);

	return self;
}

void * NodeClass_ctor(void * _self, va_list *app)
{
	// Add superclass class data and methods
	struct NodeClass * self = super_ctor(NodeClass, _self, app);

	// Initialize new functions to default values
	typedef void (* voidf)();
	* (voidf *) & self->setID             = NULL;
	* (voidf *) & self->getID             = NULL;
	* (voidf *) & self->setType           = NULL;
	* (voidf *) & self->getType           = NULL;
	* (voidf *) & self->setState          = NULL;
	* (voidf *) & self->getState          = NULL;
	* (voidf *) & self->setValue          = NULL;
	* (voidf *) & self->getValue          = NULL;
	* (voidf *) & self->setRunCount       = NULL;
	* (voidf *) & self->getRunCount       = NULL;
	* (voidf *) & self->incrementRunCount = NULL;
	* (voidf *) & self->decrementRunCount = NULL;
	* (voidf *) & self->setLinkedNode     = NULL;
	* (voidf *) & self->getLinkedNode     = NULL;
	* (voidf *) & self->setLinkedMessage  = NULL;
	* (voidf *) & self->getLinkedMessage  = NULL;
	* (voidf *) & self->getPtrToSelfIfReadyToRun = NULL;

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

		if (selector == (voidf) setID)
			{* (voidf *) & self->setID    = overloadedFunctionPtr;}

		if (selector == (voidf) getID)
			{* (voidf *) & self->getID    = overloadedFunctionPtr;}

		if (selector == (voidf) setType)
			{* (voidf *) & self->setType = overloadedFunctionPtr;}

		if (selector == (voidf) getType)
			{* (voidf *) & self->getType = overloadedFunctionPtr;}

		if (selector == (voidf) setState)
			{* (voidf *) & self->setState = overloadedFunctionPtr;}

		if (selector == (voidf) getState)
			{* (voidf *) & self->getState = overloadedFunctionPtr;}

		if (selector == (voidf) setValue)
			{* (voidf *) & self->setValue = overloadedFunctionPtr;}

		if (selector == (voidf) getValue)
			{* (voidf *) & self->getValue = overloadedFunctionPtr;}

		if (selector == (voidf) setRunCount)
			{* (voidf *) & self->setRunCount = overloadedFunctionPtr;}

		if (selector == (voidf) getRunCount)
			{* (voidf *) & self->getRunCount = overloadedFunctionPtr;}

		if (selector == (voidf) setRunCount)
			{* (voidf *) & self->setRunCount = overloadedFunctionPtr;}

		if (selector == (voidf) incrementRunCount)
			{* (voidf *) & self->incrementRunCount = overloadedFunctionPtr;}

		if (selector == (voidf) decrementRunCount)
			{* (voidf *) & self->decrementRunCount = overloadedFunctionPtr;}

		if (selector == (voidf) setLinkedNode)
			{* (voidf *) & self->setLinkedNode = overloadedFunctionPtr;}

		if (selector == (voidf) getLinkedNode)
			{* (voidf *) & self->getLinkedNode = overloadedFunctionPtr;}

		if (selector == (voidf) setLinkedMessage)
			{* (voidf *) & self->setLinkedMessage = overloadedFunctionPtr;}

		if (selector == (voidf) getLinkedMessage)
			{* (voidf *) & self->getLinkedMessage = overloadedFunctionPtr;}

		if (selector == (voidf) getPtrToSelfIfReadyToRun)
		{* (voidf *) & self->getPtrToSelfIfReadyToRun = overloadedFunctionPtr;}


	#ifdef va_copy
		va_end(ap);
	#endif

	}  // end while

	return self;
}


/*****************************/
/**** Overloaded Methods  ****/


void * Node_dtor(void * _self)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct Node * self = cast(Node, _self);
	if(self == NULL)                        {return NULL; } // fail

	// address local data members
	if ( implement_Node_dtor(self) == NULL) {return NULL; } // fail

	// call super method after local members are addressed
	// NOTE: classOf(self) calls into super will trigger looping
	if ( super_dtor(Node, _self) == NULL)   {return NULL; } // fail

	return _self;
}

void * Node_copy(void * _self, const void * _copyFromObject)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct Node * self = cast(Node, _self);
	if (self == NULL )           { return NULL; } //fail

	const struct Node * copyFromObject = _copyFromObject;
	if (copyFromObject == NULL ) { return NULL; } //fail

	// call super_copy
	// NOTE: classOf(self) calls into super will trigger looping
	super_copy(Node, self, copyFromObject);

	return implement_Node_copy(self, copyFromObject);
}

equal_t Node_equal(const void * _self, const void * _comparisonObject)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct Node * self = cast(Node, _self);
	if (self == NULL )              { return OBJECT_UNEQUAL; } //fail

	struct Node * comparisonObject = (void * )_comparisonObject;
	if (comparisonObject == NULL )  { return OBJECT_UNEQUAL; } //fail

	// Call super_equal method
	// NOTE: classOf(self) calls into super will trigger looping
	if( super_equal(Node, self, comparisonObject)== OBJECT_UNEQUAL)
		                            {return OBJECT_UNEQUAL;} //fail

	return implement_Node_equal(self, comparisonObject);
}

void * Node_config(const void * _self, const void * _master)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct Node * self = cast(Node, _self);
	if (self == NULL )     { return NULL; } //fail

	struct Node * master = cast(Node, _master);
	if ( master == NULL )  { return NULL; } //fail

	// Call super_equal method
	// NOTE: classOf(self) calls into super will trigger looping
	if( super_config(Node, self, master) == NULL )
					   { return NULL; } //fail

	return implement_Node_config(self, master);
}

puto_return_t Node_puto(const void * _self, FILE * _fp)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct Node * self = cast(Node, _self);
	if (self == NULL ) { return PUTO_ERROR; } // fail

	// the call to super_puto() is managed within implement_Node_puto()
	return implement_Node_puto(self, _fp);
}

/****************************************************************************/
/********  New functions for  class " NodeClass"  ***************************/
/****************************************************************************/


/*****************************/
/****** SetID Methods  *******/

id_t setID (const void * _self, const id_t _id)
{
	const struct NodeClass * class = classOf( cast(Node, _self) );
	if (class == NULL )        { return ID_UNKNOWN; } // fail
	if (class->setID == NULL ) { return ID_UNKNOWN; } // fail
	return class->setID(_self, _id);
}

id_t super_setID(const void * _class,
				 const void * _self, const id_t   _id)
{
	// verify that NodeClass is in the superclass chain of _class
	if ( ! isOfSuper(NodeClass, _self) ) { return ID_UNKNOWN; } // fail
	const struct NodeClass * superclass = super(_class);
	if ( superclass == NULL )            { return ID_UNKNOWN; } // fail
	if ( superclass->setID == NULL )     { return ID_UNKNOWN; } // fail
	return superclass->setID(_self, _id);
}

id_t Node_setID (const void * _self, const id_t _id)
{
	struct Node * self = cast(Node, _self);
	if( self == NULL ) { return ID_UNKNOWN; }    // fail
	return implement_Node_setID(self, _id);      // expected path
}

/*****************************/
/****** GetID Methods  *******/

id_t getID (const void * _self)
{
	const struct NodeClass * class = classOf( cast(Node, _self) );
	if (class == NULL )        { return ID_UNKNOWN; } // fail
	if (class->getID == NULL ) { return ID_UNKNOWN; } // fail
	return class->getID(_self);
}

id_t super_getID(const void * _class, const void * _self)
{
	// verify that NodeClass is in the superclass chain of _class
	if ( ! isOfSuper(NodeClass, _self) ) { return ID_UNKNOWN; } // fail
	const struct NodeClass * superclass = super(_class);
	if ( superclass == NULL )            { return ID_UNKNOWN; } // fail
	if ( superclass->getID == NULL )     { return ID_UNKNOWN; } // fail
	return superclass->getID(_self);
}

id_t Node_getID (const void * _self)
{
	struct Node * self = cast(Node, _self);
	if( self == NULL ) { return ID_UNKNOWN; } // fail
	return implement_Node_getID(self);        // expected path
}


/*****************************/
/***** SetType Methods  ******/

nodeType_t setType(const void * _self, const nodeType_t _type)
{
	const struct NodeClass * class = classOf( cast(Node, _self) );
	if ( class == NULL )          { return TYPE_UNKNOWN; } // fail
	if ( class->setType == NULL ) { return TYPE_UNKNOWN; } // fail
	return class->setType(_self, _type);
}

nodeType_t super_setType(const void * _class,
		                 const void * _self, const nodeType_t _type)
{
	// verify that NodeClass is in the superclass chain of _class
	if ( ! isOfSuper(NodeClass, _self) ) { return TYPE_UNKNOWN; } // fail
	const struct NodeClass * superclass = super(_class);
	if ( superclass == NULL )            { return TYPE_UNKNOWN; } // fail
	if ( superclass->setType == NULL )   { return TYPE_UNKNOWN; } // fail
	return superclass->setType(_self, _type);
}

nodeType_t  Node_setType(const void * _self, const nodeType_t _type)
{
	struct Node * self = cast(Node, _self);
	if ( self == NULL ) { return TYPE_UNKNOWN; }  // fail
	return implement_Node_setType(self, _type);   // expected path
}

/*****************************/
/****** GetType Methods  *******/

nodeType_t getType(const void * _self)
{
	const struct NodeClass * class = classOf( cast(Node, _self) );
	if ( class == NULL )          { return TYPE_UNKNOWN; } // fail
	if ( class->getType == NULL ) { return TYPE_UNKNOWN; } // fail
	return class->getType(_self);
}

nodeType_t super_getType(const void * _class, const void * _self)
{
	// verify that ListClass is in the superclass chain of _class
	if ( ! isOfSuper(NodeClass, _self ) ) { return TYPE_UNKNOWN; } // fail
	const struct NodeClass * superclass = super(_class);
	if ( superclass == NULL )             { return TYPE_UNKNOWN; } // fail
	if ( superclass->getType == NULL )    { return TYPE_UNKNOWN; } // fail
	return superclass->getType(_self);
}

nodeType_t  Node_getType(const void * _self)
{
	struct Node * self = cast(Node, _self);
	if ( self == NULL ) { return TYPE_UNKNOWN; }  // fail
	return implement_Node_getType(self);          // expected path
}


/*****************************/
/***** setState Methods  *****/

state_t setState (const void * _self, const state_t _state)
{
	const struct NodeClass * class = classOf( cast(Node, _self) );
	if ( class == NULL )           { return STATE_UNKNOWN; } // fail
	if ( class->setState == NULL ) { return STATE_UNKNOWN; } // fail
	return class->setState(_self, _state);
}

state_t super_setState(const void *         _class,
							  const void *  _self,
							  const state_t _state)
{
	// verify that NodeClass is in the superclass chain of _class
	if ( ! isOfSuper(NodeClass, _self ) ) { return STATE_UNKNOWN; } // fail
	const struct NodeClass * superclass = super(_class);
	if ( superclass == NULL )             { return STATE_UNKNOWN; } // fail
	if ( superclass->setState == NULL )   { return STATE_UNKNOWN; } // fail
	return superclass->setState(_self, _state);
}

state_t Node_setState (const void *         _self,
							   const state_t _state)
{
	struct Node * self = cast(Node, _self);
	if ( self == NULL ) { return STATE_UNKNOWN; }   // fail
	return implement_Node_setState(self, _state);   // expected path
}

/*****************************/
/***** getState Methods  *****/

state_t getState (const void * _self)
{
	const struct NodeClass * class = classOf( cast(Node, _self) );
	if ( class == NULL )           { return STATE_UNKNOWN; } // fail
	if ( class->getState == NULL ) { return STATE_UNKNOWN; } // fail
	return class->getState(_self);
}

state_t super_getState(const void * _class, const void * _self)
{
	// verify that NodeClass is in the superclass chain of _class
	if ( ! isOfSuper(NodeClass, _self ) ) { return STATE_UNKNOWN; } // fail
	const struct NodeClass * superclass = super(_class);
	if ( superclass == NULL )             { return STATE_UNKNOWN; } // fail
	if ( superclass->getState == NULL )   { return STATE_UNKNOWN; } // fail
	return superclass->getState(_self);
}

state_t Node_getState (const void * _self)
{
	struct Node * self = cast(Node, _self);
	if ( self == NULL ) { return STATE_UNKNOWN; }  // fail
	return implement_Node_getState(self);          // expected path
}

/*****************************/
/***** setValue Methods  *****/

value_t setValue(const void * _self, const value_t _value)
{
	const struct NodeClass * class = classOf( cast(Node, _self) );
	if ( class == NULL )           { return VALUE_UNKNOWN; } // fail
	if ( class->setValue == NULL ) { return VALUE_UNKNOWN; } // fail
	return class->setValue(_self, _value);
}

value_t super_setValue(const void * _class,
		               const void * _self, const state_t _value)
{
	// verify that NodeClass is in the superclass chain of _class
	if ( ! isOfSuper(NodeClass, _self ) ) { return VALUE_UNKNOWN; } // fail
	const struct NodeClass * superclass = super(_class);
	if ( superclass == NULL )             { return VALUE_UNKNOWN; } // fail
	if ( superclass->setValue == NULL )   { return VALUE_UNKNOWN; } // fail
	return superclass->setValue(_self, _value);
}

value_t  Node_setValue(const void * _self, const state_t _value)
{
	struct Node * self = cast(Node, _self);
	if( self == NULL ) { return VALUE_UNKNOWN; }   // fail
	return implement_Node_setValue(self, _value);  // expected path
}

/*****************************/
/***** getState Methods  *****/

value_t getValue (const void * _self)
{
	const struct NodeClass * class = classOf( cast(Node, _self) );
	if ( class == NULL )           { return VALUE_UNKNOWN; } // fail
	if ( class->getValue == NULL ) { return VALUE_UNKNOWN; } // fail
	return class->getValue(_self);
}

value_t super_getValue(const void * _class, const void * _self)
{
	// verify that NodeClass is in the superclass chain of _class
	if ( ! isOfSuper(NodeClass, _self ) ) { return VALUE_UNKNOWN; } // fail
	const struct NodeClass * superclass = super(_class);
	if ( superclass == NULL )             { return VALUE_UNKNOWN; } // fail
	if ( superclass->getValue == NULL )   { return VALUE_UNKNOWN; } // fail
	return superclass->getValue(_self);
}

value_t Node_getValue (const void * _self)
{
	struct Node * self = cast(Node, _self);
	if( self == NULL ) { return VALUE_UNKNOWN; }  // fail
	return implement_Node_getValue(self);         // expected path
}

/**************************************/
/***********  setRunCount    **********/

runCount_t  setRunCount(const void * _self, runCount_t runCount)
{
	const struct NodeClass * class = classOf( cast(Node, _self) );
	if ( class == NULL )              { return RUN_COUNT_UNKNOWN; } // fail
	if ( class->setRunCount == NULL ) { return RUN_COUNT_UNKNOWN; } // fail
	return class->setRunCount(_self, runCount);
}

runCount_t super_setRunCount(const void * _class,
		                     const void * _self, runCount_t runCount)
{
	// verify that NodeClass is in the superclass chain of _class
	if ( ! isOfSuper(NodeClass, _self) )
		{ return RUN_COUNT_UNKNOWN; } // fail
	const struct NodeClass * superclass = super(_class);
	if ( superclass == NULL )
		{ return RUN_COUNT_UNKNOWN; } // fail
	if ( superclass->setRunCount == NULL )
		{ return RUN_COUNT_UNKNOWN; } // fail
	return superclass->setRunCount(_self, runCount);
}

runCount_t Node_setRunCount(const void * _self, runCount_t runCount)
{
	// remove from the list the node with the specified ID
	struct Node * self = cast(Node, _self);
	if( self == NULL ) { return RUN_COUNT_UNKNOWN; } // fail
	return implement_Node_setRunCount(self, runCount);
}


/**************************************/
/***********  getRunCount    *************/

runCount_t  getRunCount(const void * _self)
{
	const struct NodeClass * class = classOf( cast(Node, _self) );
	if ( class == NULL )              { return RUN_COUNT_UNKNOWN; } // fail
	if ( class->setRunCount == NULL ) { return RUN_COUNT_UNKNOWN; } // fail
	return class->getRunCount(_self);
}

runCount_t super_getRunCount(const void * _class, const void * _self)
{
	// verify that NodeClass is in the superclass chain of _class
	if ( ! isOfSuper(NodeClass, _self) )
		{ return RUN_COUNT_UNKNOWN; } // fail
	const struct NodeClass * superclass = super(_class);
	if ( superclass == NULL )
		{ return RUN_COUNT_UNKNOWN; } // fail
	if ( superclass->getRunCount == NULL )
		{ return RUN_COUNT_UNKNOWN; } // fail
	return superclass->getRunCount(_self);
}

runCount_t Node_getRunCount(const void * _self)
{
	// remove from the list the node with the specified ID
	struct Node * self = cast(Node, _self);
	if( self == NULL ) { return RUN_COUNT_UNKNOWN; } // fail
	return implement_Node_getRunCount(self);
}

/**************************************/
/*******  incrementRunCount    *******/

runCount_t  incrementRunCount(const void * _self)
{
	const struct NodeClass * class = classOf( cast(Node, _self) );
	if ( class == NULL )
		{ return RUN_COUNT_UNKNOWN; } // fail
	if ( class->incrementRunCount == NULL )
		{ return RUN_COUNT_UNKNOWN; } // fail
	return class->incrementRunCount(_self);
}

runCount_t super_incrementRunCount(const void * _class, const void * _self)
{
	// verify that NodeClass is in the superclass chain of _class
	if ( ! isOfSuper(NodeClass, _self) )
		{ return RUN_COUNT_UNKNOWN; } // fail
	const struct NodeClass * superclass = super(_class);
	if ( superclass == NULL )
		{ return RUN_COUNT_UNKNOWN; } // fail
	if ( superclass->incrementRunCount == NULL )
		{ return RUN_COUNT_UNKNOWN; } // fail
	return superclass->incrementRunCount(_self);
}

runCount_t Node_incrementRunCount(const void * _self)
{
	// remove from the list the node with the specified ID
	struct Node * self = cast(Node, _self);
	if( self == NULL ) { return RUN_COUNT_UNKNOWN; } // fail
	return implement_Node_incrementRunCount(self);
}



/**************************************/
/*******  decrementRunCount    *******/

runCount_t  decrementRunCount(const void * _self)
{
	const struct NodeClass * class = classOf( cast(Node, _self) );
	if ( class == NULL )
		{ return RUN_COUNT_UNKNOWN; } // fail
	if ( class->decrementRunCount == NULL )
		{ return RUN_COUNT_UNKNOWN; } // fail
	return class->decrementRunCount(_self);
}

runCount_t super_decrementRunCount(const void * _class, const void * _self)
{
	// verify that NodeClass is in the superclass chain of _class
	if ( ! isOfSuper(NodeClass, _self) )
		{ return RUN_COUNT_UNKNOWN; } // fail
	const struct NodeClass * superclass = super(_class);
	if ( superclass == NULL )
		{ return RUN_COUNT_UNKNOWN; } // fail
	if ( superclass->decrementRunCount == NULL )
		{ return RUN_COUNT_UNKNOWN; } // fail
	return superclass->decrementRunCount(_self);
}

runCount_t Node_decrementRunCount(const void * _self)
{
	// remove from the list the node with the specified ID
	struct Node * self = cast(Node, _self);
	if( self == NULL ) { return RUN_COUNT_UNKNOWN; } // fail
	return implement_Node_decrementRunCount(self);
}


/**************************************/
/***********  setLinkedNode    *************/

void * setLinkedNode(const const void * _self, const void * _linkedNode)
{
	const struct NodeClass * class = classOf( cast(Node, _self) );
	if ( class == NULL )                { return NULL; } // fail
	if ( class->setLinkedNode == NULL ) { return NULL; } // fail
	return class->setLinkedNode(_self, _linkedNode);
}

void * super_setLinkedNode(const void * _class,
		                   const void * _self, const void * _linkedNode)
{
	// verify that NodeClass is in the superclass chain of _class
	if ( ! isOfSuper(NodeClass, _self) )     { return NULL; } // fail
	const struct NodeClass * superclass = super(_class);
	if ( superclass == NULL )                { return NULL; } // fail
	if ( superclass->setLinkedNode == NULL ) { return NULL; } // fail
	return superclass->setLinkedNode(_self, _linkedNode);
}

void * Node_setLinkedNode(const const void * _self, const void * _linkedNode)
{
	// remove from the list the node with the specified ID
	struct Node * self = cast(Node, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Node_setLinkedNode(self, _linkedNode);
}


/**************************************/
/***********  getLinkedNode    *************/

void * getLinkedNode(const const void * _self)
{
	const struct NodeClass * class = classOf( cast(Node, _self) );
	if ( class == NULL )                { return NULL; } // fail
	if ( class->getLinkedNode == NULL ) { return NULL; } // fail
	return class->getLinkedNode(_self);
}

void * super_getLinkedNode(const void * _class, const void * _self)
{
	// verify that NodeClass is in the superclass chain of _class
	if ( ! isOfSuper(NodeClass, _self) )     { return NULL; } // fail
	const struct NodeClass * superclass = super(_class);
	if ( superclass == NULL )                { return NULL; } // fail
	if ( superclass->getLinkedNode == NULL ) { return NULL; } // fail
	return superclass->getLinkedNode(_self);
}

void * Node_getLinkedNode(const const void * _self)
{
	// remove from the list the node with the specified ID
	struct Node * self = cast(Node, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Node_getLinkedNode(self);
}


/**************************************/
/***********  getPtrToSelfIfReadyToRun    *************/

void * getPtrToSelfIfReadyToRun(const const void * _self)
{
	const struct NodeClass * class = classOf( cast(Node, _self) );
	if ( class == NULL )                { return NULL; } // fail
	if ( class->getPtrToSelfIfReadyToRun == NULL ) { return NULL; } // fail
	return class->getPtrToSelfIfReadyToRun(_self);
}

void * super_getPtrToSelfIfReadyToRun(const void * _class, const void * _self)
{
	// verify that NodeClass is in the superclass chain of _class
	if ( ! isOfSuper(NodeClass, _self) )
		{ return NULL; } // fail
	const struct NodeClass * superclass = super(_class);
	if ( superclass == NULL )
		{ return NULL; } // fail
	if ( superclass->getPtrToSelfIfReadyToRun == NULL )
		{ return NULL; } // fail
	return superclass->getPtrToSelfIfReadyToRun(_self);
}

void * Node_getPtrToSelfIfReadyToRun(const const void * _self)
{
	struct Node * self = cast(Node, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Node_getPtrToSelfIfReadyToRun(self);
}


/**************************************/
/***********  setLinkedMessage    *************/

void * setLinkedMessage(const const void * _self, const void * _linkedMessage)
{
	const struct NodeClass * class = classOf( cast(Node, _self) );
	if ( class == NULL )                { return NULL; } // fail
	if ( class->setLinkedMessage == NULL ) { return NULL; } // fail
	return class->setLinkedMessage(_self, _linkedMessage);
}

void * super_setLinkedMessage(const void * _class,
							  const void * _self, const void * _linkedMessage)
{
	// verify that NodeClass is in the superclass chain of _class
	if ( ! isOfSuper(NodeClass, _self) )     { return NULL; } // fail
	const struct NodeClass * superclass = super(_class);
	if ( superclass == NULL )                { return NULL; } // fail
	if ( superclass->setLinkedMessage == NULL ) { return NULL; } // fail
	return superclass->setLinkedMessage(_self, _linkedMessage);
}

void * Node_setLinkedMessage(const const void * _self,
		                     const void * _linkedMessage)
{
	// remove from the list the node with the specified ID
	struct Node * self = cast(Node, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Node_setLinkedMessage(self, _linkedMessage);
}


/**************************************/
/*******  getLinkedMessage    *********/

void * getLinkedMessage(const const void * _self)
{
	const struct NodeClass * class = classOf( cast(Node, _self) );
	if ( class == NULL )                { return NULL; } // fail
	if ( class->getLinkedMessage == NULL ) { return NULL; } // fail
	return class->getLinkedMessage(_self);
}

void * super_getLinkedMessage(const void * _class, const void * _self)
{
	// verify that NodeClass is in the superclass chain of _class
	if ( ! isOfSuper(NodeClass, _self) )     { return NULL; } // fail
	const struct NodeClass * superclass = super(_class);
	if ( superclass == NULL )                { return NULL; } // fail
	if ( superclass->getLinkedMessage == NULL ) { return NULL; } // fail
	return superclass->getLinkedMessage(_self);
}

void * Node_getLinkedMessage(const const void * _self)
{
	// remove from the list the node with the specified ID
	struct Node * self = cast(Node, _self);
	if( self == NULL ) { return NULL; } // fail
	return implement_Node_getLinkedMessage(self);
}


/*************************************************/
/*********** Implementation Procedures ***********/

static void * implement_Node_dtor(struct Node * _self)
{
	// delete any attached local messages
	// WARNING:  may cause recursive calls to delete() if messages are linked
	struct Node * localLinkedMessage;
	localLinkedMessage = implement_Node_getLinkedMessage(_self);
	if ( localLinkedMessage != NULL ) {
		if( delete(localLinkedMessage) != localLinkedMessage)
			{ return NULL; } // failed to delete message
		setLinkedMessage(_self, NULL);
	}


	// delete any linked nodes
	// WARNING:  will cause recursive calls to delete() linked messages
	struct Node * localLinkedNode;
	localLinkedNode = implement_Node_getLinkedNode(_self);
	if ( localLinkedNode != NULL ) {
		if( delete(localLinkedNode) != localLinkedNode)
			{ return NULL; } // failed to delete linked node
		setLinkedNode(_self, NULL);
	}

	implement_Node_setID      (_self, ID_UNKNOWN);
	implement_Node_setType    (_self, TYPE_UNKNOWN);
	implement_Node_setState   (_self, STATE_UNKNOWN);
	implement_Node_setValue   (_self, VALUE_UNKNOWN);
	implement_Node_setRunCount(_self, RUN_COUNT_UNKNOWN);
	return _self;
}

void * implement_Node_copy(      struct Node * _copyToNode,
						   const struct Node * _copyFromMaster)
{
	// copy master data members, unless the value is UNKNOWN

	if ( implement_Node_getID(_copyFromMaster) != ID_UNKNOWN )
		{ implement_Node_setID (_copyToNode,
							implement_Node_getID (_copyFromMaster)); }

	if ( implement_Node_getType(_copyFromMaster) != TYPE_UNKNOWN )
		{ implement_Node_setType (_copyToNode,
							implement_Node_getType(_copyFromMaster)); }

	if ( implement_Node_getState(_copyFromMaster) != STATE_UNKNOWN )
		{ implement_Node_setState(_copyToNode,
							implement_Node_getState(_copyFromMaster)); }

	if ( implement_Node_getValue(_copyFromMaster) != VALUE_UNKNOWN )
		{ implement_Node_setValue(_copyToNode,
							implement_Node_getValue(_copyFromMaster)); }
/*
	//  runCount is a dynamic value and is not copied
	if ( implement_Node_getRunCount(_copyFromMaster) != RUN_COUNT_UNKNOWN )
		{ implement_Node_setRunCount(_copyToNode,
							implement_Node_getRunCount(_copyFromMaster)); }
*/

	// Create new(Node) and copy relevant data for linked nodes
	void * localLinkNodeFromMaster;
	localLinkNodeFromMaster = implement_Node_getLinkedNode(_copyFromMaster);
	void * localLinkNodeCopyTo = NULL;
	if (localLinkNodeFromMaster != NULL) {
		localLinkNodeCopyTo = new(classOf(localLinkNodeFromMaster));
		if ( localLinkNodeCopyTo == NULL )
			{ return NULL; } // failed to create new linkedNode
		// WARNING:  the call to copy is recursive
		copy(localLinkNodeCopyTo, localLinkNodeFromMaster);
	}
	implement_Node_setLinkedNode(_copyToNode, localLinkNodeCopyTo);


	// if the linkedMessage pointer is not NULL, then do NOT copy and set
	if ( implement_Node_getLinkedMessage(_copyFromMaster) == NULL )
		{ return _copyToNode; }

	// Create new(Node) and copy relevant data for linked messages
	void * localLinkMessageFromMaster;
	localLinkMessageFromMaster =
			                 implement_Node_getLinkedMessage(_copyFromMaster);
	void * localLinkMessageCopyTo = NULL;
	if (localLinkMessageFromMaster != NULL) {
		localLinkMessageCopyTo = new(classOf(localLinkMessageFromMaster));
		if ( localLinkMessageCopyTo == NULL )
			{ return NULL; } // failed to create new linkedNode
		// WARNING:  the call to copy is recursive
		copy(localLinkMessageCopyTo, localLinkMessageFromMaster);
	}
	implement_Node_setLinkedMessage(_copyToNode, localLinkMessageCopyTo);


	return _copyToNode;
}

int implement_Node_equal(const void * _self, const void * _comparisonObject)
{
	// Check each data member for congruence

	// WARNING:  nodes and messages should have unique IDs
	if( implement_Node_getID(_self)    !=
								implement_Node_getID   (_comparisonObject) )
		{return OBJECT_UNEQUAL;}

	if( implement_Node_getType(_self)  !=
								implement_Node_getType (_comparisonObject) )
		{return OBJECT_UNEQUAL;}

	if( implement_Node_getState(_self) !=
								implement_Node_getState(_comparisonObject) )
		{return OBJECT_UNEQUAL;}

	if( implement_Node_getValue(_self) !=
								implement_Node_getValue(_comparisonObject) )
		{return OBJECT_UNEQUAL;}

	// runCount is a dynamic value and is not included in assessing equal
	/*
	if( implement_Node_getRunCount(_self) !=
							implement_Node_getRunCount(_comparisonObject) )
		{return OBJECT_UNEQUAL;}
*/
	// TODO:  add recursive calls to test equivalence for linked nodes
	// TODO:  add call to to test equivalence for linked messages
	// TODO:  address conflict with linked nodes needing unique IDs,
	//        ... and commonID events which might well share same IDs

	/*  WARNING: nodes and messages should not point to the same objects
	 *           ... the data values above should be equal,
	 *           ... but the pointers below will differ
	 *
	if( implement_Node_getLinkedNode(_self) !=
					implement_Node_getLinkedNode(_comparisonObject) )
		{return OBJECT_UNEQUAL;}

	if( implement_Node_getLinkedNode(_self) !=
					implement_Node_getLinkedNode(_comparisonObject) )
		{return OBJECT_UNEQUAL;}
	*/

	return OBJECT_EQUAL;
}

puto_return_t implement_Node_puto(const void * _self, FILE * _fp)
{
	puto_return_t printReturnCode = PUTO_SUCCESS;

	struct Node * self = cast(Node, _self);

	// print local data members
	fprintf(_fp, "\n  Local data members managed in Node:\n");

	if (PUTO_ERROR == fprintf(_fp, "  nodeID:        %i\n",
							  implement_Node_getID(_self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  nodeType:      %i\n",
							  implement_Node_getType(_self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  nodeState:     %i\n",
							  implement_Node_getState(_self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  nodeValue:     %i\n",
							  implement_Node_getValue(_self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  runCount:      %i\n",
							  implement_Node_getRunCount(_self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  linkedNode:    %p\n",
							  implement_Node_getLinkedNode(_self) ))
			{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  linkedMessage: %p\n",
							  implement_Node_getLinkedMessage(_self) ))
			{ printReturnCode = PUTO_ERROR;  } // error detected

	fprintf(_fp, "\n  New methods added in NodeList:\n");

	if (PUTO_ERROR == fprintf(_fp, "  Node_init:              %p\n",
			                                                  Node_init ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Node_setID:             %p\n",
			                                                   Node_setID ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Node_getID:             %p\n",
			                                                   Node_getID ))
		{ printReturnCode = PUTO_ERROR;  } // error detected


	if (PUTO_ERROR == fprintf(_fp, "  Node_setType:           %p\n",
			                                                   Node_setType ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Node_getType:           %p\n",
			                                                   Node_getType ))
		{ printReturnCode = PUTO_ERROR;  } // error detected


	if (PUTO_ERROR == fprintf(_fp, "  Node_setState:          %p\n",
			                                                   Node_setState ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Node_getState:          %p\n",
			                                                   Node_getState ))
		{ printReturnCode = PUTO_ERROR;  } // error detected


	if (PUTO_ERROR == fprintf(_fp, "  Node_setValue:          %p\n",
			                                                   Node_setValue ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Node_getValue:          %p\n",
			Node_getValue ))
		{ printReturnCode = PUTO_ERROR;  } // error detected


	if (PUTO_ERROR == fprintf(_fp, "  Node_setRunCount:       %p\n",
			Node_setRunCount ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Node_getRunCount:       %p\n",
			Node_getRunCount ))
		{ printReturnCode = PUTO_ERROR;  } // error detected


	if (PUTO_ERROR == fprintf(_fp, "  Node_incrementRunCount: %p\n",
			Node_incrementRunCount ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Node_decrementRunCount: %p\n",
			Node_decrementRunCount ))
		{ printReturnCode = PUTO_ERROR;  } // error detected


	if (PUTO_ERROR == fprintf(_fp, "  Node_getPtrToSelfIfReadyToRun:  %p\n",
			Node_getPtrToSelfIfReadyToRun ))
		{ printReturnCode = PUTO_ERROR;  } // error detected


	if (PUTO_ERROR == fprintf(_fp, "  Node_setLinkedNode:     %p\n",
			Node_setLinkedNode ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Node_getLinkedNode:     %p\n",
			Node_getLinkedNode ))
		{ printReturnCode = PUTO_ERROR;  } // error detected


	if (PUTO_ERROR == fprintf(_fp, "  Node_setLinkedMessage:  %p\n",
			Node_setLinkedMessage ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Node_getLinkedMessage:  %p\n",
			Node_getLinkedMessage ))
		{ printReturnCode = PUTO_ERROR;  } // error detected


	if (PUTO_ERROR == fprintf(_fp, "  Node_config:            %p\n",
			                                                   Node_config ))
		{ printReturnCode = PUTO_ERROR;  } // error detected


	fprintf(_fp, "\n  Local Node methods:\n" );

	if (PUTO_ERROR == fprintf(_fp, "  Node_ctor:      %p\n", Node_ctor ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Node_dtor:      %p\n", Node_dtor ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Node_copy:      %p\n", Node_copy ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Node_equal:     %p\n", Node_equal ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Node_config:    %p\n", Node_config ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Node_puto:      %p\n", Node_puto ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  NodeClass_ctor: %p\n\n", NodeClass_ctor))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	fprintf(_fp, "Printing Node class:\n" );
	if (printReturnCode == Class_puto(Node, _fp))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	// print superclass data
	fprintf(_fp, "Printing the %s superclass data for Node:\n",
			((struct Object * )self)->class->superclass->className );
	// WARNING: classOf(self) calls into super will trigger looping
	if (printReturnCode == super_puto(Node, self, _fp))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	fprintf(_fp, "\nPrinting NodeClass class:\n" );
	if (printReturnCode == puto(NodeClass, _fp))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	return printReturnCode;
}

id_t implement_Node_setID(struct Node * _self, id_t _id)
{
	_self->nodeID = _id;
	return _id;
}

id_t implement_Node_getID(const struct Node * _self)
{
	return _self->nodeID;
}

nodeType_t implement_Node_setType(struct Node * _self, nodeType_t _type)
{
	_self->nodeType = _type;
	return _type;
}

nodeType_t implement_Node_getType(const struct Node * _self)
{
	return _self->nodeType;
}

state_t implement_Node_setState(struct Node * _self, const state_t _state)
{
	_self->nodeState = _state;
	return _state;
}

state_t implement_Node_getState(const struct Node * _self)
{
	return _self->nodeState;
}

value_t implement_Node_setValue(struct Node * _self, const value_t _value)
{
	_self->nodeValue = _value;
	return _value;
}

value_t implement_Node_getValue(const struct Node * _self)
{
	return _self->nodeValue;
}

static runCount_t implement_Node_setRunCount(struct Node *    _self,
		                                     const runCount_t _runCount)
{
	runCount_t localRunCount = _runCount;

	if(localRunCount == RUN_COUNT_UNKNOWN){
	    // take no action ... process special value unmodified
	}else if(localRunCount < 0){
		localRunCount = 0;  // negative values disallowed
	}

	_self->runCount = localRunCount;
	return localRunCount;
}

static runCount_t implement_Node_getRunCount(const struct Node * _self)
{
	return _self->runCount;
}

static runCount_t implement_Node_incrementRunCount(struct Node * _self)
{
	runCount_t localRunCount = implement_Node_getRunCount(_self);
	if(localRunCount == RUN_COUNT_UNKNOWN){
		localRunCount = 1;  // first increment is to "1"
	}
	else
	{
		localRunCount++;
	}
	return implement_Node_setRunCount( _self, localRunCount);
}

static runCount_t implement_Node_decrementRunCount(struct Node * _self)
{
	runCount_t localRunCount = implement_Node_getRunCount(_self);
	if(localRunCount <= 0){
			localRunCount = 0;  // can not decrement below "0"
		}
		else
		{
			localRunCount--;
		}
	return implement_Node_setRunCount( _self, localRunCount);
}

static void * implement_Node_setLinkedNode(struct Node * _self,
		                                   const void *  _linkedNode)
{
	_self->linkedNode = (void *)_linkedNode;
	return (void *)_linkedNode;
}

static void * implement_Node_getLinkedNode(const struct Node * _self)
{
	return (void *)_self->linkedNode;
}

static void * implement_Node_getPtrToSelfIfReadyToRun
                                                   (const struct Node * _self)
{
	// WARNING: acknowledged returned node pointer with decrementRunCount()
	//          ... acknowledging the node at the root is not sufficient
	struct Node * returnPointer = NULL;
	struct Node * localLinkedNodePTR = implement_Node_getLinkedNode(_self);

	// follow nodeLinks to farthest extent
	if ( localLinkedNodePTR != NULL) {
		// recursively call linked nodes looking for a ready to run node
		// nodes at the farthest extent will have priority over root nodes
		returnPointer = implement_Node_getPtrToSelfIfReadyToRun(localLinkedNodePTR);
	}

	// if higher priority node is ready, return higher priority node
	if ( returnPointer != NULL ) { return returnPointer; }  // ready found

	// if local node is ready, return local PTR
	if ( implement_Node_getRunCount(_self) > 0 ) { return (void *)_self; }

	return NULL;  // fail, no node is ready
}

static void * implement_Node_setLinkedMessage(struct Node * _self,
		                                      const void *  _linkedMessage)
{
	_self->linkedMessage = (void *)_linkedMessage;
	return (void *)_linkedMessage;
}

static void * implement_Node_getLinkedMessage(const struct Node * _self)
{
	return (void *)_self->linkedMessage;
}


void * implement_Node_config(struct Node * _self, const struct Node * _master)
{
	// basic nodes are managed by their unique Object pointers
	struct Node * master = cast(Node, _master);
	if (master == NULL ) { return NULL; } // fail
	return copy(_self, _master);
}

static void implement_Node_registerKeyClasses(void)
{
	if ( classRegistry == NULL )
		{ classRegistry = new(List, registryArray, 8); }

	if ( !List_contains(classRegistry,  Node) )
		{ List_add(classRegistry, Node); }

	if ( !List_contains(classRegistry,  NodeClass) )
		{ List_add(classRegistry, NodeClass); }

	return;
}
