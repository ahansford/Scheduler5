/*
 * objects-test.c
 *
 *  Created on: Feb 24, 2015
 *      Author: Alan
 */

#include <stdio.h>
#include <stddef.h>
#include "..\..\src\cross_compiler_defs.h"
#include "..\unity\unity_fixture.h"
//#include "..\unity\unity.h"
#include "..\..\src\mocks\RuntimeErrorStub.h"

#include "..\..\src\objects\objects.h"
#include "..\..\src\objects\objects-private.h"

/****************************************/
/*** Basic Object and Class Testing ****/

struct Object * testObject = NULL;
struct Class  * testClass  = NULL;

int x = 5;
int * intPtr = &x;

/****************************************/
/*********** Start of Testing ***********/
TEST_GROUP(objects);

TEST_SETUP(objects)
{
	RuntimeErrorStub_Reset();
	testObject = new(Object);


/*
	testClass = new(Class, 			// class descriptor
				   "testClass", 	// class name
				   Object,			// superclass
				   sizeOf(Class),	// class size
									// overloaded functions here
				   ctor, Class_ctor,
				   dtor, Class_dtor,
				   0);				// end marker
	//testClass = Class;
*/
}

TEST_TEAR_DOWN(objects)
{
	testObject = safeDelete(testObject);
	RuntimeErrorStub_Reset();
}

TEST(objects, Create)
{
}

TEST(objects, NewReturnsNonNullClassPointer)
{
	TEST_ASSERT_NOT_EQUAL(NULL, Class);
}

TEST(objects, NewReturnsNonNullObjectPointer)
{
	TEST_ASSERT_NOT_EQUAL(NULL, Object);
}


TEST(objects, NewUsingNullClassReturnsNullPointer)
{
	TEST_ASSERT_EQUAL_PTR(NULL, new(NULL));
}

TEST(objects, classOf_PointsToClass)
{
	TEST_ASSERT_EQUAL_PTR(classOf(Object), Class);
	TEST_ASSERT_EQUAL_PTR(classOf(Class), Class);
}

TEST(objects, ObjectClassDescriptorNameIsObject)
{
	TEST_ASSERT_EQUAL_STRING("Object", className(Object));
}

TEST(objects, CLASSClassDescriptorNameIsClass)
{
	TEST_ASSERT_EQUAL_STRING("Class", className(Class));
}

TEST(objects, testObject_ctorPointsTo_Object_ctor)
{
	TEST_ASSERT_EQUAL_PTR(Object_ctor, testObject->class->ctor);
}

TEST(objects, testObject_dtorPointsTo_Object_dtor)
{
	TEST_ASSERT_EQUAL_PTR(Object_dtor, testObject->class->dtor);
}

TEST(objects, testObject_equal_PointsTo_Object_equal)
{
	TEST_ASSERT_EQUAL_PTR(Object_equal, testObject->class->equal);
}

TEST(objects, testObject_config_PointsTo_Object_config)
{
	TEST_ASSERT_EQUAL_PTR(Object_config, testObject->class->config);
}

TEST(objects, testObject_putoPointsTo_Object_puto)
{
	TEST_ASSERT_EQUAL_PTR(Object_puto, testObject->class->puto);
}

TEST(objects, testClass_ctorPointsTo_Class_ctor)
{
	struct Class * myTest_Class = (void *)Class;
	TEST_ASSERT_EQUAL_PTR(Class_ctor, myTest_Class->ctor);
}

TEST(objects, testClass_dtorPointsTo_Class_dtor)
{
	struct Class * myTest_Class = (void *)Class;
	TEST_ASSERT_EQUAL_PTR(Class_dtor, myTest_Class->dtor);
}

//****  classOf  ******************
TEST(objects, classOf_AnobjectReturnsObject)
{
	TEST_ASSERT_EQUAL_PTR(Object, classOf(testObject));
}

TEST(objects, classOf_ObjectReturnsClass)
{
	TEST_ASSERT_EQUAL_PTR(Class, classOf(Object));
}

TEST(objects, classOf_ClassReturnsClass)
{
	TEST_ASSERT_EQUAL_PTR(Class, classOf(Class));
}

