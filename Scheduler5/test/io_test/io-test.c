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
struct       IO *      myTest_IO;

// test buffer is a combined memory area for both write and read data
// write data is assumed to be loaded first, and then potentially overwritten by the read operation
#define IO_COMMAND_BUFFER_SIZE  4
io_data_t scratchArrayBuffer[IO_COMMAND_BUFFER_SIZE];
//todo5:
io_data_t * testBuffer;

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
	//IO_init(IOTest_ioListOfSequences);
	//IO_init(NULL);
	IO_init();

	// create the IO object for testing
	myTest_IO = new(IO, IOTest_ioListOfSequences);
	if ( myTest_IO == NULL ) {printf("failed to allocate memory for new(IO, testBuffer)\n"); }

	io_test_cb_count  = 0;

	//scheduler_Create(testTASKS_sensors);

	otherTestBuffer[0] = 0x00;
	otherTestBuffer[1] = 0x00;
	otherTestBuffer[2] = 0x00;

	// create an AccessMEM object
	// WARNING:  must always set the buffer size when creating this object
	myTest_AccessMEM = new(AccessMEM, IO_COMMAND_BUFFER_SIZE);

	// secret access to the internal command buffer for testing
	testBuffer = Access_getBufferPointer(myTest_AccessMEM);
	testBuffer[0] = 0x00;
	testBuffer[1] = 0x00;
	testBuffer[2] = 0x00;
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

TEST(io, bufferPointer_is_IOTest_ioListOfSequences_OnCreate)
{
	TEST_ASSERT_EQUAL_PTR(IOTest_ioListOfSequences,  myTest_IO->ioSequenceList);
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
	int masterArray[IO_COMMAND_BUFFER_SIZE];
	struct List * masterSequenceList = new(List, masterArray);
	struct IO *   masterIO           = new(IO, masterSequenceList);
	TEST_ASSERT_EQUAL_PTR(masterIO,  delete(masterIO));
	masterSequenceList = safeDelete(masterSequenceList);
}

TEST(io, delete_safeDelete_returnsNullOnSuccess)
{
	int masterArray[IO_COMMAND_BUFFER_SIZE];
	struct List * masterSequenceList = new(List, masterArray);
	struct IO *   masterIO           = new(IO, masterSequenceList);
	TEST_ASSERT_EQUAL_PTR(NULL,  safeDelete(masterIO));
	masterSequenceList = safeDelete(masterSequenceList);
}

/****  Set/Get IoSequenceList  ****************/

TEST(io, IO_getIoSequenceList_returns_UnknownOnCreate)
{
	// set to IOTest_ioListOfSequences from the original new(IO, IOTest_ioListOfSequences); in setup
	TEST_ASSERT_EQUAL(IOTest_ioListOfSequences,  IO_getIoSequenceList(myTest_IO) );
}

TEST(io, IO_getIoSequenceList_returns_specificValue)
{
	myTest_IO->ioSequenceList = (void *)testBuffer;
	TEST_ASSERT_EQUAL(testBuffer,  IO_getIoSequenceList(myTest_IO) );
}

TEST(io, IO_setIoSequenceList_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(testBuffer,  IO_setIoSequenceList(myTest_IO, testBuffer));
}

TEST(io, IO_setIoSequenceList_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(NULL,  IO_setIoSequenceList(NULL, testBuffer));
}

TEST(io, IO_setIoSequenceList_canSetSpecificValue)
{
	IO_setIoSequenceList(myTest_IO, testBuffer);
	TEST_ASSERT_EQUAL(testBuffer,  myTest_IO->ioSequenceList);
}


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
	int masterArray[IO_COMMAND_BUFFER_SIZE];
	struct List * masterSequenceList = new(List, masterArray);
	struct IO * masterIO             = new(IO, masterSequenceList);
	Access_setReadCount(masterIO, 5);
	TEST_ASSERT_EQUAL_PTR(myTest_IO, copy(myTest_IO, masterIO));
	masterSequenceList = safeDelete(masterSequenceList);
	masterIO           = safeDelete(masterIO);
}

