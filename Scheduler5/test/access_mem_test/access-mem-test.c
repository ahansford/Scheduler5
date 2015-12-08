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
io_data_t testBuffer[ACCESS_COMMAND_BUFFER_SIZE];
io_data_t otherTestBuffer[ACCESS_COMMAND_BUFFER_SIZE];
//struct_task_t testTASKS_sensors[SCHEDULER_MAX_TASKS];

void * accessMem_test_general_cb(void * _self);
//struct Sensor * Sensor_test_general_cb2(struct Sensor * _self);
int io_test_cb_count;
//int sensor_test_cb_count2;

//int i;


struct List * accessMemTest_ioActionList = NULL;
void *        ASSECCTest_ioActionBuffer[4];



/****************************************/
/*********** Start of Testing ***********/
TEST_GROUP(aaccessMem);

TEST_SETUP(accessMem)
{
	RuntimeErrorStub_Reset();
	accessMemTest_ioActionList = new(List, ASSECCTest_ioActionBuffer);
	IO_init(accessMemTest_ioActionList);

	io_test_cb_count  = 0;
	//sensor_test_cb_count2 = 0;

	myTest_accessMem           = new(AccessMEM, testBuffer);
	//if ( myTest_Sensor == NULL ) {printf("failed to allcate memory for new(Sensor)\n"); }
	//myTest_SensorClass_PTR  = classOf(myTest_Sensor);
	//myTest_Sensor_class_PTR = Sensor;
	//scheduler_Create(testTASKS_sensors);

	otherTestBuffer[0] = 0x00;
	otherTestBuffer[1] = 0x00;
	otherTestBuffer[2] = 0x00;

	testBuffer[0] = 0x00;
	testBuffer[1] = 0x00;
	testBuffer[2] = 0x00;
}

TEST_TEAR_DOWN(accessMem)
{
	myTest_accessMem = safeDelete(myTest_accessMem);
	accessMemTest_ioActionList = safeDelete(accessMemTest_ioActionList);

	//for ( i = 0; i < SCHEDULER_MAX_TASKS; i++)
	//	{ scheduler_DeleteTask(i); }

	//myTest_Sensor = new(Sensor);
	//if ( myTest_Sensor != memoryLeakPointer )
	//	{ printf("\nPossible memory leak in Sensors\n"); }
	//myTest_accessMem = safeDelete(myTest_accessMem);

	RuntimeErrorStub_Reset();
}

/**************************************/
/**************  New  *****************/

TEST(accessMem, myTest_accessMem_IsNotNullOnCreate)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  myTest_accessMem);
}

TEST(accessMem, IO_IsNotNullOnCreate)
{
	const struct IO * local_IO = IO;
	TEST_ASSERT_NOT_EQUAL(NULL,  local_IO);
}

TEST(accessMem, IOClass_IsNotNullOnCreate)
{
	const struct IOClass * local_IOClass = IOClass;
	TEST_ASSERT_NOT_EQUAL(NULL,  local_IOClass);
}


TEST(accessMem, ctor_is_IO_io_ctor)
{
	TEST_ASSERT_EQUAL_PTR(IO_io_ctor,  myTest_accessMem->_.class->ctor);
}

TEST(accessMem, dtor_is_IO_io_dtor)
{
	TEST_ASSERT_EQUAL_PTR(IO_io_dtor,  myTest_accessMem->_.class->dtor);
}

TEST(accessMem, copy_is_IO_io_copy)
{
	TEST_ASSERT_EQUAL_PTR(IO_io_copy,  myTest_accessMem->_.class->copy);
}

TEST(accessMem, equal_is_IO_io_equal)
{
	TEST_ASSERT_EQUAL_PTR(IO_io_equal,  myTest_accessMem->_.class->equal);
}

TEST(accessMem, config_is_IO_io_config)
{
	TEST_ASSERT_EQUAL_PTR(IO_io_config,  myTest_accessMem->_.class->config);
}

TEST(accessMem, puto_is_IO_io_puto)
{
	TEST_ASSERT_EQUAL_PTR(Access_MEM_puto,  myTest_accessMem->_.class->puto);
}

TEST(accessMem, Class_ctor_is_IO_Class_ctor)
{
	const struct IOClass * localIOClass = IOClass;
	TEST_ASSERT_EQUAL_PTR(IOClass_ctor,  localIOClass->_.ctor);
}

TEST(accessMem, IO_write_is_IO_io_write)
{
	const struct IOClass * localIOClass = classOf(myTest_accessMem);
	TEST_ASSERT_EQUAL_PTR(IO_io_addWriteValue,  localIOClass->IO_addWriteValue);
}

