/*
 * stacks-test.c
 *
 *  Created on: Apr 25, 2015
 *      Author: alhansfo
 */

#include <string.h>
#include "..\..\src\cross_compiler_defs.h"
#include "..\unity\unity_fixture.h"
//#include "..\unity\unity.h"
#include "..\..\src\mocks\RuntimeErrorStub.h"

#include "..\..\src\stacks\stacks.h"
#include "..\..\src\stacks\stacks-private.h"
#include "..\..\src\lists\lists.h"
//#include "..\..\src\lists\lists-private.h"

/****************************************/
/*** Basic Object and Class Testing ****/
struct List *       myTestStack  = NULL;

const struct List * myStack = NULL;
struct Class *      Stack_as_Class = NULL;

const struct ListClass * myStackClass = NULL;
struct Object *          StackClass_as_Object = NULL;

#ifndef MIN_LIST_BUFFER_SIZE
#define MIN_LIST_BUFFER_SIZE 4
#endif

void * voidStackArray[MIN_LIST_BUFFER_SIZE];


typedef void (* voidf)(); 	/* generic function pointer */

/*
void Print_FunctionPointers(void);
void printVoidArray(void * _array);
void printListDetails(const void * _self);

int xList        = 5;
int * intPtrList = &xList;
int i            = 0;
*/
/****************************************/
/*********** Start of Testing ***********/

TEST_GROUP(stacks);

TEST_SETUP(stacks)
{
	RuntimeErrorStub_Reset();
	Stack_init();

	//myTestStack = new(Stack, voidStackArray, MIN_LIST_BUFFER_SIZE);
	myTestStack = new(Stack, voidStackArray);

	//myStack         = (void *)Stack;
	//Stack_as_Class  = (void *)Stack;

	//myStackClass    = (void *)StackClass;

}

TEST_TEAR_DOWN(stacks)
{
	//delete(myTestStack);
	myTestStack = safeDelete(myTestStack);
	RuntimeErrorStub_Reset();
}


/*************************************/
/*********  Setup List ***************/

TEST(stacks, Create)
{
	TEST_ASSERT_NOT_EQUAL(NULL, Stack);
	TEST_ASSERT_NOT_EQUAL(NULL, StackClass);
	TEST_ASSERT_NOT_EQUAL(NULL, myTestStack);
}

TEST(stacks, StackClass_is_registered_after_Stack_init)
{
	TEST_ASSERT_NOT_EQUAL(NULL, classRegistry);
	Stack_init();
	TEST_ASSERT_EQUAL(TRUE, List_contains(classRegistry, StackClass));
}

TEST(stacks, Stack_is_registered_after_Stack_init)
{
	TEST_ASSERT_NOT_EQUAL(NULL, classRegistry);
	Stack_init();
	TEST_ASSERT_EQUAL(TRUE, List_contains(classRegistry, Stack));
}

TEST(stacks, Add_returns_self_pointer)
{
	TEST_ASSERT_EQUAL_PTR((void *)1, add(myTestStack, (void *)1));
}

TEST(stacks, take_returns_added_pointer)
{
	add(myTestStack, (void *)5);
	TEST_ASSERT_EQUAL_PTR((void *)5, take(myTestStack));
}

TEST(stacks, Can_add_four_items)
{
	TEST_ASSERT_EQUAL_PTR((void *)1, add(myTestStack, (void *)1));
	TEST_ASSERT_EQUAL_PTR((void *)2, add(myTestStack, (void *)2));
	TEST_ASSERT_EQUAL_PTR((void *)3, add(myTestStack, (void *)3));
	TEST_ASSERT_EQUAL_PTR((void *)4, add(myTestStack, (void *)4));
}

TEST(stacks, Can_take_four_items_in_LIFO_order)
{
	add(myTestStack, (void *)1);
	add(myTestStack, (void *)2);
	add(myTestStack, (void *)3);
	add(myTestStack, (void *)4);

	TEST_ASSERT_EQUAL_PTR((void *)4, take(myTestStack));
	TEST_ASSERT_EQUAL_PTR((void *)3, take(myTestStack));
	TEST_ASSERT_EQUAL_PTR((void *)2, take(myTestStack));
	TEST_ASSERT_EQUAL_PTR((void *)1, take(myTestStack));
}

TEST(stacks, Taking_item_with_count_zero_returns_NULL)
{
	TEST_ASSERT_EQUAL_PTR(NULL, take(myTestStack));
}

TEST(stacks, Taking_extra_item_returns_NULL)
{
	add(myTestStack, (void *)1);
	take(myTestStack);
	TEST_ASSERT_EQUAL_PTR(NULL, take(myTestStack));
}

