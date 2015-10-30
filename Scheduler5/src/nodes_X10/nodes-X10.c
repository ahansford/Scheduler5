/*
 * nodes-X10.c
 *
 *  Created on: Jul 8, 2015
 *      Author: Alan
 */


#include "..\..\src\objects\objects.h"	// safety include
#include "..\..\src\nodes\nodes.h"		// safety include
#include "nodes-X10.h"
#include "nodes-X10-private.h"			// safety include


/**************************************/
/***** Implementation Functions  ******/

void * implement_X10_nodeCopy(      struct Node_X10 * _copyToNode,
						      const struct Node_X10 * _copyFromMaster);

int implement_X10_nodeEqual(const struct Node_X10 * _self,
						    const struct Node_X10 * _comparisonObject);

puto_return_t implememnt_Node_X10_puto(const void * _self, FILE * _fp);

X10_house_code_t implement_X10_setHouseCode(struct Node_X10 * _self,
										 const X10_house_code_t _houseCode);

X10_house_code_t implement_X10_getHouseCode(const struct Node_X10 * _self);

X10_unit_code_t implement_X10_setUnitCode(struct Node_X10 * _self,
										  const X10_unit_code_t _unitCode);

X10_unit_code_t implement_X10_getUnitCode(const struct Node_X10 * _self);

void * implement_X10_config(void * _self, void * _master);


/**********  *******************/
/**** INITIALIZATIONS  *******/

const void * NodeClass_X10;
const void * Node_X10;

void Node_X10_init(void)
{
	/* MUST be called before any light is created */

	// Insert the lines below for any derived subclasses
	//if (!Node_X10)      {Node_X10_init();}
	//if (!NodeClass_X10) {Node_X10_init();}
	if (!Node)      {Node_init();}
	if (!NodeClass) {Node_init();}

	if (! NodeClass_X10) {
		  NodeClass_X10 = new(Class,  			// should be "Class"
						      "NodeClass_X10",	// should be "SomethingClass"
						      Class,  			// should be "Class"
						      sizeof(struct NodeClass_X10),	// size of self
						      ctor, NodeClass_X10_ctor,	 //SomethingClass_ctor
						      0);	// Terminator
	}
	if (! Node_X10) {
		  Node_X10 =  new(NodeClass_X10,	// SomethingClass from above
				      "Node_X10",			// name like "Something"
					  Node,  				// "superclass(Something)"
				      sizeof(struct Node_X10),	// size of self
					  // Overloaded superclass functions
					  // Remember to call superclass->method
					  ctor,     	Node_X10_ctor,
					  copy,    		Node_X10_copy,
					  equal,    	Node_X10_equal,
					  puto,     	Node_X10_puto,
					  config,    	Node_X10_config,
					  // New functions added in this class
					  // Do not call superclass->method
					  setHouseCode,	Node_X10_setHouseCode,
					  getHouseCode,	Node_X10_getHouseCode,
					  setUnitCode,  Node_X10_setUnitCode,
					  getUnitCode,  Node_X10_getUnitCode,

					  0);	// terminator character
	}

	// requires list.h to support class registry
	/*
	if ( classRegistry == NULL )
		{ classRegistry = new(List, registryArray, 8); }

	if ( !List_contains(classRegistry,  Node_X10) )
		{ List_add(classRegistry, Node_X10); }

	if ( !List_contains(classRegistry,  NodeClass_X10) )
		{ List_add(classRegistry, NodeClass_X10); }
	*/
	return;
}

/*****************************/
/****** Constructors  ********/

void * Node_X10_ctor(void * _self, va_list * app)
{
	// Add superclass class data members
	struct Node_X10 * self = super_ctor(Node_X10, _self, app);

	// set default values for any new data members
	self->house  = X10_HOUSE_CODE_UNKNOWN;
	self->unit   = X10_UNIT_CODE_UNKNOWN;

	// overwrite data members with new data
	// Only uncomment if all data members will be specified in the new() command
	// ... this seems like an undue burden on the user.  Leave commented out
	//self->house  = va_arg(* app, X10_house_code_t);	// example
	//self->unit   = va_arg(* app, X10_unit_code_t);	// example

	return self;
}

