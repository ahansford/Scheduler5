/*
 * Cross_Platform_Compiler_Defs.h
 *
 *  Created on:
 *      Author: alan
 */

#ifndef CROSS_PLATFORM_COMPILER_DEFS_H_
#define CROSS_PLATFORM_COMPILER_DEFS_H_

//////////////////////////////////////////////////////////////////////////////
// Compiler Directive Switches: Should be set for compiler environment


//#define __ARM
//#define __8051


//////////////////////////////////////////////////////////////////////////////
//Generic complier definitions - PRE

typedef enum { FALSE = 0, TRUE =!FALSE} bool;

//////////////////////////////////////////////////////////////////////////////
// Windows Platform complier definitions

#if !defined ( __C51__ )
//#warning __C51__ !defined
#define __W64

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define pdata

#if !defined (NULL)
#define NULL (0)
	#endif

#endif

//////////////////////////////////////////////////////////////////////////////
// C51 Embedded Platform complier definitions

#if defined( __C51__ )

#warning __C51__ defined

//{
	#if !defined (uint32_t)
	typedef unsigned long int uint32_t;
	#endif

	#if !defined (int32_t)
	typedef long int int32_t;
	#endif

	#if !defined (uint8_t)
	typedef unsigned char uint8_t;
	#endif

	#if !defined (uint16_t)
	typedef unsigned int uint16_t;
	#endif

	#if !defined ( int8_t )
	typedef char int8_t;
	#endif

	#if !defined ( int16_t )
	typedef int int16_t;
	#endif

	#if !defined ( NULL )
	#define NULL (0)
	#endif

	#include <stdio.h>

//}


#endif

//////////////////////////////////////////////////////////////////////////////
// 8051 complier definitions


#include "compiler_defs.h"


//////////////////////////////////////////////////////////////////////////////
// Generic complier definitions - POST



//////////////////////////////////////////////////////////////////////////////
// END
#endif /* CROSS_PLATFORM_COMPILER_DEFS_H_ */
