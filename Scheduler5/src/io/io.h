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
//#include "..\..\src\nodes\nodes.h"     // supports class registration
#include "..\..\src\lists\lists.h"

/***********************************************/
/**** forward declarations and typedefs  *******/

extern const void * IO;
extern const void * IOClass;

typedef unsigned char io_data_t;

typedef enum io_read_write_t {
	IO_ACTION_UNKNOWN = -1,
	IO_WRITE_SINGLE,
	IO_WRITE_SEQUENTIAL,
	IO_WRITE_READ_SINGLE,
	IO_WRITE_READ_SEQUENTIAL,
	IO_READ_SINGLE,
	IO_READ_SEQUENTIAL
} io_read_write_t;

//typedef struct IO * (* io_cb_fnct)(struct IO * _io);
typedef void * (* io_cb_fnct)(void * _io);



/***********************************************/
/************ protected includes  **************/
/***** must be after externs and typedefs  *****/

#include "io-private.h"


/***********************************************/
/****** application interface functions  *******/

// MUST be called the class before other methods are called
// returns self on success, otherwise NULL
void IO_init(struct List * _ioSequenceList);

// create a new sensor with new(), must specified
// ... struct IO myIO = new(IO, ioActionList);

// resets write and read counts indicating that the CMD buffer is clear
void * IO_clearCommandSequences(void * _self);

// write value to the sequence buffer
// values will be written to IO address when IO_update() is called
// returns self on success
void * IO_addWriteCommandToSequence(void * _self, io_data_t _value);

// use IO_setReadCount(_self, _readCount) to trigger a read sequence
// read executes after the preceding write commands
// the writeCount is set automatically when the write command is added

// WARNING:  there are no protections against counts larger than the buffer
//           ... code calling the io methods should carefully manages sizes

// adds the sequence of commands to the ioSequenceList
// returns command sequence on success
void * IO_addIOSequenceToList(void * _self);

// executes the io state machine, called from the scheduler
// returns processed struct IO object on completion
void IO_update(void);

// executes the specified ioAction
// called via IO_update()
void * IO_processSequence(void * _self);

// possible new method to allow multiple sequences to hold the comm bus
// method returns NULL if no follow-on sequence is needed
// returns the follow-on sequence if one exists
// this function would be overloadable
//void * IO_getFollowOnSequence(void * _self);

// hardware driver should call when I/O action is complete
// if value in struct IO is NULL, the sequence will be
// marked completed and the state will increment automatically
void IO_sequenceComplete_cb(void);



// WARNING: this method is not implemented
void * IO_xxxx(void * _self);


/******************************/
/****** access methods  *******/

// address for read or write operation
void * IO_getAddress(const void * _self);
void * IO_setAddress(      void * _self, void * _address);

// type of operation
io_read_write_t IO_getIOAction(const void * _self);
io_read_write_t IO_setIOAction(void * _self, io_read_write_t _ioAction);

// number of values to read
int IO_getReadCount(const void * _self);
int IO_setReadCount(      void * _self, int _readCount);

// number ofbytes to write
int IO_getWriteCount(const void * _self);
int IO_setWriteCount(      void * _self, int _writeCount);

// struct List pointer to buffer holding data for writes and reads
// write operation is first, and read data overwrites original write data
void * IO_getBufferPointer(const void * _self);
void * IO_setBufferPointer(      void * _self, void * _bufferPointer);

// function pointer "void (* functionPointer)(void *_objectPointer)
// called when operation is complete
io_cb_fnct IO_get_actionDone_cb(const void * _self);
io_cb_fnct IO_set_actionDone_cb(      void * _self, io_cb_fnct _cb);

// object pointer value for callback
void * IO_getObjectPointer(const void * _self);
void * IO_setObjectPointer(      void * _self, void * _objectPointer);


#endif /* SRC_IO_IO_H_ */