TEST(accessMem, IO_writeRead_is_IO_io_writeRead)
{
	const struct IOClass * localIOClass = classOf(myTest_accessMem);
	TEST_ASSERT_EQUAL_PTR(IO_io_processSequence,  localIOClass->IO_processSequence);
}

TEST(accessMem, IO_read_is_IO_io_read)
{
	const struct IOClass * localIOClass = classOf(myTest_accessMem);
	TEST_ASSERT_EQUAL_PTR(IO_io_xxxx,  localIOClass->IO_xxxx);
}


TEST(accessMem, address_is_Null_OnCreate)
{
	TEST_ASSERT_EQUAL_PTR(NULL,  myTest_accessMem->address);
}

TEST(accessMem, ioAction_is_UNKNOWN_OnCreate)
{
	TEST_ASSERT_EQUAL(IO_ACTION_UNKNOWN,  myTest_accessMem->ioAction);
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

TEST(accessMem, IO_actionComplete_cb_is_Null_OnCreate)
{
	TEST_ASSERT_EQUAL_PTR(NULL,  myTest_accessMem->actionDone_cb);
}

TEST(accessMem, objectPointer_is_Null_OnCreate)
{
	TEST_ASSERT_EQUAL_PTR(NULL,  myTest_accessMem->objectPointer);
}

/****  delete/Sensor_dtor  ****************/

TEST(accessMem, delete_returns_SelfOnSuccess)
{
	struct IO * otherIO = new(IO, otherTestBuffer);
	TEST_ASSERT_EQUAL_PTR(otherIO,  delete(otherIO));
}

TEST(accessMem, delete_safeDelete_returnsNullOnSuccess)
{
	struct IO * otherIO = new(IO, otherTestBuffer);
	TEST_ASSERT_EQUAL_PTR(NULL,  safeDelete(otherIO));
}

/****  Set/Get address  ****************/

TEST(accessMem, IO_getAddress_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(NULL,  IO_getAddress(myTest_accessMem) );
}

TEST(accessMem, IO_getAddress_returns_specificValue)
{
	myTest_accessMem->address = testBuffer;
	TEST_ASSERT_EQUAL(testBuffer,  IO_getAddress(myTest_accessMem) );
}

TEST(accessMem, IO_setAddress_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(testBuffer,  IO_setAddress(myTest_accessMem, testBuffer));
}

TEST(accessMem, IO_setAddress_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(NULL,  IO_setAddress(NULL, testBuffer));
}

TEST(accessMem, IO_setAddress_canSetSpecificValue)
{
	IO_setAddress(myTest_accessMem, testBuffer);
	TEST_ASSERT_EQUAL(testBuffer,  myTest_accessMem->address);
}

/****  Set/Get ioAction  ****************/

TEST(accessMem, IO_getIOAction_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(IO_ACTION_UNKNOWN,  IO_getIOAction(myTest_accessMem) );
}

TEST(accessMem, IO_getIOAction_returns_specificValue)
{
	myTest_accessMem->ioAction = IO_WRITE_SINGLE;
	TEST_ASSERT_EQUAL(IO_WRITE_SINGLE,  IO_getIOAction(myTest_accessMem) );
}

TEST(accessMem, IO_setIOAction_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(IO_WRITE_SINGLE,  IO_setIOAction(myTest_accessMem, IO_WRITE_SINGLE));
}

TEST(accessMem, IO_setIOAction_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(IO_ACTION_UNKNOWN,  IO_setIOAction(NULL, IO_WRITE_SINGLE));
}

TEST(accessMem, IO_setIOAction_canSetSpecificValue)
{
	IO_setIOAction(myTest_accessMem, IO_WRITE_SINGLE);
	TEST_ASSERT_EQUAL(IO_WRITE_SINGLE,  myTest_accessMem->ioAction);
}

/****  Set/Get readCount  ****************/

TEST(accessMem, IO_getReadCount_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(0,  IO_getReadCount(myTest_accessMem) );
}

TEST(accessMem, IO_getReadCount_returns_specificValue)
{
	myTest_accessMem->readCount = 1;
	TEST_ASSERT_EQUAL(1,  IO_getReadCount(myTest_accessMem) );
}

TEST(accessMem, IO_setReadCount_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(2,  IO_setReadCount(myTest_accessMem, 2));
}

TEST(accessMem, IO_setReadCount_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(0,  IO_setReadCount(NULL, 3));
}

TEST(accessMem, IO_setReadCount_canSetSpecificValue)
{
	IO_setReadCount(myTest_accessMem, 4);
	TEST_ASSERT_EQUAL(4,  myTest_accessMem->readCount);
}

/****  Set/Get writeCount  ****************/

TEST(accessMem, IO_getWriteCount_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(0,  IO_getWriteCount(myTest_accessMem) );
}

TEST(accessMem, IO_getWriteCount_returns_specificValue)
{
	myTest_accessMem->writeCount = 1;
	TEST_ASSERT_EQUAL(1,  IO_getWriteCount(myTest_accessMem) );
}

TEST(accessMem, IO_setWriteCount_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(2,  IO_setWriteCount(myTest_accessMem, 2));
}

TEST(accessMem, IO_setWriteCount_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(0,  IO_setWriteCount(NULL, 3));
}

TEST(accessMem, IO_setWriteCount_canSetSpecificValue)
{
	IO_setWriteCount(myTest_accessMem, 4);
	TEST_ASSERT_EQUAL(4,  myTest_accessMem->writeCount);
}

/****  Set/Get bufferPointer  ****************/

TEST(accessMem, IO_getBufferPointer_returns_UnknownOnCreate)
{
	// set to testBuffer from the original new(IO, testBuffer); in setup
	TEST_ASSERT_EQUAL(testBuffer,  IO_getBufferPointer(myTest_accessMem) );
}

TEST(accessMem, IO_getBufferPointer_returns_specificValue)
{
	myTest_accessMem->bufferPointer = (void *)testBuffer;
	TEST_ASSERT_EQUAL(testBuffer,  IO_getBufferPointer(myTest_accessMem) );
}

TEST(accessMem, IO_setBufferPointer_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(testBuffer,  IO_setBufferPointer(myTest_accessMem, testBuffer));
}

TEST(accessMem, IO_setBufferPointer_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(NULL,  IO_setBufferPointer(NULL, testBuffer));
}

TEST(accessMem, IO_setBufferPointer_canSetSpecificValue)
{
	IO_setBufferPointer(myTest_accessMem, testBuffer);
	TEST_ASSERT_EQUAL(testBuffer,  myTest_accessMem->bufferPointer);
}

/****  Set/Get bufferSize  ****************/

TEST(accessMem, IO_getBufferSize_returns_DefineValueOnCreate)
{
	TEST_ASSERT_EQUAL(ACCESS_COMMAND_BUFFER_SIZE,  Access_getBufferSize(myTest_accessMem) );
}

TEST(accessMem, IO_getBufferSize_returns_specificValue)
{
	myTest_accessMem->bufferSize = 1;
	TEST_ASSERT_EQUAL(1,  Access_getBufferSize(myTest_accessMem) );
}

TEST(accessMem, IO_setBufferSize_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(2,  IO_setBufferSize(myTest_accessMem, 2));
}

TEST(accessMem, IO_setBufferSize_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(0,  IO_setBufferSize(NULL, 3));
}

TEST(accessMem, IO_setBufferSize_canSetSpecificValue)
{
	IO_setBufferSize(myTest_accessMem, 4);
	TEST_ASSERT_EQUAL(4,  myTest_accessMem->bufferSize);
}

/****  Set/Get IO_actionComplete_cb  ****************/

TEST(accessMem, IO_getIO_actionComplete_cb_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(NULL,  IO_get_actionDone_cb(myTest_accessMem) );
}

TEST(accessMem, IO_getIO_actionComplete_cb_returns_specificValue)
{
	myTest_accessMem->actionDone_cb = (void *)testBuffer;
	TEST_ASSERT_EQUAL(testBuffer,  IO_get_actionDone_cb(myTest_accessMem) );
}

TEST(accessMem, IO_setIO_actionComplete_cb_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(testBuffer,  IO_set_actionDone_cb(myTest_accessMem, (io_cb_fnct)testBuffer));
}

TEST(accessMem, IO_setIO_actionComplete_cb_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(NULL,  IO_set_actionDone_cb(NULL, (io_cb_fnct)testBuffer));
}

TEST(accessMem, IO_setIO_actionComplete_cb_canSetSpecificValue)
{
	IO_set_actionDone_cb(myTest_accessMem, (io_cb_fnct)testBuffer);
	TEST_ASSERT_EQUAL(testBuffer,  myTest_accessMem->actionDone_cb);
}


/****  Set/Get objectPointer  ****************/

TEST(accessMem, IO_getObjectPointer_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(NULL,  Access_getObjectPointer(myTest_accessMem) );
}

TEST(accessMem, IO_getObjectPointer_returns_specificValue)
{
	myTest_accessMem->objectPointer = (void *)testBuffer;
	TEST_ASSERT_EQUAL(testBuffer,  Access_getObjectPointer(myTest_accessMem) );
}

TEST(accessMem, IO_setObjectPointer_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(testBuffer,  IO_setObjectPointer(myTest_accessMem, testBuffer));
}

TEST(accessMem, IO_setObjectPointer_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(NULL,  IO_setObjectPointer(NULL, testBuffer));
}

TEST(accessMem, IO_setObjectPointer_canSetSpecificValue)
{
	IO_setObjectPointer(myTest_accessMem, testBuffer);
	TEST_ASSERT_EQUAL(testBuffer,  myTest_accessMem->objectPointer);
}

//****  copy IO_io_copy  ****************

TEST(accessMem, copy_returnsSelfOnSuccess)
{
	struct io * masterIO = new(IO, otherTestBuffer);
	IO_setReadCount(masterIO, 5);
	TEST_ASSERT_EQUAL_PTR(myTest_accessMem, copy(myTest_accessMem, masterIO));
	masterIO = safeDelete(masterIO);
}

TEST(accessMem, copy_AllItemsCopiedToSelf)
{
	// NOTE: sensorState, and pointers are unique for every sensor
	struct IO * masterIO = new(IO, otherTestBuffer);
	IO_setAddress       (masterIO, otherTestBuffer);
	IO_setIOAction      (masterIO, IO_WRITE_SINGLE);
	IO_setReadCount     (masterIO, 5);
	IO_setWriteCount    (masterIO, 6);
	IO_setBufferPointer (masterIO, otherTestBuffer);
	IO_set_actionDone_cb(masterIO, (io_cb_fnct)otherTestBuffer);
	IO_setObjectPointer (masterIO, otherTestBuffer);

	copy(myTest_accessMem, masterIO);

	// NOTE: sensorState, and pointers are unique for every sensor
	TEST_ASSERT_EQUAL(otherTestBuffer,     myTest_accessMem->address);
	TEST_ASSERT_EQUAL(IO_WRITE_SINGLE,            myTest_accessMem->ioAction);
	TEST_ASSERT_EQUAL(5,                   myTest_accessMem->readCount);
	TEST_ASSERT_EQUAL(6,                   myTest_accessMem->writeCount);
	// The buffer pointer is unique for every new(IO, &buffer) instantiation
	//TEST_ASSERT_EQUAL_PTR(otherTestBuffer, myTest_accessMem->bufferPointer);
	TEST_ASSERT_EQUAL_PTR(otherTestBuffer, myTest_accessMem->actionDone_cb);
	TEST_ASSERT_EQUAL_PTR(otherTestBuffer, myTest_accessMem->objectPointer);

	masterIO = safeDelete(masterIO);
}

TEST(accessMem, copy_returnsNullOnNullSelf)
{
	struct IO * masterIO = new(IO, otherTestBuffer);
	IO_setReadCount(masterIO, 5);
	TEST_ASSERT_EQUAL_PTR(NULL, copy(NULL, masterIO)  );
	masterIO = safeDelete(masterIO);
}

TEST(accessMem, copy_returnsNullOnNullMaster)
{
	struct IO * masterIO = new(IO, otherTestBuffer);
	IO_setReadCount(masterIO, 5);
	TEST_ASSERT_EQUAL_PTR(NULL, copy(myTest_accessMem, NULL) );
	masterIO = safeDelete(masterIO);
}

//****  equal IO_io_equal  ********************

TEST(accessMem, myTest_accessMem_IsEqualTo_myTest_accessMem)
{
	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_accessMem, myTest_accessMem));
}

