/*
 * io-test.c
 *
 *  Created on: Sep 30, 2015
 *      Author: Alan
 */



#include "..\..\src\cross_compiler_defs.h"
#include "..\unity\unity_fixture.h"
//#include "..\unity\unity.h"
#include "..\..\src\mocks\RuntimeErrorStub.h"

#include "..\..\src\io\io.h"
#include "..\..\src\access_mem\access-mem.h"
//#include "..\..\src\sensors\sensors-private.h"

/****************************************/
/*** Basic Object and Class Testing ****/
struct       AccessMEM *      myTest_accessMem;
//const struct IOClass * myTest_accessMemClass_PTR;
//const struct Class *       myTest_accessMem_class_PTR;

// test buffer is a combined memory area for both write and read data
// write data is assumed to be loaded first, and then potentially overwritten by the read operation
#define ACCESS_COMMAND_BUFFER_SIZE  4
access_data_t testBuffer[ACCESS_COMMAND_BUFFER_SIZE];
access_data_t otherTestBuffer[ACCESS_COMMAND_BUFFER_SIZE];
//struct_task_t testTASKS_sensors[SCHEDULER_MAX_TASKS];

void * accessMem_test_general_cb(void * _self);
//struct Sensor * Sensor_test_general_cb2(struct Sensor * _self);
int access_test_cb_count;
//int access_test_cb_count2;

void * access_test_cb_ptr = NULL;

//int i;


//struct List * accessMemTest_ioActionList = NULL;
void *        ASSECCTest_ioActionBuffer[4];



/****************************************/
/*********** Start of Testing ***********/
TEST_GROUP(aaccessMem);

TEST_SETUP(accessMem)
{
	RuntimeErrorStub_Reset();

	//accessMemTest_ioActionList = new(List, ASSECCTest_ioActionBuffer);
	//IO_init(accessMemTest_ioActionList);

	Access_init();

	// create a new AccessMEM object using the externally created testBuffer
	myTest_accessMem = new(AccessMEM, testBuffer);

	if ( myTest_accessMem == NULL ) {printf("failed to allocate memory for new(AccessMEM)\n"); }

	// TODO: remove
	//myTest_SensorClass_PTR  = classOf(myTest_Sensor);
	//myTest_Sensor_class_PTR = Sensor;
	//scheduler_Create(testTASKS_sensors);

	otherTestBuffer[0] = 0x00;
	otherTestBuffer[1] = 0x00;
	otherTestBuffer[2] = 0x00;

	testBuffer[0] = 0x00;
	testBuffer[1] = 0x00;
	testBuffer[2] = 0x00;

	access_test_cb_count  = 0;
	//sensor_test_cb_count2 = 0;
}

TEST_TEAR_DOWN(accessMem)
{
	myTest_accessMem = safeDelete(myTest_accessMem);
	RuntimeErrorStub_Reset();
}

/**************************************/
/**************  New  *****************/

TEST(accessMem, myTest_accessMem_IsNotNullOnCreate)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  myTest_accessMem);
}

TEST(accessMem, AccessMEM_IsNotNullOnCreate)
{
	const struct AccessMEM * local_AccessMEM = AccessMEM;
	TEST_ASSERT_NOT_EQUAL(NULL,  local_AccessMEM);
}

TEST(accessMem, AccessMEMClass_IsNotNullOnCreate)
{
	const struct AccessMEMClass * local_AccessMEMClass = AccessMEMClass;
	TEST_ASSERT_NOT_EQUAL(NULL,  local_AccessMEMClass);
}


TEST(accessMem, ctor_is_Access_MEM_ctor)
{
	TEST_ASSERT_EQUAL_PTR(Access_MEM_ctor,  myTest_accessMem->_.class->ctor);
}

TEST(accessMem, dtor_is_Access_MEM_dtor)
{
	TEST_ASSERT_EQUAL_PTR(Access_MEM_dtor,  myTest_accessMem->_.class->dtor);
}

TEST(accessMem, copy_is_Access_MEM_copy)
{
	TEST_ASSERT_EQUAL_PTR(Access_MEM_copy,  myTest_accessMem->_.class->copy);
}

TEST(accessMem, equal_is_Access_MEM_equal)
{
	TEST_ASSERT_EQUAL_PTR(Access_MEM_equal,  myTest_accessMem->_.class->equal);
}

TEST(accessMem, config_is_Access_MEM_config)
{
	TEST_ASSERT_EQUAL_PTR(Access_MEM_config,  myTest_accessMem->_.class->config);
}

TEST(accessMem, puto_is_Access_MEM_puto)
{
	TEST_ASSERT_EQUAL_PTR(Access_MEM_puto,  myTest_accessMem->_.class->puto);
}

