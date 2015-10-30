/*
 * lists-test.c
 *
 *  Created on: Mar 13, 2015
 *      Author: Alan
 */

#include <string.h>
//#include <stdlib.h> // support for free()
//#include <assert.h>
#include "..\..\src\cross_compiler_defs.h"
#include "..\unity\unity_fixture.h"
//#include "..\unity\unity.h"
#include "..\..\src\mocks\RuntimeErrorStub.h"

//#include "..\..\src\objects\objects.h"
//#include "..\..\src\objects\objects-private.h"
#include "..\..\src\lists\lists.h"
#include "..\..\src\lists\lists-private.h"



/****************************************/
/*** Basic Object and Class Testing ****/
struct List *       myTestList  = NULL;

const struct List * myList = NULL;
struct Class *      List_as_Class = NULL;

const struct ListClass * myListClass = NULL;
struct Object *          ListClass_as_Object = NULL;

#ifndef MIN_LIST_BUFFER_SIZE
#define MIN_LIST_BUFFER_SIZE 4
#endif

void * voidArray[MIN_LIST_BUFFER_SIZE];
void * otherVoidArray[MIN_LIST_BUFFER_SIZE];

struct Object * testObject;

typedef void (* voidf)(); 	/* generic function pointer */
void Print_FunctionPointers(void);
void printVoidArray(void * _array);
void printListDetails(const void * _self);
void Lists_deleteAllItems(void * _listPointer);


//extern const void * List; 		// new(List, void * []);
//extern const void * ListClass; 	// new(List, void * []);

//int xList        = 5;
//int * intPtrList = &xList;
int i            = 0;

/****************************************/
/*********** Start of Testing ***********/

TEST_GROUP(lists);

TEST_SETUP(lists)
{
	RuntimeErrorStub_Reset();
	List_init();

	//myTestList = new(List, voidArray, MIN_LIST_BUFFER_SIZE);
	myTestList = new(List, voidArray);

	myList         = (void *)List;
	List_as_Class  = (void *)List;

	myListClass    = (void *)ListClass;

	testObject = new(Object);
}

TEST_TEAR_DOWN(lists)
{
	Lists_deleteAllItems(myTestList);
	testObject = safeDelete(testObject);
	myTestList = safeDelete(myTestList);

	RuntimeErrorStub_Reset();
}


/*************************************/
/*********  Setup List ***************/

TEST(lists, Create)
{
}

TEST(lists, Delete_ReturnsMyTestList)
{
	TEST_ASSERT_EQUAL_PTR(myTestList, delete(myTestList));
	// WARNING:  If myTestList is deleted, then pointer must be set to NULL
	//           Delete frees the memory ... reuse of myTestList without new
	//           will overwrite active memory
	myTestList = NULL;
}

TEST(lists, Delete_SecondDeleteReturnsNull)
{
	myTestList = safeDelete(myTestList);
	// WARNING:  If myTestList is deleted, then pointer must be set to NULL
	//           delete() frees the memory ... reuse of myTestList without new()
	//           will overwrite active memory
	TEST_ASSERT_EQUAL_PTR(NULL, delete(myTestList));
}

TEST(lists, clearListObjectValues_ClearsLocalData)
{
	// add one item to move counts off of zero
	TEST_ASSERT_EQUAL_PTR((void *)1, add(myTestList, (void *)1) );

	clearListObjectValues(myTestList);

	TEST_ASSERT_EQUAL_PTR(NULL, bufPointer(myTestList));
	TEST_ASSERT_EQUAL(0, arrayDimension(myTestList));
	TEST_ASSERT_EQUAL(0, getItemCount(myTestList));
	TEST_ASSERT_EQUAL(0, beginIndex(myTestList));
	TEST_ASSERT_EQUAL(0, endIndex(myTestList));
}


TEST(lists, new_buf_PointsTo_array)
{
	TEST_ASSERT_EQUAL_PTR(voidArray, myTestList->buf);
}


TEST(lists, new_arrayDimension_reports_MIN_LIST_BUFFER_SIZE)
{
	TEST_ASSERT_EQUAL(MIN_LIST_BUFFER_SIZE, myTestList->arrayDimension);
	TEST_ASSERT_EQUAL(MIN_LIST_BUFFER_SIZE, arrayDimension(myTestList));
}

TEST(lists, new_BeginEndCount_report_zero)
{
	TEST_ASSERT_EQUAL(0, myTestList->countOfItems);
	TEST_ASSERT_EQUAL(0, myTestList->beginIndex);
	TEST_ASSERT_EQUAL(0, myTestList->endIndex);

	TEST_ASSERT_EQUAL(0, getItemCount(myTestList));
	TEST_ASSERT_EQUAL(0, beginIndex(myTestList));
	TEST_ASSERT_EQUAL(0, endIndex(myTestList));
}

TEST(lists, init_List_dtor_Returns_Null_on_NullPtr)
{
	TEST_ASSERT_EQUAL_PTR(NULL, List_dtor(NULL));
}

TEST(lists, init_List_dtor_Returns_self_on_myTestList)
{
	void* originalPointer = myTestList;
	TEST_ASSERT_EQUAL_PTR(originalPointer, List_dtor(myTestList));
}

/**************************************/
/*********  ListClass  ****************/
TEST(lists, init_ListClass_Returns_NonNull)
{
	TEST_ASSERT_NOT_EQUAL(NULL, ListClass);
}

TEST(lists, init_ListClass_Equals_myListClass)
{
	TEST_ASSERT_EQUAL_PTR(ListClass, myListClass);
}


TEST(lists, init_ListClass_NameIs_ListClass)
{
	TEST_ASSERT_EQUAL_STRING("ListClass", myListClass->_.className);
}

TEST(lists, init_ListClass_ctor_Returns_ListClass_ctor)
{
	TEST_ASSERT_EQUAL_PTR(ListClass_ctor, myListClass->_.ctor);
	TEST_ASSERT_EQUAL_PTR(ListClass_ctor, myList->_.class->ctor);
}

TEST(lists, init_ListClass_super_Returns_Class)
{
	TEST_ASSERT_EQUAL_PTR(Class, myListClass->_.superclass);
}

TEST(lists, init_ListClass_SizeOfClass_List_Returns_64)
{
	TEST_ASSERT_EQUAL(64, sizeof(struct ListClass));
}

TEST(lists, init_ListClass_add_Returns_List_add)
{
	const struct ListClass * lcp = classOf(myTestList);
	TEST_ASSERT_EQUAL_PTR(List_add, lcp->add);
	TEST_ASSERT_EQUAL_PTR(List_add, ((struct ListClass *)myList)->add);
	TEST_ASSERT_EQUAL_PTR(List_add, ((struct ListClass *)classOf(myTestList))->add);
}


/*********************************/
/*********  List  ****************/

TEST(lists, init_List_Returns_NonNull)
{
	TEST_ASSERT_NOT_EQUAL(NULL, List);
}

TEST(lists, init_List_NameIs_List)
{
	TEST_ASSERT_EQUAL_STRING("List", List_as_Class->className);
	TEST_ASSERT_EQUAL_STRING("List", myTestList->_.class->className);
}

