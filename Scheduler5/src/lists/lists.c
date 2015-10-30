/*
 * lists.c
 *
 *  Created on: Mar 13, 2015
 *      Author: Alan
 */

#include "lists.h"
#include "lists-private.h"				// safety include
#include "..\..\src\objects\objects.h"	// safety include
#include <stdlib.h>
//#include <stddef.h>
#include <string.h>
#include <assert.h>


/**************************************/
/***** Implementation Functions  ******/

static void adjustBufferSizeIfNeeded(const void * _self);
static void reduceBuffer(const void * _self);
static void extendBuffer(const void * _self);


static void * implement_List_dtor(struct List * _self);
static void * implement_List_copy(       struct List * _self,
		                           const struct List * _copyFromObject);

static equal_t implement_List_equal(const struct List * _self,
		                            const struct List * _comparisonList);
static puto_return_t implement_List_puto (const void * _self, FILE * _fp);
static containts_t   implement_List_contains(struct List * _self,
										     const void *  _element);
//static void * implement_List_config      (struct List * _self);
static int    implement_List_countOfItems(struct List * _self);
static void * implement_List_resetIndex  (struct List * _self);
static void * implement_List_incrementGet(struct List * _self);

static void implement_List_registerKeyClasses(void);

/*****************************/
/**** INITIALIZATIONS  *******/

const void * ListClass = NULL;
const void * List      = NULL;

//classRegistry = NULL;

// MUST be called before any list is created //
void List_init(void)
{
	// Insert the lines below for any derived subclasses
	//if (!List)      {List_init();}
	//if (!ListClass) {List_init();}

	if (! ListClass) {
		ListClass = new(Class,  		// should be "Class"
						"ListClass",	// should be "SomethingClass"
						Class,  		// should be "Class"
						sizeof(struct ListClass),
						ctor, ListClass_ctor, //SomethingClass_ctor
					    0);  // Terminator
	}
	if (! List) {
		List = new(ListClass,				// SomethingClass from above
				    "List",		  			// name like "Something"
				    Object,					// "superclass(Something)"
				    sizeof(struct List),	// size of self
					// Overloaded superclass functions
					// Remember to call superclass->method
					ctor, 		List_ctor,	//Something_ctor
					dtor, 		List_dtor,
					copy, 		List_copy,
					equal, 		List_equal,
					puto,		List_puto,
					// New functions added in this class
					// Do not call superclass->method
					add, 			List_add,
					take, 			List_take,
					contains,		List_contains,
					getItemCount,	List_getItemCount,
					resetIndex, 	List_resetIndex,
					incrementGet, 	List_getIncrement,
					0);	// Terminator
	}

	// registers Object, Class, List and ListClass
	implement_List_registerKeyClasses();

	return;
}


/***********************************/
/********** Constructors  **********/

void * List_ctor(void * _self, va_list * app)
{
	// Add superclass class data members
	struct List * self   = super_ctor(List, _self, app);
	//struct ListClass * class   = cast(ListClass, _self);

	// set default data member values to NULL
	self->buf            = NULL;
	self->arrayDimension = 0;

	// overwrite data members with new data
	// "struct List * myList = new(List, pointerToVoidPointerArray);
	// NOTE:  the pointer to the void * array is required in new()
	// NOTE:  The length of the array can be provided, or not
	self->buf            = va_arg(* app, const void **);
	//self->arrayDimension = va_arg(* app, int);
	self->arrayDimension = sizeof(*self->buf);
	self->countOfItems   = 0;
	self->beginIndex     = 0;
	self->endIndex       = 0;
	self->currentIndex   = self->beginIndex ;
	self->mutexTag       = 0;

	// Clear List values
	implement_List_setArrayValuesToNull(self);

	return self;
}