TEST(accessMem, Class_ctor_is_Access_MEM_Class_ctor)
{
	const struct AccessMEMClass * localAccessMEMClass = AccessMEMClass;
	TEST_ASSERT_EQUAL_PTR(Access_MEMClass_ctor,  localAccessMEMClass->_.ctor);
}

// ++++++++ Overloadable Functions ++++++++++++ //

TEST(accessMem, Access_addWriteCommandToSequence_is_Access_MEM_addWriteCommandToSequence)
{
	const struct AccessMEMClass * localIOClass = classOf(myTest_accessMem);
	TEST_ASSERT_EQUAL_PTR(Access_MEM_addWriteCommandToSequence,  localIOClass->Access_addWriteCommandToSequence);
}

TEST(accessMem, Access_processSequence_is_Access_MEM_processSequence)
{
	const struct AccessMEMClass * localIOClass = classOf(myTest_accessMem);
	TEST_ASSERT_EQUAL_PTR(Access_MEM_processSequence,  localIOClass->Access_processSequence);
}
/*
TEST(accessMem, ACCESS_MEM_xxxx_is_ACCESS_MEM_xxxx)
{
	const struct IOClass * localIOClass = classOf(myTest_accessMem);
	TEST_ASSERT_EQUAL_PTR(ACCESS_MEM_xxxx,  localIOClass->ACCESS_MEM_xxxx);
}
*/

TEST(accessMem, address_is_Null_OnCreate)
{
	TEST_ASSERT_EQUAL_PTR(NULL,  myTest_accessMem->address);
}

TEST(accessMem, ioAction_is_UNKNOWN_OnCreate)
{
	TEST_ASSERT_EQUAL(ACCESS_ACTION_UNKNOWN,  myTest_accessMem->ioAction);
}

TEST(accessMem, readCount_is_Zero_OnCreate)
{
	TEST_ASSERT_EQUAL(0,  myTest_accessMem->readCount);
}

TEST(accessMem, writeCount_is_Zero_OnCreate)
{
	TEST_ASSERT_EQUAL(0,  myTest_accessMem->writeCount);
}

TEST(accessMem, bufferPointer_is_testBuffer_OnCreate)
{
	TEST_ASSERT_EQUAL_PTR(testBuffer,  myTest_accessMem->bufferPointer);
}

TEST(accessMem, actionComplete_cb_is_Null_OnCreate)
{
	TEST_ASSERT_EQUAL_PTR(NULL,  myTest_accessMem->actionDone_cb);
}

TEST(accessMem, objectPointer_is_Null_OnCreate)
{
	TEST_ASSERT_EQUAL_PTR(NULL,  myTest_accessMem->objectPointer);
}

TEST(accessMem, hardwareConfig_is_Null_OnCreate)
{
	TEST_ASSERT_EQUAL_PTR(NULL,  myTest_accessMem->hardwareConfig);
}

/****  delete/Access_dtor  ****************/

TEST(accessMem, delete_returns_SelfOnSuccess)
{
	struct AccessMEM * otherAccessMEM = new(AccessMEM, otherTestBuffer);
	TEST_ASSERT_EQUAL_PTR(otherAccessMEM,  delete(otherAccessMEM));
}

TEST(accessMem, delete_safeDelete_returnsNullOnSuccess)
{
	struct AccessMEM * otherAccessMEM = new(AccessMEM, otherTestBuffer);
	TEST_ASSERT_EQUAL_PTR(NULL,  safeDelete(otherAccessMEM));
}

/****  Set/Get address  ****************/

TEST(accessMem, Access_getAddress_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(NULL,  Access_getAddress(myTest_accessMem) );
}

TEST(accessMem, Access_getAddress_returns_specificValue)
{
	myTest_accessMem->address = testBuffer;
	TEST_ASSERT_EQUAL(testBuffer,  Access_getAddress(myTest_accessMem) );
}

TEST(accessMem, Access_setAddress_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(testBuffer,  Access_setAddress(myTest_accessMem, testBuffer));
}

TEST(accessMem, Access_setAddress_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(NULL,  Access_setAddress(NULL, testBuffer));
}

TEST(accessMem, Access_setAddress_canSetSpecificValue)
{
	Access_setAddress(myTest_accessMem, testBuffer);
	TEST_ASSERT_EQUAL(testBuffer,  myTest_accessMem->address);
}

/****  Set/Get ioAction  ****************/

TEST(accessMem, Access_getIOAction_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(ACCESS_ACTION_UNKNOWN,  Access_getIOAction(myTest_accessMem) );
}

