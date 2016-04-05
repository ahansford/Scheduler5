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
//#include "..\..\src\sensors\sensors-private.h"

/****************************************/
/*** Basic Object and Class Testing ****/
struct       IO *      myTest_IO;
//const struct IOClass * myTest_IOClass_PTR;
//const struct Class *       myTest_IO_class_PTR;

// test buffer is a combined memory area for both write and read data
// write data is assumed to be loaded first, and then potentially overwritten by the read operation
#define IO_COMMAND_BUFFER_SIZE  4
io_data_t testBuffer[IO_COMMAND_BUFFER_SIZE];
io_data_t otherTestBuffer[IO_COMMAND_BUFFER_SIZE];
//struct_task_t testTASKS_sensors[SCHEDULER_MAX_TASKS];

void * io_test_general_cb(void * _self);
//struct Sensor * Sensor_test_general_cb2(struct Sensor * _self);
int io_test_cb_count;
//int sensor_test_cb_count2;

//int i;

// pointer to List of sequences
struct List * IOTest_ioListOfSequences = NULL;
void *        IOTest_ioListBuffer[4];

// local Access object pointer
struct AccessMEM * myTest_AccessMEM;


/****************************************/
/*********** Start of Testing ***********/
TEST_GROUP(io);

TEST_SETUP(io)
{
	RuntimeErrorStub_Reset();

	// Initialize the list buffer, the list and IO classes
	IOTest_ioListOfSequences = new(List, IOTest_ioListBuffer);
	IO_init(IOTest_ioListOfSequences);

	// create the IO object for testing
	myTest_IO = new(IO, testBuffer);
	if ( myTest_IO == NULL ) {printf("failed to allocate memory for new(IO, testBuffer)\n"); }

	io_test_cb_count  = 0;

	//sensor_test_cb_count2 = 0;
	//myTest_SensorClass_PTR  = classOf(myTest_Sensor);
	//myTest_Sensor_class_PTR = Sensor;
	//scheduler_Create(testTASKS_sensors);

	otherTestBuffer[0] = 0x00;
	otherTestBuffer[1] = 0x00;
	otherTestBuffer[2] = 0x00;

	testBuffer[0] = 0x00;
	testBuffer[1] = 0x00;
	testBuffer[2] = 0x00;

	// create an AccessMEM object with an EXTERNAL data buffer
	// WARNING:  must always set the buffer size when creating this object
	myTest_AccessMEM = new(AccessMEM, testBuffer);
	Access_setBufferSize(myTest_AccessMEM, IO_COMMAND_BUFFER_SIZE);

}

TEST_TEAR_DOWN(io)
{
	myTest_IO = safeDelete(myTest_IO);
	IOTest_ioListOfSequences = safeDelete(IOTest_ioListOfSequences);


	//for ( i = 0; i < SCHEDULER_MAX_TASKS; i++)
	//	{ scheduler_DeleteTask(i); }


	myTest_AccessMEM = safeDelete(myTest_AccessMEM);

	RuntimeErrorStub_Reset();
}

/**************************************/
/**************  New  *****************/

TEST(io, myTest_io_IsNotNullOnCreate)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  myTest_IO);
}

TEST(io, IO_IsNotNullOnCreate)
{
	const struct IO * local_IO = IO;
	TEST_ASSERT_NOT_EQUAL(NULL,  local_IO);
}

TEST(io, IOClass_IsNotNullOnCreate)
{
	const struct IOClass * local_IOClass = IOClass;
	TEST_ASSERT_NOT_EQUAL(NULL,  local_IOClass);
}

//+++++++++++++++//

TEST(io, ctor_is_IO_io_ctor)
{
	TEST_ASSERT_EQUAL_PTR(IO_io_ctor,  myTest_IO->_.class->ctor);
}

TEST(io, dtor_is_IO_io_dtor)
{
	TEST_ASSERT_EQUAL_PTR(IO_io_dtor,  myTest_IO->_.class->dtor);
}

TEST(io, copy_is_IO_io_copy)
{
	TEST_ASSERT_EQUAL_PTR(IO_io_copy,  myTest_IO->_.class->copy);
}

TEST(io, equal_is_IO_io_equal)
{
	TEST_ASSERT_EQUAL_PTR(IO_io_equal,  myTest_IO->_.class->equal);
}

TEST(io, config_is_IO_io_config)
{
	TEST_ASSERT_EQUAL_PTR(IO_io_config,  myTest_IO->_.class->config);
}