TEST(stacks, Add_and_take_ten_items_in_LIFO_order)
{
	add(myTestStack, (void *)1);
	add(myTestStack, (void *)2);
	add(myTestStack, (void *)3);
	add(myTestStack, (void *)4);
	add(myTestStack, (void *)5);
	add(myTestStack, (void *)6);
	add(myTestStack, (void *)7);
	add(myTestStack, (void *)8);
	add(myTestStack, (void *)9);
	TEST_ASSERT_EQUAL_PTR((void *)9, take(myTestStack));
	TEST_ASSERT_EQUAL_PTR((void *)8, take(myTestStack));
	TEST_ASSERT_EQUAL_PTR((void *)7, take(myTestStack));
	TEST_ASSERT_EQUAL_PTR((void *)6, take(myTestStack));
	TEST_ASSERT_EQUAL_PTR((void *)5, take(myTestStack));
	add(myTestStack, (void *)16);
	add(myTestStack, (void *)17);
	add(myTestStack, (void *)18);

	TEST_ASSERT_EQUAL_PTR((void *)18, take(myTestStack));
	TEST_ASSERT_EQUAL_PTR((void *)17, take(myTestStack));
	TEST_ASSERT_EQUAL_PTR((void *)16, take(myTestStack));
	TEST_ASSERT_EQUAL_PTR((void *)4, take(myTestStack));
	TEST_ASSERT_EQUAL_PTR((void *)3, take(myTestStack));
	TEST_ASSERT_EQUAL_PTR((void *)2, take(myTestStack));
	TEST_ASSERT_EQUAL_PTR((void *)1, take(myTestStack));
	TEST_ASSERT_EQUAL_PTR((void *)NULL, take(myTestStack));
	TEST_ASSERT_EQUAL_PTR((void *)NULL, take(myTestStack));
}

TEST(stacks, contain_does_not_reorder_stack)
{
	add(myTestStack, (void *)1);
	add(myTestStack, (void *)2);
	add(myTestStack, (void *)3);
	add(myTestStack, (void *)4);
	add(myTestStack, (void *)5);

	// random ordered test for contains of each value
	TEST_ASSERT_EQUAL(TRUE, contains(myTestStack, (void *)1));
	TEST_ASSERT_EQUAL(TRUE, contains(myTestStack, (void *)4));
	TEST_ASSERT_EQUAL(TRUE, contains(myTestStack, (void *)3));
	TEST_ASSERT_EQUAL(TRUE, contains(myTestStack, (void *)5));
	TEST_ASSERT_EQUAL(TRUE, contains(myTestStack, (void *)2));

	TEST_ASSERT_EQUAL_PTR((void *)5, take(myTestStack) );
	TEST_ASSERT_EQUAL_PTR((void *)4, take(myTestStack) );
	TEST_ASSERT_EQUAL_PTR((void *)3, take(myTestStack) );
	TEST_ASSERT_EQUAL_PTR((void *)2, take(myTestStack) );
	TEST_ASSERT_EQUAL_PTR((void *)1, take(myTestStack) );
	TEST_ASSERT_EQUAL_PTR(NULL, take(myTestStack));
}

/***************************************************/
/*********** Start of Stack Init Testing ***********/
/***************************************************/

TEST_GROUP(stacks_init);

TEST_SETUP(stacks_init)
{
	RuntimeErrorStub_Reset();
	//clear_Stack_and_StackClass();
}

TEST_TEAR_DOWN(stacks_init)
{
	//clear_Stack_and_StackClass();
	RuntimeErrorStub_Reset();
}

/*************************************/

TEST(stacks, Multiple_calls_to_Stack_init_do_not_change_ptr)
{
	const void * oldStack = Stack;
	const void * oldStackClass = StackClass;
	Stack_init();
	Stack_init();
	TEST_ASSERT_EQUAL_PTR(oldStack, Stack);
	TEST_ASSERT_EQUAL_PTR(oldStackClass, StackClass);
}

TEST(stacks, New_createsAnEmptyStack)
{/**/
	//Stack_init();
	//myTestStack = new(Stack, voidStackArray, MIN_LIST_BUFFER_SIZE);
	//myTestStack = new(Stack, voidStackArray);
	TEST_ASSERT_NOT_EQUAL(NULL, myTestStack);

	TEST_ASSERT_EQUAL_PTR(voidStackArray, bufPointer(myTestStack));
	TEST_ASSERT_EQUAL(MIN_LIST_BUFFER_SIZE, arrayDimension(myTestStack));
	TEST_ASSERT_EQUAL(0, getItemCount(myTestStack));
	TEST_ASSERT_EQUAL(0, beginIndex(myTestStack));
	TEST_ASSERT_EQUAL(0, endIndex(myTestStack));

	//clearListObjectValues(myTestStack);
	//delete(myTestStack);
	//myTestStack = NULL;
}