TEST(accessMem, equal_UnequalAddressReturn_Unequal)
{
	struct IO * masterIO = new(IO, otherTestBuffer);
	IO_setAddress(masterIO, otherTestBuffer);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_accessMem, masterIO) );
	masterIO = safeDelete(masterIO);
}

TEST(accessMem, equal_UnequalIOActionReturn_Unequal)
{
	struct IO * masterIO = new(IO, otherTestBuffer);
	IO_setIOAction(masterIO, IO_READ_SINGLE);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_accessMem, masterIO) );
	masterIO = safeDelete(masterIO);
}

TEST(accessMem, equal_UnequalReadCountReturn_Unequal)
{
	struct IO * masterIO = new(IO, otherTestBuffer);
	IO_setReadCount(masterIO, 5);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_accessMem, masterIO) );
	masterIO = safeDelete(masterIO);
}

TEST(accessMem, equal_UnequalWriteCountReturn_Unequal)
{
	struct IO * masterIO = new(IO, otherTestBuffer);
	IO_setWriteCount(masterIO, 6);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_accessMem, masterIO) );
	masterIO = safeDelete(masterIO);
}

TEST(accessMem, equal_UnequalBufferPointerReturn_Equal)
{
	struct IO * masterIO = new(IO, otherTestBuffer);
	IO_setBufferPointer(masterIO, otherTestBuffer);
	// bufferPointers are unique and will not trigger OBJECT_UNEQUAL
	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_accessMem, masterIO) );
	masterIO = safeDelete(masterIO);
}