TEST(io, copy_AllItemsCopiedToSelf)
{
	// NOTE: sensorState, and pointers are unique for every sensor
	int masterArray[IO_COMMAND_BUFFER_SIZE];
	struct List * masterSequenceList = new(List, masterArray);
	struct IO *   masterIO           = new(IO, masterSequenceList);
	IO_setActionDone_cb(masterIO, (access_cb_fnct)otherTestBuffer);
	IO_setObjectPointer(masterIO, masterIO);

	copy(myTest_IO, masterIO);

	// NOTE: Sequence List pointer should be unchanged
	TEST_ASSERT_EQUAL_PTR(IOTest_ioListOfSequences, IO_getIoSequenceList(myTest_IO));
	// the items below should have been copied as expected
	TEST_ASSERT_EQUAL_PTR(otherTestBuffer,    IO_getActionDone_cb(myTest_IO));
	TEST_ASSERT_EQUAL_PTR(masterIO,           IO_getObjectPointer(myTest_IO));

	masterSequenceList = safeDelete(masterSequenceList);
	masterIO           = safeDelete(masterIO);
}

TEST(io, copy_returnsNullOnNullSelf)
{
	int masterArray[IO_COMMAND_BUFFER_SIZE];
	struct List * masterSequenceList = new(List, masterArray);
	struct IO *   masterIO           = new(IO, masterSequenceList);
	IO_setActionDone_cb(masterIO, (access_cb_fnct)otherTestBuffer);
	TEST_ASSERT_EQUAL_PTR(NULL, copy(NULL, masterIO)  );
	masterSequenceList = safeDelete(masterSequenceList);
	masterIO           = safeDelete(masterIO);
}

TEST(io, copy_returnsNullOnNullMaster)
{
	int masterArray[IO_COMMAND_BUFFER_SIZE];
	struct List * masterSequenceList = new(List, masterArray);
	struct IO *   masterIO           = new(IO, masterSequenceList);
	IO_setActionDone_cb(masterIO, (access_cb_fnct)otherTestBuffer);
	TEST_ASSERT_EQUAL_PTR(NULL, copy(myTest_IO, NULL)  );
	masterSequenceList = safeDelete(masterSequenceList);
	masterIO           = safeDelete(masterIO);
}

//****  equal IO_io_equal  ********************

TEST(io, myTest_IO_IsEqualTo_myTest_IO)
{
	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_IO, myTest_IO));
}

TEST(io, equal_UnequalSequenceListPointerReturn_Equal)
{
	int masterArray[IO_COMMAND_BUFFER_SIZE];
	struct List * masterSequenceList = new(List, masterArray);
	struct IO *   masterIO           = new(IO, masterSequenceList);
	// bufferPointers are unique and will not trigger OBJECT_UNEQUAL
	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_IO, masterIO) );
	masterSequenceList = safeDelete(masterSequenceList);
	masterIO           = safeDelete(masterIO);
}

TEST(io, equal_UnequalActionDoneCB_Unequal)
{
	int masterArray[IO_COMMAND_BUFFER_SIZE];
	struct List * masterSequenceList = new(List, masterArray);
	struct IO *   masterIO           = new(IO, masterSequenceList);
	IO_setActionDone_cb(masterIO, (access_cb_fnct)otherTestBuffer);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_IO, masterIO) );
	masterSequenceList = safeDelete(masterSequenceList);
	masterIO           = safeDelete(masterIO);
}

TEST(io, equal_UnequalObjectPointerUnequal)
{
	int masterArray[IO_COMMAND_BUFFER_SIZE];
	struct List * masterSequenceList = new(List, masterArray);
	struct IO *   masterIO           = new(IO, masterSequenceList);
	IO_setObjectPointer(masterIO, otherTestBuffer);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_IO, masterIO) );
	masterSequenceList = safeDelete(masterSequenceList);
	masterIO           = safeDelete(masterIO);
}

