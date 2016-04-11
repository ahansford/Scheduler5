/*
 * io.h
 *
 *  Created on: Sep 30, 2015
 *      Author: Alan
 */

#ifndef SRC_IO_IO_H_
#define SRC_IO_IO_H_

/***********************************************/
/****** superclass and system includes  ********/

//#include "..\cross_compiler_defs.h"
#include "..\..\src\objects\objects.h"
#include "..\..\src\scheduler\scheduler.h"
//#include "..\..\src\nodes\nodes.h"
#include "..\..\src\lists\lists.h"
#include "..\..\src\access_mem\access-mem.h"
/***********************************************/
/**** forward declarations and typedefs  *******/

extern const void * IO;
extern const void * IOClass;

/*!
 * Data type for commands and data sent to and from the memory area or sensor.
 */
typedef unsigned char io_data_t;

/*
typedef enum io_read_write_t {
	IO_ACTION_UNKNOWN = -1,
	//IO_WRITE_SINGLE,
	//IO_WRITE_SEQUENTIAL,
	//IO_WRITE_READ_SINGLE,
	//IO_WRITE_READ_SEQUENTIAL,
	//IO_READ_SINGLE,
	//IO_READ_SEQUENTIAL
} io_read_write_t;
*/
/*!
 * The generic callback typedef that takes a void pointer and returns a void
 * pointer.  Registered functions of this type can be called on completion of
 * the IO operation.  The void*-void* form was selected over the IO*-IO* form
 * so that external modules can register callbacks as well.  For example
 * a sensor can register Sensor_incrementMiniState() using void*-void*.
 */
typedef void * (* io_cb_fnct)(void * _io);



/***********************************************/
/************ protected includes  **************/
/***** must be after externs and typedefs  *****/

#include "io-private.h"


/***********************************************/
/****** application interface functions  *******/

/*!
 *	MUST be called the class before other methods are called returns self on
 *	success, otherwise returns NULL.
 *
 *	@code
 *
 *	// create an initial array to seed a List for IO storage
 *	void * IO_actionBuffer[4];
 *	// Create a List where IO will store IO sequences waiting to be executed
 *	struct List * IOTest_ioActionList = new(List, IO_actionBuffer);
 *	IO_init(IOTest_ioActionList);
 *
 *	// Create a new IO object
 * 	struct IO * myIOobject = new(IO, ioActionList);
 *
 *	// Create an area in memory where reads and write are allowed
 *	// Set address to this area
 *	io_data_t allowedMemoryArea[16];
 *
 *	// create an Access object
 *	myAccessObject = new(AccessMEM, MAX_NUMBER_OF_COMMANDS);
 *	IO_setAddress(myAccessObject, allowedMemoryArea);
 *
 *	// Clear the command buffer
 *	IO_clearCommandSequences(myAccessObject);
 *
 *	// Add write commands to the buffer (example only)
 *	// Write counts are managed by the add operation
 *	IO_addWriteCommandToSequence(myAccessObject, 0x03);
 *	IO_addWriteCommandToSequence(myAccessObject, 0x41);
 *
 *	// OPTIONAL:  If read operations are executed, the read count is set here.
 *	IO_setReadCount(myAccessObject, 1);
 *
 *	// Set IO action to desired format
 *	IO_setIOAction(myAccessObject, IO_WRITE_SEQUENTIAL);
 *
 *	if ( Access_sequenceIsValid(myAccessObject) == NULL ) { return NULL; }
 *
 *	// Set communication complete callback ... fired when communication is done
 *	IO_set_actionDone_cb(localIoStructPtr, (io_cb_fnct) My_CommCompleteDone_CB);
 *	IO_setObjectPointer(localIoStructPtr, (void *)My_Pointer);
 *
 *	// Add the IO sequence to the main IO list IOTest_ioActionList
 *	IO_addIOSequenceToList(myIOobject, myAccessObject)
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
void IO_init(struct List * _ioSequenceList);


/*!
 * Adds the sequence of commands to the List of sequences managed by IO.
 * Returns command sequence on success.
 */
void * IO_addIOSequenceToList(void * _self, void * _ioSequence);

/*!
 * Executes the IO state machine, and is called from the scheduler.  Returns
 * processed struct IO object pointer on completion.
 */
void * IO_update(void * _self);


/*!
 * Possible new method to allow multiple sequences to hold control of the
 * communication bus.  Method returns NULL if no follow-on sequence is needed.
 * Returns the follow-on sequence if one exists.  This function would
 * be overloadable.
 * void * IO_getFollowOnSequence(void * _self);
 */

// WARNING: this method is not implemented
void * IO_xxxx(void * _self);


/******************************/
/****** access methods  *******/

/*!
 * Called when operation is complete if function pointer is not NULL.
 * Passes parameter of objectPointer, accessible through
 * IO_getObjectPointer(), even if this value is NULL.
 *
 * WARNING: this callback is fired when any IO sequence is completed
 * IO sequences can be from various modules and to various sink devices
 * there is NO 1:1 relationship between a given sequence and this cb
 * likely use is as part of a callback chain spanning several modules
 */
io_cb_fnct IO_getActionDone_cb(const void * _self);
io_cb_fnct IO_setActionDone_cb(      void * _self, io_cb_fnct _cb);

/*!
 * Object pointer parameter value for action done callback.  Points back to
 * the original object owning the IO structure.
*/
void * IO_getObjectPointer(const void * _self);
void * IO_setObjectPointer(      void * _self, void * _objectPointer);


#endif /* SRC_IO_IO_H_ */
