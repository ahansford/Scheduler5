/*
 * objects-test-runner.c
 *
 *  Created on: Feb 24, 2015
 *      Author: Alan
 */


#include "..\unity\unity_fixture.h"
#include "..\unity\unity.h"

TEST_GROUP_RUNNER(points)
{
	//RUN_TEST_CASE(points, TestPointReports35AfterNew);
	//RUN_TEST_CASE(points, NewReturnsNonNullPointer);
	//RUN_TEST_CASE(points, MoveOriginalPoint35_to_79);

	//RUN_TEST_CASE(points, draw_testPointReports35);
	//RUN_TEST_CASE(points, draw_NullReportsNonPrintable);
	// *** Next test fails due to unbounded ptr search for class or object class descriptor ***
	//RUN_TEST_CASE(points, draw_IntPtrReportsNonPrintable);


	//RUN_TEST_CASE(points, init_drawReportsPoint_draw);
	//RUN_TEST_CASE(points, init_PointReportsPoint_ctor);
	//RUN_TEST_CASE(points, init_PointClassReportsPointClass_ctor);
}