TEST(accessMem, equal_UnequalBufferSizeReturn_Unequal)
{
	struct IO * masterIO = new(IO, otherTestBuffer);
	IO_setBufferSize(masterIO, myTest_accessMem->bufferSize + 1 );
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_accessMem, masterIO) );
	masterIO = safeDelete(masterIO);
}

TEST(accessMem, equal_UnequalActionDoneCB_Unequal)
{
	struct IO * masterIO = new(IO, otherTestBuffer);
	IO_set_actionDone_cb(masterIO, (io_cb_fnct)otherTestBuffer);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_accessMem, masterIO) );
	masterIO = safeDelete(masterIO);
}

TEST(accessMem, equal_UnequalObjectPointerUnequal)
{
	struct IO * masterIO = new(IO, otherTestBuffer);
	IO_setObjectPointer(masterIO, otherTestBuffer);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_accessMem, masterIO) );
	masterIO = safeDelete(masterIO);
}

TEST(accessMem, equal_NullReturns_Null)
{
	struct IO * masterIO = new(IO, otherTestBuffer);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_accessMem, NULL));
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(NULL, masterIO ));
	masterIO = safeDelete(masterIO);
}

TEST(accessMem, equal_CopiedSensorReturnsEqual)
{
	struct IO * masterIO = new(IO, otherTestBuffer);
	IO_setAddress       (masterIO, otherTestBuffer);
	IO_setIOAction      (masterIO, IO_WRITE_SINGLE);
	IO_setReadCount     (masterIO, 5);
	IO_setWriteCount    (masterIO, 6);
	//IO_setBufferPointer (masterIO, otherTestBuffer);  // set in new(IO, bufferAddress);
	IO_set_actionDone_cb(masterIO, (io_cb_fnct)otherTestBuffer);
	IO_setObjectPointer (masterIO, otherTestBuffer);

	copy(myTest_accessMem, masterIO);

	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_accessMem, masterIO));
	masterIO = safeDelete(masterIO);
}

