/*
 * io-private.h
 *
 *  Created on: Sep 30, 2015
 *      Author: Alan
 */

#ifndef SRC_ACCESS_MEM_ACCESS_MEM_PRIVATE_H_
#define SRC_ACCESS_MEM_ACCESS_MEM_PRIVATE_H_

struct AccessMEM {
	const struct Object _;	// should the Object or the "superclass"
	void * 			address;
	access_read_write_t ioAction;
	int 			readCount;
	int 			writeCount;
	access_data_t * bufferPointer;
	int				bufferSize;
	access_cb_fnct  actionDone_cb;
	void * 			objectPointer;
	void *			hardwareConfig;
};

struct AccessMEMClass {
	const struct Class	_;	// should be superclass: Class or "somethingClass"
	void *	(*Access_addWriteCommandToSequence)	    (void * _self, char _value);
	void *	(*Access_processSequence)	(void * _self);
	void *	(*Access_xxxx)	    		(void * _self);

};

typedef enum access_update_state_t {
	ACCESS_UPDATE_UNKNOWN,
	ACCESS_UPDATE_IDLE,
	ACCESS_UPDATE_EXECUTE_COMMAND,
	ACCESS_UPDATE_WAITING_COMMAND,
	ACCESS_UPDATE_SEQUENCE_COMPLETE,
	ACCESS_UPDATE_RESET_TO_IDLE
} access_update_state_t;


/*****************************/
/****** Constructors  ********/

void * Access_MEMClass_ctor(void * _self, va_list * app);
void * Access_MEM_ctor  (void * _self, va_list * app);

/***********************************/
/****** Overload Functions  ********/

void *        Access_MEM_dtor  (      void * _self);
void *        Access_MEM_copy  (      void * _copyTo, const void * _copyFrom);
equal_t       Access_MEM_equal (const void * _self, const void * _comparisonObject);
void *        Access_MEM_config(const void * _self, const void * _master);
puto_return_t Access_MEM_puto  (const void * _self, FILE *       _fp);


/**********************************/
/**********  new methods  *********/

void * super_Access_addWriteCommandToSequence(const void * _class,
								                  void * _self, access_data_t _value);
void *       Access_MEM_addWriteCommandToSequence(void * _self, access_data_t _value);

void * super_Access_processSequence(const void * _class, void * _self);
void *    Access_MEM_processSequence(void * _self);

// not implemented
void * super_IO_xxxx(const void * _class, void * _self);
void *    IO_io_xxxx(void * _self);

//  gets struct IO item from internal list specified by IO_init(struct List *)
void * IO_getActionFromList(void);


#endif /* SRC_ACCESS_MEM_ACCESS_MEM_PRIVATE_H_ */