void * NodeClass_X10_ctor(void * _self, va_list *app)
{
	// Add superclass class data and methods
	struct NodeClass_X10 * self = super_ctor(NodeClass_X10, _self, app);

	// Initialize new functions to default values
	typedef void (* voidf)();
	* (voidf *) & self->setHouseCode   = NULL;
	* (voidf *) & self->getHouseCode   = NULL;
	* (voidf *) & self->setUnitCode    = NULL;
	* (voidf *) & self->getUnitCode    = NULL;

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

		if (selector == (voidf) ctor)
			{* (voidf *) & ((struct Class *)self)->ctor  = overloadedFunctionPtr;}

		if (selector == (voidf) copy)
			{* (voidf *) & ((struct Class *)self)->copy = overloadedFunctionPtr;}

		if (selector == (voidf) equal)
			{* (voidf *) & ((struct Class *)self)->equal = overloadedFunctionPtr;}

		if (selector == (voidf) config)
			{* (voidf *) & ((struct Class *)self)->config = overloadedFunctionPtr;}

		if (selector == (voidf) puto)
			{* (voidf *) & ((struct Class *)self)->puto = overloadedFunctionPtr;}

		//if (selector == (voidf) config)
		//	{* (voidf *) & ((struct NodeClass *)self)->config     = overloadedFunctionPtr;}

		if (selector == (voidf) setHouseCode)
			{* (voidf *) & self->setHouseCode   = overloadedFunctionPtr;}

		if (selector == (voidf) getHouseCode)
			{* (voidf *) & self->getHouseCode   = overloadedFunctionPtr;}

		if (selector == (voidf) setUnitCode)
			{* (voidf *) & self->setUnitCode    = overloadedFunctionPtr;}

		if (selector == (voidf) getUnitCode)
			{* (voidf *) & self->getUnitCode    = overloadedFunctionPtr;}

	#ifdef va_copy
		va_end(ap);
	#endif

	}

	return self;
}

/*****************************/
/**** Overloaded Methods  ***/

void * Node_X10_copy(void * _self, const void * _copyFromObject)
{   // return pointer to copied object, else return NULL on failure

	// Validate object pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct Node_X10 * self = cast(Node_X10, _self);
	if (self == NULL )           { return NULL; } //fail

	struct Node_X10 * copyFromObject = (void *)_copyFromObject;
	if (copyFromObject == NULL ) { return NULL; } //fail


	// call super_copy variant
	// NOTE: classOf(self) calls into super will trigger looping
	super_copy(Node_X10, self, copyFromObject);

	return implement_X10_nodeCopy(self,copyFromObject);
}

equal_t Node_X10_equal(const void * _self, const void * _comparisonObject)
{
	// return TRUE if objects are equal, otherwise return FALSE

	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct Node_X10 * self = cast(Node_X10, _self);
	if (self == NULL )              { return OBJECT_UNEQUAL; } //fail

	struct Node_X10 * comparisonObject = cast(Node_X10, _comparisonObject);
	if (comparisonObject == NULL )  { return OBJECT_UNEQUAL; } //fail

	// Call super_equal method
	// NOTE: classOf(self) calls into super will trigger looping
	if( super_equal(Node_X10, self, comparisonObject)== OBJECT_UNEQUAL)
		{return OBJECT_UNEQUAL;}

	return implement_X10_nodeEqual(self, comparisonObject);
}

puto_return_t Node_X10_puto(const void * _self, FILE * _fp)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct Node_X10 * self = cast(Node_X10, _self);
	if (self == NULL ) { return PUTO_ERROR; } //fail
	return implememnt_Node_X10_puto(self, _fp);
}