TEST(io, puto_is_IO_io_puto)
{
	TEST_ASSERT_EQUAL_PTR(IO_io_puto,  myTest_IO->_.class->puto);
}

TEST(io, Class_ctor_is_IO_Class_ctor)
{
	const struct IOClass * localIOClass = IOClass;
	TEST_ASSERT_EQUAL_PTR(IOClass_ctor,  localIOClass->_.ctor);
}
/*
TEST(io, IO_write_is_IO_io_write)
{
	//const struct IOClass * localIOClass = classOf(myTest_IO);
	//TEST_ASSERT_EQUAL_PTR(IO_io_addWriteValue,  localIOClass->IO_addWriteValue);
}

TEST(io, IO_writeRead_is_IO_io_writeRead)
{
	const struct IOClass * localIOClass = classOf(myTest_IO);
	TEST_ASSERT_EQUAL_PTR(IO_io_processSequence,  localIOClass->IO_processSequence);
}

TEST(io, IO_read_is_IO_io_read)
{
	const struct IOClass * localIOClass = classOf(myTest_IO);
	TEST_ASSERT_EQUAL_PTR(IO_io_xxxx,  localIOClass->IO_xxxx);
}
*/

/*
TEST(io, address_is_Null_OnCreate)
{
	TEST_ASSERT_EQUAL_PTR(NULL,  myTest_IO->address);
}

TEST(io, ioAction_is_UNKNOWN_OnCreate)
{
	TEST_ASSERT_EQUAL(IO_ACTION_UNKNOWN,  myTest_IO->ioAction);
}

TEST(io, readCount_is_Zero_OnCreate)
{
	TEST_ASSERT_EQUAL(0,  myTest_IO->readCount);
}

TEST(io, writeCount_is_Zero_OnCreate)
{
	TEST_ASSERT_EQUAL(0,  myTest_IO->writeCount);
}
*/

TEST(io, bufferPointer_is_testBuffer_OnCreate)
{
	TEST_ASSERT_EQUAL_PTR(testBuffer,  myTest_IO->bufferPointer);
}

TEST(io, IO_actionComplete_cb_is_Null_OnCreate)
{
	TEST_ASSERT_EQUAL_PTR(NULL,  myTest_IO->actionDone_cb);
}

TEST(io, objectPointer_is_Null_OnCreate)
{
	TEST_ASSERT_EQUAL_PTR(NULL,  myTest_IO->objectPointer);
}

/****  delete/Sensor_dtor  ****************/

TEST(io, delete_returns_SelfOnSuccess)
{
	struct IO * otherIO = new(IO, otherTestBuffer);
	TEST_ASSERT_EQUAL_PTR(otherIO,  delete(otherIO));
}

TEST(io, delete_safeDelete_returnsNullOnSuccess)
{
	struct IO * otherIO = new(IO, otherTestBuffer);
	TEST_ASSERT_EQUAL_PTR(NULL,  safeDelete(otherIO));
}

/****  Set/Get bufferPointer  ****************/
/*
TEST(io, IO_getBufferPointer_returns_UnknownOnCreate)
{
	// set to testBuffer from the original new(IO, testBuffer); in setup
	TEST_ASSERT_EQUAL(testBuffer,  IO_getBufferPointer(myTest_IO) );
}

TEST(io, IO_getBufferPointer_returns_specificValue)
{
	myTest_IO->bufferPointer = (void *)testBuffer;
	TEST_ASSERT_EQUAL(testBuffer,  IO_getBufferPointer(myTest_IO) );
}

TEST(io, IO_setBufferPointer_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(testBuffer,  IO_setBufferPointer(myTest_IO, testBuffer));
}

TEST(io, IO_setBufferPointer_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(NULL,  IO_setBufferPointer(NULL, testBuffer));
}

TEST(io, IO_setBufferPointer_canSetSpecificValue)
{
	IO_setBufferPointer(myTest_IO, testBuffer);
	TEST_ASSERT_EQUAL(testBuffer,  myTest_IO->bufferPointer);
}
*/
/****  Set/Get bufferSize  ****************/
/*
TEST(io, IO_getBufferSize_returns_DefineValueOnCreate)
{
	TEST_ASSERT_EQUAL(IO_COMMAND_BUFFER_SIZE,  IO_getBufferSize(myTest_IO) );
}

TEST(io, IO_getBufferSize_returns_specificValue)
{
	myTest_IO->bufferSize = 1;
	TEST_ASSERT_EQUAL(1,  IO_getBufferSize(myTest_IO) );
}

TEST(io, IO_setBufferSize_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(2,  IO_setBufferSize(myTest_IO, 2));
}

TEST(io, IO_setBufferSize_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(0,  IO_setBufferSize(NULL, 3));
}

TEST(io, IO_setBufferSize_canSetSpecificValue)
{
	IO_setBufferSize(myTest_IO, 4);
	TEST_ASSERT_EQUAL(4,  myTest_IO->bufferSize);
}
*/
/****  Set/Get ActionComplete_cb  ****************/
/**/
TEST(io, IO_getIO_actionComplete_cb_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(NULL,  IO_getActionDone_cb(myTest_IO) );
}