void * ListClass_ctor(void * _self, va_list *app)
{
	// Add superclass class data and methods
	struct ListClass * self = super_ctor(ListClass, _self, app);

	// Initialize new function pointers to NULL
	typedef void (* voidf)();
	* (voidf *) & self->add = NULL;
	* (voidf *) & self->take = NULL;
	* (voidf *) & self->contains = NULL;
	* (voidf *) & self->getItemCount = NULL;
	* (voidf *) & self->resetIndex = NULL;
	* (voidf *) & self->getIncrement = NULL;

	// Update any overloaded methods //
	// using style ... , methodSelector, overloadedFunctionPtr, //
	voidf selector;

	#ifdef va_copy
		va_list ap;
		va_copy(ap, * app);
	#else
		va_list ap = * app;
	#endif

	// if non-NULL selector then, grab the next method pointer
	while ( (selector = va_arg(ap, voidf)) ) {

		voidf overloadedFunctionPtr  = va_arg(ap, voidf);

		// ObjectClass overloads:
		if (selector == (voidf) dtor)
			{ * (voidf *) & self->_.dtor = overloadedFunctionPtr; }

		if (selector == (voidf) copy)
			{ * (voidf *) & self->_.copy = overloadedFunctionPtr; }

		if (selector == (voidf) equal)
			{* (voidf *) & self->_.equal = overloadedFunctionPtr; }

		if (selector == (voidf) puto)
			{* (voidf *) & self->_.puto = overloadedFunctionPtr; }

		// New ListClass assignments:
		if (selector == (voidf) add)
			{ * (voidf *) & self->add = overloadedFunctionPtr; }

		if (selector == (voidf) take)
			{* (voidf *) & self->take = overloadedFunctionPtr;}

		if (selector == (voidf) contains)
			{* (voidf *) & self->contains = overloadedFunctionPtr;}

		if (selector == (voidf) getItemCount)
			{* (voidf *) & self->getItemCount = overloadedFunctionPtr;}

		if (selector == (voidf) resetIndex)
			{* (voidf *) & self->resetIndex = overloadedFunctionPtr;}

		if (selector == (voidf) incrementGet)
			{* (voidf *) & self->getIncrement = overloadedFunctionPtr;}


	#ifdef va_copy
		va_end(ap);
	#endif

	}

	return self;
}

/********************************/
/****** Overload Methods  *******/

void * List_dtor(void * _self)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct List * self = cast(List, _self);
	if(self == NULL)                        {return NULL; } // fail

	// address local members first
	if ( implement_List_dtor(self) == NULL) {return NULL; } // fail

	// call super method after local members are addressed
	// NOTE: classOf(self) calls into super will trigger looping
	if ( super_dtor(List, _self) == NULL)   {return NULL; } // fail

	return _self;
}

void * List_copy( void * _self, const void * _copyFromObject)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct List * self = cast(List, _self);
	if (self == NULL )           { return NULL; } //fail

	struct List * copyFromObject = cast(List, _copyFromObject);
	if (copyFromObject == NULL ) { return NULL; } //fail

	// call super method first
	// NOTE: classOf(self) calls into super will trigger looping
	if (super_copy(List, self, copyFromObject) == NULL )
	                             { return NULL; } //fail

	// address local components
	return implement_List_copy(self, copyFromObject); // expected path
}

equal_t List_equal(const void * _self, const void * _comparisonObject)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct List * self = cast(List, _self);
	if (self == NULL )             { return OBJECT_UNEQUAL; } // fail

	struct List * comparisonObject = cast(List, _comparisonObject);
	if (comparisonObject == NULL ) { return OBJECT_UNEQUAL; } // fail

	// call super method first
	// NOTE: classOf(self) calls into super will trigger looping
	if( super_equal(List, self, comparisonObject)== OBJECT_UNEQUAL)
		                           { return OBJECT_UNEQUAL;} // fail

	// address local components
	return implement_List_equal(self, comparisonObject); // expected path
}