TEST(objects, classOf_NullReturnsNull)
{
	TEST_ASSERT_EQUAL_PTR(NULL, classOf(NULL));
}



//****  sizeOf  ******************

TEST(objects, sizeOf_testObjectReturns4)
{
	TEST_ASSERT_EQUAL(4, sizeOf(testObject));
}

TEST(objects, sizeOf_ObjectReturns40)
{
	TEST_ASSERT_EQUAL(40, sizeOf(Object));
}

TEST(objects, sizeOf_ClassReturns40)
{
	TEST_ASSERT_EQUAL(40, sizeOf(Class));
}

TEST(objects, sizeOf_NullReturns0)
{
	TEST_ASSERT_EQUAL(0, sizeOf(NULL));
}

//****  Object_copy  ********************
/**/
TEST(objects, copy_objectEqualsOriginal)
{
	struct Object * copyFromObject = new(Object);
	TEST_ASSERT_EQUAL(testObject, copy(testObject, copyFromObject));
	copyFromObject = safeDelete(copyFromObject);
}

TEST(objects, copy_returnsNullOnNullSelf)
{
	struct Object * copyFromObject = new(Object);
	TEST_ASSERT_EQUAL(NULL, copy(NULL, copyFromObject));
	copyFromObject = safeDelete(copyFromObject);
}

TEST(objects, copy_returnsNullOnNullTarget)
{
	TEST_ASSERT_EQUAL(NULL, copy(testObject, NULL));
}

//****  Object_equal  ********************
/**/
TEST(objects, equal_Null_and_Null_Returns_False)
{
	TEST_ASSERT_EQUAL(FALSE, equal(NULL, NULL));
}

TEST(objects, equal_Object_and_Object_Returns_True)
{
	TEST_ASSERT_EQUAL(TRUE, equal(Object, Object));
}

TEST(objects, equal_TestObject_and_TestObject_Returns_True)
{
	TEST_ASSERT_EQUAL(TRUE, equal(testObject, testObject));
}

TEST(objects, equal_ClassAndObject_Returns_False)
{
	TEST_ASSERT_EQUAL(FALSE, equal(Class,  Object));
	TEST_ASSERT_EQUAL(FALSE, equal(Object, Class ));
}

TEST(objects, equal_ClassAndClass_Returns_True)
{
	TEST_ASSERT_EQUAL(TRUE, equal(Class, Class));
}

TEST(objects, equal_TestObject_and_Object_Returns_True)
{
	TEST_ASSERT_EQUAL(TRUE, equal(testObject, Object));
}

//****  Object_config  ****************
/**/
TEST(objects, config_Null_or_Null_Returns_Null)
{
	struct Object * master = new(Object);
	TEST_ASSERT_EQUAL(NULL, config(NULL, master));
	TEST_ASSERT_EQUAL(NULL, config(testObject, NULL));
	master = safeDelete(master);
}

TEST(objects, config_testObject_returnsSelf)
{
	struct Object * master = new(Object);
	TEST_ASSERT_EQUAL(testObject, config(testObject, master) );
	TEST_ASSERT_EQUAL(Object,     config(Object,     master) );
	TEST_ASSERT_EQUAL(Class,      config(Class,      master) );
	master = safeDelete(master);
}

//****  super  ********************

IGNORE_TEST(objects, super_testObject_Returns_Null)
{
	TEST_ASSERT_EQUAL_PTR(NULL, super(testObject));
}

TEST(objects, super_Object_Returns_Object)
{
	TEST_ASSERT_EQUAL_PTR(Object, super(Object));
}

TEST(objects, super_Class_Returns_Object)
{
	TEST_ASSERT_EQUAL_PTR(Object, super(Class));
}

TEST(objects, super_Null_Returns_Null)
{
	TEST_ASSERT_EQUAL_PTR(NULL, super(NULL));
}

//****  isOfSuper  ********************

IGNORE_TEST(objects, isOfSuper_testObject_Object_Returns_True)
{
	TEST_ASSERT_EQUAL_PTR(Object, super(testObject));
}

TEST(objects, isOfSuper_Object_Object_Returns_False)
{
	TEST_ASSERT_EQUAL(FALSE, isOfSuper(Object, Object));
}

