/*
 * io-test-runner.c
 *
 *  Created on: Sep 30, 2015
 *      Author: Alan
 */



#include "..\unity\unity_fixture.h"
//#include "..\unity\unity.h"

TEST_GROUP_RUNNER(io)
{
	/**************************************/
	/**************  New  *****************/
	/**/
	RUN_TEST_CASE(io, myTest_io_IsNotNullOnCreate);
	RUN_TEST_CASE(io, IO_IsNotNullOnCreate);
	RUN_TEST_CASE(io, IOClass_IsNotNullOnCreate);

	RUN_TEST_CASE(io, ctor_is_IO_io_ctor);
	RUN_TEST_CASE(io, dtor_is_IO_io_dtor);
	RUN_TEST_CASE(io, copy_is_IO_io_copy);
	RUN_TEST_CASE(io, equal_is_IO_io_equal);
	RUN_TEST_CASE(io, config_is_IO_io_config);
	RUN_TEST_CASE(io, puto_is_IO_io_puto);

	RUN_TEST_CASE(io, Class_ctor_is_IO_Class_ctor);

	RUN_TEST_CASE(io, IO_write_is_IO_io_write);
	RUN_TEST_CASE(io, IO_writeRead_is_IO_io_writeRead);
	RUN_TEST_CASE(io, IO_read_is_IO_io_read);

	RUN_TEST_CASE(io, address_is_Null_OnCreate);
	RUN_TEST_CASE(io, ioAction_is_UNKNOWN_OnCreate);
	RUN_TEST_CASE(io, readCount_is_Zero_OnCreate);
	RUN_TEST_CASE(io, writeCount_is_Zero_OnCreate);
	RUN_TEST_CASE(io, bufferPointer_is_testBuffer_OnCreate);
	RUN_TEST_CASE(io, IO_actionComplete_cb_is_Null_OnCreate);
	RUN_TEST_CASE(io, objectPointer_is_Null_OnCreate);

	/****  delete/Sensor_dtor  ****************/
	/**/
	RUN_TEST_CASE(sensor, delete_returns_SelfOnSuccess);
	RUN_TEST_CASE(sensor, delete_safeDelete_returnsNullOnSuccess);


	/****  Set/Get address  ****************/
	/**/
	RUN_TEST_CASE(io, IO_getAddress_returns_UnknownOnCreate);
	RUN_TEST_CASE(io, IO_getAddress_returns_specificValue);
	RUN_TEST_CASE(io, IO_setAddress_returnsSpecificValue);
	RUN_TEST_CASE(io, IO_setAddress_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(io, IO_setAddress_canSetSpecificValue);


	/****  Set/Get ioAction  ****************/
	/**/
	RUN_TEST_CASE(io, IO_getIOAction_returns_UnknownOnCreate);
	RUN_TEST_CASE(io, IO_getIOAction_returns_specificValue);
	RUN_TEST_CASE(io, IO_setIOAction_returnsSpecificValue);
	RUN_TEST_CASE(io, IO_setIOAction_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(io, IO_setIOAction_canSetSpecificValue);

	/****  Set/Get readCount  ****************/
	/**/
	RUN_TEST_CASE(io, IO_getReadCount_returns_UnknownOnCreate);
	RUN_TEST_CASE(io, IO_getReadCount_returns_specificValue);
	RUN_TEST_CASE(io, IO_setReadCount_returnsSpecificValue);
	RUN_TEST_CASE(io, IO_setReadCount_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(io, IO_setReadCount_canSetSpecificValue);

	/****  Set/Get writeCount  ****************/
	/**/
	RUN_TEST_CASE(io, IO_getWriteCount_returns_UnknownOnCreate);
	RUN_TEST_CASE(io, IO_getWriteCount_returns_specificValue);
	RUN_TEST_CASE(io, IO_setWriteCount_returnsSpecificValue);
	RUN_TEST_CASE(io, IO_setWriteCount_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(io, IO_setWriteCount_canSetSpecificValue);

	/****  Set/Get bufferPointer  ****************/
	/**/
	RUN_TEST_CASE(io, IO_getBufferPointer_returns_UnknownOnCreate);
	RUN_TEST_CASE(io, IO_getBufferPointer_returns_specificValue);
	RUN_TEST_CASE(io, IO_setBufferPointer_returnsSpecificValue);
	RUN_TEST_CASE(io, IO_setBufferPointer_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(io, IO_setBufferPointer_canSetSpecificValue);

	/****  Set/Get IO_actionComplete_cb  ****************/
	/**/
	RUN_TEST_CASE(io, IO_getIO_actionComplete_cb_returns_UnknownOnCreate);
	RUN_TEST_CASE(io, IO_getIO_actionComplete_cb_returns_specificValue);
	RUN_TEST_CASE(io, IO_setIO_actionComplete_cb_returnsSpecificValue);
	RUN_TEST_CASE(io, IO_setIO_actionComplete_cb_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(io, IO_setIO_actionComplete_cb_canSetSpecificValue);

	/****  Set/Get objectPointer  ****************/
	/**/
	RUN_TEST_CASE(io, IO_getObjectPointer_returns_UnknownOnCreate);
	RUN_TEST_CASE(io, IO_getObjectPointer_returns_specificValue);
	RUN_TEST_CASE(io, IO_setObjectPointer_returnsSpecificValue);
	RUN_TEST_CASE(io, IO_setObjectPointer_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(io, IO_setObjectPointer_canSetSpecificValue);

	//****  copy/Time_copy  ****************
	/**/
	RUN_TEST_CASE(io, copy_returnsSelfOnSuccess);
	RUN_TEST_CASE(io, copy_AllItemsCopiedToSelf);
	RUN_TEST_CASE(io, copy_returnsNullOnNullSelf);
	RUN_TEST_CASE(io, copy_returnsNullOnNullMaster);

	//****  equal IO_io_equal  ********************
	/**/
	RUN_TEST_CASE(io, myTest_IO_IsEqualTo_myTest_IO);
	RUN_TEST_CASE(io, equal_UnequalAddressReturn_Unequal);
	RUN_TEST_CASE(io, equal_UnequalIOActionReturn_Unequal);
	RUN_TEST_CASE(io, equal_UnequalReadCountReturn_Unequal);
	RUN_TEST_CASE(io, equal_UnequalWriteCountReturn_Unequal);
	RUN_TEST_CASE(io, equal_UnequalBufferPointerReturn_Unequal);
	RUN_TEST_CASE(io, equal_UnequalActionDoneCB_Unequal);
	RUN_TEST_CASE(io, equal_UnequalObjectPointerUnequal);

	RUN_TEST_CASE(io, equal_NullReturns_Null);
	RUN_TEST_CASE(io, equal_CopiedSensorReturnsEqual);

	//****  IO_addWriteValue  ********************
	/**/
	RUN_TEST_CASE(io, IO_addWriteValue_Returns_selfOnSuccess);
	RUN_TEST_CASE(io, IO_addWriteValue_incrementsWriteCountOnSingleAdd);
	RUN_TEST_CASE(io, IO_addWriteValue_addsSingleValue);
	RUN_TEST_CASE(io, IO_addWriteValue_addsMultipleValues);
	RUN_TEST_CASE(io, IO_addWriteValue_incrementsWriteCountOnMultipleAdd);

	//****  IO_addIOActionToList  *********************
	/**/
	RUN_TEST_CASE(io, IO_addIOActionToList_returnsNullOnNullPtr);
	RUN_TEST_CASE(io, IO_addIOActionToList_Returns_selfOnSuccess);
	RUN_TEST_CASE(io, IO_addIOActionToList_addsSingleIOObject);
	RUN_TEST_CASE(io, IO_addIOActionToList_addsMultipleIOObjects);

	//****  IO_getActionFromList  *********************
	/**/
	RUN_TEST_CASE(io, IO_getActionFromList_returnsNullOnNullPtr);
	RUN_TEST_CASE(io, IO_getActionFromList_Returns_NullOnemptyList);
	RUN_TEST_CASE(io, IO_getActionFromList_Returns_ioObjectFromList);
	RUN_TEST_CASE(io, IO_getActionFromList_Returns_MultipleioObjectsFromList);

	//****  IO_processSequence  ****************
	/**/
	RUN_TEST_CASE(io, IO_processSequence_Returns_selfOnSuccess);
	RUN_TEST_CASE(io, IO_processSequence_writeSingleToSingleAddress);
	RUN_TEST_CASE(io, IO_processSequence_writeMultipleValuesToSingleaddress);
	RUN_TEST_CASE(io, IO_processSequence_writeMultipleValuesToSequentialLocation);

	RUN_TEST_CASE(io, IO_processSequence_readSingleValue);
	RUN_TEST_CASE(io, IO_processSequence_readSequentialMultipleValues);
	RUN_TEST_CASE(io, IO_processSequence_readMultipleValuesfromSingleLocation);


	//****  IO_update  *********************
	/**/
	RUN_TEST_CASE(io, IO_update_writeSingleToSingleAddress);
	RUN_TEST_CASE(io, IO_update_writeMultipleValuesToSingleAddress);
	RUN_TEST_CASE(io, IO_update_writeMultipleValuesToSequentialLocation);

	RUN_TEST_CASE(io, IO_update_readSingleValue);
	RUN_TEST_CASE(io, IO_update_readSequentialMultipleValues);
	RUN_TEST_CASE(io, IO_update_readMultipleValuesfromSingleLocation);

	RUN_TEST_CASE(io, IO_update_firesCallback);
	RUN_TEST_CASE(io, IO_update_canBecalledMultipleTimesWithEmplyList);


}
