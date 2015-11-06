/*
 * objects.c
 *
 *  Created on: Feb 24, 2015
 *      Author: Alan
 */

#include "objects.h"
#include "objects-private.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
//#include "..\..\src\times\times.h"
#include "..\..\test\unity\unity_fixture.h"  // include for unity memory leak test
#include "..\..\test\unity\unity_fixture_malloc_overrides.h"  // include for unity memory leak test


/*****************************/
/*** Support functions  ******/

puto_return_t implement_putoPreamble(void * _self, FILE * _fp);


/*****************************/
/**** INITIALIZATIONS  *******/

static const struct Class object [] = {
	{ {object + 1},  			// points to class descriptor CLASS
	   "Object", 				// Name
	   object,					// Object is it's own superclass
	   sizeof(struct Object),	// size of "struct Object"
	   Object_ctor,				// points to constructor for objects
	   Object_dtor,				// points to destructor for objects
	   Object_copy,			    // points to clone for objects
	   Object_equal,			// points to equal for objects
	   Object_config,			// points to config function for Object
	   Object_puto				// points to "put to file" for objects
	},
    {  {object + 1},  			// points to class descriptor CLASS
	   "Class", 				// Name
	   object,					// points to superclass
	   sizeof(struct Class),	// size of "struct Class"
	   Class_ctor,				// points to constructor for classes
	   Class_dtor,				// points to destructor for classes
	   Class_copy,			    // points to clone for objects
	   Class_equal,				// points to equal function for Object
	   Class_config,			// points to config function for Object
	   Class_puto				// points to "put to file" for Object
    }
};

const void * Object = object;  	  // first elements of initialization array
const void * Class  = object + 1; // second elements of initialization array


/****************************/

void * new(const void * _class, ...)
{
	// verify valid class descriptor and memory size
	const struct Class * class = _class;
	if (class == NULL)               { return  NULL; } // fail
	if (class->sizeOfThisClass <= 0) { return  NULL; } // fail

	// verify that memory allocation completed successfully
	struct Object * object;
	object = (void *)calloc(1, class->sizeOfThisClass);
	if (object == NULL)              { return  NULL; } // fail

	// initialize the class pointer to the appropriate class description
	object->class = class;

	// start VA list and call top level generic constructor
	va_list ap;
	va_start(ap, _class);
	object = ctor(object, & ap);
	va_end(ap);

	return object;
}

void * delete(void * _self)
{
	struct Object * self = cast(Object, _self);
	if ( self == NULL)        {return NULL; }  // fail

	// execute dtor  non-NULL pointer return indicates success
	if ( dtor(self) == NULL ) {return NULL; }  // fail


	// WARNING:  The object's pointer must be set to NULL as part of delete()
	// myTestList = ((myTestList == delete(myTestList)) ? NULL : myTestList );
	// myTestList = safeDelete(myTestList);

	// WARNING:  Watch for a second delete() call in unit testing when
	// a delete() call is made in both the cleanup and in a specific unit test
	// use one of the pointer assignments above for safety

	// free the memory block
	if( self == NULL )        { return NULL; }  // fail
		free(self);

	return (void *) _self;	// return original pointer on success
}

/*****************************/
/**********  ctor  ***********/

void * ctor(void * _self, va_list * app)
{
	const struct Class * class = classOf( cast(Object, _self) );
	if (class == NULL)       { return NULL; } // fail
	if (class->ctor == NULL) { return NULL; } // fail
	return class->ctor(_self, app);
}

void * super_ctor(const void * _class, void * _self, va_list * app)
{
	const struct Class * superclass = super(_class);
	if( superclass == NULL )       { return NULL; }  // fail
	if( superclass->ctor == NULL ) { return NULL; }  // failed return path
	return (superclass->ctor(_self, app));
}

void * Object_ctor(void * _self, va_list * app)
{
	return _self;
}