TEST(lists, init_List_ctor_Returns_List_ctor)
{
	TEST_ASSERT_EQUAL_PTR(List_ctor, List_as_Class->ctor);
	TEST_ASSERT_EQUAL_PTR(List_ctor, myTestList->_.class->ctor);
}

TEST(lists, init_List_dtor_Returns_List_dtor)
{
	TEST_ASSERT_EQUAL_PTR(List_dtor, List_as_Class->dtor);
	TEST_ASSERT_EQUAL_PTR(List_dtor, myTestList->_.class->dtor);
}

TEST(lists, init_List_class_Returns_ListClass)
{
	TEST_ASSERT_EQUAL_PTR(ListClass, myList->_.class);
	TEST_ASSERT_EQUAL_PTR(ListClass, myTestList->_.class->_.class);

}

TEST(lists, init_List_Super_Returns_Object)
{
	TEST_ASSERT_EQUAL_PTR(Object, List_as_Class->superclass);
	TEST_ASSERT_EQUAL_PTR(Object, myTestList->_.class->superclass);
}

TEST(lists, init_List_SizeOfObject_List_Returns_32)
{
	TEST_ASSERT_EQUAL(32, sizeof(*myTestList));
	TEST_ASSERT_EQUAL(32, myTestList->_.class->sizeOfThisClass);
}

TEST(lists, init_List_size_is_32)
{
	TEST_ASSERT_EQUAL(32, sizeof(struct List));
}


// *** Testing NEW method pointers **************************************//
// *** const struct XXXXClass * myXXXXClass *****************************//
// *** use (XXXX_method, myXXXXClass->method ) = classOf(myTestXXXX); ***//

TEST(lists, init_ListClass_add)
{
	TEST_ASSERT_EQUAL_PTR(List_add, ((struct ListClass *)classOf(myTestList))->add);
	TEST_ASSERT_EQUAL_PTR(List_add, ((struct ListClass *)myList)->add);
}

TEST(lists, init_ListClass_take)
{
	TEST_ASSERT_EQUAL_PTR(List_take, ((struct ListClass *)classOf(myTestList))->take);
	TEST_ASSERT_EQUAL_PTR(List_take, ((struct ListClass *)myList)->take);
}

IGNORE_TEST(lists, init_ListClass_addFirst)
{
	//TEST_ASSERT_EQUAL_PTR(List_addFirst, ((struct ListClass *)classOf(myTestList))->addFirst);
	//TEST_ASSERT_EQUAL_PTR(List_addFirst, ((struct ListClass *)myList)->addFirst);
}

IGNORE_TEST(lists, init_ListClass_addLast)
{
	//TEST_ASSERT_EQUAL_PTR(List_addLast, ((struct ListClass *)classOf(myTestList))->addLast);
	//TEST_ASSERT_EQUAL_PTR(List_addLast, ((struct ListClass *)myList)->addLast);
}

IGNORE_TEST(lists, init_ListClass_takeFirst)
{
	//TEST_ASSERT_EQUAL_PTR(List_takeFirst, ((struct ListClass *)classOf(myTestList))->takeFirst);
	//TEST_ASSERT_EQUAL_PTR(List_takeFirst, ((struct ListClass *)myList)->takeFirst);
}

IGNORE_TEST(lists, init_ListClass_takeLast)
{
	//TEST_ASSERT_EQUAL_PTR(List_takeLast, ((struct ListClass *)classOf(myTestList))->takeLast);
	//TEST_ASSERT_EQUAL_PTR(List_takeLast, ((struct ListClass *)myList)->takeLast);
}


// *** Testing New method behaviors ********************//

TEST(lists, List_add_ReturnsElementOnSuccess)
{
	TEST_ASSERT_EQUAL_PTR(testObject, List_add(myTestList, testObject));
	List_take(myTestList);
}

TEST(lists, List_add_ReturnsNullOnNullList)
{
	TEST_ASSERT_EQUAL_PTR(NULL, List_add(NULL, testObject));
}

TEST(lists, List_take_ReturnsItemOnSuccess)
{
	List_add(myTestList, testObject);
	TEST_ASSERT_EQUAL_PTR(testObject, List_take(myTestList));
}

TEST(lists, List_take_ReturnsNullOnNullList)
{
	List_add(myTestList, testObject);
	TEST_ASSERT_EQUAL_PTR(NULL, List_take(NULL));
	List_take(myTestList);
}


TEST(lists, List_take_ReturnsNullOnNoItem)
{
	TEST_ASSERT_EQUAL_PTR(NULL, List_take(myTestList));
}

/**********************************/
TEST(lists, add_ReturnsElementOnSuccess)
{
	TEST_ASSERT_EQUAL_PTR(testObject, add(myTestList, testObject));
	take(myTestList);
}

TEST(lists, add_ReturnsNullOnNullList)
{
	TEST_ASSERT_EQUAL_PTR(NULL, add(NULL, testObject));
}

IGNORE_TEST(lists, add_Returns_Null_For_NonValid_List_Ptr)
{
	// system destablizes when trying to access a nonvalid list
	// the lookup function may solve the issue in an embedded environment
	TEST_ASSERT_EQUAL_PTR(NULL, add(myTestList+1, testObject));
}

TEST(lists, take_ReturnsItemOnSuccess)
{
	add(myTestList, testObject);
	TEST_ASSERT_EQUAL_PTR(testObject, take(myTestList));
}

TEST(lists, take_ReturnsNullOnNullList)
{
	add(myTestList, testObject);
	TEST_ASSERT_EQUAL_PTR(NULL, take(NULL));
	take(myTestList);
}

TEST(lists, take_ReturnsNullOnNoItem)
{
	TEST_ASSERT_EQUAL_PTR(NULL, take(myTestList));
}

TEST(lists, contains_Returns_zero_for_Null_list)
{
	TEST_ASSERT_EQUAL(FALSE, contains(myTestList, testObject));
}

TEST(lists, contains_Returns_one_for_item_found)
{
	add(myTestList, testObject);
	TEST_ASSERT_EQUAL(TRUE, contains(myTestList, testObject));
	take(myTestList);
}

TEST(lists, contains_does_not_reorder_list)
{
	struct Object * object1 = new(Object);
	struct Object * object2 = new(Object);
	struct Object * object3 = new(Object);
	struct Object * object4 = new(Object);
	struct Object * object5 = new(Object);


	add(myTestList, object1);
	add(myTestList, object2);
	add(myTestList, object3);
	add(myTestList, object4);
	add(myTestList, object5);

	// random ordered test for contains of each value
	TEST_ASSERT_EQUAL(TRUE, contains(myTestList, object1));
	TEST_ASSERT_EQUAL(TRUE, contains(myTestList, object4));
	TEST_ASSERT_EQUAL(TRUE, contains(myTestList, object3));
	TEST_ASSERT_EQUAL(TRUE, contains(myTestList, object5));
	TEST_ASSERT_EQUAL(TRUE, contains(myTestList, object2));

	TEST_ASSERT_EQUAL_PTR(object1, take(myTestList) );
	TEST_ASSERT_EQUAL_PTR(object2, take(myTestList) );
	TEST_ASSERT_EQUAL_PTR(object3, take(myTestList) );
	TEST_ASSERT_EQUAL_PTR(object4, take(myTestList) );
	TEST_ASSERT_EQUAL_PTR(object5, take(myTestList) );
	TEST_ASSERT_EQUAL_PTR(NULL, take(myTestList));

	object1 = safeDelete(object1);
	object2 = safeDelete(object2);
	object3 = safeDelete(object3);
	object4 = safeDelete(object4);
	object5 = safeDelete(object5);
}