TEST(io, equal_NullReturns_Null)
{
	int masterArray[IO_COMMAND_BUFFER_SIZE];
	struct List * masterSequenceList = new(List, masterArray);
	struct IO *   masterIO           = new(IO, masterSequenceList);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_IO, NULL));
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(NULL, masterIO ));
	masterSequenceList = safeDelete(masterSequenceList);
	masterIO           = safeDelete(masterIO);
}

TEST(io, equal_CopiedSensorReturnsEqual)
{
	int masterArray[IO_COMMAND_BUFFER_SIZE];
	struct List * masterSequenceList = new(List, masterArray);
	struct IO *   masterIO           = new(IO, masterSequenceList);
	IO_setActionDone_cb(masterIO, (io_cb_fnct)otherTestBuffer);
	IO_setObjectPointer (masterIO, otherTestBuffer);

	copy(myTest_IO, masterIO);

	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_IO, masterIO));
	masterSequenceList = safeDelete(masterSequenceList);
	masterIO           = safeDelete(masterIO);
}

//****  IO_addIOSequenceToList  *********************

TEST(io, IO_addIOSequenceToList_returnsNullOnNullPtr)
{
	TEST_ASSERT_EQUAL(NULL, IO_addIOSequenceToList(myTest_IO, NULL) );
}

TEST(io, IO_addIOSequenceToList_Returns_selfOnSuccess)
{
	//TODO:  Should not be able to add an invalid sequence to the list ... place check
	TEST_ASSERT_EQUAL(myTest_AccessMEM, IO_addIOSequenceToList(myTest_IO, myTest_AccessMEM) );
}

TEST(io, IO_addIOSequenceToList_takeRemovesItemFromList)
{
	IO_addIOSequenceToList(myTest_IO, myTest_AccessMEM);
	TEST_ASSERT_EQUAL(myTest_AccessMEM, IO_getIOSequenceFromList(myTest_IO) );
}

TEST(io, IO_addIOSequenceToList_listContainsOneItemAfterAdd)
{
	IO_addIOSequenceToList(myTest_IO, myTest_AccessMEM);
	TEST_ASSERT_EQUAL(1, getItemCount(IO_getIoSequenceList(myTest_IO)) );
}

TEST(io, IO_addIOSequenceToList_addsMultipleIOObjects)
{
	struct AccessMEM * myTest_Access_1 = new(AccessMEM, testBuffer);
	struct AccessMEM * myTest_Access_2 = new(AccessMEM, testBuffer);
	struct AccessMEM * myTest_Access_3 = new(AccessMEM, testBuffer);
	IO_addIOSequenceToList(myTest_IO, myTest_Access_1);
	IO_addIOSequenceToList(myTest_IO, myTest_Access_2);
	IO_addIOSequenceToList(myTest_IO, myTest_Access_3);
	TEST_ASSERT_EQUAL(myTest_Access_1, IO_getIOSequenceFromList(myTest_IO) );
	TEST_ASSERT_EQUAL(myTest_Access_2, IO_getIOSequenceFromList(myTest_IO) );
	TEST_ASSERT_EQUAL(myTest_Access_3, IO_getIOSequenceFromList(myTest_IO) );
	TEST_ASSERT_EQUAL(NULL, IO_getIOSequenceFromList(myTest_IO) );
	myTest_Access_1 = safeDelete(myTest_Access_1);
	myTest_Access_2 = safeDelete(myTest_Access_2);
	myTest_Access_3 = safeDelete(myTest_Access_3);
}

//****  IO_getIOSequenceFromList  *********************

TEST(io, IO_getIOSequenceFromList_returnsNullOnNullPtr)
{
	TEST_ASSERT_EQUAL(NULL, IO_getIOSequenceFromList(NULL) );
}

TEST(io, IO_getIOSequenceFromList_Returns_NullOnemptyList)
{
	TEST_ASSERT_EQUAL(NULL, IO_getIOSequenceFromList(myTest_AccessMEM) );
}