TEST(accessMem, Access_getIOAction_returns_specificValue)
{
	myTest_accessMem->ioAction = ACCESS_WRITE_SINGLE;
	TEST_ASSERT_EQUAL(ACCESS_WRITE_SINGLE,  Access_getIOAction(myTest_accessMem) );
}

TEST(accessMem, Access_setIOAction_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(ACCESS_WRITE_SINGLE,  Access_setIOAction(myTest_accessMem, IO_WRITE_SINGLE));
}

TEST(accessMem, Access_setIOAction_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(ACCESS_ACTION_UNKNOWN,  Access_setIOAction(NULL, ACCESS_WRITE_SINGLE));
}

TEST(accessMem, Access_setIOAction_canSetSpecificValue)
{
	Access_setIOAction(myTest_accessMem, ACCESS_WRITE_SINGLE);
	TEST_ASSERT_EQUAL(ACCESS_WRITE_SINGLE,  myTest_accessMem->ioAction);
}

/****  Set/Get readCount  ****************/

TEST(accessMem, Access_getReadCount_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(0,  Access_getReadCount(myTest_accessMem) );
}

TEST(accessMem, Access_getReadCount_returns_specificValue)
{
	myTest_accessMem->readCount = 1;
	TEST_ASSERT_EQUAL(1,  Access_getReadCount(myTest_accessMem) );
}

TEST(accessMem, Access_setReadCount_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(2,  Access_setReadCount(myTest_accessMem, 2));
}

TEST(accessMem, Access_setReadCount_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(0,  Access_setReadCount(NULL, 3));
}

TEST(accessMem, Access_setReadCount_canSetSpecificValue)
{
	Access_setReadCount(myTest_accessMem, 4);
	TEST_ASSERT_EQUAL(4,  myTest_accessMem->readCount);
}

/****  Set/Get writeCount  ****************/

TEST(accessMem, Access_getWriteCount_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(0,  Access_getWriteCount(myTest_accessMem) );
}

TEST(accessMem, Access_getWriteCount_returns_specificValue)
{
	myTest_accessMem->writeCount = 1;
	TEST_ASSERT_EQUAL(1,  Access_getWriteCount(myTest_accessMem) );
}

TEST(accessMem, Access_setWriteCount_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(2,  Access_setWriteCount(myTest_accessMem, 2));
}

TEST(accessMem, Access_setWriteCount_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(0,  Access_setWriteCount(NULL, 3));
}

TEST(accessMem, Access_setWriteCount_canSetSpecificValue)
{
	Access_setWriteCount(myTest_accessMem, 4);
	TEST_ASSERT_EQUAL(4,  myTest_accessMem->writeCount);
}

/****  Set/Get bufferPointer  ****************/

TEST(accessMem, Access_getBufferPointer_returns_UnknownOnCreate)
{
	// set to testBuffer from the original new(AccessMEM, testBuffer); in setup
	TEST_ASSERT_EQUAL(testBuffer,  Access_getBufferPointer(myTest_accessMem) );
}

TEST(accessMem, Access_getBufferPointer_returns_specificValue)
{
	myTest_accessMem->bufferPointer = (void *)testBuffer;
	TEST_ASSERT_EQUAL(testBuffer,  Access_getBufferPointer(myTest_accessMem) );
}

TEST(accessMem, Access_setBufferPointer_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(testBuffer,  Access_setBufferPointer(myTest_accessMem, testBuffer));
}

TEST(accessMem, Access_setBufferPointer_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(NULL,  Access_setBufferPointer(NULL, testBuffer));
}

TEST(accessMem, Access_setBufferPointer_canSetSpecificValue)
{
	Access_setBufferPointer(myTest_accessMem, testBuffer);
	TEST_ASSERT_EQUAL(testBuffer,  myTest_accessMem->bufferPointer);
}

/****  Set/Get bufferSize  ****************/

TEST(accessMem, Access_getBufferSize_returns_DefineValueOnCreate)
{
	TEST_ASSERT_EQUAL(ACCESS_COMMAND_BUFFER_SIZE,  Access_getBufferSize(myTest_accessMem) );
}

TEST(accessMem, Access_getBufferSize_returns_specificValue)
{
	myTest_accessMem->bufferSize = 1;
	TEST_ASSERT_EQUAL(1,  Access_getBufferSize(myTest_accessMem) );
}

TEST(accessMem, Access_setBufferSize_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(2,  Access_setBufferSize(myTest_accessMem, 2));
}

TEST(accessMem, Access_setBufferSize_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(0,  Access_setBufferSize(NULL, 3));
}

TEST(accessMem, Access_setBufferSize_canSetSpecificValue)
{
	Access_setBufferSize(myTest_accessMem, 4);
	TEST_ASSERT_EQUAL(4,  myTest_accessMem->bufferSize);
}