/**********************************/
TEST(lists, List_addLast_CountAndEndIncrement_OnAdd)
{
	List_addLast(myTestList, NULL);
	TEST_ASSERT_EQUAL(1, getItemCount(myTestList));
	TEST_ASSERT_EQUAL(1, endIndex(myTestList));
}

TEST(lists, List_addLast_ReturnsElement_OnAdd)
{
	TEST_ASSERT_EQUAL(testObject, List_addLast(myTestList, testObject));
	take(myTestList);
}

TEST(lists, List_addLast_CanAddMaxItems)
{
	// WARNING: arrayDimension(myTestList) changes when array extends on max item add
	int originalArrayDimension = arrayDimension(myTestList);
	for ( i = 0; i < originalArrayDimension; i++) {
		TEST_ASSERT_EQUAL((void *)i+1, List_addLast(myTestList, (void *)i+1));
		TEST_ASSERT_EQUAL(i+1, getItemCount(myTestList));
	}

	for ( i = (originalArrayDimension-1); i >= 0; i--) {
		TEST_ASSERT_EQUAL((void *)i+1, List_takeLast(myTestList));
		TEST_ASSERT_EQUAL(i, getItemCount(myTestList));
	}
}

TEST(lists, List_addLast_CanAddFourItems)
{
	//printf("283: List_addLast_CanAddFourItems \n");
	TEST_ASSERT_EQUAL(1, List_addLast(myTestList, (void *)1));
	TEST_ASSERT_EQUAL(2, List_addLast(myTestList, (void *)2));
	TEST_ASSERT_EQUAL(3, List_addLast(myTestList, (void *)3));
	//WARNING:  arrayDimension(myTestList) extends on 4th add
	TEST_ASSERT_EQUAL(4, List_addLast(myTestList, (void *)4));
	//printVoidArray(myTestList);
	TEST_ASSERT_EQUAL(1, myTestList->buf[0 + MIN_LIST_BUFFER_SIZE]);
	TEST_ASSERT_EQUAL(2, myTestList->buf[1 + MIN_LIST_BUFFER_SIZE]);
	TEST_ASSERT_EQUAL(3, myTestList->buf[2 + MIN_LIST_BUFFER_SIZE]);
	TEST_ASSERT_EQUAL(4, myTestList->buf[3 + MIN_LIST_BUFFER_SIZE]);

	// remove non-Object pointers from list to protect delete()
	for ( i = 0; i < 4; i++ ) {
		take(myTestList);
	}
}

TEST(lists, List_addLast_ArrayExtendsOnAddMax)
{
	TEST_ASSERT_EQUAL(MIN_LIST_BUFFER_SIZE, arrayDimension(myTestList));
	List_addLast(myTestList, (void *)1);
	List_addLast(myTestList, (void *)2);
	List_addLast(myTestList, (void *)3);
	List_addLast(myTestList, (void *)4);

	TEST_ASSERT_EQUAL(MIN_LIST_BUFFER_SIZE*2, arrayDimension(myTestList));

	// remove non-Object pointers from list to protect delete()
	int localCountOfItems = myTestList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(myTestList);
	}

}

/**********************************/

TEST(lists, List_addFirst_CountAndEndIncrement_OnAdd)
{
	List_addFirst(myTestList, NULL);
	TEST_ASSERT_EQUAL(1, myTestList->countOfItems);
	TEST_ASSERT_EQUAL(arrayDimension(myTestList) - 1, beginIndex(myTestList));
	TEST_ASSERT_EQUAL(myTestList->arrayDimension - 1, myTestList->beginIndex);

}

TEST(lists, List_addFirst_ReturnsElement_OnAdd)
{
	TEST_ASSERT_EQUAL(testObject, List_addFirst(myTestList, testObject));
	take(myTestList);
}

TEST(lists, List_addFirst_CanAddFourItems)
{
	//printf("\n319: List_addFirst_CanAddFourItems ... inital values");
	//TEST_ASSERT_EQUAL(NULL, List_takeLast(myTestList));
	//TEST_ASSERT_EQUAL(NULL, List_takeLast(myTestList));
	//TEST_ASSERT_EQUAL(NULL, List_takeLast(myTestList));
	//TEST_ASSERT_EQUAL(NULL, List_takeLast(myTestList));

	//printVoidArray(myTestList);
	TEST_ASSERT_EQUAL( 5, List_addFirst(myTestList, (void *)5));
	//printf("\nList_addFirst_CanAddFourItems ... after first add");
	//printVoidArray(myTestList);
	TEST_ASSERT_EQUAL( 6, List_addFirst(myTestList, (void *)6));
	//printVoidArray(myTestList);
	TEST_ASSERT_EQUAL( 7, List_addFirst(myTestList, (void *)7));
	//printVoidArray(myTestList);
	TEST_ASSERT_EQUAL( 8, List_addFirst(myTestList, (void *)8));

	//printVoidArray(myTestList);
	TEST_ASSERT_EQUAL(8, myTestList->buf[arrayDimension(myTestList)-4]);
	TEST_ASSERT_EQUAL(7, myTestList->buf[arrayDimension(myTestList)-3]);
	TEST_ASSERT_EQUAL(6, myTestList->buf[arrayDimension(myTestList)-2]);
	TEST_ASSERT_EQUAL(5, myTestList->buf[arrayDimension(myTestList)-1]);

	// remove non-Object pointers from list to protect delete()
	int localCountOfItems = myTestList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(myTestList);
	}
}

TEST(lists, List_addFirst_ArrayExtendsOnAddMax)
{
	TEST_ASSERT_EQUAL(MIN_LIST_BUFFER_SIZE, arrayDimension(myTestList));
	List_addFirst(myTestList, (void *)1);
	List_addFirst(myTestList, (void *)2);
	List_addFirst(myTestList, (void *)3);
	List_addFirst(myTestList, (void *)4);

	TEST_ASSERT_EQUAL(MIN_LIST_BUFFER_SIZE*2, arrayDimension(myTestList));

	// remove non-Object pointers from list to protect delete()
	int localCountOfItems = myTestList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(myTestList);
	}
}
/**********************************/
TEST(lists, List_takeLast_ReturnsElements_OnTake)
{
	List_addLast(myTestList, testObject);
	TEST_ASSERT_EQUAL(testObject, List_takeLast(myTestList));
}