TEST(objects, isOfSuper_Class_Class_Returns_False)
{
	TEST_ASSERT_EQUAL(FALSE, isOfSuper(Class, Class));
}

TEST(objects, isOfSuper_Class_Object_Returns_True)
{
	TEST_ASSERT_EQUAL(TRUE, isOfSuper(Class, Object));
}

TEST(objects, isOfSuper_Null_Null_Returns_False)
{
	TEST_ASSERT_EQUAL(FALSE, isOfSuper(NULL, NULL));
}

//****  delete  ******************

TEST(objects, delete_ofAnobjectReturnsAnobject)
{
	void * anObject = new(Object);
	TEST_ASSERT_EQUAL_PTR(anObject, delete(anObject) );
	anObject = NULL;
}

TEST(objects, delete_ofObjectReturnsNull)
{
	TEST_ASSERT_EQUAL_PTR(NULL, delete((void *)Object) );
}

TEST(objects, delete_ofClassReturnsNull)
{
	TEST_ASSERT_EQUAL_PTR(NULL, delete((void *)Class) );
}

TEST(objects, delete_ofNullReturnsNull)
{
	TEST_ASSERT_EQUAL_PTR(NULL, delete(NULL) );
}

// test below is not supported
IGNORE_TEST(objects, delete_ofIntPtrReturnsNull)
{
	int a;
	int * localIntPtr;
	a = 7;
	localIntPtr = &a;
	TEST_ASSERT_EQUAL_PTR( NULL, delete(localIntPtr) );
}


//****  isA  ******************
TEST(objects, isA_Object_testObjectReturnsTrue)
{
	TEST_ASSERT_EQUAL( Object, classOf(testObject) );
	TEST_ASSERT_EQUAL( 1, isA_sc(testObject, Object) );
}

TEST(objects, isA_Object_testClassReturnsFalse)
{
	struct Class * myTest_Class = (void *)Class;
	TEST_ASSERT_EQUAL( Class, classOf(myTest_Class) );
	TEST_ASSERT_EQUAL( 0, isA_sc(myTest_Class, Object) );
}

TEST(objects, isA_Object_ClassReturnsFalse)
{
	TEST_ASSERT_EQUAL( 0, isA_sc(Class, Object) );
}

TEST(objects, isA_Object_ObjectReturnsFalse)
{
	TEST_ASSERT_EQUAL( Class, classOf(Object) );
	TEST_ASSERT_EQUAL( 0, isA_sc(Object, Object) );
}

TEST(objects, isA_Object_intPtrReturnsFalse)
{
	TEST_ASSERT_EQUAL( 0, isA_sc(intPtr, Object) );
}

TEST(objects, isA_Object_NullReturnsFalse)
{
	TEST_ASSERT_EQUAL( NULL, classOf(NULL) );
	TEST_ASSERT_EQUAL( 0, isA_sc(NULL, Object) );
}

        //++++++++++++++++++++++//

TEST(objects, isA_Class_testObjectReturnsFalse)
{
	TEST_ASSERT_EQUAL( 0, isA_sc(testObject, Class) );
}

TEST(objects, isA_Class_testClassReturnsTrue)
{
	struct Class * myTest_Class = (void *)Class;
	TEST_ASSERT_EQUAL( 1, isA_sc(myTest_Class, Class) );
}

TEST(objects, isA_Class_ClassReturnsTrue)
{
	TEST_ASSERT_EQUAL( 1, isA_sc(Class, Class) );
}

TEST(objects, isA_Class_ObjectReturnsTrue)
{
	TEST_ASSERT_EQUAL( 1, isA_sc(Object, Class) );
}

TEST(objects, isA_Class_intPtrReturnsFalse)
{
	TEST_ASSERT_EQUAL( 0, isA_sc(intPtr, Class) );
}

TEST(objects, isA_Class_NullReturnsFalse)
{
	TEST_ASSERT_EQUAL( 0, isA_sc(NULL, Class) );
}

TEST(objects, isA_Null_ClassReturnsFalse)
{
	TEST_ASSERT_EQUAL( 0, isA_sc(Class, NULL) );
}


