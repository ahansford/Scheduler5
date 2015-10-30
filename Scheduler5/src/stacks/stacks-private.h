/*
 * lists-private.h
 *
 *  Created on: Mar 13, 2015
 *      Author: Alan
 */

#ifndef STACKS_PRIVATE_H_
#define STACKS_PRIVATE_H_

//#include "..\..\src\lists\lists.h"

extern const void * StackClass;

struct Stack {
	const struct List   _;	    /* the base Object at the start of Point */
	const void **		buf;	/* ptr to array of void pointers */
	int                 arrayDimension;
	int                 countOfItems;
	int                 beginIndex;
	int                 endIndex;
};

void * Stack_ctor(void * _self, va_list * app);
void * Stack_dtor(void * _self);

/*
void * registryArray[8];
const struct List * classRegistry;


#define bufPointer(p) 	  ((p) ? ((const struct List *) (p)) -> buf : NULL)
#define arrayDimension(p) ((p) ? ((const struct List *) (p)) -> arrayDimension : 0)
#define countOfItems(p)   ((p) ? ((const struct List *) (p)) -> countOfItems : 0)
#define beginIndex(p)     ((p) ? ((const struct List *) (p)) -> beginIndex : 0)
#define endIndex(p)       ((p) ? ((const struct List *) (p)) -> endIndex : 0)

int isClass(const void * _class);
*/
void Stack_registerKeyClasses(void);
/*
void * super_add_cse(const void * _class, const void * _self, const void * _element);
void * super_take(const void * _class, const void * _self);

void * super_addFirst(const void * _class, const void * _self,
												const void * _element);
void * super_addLast(const void * _class, const void * _self,
												const void * _element);
void * super_takeFirst(const void * _class, const void * _self);
void * super_takeLast(const void * _class, const void * _self);
*/

/*
void * List_add(const void * _self, const void * _element);
void * List_take(const void * _self);
void * List_addFirst(const void * _self, const void * _element);
void * List_addLast(const void * _self, const void * _element);
void * List_takeFirst(const void * _self);
void * List_takeLast(const void * _self);
int    List_contains(const void * _self, const void * _element);
*/
/*
void * addFirst(const void * _self, const void * _element);
void * addLast(const void * _self, const void * _element);
void * takeFirst(const void * _self);
void * takeLast(const void * _self);

void * setArrayValuesToNull(void * _self);
void * clearListObjectValues(void * _self);
*/

void   clear_Stack_and_StackClass(void);

struct StackClass {
	const struct ListClass _;			/* PointClass : Class */
	void * (* add)(const void * _self, const void * element);
	void * (* take)(const void * _self);
	//void * (* addFirst)(const void * _self, const void * element);
	//void * (* addLast)(const void * _self, const void * element);
	//void * (* takeFirst)(const void * _self);
	//void * (* takeLast)(const void * _self);
};

void * StackClass_ctor(void * _self, va_list *app);



#endif /* STACKS_PRIVATE_H_ */