//****  IO_addWriteValue  ********************

TEST(accessMem, IO_addWriteValue_Returns_selfOnSuccess)
{
	TEST_ASSERT_EQUAL(myTest_accessMem, IO_addWriteCommandToSequence(myTest_accessMem, 0xFF));
}

TEST(accessMem, IO_addWriteValue_incrementsWriteCountOnSingleAdd)
{
	IO_addWriteCommandToSequence(myTest_accessMem, 0xFF);
	TEST_ASSERT_EQUAL(1, myTest_accessMem->writeCount);
}

TEST(accessMem, IO_addWriteValue_addsSingleValue)
{
	IO_addWriteCommandToSequence(myTest_accessMem, 0xFF);
	TEST_ASSERT_EQUAL(0xFF, myTest_accessMem->bufferPointer[0] );
}

TEST(accessMem, IO_addWriteValue_addsMultipleValues)
{
	IO_addWriteCommandToSequence(myTest_accessMem, 0xFF);
	IO_addWriteCommandToSequence(myTest_accessMem, 0xAA);
	IO_addWriteCommandToSequence(myTest_accessMem, 0x77);
	TEST_ASSERT_EQUAL(0xFF, myTest_accessMem->bufferPointer[0] );
	TEST_ASSERT_EQUAL(0xAA, myTest_accessMem->bufferPointer[1] );
	TEST_ASSERT_EQUAL(0x77, myTest_accessMem->bufferPointer[2] );
}

TEST(accessMem, IO_addWriteValue_incrementsWriteCountOnMultipleAdd)
{
	IO_addWriteCommandToSequence(myTest_accessMem, 0xFF);
	IO_addWriteCommandToSequence(myTest_accessMem, 0xAA);
	IO_addWriteCommandToSequence(myTest_accessMem, 0x77);
	TEST_ASSERT_EQUAL(3, myTest_accessMem->writeCount );
}

TEST(accessMem, IO_addWriteValue_AddingMoreThanBufferSizeReturnsNULL)
{
	// pre-load the command buffer with max number of values
	int i;
	for ( i = 0; i < ACCESS_COMMAND_BUFFER_SIZE; i++ ) {
		IO_addWriteCommandToSequence( myTest_accessMem, (io_data_t)i );
	}

	// add one too many values
	TEST_ASSERT_EQUAL(NULL, IO_addWriteCommandToSequence( myTest_accessMem, 0x00 ) );
}

//****  IO_addIOActionToList  *********************