void * Class_ctor(void * _self, va_list * app)
{
	typedef void (* voidf)(); 	// generic function pointer

	// Initialize data and pointers
	struct Class * self   = _self;
	self->className       = va_arg(* app, char *);
	self->superclass      = va_arg(* app, struct Class *);
	self->sizeOfThisClass = va_arg(* app, int);

	// Copy the superclass method function pointers
	if (self->superclass == NULL) { return NULL; } // fail
	const int offset = offsetof(struct Class, ctor);
	memcpy( (char *) self + offset,			/*  */
			(char *)self->superclass + offset,	/*  */
			sizeOf(self->superclass) - offset);	/*  */

	// Update any overloaded methods
	voidf selector;   // use "methodSelector, overloadedFunctionPtr,"

	#ifdef va_copy
		va_list ap;
		va_copy(ap, * app);
	#else
		va_list ap = * app;
	#endif

	// if non-NULL selector then, grab the next method pointer
	// end selector list with "0); // terminator character"
	while ( (selector = va_arg(ap, voidf)) ) {
		voidf overloadedFunctionPtr  = va_arg(ap, voidf);

		if (selector == (voidf) ctor)
			{* (voidf *) & self->ctor = overloadedFunctionPtr;}

		else if (selector == (voidf) dtor)
			{* (voidf *) & self->dtor = overloadedFunctionPtr;}

		else if (selector == (voidf) copy)
			{* (voidf *) & self->copy = overloadedFunctionPtr;}

		else if (selector == (voidf) equal)
			{* (voidf *) & self->equal = overloadedFunctionPtr;}

		else if (selector == (voidf) config)
			{* (voidf *) & self->config = overloadedFunctionPtr;}

		else if (selector == (voidf) puto)
			{* (voidf *) & self->puto = overloadedFunctionPtr;}

	#ifdef va_copy
		va_end(ap);
	#endif

	}

	return self;
}


/*****************************/
/**********  dtor  ***********/

void * dtor(void * _self)
{
	const struct Class * class = classOf(_self);
	if(class == NULL)       {return NULL; }  // fail
	if(class->dtor == NULL) {return NULL; }  // fail
	return (class->dtor(_self));
}

void * super_dtor(const void * _class, void * _self)
{
	const struct Class * superclass = super(_class);
	if(superclass == NULL)       {return NULL; }  // fail
	if(superclass->dtor == NULL) {return NULL; }  // fail
	return (superclass->dtor(_self));
}

void * Object_dtor(void * _self)
{
	// cannot destroy a base Object.  Should just return _self
	return _self;
}

void * Class_dtor(void * _self)
{
	// cannot destroy a base Class.  Should just return NULL
	return NULL;
}


/*****************************/
/*********  copy  ***********/

void * copy(void * _self, const void * _copyFromObject)
{
	const struct Class * class = classOf( cast(Object, _self) );
	if (class == NULL )           { return NULL; } // fail
	if (class->copy == NULL )     { return NULL; } // fail
	if (_copyFromObject == NULL ) { return NULL; } // fail
	return class->copy(_self, _copyFromObject);
}

void * super_copy(const void * _class,
                        void * _self,
			      const void * _copyFromObject)
{
	const struct Class * superclass = super(_class);
	if ( _self == NULL )            { return NULL; }// fail
	if ( superclass == NULL )       { return NULL; }// fail
	if ( superclass->copy == NULL ) { return NULL; }// fail
	return superclass->copy(_self, _copyFromObject);
}

void * Object_copy(void * _self, const void * _copyFromObject)
{
	// returns pointer to self on success, else returns NULL
	struct Node * self = cast(Object, _self);
	if( self == NULL )            { return NULL; }

	//_copyFromObject == NULL check is normally placed in the implement method
	if( _copyFromObject == NULL ) { return NULL; }

	// there are no data members in Object to copy
	// normally use form: "return implement_copy( _self, _copyFromObject);"
	return _self;
}

