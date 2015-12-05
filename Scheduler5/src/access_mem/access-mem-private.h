/*
 * io-private.h
 *
 *  Created on: Sep 30, 2015
 *      Author: Alan
 */

#ifndef SRC_ACCESS_MEM_ACCESS_MEM_PRIVATE_H_
#define SRC_ACCESS_MEM_ACCESS_MEM_PRIVATE_H_

struct Access_MEM {
	const struct Object _;	// should the Object or the "superclass"
	void * 			address;
	io_read_write_t ioAction;
	int 			readCount;
	int 			writeCount;
	io_data_t *   	bufferPointer;
	int				bufferSize;
	io_cb_fnct		actionDone_cb;
	void * 			objectPointer;
	void *			hardwareConfig;
};

struct Access_MEMClass {
	const struct Class	_;	// should be superclass: Class or "somethingClass"
	void *	(*Access_addWriteCommandToSequence)	    (void * _self, char _value);
	void *	(*Access_processSequence)	(void * _self);
	void *	(*Access_xxxx)	    		(void * _self);

};

typedef enum io_update_state_t {
	IO_UPDATE_UNKNOWN,
	IO_UPDATE_IDLE,
	IO_UPDATE_EXECUTE_COMMAND,
	IO_UPDATE_WAITING_COMMAND,
	IO_UPDATE_SEQUENCE_COMPLETE,
	IO_UPDATE_RESET_TO_IDLE
} io_update_state_t;


/*****************************/
/****** Constructors  ********/

void * Access_MEMClass_ctor(void * _self, va_list * app);
void * Access_MEM_ctor  (void * _self, va_list * app);

/***********************************/
/****** Overload Functions  ********/

void *        IO_io_dtor  (      void * _self);
void *        IO_io_copy  (      void * _copyTo, const void * _copyFrom);
equal_t       IO_io_equal (const void * _self, const void * _comparisonObject);
void *        IO_io_config(const void * _self, const void * _master);
puto_return_t IO_io_puto  (const void * _self, FILE *       _fp);


/**********************************/
/**********  new methods  *********/

void * super_Access_addWriteCommandToSequence(const void * _class,
								                  void * _self, io_data_t _value);
void *       Access_MEM_addWriteCommandToSequence(void * _self, io_data_t _value);

void * super_Access_processSequence(const void * _class, void * _self);
void *    Access_MEM_processSequence(void * _self);

// not implemented
void * super_IO_xxxx(const void * _class, void * _self);
void *    IO_io_xxxx(void * _self);

//  gets struct IO item from internal list specified by IO_init(struct List *)
void * IO_getActionFromList(void);


#endif /* SRC_ACCESS_MEM_ACCESS_MEM_PRIVATE_H_ */