TEST(io, IO_getIO_actionComplete_cb_returns_specificValue)
{
	myTest_IO->actionDone_cb = (void *)testBuffer;
	TEST_ASSERT_EQUAL(testBuffer,  IO_getActionDone_cb(myTest_IO) );
}

TEST(io, IO_setIO_actionComplete_cb_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(testBuffer,  IO_setActionDone_cb(myTest_IO, (io_cb_fnct)testBuffer));
}

TEST(io, IO_setIO_actionComplete_cb_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(NULL,  IO_setActionDone_cb(NULL, (io_cb_fnct)testBuffer));
}

TEST(io, IO_setIO_actionComplete_cb_canSetSpecificValue)
{
	IO_setActionDone_cb(myTest_IO, (io_cb_fnct)testBuffer);
	TEST_ASSERT_EQUAL(testBuffer,  myTest_IO->actionDone_cb);
}


/****  Set/Get objectPointer  ****************/
/**/
TEST(io, IO_getObjectPointer_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(NULL,  IO_getObjectPointer(myTest_IO) );
}

TEST(io, IO_getObjectPointer_returns_specificValue)
{
	myTest_IO->objectPointer = (void *)testBuffer;
	TEST_ASSERT_EQUAL(testBuffer,  IO_getObjectPointer(myTest_IO) );
}

TEST(io, IO_setObjectPointer_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(testBuffer,  IO_setObjectPointer(myTest_IO, testBuffer));
}

TEST(io, IO_setObjectPointer_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(NULL,  IO_setObjectPointer(NULL, testBuffer));
}

TEST(io, IO_setObjectPointer_canSetSpecificValue)
{
	IO_setObjectPointer(myTest_IO, testBuffer);
	TEST_ASSERT_EQUAL(testBuffer,  myTest_IO->objectPointer);
}

//****  copy IO_io_copy  ****************

TEST(io, copy_returnsSelfOnSuccess)
{
	struct IO * masterIO = new(IO, otherTestBuffer);
	Access_setReadCount(masterIO, 5);
	TEST_ASSERT_EQUAL_PTR(myTest_IO, copy(myTest_IO, masterIO));
	masterIO = safeDelete(masterIO);
}

TEST(io, copy_AllItemsCopiedToSelf)
{
	// NOTE: sensorState, and pointers are unique for every sensor
	struct IO * masterIO = new(IO, otherTestBuffer);
	//IO_setAddress       (masterIO, otherTestBuffer);
	//IO_setIOAction      (masterIO, ACCESS_WRITE_SINGLE);
	//IO_setReadCount     (masterIO, 5);
	//IO_setWriteCount    (masterIO, 6);
	//IO_setBufferPointer (masterIO, otherTestBuffer);
	IO_setActionDone_cb(masterIO, (access_cb_fnct)otherTestBuffer);
	IO_setObjectPointer(masterIO, otherTestBuffer);

	copy(myTest_IO, masterIO);

	// NOTE: sensorState, and pointers are unique for every sensor
	//TEST_ASSERT_EQUAL(otherTestBuffer,     myTest_IO->address);
	//TEST_ASSERT_EQUAL(IO_WRITE_SINGLE,            myTest_IO->ioAction);
	//TEST_ASSERT_EQUAL(5,                   myTest_IO->readCount);
	//TEST_ASSERT_EQUAL(6,                   myTest_IO->writeCount);
	//// The buffer pointer is unique for every new(IO, &buffer) instantiation
	////TEST_ASSERT_EQUAL_PTR(otherTestBuffer, myTest_IO->bufferPointer);
	TEST_ASSERT_EQUAL_PTR(otherTestBuffer, myTest_IO->actionDone_cb);
	TEST_ASSERT_EQUAL_PTR(otherTestBuffer, myTest_IO->objectPointer);

	masterIO = safeDelete(masterIO);
}

