/*
 * stacks.c
 *
 *  Created on: Mar 13, 2015
 *      Author: Alan
 */

#include "..\..\src\lists\lists.h"
#include "..\..\src\lists\lists-private.h"
#include "stacks.h"
#include "stacks-private.h"
#include "..\..\src\objects\objects.h"
#include <stdlib.h>
//#include <stddef.h>
#include <string.h>
//#include <assert.h>

/*
static void adjustBufferSizeIfNeeded(const void * _self);
static void reduceBuffer(const void * _self);
static void extendBuffer(const void * _self);
*/


/*****************************/
/**** INITIALIZATIONS  *******/

const void * StackClass = NULL;
const void * Stack      = NULL;

//classRegistry = NULL;

void Stack_init(void)
// MUST be called before any list is created //
{
	if( ! ListClass ) { List_init(); }
	if( ! List )      { List_init(); }

	if (! StackClass) {
		StackClass = new(Class,  // Usually this should be "Class"
						 "StackClass",
						 Class,  // Usually this should be "Class"
						 //Class,
						 sizeof(struct StackClass),
						 // Overloaded superclass functions
						 // Remember to call superclass->method
						 ctor, StackClass_ctor,

						 // Terminator
						 0);
	}
	if (! Stack) {
		Stack = new(StackClass,
				    "Stack",
				    List,
				    sizeof(struct Stack),
					// Overloaded superclass functions
					// Remember to call superclass->method
					ctor, Stack_ctor,
					dtor, Stack_dtor,
					// New functions added in this class
					// Do not call superclass->method
					add, List_addLast,
					take, List_takeLast,
					//contains, List_contains,
					//addFirst, List_addFirst,
					//addLast, List_addLast,
					//takeFirst, List_takeFirst,
					//takeLast, List_takeLast,
					// Terminator
					0);
	}

	// register Stack and StackClass
	Stack_registerKeyClasses();

	return;
}

/*****************************/
/********  SUPPORT  **********/
/*
int isClass(const void * _class)
{
	// confirms that a class is registered in the classRegistry list
	// all new classes should be registered when created
	// requires that Object and List modules be active
	if( _class == NULL ) {return 0;}	// failed return
	return List_contains(classRegistry, _class);
}
*/
void Stack_registerKeyClasses(void)
{
	// registers Object, Class, List and ListClass
	if ( classRegistry == NULL ) { classRegistry = new(List, registryArray, 8); }

	if ( !List_contains(classRegistry,  Stack) )      { add(classRegistry, Stack); }
	if ( !List_contains(classRegistry,  StackClass) ) { add(classRegistry, StackClass); }
	return;
 }

/***********************************/
/** Constructors and Destructors  **/

void * Stack_ctor(void * _self, va_list * app)
{
	/// Add superclass class data members //
	struct Stack * self   = super_ctor(Stack, _self, app);

	/*
	// set default NULL values //
	self->buf            = NULL;
	self->arrayDimension = 0;

	// overwrite with new data //
	self->buf            = va_arg(* app, const void **);
	self->arrayDimension = va_arg(* app, int);
	self->countOfItems   = 0;
	self->beginIndex     = 0;
	self->endIndex       = 0;

	// insure that array values are NULL on creation //
	setArrayValuesToNull(self);
	*/
	return self;
}