//****  isOf  ******************
TEST(objects, isOf_Object_testObjectReturnsTrue)
{
	TEST_ASSERT_EQUAL( Object, classOf(testObject) );
	TEST_ASSERT_EQUAL( 1, isOf_sc(testObject, Object) );
}

TEST(objects, isOf_Object_testClassReturnsTrue)
{
	struct Class * myTest_Class = (void *)Class;
	TEST_ASSERT_EQUAL( Class, classOf(myTest_Class) );
	TEST_ASSERT_EQUAL( 1, isOf_sc(myTest_Class, Object) );
}

TEST(objects, isOf_Object_ClassReturnsTrue)
{
	TEST_ASSERT_EQUAL( Class, classOf(Class) );
	TEST_ASSERT_EQUAL( Object, super(Class) );
	TEST_ASSERT_EQUAL( 1, isOf_sc(Class, Object) );
}

TEST(objects, isOf_Object_ObjectReturnsTrue)
{
	TEST_ASSERT_EQUAL( Class, classOf(Object) );
	TEST_ASSERT_EQUAL( Object, super(Class) );
	TEST_ASSERT_EQUAL( 1, isOf_sc(Object, Object) );
}

TEST(objects, isOf_Object_intPtrReturnsFalse)
{
	TEST_ASSERT_EQUAL( 5, *intPtr);
	TEST_ASSERT_EQUAL( NULL, super(intPtr));
	TEST_ASSERT_EQUAL( 0, isOf_sc(intPtr, Object) );
}

TEST(objects, isOf_Object_NullReturnsFalse)
{
	TEST_ASSERT_EQUAL( NULL, classOf(NULL) );
	TEST_ASSERT_EQUAL( 0, isOf_sc(NULL, Object) );
}

        //++++++++++++++++++++++//

TEST(objects, isOf_Class_testObjectReturnsFalse)
{
	TEST_ASSERT_EQUAL( 0, isOf_sc(testObject, Class) );
}

TEST(objects, isOf_Class_testClassReturnsTrue)
{
	struct Class * myTest_Class = (void *)Class;
	TEST_ASSERT_EQUAL( 1, isOf_sc(myTest_Class, Class) );
}

TEST(objects, isOf_Class_ClassReturnsTrue)
{
	TEST_ASSERT_EQUAL( Object, super(Class) );
	TEST_ASSERT_EQUAL( 1, isOf_sc(Class, Class) );
}

TEST(objects, isOf_Class_ObjectReturnsTrue)
{
	TEST_ASSERT_EQUAL( 1, isOf_sc(Object, Class) );
}

TEST(objects, isOf_Class_intPtrReturnsFalse)
{
	TEST_ASSERT_EQUAL( 5, *intPtr);
	TEST_ASSERT_EQUAL( NULL, super(intPtr));
	TEST_ASSERT_EQUAL( 0, isOf_sc(intPtr, Class) );
}

TEST(objects, isOf_Class_NullReturnsFalse)
{
	TEST_ASSERT_EQUAL( 0, isOf_sc(NULL, Class) );
}

TEST(objects, isOf_Null_ClassReturnsFalse)
{
	TEST_ASSERT_EQUAL( 0, isOf_sc(Class, NULL) );
}


//****  cast  ******************

TEST(objects, cast_Object_and_testObject_Returns_testObject)
{
	TEST_ASSERT_EQUAL(1, isOf_sc(testObject, Object));
	TEST_ASSERT_EQUAL(testObject, cast(Object, testObject));
}

TEST(objects, cast_Class_And_testObject_Returns_Null)
{
	TEST_ASSERT_EQUAL(NULL, cast(Class, testObject));
}

TEST(objects, cast_Class_And_testClass_Returns_testClass)
{
	struct Class * myTest_Class = (void *)Class;
	TEST_ASSERT_EQUAL(1, isOf_sc(myTest_Class, Class));
	TEST_ASSERT_EQUAL(myTest_Class, cast(Class, myTest_Class));
}

TEST(objects, cast_Object_And_testClass_Returns_testClass)
{
	struct Class * myTest_Class = (void *)Class;
	TEST_ASSERT_EQUAL(myTest_Class, cast(Object, myTest_Class));
}

