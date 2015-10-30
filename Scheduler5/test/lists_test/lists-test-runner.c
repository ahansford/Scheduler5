/*
 * list-test-runner.c
 *
 *  Created on: Mar 13, 2015
 *      Author: Alan
 */

#include "..\unity\unity_fixture.h"
#include "..\unity\unity.h"

TEST_GROUP_RUNNER(lists_init)
{
	/**/
	RUN_TEST_CASE(lists_init, ListClassNotNullAfterInit);
	RUN_TEST_CASE(lists_init, ListNotNullAfterInit);
	RUN_TEST_CASE(lists_init, MultipleCallsToInitDoNotChangePtr);

	/**/
	RUN_TEST_CASE(lists_init, New_createsAnEmptyList);

	RUN_TEST_CASE(lists_init, clearListObjectValues_returns_MyList);
	RUN_TEST_CASE(lists_init, clearListObjectValues_ZerosAndNulls);

	/*****  delete list  *****************/
	/**/
	RUN_TEST_CASE(lists_init, delete_Returns_myTestList_OnEmptyList);
	RUN_TEST_CASE(lists_init, delete_Returns_myTestList_OnSingleItem);
	RUN_TEST_CASE(lists_init, delete_Returns_myTestList_OnMultiItemList);

}


TEST_GROUP_RUNNER(lists)
{

	/*********  Setup List ***************/
	/**/
	RUN_TEST_CASE(lists, Create);
	RUN_TEST_CASE(lists, Delete_ReturnsMyTestList);

	// Test below fails without free() in delete
	// Test below requires myTestList be set to NULL after 1st delete
	RUN_TEST_CASE(lists, Delete_SecondDeleteReturnsNull);

	/**/
	RUN_TEST_CASE(lists, clearListObjectValues_ClearsLocalData);
	RUN_TEST_CASE(lists, new_buf_PointsTo_array);
	RUN_TEST_CASE(lists, new_arrayDimension_reports_MIN_LIST_BUFFER_SIZE);
	RUN_TEST_CASE(lists, new_BeginEndCount_report_zero);
	RUN_TEST_CASE(lists, init_List_dtor_Returns_Null_on_NullPtr);
	RUN_TEST_CASE(lists, init_List_dtor_Returns_self_on_myTestList);

	/*********  ListClass  ****************/
	/**/
	RUN_TEST_CASE(lists, init_ListClass_Returns_NonNull);
	RUN_TEST_CASE(lists, init_ListClass_Equals_myListClass);
	RUN_TEST_CASE(lists, init_ListClass_NameIs_ListClass);
	RUN_TEST_CASE(lists, init_ListClass_ctor_Returns_ListClass_ctor);
	RUN_TEST_CASE(lists, init_ListClass_super_Returns_Class);
	RUN_TEST_CASE(lists, init_ListClass_SizeOfClass_List_Returns_64);
	RUN_TEST_CASE(lists, init_ListClass_add_Returns_List_add);

	/*********  List  ****************/
	/**/
	RUN_TEST_CASE(lists, init_List_Returns_NonNull);
	RUN_TEST_CASE(lists, init_List_NameIs_List);
	RUN_TEST_CASE(lists, init_List_ctor_Returns_List_ctor);
	RUN_TEST_CASE(lists, init_List_dtor_Returns_List_dtor);

	RUN_TEST_CASE(lists, init_List_class_Returns_ListClass);
	RUN_TEST_CASE(lists, init_List_Super_Returns_Object);
	RUN_TEST_CASE(lists, init_List_SizeOfObject_List_Returns_32);
	RUN_TEST_CASE(lists, init_List_size_is_32);

	// *** Testing NEW method pointers ********************//
	/**/
	RUN_TEST_CASE(lists, init_ListClass_add);
	RUN_TEST_CASE(lists, init_ListClass_take);
	RUN_TEST_CASE(lists, init_ListClass_addFirst);
	RUN_TEST_CASE(lists, init_ListClass_addLast);
	RUN_TEST_CASE(lists, init_ListClass_takeFirst);
	RUN_TEST_CASE(lists, init_ListClass_takeLast);

	// *** Testing New method behaviors ********************//
	/**/
	RUN_TEST_CASE(lists, List_add_ReturnsElementOnSuccess);
	RUN_TEST_CASE(lists, List_add_ReturnsNullOnNullList);

	RUN_TEST_CASE(lists, List_take_ReturnsItemOnSuccess);
	RUN_TEST_CASE(lists, List_take_ReturnsNullOnNullList);
	RUN_TEST_CASE(lists, List_take_ReturnsNullOnNoItem);

	/**********************************/

	RUN_TEST_CASE(lists, add_ReturnsElementOnSuccess);
	RUN_TEST_CASE(lists, add_ReturnsNullOnNullList);
	RUN_TEST_CASE(lists, add_Returns_Null_For_NonValid_List_Ptr);


	RUN_TEST_CASE(lists, take_ReturnsItemOnSuccess);
	RUN_TEST_CASE(lists, take_ReturnsNullOnNullList);
	RUN_TEST_CASE(lists, take_ReturnsNullOnNoItem);
/**/
	RUN_TEST_CASE(lists, contains_Returns_zero_for_Null_list);
	RUN_TEST_CASE(lists, contains_Returns_one_for_item_found);
	RUN_TEST_CASE(lists, contains_does_not_reorder_list);

	/**********************************/

	RUN_TEST_CASE(lists, List_addLast_CountAndEndIncrement_OnAdd);
	RUN_TEST_CASE(lists, List_addLast_ReturnsElement_OnAdd);
	// WARNING:  Test below has limited meaning with extensible array
	RUN_TEST_CASE(lists, List_addLast_CanAddMaxItems);
	RUN_TEST_CASE(lists, List_addLast_CanAddFourItems);
	RUN_TEST_CASE(lists, List_addLast_ArrayExtendsOnAddMax);

	RUN_TEST_CASE(lists, List_addFirst_CountAndEndIncrement_OnAdd);
	RUN_TEST_CASE(lists, List_addFirst_ReturnsElement_OnAdd);
	RUN_TEST_CASE(lists, List_addFirst_CanAddFourItems);
	RUN_TEST_CASE(lists, List_addFirst_ArrayExtendsOnAddMax);

	RUN_TEST_CASE(lists, List_takeLast_ReturnsElements_OnTake);
	RUN_TEST_CASE(lists, List_takeLast_ReturnsNull_OnEmpty);
	RUN_TEST_CASE(lists, List_takeLast_ReturnsNull_OnNullList);
	RUN_TEST_CASE(lists, List_takeLast_ValueAtTakenIndexOverWrittenWithNull);
	RUN_TEST_CASE(lists, List_takeLast_AllValuesTakenAreOverwrittenWithNull);
/**/
	RUN_TEST_CASE(lists, List_takeFirst_ReturnsElements_OnTake);
	RUN_TEST_CASE(lists, List_takeFirst_ReturnsNull_OnEmpty);
	RUN_TEST_CASE(lists, List_takeFirst_ReturnsNull_OnNullList);
	RUN_TEST_CASE(lists, List_takeFirst_ValueAtTakenIndexOverWrittenWithNull);


	/****  isOfSuper_sc  ****************************/
	/**/
	RUN_TEST_CASE(lists, isOfSuper_sc_ReturnsNullForSameClass);
	RUN_TEST_CASE(lists, isOfSuper_sc_ReturnsNullForSubClass);
	RUN_TEST_CASE(lists, isOfSuper_sc_ReturnsTrueForActualSuperClassRelationship);


	/********  extendArray with addLast  *************/
	/**/
	RUN_TEST_CASE(lists, extentArrayAddLastWrapsBeginToFour);
	RUN_TEST_CASE(lists, secondExtentArrayAddLastWrapsBeginToTwelve);
	RUN_TEST_CASE(lists, extentArrayAddLastWrapsEndToZero);
	RUN_TEST_CASE(lists, secondExtentArrayAddLastWrapsEndToFour);

	/********  extendArray with addFirst  *************/
	/**/
	RUN_TEST_CASE(lists, extentArrayAddFirstWrapsBeginToFour);
	RUN_TEST_CASE(lists, secondExtentArrayAddFirstWrapsBeginToEight);
	RUN_TEST_CASE(lists, extentArrayAddFirstWrapsEndToFour);
	RUN_TEST_CASE(lists, secondExtentArrayAddFirstWrapsEndToTwelve);

	/********  Reduce Array  *************/
	RUN_TEST_CASE(lists, reduceArrayCutsExtendedArrayInHalf);

	//****  List_puto  ****************************
	RUN_TEST_CASE(lists, List_puto_Execute_to_console);

	//****  Rotate list contents test  *********************
		// These are console inspection tests
		//RUN_TEST_CASE(lists, Rotate_list_up);
		//RUN_TEST_CASE(lists, Rotate_list_down);

	//****  List contains test  ****************************
	/**/
	RUN_TEST_CASE(lists, List_contains_test);
	RUN_TEST_CASE(lists, List_contains_Class);

	//****  List resetIndex test  **************************
	/**/
	RUN_TEST_CASE(lists, resetIndex_returnsNullOnNullList);
	RUN_TEST_CASE(lists, resetIndex_returnsNullOnEmptyList);
	RUN_TEST_CASE(lists, resetIndex_returnsFirstItemInTheList);
	RUN_TEST_CASE(lists, resetIndex_returnsFirstAfterMoreItemsAdded);


	//****  List getNext test  **************************
	/**/
	RUN_TEST_CASE(lists, getNext_returnsNullOnNullList);
	RUN_TEST_CASE(lists, getNext_returnsNullOnEmptyList);
	RUN_TEST_CASE(lists, getNext_returnsFirstItemInSingleItemList);
	RUN_TEST_CASE(lists, getNext_returnsSecondItemOfTwoItemList);
	RUN_TEST_CASE(lists, getNext_LoopsThroughMultiItemList);
	RUN_TEST_CASE(lists, getNext_LoopsThroughWrappedList);


	//****  List copy test  **************************
	/**/
	RUN_TEST_CASE(lists, copy_returnsNullOnNullPointer);
	RUN_TEST_CASE(lists, copy_returnsSelfOnSuccess);
	RUN_TEST_CASE(lists, copy_returnsCopiesCorrectData);

	//****  List equal test  **************************
	/**/
	RUN_TEST_CASE(lists, equal_NullComparisonsReturnUnequal);
	RUN_TEST_CASE(lists, equal_EmptyListsReturnUnequal);
	RUN_TEST_CASE(lists, equal_CopiedListsReturnEqual);


	//RUN_TEST_CASE(lists, memory_leak);
}