puto_return_t List_puto(const void * _self, FILE * _fp)
{
	// Validate pointers
	// NOTE: This is an overload method
	// ... use "struct myClass * self = cast(myClass, _self);"
	struct List * self = cast(List, _self);
	if (self == NULL )  { return PUTO_ERROR; } // fail

	// the call to super_puto() is managed within implement_List_puto()
	return implement_List_puto(_self, _fp);
}

/****************************************************************************/
/********  New functions for  class "ListClass"  ****************************/
/****************************************************************************/

/*****************************/
/*********** add  ************/

void * add(const void * _self, const void * _element)
{
	const struct ListClass * class = classOf( cast(List, _self) );
	if ( class == NULL )      { return NULL; } // fail
	if ( class->add == NULL ) { return NULL; } // fail
	return class->add(_self, _element);
}

void * super_add(const void * _class,
		         const void * _self, const void * _element)
{
	// verify that ListClass is in the superclass chain of _class
	if ( ! isOfSuper(ListClass, _self) ) { return NULL; } // fail
	const struct ListClass * superclass = super(_class);
	if ( superclass == NULL )            { return NULL; } // fail
	if ( superclass->add == NULL )       { return NULL; } // fail
	return superclass->add(_self, _element);
}

void * List_add(const void * _self, const void * _element)
{
	// operates as a simple FIFO list using addLast
	struct List * self = cast(List, _self);
	if ( self == NULL ) { return NULL; }   // fail
	return List_addLast(self, _element);   // expected path
}


/*****************************/
/********** take  ************/

void * take(const void * _self)
{
	const struct ListClass * class = classOf( cast(List, _self) );
	if (class == NULL)       { return NULL; } // fail
	if (class->take == NULL) { return NULL; } // fail
	return class->take(_self);
}

void * super_take(const void * _class, const void * _self)
{
	// verify that ListClass is in the superclass chain of _class
	if ( ! isOfSuper(ListClass, _self) ) { return NULL; } // fail
	const struct ListClass * superclass = super(_class);
	if ( superclass == NULL )            { return NULL; } // fail
	if ( superclass->take == NULL )      { return NULL; } // fail
	return superclass->take(_self);
}

void * List_take(const void * _self)
{
	// operates as a simple FIFO list using takeFirst
	struct List * self = cast(List, _self);
	if (self == NULL) { return NULL; }  // fail
	return List_takeFirst(self);
}


/*****************************/
/********* contains  *********/

containts_t contains(const void * _self, const void * _element)
{
	const struct ListClass * class = classOf( cast(List, _self) );
	if (class == NULL)           {return DOES_NOT_CONTAIN;} // fail
	if (class->contains == NULL) {return DOES_NOT_CONTAIN;} // fail
	return class->contains(_self, _element);
}

containts_t super_contains(const void * _class,
		                   const void * _self, const void * _element)
{
	// verify that ListClass is in the superclass chain of _class
	if ( ! isOfSuper(ListClass, _self) ) { return DOES_NOT_CONTAIN; } // fail
	const struct ListClass * superclass = super(_class);
	if ( superclass == NULL )            { return DOES_NOT_CONTAIN; } // fail
	if ( superclass->contains == NULL )  { return DOES_NOT_CONTAIN; } // fail
	return superclass->contains(_self, _element);
}

containts_t List_contains(const void * _self, const void * _element)
{
	// returns equal_TRUE if object is included in the list
	struct List * self = cast(List, _self);
	if (self == NULL )    { return DOES_NOT_CONTAIN; } //fail
	return implement_List_contains(self,_element);     // expected path
}


/**************************************/
/**********  countOfItems  ************/

int getItemCount(const void * _self)
{
	const struct ListClass * class = classOf( cast(List, _self) );
	if (class == NULL)               { return 0; } // fail
	if (class->getItemCount == NULL) { return 0; } // fail
	return class->getItemCount(_self);
}