void * StackClass_ctor(void * _self, va_list *app)
{
	// Add superclass class data and methods //
	struct ListClass * self = super_ctor(ListClass, _self, app);

	// Initialize overloaded function pointers to NULL //
	typedef void (* voidf)();
	* (voidf *) & self->add = NULL;
	* (voidf *) & self->take = NULL;
	//* (voidf *) & self->contains = NULL;

	//* (voidf *) & self->addFirst = NULL;
	//* (voidf *) & self->addLast = NULL;
	//* (voidf *) & self->takeFirst = NULL;
	//* (voidf *) & self->takeLast = NULL;


	// Update any overloaded methods //
	// using style ... , methodSelector, overloadedFunctionPtr, //
	voidf selector;

	#ifdef va_copy
		va_list ap;
		va_copy(ap, * app);
	#else
		va_list ap = * app;
	#endif

	while ( (selector = va_arg(ap, voidf)) ) {            /* if non-NULL selector then,   */
		voidf overloadedFunctionPtr  = va_arg(ap, voidf); /* grab the next method pointer */

		if (selector == (voidf) add)
			{* (voidf *) & self->add = overloadedFunctionPtr;}
		if (selector == (voidf) take)
			{* (voidf *) & self->take = overloadedFunctionPtr;}
		if (selector == (voidf) contains)
			{* (voidf *) & self->contains = overloadedFunctionPtr;}
		/*
		if (selector == (voidf) addFirst)
			{* (voidf *) & self->addFirst = overloadedFunctionPtr;}
		if (selector == (voidf) addLast)
			{* (voidf *) & self->addLast = overloadedFunctionPtr;}
		if (selector == (voidf) takeFirst)
			{* (voidf *) & self->takeFirst = overloadedFunctionPtr;}
		if (selector == (voidf) takeLast)
			{* (voidf *) & self->takeLast = overloadedFunctionPtr;}
		*/

	#ifdef va_copy
		va_end(ap);
	#endif

	}

	return self;
}

void * Stack_dtor(void * _self)
{
	struct List * self = cast(Stack, _self);
	if(self == NULL) {return NULL; }  // failed return path

	// returns pointer to self on successful delete
	// NULL return indicates failure
	//if (clearListObjectValues(self) != _self) {return NULL; }  // failed return path

	// destroy the superclass components
	return super_dtor(Stack, _self);
}

/*****************************/
/****** Super Methods  *******/
/*
void * super_add_cse(const void * _class, const void * _self, const void * _element)
{
	// TODO: verify with testing
	//if ( !cast_cs(_class, _self) )    { return NULL; } // failed return path
	//if (classOf(_self) == _class)     { return NULL; } // failed return path
	if ( !isOfSuper_sc(_self, _class) ) { return NULL; } // failed return path
	printf("\nsuper_add: _self is in the superclass chain of _class\n");
	const struct ListClass * superclass = super(classOf(_self));
	printf("super_add: superclass is assigned\n");
	printf("super_add: superclass->add: %p", superclass->add);
	if ( superclass->add ) {
		return superclass->add(_self, _element);	// expected return path
	}
	return NULL;  									// failed return path
}

void * super_take(const void * _class, const void * _self)
{
	// TODO: verify with testing
	if ( !cast_cs(_class, _self) )   { return NULL; } // failed return path
	if (classOf(_self) == _class) { return NULL; }    // failed return path
	const struct ListClass * superclass = super(classOf(_self));
	if ( superclass->take ) {
		return superclass->take(_self);  // expected return path
	}
	return NULL;  // failed return path
}
*/
/*
void * super_addFirst(const void * _class, const void * _self, const void * _element)
{
	// TODO: verify with testing
	if ( !cast_cs(_class, _self) )   { return NULL; } // failed return path
	if (classOf(_self) == _class) { return NULL; } // failed return path
	const struct ListClass * superclass = super(classOf(_self));
	if ( superclass->addFirst ) {
		return superclass->addFirst(_self, _element);  // expected return path
	}
	return NULL;  // failed return path
}

void * super_addLast(const void * _class, const void * _self, const void * _element)
{
	// TODO: verify with testing
	if ( !cast_cs(_class, _self) )   { return NULL; } // failed return path
	if (classOf(_self) == _class) { return NULL; } // failed return path
	const struct ListClass * superclass = super(classOf(_self));
	if ( superclass->addLast ) {
		return superclass->addLast(_self, _element);  // expected return path
	}
	return NULL;  // failed return path
}

void * super_takeFirst(const void * _class, const void * _self)
{
	// TODO: verify with testing
	if ( !cast_cs(_class, _self) )   { return NULL; } // failed return path
	if (classOf(_self) == _class) { return NULL; } // failed return path
	const struct ListClass * superclass = super(classOf(_self));
	if ( superclass->takeFirst ) {
		return superclass->takeFirst(_self);  // expected return path
	}
	return NULL;  // failed return path
}

void * super_takeLast(const void * _class, const void * _self)
{
	// TODO: verify with testing
	if ( !cast_cs(_class, _self) )   { return NULL; } // failed return path
	if (classOf(_self) == _class) { return NULL; } // failed return path
	const struct ListClass * superclass = super(classOf(_self));
	if ( superclass->takeLast ) {
		return superclass->takeLast(_self);  // expected return path
	}
	return NULL;  // failed return path;
}
*/