TEST(io, copy_returnsNullOnNullSelf)
{
	struct AccessMEM * masterIO = new(AccessMEM, otherTestBuffer);
	Access_setReadCount(masterIO, 5);
	TEST_ASSERT_EQUAL_PTR(NULL, copy(NULL, masterIO)  );
	masterIO = safeDelete(masterIO);
}

TEST(io, copy_returnsNullOnNullMaster)
{
	struct AccessMEM * masterIO = new(AccessMEM, otherTestBuffer);
	Access_setReadCount(masterIO, 5);
	TEST_ASSERT_EQUAL_PTR(NULL, copy(myTest_IO, NULL) );
	masterIO = safeDelete(masterIO);
}

//****  equal IO_io_equal  ********************

TEST(io, myTest_IO_IsEqualTo_myTest_IO)
{
	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_IO, myTest_IO));
}

/*
TEST(io, equal_UnequalBufferPointerReturn_Equal)
{
	struct AccessMEM * masterIO = new(AccessMEM, otherTestBuffer);
	Access_setBufferPointer(masterIO, otherTestBuffer);
	// bufferPointers are unique and will not trigger OBJECT_UNEQUAL
	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_IO, masterIO) );
	masterIO = safeDelete(masterIO);
}

TEST(io, equal_UnequalBufferSizeReturn_Unequal)
{
	struct AccessMEM * masterIO = new(AccessMEM, otherTestBuffer);
	Access_setBufferSize(masterIO, myTest_IO->bufferSize + 1 );
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_IO, masterIO) );
	masterIO = safeDelete(masterIO);
}
*/
TEST(io, equal_UnequalActionDoneCB_Unequal)
{
	struct AccessMEM * masterIO = new(AccessMEM, otherTestBuffer);
	Access_setActionDone_cb(masterIO, (access_cb_fnct)otherTestBuffer);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_IO, masterIO) );
	masterIO = safeDelete(masterIO);
}

TEST(io, equal_UnequalObjectPointerUnequal)
{
	struct AccessMEM * masterIO = new(AccessMEM, otherTestBuffer);
	Access_setObjectPointer(masterIO, otherTestBuffer);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_IO, masterIO) );
	masterIO = safeDelete(masterIO);
}

TEST(io, equal_NullReturns_Null)
{
	struct AccessMEM * masterIO = new(AccessMEM, otherTestBuffer);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_IO, NULL));
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(NULL, masterIO ));
	masterIO = safeDelete(masterIO);
}

TEST(io, equal_CopiedSensorReturnsEqual)
{
	struct IO * masterIO = new(IO, otherTestBuffer);
	//Access_setAddress       (masterIO, otherTestBuffer);
	//Access_setIOAction      (masterIO, ACCESS_WRITE_SINGLE);
	//Access_setReadCount     (masterIO, 5);
	//Access_setWriteCount    (masterIO, 6);
	//IO_setBufferPointer (masterIO, otherTestBuffer);  // set in new(IO, bufferAddress);
	IO_setActionDone_cb(masterIO, (io_cb_fnct)otherTestBuffer);
	IO_setObjectPointer (masterIO, otherTestBuffer);

	copy(myTest_IO, masterIO);

	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_IO, masterIO));
	masterIO = safeDelete(masterIO);
}

//****  IO_addIOSequenceToList  *********************

TEST(io, IO_addIOSequenceToList_returnsNullOnNullPtr)
{
	TEST_ASSERT_EQUAL(NULL, IO_addIOSequenceToList(NULL) );
}

TEST(io, IO_addIOSequenceToList_Returns_selfOnSuccess)
{
	TEST_ASSERT_EQUAL(myTest_AccessMEM, IO_addIOSequenceToList(myTest_AccessMEM) );
}

TEST(io, IO_addIOSequenceToList_takeRemovesItemFromList)
{
	IO_addIOSequenceToList(myTest_AccessMEM);
	TEST_ASSERT_EQUAL(myTest_AccessMEM, take(IOTest_ioListOfSequences) );
}

TEST(io, IO_addIOSequenceToList_listContainsOneItemAfterAdd)
{
	IO_addIOSequenceToList(myTest_AccessMEM);
	TEST_ASSERT_EQUAL(1, getItemCount(IOTest_ioListOfSequences) );
}