TEST(accessMem, Access_autoUpdateBufferSize_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(0,  Access_autoUpdateBufferSize(NULL));
}

TEST(accessMem, Access_autoUpdateBufferSize_returnsCorrectBuffereSize)
{
	TEST_ASSERT_EQUAL(sizeof(testBuffer),  Access_autoUpdateBufferSize(myTest_accessMem));
}

/****  Set/Get Access_actionComplete_cb  ****************/

TEST(accessMem, Access_getIO_actionComplete_cb_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(NULL,  Access_getActionDone_cb(myTest_accessMem) );
}

TEST(accessMem, Access_getIO_actionComplete_cb_returns_specificValue)
{
	myTest_accessMem->actionDone_cb = (void *)testBuffer;
	TEST_ASSERT_EQUAL(testBuffer,  Access_getActionDone_cb(myTest_accessMem) );
}

TEST(accessMem, Access_setIO_actionComplete_cb_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(testBuffer,  Access_setActionDone_cb(myTest_accessMem, (io_cb_fnct)testBuffer));
}

TEST(accessMem, Access_setIO_actionComplete_cb_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(NULL,  Access_setActionDone_cb(NULL, (io_cb_fnct)testBuffer));
}

TEST(accessMem, Access_setIO_actionComplete_cb_canSetSpecificValue)
{
	Access_setActionDone_cb(myTest_accessMem, (access_cb_fnct)testBuffer);
	TEST_ASSERT_EQUAL(testBuffer,  myTest_accessMem->actionDone_cb);
}


/****  Set/Get objectPointer  ****************/

TEST(accessMem, Access_getObjectPointer_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(NULL,  Access_getObjectPointer(myTest_accessMem) );
}

TEST(accessMem, Access_getObjectPointer_returns_specificValue)
{
	myTest_accessMem->objectPointer = (void *)testBuffer;
	TEST_ASSERT_EQUAL(testBuffer,  Access_getObjectPointer(myTest_accessMem) );
}

TEST(accessMem, Access_setObjectPointer_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(testBuffer,  Access_setObjectPointer(myTest_accessMem, testBuffer));
}

TEST(accessMem, Access_setObjectPointer_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(NULL,  Access_setObjectPointer(NULL, testBuffer));
}

TEST(accessMem, Access_setObjectPointer_canSetSpecificValue)
{
	Access_setObjectPointer(myTest_accessMem, testBuffer);
	TEST_ASSERT_EQUAL(testBuffer,  myTest_accessMem->objectPointer);
}

/****  Set/Get hardwareConfig  ****************/

TEST(accessMem, Access_getHardwareConfig_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(NULL,  Access_getHardwareConfig(myTest_accessMem) );
}

TEST(accessMem, Access_getHardwareConfig_returns_specificValue)
{
	myTest_accessMem->hardwareConfig = (void *)testBuffer;
	TEST_ASSERT_EQUAL(testBuffer,  Access_getHardwareConfig(myTest_accessMem) );
}

TEST(accessMem, Access_setHardwareConfig_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(testBuffer,  Access_setHardwareConfig(myTest_accessMem, testBuffer));
}

TEST(accessMem, Access_setHardwareConfig_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(NULL,  Access_setHardwareConfig(NULL, testBuffer));
}

TEST(accessMem, Access_setHardwareConfig_canSetSpecificValue)
{
	Access_setHardwareConfig(myTest_accessMem, testBuffer);
	TEST_ASSERT_EQUAL(testBuffer,  myTest_accessMem->hardwareConfig);
}

//****  copy AccessMEM  ****************

TEST(accessMem, copy_returnsSelfOnSuccess)
{
	struct AccessMEM * masterIO = new(AccessMEM, otherTestBuffer);
	Access_setReadCount(masterIO, 5);
	TEST_ASSERT_EQUAL_PTR(myTest_accessMem, copy(myTest_accessMem, masterIO));
	masterIO = safeDelete(masterIO);
}