TEST(accessMem, IO_addIOActionToList_returnsNullOnNullPtr)
{
	TEST_ASSERT_EQUAL(NULL, IO_addIOSequenceToList(NULL) );
}

TEST(accessMem, IO_addIOActionToList_Returns_selfOnSuccess)
{
	TEST_ASSERT_EQUAL(myTest_accessMem, IO_addIOSequenceToList(myTest_accessMem) );
}

TEST(accessMem, IO_addIOActionToList_addsSingleIOObject)
{
	IO_addIOSequenceToList(myTest_accessMem);
	TEST_ASSERT_EQUAL(myTest_accessMem, take(accessMemTest_ioActionList) );
}

TEST(accessMem, IO_addIOActionToList_addsMultipleIOObjects)
{
	struct IO * myTest_accessMem_1 = new(IO, testBuffer);
	struct IO * myTest_accessMem_2 = new(IO, testBuffer);
	struct IO * myTest_accessMem_3 = new(IO, testBuffer);
	IO_addIOSequenceToList(myTest_accessMem_1);
	IO_addIOSequenceToList(myTest_accessMem_2);
	IO_addIOSequenceToList(myTest_accessMem_3);
	TEST_ASSERT_EQUAL(myTest_accessMem_1, take(accessMemTest_ioActionList) );
	TEST_ASSERT_EQUAL(myTest_accessMem_2, take(accessMemTest_ioActionList) );
	TEST_ASSERT_EQUAL(myTest_accessMem_3, take(accessMemTest_ioActionList) );
	TEST_ASSERT_EQUAL(NULL, take(accessMemTest_ioActionList) );
	myTest_accessMem_1 = safeDelete(myTest_accessMem_1);
	myTest_accessMem_2 = safeDelete(myTest_accessMem_2);
	myTest_accessMem_3 = safeDelete(myTest_accessMem_3);
}

//****  IO_getActionFromList  *********************

TEST(accessMem, IO_getActionFromList_returnsNullOnNullPtr)
{
	TEST_ASSERT_EQUAL(NULL, IO_getActionFromList() );
}

TEST(accessMem, IO_getActionFromList_Returns_NullOnemptyList)
{
	TEST_ASSERT_EQUAL(NULL, IO_getActionFromList() );
}

TEST(accessMem, IO_getActionFromList_Returns_ioObjectFromList)
{
	IO_addIOSequenceToList(myTest_accessMem);
	TEST_ASSERT_EQUAL(myTest_accessMem, IO_getActionFromList() );
}

TEST(accessMem, IO_getActionFromList_Returns_MultipleioObjectsFromList)
{
	struct IO * myTest_accessMem_1 = new(IO, testBuffer);
	struct IO * myTest_accessMem_2 = new(IO, testBuffer);
	struct IO * myTest_accessMem_3 = new(IO, testBuffer);
	IO_addIOSequenceToList(myTest_accessMem_1);
	IO_addIOSequenceToList(myTest_accessMem_2);
	IO_addIOSequenceToList(myTest_accessMem_3);
	TEST_ASSERT_EQUAL(myTest_accessMem_1, IO_getActionFromList() );
	TEST_ASSERT_EQUAL(myTest_accessMem_2, IO_getActionFromList() );
	TEST_ASSERT_EQUAL(myTest_accessMem_3, IO_getActionFromList() );
	TEST_ASSERT_EQUAL(NULL, take(accessMemTest_ioActionList) );
	myTest_accessMem_1 = safeDelete(myTest_accessMem_1);
	myTest_accessMem_2 = safeDelete(myTest_accessMem_2);
	myTest_accessMem_3 = safeDelete(myTest_accessMem_3);
}

//****  IO_processSequence  ****************

TEST(accessMem, IO_processSequence_Returns_selfOnSuccess)
{
	IO_addWriteCommandToSequence(myTest_accessMem, 0xFF);
	IO_setAddress     (myTest_accessMem, otherTestBuffer);
	IO_setIOAction    (myTest_accessMem, IO_WRITE_SINGLE);
	IO_processSequence(myTest_accessMem);
	TEST_ASSERT_EQUAL(myTest_accessMem, IO_processSequence(myTest_accessMem) );
}

TEST(accessMem, IO_processSequence_writeSingleToSingleAddress)
{
	IO_addWriteCommandToSequence(myTest_accessMem, 0xFF);
	IO_setAddress   (myTest_accessMem, otherTestBuffer);
	IO_setIOAction  (myTest_accessMem, IO_WRITE_SINGLE);
	IO_processSequence       (myTest_accessMem);
	TEST_ASSERT_EQUAL(0xFF, otherTestBuffer[0] );
}

