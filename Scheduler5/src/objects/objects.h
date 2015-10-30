/*
 * objects.h
 *
 *  Created on: Feb 24, 2015
 *      Author: Alan
 */

#ifndef OBJECTS_H_
#define OBJECTS_H_

/***********************************************/
/****** superclass and system includes  ********/

#include <stdarg.h>
#include <stdio.h>
//#include "..\cross_compiler_defs.h"


/***********************************************/
/**** forward declarations and typedefs  *******/

extern const void * Object;
extern const void * Class;

typedef enum equal_t { OBJECT_UNEQUAL = 0,
	                   OBJECT_EQUAL  = 1
} equal_t;

typedef enum puto_return_t { PUTO_ERROR = -1,
	                              PUTO_SUCCESS
} puto_return_t;

/***********************************************/
/************ protected includes  **************/

#include "objects-private.h"


/***********************************************/
/****** application interface functions  *******/

// MUST be called before any list is created
void   object_init(void);

void * new   (const void * _class, ...);

// WARNING:  The object's pointer must be set to NULL as part of delete()
// myObjectPtr = ((myObject == delete(myObjectPtr)) ? NULL : myObjectPtr );
// myObjectPtr = safeDelete(myObjectPtr);
void * delete(      void * _self);
#define safeDelete(p) (( (p) == delete(p) ) ? NULL : (p))

void *        ctor  (      void * _self, va_list * app);
void *        dtor  (      void * _self);
void *        copy  (      void * _self, const void * _copyFromObject);
equal_t       equal (const void * _self, const void * _comparisonObject);
// config() takes no action at the Object of Class level
// config() is nominally expected to copy() the _master to _self
// complex objects may overload with more appropriate algorithms
void *        config(const void * _self, const void * _master);
puto_return_t puto  (const void * _self, FILE * fp);


const void * classOf(const void * _self);
int          sizeOf (const void * _self);
const void * super   (const void * self); // class' superclass


#endif /* OBJECTS_H_ */
