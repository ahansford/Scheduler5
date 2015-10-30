/*
 * objects-private.h
 *
 *  Created on: Feb 24, 2015
 *      Author: Alan
 */

#ifndef OBJECTS_PRIVATE_H_
#define OBJECTS_PRIVATE_H_

/***********************************/
/*** class and object structures ***/

struct Object {
	const struct Class * class; // points to object class descriptor
};

struct Class {
	const struct Object		_; 			     // struct * to class descriptor
	const char * 			className;		 // text name of this class
	const struct Class * 	superclass;		 // struct * to superclass
	int 					sizeOfThisClass; // size of this structure
	void * 					(* ctor)   (      void * _self, va_list * app);
	void * 					(* dtor)   (      void * _self);
	void * 					(* copy)   (      void * _self,
			                            const void * _copyFromObject);
	equal_t    				(* equal)  (const void * _self,
			                            const void * _comparisonObject);
	void *  	            (* config) (const void * _self,
								        const void * _master);
	puto_return_t 		    (* puto)   (const void * _self, FILE * fp);
};


/*****************************/
/****** Constructors  ********/

void * super_ctor (const void * _class, void * _self, va_list * app);
void * Object_ctor(                     void * _self, va_list * app);
void * Class_ctor (                     void * _self, va_list * app);

void * super_dtor (const void * _class, void * _self);
void * Object_dtor(                     void * _self);
void * Class_dtor (                     void * _self);

/**********************************/
/***********  new methods  ********/
/**********************************/

void *  super_copy(const void * _class,
		           void * _self, const void * _copyFromObject);
void * Object_copy(void * _self, const void * _copyFromObject);
void *  Class_copy(void * _self, const void * _copyFromObject);

equal_t  super_equal(const void * _class,
		             const void * _self, const void * _comparisonObject);
equal_t Object_equal(const void * _self, const void * _comparisonObject);
equal_t	 Class_equal(const void * _self, const void * _comparisonObject);

void *  super_config(const void * _class,
			 	     const void * _self, const void * _master);
void * Object_config(const void * _self, const void * _master);
void *  Class_config(const void * _self, const void * _master);

puto_return_t  super_puto(const void * _class,
		                  const void * _self,  FILE * _fp);
puto_return_t Object_puto(const void * _self,  FILE * _fp);
puto_return_t  Class_puto(const void * _class, FILE * _fp);

// other support functions
const void * super(const void * _self);
int isOf_sc       (const void * _self, const struct Class * _class);
int isA_sc        (const void * _self, const struct Class * _class);
int isOfSuper  (const void * _self, const struct Class * _superClass);
void * cast       (const struct Class * _class, const void * _self);

//  Functions added for ease of test
int Any_equal(const void * _self, const void * _comparisonObject);
void Object_test_printf(const void * _self);
#define className(p)   (((const struct Class *) (p)) -> className)
//#define UnsafeClass(p)   (((const struct Object *) (p))->_.class)

#endif /* OBJECTS_PRIVATE_H_ */