TEST(accessMem, copy_AllItemsCopiedToSelf)
{
	// NOTE: sensorState, and pointers are unique for every sensor
	struct AccessMEM * masterAccess = new(AccessMEM, otherTestBuffer);
	Access_setAddress       (masterAccess, otherTestBuffer);
	Access_setIOAction      (masterAccess, ACCESS_WRITE_SINGLE);
	Access_setReadCount     (masterAccess, 2);
	Access_setWriteCount    (masterAccess, 6);
	Access_setBufferPointer (masterAccess, otherTestBuffer);
	Access_setActionDone_cb (masterAccess, (io_cb_fnct)otherTestBuffer);
	Access_setObjectPointer (masterAccess, otherTestBuffer);
	Access_setHardwareConfig(masterAccess, otherTestBuffer);

	copy(myTest_accessMem, masterAccess);

	// NOTE: sensorState, and pointers are unique for every sensor
	TEST_ASSERT_EQUAL(otherTestBuffer,     myTest_accessMem->address);
	TEST_ASSERT_EQUAL(ACCESS_WRITE_SINGLE, myTest_accessMem->ioAction);
	TEST_ASSERT_EQUAL(2,                   myTest_accessMem->readCount);
	TEST_ASSERT_EQUAL(6,                   myTest_accessMem->writeCount);
	// The buffer pointer is unique for every new(AccessMEM, &buffer) instantiation
	//TEST_ASSERT_EQUAL_PTR(otherTestBuffer, myTest_accessMem->bufferPointer);
	TEST_ASSERT_EQUAL_PTR(otherTestBuffer, myTest_accessMem->actionDone_cb);
	TEST_ASSERT_EQUAL_PTR(otherTestBuffer, myTest_accessMem->objectPointer);
	TEST_ASSERT_EQUAL_PTR(otherTestBuffer, myTest_accessMem->hardwareConfig);

	masterAccess = safeDelete(masterAccess);
}

TEST(accessMem, copy_returnsNullOnNullSelf)
{
	struct AccessMEM * masterIO = new(AccessMEM, otherTestBuffer);
	Access_setReadCount(masterIO, 5);
	TEST_ASSERT_EQUAL_PTR(NULL, copy(NULL, masterIO) );
	masterIO = safeDelete(masterIO);
}

TEST(accessMem, copy_returnsNullOnNullMaster)
{
	struct AccessMEM * masterIO = new(AccessMEM, otherTestBuffer);
	Access_setReadCount(masterIO, 5);
	TEST_ASSERT_EQUAL_PTR(NULL, copy(myTest_accessMem, NULL) );
	masterIO = safeDelete(masterIO);
}

//****  equal AccessMEM  ********************

TEST(accessMem, myTest_accessMem_IsEqualTo_myTest_accessMem)
{
	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_accessMem, myTest_accessMem));
}

TEST(accessMem, equal_UnequalAddressReturn_Unequal)
{
	struct AccessMEM * masterIO = new(AccessMEM, otherTestBuffer);
	Access_setAddress(masterIO, otherTestBuffer);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_accessMem, masterIO) );
	masterIO = safeDelete(masterIO);
}

TEST(accessMem, equal_UnequalIOActionReturn_Unequal)
{
	struct AccessMEM * masterIO = new(AccessMEM, otherTestBuffer);
	Access_setIOAction(masterIO, ACCESS_READ_SINGLE);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_accessMem, masterIO) );
	masterIO = safeDelete(masterIO);
}

TEST(accessMem, equal_UnequalReadCountReturn_Unequal)
{
	struct AccessMEM * masterIO = new(AccessMEM, otherTestBuffer);
	Access_setReadCount(masterIO, 1);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_accessMem, masterIO) );
	masterIO = safeDelete(masterIO);
}

TEST(accessMem, equal_UnequalWriteCountReturn_Unequal)
{
	struct AccessMEM * masterIO = new(AccessMEM, otherTestBuffer);
	Access_setWriteCount(masterIO, 6);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_accessMem, masterIO) );
	masterIO = safeDelete(masterIO);
}

TEST(accessMem, equal_UnequalBufferPointerReturn_Equal)
{
	struct AccessMEM * masterIO = new(AccessMEM, otherTestBuffer);
	Access_setBufferPointer(masterIO, otherTestBuffer);
	// bufferPointers are unique and will not trigger OBJECT_UNEQUAL
	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_accessMem, masterIO) );
	masterIO = safeDelete(masterIO);
}

TEST(accessMem, equal_UnequalBufferSizeReturn_Unequal)
{
	struct AccessMEM * masterIO = new(AccessMEM, otherTestBuffer);
	Access_setBufferSize(masterIO, myTest_accessMem->bufferSize + 1 );
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_accessMem, masterIO) );
	masterIO = safeDelete(masterIO);
}

TEST(accessMem, equal_UnequalActionDoneCB_Unequal)
{
	struct AccessMEM * masterIO = new(AccessMEM, otherTestBuffer);
	Access_setActionDone_cb(masterIO, (access_cb_fnct)otherTestBuffer);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_accessMem, masterIO) );
	masterIO = safeDelete(masterIO);
}

TEST(accessMem, equal_UnequalHardwareConfigUnequal)
{
	struct AccessMEM * masterIO = new(AccessMEM, otherTestBuffer);
	Access_setHardwareConfig(masterIO, otherTestBuffer);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_accessMem, masterIO) );
	masterIO = safeDelete(masterIO);
}

