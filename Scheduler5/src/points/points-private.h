/*
 * points-private.h
 *
 *  Created on: Mar 10, 2015
 *      Author: Alan
 */

#ifndef POINTS_PRIVATE_H_
#define POINTS_PRIVATE_H_

//#include "..\..\src\objects\objects.h"

struct Point {
	const struct Object _;	/* the base Object at the start of Point */
	int                 x;
	int                 y;
};

#define x(p) (((const struct Point *) (p)) -> x)
#define y(p) (((const struct Point *) (p)) -> y)

void super_draw(const void * _class, const void * _self);

void * Point_ctor(void * _self, va_list * app);
void Point_draw (const void * _self);

struct PointClass {
	const struct Class _;			/* PointClass : Class */
	void (* draw) (const void * self);
};

void * PointClass_ctor(void * _self, va_list *app);


#endif /* POINTS_PRIVATE_H_ */