TEST(lists, List_takeLast_ReturnsNull_OnEmpty)
{
	TEST_ASSERT_EQUAL(0, myTestList->countOfItems);
	TEST_ASSERT_EQUAL(NULL, List_takeLast(myTestList));
}

TEST(lists, List_takeLast_ReturnsNull_OnNullList)
{
	TEST_ASSERT_EQUAL(NULL, List_takeLast(NULL));
}

TEST(lists, List_takeLast_ValueAtTakenIndexOverWrittenWithNull)
{
	List_addFirst(myTestList, testObject);
	List_takeLast(myTestList);
	TEST_ASSERT_EQUAL(NULL, myTestList->buf[0]);
}

TEST(lists, List_takeLast_AllValuesTakenAreOverwrittenWithNull)
{	/**/
	List_addLast(myTestList, (void *)1);
	List_addLast(myTestList, (void *)2);
	List_addLast(myTestList, (void *)3);
	List_addLast(myTestList, (void *)4);

	TEST_ASSERT_EQUAL(4, List_takeLast(myTestList));
	TEST_ASSERT_EQUAL(3, List_takeLast(myTestList));
	TEST_ASSERT_EQUAL(2, List_takeLast(myTestList));
	TEST_ASSERT_EQUAL(1, List_takeLast(myTestList));

	TEST_ASSERT_EQUAL(NULL, myTestList->buf[0]);
	TEST_ASSERT_EQUAL(NULL, myTestList->buf[1]);
	TEST_ASSERT_EQUAL(NULL, myTestList->buf[2]);
	TEST_ASSERT_EQUAL(NULL, myTestList->buf[3]);

	// remove non-Object pointers from list to protect delete()
	int localCountOfItems = myTestList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(myTestList);
	}
}


/**********************************/
TEST(lists, List_takeFirst_ReturnsElements_OnTake)
{
	List_addLast(myTestList, testObject);
	TEST_ASSERT_EQUAL(testObject, List_takeFirst(myTestList));
}

TEST(lists, List_takeFirst_ReturnsNull_OnEmpty)
{
	TEST_ASSERT_EQUAL(0, myTestList->countOfItems);
	TEST_ASSERT_EQUAL(NULL, List_takeFirst(myTestList));
}

TEST(lists, List_takeFirst_ReturnsNull_OnNullList)
{
	TEST_ASSERT_EQUAL(NULL, List_takeFirst(NULL));
}

TEST(lists, List_takeFirst_ValueAtTakenIndexOverWrittenWithNull)
{
	List_addFirst(myTestList, testObject);
	List_takeFirst(myTestList);
	TEST_ASSERT_EQUAL(NULL, myTestList->buf[0]);
}




/*********  isOfSuper_sc  ****************/

TEST(lists, isOfSuper_sc_ReturnsNullForSameClass)
{
	// myTestList is on class List, not of superClass List
	TEST_ASSERT_EQUAL(0, isOfSuper(myTestList, List));
}

TEST(lists, isOfSuper_sc_ReturnsNullForSubClass)
{
	// List is a subclass of Object, not a superclass
	struct Object * myObject = new(Object);
	TEST_ASSERT_EQUAL(0, isOfSuper(myObject, List));
	myObject = safeDelete(myObject);
}

TEST(lists, isOfSuper_sc_ReturnsTrueForActualSuperClassRelationship)
{
	// myTestList is of class List, and has a superclass of Object
	// myListClass is of class Class, and has a super class of Object
	TEST_ASSERT_EQUAL(1, isOfSuper(myTestList, Object));
	TEST_ASSERT_EQUAL(1, isOfSuper(myListClass, Object));
}

/********  extendArray with addLast  *************/
TEST(lists, extentArrayAddLastWrapsBeginToFour)
{
	// endIndex after three adds
	List_addLast(myTestList, (void *)1);
	List_addLast(myTestList, (void *)2);
	List_addLast(myTestList, (void *)3);
	TEST_ASSERT_EQUAL(0, beginIndex(myTestList));

	// Adding 4th item
	List_addLast(myTestList, (void *)4);
	TEST_ASSERT_EQUAL(4, beginIndex(myTestList));

	// remove non-Object pointers from list to protect delete()
	int localCountOfItems = myTestList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(myTestList);
	}
}

TEST(lists, secondExtentArrayAddLastWrapsBeginToTwelve)
{
	// endIndex after three adds
	List_addLast(myTestList, (void *)1);
	List_addLast(myTestList, (void *)2);
	List_addLast(myTestList, (void *)3);
	List_addLast(myTestList, (void *)4);
	List_addLast(myTestList, (void *)5);
	List_addLast(myTestList, (void *)6);
	List_addLast(myTestList, (void *)7);
	TEST_ASSERT_EQUAL(4, beginIndex(myTestList));

	// Adding 8th item
	List_addLast(myTestList, (void *)8);
	TEST_ASSERT_EQUAL(12, beginIndex(myTestList));

	// remove non-Object pointers from list to protect delete()
	int localCountOfItems = myTestList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(myTestList);
	}
}

TEST(lists, extentArrayAddLastWrapsEndToZero)
{
	// endIndex after three adds
	List_addLast(myTestList, (void *)1);
	List_addLast(myTestList, (void *)2);
	List_addLast(myTestList, (void *)3);
	TEST_ASSERT_EQUAL(3, endIndex(myTestList));

	// Adding 4th item
	List_addLast(myTestList, (void *)4);
	TEST_ASSERT_EQUAL(0, endIndex(myTestList));

	// remove non-Object pointers from list to protect delete()
	int localCountOfItems = myTestList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(myTestList);
	}
}

TEST(lists, secondExtentArrayAddLastWrapsEndToFour)
{
	// endIndex after three adds
	List_addLast(myTestList, (void *)1);
	List_addLast(myTestList, (void *)2);
	List_addLast(myTestList, (void *)3);
	List_addLast(myTestList, (void *)4);
	List_addLast(myTestList, (void *)5);
	List_addLast(myTestList, (void *)6);
	List_addLast(myTestList, (void *)7);
	TEST_ASSERT_EQUAL(3, endIndex(myTestList));

	// Adding 8th item
	List_addLast(myTestList, (void *)8);
	TEST_ASSERT_EQUAL(4, endIndex(myTestList));

	// remove non-Object pointers from list to protect delete()
	int localCountOfItems = myTestList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(myTestList);
	}
}

/********  extendArray with addFirst  *************/
TEST(lists, extentArrayAddFirstWrapsBeginToFour)
{
	// endIndex after three adds
	List_addFirst(myTestList, (void *)1);
	List_addFirst(myTestList, (void *)2);
	List_addFirst(myTestList, (void *)3);
	TEST_ASSERT_EQUAL(1, beginIndex(myTestList));

	// Adding 4th item
	List_addFirst(myTestList, (void *)4);
	TEST_ASSERT_EQUAL(4, beginIndex(myTestList));

	// remove non-Object pointers from list to protect delete()
	int localCountOfItems = myTestList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(myTestList);
	}
}