TEST(accessMem, equal_UnequalObjectPointerUnequal)
{
	struct AccessMEM * masterIO = new(AccessMEM, otherTestBuffer);
	Access_setObjectPointer(masterIO, otherTestBuffer);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_accessMem, masterIO) );
	masterIO = safeDelete(masterIO);
}


TEST(accessMem, equal_NullReturns_OBJECT_UNEQUAL)
{
	struct AccessMEM * masterIO = new(AccessMEM, otherTestBuffer);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_accessMem, NULL));
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(NULL, masterIO ));
	masterIO = safeDelete(masterIO);
}

TEST(accessMem, equal_CopiedSensorReturnsEqual)
{
	struct AccessMEM * masterIO = new(AccessMEM, otherTestBuffer);
	Access_setAddress       (masterIO, otherTestBuffer);
	Access_setIOAction      (masterIO, ACCESS_WRITE_SINGLE);
	Access_setReadCount     (masterIO, 5);
	Access_setWriteCount    (masterIO, 6);
	//Access_setBufferPointer (masterIO, otherTestBuffer);  // set in new(AccessMEM, bufferAddress);
	Access_setActionDone_cb(masterIO, (access_cb_fnct)otherTestBuffer);
	Access_setObjectPointer (masterIO, otherTestBuffer);

	copy(myTest_accessMem, masterIO);

	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_accessMem, masterIO));
	masterIO = safeDelete(masterIO);
}

//****  AccessMEM_addWriteValue  ********************

TEST(accessMem, AccessMEM_addWriteValue_Returns_selfOnSuccess)
{
	TEST_ASSERT_EQUAL(myTest_accessMem, Access_addWriteCommandToSequence(myTest_accessMem, 0xFF));
}

TEST(accessMem, AccessMEM_addWriteValue_incrementsWriteCountOnSingleAdd)
{
	Access_addWriteCommandToSequence(myTest_accessMem, 0xFF);
	TEST_ASSERT_EQUAL(1, myTest_accessMem->writeCount);
}

TEST(accessMem, AccessMEM_addWriteValue_addsSingleValue)
{
	Access_addWriteCommandToSequence(myTest_accessMem, 0xFF);
	TEST_ASSERT_EQUAL(0xFF, myTest_accessMem->bufferPointer[0] );
}

TEST(accessMem, AccessMEM_addWriteValue_addsMultipleValues)
{
	Access_addWriteCommandToSequence(myTest_accessMem, 0xFF);
	Access_addWriteCommandToSequence(myTest_accessMem, 0xAA);
	Access_addWriteCommandToSequence(myTest_accessMem, 0x77);
	TEST_ASSERT_EQUAL(0xFF, myTest_accessMem->bufferPointer[0] );
	TEST_ASSERT_EQUAL(0xAA, myTest_accessMem->bufferPointer[1] );
	TEST_ASSERT_EQUAL(0x77, myTest_accessMem->bufferPointer[2] );
}

TEST(accessMem, AccessMEM_addWriteValue_incrementsWriteCountOnMultipleAdd)
{
	Access_addWriteCommandToSequence(myTest_accessMem, 0xFF);
	Access_addWriteCommandToSequence(myTest_accessMem, 0xAA);
	Access_addWriteCommandToSequence(myTest_accessMem, 0x77);
	TEST_ASSERT_EQUAL(3, myTest_accessMem->writeCount );
}

TEST(accessMem, AccessMEM_addWriteValue_AddingMoreThanBufferSizeReturnsNULL)
{
	// pre-load the command buffer with max number of values
	int i;
	for ( i = 0; i < ACCESS_COMMAND_BUFFER_SIZE; i++ ) {
		Access_addWriteCommandToSequence( myTest_accessMem, (io_data_t)i );
	}

	// add one too many values
	TEST_ASSERT_EQUAL(NULL, Access_addWriteCommandToSequence( myTest_accessMem, 0x00 ) );
}

//****  IO_processSequence  ****************

TEST(accessMem, IO_processSequence_Returns_selfOnSuccess)
{
	Access_addWriteCommandToSequence(myTest_accessMem, 0xFF);
	Access_setAddress     (myTest_accessMem, otherTestBuffer);
	Access_setIOAction    (myTest_accessMem, ACCESS_WRITE_SINGLE);
	Access_processSequence(myTest_accessMem);
	TEST_ASSERT_EQUAL(myTest_accessMem, Access_processSequence(myTest_accessMem) );
}