TEST(io, IO_getIOSequenceFromList_Returns_objectFromList)
{
	IO_addIOSequenceToList(myTest_IO, myTest_AccessMEM);
	TEST_ASSERT_EQUAL(myTest_AccessMEM, IO_getIOSequenceFromList(myTest_IO) );
}

TEST(io, IO_getIOSequenceFromList_listContainsZeroItemsAfterGet)
{
	IO_addIOSequenceToList(myTest_IO, myTest_AccessMEM);
	IO_getIOSequenceFromList(myTest_IO);
	TEST_ASSERT_EQUAL(0, getItemCount(IO_getIoSequenceList(myTest_IO)) );
}

TEST(io, IO_getIOSequenceFromList_Returns_MultipleioObjectsFromList)
{
	struct AccessMEM * myTest_AccessMEM_1 = new(AccessMEM, IO_COMMAND_BUFFER_SIZE);
	struct AccessMEM * myTest_AccessMEM_2 = new(AccessMEM, IO_COMMAND_BUFFER_SIZE);
	struct AccessMEM * myTest_AccessMEM_3 = new(AccessMEM, IO_COMMAND_BUFFER_SIZE);
	IO_addIOSequenceToList(myTest_IO, myTest_AccessMEM_1);
	IO_addIOSequenceToList(myTest_IO, myTest_AccessMEM_2);
	IO_addIOSequenceToList(myTest_IO, myTest_AccessMEM_3);
	TEST_ASSERT_EQUAL(myTest_AccessMEM_1, IO_getIOSequenceFromList(myTest_IO) );
	TEST_ASSERT_EQUAL(myTest_AccessMEM_2, IO_getIOSequenceFromList(myTest_IO) );
	TEST_ASSERT_EQUAL(myTest_AccessMEM_3, IO_getIOSequenceFromList(myTest_IO) );
	TEST_ASSERT_EQUAL(NULL, IO_getIOSequenceFromList(myTest_IO) );
	myTest_AccessMEM_1 = safeDelete(myTest_AccessMEM_1);
	myTest_AccessMEM_2 = safeDelete(myTest_AccessMEM_2);
	myTest_AccessMEM_3 = safeDelete(myTest_AccessMEM_3);
}


//****  IO_update  *********************

TEST(io, IO_update_writeSingleToSingleAddress)
{
	Access_setIOAction              (myTest_AccessMEM, ACCESS_WRITE_SINGLE);
	Access_addWriteCommandToSequence(myTest_AccessMEM, 0xFF);
	Access_setAddress               (myTest_AccessMEM, otherTestBuffer);
	TEST_ASSERT_EQUAL(myTest_AccessMEM, Access_sequenceIsValid(myTest_AccessMEM));

	IO_addIOSequenceToList(myTest_IO, myTest_AccessMEM);

	IO_update(myTest_IO);
	IO_update(myTest_IO);
	IO_update(myTest_IO); // <<-- safety call
	IO_update(myTest_IO); // <<-- safety call
	IO_update(myTest_IO); // <<-- safety call
	IO_update(myTest_IO); // <<-- safety call
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

	IO_addIOSequenceToList(myTest_IO, myTest_AccessMEM);

	IO_update(myTest_IO);
	IO_update(myTest_IO);
	IO_update(myTest_IO); // <<-- safety call
	IO_update(myTest_IO); // <<-- safety call
	IO_update(myTest_IO); // <<-- safety call
	IO_update(myTest_IO); // <<-- safety call
	TEST_ASSERT_EQUAL(0x03, otherTestBuffer[0] );
}