TEST(io, IO_addIOSequenceToList_addsMultipleIOObjects)
{
	struct AccessMEM * myTest_Access_1 = new(AccessMEM, testBuffer);
	struct AccessMEM * myTest_Access_2 = new(AccessMEM, testBuffer);
	struct AccessMEM * myTest_Access_3 = new(AccessMEM, testBuffer);
	IO_addIOSequenceToList(myTest_Access_1);
	IO_addIOSequenceToList(myTest_Access_2);
	IO_addIOSequenceToList(myTest_Access_3);
	TEST_ASSERT_EQUAL(myTest_Access_1, take(IOTest_ioListOfSequences) );
	TEST_ASSERT_EQUAL(myTest_Access_2, take(IOTest_ioListOfSequences) );
	TEST_ASSERT_EQUAL(myTest_Access_3, take(IOTest_ioListOfSequences) );
	TEST_ASSERT_EQUAL(NULL, take(IOTest_ioListOfSequences) );
	myTest_Access_1 = safeDelete(myTest_Access_1);
	myTest_Access_2 = safeDelete(myTest_Access_2);
	myTest_Access_3 = safeDelete(myTest_Access_3);
}

//****  IO_getIOSequenceFromList  *********************

TEST(io, IO_getIOSequenceFromList_returnsNullOnNullPtr)
{
	TEST_ASSERT_EQUAL(NULL, IO_getIOSequenceFromList() );
}

TEST(io, IO_getIOSequenceFromList_Returns_NullOnemptyList)
{
	TEST_ASSERT_EQUAL(NULL, IO_getIOSequenceFromList() );
}

TEST(io, IO_getIOSequenceFromList_Returns_objectFromList)
{
	IO_addIOSequenceToList(myTest_AccessMEM);
	TEST_ASSERT_EQUAL(myTest_AccessMEM, IO_getIOSequenceFromList() );
}

TEST(io, IO_getIOSequenceFromList_listContainsZeroItemsAfterGet)
{
	IO_addIOSequenceToList(myTest_AccessMEM);
	IO_getIOSequenceFromList();
	TEST_ASSERT_EQUAL(0, getItemCount(IOTest_ioListOfSequences) );
}

TEST(io, IO_getIOSequenceFromList_Returns_MultipleioObjectsFromList)
{
	struct AccessMEM * myTest_IO_1 = new(AccessMEM, testBuffer);
	struct AccessMEM * myTest_IO_2 = new(AccessMEM, testBuffer);
	struct AccessMEM * myTest_IO_3 = new(AccessMEM, testBuffer);
	IO_addIOSequenceToList(myTest_IO_1);
	IO_addIOSequenceToList(myTest_IO_2);
	IO_addIOSequenceToList(myTest_IO_3);
	TEST_ASSERT_EQUAL(myTest_IO_1, IO_getIOSequenceFromList() );
	TEST_ASSERT_EQUAL(myTest_IO_2, IO_getIOSequenceFromList() );
	TEST_ASSERT_EQUAL(myTest_IO_3, IO_getIOSequenceFromList() );
	TEST_ASSERT_EQUAL(NULL, take(IOTest_ioListOfSequences) );
	myTest_IO_1 = safeDelete(myTest_IO_1);
	myTest_IO_2 = safeDelete(myTest_IO_2);
	myTest_IO_3 = safeDelete(myTest_IO_3);
}


//****  IO_update  *********************

TEST(io, IO_update_writeSingleToSingleAddress)
{
	Access_setIOAction              (myTest_AccessMEM, ACCESS_WRITE_SINGLE);
	Access_addWriteCommandToSequence(myTest_AccessMEM, 0xFF);
	Access_setAddress               (myTest_AccessMEM, otherTestBuffer);
	//Access_setActionDone_cb         (myTest_AccessMEM, IO_sequenceComplete_cb);
	//Access_setObjectPointer         (myTest_AccessMEM, (void *)myTest_AccessMEM);
	TEST_ASSERT_EQUAL(myTest_AccessMEM, Access_sequenceIsValid(myTest_AccessMEM));

	IO_addIOSequenceToList(myTest_AccessMEM);

	IO_update();
	IO_update();
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	TEST_ASSERT_EQUAL(0xFF, otherTestBuffer[0] );
}