TEST(accessMem, IO_processSequence_writeSingleToSingleAddress)
{
	Access_addWriteCommandToSequence(myTest_accessMem, 0xFF);
	Access_setAddress   (myTest_accessMem, otherTestBuffer);
	Access_setIOAction  (myTest_accessMem, ACCESS_WRITE_SINGLE);
	Access_processSequence       (myTest_accessMem);
	TEST_ASSERT_EQUAL(0xFF, otherTestBuffer[0] );
}

TEST(accessMem, IO_processSequence_writeMultipleValuesToSingleaddress)
{
	Access_setAddress   (myTest_accessMem, otherTestBuffer);
	Access_setIOAction  (myTest_accessMem, ACCESS_WRITE_SINGLE);
	Access_addWriteCommandToSequence(myTest_accessMem, 0x01);
	Access_addWriteCommandToSequence(myTest_accessMem, 0x02);
	Access_addWriteCommandToSequence(myTest_accessMem, 0x03);
	Access_processSequence       (myTest_accessMem);
	TEST_ASSERT_EQUAL(0x03, otherTestBuffer[0] );
}

TEST(accessMem, IO_processSequence_writeMultipleValuesToSequentialLocation)
{
	Access_addWriteCommandToSequence(myTest_accessMem, 0x01);
	Access_addWriteCommandToSequence(myTest_accessMem, 0x02);
	Access_addWriteCommandToSequence(myTest_accessMem, 0x03);
	Access_setAddress     (myTest_accessMem, otherTestBuffer);
	Access_setIOAction    (myTest_accessMem, ACCESS_WRITE_SEQUENTIAL);
	Access_processSequence(myTest_accessMem);
	TEST_ASSERT_EQUAL(0x01, otherTestBuffer[0] );
	TEST_ASSERT_EQUAL(0x02, otherTestBuffer[1] );
	TEST_ASSERT_EQUAL(0x03, otherTestBuffer[2] );
}

TEST(accessMem, IO_processSequence_readSingleValue)
{
	otherTestBuffer[0] = 0x01;
	Access_setReadCount (myTest_accessMem, 1);
	Access_setAddress     (myTest_accessMem, otherTestBuffer);
	Access_setIOAction    (myTest_accessMem, ACCESS_READ_SINGLE);
	Access_processSequence(myTest_accessMem);
	TEST_ASSERT_EQUAL(0x01, testBuffer[0] );
}

TEST(accessMem, IO_processSequence_readSequentialMultipleValues)
{
	otherTestBuffer[0] = 0x01;
	otherTestBuffer[1] = 0x02;
	otherTestBuffer[2] = 0x03;
	Access_setReadCount (myTest_accessMem, 3);
	Access_setAddress   (myTest_accessMem, otherTestBuffer);
	Access_setIOAction  (myTest_accessMem, ACCESS_READ_SEQUENTIAL);
	Access_processSequence       (myTest_accessMem);
	TEST_ASSERT_EQUAL(0x01, testBuffer[0] );
	TEST_ASSERT_EQUAL(0x02, testBuffer[1] );
	TEST_ASSERT_EQUAL(0x03, testBuffer[2] );
}

TEST(accessMem, IO_processSequence_readMultipleValuesfromSingleLocation)
{
	otherTestBuffer[0] = 0x01;
	otherTestBuffer[1] = 0x02;
	otherTestBuffer[2] = 0x03;
	Access_setReadCount (myTest_accessMem, 3);
	Access_setAddress   (myTest_accessMem, otherTestBuffer);
	Access_setIOAction  (myTest_accessMem, ACCESS_READ_SINGLE);
	Access_processSequence       (myTest_accessMem);
	TEST_ASSERT_EQUAL(0x01, testBuffer[0] );
	TEST_ASSERT_EQUAL(0x01, testBuffer[1] );
	TEST_ASSERT_EQUAL(0x01, testBuffer[2] );
}

TEST(accessMem, IO_processSequence_writeSingleReturnsNullOnNullBuffer)
{
	Access_addWriteCommandToSequence(myTest_accessMem, 0xFF);
	Access_setBufferPointer(myTest_accessMem, NULL);
	Access_setAddress (myTest_accessMem, otherTestBuffer);
	Access_setIOAction(myTest_accessMem, ACCESS_WRITE_SINGLE);
	TEST_ASSERT_EQUAL(NULL, Access_processSequence(myTest_accessMem) );
}

TEST(accessMem, IO_processSequence_writeSingleReturnsNullOnNullAddress)
{
	Access_addWriteCommandToSequence(myTest_accessMem, 0xFF);
	//Access_setBufferPointer(myTest_accessMem, NULL);
	//Access_setAddress (myTest_accessMem, otherTestBuffer);
	Access_setIOAction(myTest_accessMem, ACCESS_WRITE_SINGLE);
	TEST_ASSERT_EQUAL(NULL, Access_processSequence(myTest_accessMem) );
}

