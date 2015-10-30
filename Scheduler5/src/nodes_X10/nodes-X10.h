/*
 * nodes-X10.h
 *
 *  Created on: Jul 8, 2015
 *      Author: Alan
 */

#ifndef SRC_NODES_X10_NODES_X10_H_
#define SRC_NODES_X10_NODES_X10_H_

/***********************************************/
/****** superclass and system includes  ********/

//#include "..\cross_compiler_defs.h"
#include "..\..\src\nodes\nodes.h"


/***********************************************/
/**** Forward declarations and typedefs  *******/

extern const void * Node_X10; // struct Node_X10 * new node = new(Node_X10);
extern const void * NodeClass_X10;

typedef enum X10_house_code_t {
	X10_HOUSE_CODE_UNKNOWN = -1, X10_A = 1, X10_B, X10_C, X10_D, X10_E, X10_F,
	X10_G, X10_H, X10_I, X10_J, X10_K, X10_L,
	X10_M, X10_N, X10_O, X10_P
} X10_house_code_t;

typedef enum X10_unit_code_t {
	X10_UNIT_CODE_UNKNOWN = -1
} X10_unit_code_t;


/***********************************************/
/************ protected includes  **************/

#include "nodes-X10-private.h"


/***********************************************/
/****** application interface functions  *******/

// MUST be called before any new(Node_X10) is created //
void Node_X10_init(void);

// Returns the requested house code if the call is successful
X10_house_code_t setHouseCode(const void * _self, const X10_house_code_t _houseCode);
X10_house_code_t getHouseCode(const void * _self);

// Returns the requested unit code if the call is successful
X10_unit_code_t  setUnitCode (const void * _self, const X10_unit_code_t _unitCode);
X10_unit_code_t  getUnitCode (const void * _self);

#endif /* SRC_NODES_X10_NODES_X10_H_ */
