/*
 * io.h
 *
 *  Created on: Sep 30, 2015
 *      Author: Alan
 */

#ifndef SRC_ACCESS_MEM_ACCESS_MEM_H_
#define SRC_ACCESS_MEM_ACCESS_MEM_H_

/***********************************************/
/****** superclass and system includes  ********/

//#include "..\cross_compiler_defs.h"
#include "..\..\src\objects\objects.h"
#include "..\..\src\scheduler\scheduler.h"
#include "..\..\src\lists\lists.h"
//#include "..\..\src\nodes\nodes.h"

/***********************************************/
/**** forward declarations and typedefs  *******/

extern const void * AccessMEM;
extern const void * AccessMEMClass;

/*!
 * Data type for commands and data sent to and from the memory area or sensor.
 */
typedef unsigned char access_data_t;

typedef enum access_read_write_t {
	ACCESS_ACTION_UNKNOWN = -1,
	ACCESS_WRITE_SINGLE,
	ACCESS_WRITE_SEQUENTIAL,
	ACCESS_WRITE_READ_SINGLE,
	ACCESS_WRITE_READ_SEQUENTIAL,
	ACCESS_READ_SINGLE,
	ACCESS_READ_SEQUENTIAL
} access_read_write_t;

/*!
 * The generic callback typedef that takes a void pointer and returns a void
 * pointer.  Registered functions of this type can be called on completion of
 * the IO operation.  The void*-void* form was selected over the IO*-IO* form
 * so that external modules can registers callbacks as well.  For example
 * a sensor can register Sensor_incrementMiniState() using void*-void*.
 */
typedef void * (* access_cb_fnct)(void * _io);



/***********************************************/
/************ protected includes  **************/
/***** must be after externs and typedefs  *****/

#include "access-mem-private.h"


/***********************************************/
/****** application interface functions  *******/

/*!
 *	IO_init(struct List * ioActionList) MUST be called the class before other methods are called...
 *	returns self on success, otherwise returns NULL.
 *
 *	@code
 *	// Create a List where IO will store IO sequences waiting to be executed
 *	void * IO_actionBuffer[4];
 *	struct List * IOTest_ioActionList = new(List, IO_actionBuffer);
 *	IO_init(IOTest_ioActionList);
 *
 *	// Create a new IO object
 * 	struct IO * myIOobject = new(IO, ioActionList);
 *
 *	// Create an area in memory where reads and write are allowed
 *	// Set address to this area
 *	io_data_t allowedMemoryArea[16];
 *	IO_setAddress(myIOobject, allowedMemoryArea);
 *
 *	// Clear the command buffer
 *	IO_clearCommandSequences(myIOobject);
 *
 *	// Add write commands to the buffer (example only)
 *	// Write counts are managed by the add operation
 *	IO_addWriteCommandToSequence(myIOobject, 0x03);
 *	IO_addWriteCommandToSequence(myIOobject, 0x41);
 *
 *	// OPTIONAL:  If read operations are executed, the read count is set here.
 *	IO_setReadCount(myIOobject, 1);
 *
 *	// Set IO action to desired format
 *	IO_setIOAction(myIOobject, IO_WRITE_SEQUENTIAL);
 *
 *	// Set communication complete callback ... fired when communication is done
 *	IO_set_actionDone_cb(localIoStructPtr, (io_cb_fnct) My_CommCompleteDone_CB);
 *	IO_setObjectPointer(localIoStructPtr, (void *)My_Pointer);
 *
 *	// Add the IO sequence to the main IO list IOTest_ioActionList
 *	IO_addIOSequenceToList(myIOobject)
 *
 * 	// Set recurrent scheduler task specifying a suitable period in ticks.
 * 	scheduler_AddTask(IO_update,
 *                    _ticksOfDelay,
 *					  _period);
 *
 *	// Once the communication sequence completes, the callback is fired by IO.
 *	// ... My_CommCompleteDone_CB(My_Pointer);
 *	// The module calling IO can use this callback to initiate next steps.
 *
 *	@endcode
 *
 */
void Access_init(void);

/*!
 * Resets write and read counts indicating that the CMD buffer is clear.
 * Does not delete data previously loaded into the buffer area.
 */
void * Access_clearCommandBuffer(void * _self);

/*!
 * Writes communication sequences to the command holding buffer.  Values will
 * be written to IO address when IO_update()via the scheduler task.  Returns
 * self on success.  The writeCount is automatically managed by add.
 */
void * Access_addWriteCommandToSequence(void * _self, access_data_t _value);

/*!
 * Use AccessMEM_setReadCount(_self, _readCount) to trigger a read sequence.
 * Reads execute immediately after preceding write commands.  The writeCount is
 * set automatically when write commands are added to the command buffer with
 * Access_addWriteCommandToSequence().  WARNING:  Reads will fail if the buffer
 * size is smaller than the read count.
 */

/*!
 * Adds the sequence of commands to the buffer of sequences managed by Access.
 * Returns AccessMEM pointer on success.
 */
void * Access_addIOSequenceToList(void * _self);

/*!
 * Executes communications sequence.
 */
void * Access_processSequence(void * _self);

/*!
 * Generic Access callback that fires when Access I/O action is complete.
 * The sequence will be marked completed.
 */
void * Access_sequenceComplete_cb(struct AccessMEM * _self);



// WARNING: this method is not implemented
void * Access_xxxx(void);


/******************************/
/****** access methods  *******/

/*!
 * Address for read or write operations.
 * Memory access to a NULL address is ignored.
 */
void * Access_getAddress(const void * _self);
void * Access_setAddress(      void * _self, void * _address);

//! Type of Access operation
access_read_write_t Access_getIOAction(const void * _self);
access_read_write_t Access_setIOAction(void * _self,
		                               access_read_write_t _ioAction);

//! Number of values to read.
//! Returns the value set.
//! returns and sets zero if count exceeds buffere size.
int Access_getReadCount(const void * _self);
int Access_setReadCount(      void * _self, int _readCount);

//! Number of bytes to write.
//! Automatically managed when adding a command to buffer.
int Access_getWriteCount(const void * _self);
int Access_setWriteCount(      void * _self, int _writeCount);

/*!
 * Struct List pointer to buffer holding data for writes and reads.
 * Write operations are executed first.  Read data overwrites any previous
 * write data in the command buffer.
 */
void * Access_getBufferPointer(const void * _self);
void * Access_setBufferPointer(      void * _self, void * _bufferPointer);

//! Number of values that can be stored in the command buffer.
int Access_getBufferSize(    const void * _self);
int Access_setBufferSize(          void * _self, int _bufferSize);
int Access_autoUpdateBufferSize(   void * _self);

/*!
 * Called when operation is complete is function pointer is not NULL.
 * Passes parameter of objectPointer, accessible through
 * IO_getObjectPointer(), even if this value is NULL.
 */
access_cb_fnct Access_getActionDone_cb(const void * _self);
access_cb_fnct Access_setActionDone_cb(      void * _self, access_cb_fnct _cb);

//! Object pointer parameter value for action done callback.
void * Access_getObjectPointer(const void * _self);
void * Access_setObjectPointer(      void * _self, void * _objectPointer);

//! Object pointer parameter value for hardware configuration settings.
void * Access_getHardwareConfig(const void * _self);
void * Access_setHardwareConfig(      void * _self, void * _hardwareConfig);

#endif /* SRC_ACCESS_MEM_ACCESS_MEM_H_ */