TEST(accessMem, IO_processSequence_writeMultipleValuesToSingleaddress)
{
	IO_setAddress   (myTest_accessMem, otherTestBuffer);
	IO_setIOAction  (myTest_accessMem, IO_WRITE_SINGLE);
	IO_addWriteCommandToSequence(myTest_accessMem, 0x01);
	IO_addWriteCommandToSequence(myTest_accessMem, 0x02);
	IO_addWriteCommandToSequence(myTest_accessMem, 0x03);
	IO_processSequence       (myTest_accessMem);
	TEST_ASSERT_EQUAL(0x03, otherTestBuffer[0] );
}



TEST(accessMem, IO_processSequence_writeMultipleValuesToSequentialLocation)
{
	IO_addWriteCommandToSequence(myTest_accessMem, 0x01);
	IO_addWriteCommandToSequence(myTest_accessMem, 0x02);
	IO_addWriteCommandToSequence(myTest_accessMem, 0x03);
	IO_setAddress     (myTest_accessMem, otherTestBuffer);
	IO_setIOAction    (myTest_accessMem, IO_WRITE_SEQUENTIAL);
	IO_processSequence(myTest_accessMem);
	TEST_ASSERT_EQUAL(0x01, otherTestBuffer[0] );
	TEST_ASSERT_EQUAL(0x02, otherTestBuffer[1] );
	TEST_ASSERT_EQUAL(0x03, otherTestBuffer[2] );
}

TEST(accessMem, IO_processSequence_readSingleValue)
{
	otherTestBuffer[0] = 0x01;
	IO_setReadCount (myTest_accessMem, 1);
	IO_setAddress     (myTest_accessMem, otherTestBuffer);
	IO_setIOAction    (myTest_accessMem, IO_READ_SINGLE);
	IO_processSequence(myTest_accessMem);
	TEST_ASSERT_EQUAL(0x01, testBuffer[0] );
}

TEST(accessMem, IO_processSequence_readSequentialMultipleValues)
{
	otherTestBuffer[0] = 0x01;
	otherTestBuffer[1] = 0x02;
	otherTestBuffer[2] = 0x03;
	IO_setReadCount (myTest_accessMem, 3);
	IO_setAddress   (myTest_accessMem, otherTestBuffer);
	IO_setIOAction  (myTest_accessMem, IO_READ_SEQUENTIAL);
	IO_processSequence       (myTest_accessMem);
	TEST_ASSERT_EQUAL(0x01, testBuffer[0] );
	TEST_ASSERT_EQUAL(0x02, testBuffer[1] );
	TEST_ASSERT_EQUAL(0x03, testBuffer[2] );
}

TEST(accessMem, IO_processSequence_readMultipleValuesfromSingleLocation)
{
	otherTestBuffer[0] = 0x01;
	otherTestBuffer[1] = 0x02;
	otherTestBuffer[2] = 0x03;
	IO_setReadCount (myTest_accessMem, 3);
	IO_setAddress   (myTest_accessMem, otherTestBuffer);
	IO_setIOAction  (myTest_accessMem, IO_READ_SINGLE);
	IO_processSequence       (myTest_accessMem);
	TEST_ASSERT_EQUAL(0x01, testBuffer[0] );
	TEST_ASSERT_EQUAL(0x01, testBuffer[1] );
	TEST_ASSERT_EQUAL(0x01, testBuffer[2] );
}


//****  IO_update  *********************

TEST(accessMem, IO_update_writeSingleToSingleAddress)
{
	IO_addWriteCommandToSequence(myTest_accessMem, 0xFF);
	IO_setAddress   (myTest_accessMem, otherTestBuffer);
	IO_setIOAction  (myTest_accessMem, IO_WRITE_SINGLE);
	IO_addIOSequenceToList(myTest_accessMem);
	IO_update();
	IO_update();
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	TEST_ASSERT_EQUAL(0xFF, otherTestBuffer[0] );
}

TEST(accessMem, IO_update_writeMultipleValuesToSingleAddress)
{
	IO_setAddress(myTest_accessMem, otherTestBuffer);
	IO_setIOAction(myTest_accessMem, IO_WRITE_SINGLE);
	IO_addWriteCommandToSequence(myTest_accessMem, 0x01);
	IO_addWriteCommandToSequence(myTest_accessMem, 0x02);
	IO_addWriteCommandToSequence(myTest_accessMem, 0x03);
	IO_addIOSequenceToList(myTest_accessMem);
	IO_update();
	IO_update();
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	TEST_ASSERT_EQUAL(0x03, otherTestBuffer[0] );
}