/*****************************/
/****** List Methods  *******/
/*
void * List_add(const void * _self, const void * _element)
{
	// operates as a simple FIFO list using addLast
	struct List * self = cast_cs(List, _self);
	if (self != NULL) {
		return List_addLast(_self, _element);	// expected return path
	}
	return NULL;  								// failed return path;
}

void * List_take(const void * _self)
{
	// operates as a simple FIFO list using takeFirst
	struct List * self = cast_cs(List, _self);
	if (self != NULL) {
		return List_takeFirst(_self);		// expected return path
	}
	return NULL;  							// failed return path;
}

void * List_addFirst(const void * _self, const void * _element)
{
	struct List * self = cast_cs(List, _self);
	if (self == NULL)  { return 0; }  // failed return path;

	//  increment item count and extend buffer if needed  //
	self->countOfItems++;
	adjustBufferSizeIfNeeded(self);

	// adjust beginning index //
	self->beginIndex--;
	if (self->beginIndex < 0) {
		self->beginIndex = beginIndex(self) + arrayDimension(self);
	}

	// add the new element value  //
	self->buf[beginIndex(self)] = _element;
	return (void *)_element;  // expected return path
}

void * List_addLast(const void * _self, const void * _element)
{
	struct List * self = cast_cs(List, _self);
	if( self == NULL )  { return 0; }  // failed return path;


	//  increment item count and extend buffer if needed  //
	self->countOfItems++;
	adjustBufferSizeIfNeeded(self);

	// add the new element value  //
	// endIndex is already pointing at available entry point  //
	self->buf[endIndex(self)] = _element;

	// adjust ending index for next addition //
	self->endIndex++;
	if(self->endIndex >= arrayDimension(self)) {
		self->endIndex = endIndex(self) - arrayDimension(self);
	}

	return (void *)_element;  // expected return path
}

void * List_takeFirst(const void * _self)
{
	void * element;
	struct List * self = cast_cs(List, _self);
	if ( self == NULL )            { return NULL; }  // failed return path;
	if ( countOfItems(self) <= 0 ) { return NULL; }  // failed return path;

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
	struct List * self = cast_cs(List, _self);
	if ( self == NULL )            { return NULL; }  // failed return path;
	if ( countOfItems(self) <= 0 ) { return NULL; }  // failed return path;

	// Recalculate the endIndex and then take element at new endIndex
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

int List_equal(const void * _self, const void * _comparisonList)
{   // return equal_TRUE if objects are equal, else return equal_FALSE
	// TODO: WARNING: convert to contains structure as this is too rigid
	enum { equal_FALSE = 0, equal_TRUE = 1 };
	int i;
	struct List * self             = cast_cs(List, _self);
	struct List * comparisonList = cast_cs(List, _comparisonList);
	if (self == NULL )             { return equal_FALSE; } //failed return
	if (comparisonList == NULL ) { return equal_FALSE; } //failed return

	// failed return on count mismatch
	if (arrayDimension(_self) != arrayDimension(_comparisonList))
		{ return equal_FALSE; } //failed return

	// iterate through the list entries, to check if the "contents" are equal
	for( i = 0; i < arrayDimension(_self); i++) {
		// TODO: not knowing the type of object, it may be hard to compare the elements
		// TODO: add a pointer type to the list structure, and then use a switch construct
		if ( (*((int *)(self->buf[i]))) != (*((int *)(comparisonList->buf[i]))) )  {
			return equal_FALSE; } //failed return
	}

	return equal_TRUE;  // execution progresses at this point means lists are equal
}

int List_contains(const void * _self, const void * _element)
{
	// return equal_TRUE if objects are equal, else return equal_FALSE
	enum { equal_FALSE = 0, equal_TRUE = 1 };
	int i;
	void * listItem;
	struct List * self = cast_cs(List, _self);
	void * element = (void *)_element;

	if (self == NULL )    { return equal_FALSE; } //failed return
	// TODO: should a NULL element be excluded?  Likely no
	// if (element == NULL ) { return equal_FALSE; } //failed return

	// iterate through the list entries, checking for a element match
	for( i = 0; i < countOfItems(_self); i++) {
		// Get an item from list and return it non-destructively
		listItem = List_addLast(_self, List_takeFirst(_self));
		if ( listItem == element)  { return equal_TRUE; } // element found
	}
	return equal_FALSE;  // execution here means element is NOT contained in list
}
*/
/*****************************/
/****** Public Methods  ******/

