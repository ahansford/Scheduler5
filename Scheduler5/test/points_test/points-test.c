/*
 * objects-test.c
 *
 *  Created on: Feb 24, 2015
 *      Author: Alan
 */


#include "..\..\src\cross_compiler_defs.h"
#include "..\unity\unity_fixture.h"
//#include "..\unity\unity.h"
#include "..\..\src\mocks\RuntimeErrorStub.h"

#include "..\..\src\points\points.h"
#include "..\..\src\points\points-private.h"

/****************************************/
/*** Basic Object and Class Testing ****/
struct Point * myTestPoint;
const struct PointClass * myPointClass;
const struct Point *      myPoint;


/****************************************/
/*********** Start of Testing ***********/
TEST_GROUP(points);

TEST_SETUP(points)
{
	RuntimeErrorStub_Reset();
	initPoint();
	myTestPoint = new(Point, 3, 5);
	myPointClass = classOf(myTestPoint);
	myPoint = (void *)Point;
}

TEST_TEAR_DOWN(points)
{
	RuntimeErrorStub_Reset();
}
/*
TEST(points, TestPointReports35AfterNew)
{
	TEST_ASSERT_EQUAL(3, x(myTestPoint));
	TEST_ASSERT_EQUAL(5, y(myTestPoint));
}

TEST(points, NewReturnsNonNullPointer)
{
	TEST_ASSERT_NOT_EQUAL(NULL, myTestPoint);
}

TEST(points, MoveOriginalPoint35_to_79)
{
	move(myTestPoint, 4, 4);
	TEST_ASSERT_EQUAL(3+4, x(myTestPoint));
	TEST_ASSERT_EQUAL(5+4, y(myTestPoint));
}

TEST(points, draw_testPointReports35)
{
	draw(myTestPoint);
	Point_draw(myTestPoint);
	TEST_ASSERT_EQUAL(0, 0);
}

TEST(points, draw_IntPtrReportsNonPrintable)
{
	int x = 5;
	int * intPtr = &x;
	draw(intPtr);
	Point_draw(intPtr);
	TEST_ASSERT_EQUAL(0, 0);
}

TEST(points, draw_NullReportsNonPrintable)
{
	TEST_ASSERT_EQUAL(NULL, classOf(NULL));
	draw(NULL);
	Point_draw(NULL);
	TEST_ASSERT_EQUAL(0, 0);
}
*/
// *** Testing NEW method pointers **************************************//
// *** const struct XXXXClass * myXXXXClass *****************************//
// *** use (XXXX_method, myXXXXClass->method ) = classOf(myTestXXXX); ***//
/*
TEST(points, init_drawReportsPoint_draw)
{
	TEST_ASSERT_EQUAL_PTR(Point_draw, myPointClass->draw);
}

TEST(points, init_PointReportsPoint_ctor)
{
	TEST_ASSERT_EQUAL_PTR(Point_ctor, myPointClass->_.ctor  );
}
*/
// *** Testing NEW class pointers ****************************************//
// *** const struct XXXX * myXXXX = (void *)XXXX;  ***********************//
// *** use (XXXXClass_method, myXXXX->_.class->method ); *****************//

/*
TEST(points, init_PointClassReportsPointClass_ctor)
{
	TEST_ASSERT_EQUAL_PTR(PointClass_ctor, myPoint->_.class->ctor );
}
*/