int super_getItemCount(const void * _class, const void * _self)
{
	// verify that ListClass is in the superclass chain of _class
	if (! isOfSuper(ListClass, _self) )     { return 0; } // fail
	const struct ListClass * superclass = super(_class);
	if ( superclass == NULL )               { return 0; } // fail
	if ( superclass->getItemCount == NULL ) { return 0; } // fail
	return superclass->getItemCount(_self);
}

int List_getItemCount(const void * _self)
{
	// returns the number of items in the list
	struct List * self = cast(List, _self);
	if (self == NULL) { return 0; }  // fail
	return implement_List_countOfItems(self);
}


/********************************/
/********* resetIndex  **********/

void * resetIndex(const void * _self)
{
	const struct ListClass * class = classOf( cast(List, _self) );
	if (class == NULL)             { return NULL; } // fail
	if (class->resetIndex == NULL) { return NULL; } // fail
	return class->resetIndex(_self);
}

void * super_resetIndex(const void * _class, const void * _self)
{
	// verify that ListClass is in the superclass chain of _class
	if (! isOfSuper(ListClass, _self) )    { return NULL; } // fail
	const struct ListClass * superclass = super(_class);
	if ( superclass == NULL )              { return NULL; } // fail
	if ( superclass->resetIndex == NULL )  { return NULL; } // fail
	return superclass->resetIndex(_self);
}

void * List_resetIndex(const void * _self)
{
	// resets currentIndex to beinIndex and returns pointer
	struct List * self = cast(List, _self);
	if (self == NULL) { return NULL; }      // fail
	return implement_List_resetIndex(self); // expected path
}


/*****************************/
/********* getNext  **********/

void * incrementGet(const void * _self)
{
	const struct ListClass * class = classOf( cast(List, _self) );
	if (class == NULL)               { return NULL; } // fail
	if (class->getIncrement == NULL) { return NULL; } // fail
	return class->getIncrement(_self);
}

void * super_getIncrement(const void * _class, const void * _self)
{
	// verify that ListClass is in the superclass chain of _class
	if (! isOfSuper(ListClass, _self) )     { return NULL; } // fail
	const struct ListClass * superclass = super(_class);
	if ( superclass == NULL )               { return NULL; } // fail
	if ( superclass->getIncrement == NULL ) { return NULL; } // fail
	return superclass->getIncrement(_self);
}

void * List_getIncrement(const void * _self)
{
	//  return pointer to the currentIndex item, and increment
	struct List * self = cast(List, _self);
	if (self == NULL) { return NULL; }        // fail
	return implement_List_incrementGet(self); // expected path
}


/**************************************/
/***** implementation functions  ******/

void * List_addFirst(const void * _self, const void * _element)
{
	struct List * self = cast(List, _self);
	if (self == NULL)  { return 0; }  // fail

	/*  increment item count and extend buffer if needed  */
	self->countOfItems++;
	adjustBufferSizeIfNeeded(self);

	/* adjust beginning index */
	self->beginIndex--;
	if (self->beginIndex < 0) {
		self->beginIndex = beginIndex(self) + arrayDimension(self);
	}

	/* add the new element value  */
	self->buf[beginIndex(self)] = _element;
	return (void *)_element;  // expected path
}

void * List_addLast(const void * _self, const void * _element)
{
	struct List * self = cast(List, _self);
	if( self == NULL )  { return 0; }  // failed return path;


	/*  increment item count and extend buffer if needed  */
	self->countOfItems++;
	adjustBufferSizeIfNeeded(self);

	/* add the new element value  */
	/* endIndex is already pointing at available entry point  */
	self->buf[endIndex(self)] = _element;

	/* adjust ending index for next addition */
	self->endIndex++;
	if(self->endIndex >= arrayDimension(self)) {
		self->endIndex = endIndex(self) - arrayDimension(self);
	}

	return (void *)_element;  // expected return path
}