/*
void * add(const void * _self, const void * _element)
{

	const struct ListClass * class = classOf( cast_cs(List, _self) );

	// non-valid pointers destablize the windows environment, but may work in embedded
	if (!isClass(class)) { return NULL; } // failed return path

	if (class == NULL)          {return NULL;} // failed return path
	if (class->add == NULL) {return NULL;} // failed return path

	return class->add(_self, _element);
}

void * take(const void * _self)
{
	const struct ListClass * class = classOf( cast_cs(List, _self) );

	// non-valid pointers destablize the windows environment, but may work in embedded
	if (!isClass(class)) { return NULL; } // failed return path

	if (class == NULL)            {return NULL;} // failed return path
	if (class->take == NULL) {return NULL;} // failed return path

	return class->take(_self);
}
*/
/*
void * addFirst(const void * _self, const void * element)
{
	// TODO:  What is this routine doing?  Why is not not captured in test?
	return NULL;
}

void * addLast(const void * _self, const void * element)
{
	// TODO:  What is this routine doing?  Why is not not captured in test?
	return NULL;
}

void * takeFirst(const void * _self)
{
	const struct ListClass * class = classOf( cast_cs(List, _self) );
	// TODO:  What is this routine doing?  Why is not not captured in test?
	// note the print statement below
	if( class && class->takeFirst ) {
		printf("\nNon-printable object trapped in draw. \n");
		return NULL;
	}
	class->takeFirst(_self);
	return NULL;
}

void * takeLast(const void * _self)
{
	// TODO:  What is this routine doing?  Why is not not captured in test?
	return NULL;
}
*/

