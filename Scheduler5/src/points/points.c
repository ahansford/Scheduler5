/*
 * points.c
 *
 *  Created on: Mar 10, 2015
 *      Author: Alan
 */

#include "points.h"
#include "points-private.h"
//#include "..\..\src\objects\objects.h"
//#include <stdlib.h>
//#include <stddef.h>
//#include <string.h>
#include <assert.h>



/*****************************/
/**** INITIALIZATIONS  *******/

const void * PointClass;
const void * Point;

void initPoint(void)
/* MUST be called before any point is created */
{
	//if (Object) {object_init();}
	//if (!Class)  {object_init();}
	if (! PointClass) {
		PointClass = new(Class,
						 "PointClass",
						 Class,
						 sizeof(struct PointClass),
						 ctor, PointClass_ctor,
						 0);
	}
	if (! Point) {
		Point = new(PointClass,
				    "Point",
				    Object,
				    sizeof(struct Point),
				    ctor, Point_ctor,
				    draw, Point_draw,
				    0);
	}
	return;
}

/*****************************/
/****** Constructors  ********/

void * Point_ctor(void * _self, va_list * app)
{
	struct Point * self = super_ctor(Point, _self, app);
	self->x = va_arg(* app, int);
	self->y = va_arg(* app, int);
	return self;
}

void * PointClass_ctor(void * _self, va_list *app)
{
	/* Add superclass class data and methods */
	struct PointClass * self = super_ctor(PointClass, _self, app);

	/* Initialize new function pointers to NULL */
	typedef void (* voidf)();
	* (voidf *) & self->draw = NULL;
	// move is not part of struct PointClass


	/* Update any overloaded methods */
	/* using style ... , methodSelector, overloadedFunctionPtr, */
	voidf selector;

	#ifdef va_copy
		va_list ap;
		va_copy(ap, * app);
	#else
		va_list ap = * app;
	#endif

	while ( (selector = va_arg(ap, voidf)) ) {            /* if non-NULL selector then,   */
		voidf overloadedFunctionPtr  = va_arg(ap, voidf); /* grab the next method pointer */

		if (selector == (voidf) draw)
			{* (voidf *) & self->draw = overloadedFunctionPtr;}

	#ifdef va_copy
		va_end(ap);
	#endif

	}

	return self;
}

/*****************************/
/****** Draw Methods  *******/

void draw (const void * _self)
{
	const struct PointClass * class = classOf( cast(Point, _self) );
	// TODO: Text and logic look defective.  Neither captures null ptrs
	if( class && class->draw ) {
		printf("\nNon-printable object trapped in draw. \n");
		return;
	}
	class->draw(_self);
	return;
}

void super_draw(const void * _class, const void * _self)
{
	const struct PointClass * superclass = super(_class);
	if ( _self && superclass->draw ) {
		superclass->draw(_self);
	}
	return;
}

void Point_draw (const void * _self)
{
	//const struct Point * self = _self;
	const struct Point * self = cast(Point, _self);
	if(self) {
		printf("\n\".\" at %d,%d\n", self -> x, self -> y);
	}
	else {
		printf("\nNon-printable object trapped in Point_draw. \n");
	}
}

/*****************************/
/****** Move Methods  *******/

void move (void * _self, int dx, int dy)
{
	struct Point * self = _self;
	assert(self);
	self->x += (dx);
	self->y += (dy);
	return;
}

