/*
 * objects-test-runner.c
 *
 *  Created on: Feb 24, 2015
 *      Author: Alan
 */


#include "..\unity\unity_fixture.h"
#include "..\unity\unity.h"

TEST_GROUP_RUNNER(objects)
{
	RUN_TEST_CASE(objects, Create);
	RUN_TEST_CASE(objects, NewReturnsNonNullClassPointer);
	RUN_TEST_CASE(objects, NewReturnsNonNullObjectPointer);
	RUN_TEST_CASE(objects, NewUsingNullClassReturnsNullPointer);
	RUN_TEST_CASE(objects, ObjectClassDescriptorNameIsObject);
	RUN_TEST_CASE(objects, CLASSClassDescriptorNameIsClass);

	RUN_TEST_CASE(objects, testObject_ctorPointsTo_Object_ctor);
	/**/

	RUN_TEST_CASE(objects, testObject_dtorPointsTo_Object_dtor);
	RUN_TEST_CASE(objects, testObject_equal_PointsTo_Object_equal);
	RUN_TEST_CASE(objects, testObject_config_PointsTo_Object_config);
	RUN_TEST_CASE(objects, testObject_putoPointsTo_Object_puto);
	RUN_TEST_CASE(objects, testClass_ctorPointsTo_Class_ctor);
	RUN_TEST_CASE(objects, testClass_dtorPointsTo_Class_dtor);

	//****  classOf  ******************
	/**/
	RUN_TEST_CASE(objects, classOf_AnobjectReturnsObject);
	RUN_TEST_CASE(objects, classOf_ObjectReturnsClass);
	RUN_TEST_CASE(objects, classOf_ClassReturnsClass);
	RUN_TEST_CASE(objects, classOf_NullReturnsNull);

	//****  sizeOf  ******************
	/**/
	RUN_TEST_CASE(objects, sizeOf_testObjectReturns4);
	RUN_TEST_CASE(objects, sizeOf_ObjectReturns40);
	RUN_TEST_CASE(objects, sizeOf_ClassReturns40);
	RUN_TEST_CASE(objects, sizeOf_NullReturns0);

	//****  Object_copy  ****************
	/**/
	RUN_TEST_CASE(objects, copy_objectEqualsOriginal);
	RUN_TEST_CASE(objects, copy_returnsNullOnNullSelf);
	RUN_TEST_CASE(objects, copy_returnsNullOnNullTarget);

	//****  Object_equal  ****************
	/**/
	RUN_TEST_CASE(objects, equal_Null_and_Null_Returns_False);
	RUN_TEST_CASE(objects, equal_Object_and_Object_Returns_True);
	RUN_TEST_CASE(objects, equal_TestObject_and_TestObject_Returns_True);
	RUN_TEST_CASE(objects, equal_ClassAndObject_Returns_False);
	RUN_TEST_CASE(objects, equal_ClassAndClass_Returns_True);
	RUN_TEST_CASE(objects, equal_TestObject_and_Object_Returns_True);

	//****  Object_config  ****************
	/**/
	RUN_TEST_CASE(objects, config_Null_or_Null_Returns_Null);
	RUN_TEST_CASE(objects, config_testObject_returnsSelf);

	//****  super  ********************
	/**/
	RUN_TEST_CASE(objects, super_testObject_Returns_Null);
	RUN_TEST_CASE(objects, super_Object_Returns_Object);
	RUN_TEST_CASE(objects, super_Class_Returns_Object);
	RUN_TEST_CASE(objects, super_Null_Returns_Null);

	//****  isOfSuper  ********************
	/**/
	RUN_TEST_CASE(objects, isOfSuper_testObject_Object_Returns_True);
	RUN_TEST_CASE(objects, isOfSuper_Object_Object_Returns_False);
	RUN_TEST_CASE(objects, isOfSuper_Class_Class_Returns_False);
	RUN_TEST_CASE(objects, isOfSuper_Class_Object_Returns_True);
	RUN_TEST_CASE(objects, isOfSuper_Null_Null_Returns_False);

	//****  delete  ******************
	/**/
	RUN_TEST_CASE(objects, delete_ofAnobjectReturnsAnobject);
	RUN_TEST_CASE(objects, delete_ofObjectReturnsNull);
	RUN_TEST_CASE(objects, delete_ofClassReturnsNull);
	RUN_TEST_CASE(objects, delete_ofNullReturnsNull);
		// test below is not supported
		//RUN_TEST_CASE(objects, delete_ofIntPtrReturnsNull);

	//****  isA  ******************
	/**/
	RUN_TEST_CASE(objects, isA_Object_testObjectReturnsTrue);
	RUN_TEST_CASE(objects, isA_Object_testClassReturnsFalse);
	RUN_TEST_CASE(objects, isA_Object_ClassReturnsFalse);
	RUN_TEST_CASE(objects, isA_Object_ObjectReturnsFalse);
	RUN_TEST_CASE(objects, isA_Object_intPtrReturnsFalse);
	RUN_TEST_CASE(objects, isA_Object_NullReturnsFalse);

	RUN_TEST_CASE(objects, isA_Class_testObjectReturnsFalse);
	RUN_TEST_CASE(objects, isA_Class_testClassReturnsTrue);
	RUN_TEST_CASE(objects, isA_Class_ClassReturnsTrue);
	RUN_TEST_CASE(objects, isA_Class_ObjectReturnsTrue);
	RUN_TEST_CASE(objects, isA_Class_intPtrReturnsFalse);
	RUN_TEST_CASE(objects, isA_Class_NullReturnsFalse);
	RUN_TEST_CASE(objects, isA_Null_ClassReturnsFalse);

	//****  isOf  ******************
	/**/
	RUN_TEST_CASE(objects, isOf_Object_testObjectReturnsTrue);
	RUN_TEST_CASE(objects, isOf_Object_testClassReturnsTrue);
	RUN_TEST_CASE(objects, isOf_Object_ClassReturnsTrue);
	RUN_TEST_CASE(objects, isOf_Object_ObjectReturnsTrue);
		//RUN_TEST_CASE(objects, isOf_Object_intPtrReturnsFalse);
	RUN_TEST_CASE(objects, isOf_Object_NullReturnsFalse);


	RUN_TEST_CASE(objects, isOf_Class_testObjectReturnsFalse);
	RUN_TEST_CASE(objects, isOf_Class_testClassReturnsTrue);

	/**/
	RUN_TEST_CASE(objects, isOf_Class_ClassReturnsTrue);
	RUN_TEST_CASE(objects, isOf_Class_ObjectReturnsTrue);
		//RUN_TEST_CASE(objects, isOf_Class_intPtrReturnsFalse);
	RUN_TEST_CASE(objects, isOf_Class_NullReturnsFalse);
	RUN_TEST_CASE(objects, isOf_Null_ClassReturnsFalse);

	//****  cast  ****************************
	/**/
	RUN_TEST_CASE(objects, cast_Object_and_testObject_Returns_testObject);
	RUN_TEST_CASE(objects, cast_Class_And_testObject_Returns_Null);
	RUN_TEST_CASE(objects, cast_Class_And_testClass_Returns_testClass);
	RUN_TEST_CASE(objects, cast_Object_And_testClass_Returns_testClass);
	RUN_TEST_CASE(objects, cast_Object_And_Class_Returns_Class);
	RUN_TEST_CASE(objects, cast_Class_And_Object_Returns_Object);
	RUN_TEST_CASE(objects, cast_Class_And_Class_Returns_Class);
	RUN_TEST_CASE(objects, cast_ObjectPlusOne_and_testObject_Returns_Null);
	RUN_TEST_CASE(objects, cast_ClassPlusOne_and_testClass_Returns_Null);
	RUN_TEST_CASE(objects, cast_Object_and_testObjectPlusOne_Returns_Null);
	RUN_TEST_CASE(objects, cast_Null_ClassReturnsNull);
	RUN_TEST_CASE(objects, cast_Class_NullReturnsNull);
	RUN_TEST_CASE(objects, cast_NullPlusOne_and_Object_Returns_Null);

	//****  puto  ****************************
	RUN_TEST_CASE(objects, puto_Execute_to_console);

	//****  small programs  ******************
	/**/
	RUN_TEST_CASE(objects, SmallProgram);
	RUN_TEST_CASE(objects, MixedBag);

}

