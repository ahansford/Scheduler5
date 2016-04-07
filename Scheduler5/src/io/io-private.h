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
	io_data_t *   	bufferPointer;
	//int				bufferSize;
	io_cb_fnct		actionDone_cb;
	void * 			objectPointer;  // back ptr to the original item
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
void * IO_getIOSequenceFromList(void);

/*!
 * Generic IO callback that fires when I/O action is complete.  The IO state variable will
 * increment automatically to a completed setting.
 */
void * IO_sequenceComplete_cb(void * _self);


#endif /* SRC_IO_IO_PRIVATE_H_ */