void * List_takeFirst(const void * _self)
{
	void * element;
	struct List * self = cast(List, _self);
	if ( self == NULL )            { return NULL; }  // fail
	if ( getItemCount(self) <= 0 ) { return NULL; }  // fail

	// Take element at beginIndex
	element = (void *)self->buf[beginIndex(self)];

	// set the taken element to NULL
	self->buf[beginIndex(self)] =  NULL;

	// Adjust beginIndex
	self->beginIndex++;
	if ( beginIndex(self) >= arrayDimension(self) ) {
		self->beginIndex = beginIndex(self) - arrayDimension(self);
	}

	// Adjust buffer size if needed
	self->countOfItems--;
	adjustBufferSizeIfNeeded(self);

	return element;      // expected return path
}

void * List_takeLast(const void * _self)
{
	void * element;
	struct List * self = cast(List, _self);
	if ( self == NULL )            { return NULL; }  // fail
	if ( getItemCount(self) <= 0 ) { return NULL; }  // fail

	// Recalculate the endIndex, then take element at new endIndex
	self->endIndex--;
	if ( endIndex(self) < 0 ) {
		self->endIndex = endIndex(self) + arrayDimension(self);
	}

	element = (void *)self->buf[endIndex(self)];

	// set the taken element to NULL
	self->buf[endIndex(self)] =  NULL;

	// Adjust buffer size if needed
	self->countOfItems--;
	adjustBufferSizeIfNeeded(self);

	return element;          // expected return path
}


/*****************************/
/********  SUPPORT  **********/

int isClass(const void * _class)
{
	// confirms that a class is registered in the classRegistry list
	// all new classes should be registered when created
	// requires that Object and List modules be active
	if( _class == NULL ) {return 0;}	// failed return
	return List_contains(classRegistry, _class);
}



static void adjustBufferSizeIfNeeded(const void * _self)
{
	struct List * self = cast(List, _self);
	if (self == NULL)              { return; }  // failed return path
	if (getItemCount(_self) <= 0 ) { return; }	// no extension needed
	if (self->buf == NULL)         { return; } 	// failed return path

	// Extend array if next addition would fill array

	// WARNING:  The "- 1" in the else if is essential to avoid chattering
	if (getItemCount(self) >= arrayDimension(self)) { 	// extension needed
		extendBuffer(_self);
		return;
	}
	else if (getItemCount(self) < (( arrayDimension(self) / 2 ) - 1 )) {
		// see warning above
		reduceBuffer(_self);
		return;
	}
	return;
}

static void reduceBuffer(const void * _self)
{
	struct List * self = cast(List, _self);
	if (self == NULL)              { return; } 	// failed return path
	if (arrayDimension(self) <= 4) { return; } 	// failed return path
	if (self->buf == NULL)         { return; } 	// failed return path

	void * oldArrayPointer = self->buf;

	// calculate the reduced array element count
	int newDimension = ( arrayDimension(self) / 2 );

	// exit without reduction if existing element count will not fit
	if (getItemCount(self) >= newDimension) { return; } // failed return path

	// calculate the new memory size count and allocate new array
	int newMemoryCount = newDimension * sizeof(* self->buf);
	void * newArrayPointer = (void *)malloc(newMemoryCount);

	// verify that new memory was successfully allocated for the array
	if (newArrayPointer == NULL) {
		assert("Lists:reduceBuffer failed to allocate memory for buffer reduction");
	}

	// set new array memory area to zero
	memset(newArrayPointer, 0, newMemoryCount);

	// copy old array upper upper block to new array upper block
	// ... from beginIndex to endIndex (non-wrapped),
	// ... or from beginIndex to arrayDim (wrapped)
	void * copyTo = newArrayPointer;
	void * copyFrom = oldArrayPointer + beginIndex(self) * sizeof(* self->buf);

	// create copy indexes.  it may take two copy operations to complete
	int memoryCopyCount1;
	int memoryCopyCount2;
	int countFromBeginIndexToArrayEnd = arrayDimension(self) - beginIndex(self);

	// test if array is wrapped.  requires two copy operations if wrapped
	if (getItemCount(self) > countFromBeginIndexToArrayEnd) {
		// array is wrapped ... adjust copy indexes for two step copy
		memoryCopyCount1 = countFromBeginIndexToArrayEnd * sizeof(* self->buf);
		memoryCopyCount2 = (getItemCount(self) - countFromBeginIndexToArrayEnd)
														* sizeof(*self->buf);
	}
	else {
		// array is not wrapped ... adjust copy indexes for single copy
		memoryCopyCount1 = getItemCount(self) * sizeof(* self->buf);
		memoryCopyCount2 = 0;
	}

	// first copy is valid for all array configurations
	memcpy(copyTo, copyFrom, memoryCopyCount1 );

	// second copy is valid for any wrapped array data configurations
	if(memoryCopyCount2) {			// true if array is wrapped
		copyTo = newArrayPointer +
				         countFromBeginIndexToArrayEnd * sizeof(* self->buf);
		copyFrom = oldArrayPointer;
		memcpy(copyTo, copyFrom, memoryCopyCount2 );
	}

	// set the new beginIndex and endIndex values, which are zero aligned
	int newBeginIndex = 0;
	int newEndIndex  = getItemCount(self);

	// copy is complete; free just the original buffer only
	free(self->buf);

	// Update self with new data components
	self->arrayDimension = newDimension;
	self->buf =newArrayPointer;
	self->beginIndex = newBeginIndex;
	self->endIndex = newEndIndex;

	return;	// expected return path
}