TEST(lists, secondExtentArrayAddFirstWrapsBeginToEight)
{
	// endIndex after three adds
	List_addFirst(myTestList, (void *)1);
	List_addFirst(myTestList, (void *)2);
	List_addFirst(myTestList, (void *)3);
	List_addFirst(myTestList, (void *)4);
	List_addFirst(myTestList, (void *)5);
	List_addFirst(myTestList, (void *)6);
	List_addFirst(myTestList, (void *)7);
	TEST_ASSERT_EQUAL(1, beginIndex(myTestList));

	// Adding 8th item
	List_addFirst(myTestList, (void *)8);
	TEST_ASSERT_EQUAL(8, beginIndex(myTestList));

	// remove non-Object pointers from list to protect delete()
	int localCountOfItems = myTestList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(myTestList);
	}
}

TEST(lists, extentArrayAddFirstWrapsEndToFour)
{
	// endIndex after three adds
	List_addFirst(myTestList, (void *)1);
	List_addFirst(myTestList, (void *)2);
	List_addFirst(myTestList, (void *)3);
	TEST_ASSERT_EQUAL(0, endIndex(myTestList));

	// Adding 4th item
	List_addFirst(myTestList, (void *)4);
	TEST_ASSERT_EQUAL(0, endIndex(myTestList));

	// remove non-Object pointers from list to protect delete()
	int localCountOfItems = myTestList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(myTestList);
	}
}

TEST(lists, secondExtentArrayAddFirstWrapsEndToTwelve)
{
	// endIndex after three adds
	List_addFirst(myTestList, (void *)1);
	List_addFirst(myTestList, (void *)2);
	List_addFirst(myTestList, (void *)3);
	List_addFirst(myTestList, (void *)4);
	List_addFirst(myTestList, (void *)5);
	List_addFirst(myTestList, (void *)6);
	List_addFirst(myTestList, (void *)7);
	TEST_ASSERT_EQUAL(0, endIndex(myTestList));

	// Adding 8th item
	List_addFirst(myTestList, (void *)8);
	TEST_ASSERT_EQUAL(0, endIndex(myTestList));

	// remove non-Object pointers from list to protect delete()
	int localCountOfItems = myTestList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(myTestList);
	}
}

/********  reduceArray with takeLast  *************/
TEST(lists, reduceArrayCutsExtendedArrayInHalf)
{
	// endIndex after three adds
	List_addLast(myTestList, (void *)1);
	List_addLast(myTestList, (void *)2);
	List_addLast(myTestList, (void *)3);
	List_addLast(myTestList, (void *)4);
	List_addLast(myTestList, (void *)5);
	List_addLast(myTestList, (void *)6);
	List_addLast(myTestList, (void *)7);
	TEST_ASSERT_EQUAL(8, arrayDimension(myTestList));

	// Adding 8th item
	List_addLast(myTestList, (void *)8);
	TEST_ASSERT_EQUAL(16, arrayDimension(myTestList));

	// taking 9 items
	List_takeLast(myTestList);
	TEST_ASSERT_EQUAL(16, arrayDimension(myTestList));
	List_takeLast(myTestList);
	TEST_ASSERT_EQUAL(8, arrayDimension(myTestList));
	List_takeLast(myTestList);
	TEST_ASSERT_EQUAL(8, arrayDimension(myTestList));
	List_takeLast(myTestList);
	TEST_ASSERT_EQUAL(8, arrayDimension(myTestList));
	List_takeLast(myTestList);
	TEST_ASSERT_EQUAL(8, arrayDimension(myTestList));
	List_takeLast(myTestList);
	TEST_ASSERT_EQUAL(4, arrayDimension(myTestList));
	List_takeLast(myTestList);
	TEST_ASSERT_EQUAL(4, arrayDimension(myTestList));
	List_takeLast(myTestList);
	TEST_ASSERT_EQUAL(4, arrayDimension(myTestList));
	List_takeLast(myTestList);
	TEST_ASSERT_EQUAL(4, arrayDimension(myTestList));

	// remove non-Object pointers from list to protect delete()
	int localCountOfItems = myTestList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(myTestList);
	}
}


//****  List_puto  ****************************

TEST(lists, List_puto_Execute_to_console)
{
	char outputFilename[] = "list_puto.txt";
	FILE * fp = fopen(outputFilename, "a");

	fprintf(fp, "PRINTING: myTestList with puto in %s at line: %u\n", __FILE__, __LINE__);
	puto(myTestList,  fp);

	fprintf(fp, "PRINTING: List with puto in %s at line: %u\n", __FILE__, __LINE__);
	puto(List,  fp);

	fprintf(fp, "PRINTING: ListClass with puto in %s at line: %u\n", __FILE__, __LINE__);
	puto(ListClass, fp);

	fclose(fp);
}

//****  Rotate list contents test  *********************
TEST(lists, Rotate_list_up)
{
	List_addLast(myTestList, (void *)1);
	printVoidArray(myTestList);
	List_addLast(myTestList, (void *)2);
	printVoidArray(myTestList);
	List_addLast(myTestList, (void *)3);
	printVoidArray(myTestList);
	List_addLast(myTestList, (void *)4);
	printVoidArray(myTestList);

	List_addLast(myTestList, List_takeFirst(myTestList));
	printVoidArray(myTestList);
	List_addLast(myTestList, List_takeFirst(myTestList));
	printVoidArray(myTestList);
	List_addLast(myTestList, List_takeFirst(myTestList));
	printVoidArray(myTestList);
	List_addLast(myTestList, List_takeFirst(myTestList));
	printVoidArray(myTestList);

	// remove non-Object pointers from list to protect delete()
	int localCountOfItems = myTestList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(myTestList);
	}
}

TEST(lists, Rotate_list_down)
{
	List_addFirst(myTestList, (void *)1);
	printVoidArray(myTestList);
	List_addFirst(myTestList, (void *)2);
	printVoidArray(myTestList);
	List_addFirst(myTestList, (void *)3);
	printVoidArray(myTestList);
	List_addFirst(myTestList, (void *)4);
	printVoidArray(myTestList);

	List_addFirst(myTestList, List_takeLast(myTestList));
	printVoidArray(myTestList);
	List_addFirst(myTestList, List_takeLast(myTestList));
	printVoidArray(myTestList);
	List_addFirst(myTestList, List_takeLast(myTestList));
	printVoidArray(myTestList);
	List_addFirst(myTestList, List_takeLast(myTestList));
	printVoidArray(myTestList);

	// remove non-Object pointers from list to protect delete()
	int localCountOfItems = myTestList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(myTestList);
	}
}

//****  List contains test  ****************************

TEST(lists, List_contains_test)
{
	List_addLast(myTestList, (void *)1);
	List_addLast(myTestList, (void *)2);
	List_addLast(myTestList, (void *)3);
	TEST_ASSERT_EQUAL(FALSE, List_contains(myTestList, (void *)0));
	TEST_ASSERT_EQUAL(TRUE,  List_contains(myTestList, (void *)3));

	List_addLast(myTestList, (void *)4);
	List_addLast(myTestList, (void *)5);
	List_addLast(myTestList, (void *)6);

	TEST_ASSERT_EQUAL(TRUE,  List_contains(myTestList, (void *)5));

	List_addLast(myTestList, (void *)7);
	TEST_ASSERT_EQUAL(TRUE,  List_contains(myTestList, (void *)6));
	TEST_ASSERT_EQUAL(FALSE,  List_contains(myTestList, (void *)10));

	// remove non-Object pointers from list to protect delete()
	int localCountOfItems = myTestList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(myTestList);
	}
}