TEST(accessMem, IO_processSequence_writeSingleReturnsNullOnZeroCount)
{
	//Access_addWriteCommandToSequence(myTest_accessMem, 0xFF);
	Access_setAddress (myTest_accessMem, otherTestBuffer);
	Access_setIOAction(myTest_accessMem, ACCESS_WRITE_SINGLE);
	TEST_ASSERT_EQUAL(NULL, Access_processSequence(myTest_accessMem) );
}

TEST(accessMem, IO_processSequence_writeSequentialReturnsNullOnZeroCount)
{
	//Access_addWriteCommandToSequence(myTest_accessMem, 0xFF);
	Access_setAddress (myTest_accessMem, otherTestBuffer);
	Access_setIOAction(myTest_accessMem, ACCESS_WRITE_SEQUENTIAL);
	TEST_ASSERT_EQUAL(NULL, Access_processSequence(myTest_accessMem) );
}

TEST(accessMem, IO_processSequence_readSingleReturnsNullOnZeroCount)
{
	//Access_addWriteCommandToSequence(myTest_accessMem, 0xFF);
	Access_setAddress (myTest_accessMem, otherTestBuffer);
	Access_setIOAction(myTest_accessMem, ACCESS_READ_SINGLE);
	TEST_ASSERT_EQUAL(NULL, Access_processSequence(myTest_accessMem) );
}

TEST(accessMem, IO_processSequence_readSequentialReturnsNullOnZeroCount)
{
	//Access_addWriteCommandToSequence(myTest_accessMem, 0xFF);
	Access_setAddress (myTest_accessMem, otherTestBuffer);
	Access_setIOAction(myTest_accessMem, ACCESS_READ_SEQUENTIAL);
	TEST_ASSERT_EQUAL(NULL, Access_processSequence(myTest_accessMem) );
}

TEST(accessMem, IO_processSequence_writeReadSingleReturnsNullOnZeroCount)
{
	//Access_addWriteCommandToSequence(myTest_accessMem, 0xFF);
	Access_setReadCount(myTest_accessMem, 2);
	Access_setAddress  (myTest_accessMem, otherTestBuffer);
	Access_setIOAction (myTest_accessMem, ACCESS_WRITE_READ_SINGLE);
	TEST_ASSERT_EQUAL(NULL, Access_processSequence(myTest_accessMem) );
}

TEST(accessMem, IO_processSequence_writeReadSequentialReturnsNullOnZeroCount)
{
	Access_addWriteCommandToSequence(myTest_accessMem, 0xFF);
	Access_setReadCount(myTest_accessMem, 0);
	Access_setAddress  (myTest_accessMem, otherTestBuffer);
	Access_setIOAction (myTest_accessMem, ACCESS_WRITE_READ_SEQUENTIAL);
	TEST_ASSERT_EQUAL(NULL, Access_processSequence(myTest_accessMem) );
}

TEST(accessMem, IO_processSequence_readSingleReturnsNullOnOverCount)
{
	Access_addWriteCommandToSequence(myTest_accessMem, 0xFF);
	Access_autoUpdateBufferSize(myTest_accessMem);
	Access_setReadCount(myTest_accessMem, Access_getBufferSize(myTest_accessMem)+1);
	Access_setAddress (myTest_accessMem, otherTestBuffer);
	Access_setIOAction(myTest_accessMem, ACCESS_WRITE_READ_SINGLE);
	TEST_ASSERT_EQUAL(NULL, Access_processSequence(myTest_accessMem) );
}

//++++++++++++++++++++++++++++++//

TEST(accessMem, Access_sequenceComplete_cb_firesDesignatedCallback)
{
	TEST_ASSERT_EQUAL(0, access_test_cb_count);
	Access_setActionDone_cb(myTest_accessMem, (void *)accessMem_test_general_cb);
	Access_sequenceComplete_cb(myTest_accessMem);
	TEST_ASSERT_EQUAL(1, access_test_cb_count);
}

TEST(accessMem, Access_sequenceComplete_cb_sendsCorrectPtr)
{
	TEST_ASSERT_EQUAL(0, access_test_cb_count );
	Access_setActionDone_cb(myTest_accessMem, (void *)accessMem_test_general_cb);
	Access_setObjectPointer(myTest_accessMem, (void *)myTest_accessMem);
	Access_sequenceComplete_cb(myTest_accessMem);
	TEST_ASSERT_EQUAL(myTest_accessMem, access_test_cb_ptr );
}

//****  Support Methods  **************

/**/
void * accessMem_test_general_cb(void * _self)
{
	access_test_cb_count++;
	access_test_cb_ptr = _self;
	//printf("  XXX access_test_general_cb\n");
	return _self;
}
