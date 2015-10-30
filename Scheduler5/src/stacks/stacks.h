/*
 * lists.h
 *
 *  Created on: Mar 13, 2015
 *      Author: Alan
 */

#ifndef STACKS_H_
#define STACKS_H_

#include <stdarg.h>
#include <stdio.h>
#include "..\..\src\lists\lists.h"
#include "stacks-private.h"
//#include "..\..\src\stacks\stack-private.h"

extern const void * Stack; 		// new(Stack, void * [], size);
//extern const void * StackClass; //


/* MUST be called before any list is created */
void Stack_init(void);

/*
void * add(const void * _self, const void * _element); // FIFO
void * take(const void * _self);                       // FIFO
*/



#endif /* STACKS_H_ */