TEST(lists, List_contains_Class)
{
	TEST_ASSERT_EQUAL(TRUE,  List_contains(classRegistry,  List));
	TEST_ASSERT_EQUAL(FALSE,  List_contains(classRegistry, myTestList));
	TEST_ASSERT_EQUAL(TRUE,  List_contains(classRegistry,  ListClass));
	TEST_ASSERT_EQUAL(TRUE,  List_contains(classRegistry,  Class));
	TEST_ASSERT_EQUAL(TRUE,  List_contains(classRegistry,  Object));

	/**/
	TEST_ASSERT_EQUAL(TRUE,   isClass(List));
	TEST_ASSERT_EQUAL(FALSE,  isClass(myTestList));
	TEST_ASSERT_EQUAL(TRUE,   isClass(ListClass));
	TEST_ASSERT_EQUAL(TRUE,   isClass(Class));
	TEST_ASSERT_EQUAL(TRUE,   isClass(Object));
	TEST_ASSERT_EQUAL(FALSE,  isClass(NULL));

}

//****  List resetIndex test  **************************

TEST(lists, resetIndex_returnsNullOnNullList)
{
	TEST_ASSERT_EQUAL_PTR(NULL,  resetIndex(NULL));
}

TEST(lists, resetIndex_returnsNullOnEmptyList)
{
	TEST_ASSERT_EQUAL_PTR(NULL,  resetIndex(myTestList));
}

TEST(lists, resetIndex_returnsFirstItemInTheList)
{
	add(myTestList, testObject);
	TEST_ASSERT_EQUAL_PTR(testObject,  resetIndex(myTestList));

	int localCountOfItems = myTestList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(myTestList);
	}
}

TEST(lists, resetIndex_returnsFirstAfterMoreItemsAdded)
{
	add(myTestList, (void *)5);
	add(myTestList, (void *)6);
	add(myTestList, (void *)7);
	TEST_ASSERT_EQUAL_PTR((void *)5,  resetIndex(myTestList));

	// remove non-Object pointers from list to protect delete()
	int localCountOfItems = myTestList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(myTestList);
	}
}

//****  List getNext test  **************************

TEST(lists, getNext_returnsNullOnNullList)
{
	TEST_ASSERT_EQUAL_PTR(NULL,  incrementGet(NULL));
}

TEST(lists, getNext_returnsNullOnEmptyList)
{
	TEST_ASSERT_EQUAL_PTR(NULL,  incrementGet(myTestList));
}

TEST(lists, getNext_returnsFirstItemInSingleItemList)
{
	add(myTestList, testObject);
	TEST_ASSERT_EQUAL_PTR(testObject,  incrementGet(myTestList));

	int localCountOfItems = myTestList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(myTestList);
	}
}

TEST(lists, getNext_returnsSecondItemOfTwoItemList)
{
	add(myTestList, (void *)5);
	add(myTestList, (void *)6);
	TEST_ASSERT_EQUAL_PTR((void *)6,  incrementGet(myTestList));

	// remove non-Object pointers from list to protect delete()
	int localCountOfItems = myTestList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(myTestList);
	}
}

TEST(lists, getNext_LoopsThroughMultiItemList)
{
	add(myTestList, (void *)1);
	add(myTestList, (void *)2);
	add(myTestList, (void *)3);
	resetIndex(myTestList);
	TEST_ASSERT_EQUAL_PTR((void *)2,  incrementGet(myTestList));
	TEST_ASSERT_EQUAL_PTR((void *)3,  incrementGet(myTestList));
	TEST_ASSERT_EQUAL_PTR((void *)1,  incrementGet(myTestList));

	TEST_ASSERT_EQUAL_PTR((void *)2,  incrementGet(myTestList));
	TEST_ASSERT_EQUAL_PTR((void *)3,  incrementGet(myTestList));
	TEST_ASSERT_EQUAL_PTR((void *)1,  incrementGet(myTestList));

	// remove non-Object pointers from list to protect delete()
	int localCountOfItems = myTestList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(myTestList);
	}
}

TEST(lists, getNext_LoopsThroughWrappedList)
{
	add(myTestList, (void *)1);
	add(myTestList, (void *)2);
	add(myTestList, (void *)3);
	take(myTestList);             // take 1 ... 23
	take(myTestList);             // take 2 ... 3
	add(myTestList, (void *)4);   // add  4 ... 34
	add(myTestList, (void *)5);   // add  5 ... 345

	resetIndex(myTestList);       // returns 3
	TEST_ASSERT_EQUAL_PTR((void *)4,  incrementGet(myTestList));
	TEST_ASSERT_EQUAL_PTR((void *)5,  incrementGet(myTestList));
	TEST_ASSERT_EQUAL_PTR((void *)3,  incrementGet(myTestList));

	TEST_ASSERT_EQUAL_PTR((void *)4,  incrementGet(myTestList));
	TEST_ASSERT_EQUAL_PTR((void *)5,  incrementGet(myTestList));
	TEST_ASSERT_EQUAL_PTR((void *)3,  incrementGet(myTestList));

	// remove non-Object pointers from list to protect delete()
	int localCountOfItems = myTestList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(myTestList);
	}
}

//****  List copy test  **************************

TEST(lists, copy_returnsNullOnNullPointer)
{
	struct Object * object1 = new(Object);
	struct Object * object2 = new(Object);
	struct Object * object3 = new(Object);

	struct List * masterList = new(List, otherVoidArray);
	add(masterList, object1);
	add(masterList, object2);
	add(masterList, object3);
	TEST_ASSERT_EQUAL_PTR(NULL, copy(NULL, masterList));
	TEST_ASSERT_EQUAL_PTR(NULL, copy(myTestList, NULL));

	// remove non-Object pointers from list to protect delete()
	int localCountOfItems = myTestList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		delete(take(myTestList));
	}

	// remove non-Object pointers from list to protect delete()
	localCountOfItems = masterList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		delete(take(masterList));
	}

	//object1 = safeDelete(object1);
	//object2 = safeDelete(object2);
	//object3 = safeDelete(object3);

	masterList = safeDelete(masterList);


}

TEST(lists, copy_returnsSelfOnSuccess)
{
	struct Object * object1 = new(Object);
	struct Object * object2 = new(Object);
	struct Object * object3 = new(Object);

	struct List * masterList = new(List, otherVoidArray);
	add(masterList, object1);
	add(masterList, object2);
	add(masterList, object3);
	TEST_ASSERT_EQUAL_PTR(myTestList, copy(myTestList, masterList));

	int localCountOfItems = masterList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(masterList);
		take(myTestList);
	}
	object1 = safeDelete(object1);
	object2 = safeDelete(object2);
	object3 = safeDelete(object3);

	masterList = safeDelete(masterList);
}