void * Node_X10_config(const void * _self, void * _master)
{
	// Validate object pointers
	struct Node_X10 * self = cast(Node_X10, _self);;
	if (self == NULL )         { return NULL; } //fail

	struct Node_X10 * master = cast(Node_X10, _master);
	if (_master == NULL ) { return NULL; } //fail

	// call super_copy variant
	//super_copy(Light_X10, self, copyToObject);
	super_config(Node_X10, self, master);

	return implement_X10_config(self, master);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

/*****************************/
/*** setHouseCode Methods  ***/

X10_house_code_t setHouseCode (const void * _self,
							   const X10_house_code_t _houseCode)
{
	const struct NodeClass_X10 * class = classOf( cast(Node_X10, _self) );
	// NOTE: No check is made to insure that class ptr is a valid class
	if (class == NULL)               { return X10_HOUSE_CODE_UNKNOWN; } // fail
	if (class->setHouseCode == NULL) { return X10_HOUSE_CODE_UNKNOWN; } // fail
	return class->setHouseCode(_self, _houseCode);
}

X10_house_code_t super_setHouseCode(const void * _class,
									const void * _self,
									const X10_house_code_t _houseCode)
{
	const struct NodeClass_X10 * superclass = super(_class);
	if ( _self == NULL )
		{ return X10_HOUSE_CODE_UNKNOWN; } // fail
	if ( superclass == NULL )
		{ return X10_HOUSE_CODE_UNKNOWN; } // fail
	if ( superclass->setHouseCode == NULL )
		{ return X10_HOUSE_CODE_UNKNOWN; } // fail

	return superclass->setHouseCode(_self, _houseCode);
}

X10_house_code_t Node_X10_setHouseCode (void * _self,
										const X10_house_code_t _houseCode)
{
	struct Node_X10 * self = cast(Node_X10, _self);
	if(self==NULL) { return X10_HOUSE_CODE_UNKNOWN; }  // fail
	return implement_X10_setHouseCode(self, _houseCode);
}

/*****************************/
/** getHouseCode Methods  ****/

X10_house_code_t getHouseCode (const void * _self)
{
	const struct NodeClass_X10 * class = classOf( cast(Node_X10, _self) );
	// NOTE: No check is made to insure that class ptr is a valid class
	if (class == NULL)               { return X10_HOUSE_CODE_UNKNOWN; }// fail
	if (class->getHouseCode == NULL) { return X10_HOUSE_CODE_UNKNOWN; }// fail
	return class->getHouseCode(_self);
}

X10_house_code_t super_getHouseCode(const void * _class, const void * _self)
{
	const struct NodeClass_X10 * superclass = super(_class);
	if ( _self == NULL )
		{ return X10_HOUSE_CODE_UNKNOWN; } // fail
	if ( superclass == NULL )
		{ return X10_HOUSE_CODE_UNKNOWN; } // fail
	if ( superclass->getHouseCode == NULL )
		{ return X10_HOUSE_CODE_UNKNOWN; } // fail
	return superclass->getHouseCode(_self);
}

X10_house_code_t Node_X10_getHouseCode(const void * _self)
{
	struct Node_X10 * self = cast(Node_X10, _self);
	if(self==NULL) { return X10_HOUSE_CODE_UNKNOWN; }  // failed return path
	return implement_X10_getHouseCode(self);
}

/*****************************/
/**** setUnitCode Methods  ***/

X10_unit_code_t setUnitCode (const void * _self,
							 const X10_unit_code_t _unitCode)
{
	const struct NodeClass_X10 * class = classOf( cast(Node_X10, _self) );
	// NOTE: No check is made to insure that class ptr is a valid class
	if (class == NULL)              { return X10_UNIT_CODE_UNKNOWN; } // fail
	if (class->setUnitCode == NULL) { return X10_UNIT_CODE_UNKNOWN; } // fail
	return class->setUnitCode(_self, _unitCode);
}

X10_unit_code_t super_setUnitCode(const void * _class,
		 	 	 	 	 	 	  const void * _self,
								  const X10_unit_code_t _unitCode)
{
	const struct NodeClass_X10 * superclass = super(_class);
	if ( _self == NULL )
		{ return X10_UNIT_CODE_UNKNOWN; } // fail
	if ( superclass == NULL )
		{ return X10_UNIT_CODE_UNKNOWN; } // fail
	if ( superclass->setUnitCode == NULL )
		{ return X10_UNIT_CODE_UNKNOWN; } // fail

	return superclass->setUnitCode(_self, _unitCode);
}

X10_unit_code_t Node_X10_setUnitCode (void * _self,
		                              const X10_unit_code_t _unitCode)
{
	struct Node_X10 * self = cast(Node_X10, _self);
	if(self==NULL) { return X10_UNIT_CODE_UNKNOWN; }  // fail
	return implement_X10_setUnitCode(self, _unitCode);
}

/*****************************/
/** getUnitCode Methods  ****/

X10_unit_code_t getUnitCode (const void * _self)
{
	const struct NodeClass_X10 * class = classOf( cast(Node_X10, _self) );
	// NOTE: No check is made to insure that class ptr is a valid class
	if (class == NULL)              { return X10_UNIT_CODE_UNKNOWN; } // fail
	if (class->getUnitCode == NULL) { return X10_UNIT_CODE_UNKNOWN; } // fail
	return class->getUnitCode(_self);
}

X10_unit_code_t super_getUnitCode(const void * _class, const void * _self)
{
	const struct NodeClass_X10 * superclass = super(_class);
	if ( _self == NULL )
		{ return X10_UNIT_CODE_UNKNOWN; } // fail
	if ( superclass == NULL )
		{ return X10_UNIT_CODE_UNKNOWN; } // fail
	if ( superclass->getUnitCode == NULL )
		{ return X10_UNIT_CODE_UNKNOWN; } // fail

	return superclass->getUnitCode(_self);
}

X10_unit_code_t Node_X10_getUnitCode(const void * _self)
{
	struct Node_X10 * self = cast(Node_X10, _self);
	if(self==NULL) { return X10_UNIT_CODE_UNKNOWN; }  // fail
	return implement_X10_getUnitCode(self);
}

/*************************************************/
/*********** Implementation Procedures ***********/

void * implement_X10_nodeCopy(      struct Node_X10 * _copyToNode,
						      const struct Node_X10 * _copyFromMaster)
{
	implement_X10_setHouseCode(_copyToNode, implement_X10_getHouseCode(_copyFromMaster));
	implement_X10_setUnitCode (_copyToNode, implement_X10_getUnitCode (_copyFromMaster));
	return _copyToNode;
}

int implement_X10_nodeEqual(const struct Node_X10 * _self,
						      const struct Node_X10 * _comparisonObject)
{
	// return TRUE if objects are equal, otherwise return FALSE
	enum { unequal_FALSE = 0, equal_TRUE = 1 };

	// Check each data member for congruence
	if( implement_X10_getHouseCode(_self) !=
			implement_X10_getHouseCode(_comparisonObject) )
		{return unequal_FALSE;}

	if( implement_X10_getUnitCode(_self) !=
			implement_X10_getUnitCode(_comparisonObject) )
		{return unequal_FALSE;}

	return equal_TRUE;
}

puto_return_t implememnt_Node_X10_puto(const void * _self, FILE * _fp)
{

	puto_return_t printReturnCode = PUTO_SUCCESS;

	struct Node_X10 * self = cast(Node_X10, _self);

	// print local data members
	fprintf(_fp, "\n  Local data members managed in Node_X10:\n");

	if (PUTO_ERROR == fprintf(_fp, "  House code:  %i\n",
							  implement_X10_getHouseCode(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Unit code:   %i\n",
							  implement_X10_getUnitCode(self) ))
		{ printReturnCode = PUTO_ERROR;  } // error detected


	fprintf(_fp, "\n  New methods added in NodeList:\n");

	if (PUTO_ERROR == fprintf(_fp, "  Node_X10_setHouseCode:     %p\n", Node_X10_setHouseCode ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Node_X10_getHouseCode:     %p\n", Node_X10_getHouseCode ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Node_X10_setUnitCode:      %p\n", Node_X10_setUnitCode ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Node_X10_getUnitCode:      %p\n", Node_X10_getUnitCode ))
		{ printReturnCode = PUTO_ERROR;  } // error detected


	fprintf(_fp, "\n  Local Node_X10 methods:\n" );

	if (PUTO_ERROR == fprintf(_fp, "  Node_X10_ctor:      %p\n", Node_X10_ctor ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Node_X10_copy:      %p\n", Node_X10_copy ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Node_X10_equal:     %p\n", Node_X10_equal ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Node_X10_puto:      %p\n", Node_X10_puto ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  Node_X10_config:    %p\n", Node_X10_config ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  NodeClass_X10_ctor: %p\n\n", NodeClass_X10_ctor))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	fprintf(_fp, "Printing Node_X10 class:\n" );
	if (printReturnCode == Class_puto(Node_X10, _fp))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	// print superclass data
	fprintf(_fp, "Printing the %s superclass data for Node_X10:\n",
			((struct Object * )self)->class->superclass->className );
	// WARNING: classOf(self) calls into super will trigger looping
	if (printReturnCode == super_puto(Node_X10, self, _fp))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	fprintf(_fp, "\nPrinting NodeClass class:\n" );
	if (printReturnCode == puto(NodeClass_X10, _fp))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	return printReturnCode;
}

void * implement_X10_config(void * _self, void * _master)
{
	return copy(_self, _master);
}

X10_house_code_t implement_X10_setHouseCode(struct Node_X10 * _self,
											const X10_house_code_t _houseCode)
{
	_self->house = (X10_house_code_t)_houseCode;
	return _houseCode;
}

X10_house_code_t implement_X10_getHouseCode(const struct Node_X10 * _self)
{
	return _self->house;
}

X10_unit_code_t implement_X10_setUnitCode(struct Node_X10 * _self,
										  const X10_unit_code_t _unitCode)
{
	_self->unit = _unitCode;
	return _unitCode;
}

X10_unit_code_t implement_X10_getUnitCode(const struct Node_X10 * _self)
{
	return _self->unit;
}




