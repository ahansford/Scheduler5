/*
 * io-test-runner.c
 *
 *  Created on: Sep 30, 2015
 *      Author: Alan
 */



#include "..\unity\unity_fixture.h"
//#include "..\unity\unity.h"

TEST_GROUP_RUNNER(accessMem)
{
	/**************************************/
	/**************  New  *****************/

	RUN_TEST_CASE(accessMem, myTest_accessMem_IsNotNullOnCreate);

	RUN_TEST_CASE(accessMem, AccessMEM_IsNotNullOnCreate);
	RUN_TEST_CASE(accessMem, AccessMEMClass_IsNotNullOnCreate);

	RUN_TEST_CASE(accessMem, ctor_is_Access_MEM_ctor);
	RUN_TEST_CASE(accessMem, dtor_is_Access_MEM_dtor);
	RUN_TEST_CASE(accessMem, copy_is_Access_MEM_copy);
	RUN_TEST_CASE(accessMem, equal_is_Access_MEM_equal);
	RUN_TEST_CASE(accessMem, config_is_Access_MEM_config);
	RUN_TEST_CASE(accessMem, puto_is_Access_MEM_puto);

	RUN_TEST_CASE(accessMem, Class_ctor_is_Access_MEM_Class_ctor);
/**/
	RUN_TEST_CASE(accessMem, Access_addWriteCommandToSequence_is_Access_MEM_addWriteCommandToSequence);
	RUN_TEST_CASE(accessMem, Access_processSequence_is_Access_MEM_processSequence);
	//RUN_TEST_CASE(accessMem, IO_read_is_IO_io_read);


	RUN_TEST_CASE(accessMem, address_is_Null_OnCreate);
	RUN_TEST_CASE(accessMem, ioAction_is_UNKNOWN_OnCreate);
	RUN_TEST_CASE(accessMem, readCount_is_Zero_OnCreate);
	RUN_TEST_CASE(accessMem, writeCount_is_Zero_OnCreate);
	RUN_TEST_CASE(accessMem, bufferPointer_is_testBuffer_OnCreate);
	RUN_TEST_CASE(accessMem, IO_actionComplete_cb_is_Null_OnCreate);
	RUN_TEST_CASE(accessMem, objectPointer_is_Null_OnCreate);

	/****  delete/Access_dtor  ****************/
	/**/
	RUN_TEST_CASE(accessMem, delete_returns_SelfOnSuccess);
	RUN_TEST_CASE(accessMem, delete_safeDelete_returnsNullOnSuccess);


	/****  Set/Get address  ****************/
	/**/
	RUN_TEST_CASE(accessMem, Access_getAddress_returns_UnknownOnCreate);
	RUN_TEST_CASE(accessMem, Access_getAddress_returns_specificValue);
	RUN_TEST_CASE(accessMem, Access_setAddress_returnsSpecificValue);
	RUN_TEST_CASE(accessMem, Access_setAddress_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(accessMem, Access_setAddress_canSetSpecificValue);


	/****  Set/Get ioAction  ****************/
	/**/
	RUN_TEST_CASE(accessMem, Access_getIOAction_returns_UnknownOnCreate);
	RUN_TEST_CASE(accessMem, Access_getIOAction_returns_specificValue);
	RUN_TEST_CASE(accessMem, Access_setIOAction_returnsSpecificValue);
	RUN_TEST_CASE(accessMem, Access_setIOAction_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(accessMem, Access_setIOAction_canSetSpecificValue);

	/****  Set/Get readCount  ****************/
	/*
	RUN_TEST_CASE(accessMem, Access_getReadCount_returns_UnknownOnCreate);
	RUN_TEST_CASE(accessMem, Access_getReadCount_returns_specificValue);
	RUN_TEST_CASE(accessMem, Access_setReadCount_returnsSpecificValue);
	RUN_TEST_CASE(accessMem, Access_setReadCount_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(accessMem, Access_setReadCount_canSetSpecificValue);
*/
	/****  Set/Get writeCount  ****************/
	/**/
	RUN_TEST_CASE(accessMem, Access_getWriteCount_returns_UnknownOnCreate);
	RUN_TEST_CASE(accessMem, Access_getWriteCount_returns_specificValue);
	RUN_TEST_CASE(accessMem, Access_setWriteCount_returnsSpecificValue);
	RUN_TEST_CASE(accessMem, Access_setWriteCount_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(accessMem, Access_setWriteCount_canSetSpecificValue);

	/****  Set/Get bufferPointer  ****************/
	/**/
	RUN_TEST_CASE(accessMem, Access_getBufferPointer_returns_UnknownOnCreate);
	RUN_TEST_CASE(accessMem, Access_getBufferPointer_returns_specificValue);
	RUN_TEST_CASE(accessMem, Access_setBufferPointer_returnsSpecificValue);
	RUN_TEST_CASE(accessMem, Access_setBufferPointer_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(accessMem, Access_setBufferPointer_canSetSpecificValue);

	/****  Set/Get bufferSize  ****************/
	/**/
	RUN_TEST_CASE(accessMem, Access_getBufferSize_returns_DefineValueOnCreate);
	RUN_TEST_CASE(accessMem, Access_getBufferSize_returns_specificValue);
	RUN_TEST_CASE(accessMem, Access_setBufferSize_returnsSpecificValue);
	RUN_TEST_CASE(accessMem, Access_setBufferSize_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(accessMem, Access_setBufferSize_canSetSpecificValue);

	/****  Set/Get Access_actionComplete_cb  ****************/
	/**/
	RUN_TEST_CASE(accessMem, Access_getIO_actionComplete_cb_returns_UnknownOnCreate);
	RUN_TEST_CASE(accessMem, Access_getIO_actionComplete_cb_returns_specificValue);
	RUN_TEST_CASE(accessMem, Access_setIO_actionComplete_cb_returnsSpecificValue);
	RUN_TEST_CASE(accessMem, Access_setIO_actionComplete_cb_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(accessMem, Access_setIO_actionComplete_cb_canSetSpecificValue);

	/****  Set/Get objectPointer  ****************/
	/**/
	RUN_TEST_CASE(accessMem, Access_getObjectPointer_returns_UnknownOnCreate);
	RUN_TEST_CASE(accessMem, Access_getObjectPointer_returns_specificValue);
	RUN_TEST_CASE(accessMem, Access_setObjectPointer_returnsSpecificValue);
	RUN_TEST_CASE(accessMem, Access_setObjectPointer_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(accessMem, Access_setObjectPointer_canSetSpecificValue);

	//****  copy/Time_copy  ****************
	/*
	RUN_TEST_CASE(accessMem, copy_returnsSelfOnSuccess);
	RUN_TEST_CASE(accessMem, copy_AllItemsCopiedToSelf);
	RUN_TEST_CASE(accessMem, copy_returnsNullOnNullSelf);
	RUN_TEST_CASE(accessMem, copy_returnsNullOnNullMaster);
*/
	//****  equal IO_io_equal  ********************
	/*
	RUN_TEST_CASE(accessMem, myTest_accessMem_IsEqualTo_myTest_accessMem);
	RUN_TEST_CASE(accessMem, equal_UnequalAddressReturn_Unequal);
	RUN_TEST_CASE(accessMem, equal_UnequalIOActionReturn_Unequal);
	RUN_TEST_CASE(accessMem, equal_UnequalReadCountReturn_Unequal);
	RUN_TEST_CASE(accessMem, equal_UnequalWriteCountReturn_Unequal);
	RUN_TEST_CASE(accessMem, equal_UnequalBufferPointerReturn_Equal);
	RUN_TEST_CASE(accessMem, equal_UnequalBufferSizeReturn_Unequal);

	RUN_TEST_CASE(accessMem, equal_UnequalActionDoneCB_Unequal);
	RUN_TEST_CASE(accessMem, equal_UnequalObjectPointerUnequal);

	RUN_TEST_CASE(accessMem, equal_NullReturns_Null);
	RUN_TEST_CASE(accessMem, equal_CopiedSensorReturnsEqual);
*/
	//****  IO_addWriteValue  ********************
	/*
	RUN_TEST_CASE(accessMem, IO_addWriteValue_Returns_selfOnSuccess);
	RUN_TEST_CASE(accessMem, IO_addWriteValue_incrementsWriteCountOnSingleAdd);
	RUN_TEST_CASE(accessMem, IO_addWriteValue_addsSingleValue);
	RUN_TEST_CASE(accessMem, IO_addWriteValue_addsMultipleValues);
	RUN_TEST_CASE(accessMem, IO_addWriteValue_incrementsWriteCountOnMultipleAdd);
	RUN_TEST_CASE(accessMem, IO_addWriteValue_AddingMoreThanBufferSizeReturnsNULL);
*/
	//****  IO_addIOActionToList  *********************
	/*
	RUN_TEST_CASE(accessMem, IO_addIOActionToList_returnsNullOnNullPtr);
	RUN_TEST_CASE(accessMem, IO_addIOActionToList_Returns_selfOnSuccess);
	RUN_TEST_CASE(accessMem, IO_addIOActionToList_addsSingleIOObject);
	RUN_TEST_CASE(accessMem, IO_addIOActionToList_addsMultipleIOObjects);
*/
	//****  IO_getActionFromList  *********************
	/*
	RUN_TEST_CASE(accessMem, IO_getActionFromList_returnsNullOnNullPtr);
	RUN_TEST_CASE(accessMem, IO_getActionFromList_Returns_NullOnemptyList);
	RUN_TEST_CASE(accessMem, IO_getActionFromList_Returns_ioObjectFromList);
	RUN_TEST_CASE(accessMem, IO_getActionFromList_Returns_MultipleioObjectsFromList);
*/
	//****  IO_processSequence  ****************
	/*
	//RUN_TEST_CASE(io, IO_processSequence_Returns_selfOnSuccess);
	//RUN_TEST_CASE(io, IO_processSequence_writeSingleToSingleAddress);
	//RUN_TEST_CASE(io, IO_processSequence_writeMultipleValuesToSingleaddress);
	//RUN_TEST_CASE(io, IO_processSequence_writeMultipleValuesToSequentialLocation);

	//RUN_TEST_CASE(io, IO_processSequence_readSingleValue);
	//RUN_TEST_CASE(io, IO_processSequence_readSequentialMultipleValues);
	//RUN_TEST_CASE(io, IO_processSequence_readMultipleValuesfromSingleLocation);
	 */

	//****  IO_update  *********************
	/*
	//RUN_TEST_CASE(io, IO_update_writeSingleToSingleAddress);
	//RUN_TEST_CASE(io, IO_update_writeMultipleValuesToSingleAddress);
	//RUN_TEST_CASE(io, IO_update_writeMultipleValuesToSequentialLocation);

	//RUN_TEST_CASE(io, IO_update_readSingleValue);
	//RUN_TEST_CASE(io, IO_update_readSequentialMultipleValues);
	//RUN_TEST_CASE(io, IO_update_readMultipleValuesfromSingleLocation);

	//RUN_TEST_CASE(io, IO_update_firesCallback);
	//RUN_TEST_CASE(io, IO_update_canBeCalledMultipleTimesWithEmplyList);
	 */
	RUN_TEST_CASE(accessMem, Access_sequenceComplete_cb_firesDesignatedCallback);
	RUN_TEST_CASE(accessMem, Access_sequenceComplete_cb_sendsCorrectPtr);

}
