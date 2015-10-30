/*
 * lists-private.h
 *
 *  Created on: Mar 13, 2015
 *      Author: Alan
 */

#ifndef LISTS_PRIVATE_H_
#define LISTS_PRIVATE_H_

#include "..\..\src\objects\objects.h"

extern const void * ListClass;  //
//extern enum containts_t;

/***********************************/
/*** class and object structures ***/


struct List {
	const struct Object _;	    // the base Object at the start of List
	const void **		buf;	// ptr to array of void pointers
	int                 arrayDimension;
	int                 countOfItems;
	int                 beginIndex;
	int                 endIndex;
	int					currentIndex;
	int					mutexTag;
};

struct ListClass {
	const struct Class _; // should be superclass: Class or "somethingClass"
	void *      (* add)     (const void * _self, const void * element);
	void *      (* take)    (const void * _self);
	int         (* contains)(const void * _self, const void * element);
	int         (* getItemCount)(const void * _self);
	void *      (* resetIndex)(const void * _self);
	void *      (* getIncrement) (const void * _self);
};


/*****************************/
/****** Constructors  ********/

void * ListClass_ctor(void * _self, va_list * app);
void * List_ctor     (void * _self, va_list * app);

/***********************************/
/****** Overload Functions  ********/
void *        List_dtor (      void * _self);
void * List_copy( void * _self, const void * _copyFromObject);
equal_t       List_equal(const void * _self, const void * _comparisonList);
puto_return_t List_puto (const void * _class, FILE * _fp);


/**********************************/
/**********  new methods  *********/

void * super_add(const void * _class,
		         const void * _self, const void * _element);
void *  List_add(const void * _self, const void * _element);

void * super_take(const void * _class, const void * _self);
void *  List_take(                     const void * _self);

containts_t super_contains(const void * _class,
		                   const void * _self, const void * _element);
containts_t  List_contains(const void * _self, const void * _element);

int super_getItemCount(const void * _class, const void * _self);
int  List_getItemCount(                     const void * _self);

void * super_resetIndex(const void * _class, const void * _self);
void *  List_resetIndex(                     const void * _self);

void * super_getIncrement(const void * _class, const void * _self);
void *  List_getIncrement(                     const void * _self);

// direct access methods to implement other list add/take approaches
void * List_addFirst (const void * _self, const void * _element);
void * List_addLast  (const void * _self, const void * _element);
void * List_takeFirst(const void * _self);
void * List_takeLast (const void * _self);

// support functions for testing
void * implement_List_setArrayValuesToNull    (void * _self);
void * clearListObjectValues   (void * _self);

// stores registry of classes to provide positive ID of know classes
void * registryArray[8];
const struct List * classRegistry;
int isClass(const void * _class);


// accessor functions into List object data members
#define bufPointer(p) 	  ((p) ? ((const struct List *) (p))->buf : NULL)
#define arrayDimension(p) ((p) ? ((const struct List *) (p))->arrayDimension : 0)
// countOfObjects() method is included in the class structure
#define beginIndex(p)     ((p) ? ((const struct List *) (p))->beginIndex : 0)
#define endIndex(p)       ((p) ? ((const struct List *) (p))->endIndex : 0)
#define currentIndex(p)   ((p) ? ((const struct List *) (p))->currentIndex : 0)
#define mutexTag(p)   	  ((p) ? ((const struct List *) (p))->mutexTag : 0)

#endif /* LISTS_PRIVATE_H_ */
