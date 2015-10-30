/*
 * lists.h
 *
 *  Created on: Mar 13, 2015
 *      Author: Alan
 */

#ifndef SRC_LISTS_LISTS_H_
#define SRC_LISTS_LISTS_H_

/***********************************************/
/****** superclass and system includes  ********/

#include "..\..\src\objects\objects.h"
//#include "..\cross_compiler_defs.h"


/***********************************************/
/**** forward declarations and typedefs  *******/

extern const void * List; 		// new(List, void * []);
extern const void * ListClass;

typedef enum containts_t { DOES_NOT_CONTAIN = 0,
						   DOES_CONTAIN
} containts_t;

/***********************************************/
/************ protected includes  **************/

#include "lists-private.h"


/***********************************************/
/****** application interface functions  *******/

// MUST be called before any list is created
void List_init(void);

 // FIFO: First in, first out
void *  add(const void * _self, const void * _element);
void * take(const void * _self);

containts_t contains(const void * _self, const void * _element);
int		getItemCount(const void * _self);

// returns pointer to first item in the list, and resets currentIndex
// non-destructive:  the list contents are unaffected
void * resetIndex(const void * _self);

// returns pointer to item at currentIndex, and increments currentIndex
// non-destructive:  the list contents are unaffected
void * incrementGet (const void * _self);



#endif /* SRC_LISTS_LISTS_H_ */
