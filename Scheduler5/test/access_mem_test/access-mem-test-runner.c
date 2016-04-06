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

	RUN_TEST_CASE(accessMem, Access_addWriteCommandToSequence_is_Access_MEM_addWriteCommandToSequence);
	RUN_TEST_CASE(accessMem, Access_processSequence_is_Access_MEM_processSequence);
	//RUN_TEST_CASE(accessMem, Access_xxxx_is_Access_MEM_xxxx);


	RUN_TEST_CASE(accessMem, address_is_Null_OnCreate);
	RUN_TEST_CASE(accessMem, ioAction_is_UNKNOWN_OnCreate);
	RUN_TEST_CASE(accessMem, readCount_is_Zero_OnCreate);
	RUN_TEST_CASE(accessMem, writeCount_is_Zero_OnCreate);
	RUN_TEST_CASE(accessMem, bufferPointer_is_testBuffer_OnCreate);
	RUN_TEST_CASE(accessMem, actionComplete_cb_is_Null_OnCreate);
	RUN_TEST_CASE(accessMem, objectPointer_is_Null_OnCreate);
	RUN_TEST_CASE(accessMem, hardwareConfig_is_Null_OnCreate);

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
	/**/
	RUN_TEST_CASE(accessMem, Access_getReadCount_returns_UnknownOnCreate);
	RUN_TEST_CASE(accessMem, Access_getReadCount_returns_specificValue);
	RUN_TEST_CASE(accessMem, Access_setReadCount_returnsSpecificValue);
	RUN_TEST_CASE(accessMem, Access_setReadCount_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(accessMem, Access_setReadCount_canSetSpecificValue);

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

	/****  autoSet/Get bufferSize  ****************/
	/**/
	RUN_TEST_CASE(accessMem, Access_getBufferSize_returns_CorrectValueOnCreate);
	RUN_TEST_CASE(accessMem, Access_autoUpdateBufferSize_returnsZeroOnNullPtr);
	RUN_TEST_CASE(accessMem, Access_autoUpdateBufferSize_returnsCorrectBuffereSize);

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

	/****  Set/Get hardwareConfig  ****************/
	/**/
	RUN_TEST_CASE(accessMem, Access_getHardwareConfig_returns_UnknownOnCreate);
	RUN_TEST_CASE(accessMem, Access_getHardwareConfig_returns_specificValue);
	RUN_TEST_CASE(accessMem, Access_setHardwareConfig_returnsSpecificValue);
	RUN_TEST_CASE(accessMem, Access_setHardwareConfig_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(accessMem, Access_setHardwareConfig_canSetSpecificValue);

	//****  Access_sequenceIsValid  ****************
	/**/
	RUN_TEST_CASE(accessMem, Access_sequenceIsValid_returns_NullOnNullPtr);
	RUN_TEST_CASE(accessMem, Access_sequenceIsValid_returns_SelfOnSingleWrite);
	RUN_TEST_CASE(accessMem, Access_sequenceIsValid_returns_SelfOnSequentialWrite);
	RUN_TEST_CASE(accessMem, Access_sequenceIsValid_returns_SelfOnSingleWriteRead);
	RUN_TEST_CASE(accessMem, Access_sequenceIsValid_returns_SelfOnSequentialWriteRead);
	RUN_TEST_CASE(accessMem, Access_sequenceIsValid_returns_SelfOnSingleRead);
	RUN_TEST_CASE(accessMem, Access_sequenceIsValid_returns_SelfOnSequentialRead);

	RUN_TEST_CASE(accessMem, Access_sequenceIsValid_returns_NullOnIoActionLessThanWriteSingle);
	RUN_TEST_CASE(accessMem, Access_sequenceIsValid_returns_NullOnIoActionGreaterThanReadSequential);
	RUN_TEST_CASE(accessMem, Access_sequenceIsValid_returns_NullOnWriteWithNullAddress);
	RUN_TEST_CASE(accessMem, Access_sequenceIsValid_returns_NullOnWriteWithNullBuffer);
	RUN_TEST_CASE(accessMem, Access_sequenceIsValid_returns_NullOnWriteCountLargerThanBufferSize);
	RUN_TEST_CASE(accessMem, Access_sequenceIsValid_returns_NullOnReadCountGreaterThanBufferSize);
	RUN_TEST_CASE(accessMem, Access_sequenceIsValid_returns_NullOnBufferSizeNegative);
	RUN_TEST_CASE(accessMem, Access_sequenceIsValid_returns_NullOnWriteCountNegative);
	RUN_TEST_CASE(accessMem, Access_sequenceIsValid_returns_NullOnReadCountNegative);

	//****  copy AccessMEM  ****************
	/**/
	RUN_TEST_CASE(accessMem, copy_returnsSelfOnSuccess);
	RUN_TEST_CASE(accessMem, copy_AllItemsCopiedToSelf);
	RUN_TEST_CASE(accessMem, copy_returnsNullOnNullSelf);
	RUN_TEST_CASE(accessMem, copy_returnsNullOnNullMaster);

	//****  equal AccessMEM  ********************
	/**/
	RUN_TEST_CASE(accessMem, myTest_accessMem_IsEqualTo_myTest_accessMem);
	RUN_TEST_CASE(accessMem, equal_UnequalAddressReturn_Unequal);
	RUN_TEST_CASE(accessMem, equal_UnequalIOActionReturn_Unequal);
	RUN_TEST_CASE(accessMem, equal_UnequalReadCountReturn_Unequal);
	RUN_TEST_CASE(accessMem, equal_UnequalWriteCountReturn_Unequal);
	RUN_TEST_CASE(accessMem, equal_UnequalBufferPointerReturn_Equal);
	RUN_TEST_CASE(accessMem, equal_UnequalBufferSizeReturn_Unequal);
	RUN_TEST_CASE(accessMem, equal_UnequalActionDoneCB_Unequal);
	RUN_TEST_CASE(accessMem, equal_UnequalObjectPointerUnequal);
	RUN_TEST_CASE(accessMem, equal_UnequalHardwareConfigUnequal);

	RUN_TEST_CASE(accessMem, equal_NullReturns_OBJECT_UNEQUAL);
	RUN_TEST_CASE(accessMem, equal_CopiedSensorReturnsEqual);

	//****  AccessMEM_addWriteValue  ********************
	/**/
	RUN_TEST_CASE(accessMem, AccessMEM_addWriteValue_Returns_selfOnSuccess);
	RUN_TEST_CASE(accessMem, AccessMEM_addWriteValue_incrementsWriteCountOnSingleAdd);
	RUN_TEST_CASE(accessMem, AccessMEM_addWriteValue_addsSingleValue);
	RUN_TEST_CASE(accessMem, AccessMEM_addWriteValue_addsMultipleValues);
	RUN_TEST_CASE(accessMem, AccessMEM_addWriteValue_incrementsWriteCountOnMultipleAdd);
	RUN_TEST_CASE(accessMem, AccessMEM_addWriteValue_AddingMoreThanBufferSizeReturnsNULL);

	//****  IO_processSequence  ****************
	/**/
	RUN_TEST_CASE(accessMem, IO_processSequence_Returns_selfOnSuccess);
	RUN_TEST_CASE(accessMem, IO_processSequence_writeSingleToSingleAddress);
	RUN_TEST_CASE(accessMem, IO_processSequence_writeMultipleValuesToSingleaddress);
	RUN_TEST_CASE(accessMem, IO_processSequence_writeMultipleValuesToSequentialLocation);

	RUN_TEST_CASE(accessMem, IO_processSequence_readSingleValue);
	RUN_TEST_CASE(accessMem, IO_processSequence_readSequentialMultipleValues);
	RUN_TEST_CASE(accessMem, IO_processSequence_readMultipleValuesfromSingleLocation);

	RUN_TEST_CASE(accessMem, IO_processSequence_writeSingleReturnsNullOnNullBuffer);
	RUN_TEST_CASE(accessMem, IO_processSequence_writeSingleReturnsNullOnNullAddress);
	RUN_TEST_CASE(accessMem, IO_processSequence_writeSingleReturnsNullOnZeroCount);
	RUN_TEST_CASE(accessMem, IO_processSequence_writeSequentialReturnsNullOnZeroCount);
	RUN_TEST_CASE(accessMem, IO_processSequence_readSingleReturnsNullOnZeroCount);
	RUN_TEST_CASE(accessMem, IO_processSequence_readSequentialReturnsNullOnZeroCount);
	RUN_TEST_CASE(accessMem, IO_processSequence_writeReadSingleReturnsNullOnZeroCount);
	RUN_TEST_CASE(accessMem, IO_processSequence_writeReadSequentialReturnsNullOnZeroCount);
	RUN_TEST_CASE(accessMem, IO_processSequence_readSingleReturnsNullOnOverCount);

	RUN_TEST_CASE(accessMem, Access_sequenceComplete_cb_firesDesignatedCallback);
	RUN_TEST_CASE(accessMem, Access_sequenceComplete_cb_sendsCorrectPtr);

}