/*
static void adjustBufferSizeIfNeeded(const void * _self)
{
	struct List * self = cast_cs(List, _self);
	if (self == NULL)              { return; }  // failed return path
	if (countOfItems(_self) <= 0 ) { return; }	// no extension needed
	if (self->buf == NULL)         { return; } 	// failed return path

	// Extend array if next addition would fill array
	// TODO:  is it true that the next addition would fill the array,
	// or just overflow the array?

	// WARNING:  The "- 1" in the else if is essential to avoid chattering
	if (countOfItems(self) >= arrayDimension(self)) { 	// extension needed
		extendBuffer(_self);
		return;
	}
	else if (countOfItems(self) < (( arrayDimension(self) / 2 ) - 1 )) {  // Warning above
		reduceBuffer(_self);
		return;
	}
	return;
}

static void reduceBuffer(const void * _self)
{
	struct List * self = cast_cs(List, _self);
	if (self == NULL)              { return; } 	// failed return path
	if (arrayDimension(self) <= 4) { return; } 	// failed return path
	if (self->buf == NULL)         { return; } 	// failed return path

	void * oldArrayPointer = self->buf;

	// calculate the reduced array element count
	int newDimension = ( arrayDimension(self) / 2 );

	// exit without reduction if existing element count will not fit
	if (countOfItems(self) >= newDimension) { return; } // failed return path

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
	if (countOfItems(self) > countFromBeginIndexToArrayEnd) {
		// array is wrapped ... adjust copy indexes for two step copy
		memoryCopyCount1 = countFromBeginIndexToArrayEnd * sizeof(* self->buf);
		memoryCopyCount2 = (countOfItems(self) - countFromBeginIndexToArrayEnd)
																* sizeof(*self->buf);
	}
	else {
		// array is not wrapped ... adjust copy indexes for single copy
		memoryCopyCount1 = countOfItems(self) * sizeof(* self->buf);
		memoryCopyCount2 = 0;
	}

	// first copy is valid for all array configurations
	memcpy(copyTo, copyFrom, memoryCopyCount1 );

	// second copy is valid for any wrapped array data configurations
	if(memoryCopyCount2) {			// true if array is wrapped
		copyTo = newArrayPointer + countFromBeginIndexToArrayEnd * sizeof(* self->buf);
		copyFrom = oldArrayPointer;
		memcpy(copyTo, copyFrom, memoryCopyCount2 );
	}

	// set the new beginIndex and endIndex values, which are always zero aligned
	int newBeginIndex = 0;
	int newEndIndex  = countOfItems(self);

	// copy is complete; free just the original buffer only
	free (self->buf);

	// Update self with new data components
	self->arrayDimension = newDimension;
	self->buf =newArrayPointer;
	self->beginIndex = newBeginIndex;
	self->endIndex = newEndIndex;

	return;	// expected return path
}

static void extendBuffer(const void * _self)
{
	struct List * self = cast_cs(List, _self);
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
		assert("Lists:extendBuffer failed to allocate memory for buffer extension");
	}

	// set new array memory area to zero
	memset(newArrayPointer, 0, newMemoryCount);

	// copy from beginIndex to endIndex (non-wrapped) or arrayDim (wrapped) to upper block
	void * copyTo = newArrayPointer + (arrayDimension(self) + beginIndex(self)) * sizeof(*self->buf);
	void * copyFrom = oldArrayPointer + beginIndex(self)*sizeof(*self->buf);
	int copyCount = (arrayDimension(self) - beginIndex(self))*sizeof(*self->buf);
	memcpy(copyTo, copyFrom, copyCount );

	// Set the new beginIndex and tentatively set the endIndex
	int newEndIndex  = arrayDimension(self) + endIndex(self);
	int newBeginIndex = arrayDimension(self) + beginIndex(self);

	// copy from index zero to endIndex into lower block for wrapped arrays
	if(beginIndex(self) > endIndex(self)) {			// true if array is wrapped
		copyTo = newArrayPointer;
		copyFrom = oldArrayPointer;
		copyCount = endIndex(self) *sizeof(void *);
		memcpy(copyTo, copyFrom, copyCount );

		// Update endIndex (beginIndex is already correctly set)
		newEndIndex = endIndex(self);
	}

	// copy is complete; free just the original buffer only
	free (self->buf);

	// Update self with new data components
	self->arrayDimension = newDimension;
	self->buf =newArrayPointer;
	self->endIndex = newEndIndex;
	self->beginIndex = newBeginIndex;

	return; 											// expected return path
}

void * setArrayValuesToNull(void * _self)
{
	struct List * self = _self;
	if (self == NULL)              { return NULL; } 	// failed return path
	if (self->buf == NULL)         { return NULL; } 	// failed return path
	if (arrayDimension(self) <= 0) { return NULL; } 	// failed return path

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

	setArrayValuesToNull(self);
	self->buf            = NULL;
	self->arrayDimension = 0;
	self->countOfItems   = 0;
	self->beginIndex     = 0;
	self->endIndex       = 0;
	return _self;
}
*/
void clear_Stack_and_StackClass(void)
{
	void * blockAddress;

	if(StackClass) {
		blockAddress = memset((void * )StackClass, 0, sizeOf(StackClass));
		if (blockAddress != StackClass) {
			printf("clear_Stack_and_StackClass: failed StackClass clear");
		}
		free((struct Class *)StackClass);
		StackClass = NULL;
	}
	if(Stack) {
		blockAddress = memset((void *)Stack, 0, sizeOf(Stack));
		if (blockAddress != Stack) {
			printf("clear_Stack_and_StackClass: failed Stack clear");
		}
		free((struct Class *)Stack);
		Stack = NULL;
	}

	if(ListClass) {
		blockAddress = memset((void * )ListClass, 0, sizeOf(ListClass));
		if (blockAddress != ListClass) {
			printf("clear_Stack_and_StackClass: failed ListClass clear");
		}
		free((struct Class *)ListClass);
		ListClass = NULL;
	}
	if(List) {
		blockAddress = memset((void *)List, 0, sizeOf(List));
		if (blockAddress != List) {
			printf("clear_Stack_and_StackClass: failed List clear");
		}
		free((struct Class *)List);
		List = NULL;
	}

	if ( classRegistry != NULL )
		{ classRegistry = safeDelete((void *)classRegistry); }

}