static void extendBuffer(const void * _self)
{
	struct List * self = cast(List, _self);
	if (self == NULL)      { return; }  // failed return path
	if (self->buf == NULL) { return; } 	// failed return path

	void * oldArrayPointer = self->buf;

	// calculate the increased array element count
	int newDimension = ( arrayDimension(self) * 2 );

	// calculate the new memory size count and allocate new array
	int newMemoryCount = newDimension * sizeof(void *);
	void * newArrayPointer = (void *)malloc(newMemoryCount);

	// verify that new memory was successfully allocated for the array
	if (newArrayPointer == NULL) {
		assert("Lists:extendBuffer failed to allocate buffer extension memory");
	}

	// set new array memory area to zero
	memset(newArrayPointer, 0, newMemoryCount);

	// copy from beginIndex to endIndex (non-wrapped) or arrayDim (wrapped) to upper block
	void * copyTo = newArrayPointer + (arrayDimension(self) +
			                          beginIndex(self)) * sizeof(*self->buf);
	void * copyFrom = oldArrayPointer + beginIndex(self)*sizeof(*self->buf);
	int copyCount = (arrayDimension(self) - beginIndex(self))*sizeof(*self->buf);
	memcpy(copyTo, copyFrom, copyCount );

	// Set the new beginIndex and tentatively set the endIndex
	int newEndIndex  = arrayDimension(self) + endIndex(self);
	int newBeginIndex = arrayDimension(self) + beginIndex(self);

	// copy from index zero to endIndex into lower block for wrapped arrays
	if(beginIndex(self) > endIndex(self)) {	 // true if array is wrapped
		copyTo = newArrayPointer;
		copyFrom = oldArrayPointer;
		copyCount = endIndex(self) *sizeof(void *);
		memcpy(copyTo, copyFrom, copyCount );

		// Update endIndex (beginIndex is already correctly set)
		newEndIndex = endIndex(self);
	}

	// copy is complete; free just the original buffer only
	free(self->buf);

	// Update self with new data components
	self->arrayDimension = newDimension;
	self->buf =newArrayPointer;
	self->endIndex = newEndIndex;
	self->beginIndex = newBeginIndex;

	return;  // expected path
}

void * implement_List_setArrayValuesToNull(void * _self)
{
	struct List * self = _self;
	if (self == NULL)              { return NULL; } 	// fail
	if (self->buf == NULL)         { return NULL; } 	// fail
	if (arrayDimension(self) <= 0) { return NULL; } 	// fail

	int i = 0;
	for ( i = 0; i < arrayDimension(self); i++ ){
		self->buf[i] = NULL;
	}
	return self;
}