void * Class_copy(void * _self, const void * _copyFromObject)
{
	// returns pointer to self on success, else returns NULL
	struct Class * self = cast(Class, _self);
	struct Object * object = (void *)self;
	if( self == NULL ) { return NULL; }

	//_copyFromObject == NULL check is normally placed in the implement method
	struct Class * copyFromObject = (void *)cast(Class, _copyFromObject);
	if( _copyFromObject == NULL ) { return NULL; }

	// WARNING: is advisable to copy Classes at all?
	// Copy each element here
	object->class          = (void *)(copyFromObject->_.class);
	self->className        = copyFromObject->className;
	self->superclass       = copyFromObject->superclass;
	self->sizeOfThisClass  = copyFromObject->sizeOfThisClass;
	self->className        = copyFromObject->className;
	self->ctor             = copyFromObject->ctor;
	self->dtor             = copyFromObject->dtor;
	self->copy             = copyFromObject->copy;
	self->equal            = copyFromObject->equal;
	self->puto             = copyFromObject->puto;
	return _self;  // return if copy operation is executed

	return NULL;  // return if copy block above is commented out
}

/*****************************/
/*********  equal  ***********/

equal_t equal(const void * _self, const void * _comparisonObject)
{
	const struct Class * class = classOf( cast(Object, _self) );
	if (class == NULL)             { return OBJECT_UNEQUAL; } // fail
	if (class->equal == NULL)      { return OBJECT_UNEQUAL; } // fail
	if (_comparisonObject == NULL) { return OBJECT_UNEQUAL; } // fail
	return class->equal(_self, _comparisonObject);
}

equal_t super_equal(const void * _class,
				const void * _self,
				const void * _comparisonObject)
{
	const struct Class * superclass = super(_class);
	if(superclass == NULL)        { return OBJECT_UNEQUAL; }  // fail
	if(superclass->equal == NULL) { return OBJECT_UNEQUAL; }  // fail
	return superclass->equal(_self, _comparisonObject);
}

equal_t Object_equal(const void * _self, const void * _comparisonObject)
{
	// return OBJECT_EQUAL if objects equal, otherwise return OBJECT_UNEQUAL
	//enum { unequal_FALSE = 0, equal_TRUE = 1 };
	struct Object * comparisonObject = (void *)_comparisonObject;
	struct Object * self             = (void *)_self;
	if (comparisonObject == NULL )  { return OBJECT_UNEQUAL; } //fail
	if (self == NULL )              { return OBJECT_UNEQUAL; } //fail

	// returns True if this point in processing is reached
	// ... since Objects only contain a class pointer with no data members
	// normally use form: "return implement_equal( _self, _comparisonObject);"
	return OBJECT_EQUAL;
}

equal_t Class_equal(const void * _self, const void * _comparisonObject)
{
	// return OBJECT_EQUAL if objects are equal, otherwise return OBJECT_UNEQUAL
	struct Class * comparisonObject = (void *)_comparisonObject;
	struct Class * self             = (void *)_self;
	if (comparisonObject == NULL )  { return OBJECT_UNEQUAL; } //fail
	if (self == NULL )              { return OBJECT_UNEQUAL; } //fail

	// returns True all data members are congruent
	if ( self->_.class != comparisonObject->_.class )
		{ return OBJECT_UNEQUAL; }
	if ( self->className    != comparisonObject->className )
		{ return OBJECT_UNEQUAL; }
	if ( self->superclass   != comparisonObject->superclass )
		{ return OBJECT_UNEQUAL; }
	if ( self->sizeOfThisClass    != comparisonObject->sizeOfThisClass )
		{ return OBJECT_UNEQUAL; }

	if ( self->ctor    != comparisonObject->ctor )  { return OBJECT_UNEQUAL; }
	if ( self->dtor    != comparisonObject->dtor )  { return OBJECT_UNEQUAL; }
	if ( self->copy    != comparisonObject->copy )  { return OBJECT_UNEQUAL; }
	if ( self->equal   != comparisonObject->equal ) { return OBJECT_UNEQUAL; }
	if ( self->puto    != comparisonObject->puto )  { return OBJECT_UNEQUAL; }

	return OBJECT_EQUAL;
}

/*****************************/
/** autoConfigure Methods  ***/

void * config(const void * _self, const void * _master)
{
	const struct Class * class = classOf( cast(Object, _self) );
	if ( class == NULL )         { return NULL; } // fail
	if ( class->config == NULL ) { return NULL; } // fail
	return class->config (_self, _master);
}

