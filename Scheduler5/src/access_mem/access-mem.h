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
//#include "..\..\src\scheduler\scheduler.h"
//#include "..\..\src\lists\lists.h"
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
 * the Access operation.  The void*-void* form was selected over the
 * ACCESS*-ACCESS* form so that external modules can register a callback.
 * For example a sensor can register Sensor_incrementMiniState() using
 * void*-void*.
 */
typedef void * (* access_cb_fnct)(void * _io);


/***********************************************/
/************ protected includes  **************/
/***** must be after externs and typedefs  *****/

#include "access-mem-private.h"


/***********************************************/
/****** application interface functions  *******/

/*!
 *	// initialize the AccessMEM class
 *	Access_init();
 *
 *	// create a command and data buffer
 *	#define ACCESS_COMMAND_BUFFER_SIZE  4
 *	access_data_t dataCommandBuffer[ACCESS_COMMAND_BUFFER_SIZE];
 *
 *	// create an AccessMEM object
 *	// pointer and buffer size assignments are handled automatically
 *	my_accessMem = new(AccessMEM, dataCommandBuffer);
 *	if ( myTest_accessMem == NULL )
 *		{ failure to create object; error handling here }
 *
 *	// set buffer size before setting other Access object data members
 *	Access_setBufferSize(myTest_accessMem, ACCESS_COMMAND_BUFFER_SIZE);
 *
 *	// set the AccessMEM data members before triggering processing
 *  Access_setAddress   (my_accessMem, otherTestBuffer);
 *  Access_setIOAction  (my_accessMem, ACCESS_WRITE_READ_SINGLE);
 *
 *  // add any write commands to the command buffer
 *  Access_addWriteCommandToSequence(my_accessMem, 0x01);
 *  Access_addWriteCommandToSequence(my_accessMem, 0x02);
 *  Access_addWriteCommandToSequence(my_accessMem, 0x03);
 *
 *  // OPTIONAL:  If read operations are executed, the read count is set here.
 *  Access_setReadCount(my_accessMem, 3);
 *
 *  // Set the call back function to fire once Access completes.
 *  // Assumes that a higher level module is calling down into Access.
 *  // The callback function should be from the higher level module.
 *  // The object of the callback, should be a pointer to the higher
 *  // level module reference itself.  This way there is a path back up
 *  // the call chain.
 *  access_cb_fnct Access_setActionDone_cb(my_accessMem, access_cb_fnct _cb);
 *  Access_setObjectPointer(my_accessMem, void * _higherLevelObject);
 *
 *
 *  // All setup is complete.  Proceed to process the sequence.
 *  Access_processSequence(my_accessMem);
 *
 *  { use data etc. }
 *
 *  // Optional: clear the data and command buffer before the next use
 *  Access_clearCommandBuffer(my_accessMem);
 *
 */
void Access_init(void);

/*!
 * Writes communication sequences to the command holding buffer.  Values will
 * be written to IO address when IO_update() fires via the scheduler task.
 * Returns self on success.  The writeCount is automatically managed by add.
 */
void * Access_addWriteCommandToSequence(void * _self, access_data_t _value);

/*!
 * Executes communications sequence.
 */
void * Access_processSequence(void * _self);

// WARNING: this method is not implemented
void * Access_xxxx(void);

/*!
 * PRIVATE: generic Access callback that fires when Access I/O action is complete.
 * The sequence will be marked completed.
 */
void * Access_sequenceComplete_cb(struct AccessMEM * _self);

/*!
 * OPTIONAL: Resets write and read counts indicating that the CMD buffer is
 * clear.  Overwrites any data previously loaded into the buffer area, with zeros.
 */
void * Access_clearCommandBuffer(void * _self);


/******************************/
/****** access methods  *******/

/*!
 * Address for read or write operations.
 * Memory access to a NULL address is ignored.
 * Access_getAddress() should be PRIVATE
 */
void * Access_getAddress(const void * _self);
void * Access_setAddress(      void * _self, void * _address);

//! Type of Access operation
// Access_getIOAction() should be PRIVATE
access_read_write_t Access_getIOAction(const void * _self);
access_read_write_t Access_setIOAction(void * _self,
		                               access_read_write_t _ioAction);

//! Number of values to read.
//! Returns the value set.
//! returns and sets zero if count exceeds buffer size.
// Access_getReadCount() should be PRIVATE
int Access_getReadCount(const void * _self);
int Access_setReadCount(      void * _self, int _readCount);

//! PRIVATE: Number of bytes to write.
//! Automatically managed when adding a command to buffer.
int Access_getWriteCount(const void * _self);
int Access_setWriteCount(      void * _self, int _writeCount);

/*!
 * PRIVATE: pointer to buffer holding data for writes and reads.
 * Write operations are executed first.  Read data overwrites any previous
 * write data in the command buffer.
 * // Access_setBufferPointer() should be PRIVATE
 */
void * Access_getBufferPointer(const void * _self);
void * Access_setBufferPointer(      void * _self, void * _bufferPointer);

/*!
 * Number of values that can be stored in the command buffer.
 * Should be set immediately after new(AccessMEM, _commandBufferPtr); is called
 * TODO: Shift the command buffer to an internally generated buffer
 * Access_getBufferSize() is PRIVATE
 */
int Access_getBufferSize(const void * _self);
int Access_setBufferSize(      void * _self, int _size);

/*!
 * Called when operation is complete if function pointer is not NULL.
 * Passes parameter of objectPointer, accessible through
 * Access_getObjectPointer(), even if this value is NULL.
 * Likely use is to increment a sensor state machine to the next state.
 * This is done by setting the function pointer to the sensor increment
 * process, while the object pointer is set to the specific calling object
 * Access_getActionDone_cb() is PRIVATE
 */
access_cb_fnct Access_getActionDone_cb(const void * _self);
access_cb_fnct Access_setActionDone_cb(      void * _self, access_cb_fnct _cb);

//! Object pointer parameter value for action done callback.
//  Access_getObjectPointer() is PRIVATE
void * Access_getObjectPointer(const void * _self);
void * Access_setObjectPointer(      void * _self, void * _objectPointer);

//! Object pointer parameter value for hardware configuration settings.
void * Access_getHardwareConfig(const void * _self);
void * Access_setHardwareConfig(      void * _self, void * _hardwareConfig);

// used to validate a sequence before being added to an IO list
void * Access_sequenceIsValid(struct AccessMEM * _self);
#endif /* SRC_ACCESS_MEM_ACCESS_MEM_H_ */