TEST(io, IO_update_writeMultipleValuesToSequentialLocation)
{
	Access_setIOAction              (myTest_AccessMEM, ACCESS_WRITE_SEQUENTIAL);
	Access_addWriteCommandToSequence(myTest_AccessMEM, 0x01);
	Access_addWriteCommandToSequence(myTest_AccessMEM, 0x02);
	Access_addWriteCommandToSequence(myTest_AccessMEM, 0x03);
	Access_setAddress               (myTest_AccessMEM, otherTestBuffer);
	Access_setActionDone_cb(myTest_AccessMEM, (void *)IO_sequenceComplete_cb);
	Access_setObjectPointer(myTest_AccessMEM, (void *)myTest_AccessMEM);
	TEST_ASSERT_EQUAL(myTest_AccessMEM, Access_sequenceIsValid(myTest_AccessMEM));

	IO_addIOSequenceToList (myTest_IO, myTest_AccessMEM);

	IO_update(myTest_IO);
	IO_update(myTest_IO);
	IO_update(myTest_IO); // <<-- safety call
	IO_update(myTest_IO); // <<-- safety call
	IO_update(myTest_IO); // <<-- safety call
	IO_update(myTest_IO); // <<-- safety call
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

	IO_addIOSequenceToList(myTest_IO, myTest_AccessMEM);

	IO_update(myTest_IO);
	IO_update(myTest_IO);
	IO_update(myTest_IO); // <<-- safety call
	IO_update(myTest_IO); // <<-- safety call
	IO_update(myTest_IO); // <<-- safety call
	IO_update(myTest_IO); // <<-- safety call
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

	IO_addIOSequenceToList(myTest_IO, myTest_AccessMEM);

	IO_update(myTest_IO);
	IO_update(myTest_IO);
	IO_update(myTest_IO); // <<-- safety call
	IO_update(myTest_IO); // <<-- safety call
	IO_update(myTest_IO); // <<-- safety call
	IO_update(myTest_IO); // <<-- safety call
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

	IO_addIOSequenceToList(myTest_IO, myTest_AccessMEM);

	IO_update(myTest_IO);
	IO_update(myTest_IO);
	IO_update(myTest_IO); // <<-- safety call
	IO_update(myTest_IO); // <<-- safety call
	IO_update(myTest_IO); // <<-- safety call
	IO_update(myTest_IO); // <<-- safety call
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

	IO_addIOSequenceToList(myTest_IO, myTest_AccessMEM);

	TEST_ASSERT_EQUAL(0, io_test_cb_count );
	IO_update(myTest_IO);
	IO_update(myTest_IO);
	IO_update(myTest_IO); // <<-- safety call
	IO_update(myTest_IO); // <<-- safety call
	IO_update(myTest_IO); // <<-- safety call
	IO_update(myTest_IO); // <<-- safety call
	TEST_ASSERT_EQUAL(1, io_test_cb_count );
}

TEST(io, IO_update_CallbackDoesNotFireOnWrongIoAction)
{
	Access_setIOAction              (myTest_AccessMEM, ACCESS_ACTION_UNKNOWN);
	Access_addWriteCommandToSequence(myTest_AccessMEM, 0xFF);
	Access_setAddress               (myTest_AccessMEM, otherTestBuffer);
	Access_setActionDone_cb(myTest_AccessMEM, (void *)io_test_general_cb);
	Access_setObjectPointer(myTest_AccessMEM, (void *)myTest_AccessMEM);

	// confirm that sequence is invalid
	TEST_ASSERT_EQUAL(0, Access_sequenceIsValid(myTest_AccessMEM));

	IO_addIOSequenceToList(myTest_IO, myTest_AccessMEM);

	TEST_ASSERT_EQUAL(0, io_test_cb_count );
	IO_update(myTest_IO);
	IO_update(myTest_IO);
	IO_update(myTest_IO); // <<-- safety call
	IO_update(myTest_IO); // <<-- safety call
	IO_update(myTest_IO); // <<-- safety call
	IO_update(myTest_IO); // <<-- safety call
	TEST_ASSERT_EQUAL(0, io_test_cb_count );
}


TEST(io, IO_update_canBeCalledMultipleTimesWithEmplyList)
{
	IO_update(myTest_IO);
	IO_update(myTest_IO);
	IO_update(myTest_IO);
	IO_update(myTest_IO);
	IO_update(myTest_IO);
	IO_update(myTest_IO);

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