TEST(lists, copy_returnsCopiesCorrectData)
{
	struct Object * object1 = new(Object);
	struct Object * object2 = new(Object);
	struct Object * object3 = new(Object);

	struct List * masterList = new(List, otherVoidArray);
	add(masterList, object1);
	add(masterList, object2);
	add(masterList, object3);

	copy(myTestList, masterList);

	TEST_ASSERT_EQUAL(DOES_CONTAIN, contains(myTestList, object1));
	TEST_ASSERT_EQUAL(DOES_CONTAIN, contains(myTestList, object2));
	TEST_ASSERT_EQUAL(DOES_CONTAIN, contains(myTestList, object3));

	int localCountOfItems = masterList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(masterList);
		take(myTestList);
	}
	object1 = safeDelete(object1);
	object2 = safeDelete(object2);
	object3 = safeDelete(object3);

	masterList = safeDelete(masterList);
}


//****  List equal test  **************************

TEST(lists, equal_NullComparisonsReturnUnequal)
{
	struct Object * object1 = new(Object);
	struct Object * object2 = new(Object);

	struct List * masterList = new(List, otherVoidArray);
	add(masterList, object1);
	add(myTestList, object2);

	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, copy(NULL, masterList));
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, copy(myTestList, NULL));

	int localCountOfItems = masterList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(masterList);
		take(myTestList);
	}
	object1 = safeDelete(object1);
	object2 = safeDelete(object2);

	masterList = safeDelete(masterList);
}

TEST(lists, equal_EmptyListsReturnUnequal)
{
	struct List * masterList = new(List, otherVoidArray);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTestList, masterList));
	masterList = safeDelete(masterList);
}

TEST(lists, equal_CopiedListsReturnEqual)
{
	struct Object * object1 = new(Object);
	struct Object * object2 = new(Object);
	struct Object * object3 = new(Object);

	struct List * masterList = new(List, otherVoidArray);
	add(masterList, object1);
	add(masterList, object2);
	add(masterList, object3);
	copy(myTestList, masterList);
	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTestList, masterList));

	int localCountOfItems = masterList->countOfItems;
	for ( i = 0; i < localCountOfItems; i++ ) {
		take(masterList);
		take(myTestList);
	}
	object1 = safeDelete(object1);
	object2 = safeDelete(object2);
	object3 = safeDelete(object3);

	masterList = safeDelete(masterList);
}



TEST(lists, memory_leak)
{
	// new structure location will move based on other processes
	// create and destroy a large number of objects
	// then look for the base address within an additional 10 new objects

	struct List * new_List = new(List, otherVoidArray);
	struct Object * new_ItemForList = new(Object);
	add(new_List, new_ItemForList);

	struct Object * originalItemPointer = new_ItemForList;

	Lists_deleteAllItems(new_List);
	new_List = safeDelete(new_List);

	// create and destroy a large number of objects to cause a memory leak
	int j;
	for (j = 0; j < 1000; j++) {

		new_List = new(List, otherVoidArray);
		new_ItemForList = new(Object);
		add(new_List, new_ItemForList);

		//printf("new_NodeItem: %p, j: %i\n", new_ItemForList, j);

		Lists_deleteAllItems(new_List);
		new_List = safeDelete(new_List);
	}

	//printf("\nOriginal new_NodeItem target: %p\n", originalItemPointer);

	// check for original object location within a small number of additional objects
	int originalPtrCount = 0;
	for (j = 0; j < 40; j++) {

		new_List = new(List, otherVoidArray);
		new_ItemForList = new(Object);
		add(new_List, new_ItemForList);

		//printf("new_NodeItem: %p, j: %i\n",new_ItemForList, j);

		if (originalItemPointer  == new_ItemForList)  { originalPtrCount++; }

		Lists_deleteAllItems(new_List);
		new_List = safeDelete(new_List);

		}

	printf("\nLists-memory-leak-test, originalPtrCount: %i, out of %i trials\n", originalPtrCount, 40);
	TEST_ASSERT_TRUE(originalPtrCount);
}

/****************************************/
/****************************************/
/****************************************/
/*** Start of Initialization Testing ****/
TEST_GROUP(lists_init);

TEST_SETUP(lists_init)
{
	RuntimeErrorStub_Reset();

}

TEST_TEAR_DOWN(lists_init)
{

	RuntimeErrorStub_Reset();
}

TEST(lists_init, ListClassNotNullAfterInit)
{
	List_init();
	TEST_ASSERT_NOT_EQUAL(NULL, ListClass);

}

TEST(lists_init, ListNotNullAfterInit)
{
	List_init();
	TEST_ASSERT_NOT_EQUAL(NULL, List);
}


TEST(lists_init, MultipleCallsToInitDoNotChangePtr)
{
	List_init();
	struct ListClass * originalListClass = (void *)ListClass;
	struct ListClass * originalList = (void *)List;
	List_init();
	List_init();
	TEST_ASSERT_EQUAL_PTR(originalListClass, ListClass);
	TEST_ASSERT_EQUAL_PTR(originalList, List);
}

TEST(lists_init, New_createsAnEmptyList)
{/**/
	List_init();
	//myTestList = new(List, voidArray, MIN_LIST_BUFFER_SIZE);
	myTestList = new(List, voidArray);
	TEST_ASSERT_NOT_EQUAL(NULL, myTestList);

	TEST_ASSERT_EQUAL_PTR(voidArray, bufPointer(myTestList));
	TEST_ASSERT_EQUAL(MIN_LIST_BUFFER_SIZE, arrayDimension(myTestList));
	TEST_ASSERT_EQUAL(0, getItemCount(myTestList));
	TEST_ASSERT_EQUAL(0, beginIndex(myTestList));
	TEST_ASSERT_EQUAL(0, endIndex(myTestList));

	clearListObjectValues(myTestList);
	delete(myTestList);
	myTestList = NULL;
}



TEST(lists_init, clearListObjectValues_returns_MyList)
{
	List_init();
	myTestList = new(List, voidArray);

	TEST_ASSERT_EQUAL_PTR(myTestList, clearListObjectValues(myTestList));
	myTestList = safeDelete(myTestList);
}

TEST(lists_init, clearListObjectValues_ZerosAndNulls)
{
	List_init();
	myTestList = new(List, voidArray);

	clearListObjectValues(myTestList);

	TEST_ASSERT_EQUAL_PTR(NULL, bufPointer(myTestList));
	TEST_ASSERT_EQUAL(0, arrayDimension(myTestList));
	TEST_ASSERT_EQUAL(0, getItemCount(myTestList));
	TEST_ASSERT_EQUAL(0, beginIndex(myTestList));
	TEST_ASSERT_EQUAL(0, endIndex(myTestList));

	myTestList = safeDelete(myTestList);
}

/*****  delete list  *****************/
/**/

TEST(lists_init, delete_Returns_myTestList_OnEmptyList)
{
	List_init();
	myTestList = new(List, voidArray);
	TEST_ASSERT_EQUAL_PTR(myTestList, delete(myTestList));
}

