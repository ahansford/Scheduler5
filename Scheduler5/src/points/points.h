/*
 * points.h
 *
 *  Created on: Mar 10, 2015
 *      Author: Alan
 */

#ifndef POINTS_H_
#define POINTS_H_


//#include "..\cross_compiler_defs.h"
//#include <stdarg.h>
//#include <stdio.h>

//#include "points-private.h"
#include "..\..\src\objects\objects.h"

extern const void * Point; 		/* new(Point, x, y); */
extern const void * PointClass; /* adds draw to the base class */

void draw (const void * _self);
void move (void * _self, int dx, int dy);

/* MUST be called before any point is created */
void initPoint(void);

#endif /* POINTS_H_ */