TEST(accessMem, IO_update_writeMultipleValuesToSequentialLocation)
{
	IO_addWriteCommandToSequence(myTest_accessMem, 0x01);
	IO_addWriteCommandToSequence(myTest_accessMem, 0x02);
	IO_addWriteCommandToSequence(myTest_accessMem, 0x03);
	IO_setAddress     (myTest_accessMem, otherTestBuffer);
	IO_setIOAction    (myTest_accessMem, IO_WRITE_SEQUENTIAL);
	IO_addIOSequenceToList(myTest_accessMem);
	IO_update();
	IO_update();
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	TEST_ASSERT_EQUAL(0x01, otherTestBuffer[0] );
	TEST_ASSERT_EQUAL(0x02, otherTestBuffer[1] );
	TEST_ASSERT_EQUAL(0x03, otherTestBuffer[2] );

}

TEST(accessMem, IO_update_readSingleValue)
{
	otherTestBuffer[0] = 0x01;
	IO_setReadCount (myTest_accessMem, 1);
	IO_setAddress     (myTest_accessMem, otherTestBuffer);
	IO_setIOAction    (myTest_accessMem, IO_READ_SINGLE);
	IO_addIOSequenceToList(myTest_accessMem);
	IO_update();
	IO_update();
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	TEST_ASSERT_EQUAL(0x01, testBuffer[0] );
}

TEST(accessMem, IO_update_readSequentialMultipleValues)
{
	otherTestBuffer[0] = 0x01;
	otherTestBuffer[1] = 0x02;
	otherTestBuffer[2] = 0x03;
	IO_setReadCount (myTest_accessMem, 3);
	IO_setAddress   (myTest_accessMem, otherTestBuffer);
	IO_setIOAction  (myTest_accessMem, IO_READ_SEQUENTIAL);
	IO_addIOSequenceToList(myTest_accessMem);
	IO_update();
	IO_update();
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	TEST_ASSERT_EQUAL(0x01, testBuffer[0] );
	TEST_ASSERT_EQUAL(0x02, testBuffer[1] );
	TEST_ASSERT_EQUAL(0x03, testBuffer[2] );
}

TEST(accessMem, IO_update_readMultipleValuesfromSingleLocation)
{
	otherTestBuffer[0] = 0x01;
	otherTestBuffer[1] = 0x02;
	otherTestBuffer[2] = 0x03;
	IO_setReadCount (myTest_accessMem, 3);
	IO_setAddress   (myTest_accessMem, otherTestBuffer);
	IO_setIOAction  (myTest_accessMem, IO_READ_SINGLE);
	IO_addIOSequenceToList(myTest_accessMem);
	IO_update();
	IO_update();
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	TEST_ASSERT_EQUAL(0x01, testBuffer[0] );
	TEST_ASSERT_EQUAL(0x01, testBuffer[1] );
	TEST_ASSERT_EQUAL(0x01, testBuffer[2] );
}



TEST(accessMem, IO_update_firesCallback)
{
	IO_addWriteCommandToSequence(myTest_accessMem, 0xFF);
	IO_setAddress   (myTest_accessMem, otherTestBuffer);
	IO_setIOAction  (myTest_accessMem, IO_WRITE_SINGLE);
	IO_set_actionDone_cb(myTest_accessMem, (void *)accessMem_test_general_cb);
	IO_setObjectPointer(myTest_accessMem, (void *)myTest_accessMem);
	IO_addIOSequenceToList(myTest_accessMem);
	IO_update();
	IO_update();
	//IO_update(); // <<-- safety call
	TEST_ASSERT_EQUAL(0xFF, otherTestBuffer[0] );
	//IO_sequenceComplete_cb();
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	TEST_ASSERT_EQUAL(1, io_test_cb_count );
}

TEST(accessMem, IO_update_canBeCalledMultipleTimesWithEmplyList)
{
	IO_update();
	IO_update();
	IO_update();
	IO_update();
	IO_update();
	IO_update();
	TEST_ASSERT_EQUAL(0x00, testBuffer[0] );
	TEST_ASSERT_EQUAL(0x00, testBuffer[0] );
	TEST_ASSERT_EQUAL(0x00, testBuffer[1] );
	TEST_ASSERT_EQUAL(0x00, testBuffer[2] );
	TEST_ASSERT_EQUAL(0x00, otherTestBuffer[0] );
	TEST_ASSERT_EQUAL(0x00, otherTestBuffer[1] );
	TEST_ASSERT_EQUAL(0x00, otherTestBuffer[2] );

}


//****  Support Methods  ****************

/**/
void * accessMem_test_general_cb(void * _self)
{
	io_test_cb_count++;
	//printf("  XXX io_test_general_cb\n");
	return _self;
}