void * clearListObjectValues(void * _self)
{
	struct List  * self = _self;
	if (self == NULL) { return NULL; } 	// failed return path

	implement_List_setArrayValuesToNull(self);
	self->buf            = NULL;
	self->arrayDimension = 0;
	self->countOfItems   = 0;
	self->beginIndex     = 0;
	self->endIndex       = 0;
	return _self;
}

/***********************************************/
/******** implementation functions  ************/
/***********************************************/

static void * implement_List_dtor(struct List * _self)
{
	// WARNING:  leak risk for setting array values to NULL without using free()
	implement_List_setArrayValuesToNull(_self);
	clearListObjectValues(_self);
	return _self;
}

static void * implement_List_copy(       struct List * _self,
		                           const struct List * _copyFromObject)
{
	int localItemCount = getItemCount(_copyFromObject);
	void * element;
	resetIndex(_copyFromObject);
	int i;
	for ( i = 0; i < localItemCount; i++) {

		// assign the same pointers to two lists, for access the same members
		element = incrementGet(_copyFromObject);
		if ( add(_self, element) == NULL ) { return NULL; } //fail
	}
	return _self;  // expected path
}


static equal_t implement_List_equal(const struct List * _self,
		                            const struct List * _comparisonList)
{
	// the _comparisonList is the master
	// the itemCount in _self may not match that of _comparisonList
	struct List * self           = (void *)_self;
	void * element;

	int localItemCount = getItemCount(_comparisonList);
	if ( localItemCount <= 0 ) { return OBJECT_UNEQUAL; } // fail empty master

	// iterate through the list entries, to check if the "contents" are equal
	resetIndex(_comparisonList);
	int i;
	for( i = 0; i < localItemCount; i++) {
		element = incrementGet(_comparisonList);

		if ( contains(self, element) == DOES_NOT_CONTAIN )
			{ return OBJECT_UNEQUAL; } // fail item not found

	}  // end of for loop

	return OBJECT_EQUAL;  // success, equal
}


static puto_return_t implement_List_puto(const void * _self, FILE * _fp)
{
	puto_return_t printReturnCode = PUTO_SUCCESS;

	struct List * self = cast(List, _self);

	// print local data members
	fprintf(_fp, "\n  Local data members managed in List:\n");

	if (PUTO_ERROR == fprintf(_fp, "  buf:            %p\n", self->buf ) )
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  arrayDimension: %i\n",
							  self->arrayDimension ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  countOfItems:   %i\n",
								  self->countOfItems ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  beginIndex:     %i\n",
								  self->beginIndex ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  endIndex:       %i\n",
								  self->endIndex ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  currentIndex:   %i\n",
								  self->currentIndex ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp, "  mutexTag:       %i\n",
								  self->mutexTag ))
		{ printReturnCode = PUTO_ERROR;  } // error detected


	fprintf(_fp, "\n  New methods added in List:\n");

	if (PUTO_ERROR == fprintf(_fp,
		"  List_add:            %p\n", List_add) )
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp,
		"  List_take:           %p\n", List_take) )
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp,
		"  List_contains:       %p\n", List_contains) )
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp,
		"  List_getItemCount:   %p\n", List_getItemCount) )
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp,
		"  List_resetIndex:     %p\n", List_resetIndex) )
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp,
		"  List_getIncrement:   %p\n", List_getIncrement) )
		{ printReturnCode = PUTO_ERROR;  } // error detected


	fprintf(_fp, "\n  Locally overloaded Object methods:\n" );

	/* if (PUTO_ERROR == fprintf(_fp, "  List_config:    %p\n", List_config ))
			{ printReturnCode = PUTO_ERROR;  } // error detected */

	if (PUTO_ERROR == fprintf(_fp,
		"  List_ctor:      %p\n", List_ctor ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp,
		"  List_dtor:      %p\n", List_dtor  ))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp,
		"  List_copy:      %p\n", List_copy  ) )
		{ printReturnCode = PUTO_ERROR;  } // error detected */

	if (PUTO_ERROR == fprintf(_fp,
		"  List_equal:     %p\n", List_equal  ) )
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp,
		"  List_puto:      %p\n", List_puto  ) )
		{ printReturnCode = PUTO_ERROR;  } // error detected

	if (PUTO_ERROR == fprintf(_fp,
		"  ListClass_ctor: %p\n", ListClass_ctor  ) )
		{ printReturnCode = PUTO_ERROR;  } // error detected

	// print superclass data
	fprintf(_fp, "\nPrinting the %s superclass data for List:\n",
                 ((struct Object * )self)->class->superclass->className );
	// WARNING: classOf(self) calls into super will trigger looping
	if (printReturnCode == super_puto(List, self, _fp))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	fprintf(_fp, "\nPrinting List class:\n" );
	if (printReturnCode == Class_puto(List, _fp))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	fprintf(_fp, "\nPrinting ListClass class:\n" );
	if (printReturnCode == puto(ListClass, _fp))
		{ printReturnCode = PUTO_ERROR;  } // error detected

	return printReturnCode;
}

