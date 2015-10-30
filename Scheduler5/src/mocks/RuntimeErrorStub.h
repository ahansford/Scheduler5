/*
 * RuntimeErrorStubs.h
 *
 *  Created on: Jul 11, 2013
 *      Author: alhansfo
 */

#ifndef RUNTIMEERRORSTUBS_H_
#define RUNTIMEERRORSTUBS_H_
//#include "stdint.h"



void RuntimeErrorStub_Reset(void);
const char * RuntimeErrorStub_GetLastError(void);
int RuntimeErrorStub_GetLastParameter(void);
void RuntimeError(const char * message, int parmeter, const char * file, int line);
void RuntimeErrorStub_CheckLastError(void);
void RuntimeErrorStub_ConfirmLastError(const char * message);

#define RUNTIME_ERROR(description, parameter)  RuntimeError(description, parameter, __FILE__. __LINE__)

#endif /* RUNTIMEERRORSTUBS_H_ */