void * super_config(const void * _class,
			        const void * _self,
		            const void * _master)
{
	const struct Class * superclass = super(_class);
	if ( superclass == NULL )         { return NULL; } // fail
	if ( superclass->config == NULL ) { return NULL; } // fail
	return superclass->config(_self, _master);
}

void * Object_config(const void * _self, const void * _master)
{
	// configures _self using data in _master
	// returns _self on success
	struct Object * master = (void *)_master;
	struct Object * self   = (void *)_self;
	if ( master == NULL )  { return NULL; } //fail
	if ( self == NULL )    { return NULL; } //fail

	// returns True if this point in processing is reached
	// ... since Objects only contain a class pointer with no data members
	// normally use form: "return implement_config( _self, _master);"
	return (void *)_self;
}

void *  Class_config(const void * _self, const void * _master)
{
	// takes no action to configure classes
	return (void *)_self;
}


/*****************************/
/**********  puto  ***********/

puto_return_t puto(const void * _self, FILE * _fp)
{
	const struct Class * class = classOf( cast(Object, _self) );
	if (class == NULL)       { return PUTO_ERROR; } // fail
	if (class->puto == NULL) { return PUTO_ERROR; } // fail
	if( _fp == NULL )        { return PUTO_ERROR; } // fail

	if( PUTO_ERROR == implement_putoPreamble((void *)_self, _fp) )
		                     { return PUTO_ERROR; } // fail

	return (class->puto(_self, _fp));
}

puto_return_t super_puto(const void * _class,
		                      const void * _self,
		                      FILE * _fp)
{
	const struct Class * superclass = super(_class);
	if ( _self == NULL )            { return PUTO_ERROR; } // fail
	if ( superclass == NULL )       { return PUTO_ERROR; } // fail
	if ( superclass->puto == NULL ) { return PUTO_ERROR; } // fail
	return superclass->puto(_self, _fp);
}

puto_return_t Object_puto(const void * _self, FILE * _fp)
{
	fprintf(_fp, " Printing local data members managed in Object:\n");
	fprintf(_fp, "  ... no data members are stored in Object\n");

	// normally use form: "return implement_puto( _self, _fp);"
	return PUTO_SUCCESS;
}

puto_return_t Class_puto(const void * _class, FILE * _fp)
{
	// print comprehensive Class data
	const struct Class * class = _class;

	fprintf(_fp, "The class structure %s located at: %p\n",
			class->className, class);
	fprintf(_fp,
			" This structure is controlled by the class descriptor: %s at %p\n",
			class->_.class->className,
			class->_.class);
	fprintf(_fp, " The superclass is %s, located at %p\n",
			class->superclass->className, class->superclass);
	fprintf(_fp, " Size of this %s structure is %u\n", class->className,
			sizeOf(class));
	fprintf(_fp, " The basic %s methods are:\n", class->className);

	if (class->ctor)
		{ fprintf(_fp, " ctor   function located at %p\n", class->ctor); }
		else { fprintf(_fp, " ctor function is NULL\n"); }

	if (class->dtor)
		{ fprintf(_fp, " dtor   function located at %p\n", class->dtor); }
		else { fprintf(_fp, " dtor function is NULL\n"); }

	if (class->copy)
		{ fprintf(_fp, " copy   function located at %p\n", class->copy); }
		else { fprintf(_fp, " copy function is NULL\n"); }

	if (class->equal)
		{ fprintf(_fp, " equal  function located at %p\n", class->equal); }
		else { fprintf(_fp, " equal function is NULL\n"); }

	if (class->config)
		{ fprintf(_fp, " config function located at %p\n", class->config); }
		else { fprintf(_fp, " config function is NULL\n"); }


	if (class->puto)
		{ fprintf(_fp, " puto   function located at %p\n", class->puto); }
		else { fprintf(_fp, " puto function is NULL\n\n"); }

	fprintf(_fp, "\n");

	return PUTO_SUCCESS;
}


/*****************************/
/********  SUPPORT  **********/

const void * classOf(const void * _self)
{
	// accepts class or object pointers
	const struct Object * self = _self;
	if( self == NULL ) {return NULL;}	// failed return
	return self->class;
}

