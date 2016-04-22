/*
 * io-private.h
 *
 *  Created on: Sep 30, 2015
 *      Author: Alan
 */

#ifndef SRC_IO_IO_PRIVATE_H_
#define SRC_IO_IO_PRIVATE_H_

struct IO {
	const struct Object _;	// should the Object or the "superclass"
	// TODO: probably has no use, but still in the constuctor
	struct List *   	ioSequenceList;
	struct AccessMEM * 	currentSequence;
	int					ioState;
	io_cb_fnct			actionDone_cb;
	void * 				objectPointer;  // back ptr to the original item
};

struct IOClass {
	const struct Class	_;	// should be superclass: Class or "somethingClass"
	void *	(*IO_xxxx)	(void * _self);
};


typedef enum io_update_state_t {
	IO_UPDATE_UNKNOWN = -1,
	IO_UPDATE_IDLE,
	IO_UPDATE_EXECUTE_COMMAND,
	IO_UPDATE_WAITING_COMMAND,
	IO_UPDATE_SEQUENCE_COMPLETE,
	IO_UPDATE_RESET_TO_IDLE
} io_update_state_t;


/*****************************/
/****** Constructors  ********/

void * IOClass_ctor(void * _self, va_list * app);
void * IO_io_ctor  (void * _self, va_list * app);

/***********************************/
/****** Overload Functions  ********/

void *        IO_io_dtor  (      void * _self);
void *        IO_io_copy  (      void * _copyTo, const void * _copyFrom);
equal_t       IO_io_equal (const void * _self, const void * _comparisonObject);
void *        IO_io_config(const void * _self, const void * _master);
puto_return_t IO_io_puto  (const void * _self, FILE *       _fp);


/**********************************/
/**********  new methods  *********/

void * super_IO_io_addWriteValue(const void * _class,
								 void * _self, io_data_t _value);
void *       IO_io_addWriteValue(void * _self, io_data_t _value);

// not implemented
void * super_IO_xxxx(const void * _class, void * _self);
void *    IO_io_xxxx(void * _self);

//  gets struct IO item from internal list specified by IO_init(struct List *)
void * IO_getIOSequenceFromList(void * _self);



/*!
 * Possible new method to allow multiple sequences to hold control of the
 * communication bus.  Method returns NULL if no follow-on sequence is needed.
 * Returns the follow-on sequence if one exists.  This function would
 * be overloadable.
 * void * IO_getFollowOnSequence(void * _self);
 */

// WARNING: this method is not implemented
void * IO_xxxx(void * _self);


/*!
 * Generic IO callback that fires when I/O action is complete.  The IO state variable will
 * increment automatically to a completed setting.
 */
void * IO_sequenceComplete_cb(void * _self);

/*!
 * PRIVATE: Accessor method for internal List to hold IO actions
 */
void * IO_getIoSequenceList(const void * _self);
void * IO_setIoSequenceList(void * _self, void * _ioSequenceList);

void * IO_getCurrentSequence(const void * _self);
void * IO_setCurrentSequence(void * _self, void * _currentSequence);


int IO_getIoState(const void * _self);
int IO_setIoState(void * _self, int _ioState);


#endif /* SRC_IO_IO_PRIVATE_H_ */
