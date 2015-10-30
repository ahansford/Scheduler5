/*
 * event-controller-private.h
 *
 *  Created on: Aug 15, 2015
 *      Author: alhansfo
 */

#ifndef SRC_BUTTON_BUTTON_PRIVATE_H_
#define SRC_BUTTON_BUTTON_PRIVATE_H_

//#include "..\..\src\objects\objects.h" // safety include

struct Button {
	const struct Object _;	// should the superclass
	buttonState_t buttonState;
	int preliminaryCount;
	int pressCount;
	buttonPressType_t preliminaryType;
	buttonPressType_t pressType;
	int pressTime;
	int releaseTime;
	struct Button * (*button_OnReportReady_cb) (struct Button * _button);
};

struct ButtonClass {
	const struct Class	_;	// should be superclass: Class or "somethingClass"
	//second_t    	(*setSecond)(void * _self, second_t _second);
	//second_t    	(*getSecond)(void * _self);
};

/*****************************/
/****** Constructors  ********/

void * ButtonClass_ctor(void * _self, va_list * app);
void * Button_ctor     (void * _self, va_list * app);

/***********************************/
/****** Overload Functions  ********/

void *      Button_dtor  (void * _self);
void *      Button_copy  (void * _copyToButton, const void * _copyFromButton);
equal_t     Button_equal (const void * _self, const void * _comparisonObject);
void *      Button_config(const void * _self, const void * _master);
puto_return_t Button_puto(const void * _self, FILE * _fp);


/**********************************/
/**********  new methods  *********/

buttonState_t Button_getButtonState(const void * _self);
buttonState_t Button_setButtonState(void * _self, buttonState_t _buttonState);

int Button_getPreliminaryCount(const void * _self);
int Button_setPreliminaryCount(void * _self, int _pressCount);

int Button_setPressCount(      void * _self, int _pressCount);

buttonPressType_t Button_getPreliminaryType(const void * _self);
buttonPressType_t Button_setPreliminaryType(           void * _self,
		                                    buttonPressType_t _pressType);

buttonPressType_t Button_setPressType(           void * _self,
		                              buttonPressType_t _pressType);

int Button_getPressTime(const void * _self);
int Button_setPressTime(      void * _self, int _pressTime);

int Button_getReleaseTime(const void * _self);
int Button_setReleaseTime(      void * _self, int _releaseTime);

void * Button_reset      (void * _self);
void   Button_updateState(void * _self, int _timeStamp);

// no-op button event call back, fires when button state is fully determined
// returns self pointer on success, otherwise returns NULL
struct Button * Button_PostOnReportReadyCallback(struct Button * _self);

#endif /* SRC_BUTTON_BUTTON_PRIVATE_H */