int sizeOf(const void * _self)
{
	/* can accept class or object pointers */
	const struct Class * class = classOf(_self);
	if( class == NULL ) {return 0;}	// failed return
	return class->sizeOfThisClass;
}

const void * super(const void * _self)
{
	// returns pointer to superclass descriptor
	const struct Class * self = _self;
	if ( self        == NULL ) { return NULL; }	// failed return path
	return self->superclass;
}

int isOfSuper(const void * _self, const struct Class * _superClass)
{
	// if _self isOfSuper _class return isOfSuper_TRUE, else isOfSuper_FAIL

	enum { isOfSuper_FAIL = 0, isOfSuper_TRUE = 1 };

	// fail on NULL pointers
	if ( _self  == NULL )      { return isOfSuper_FAIL; } // fail
	if ( _superClass == NULL ) { return isOfSuper_FAIL; } // fail

	// can not be superclass of own self.  Prevents Object-Object looping
	if ( _self == _superClass ) { return isOfSuper_FAIL; } // fail

	// fail if the class of _self and the suggested superclass are the same
	const struct Class * myClass = classOf(_self);
	if ( myClass  == _superClass) { return isOfSuper_FAIL; } // fail

	// follow the superclass down to Object looking for _superClass
	// return isOfSuper_TRUE if _superClass is in the _self superclass chain
	const struct Class * mySuperClass = super(myClass);
	do {
		// check for successful superClass match found.  SUCCESSFUL return
		if (mySuperClass == _superClass) { return isOfSuper_TRUE; }

		// check if the base class of Object has been reached
		// indicates end of chain, and failure to confirm the match
		if ( myClass == Object )  { return isOfSuper_FAIL; } // fail

		// get next superclass in the chain for next check
		mySuperClass = super(mySuperClass);

	} while (mySuperClass != Object);

	// NOTE:  mySuperClass != Object is a redundant check, but it depends on
	//        the detailed construction of Object having superclass of Object
	//        Normal execution should not reach the end of the do loop
	return isOfSuper_FAIL;  // fail
}

int isA_sc(const void * _self, const struct Class * _class)
{
	// if _self isA _class (or object), then return 1, else 0
	// accepts object items and class items

	if( _class == NULL ) { return 0; }	// failed return path
	if( _self  == NULL ) { return 0; }	// failed return path

	const struct Object * self = _self;
	return ( classOf(self) == _class );
}

int isOf_sc(const void * _self, const struct Class * _class)
{
	// if _self isOf _class, then return isOf_TRUE, else isOf_FALSE

	enum { isOf_FALSE = 0, isOf_TRUE = 1 };

	// fail on NULL pointers
	if ( _self  == NULL )   { return isOf_FALSE; }	// failed return path
	if ( _class == NULL )   { return isOf_FALSE; }	// failed return path

	// returng success if _class == Object since everything is an Object
	if ( _class == Object ) { return isOf_TRUE;  }

	//// follow the superclass down to Object looking for _superClass
	//// return isOfSuper_TRUE if _superClass is in the _self superclass chain
	const struct Class * myClass = classOf(_self);
	if ( _class == myClass ) { return isOf_TRUE;  }

	if( isOfSuper(_self, _class) ) { return isOf_TRUE;}
	return isOf_FALSE;
}

void * cast(const struct Class * _class, const void * _self)
{
	// if _self is within the superclass chain of _class, then return _self
	return ( isOf_sc(_self, _class) ? ((void *)_self) : NULL );
}

/****************************************/
/****************************************/
/****************************************/
puto_return_t implement_putoPreamble(void * _self, FILE * _fp)
{
	// Print preamble to the file
	const struct Class * class = classOf( cast(Object, _self) );
	if( _self == NULL )
	{ 	fprintf(_fp, "ERROR: _self in puto is NULL\n");
		return PUTO_ERROR; }  // fail
	else { fprintf(_fp, "This object is located at: %p", _self); } // expected

	fprintf(_fp,
		",\n... and is controlled by the class descriptor: %s at %p\n",
		class->className, class);
	return PUTO_SUCCESS;
}