TEST(io, IO_update_writeMultipleValuesToSingleAddress)
{
	Access_setIOAction              (myTest_AccessMEM, ACCESS_WRITE_SINGLE);
	Access_addWriteCommandToSequence(myTest_AccessMEM, 0x01);
	Access_addWriteCommandToSequence(myTest_AccessMEM, 0x02);
	Access_addWriteCommandToSequence(myTest_AccessMEM, 0x03);
	Access_setAddress               (myTest_AccessMEM, otherTestBuffer);
	TEST_ASSERT_EQUAL(myTest_AccessMEM, Access_sequenceIsValid(myTest_AccessMEM));

	IO_addIOSequenceToList(myTest_AccessMEM);

	IO_update();
	IO_update();
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	TEST_ASSERT_EQUAL(0x03, otherTestBuffer[0] );
}

TEST(io, IO_update_writeMultipleValuesToSequentialLocation)
{
	Access_setIOAction             (myTest_AccessMEM, ACCESS_WRITE_SEQUENTIAL);
	Access_addWriteCommandToSequence(myTest_AccessMEM, 0x01);
	Access_addWriteCommandToSequence(myTest_AccessMEM, 0x02);
	Access_addWriteCommandToSequence(myTest_AccessMEM, 0x03);
	Access_setAddress               (myTest_AccessMEM, otherTestBuffer);
	Access_setActionDone_cb(myTest_AccessMEM, (void *)IO_sequenceComplete_cb);
	Access_setObjectPointer(myTest_AccessMEM, (void *)myTest_AccessMEM);
	TEST_ASSERT_EQUAL(myTest_AccessMEM, Access_sequenceIsValid(myTest_AccessMEM));

	IO_addIOSequenceToList (myTest_AccessMEM);

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

TEST(io, IO_update_readSingleValue)
{
	Access_setIOAction (myTest_AccessMEM, ACCESS_READ_SINGLE);
	Access_setAddress  (myTest_AccessMEM, otherTestBuffer);
	Access_setReadCount(myTest_AccessMEM, 1);
	otherTestBuffer[0] = 0x01;
	TEST_ASSERT_EQUAL(myTest_AccessMEM, Access_sequenceIsValid(myTest_AccessMEM));

	IO_addIOSequenceToList(myTest_AccessMEM);

	IO_update();
	IO_update();
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	TEST_ASSERT_EQUAL(0x01, testBuffer[0] );
}

TEST(io, IO_update_readSequentialMultipleValues)
{
	Access_setIOAction (myTest_AccessMEM, ACCESS_READ_SEQUENTIAL);
	Access_setAddress  (myTest_AccessMEM, otherTestBuffer);
	Access_setReadCount(myTest_AccessMEM, 3);
	otherTestBuffer[0] = 0x01;
	otherTestBuffer[1] = 0x02;
	otherTestBuffer[2] = 0x03;

	TEST_ASSERT_EQUAL(myTest_AccessMEM, Access_sequenceIsValid(myTest_AccessMEM));

	IO_addIOSequenceToList(myTest_AccessMEM);

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

TEST(io, IO_update_readMultipleValuesfromSingleLocation)
{
	Access_setIOAction  (myTest_AccessMEM, ACCESS_READ_SINGLE);
	Access_setAddress   (myTest_AccessMEM, otherTestBuffer);
	Access_setReadCount (myTest_AccessMEM, 3);
	otherTestBuffer[0] = 0x01;
	otherTestBuffer[1] = 0x02;
	otherTestBuffer[2] = 0x03;
	TEST_ASSERT_EQUAL(myTest_AccessMEM, Access_sequenceIsValid(myTest_AccessMEM));

	IO_addIOSequenceToList(myTest_AccessMEM);

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

TEST(io, IO_update_firesCallback)
{
	Access_setIOAction              (myTest_AccessMEM, ACCESS_WRITE_SINGLE);
	Access_addWriteCommandToSequence(myTest_AccessMEM, 0xFF);
	Access_setAddress               (myTest_AccessMEM, otherTestBuffer);
	Access_setActionDone_cb(myTest_AccessMEM, (void *)io_test_general_cb);
	Access_setObjectPointer(myTest_AccessMEM, (void *)myTest_AccessMEM);
	TEST_ASSERT_EQUAL(myTest_AccessMEM, Access_sequenceIsValid(myTest_AccessMEM));

	IO_addIOSequenceToList(myTest_AccessMEM);

	TEST_ASSERT_EQUAL(0, io_test_cb_count );
	IO_update();
	IO_update();
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	IO_update(); // <<-- safety call
	TEST_ASSERT_EQUAL(1, io_test_cb_count );
}

TEST(io, IO_update_canBeCalledMultipleTimesWithEmplyList)
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

void * io_test_general_cb(void * _self)
{
	io_test_cb_count++;
	//printf("  XXX io_test_general_cb\n");
	return _self;
}