TEST(lists_init, delete_Returns_myTestList_OnSingleItem)
{
	List_init();
	myTestList = new(List, voidArray);
	add(myTestList, (void *)1);
	TEST_ASSERT_EQUAL_PTR(myTestList, delete(myTestList));
}

TEST(lists_init, delete_Returns_myTestList_OnMultiItemList)
{
	List_init();
	myTestList = new(List, voidArray);
	add(myTestList, (void *)1);
	add(myTestList, (void *)2);
	TEST_ASSERT_EQUAL_PTR(myTestList, delete(myTestList));
}

/****************************************/
/****************************************/
/****************************************/


void Print_FunctionPointers(void)
{
	printf("\nObject          pointer: %p \n", Object);
	printf(  "Class           pointer: %p \n", Class);
	printf(  "ListClass       pointer: %p \n", ListClass);
	printf(  "List            pointer: %p \n", List);
	printf(  "myTestList      pointer: %p \n", myTestList);
	printf(  "add             pointer: %p \n", add);
	printf(  "List_add        pointer: %p \n", List_add);
	printf(  "super_add_cse   pointer: %p \n", super_add);
	printf(  "Object_ctor     pointer: %p \n", Object_ctor);
	printf(  "ctor            pointer: %p \n", ctor);
	printf(  "Class_ctor      pointer: %p \n", Class_ctor);
	printf(  "ListClass_ctor  pointer: %p \n", ListClass_ctor);
	printf(  "List_ctor       pointer: %p \n", List_ctor);

	printf(  "super_ctor      pointer: %p \n", super_ctor);
	printf("\n&myTestList                     pointer: %p \n", &myTestList);
	printf("myTestList                      pointer: %p \n", (void *)myTestList);
	//printf("*myTestList                     pointer: %p \n", *myTestList);
	//printf("*(struct Class *)myTestList     pointer: %p \n", *(struct Class *)myTestList);
	printf("myTestList->_                   pointer: %p \n", (void *)myTestList->_.class);

	printf("myList = List; vvvv  myList     pointer: %p \n", myList);
	printf("&myList                         pointer: %p \n", &myList);
	//printf("(void*)*myList                  pointer: %p \n", (*myList));

	printf("myTestList->_.class->ctor               pointer: %p \n", myTestList->_.class->ctor);
	printf("(struct Class *)myList->_.class->ctor   pointer: %p \n", (struct Class *)myList->_.class->ctor );
	printf("((struct Object *)myList)->class->ctor  pointer: %p \n", ((struct Object *)myList)->class->ctor);
	printf("myList->_.class->ctor                   pointer: %p \n", myList->_.class->ctor);
	printf("*myList->_.class->ctor                  pointer: %p \n", *myList->_.class->ctor);

	//ListClass_as_Object = (void*)ListClass;
	printf("(struct Object *)myListClass->_.ctor    pointer: %p \n\n", (struct Object *)myListClass->_.ctor);

	//myListClass = ListClass;
	struct ListClass * localSelf = (void *)myListClass;
	printf("FAIL: (voidf *)localSelf->add                   pointer: %p \n", (voidf *)localSelf->add);
	printf("FAIL: ((struct ListClass *)myListClass)->add    pointer: %p \n", ((struct ListClass *)myListClass)->add);
	printf("FAIL: *myListClass->add                         pointer: %p \n", *myListClass->add);
	printf("FAIL: ((struct ListClass *)myListClass)->add    pointer: %p \n", ((struct ListClass *)myListClass)->add);
	printf("FAIL: *((struct ListClass *)myListClass)->add   pointer: %p \n", *((struct ListClass *)myListClass)->add);
	printf("((struct ListClass *)classOf(myTestList))->add  pointer: %p \n", ((struct ListClass *)classOf(myTestList))->add);
	printf("((struct ListClass *)myList)->add               pointer: %p \n", ((struct ListClass *)myList)->add);

	printf("(*List_add)                                     pointer: %p \n", (*List_add));
	printf("add                                             pointer: %p \n", add);
	printf("(voidf *)add                                    pointer: %p \n", (voidf *)add);

	printf("(void*)myTestList->beginIndex          value: %p \n", (void*)myTestList->beginIndex);
	printf("((struct Object *)myListClass)->class->ctor     pointer: %p \n", ((struct Object *)myListClass)->class->ctor);
	printf("(struct Class *)myListClass->_.ctor    pointer: %p \n", (struct Class *)myListClass->_.ctor);
	printf("myList->_.class->ctor                  pointer: %p \n", myList->_.class->ctor);
	printf("(void *)ListClass_ctor                 pointer: %p \n", (void *)ListClass_ctor);
	printf("myList->_.class->ctor                  pointer: %p \n", myList->_.class->ctor);

	printf("(void *)List_ctor                      pointer: %p \n", (void *)List_ctor);
	printf("(void *)ctor                           pointer: %p \n", (void *)ctor);
	printf("(void *)Object_ctor                    pointer: %p \n", (void *)Object_ctor);
	printf("(void *)Class_ctor                     pointer: %p \n", (void *)Class_ctor);

	printf("&List                         pointer: %p \n", &List);
	printf("(struct Class *)List          pointer: %p \n", (struct Class *)List);
	//printf("*(struct Class *)List         pointer: %p \n", *(struct Class *)List);
	printf("(struct Object *)List          pointer: %p \n", (struct Object *)List);
	//printf("*(struct Object *)List         pointer: %p \n", *(struct Object *)List);
	printf("&ListClass                    pointer: %p \n", &ListClass);
	printf("(struct Class *)ListClass     pointer: %p \n", (struct Class *)ListClass);
	//printf("*(struct Class *)ListClass    pointer: %p \n", *(struct Class *)ListClass);
	//myListClass = ListClass;


	return;
}


void printVoidArray(void * _self)
{
	struct List * self = _self;
	int i = 0;
	printf("\nPrinting voidArray contents: ");
	for ( i = 0; i < arrayDimension(myTestList); i++ ){
		printf("%p, ", self->buf[i]);
	}
	printf("\n");
	return;
}

void printListDetails(const void * _self)
{
	/**/
	struct List * self = (void *)_self;
	if (self == NULL)              { return; } 	// failed return path
	if (self->buf == NULL)         { return; } 	// failed return path
	printf("\nPrinting List details: \n");
	printf("buf pointer: %p, arrayDimension: %i, countOfItems: %i, beginIndex: %i, endIndex: %i",
			self->buf, self->arrayDimension, self->countOfItems, self->beginIndex, self->endIndex);
	printVoidArray(self);

	return;
}

void Lists_deleteAllItems(void * _listPointer)
{
	void * localPTR;
	int deleteIndex;
	int localItemCount = getItemCount(_listPointer);
	for ( deleteIndex = 0; deleteIndex < localItemCount; deleteIndex++) {
		// WARNING:  deleting non-objects is not allowed
		localPTR = take(_listPointer);
		//printf("deleting pointer: %p\n", localPTR);
		delete(localPTR);
	}
	return;
}
