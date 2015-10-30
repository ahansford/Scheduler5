/*
 * lights-X10-private.h
 *
 *  Created on: Jul 8, 2015
 *      Author: Alan
 */

#ifndef SRC_NODES_X10_NODES_PRIVATE_H_
#define SRC_NODES_X10_NODES_PRIVATE_H_

/***********************************/
/*** class and object structures ***/

struct Node_X10 {
	const struct Node	_;	// should be of LightClass
	X10_house_code_t	house;
	X10_unit_code_t		unit;
};

struct NodeClass_X10 {
	const struct NodeClass 	_;	// should be of Class //
	X10_house_code_t	(* setHouseCode)(const void * _self,
										 const X10_house_code_t _houseCode);
	X10_house_code_t	(* getHouseCode)(const void * _self);
	X10_unit_code_t		(* setUnitCode)	(const void * _self,
										 const X10_unit_code_t _unitCode);
	X10_unit_code_t		(* getUnitCode)	(const void * _self);
};


/*****************************/
/****** Constructors  ********/

void * NodeClass_X10_ctor(void * _self, va_list * app);
void *      Node_X10_ctor(void * _self, va_list * app);

/***********************************/
/****** Overload Functions  ********/

// overloaded Object methods
void *     Node_X10_copy(       void * _self, const void * _copyFromObject);
equal_t    Node_X10_equal(const void * _self, const void * _comparisonObject);
puto_return_t Node_X10_puto(const void * _self, FILE * _fp);

// overloaded Node methods
void *        Node_X10_config(const void * _self, void * _master);


/**********************************/
/***********  new methods  ********/
/**********************************/

X10_house_code_t Node_X10_setHouseCode(      void * _self,
									   const X10_house_code_t _houseCode);
X10_house_code_t Node_X10_getHouseCode(const void * _self);

X10_unit_code_t  Node_X10_setUnitCode (      void * _self,
		                               const X10_unit_code_t _unitCode);
X10_unit_code_t  Node_X10_getUnitCode (const void * _self);

#endif /* SRC_NODES_X10_NODES_PRIVATE_H_ */
