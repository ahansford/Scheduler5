/*
 * RuntimeErrorStubs.c
 *
 *  Created on: Jul 11, 2013
 *      Author: alhansfo
 */


//#include <stdint.h>
#include "..\mocks\RuntimeErrorStub.h"
#include "..\..\test\unity\unity_fixture.h"

static const char * message = "No Error";
static int parameter = -1;
static const char * file = 0;
static int line = -1;

void RuntimeErrorStub_Reset(void)
{
	message = "No Error";
	parameter = -1;
}

const char * RuntimeErrorStub_GetLastError(void)
{
	return message;
}

void RuntimeError(const char * m, int p, const char * f, int l)
{
	message = m;
	parameter = p;
	file = f;
	line = l;
}

int RuntimeErrorStub_GetLastParameter(void)
{
	return parameter;
}

void RuntimeErrorStub_CheckLastError(void)
{
	TEST_ASSERT_EQUAL_STRING("No Error", RuntimeErrorStub_GetLastError());
	TEST_ASSERT_EQUAL(-1, RuntimeErrorStub_GetLastParameter());
	RuntimeErrorStub_Reset();
	return;
}

void RuntimeErrorStub_ConfirmLastError(const char * message)
{
	TEST_ASSERT_EQUAL_STRING(message, RuntimeErrorStub_GetLastError());
	RuntimeErrorStub_Reset();
	return;
}