TEST(objects, cast_Object_And_Class_Returns_Class)
{
	TEST_ASSERT_EQUAL(Class, cast(Object, Class));
}

TEST(objects, cast_Class_And_Object_Returns_Object)
{
	TEST_ASSERT_EQUAL(Object, cast(Class, Object));
}

TEST(objects, cast_Class_And_Class_Returns_Class)
{
	TEST_ASSERT_EQUAL(Class, cast(Class, Class));
}

TEST(objects, cast_ObjectPlusOne_and_testObject_Returns_Null)
{
	struct Class * myTest_Class = (void *)Class;
	TEST_ASSERT_EQUAL(NULL, cast(Object+1, myTest_Class));
	TEST_ASSERT_EQUAL(NULL, cast(Object+1, testObject));
}

TEST(objects, cast_ClassPlusOne_and_testClass_Returns_Null)
{
	struct Class * myTest_Class = (void *)Class;
	TEST_ASSERT_EQUAL(NULL, cast(Class+1, myTest_Class));
}

TEST(objects, cast_Object_and_testObjectPlusOne_Returns_Null)
{
	//WARNING:  This test will fail due to lack of checking
	//          for valid object ... implement classRegistry
	// TODO:  need a trap for non-object pointers
	//TEST_ASSERT_EQUAL(NULL, cast(Object, testObject+1) );
}

TEST(objects, cast_Null_ClassReturnsNull)
{
	TEST_ASSERT_EQUAL(NULL, cast(Class, NULL));
}

TEST(objects, cast_Class_NullReturnsNull)
{
	TEST_ASSERT_EQUAL(NULL, cast(NULL, Class));
}

TEST(objects, cast_NullPlusOne_and_Object_Returns_Null)
{
	TEST_ASSERT_EQUAL(NULL, cast(NULL+1, Object));
}

//****  puto  ****************************

TEST(objects, puto_Execute_to_console)
{
	char outputFilename[] = "object_puto.txt";
	FILE * fp = fopen(outputFilename, "a");

	fprintf(fp, "\nPRINTING: testObject in %s at line: %u\n", __FILE__, __LINE__);
	puto(testObject,  fp);

	fprintf(fp, "\nPRINTING: Object in %s at line: %u\n", __FILE__, __LINE__);
	puto(Object,  fp);

	fprintf(fp, "\nPRINTING: Class in %s at line: %u\n", __FILE__, __LINE__);
	puto(Class,  fp);

	fclose(fp);
}




//**************************************//
//****  small programs  ******************
TEST(objects, SmallProgram)
{
	void * testObject = new(Object);

	void * anyObject = new(Object);
	TEST_ASSERT_EQUAL(1, equal(testObject, testObject));
	TEST_ASSERT_EQUAL(1, equal(anyObject, anyObject));
	TEST_ASSERT_EQUAL(1, equal(testObject, anyObject));

	TEST_ASSERT_EQUAL(equal(anyObject, testObject), equal(testObject, anyObject));

	delete(anyObject);
	delete(testObject);
}

TEST(objects, MixedBag)
{
	struct Object * newObject = NULL;
	struct Class  * newClass = (void *)Class;

	newObject = new(Object);

	//struct Object * ObjectPtr = (struct Object *) Object;
	//struct Class  * ClassPtr = (struct Class  *) Class;

	TEST_ASSERT_NOT_EQUAL(NULL, newObject);
	TEST_ASSERT_NOT_EQUAL(NULL, newClass);
/*
	Object_test_printf(newObject);
	Object_test_printf(newClass);
	Object_test_printf(Object);
	Object_test_printf(ClassPtr);
*/
	TEST_ASSERT_EQUAL_STRING( "Class" , newClass->className );
	TEST_ASSERT_EQUAL( Object , newClass->superclass );
	TEST_ASSERT_EQUAL( (10*4) , newClass->sizeOfThisClass );
	//TEST_ASSERT_EQUAL( Object_ctor , ObjectPtr->class->ctor );
	//TEST_ASSERT_EQUAL( dtor , newClass->dtor );

	delete(newObject);
	//delete(newClass);
}