static containts_t implement_List_contains(struct List * _self,
		                                   const void *  _element)
{
	// return equal_TRUE if object is included in the list
	if (_self == NULL )    { return DOES_NOT_CONTAIN; } //fail

	// iterate through the ENTIRE list entries, checking for an element match
	int i;
	implement_List_resetIndex(_self);
	for( i = 0; i < getItemCount(_self); i++) {

		if ( incrementGet(_self) == _element) { return DOES_CONTAIN; } //found
	}
	return DOES_NOT_CONTAIN; //fail
}

static int implement_List_countOfItems(struct List * _self)
{
	return _self->countOfItems;
}

void * implement_List_resetIndex(struct List * _self)
{
	struct List * self = (void *)_self;
	if ( getItemCount(self) <= 0 ) {return NULL; } // fail for empty list

	self->currentIndex = beginIndex(self);
	return (void *)self->buf[ currentIndex(self) ];
}

void * implement_List_incrementGet(struct List * _self)
{
	struct List * self = (void *)_self;

	if ( getItemCount(self) <= 0 ) {return NULL; } // fail for empty list

	// confirm that currentIndex does not equal endIndex, otherwise increment
	if( currentIndex(self) == endIndex(self)) {
		// currentIndex can point to the endIndex
		// endIndex is an unused location
		// the currentIndex should be wrapped to the beginIndex
		self->currentIndex = beginIndex(self); }
	else {
		// Adjust currentIndex to next value in the list
		self->currentIndex++;
	}

	// wrap index if beginIndex is past usable array range
	if ( self->currentIndex >= arrayDimension(self) )
		{ self->currentIndex = 0; }

	// reset the currentIndex if it equals endIndex after increment
	if( currentIndex(self) == endIndex(self))
	                                 { self->currentIndex = beginIndex(self);}

	// get value at currentIndex
	void * returnPTR;
	returnPTR = (void *)self->buf[ currentIndex((const struct List *)self) ];

	return returnPTR;
}

static void implement_List_registerKeyClasses(void)
{
	// registers Object, Class, List and ListClass
	if ( classRegistry == NULL ) { classRegistry = new(List, registryArray, 8); }

	if ( !List_contains(classRegistry,  List) )
		{ List_add(classRegistry, List); }
	if ( !List_contains(classRegistry,  ListClass) )
		{ List_add(classRegistry, ListClass); }
	if ( !List_contains(classRegistry,  Class) )
		{ List_add(classRegistry, Class); }
	if ( !List_contains(classRegistry,  Object) )
		{ List_add(classRegistry, Object); }
	return;
 }
