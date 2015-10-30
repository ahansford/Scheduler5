/*
 * nodes-test.c
 *
 *  Created on: July, 2015
 *      Author: Alan
 */


#include "..\..\src\cross_compiler_defs.h"
#include "..\unity\unity_fixture.h"
//#include "..\unity\unity.h"
#include "..\..\src\mocks\RuntimeErrorStub.h"

#include "..\..\src\button\button.h"
//#include "..\..\src\times\times-private.h"

/****************************************/
/*** Basic Object and Class Testing ****/
struct       Button *      myTest_Button;
const struct ButtonClass * myTest_ButtonClass_PTR;
const struct Class *       myTest_Button_class_PTR;

struct Button * button_test_general_cb(struct Button * _self);
int button_test_cb_count;

/****************************************/
/*********** Start of Testing ***********/
TEST_GROUP(button);

TEST_SETUP(button)
{
	RuntimeErrorStub_Reset();
	Button_init(100);
	myTest_Button  = new(Button);
	myTest_ButtonClass_PTR  = classOf(myTest_Button);
	myTest_Button_class_PTR = Button;
}

TEST_TEAR_DOWN(button)
{
	myTest_Button = safeDelete(myTest_Button);
	RuntimeErrorStub_Reset();
}

/**************************************/
/**************  New  *****************/

TEST(button, myTest_Button_IsNotNullOnCreate)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  myTest_Button);
}

TEST(button, Button_buttonState_isUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(BUTTON_STATE_UNKNOWN,  myTest_Button->buttonState);
}

TEST(button, Button_pressCount_isUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(0,  myTest_Button->pressCount);
}

TEST(button, Button_pressType_isUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(BUTTON_PRESS_TYPE_UNKNOWN,  myTest_Button->pressType);
}

TEST(button, Button_pressTime_isUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(BUTTON_TIME_UNKNOWN,  myTest_Button->pressTime);
}

TEST(button, Button_releaseTime_isUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(BUTTON_TIME_UNKNOWN,  myTest_Button->releaseTime);
}



/**************************************/
/*************  Button  ***************/

TEST(button, init_Button_ClassReturnsNonNull)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  myTest_Button->_.class);
}

TEST(button, init_classOf_myTest_Button_is_Button)
{
	TEST_ASSERT_EQUAL_PTR(Button,  myTest_Button->_.class);
}

TEST(button, init_classOf_Button_is_ButtonClass)
{
	TEST_ASSERT_EQUAL_PTR(ButtonClass,  myTest_Button->_.class->_.class);
}

TEST(button, init_ctor_pointsTo_ButtonClass_ctor)
{
	TEST_ASSERT_EQUAL_PTR(Button_ctor,  myTest_Button->_.class->ctor);
	TEST_ASSERT_EQUAL_PTR(ButtonClass_ctor,  myTest_Button->_.class->_.class->ctor);
}

TEST(button, init_ctor_pointsTo_Button_ctor)
{
	TEST_ASSERT_EQUAL_PTR(Button_ctor,  myTest_Button->_.class->ctor);
}

TEST(button, init_dtor_pointsTo_Button_dtor)
{
	TEST_ASSERT_EQUAL_PTR(Button_dtor,  ((struct Class *)myTest_ButtonClass_PTR)->dtor );
	TEST_ASSERT_EQUAL_PTR(Button_dtor,  myTest_Button->_.class->dtor);
}

TEST(button, init_copy_pointsTo_Button_copy)
{
	TEST_ASSERT_EQUAL_PTR(Button_copy,  myTest_Button->_.class->copy);
}

TEST(button, init_equal_pointsTo_Button_equal)
{
	TEST_ASSERT_EQUAL_PTR(Button_equal,  myTest_Button->_.class->equal);
}

TEST(button, init_config_pointsTo_Time_config)
{
	TEST_ASSERT_EQUAL_PTR(Button_config,  myTest_Button->_.class->config);
}

TEST(button, init_ButtonClass_NameIs_ButtonClass)
{
	TEST_ASSERT_EQUAL_STRING("ButtonClass",  myTest_Button->_.class->_.class->className);
}

/**************************************/
/*******  TimeClass Methods  *********/
/*
TEST(button, init_setSecond_pointsTo_Time_setSecond)
{
	TEST_ASSERT_EQUAL_PTR(Time_setSecond,  myTest_ButtonClass_PTR->setSecond );
}

TEST(button, init_getSecond_pointsTo_Time_getSecond)
{
	TEST_ASSERT_EQUAL_PTR(Time_getSecond,  myTest_ButtonClass_PTR->getSecond);
}
*/


//****  delete/Time_dtor  ****************

TEST(button, delete_returns_SelfOnSuccess)
{
	TEST_ASSERT_EQUAL_PTR(myTest_Button,  delete(myTest_Button));
	myTest_Button = NULL;
}

TEST(button, delete_safeDelete_returnsNullOnSuccess)
{
	TEST_ASSERT_EQUAL_PTR(NULL,  safeDelete(myTest_Button));
	myTest_Button = NULL;
}

//****  set and get buttonState  ****************
/**/

TEST(button, getButtonState_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(BUTTON_STATE_UNKNOWN,  Button_getButtonState(myTest_Button));
}

TEST(button, getButtonState_returns_specificValue)
{
	myTest_Button->buttonState = BUTTON_REPORT;
	TEST_ASSERT_EQUAL(BUTTON_REPORT,  Button_getButtonState(myTest_Button));
}

TEST(button, setButtonState_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(myTest_Button->buttonState,  Button_setButtonState(myTest_Button, 5));
}

TEST(button, setButtonState_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(BUTTON_STATE_UNKNOWN,  Button_setButtonState(NULL, BUTTON_REPORT));
}

TEST(button, setButtonState_returnsSpecificValueOnSuccess)
{
	TEST_ASSERT_EQUAL(BUTTON_REPORT,  Button_setButtonState(myTest_Button, BUTTON_REPORT));
}

TEST(button, setButtonState_canSetSpecificValue)
{
	Button_setButtonState(myTest_Button, BUTTON_WAITING);
	TEST_ASSERT_EQUAL(BUTTON_WAITING,  myTest_Button->buttonState);
}

//****  set and get pressCount  ****************
/**/

TEST(button, getPressCount_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(0,  Button_getPressCount(myTest_Button));
}

TEST(button, getPressCount_returns_specificValue)
{
	myTest_Button->pressCount = 0;
	TEST_ASSERT_EQUAL(0,  Button_getPressCount(myTest_Button));
}

TEST(button, setPressCount_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(myTest_Button->pressCount,  Button_setPressCount(myTest_Button, 5));
}

TEST(button, setPressCount_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(0,  Button_setPressCount(NULL, 7));
}

TEST(button, setPressCount_returnsSpecificValueOnSuccess)
{
	TEST_ASSERT_EQUAL(6,  Button_setPressCount(myTest_Button, 6));
}

TEST(button, setPressCount_canSetSpecificValue)
{
	Button_setPressCount(myTest_Button, 9);
	TEST_ASSERT_EQUAL(9,  myTest_Button->pressCount);
}

//****  set and get pressType  ****************
/**/

TEST(button, getPressType_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(BUTTON_PRESS_TYPE_UNKNOWN,  Button_getPressType(myTest_Button));
}

TEST(button, getPressType_returns_specificValue)
{
	myTest_Button->pressType = BUTTON_SHORT_PRESS;
	TEST_ASSERT_EQUAL(BUTTON_SHORT_PRESS,  Button_getPressType(myTest_Button));
}

TEST(button, setPressType_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(myTest_Button->pressType,  Button_setPressType(myTest_Button, 5));
}

TEST(button, setPressType_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(BUTTON_PRESS_TYPE_UNKNOWN,  Button_setPressType(NULL, BUTTON_SHORT_PRESS));
}

TEST(button, setPressType_returnsSpecificValueOnSuccess)
{
	TEST_ASSERT_EQUAL(BUTTON_SHORT_PRESS,  Button_setPressType(myTest_Button, BUTTON_SHORT_PRESS));
}

TEST(button, setPressType_canSetSpecificValue)
{
	Button_setPressType(myTest_Button, BUTTON_FIVE_SECONDS);
	TEST_ASSERT_EQUAL(BUTTON_FIVE_SECONDS,  myTest_Button->pressType);
}

//****  set and get pressTime  ****************
/**/

TEST(button, getPressTime_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(BUTTON_TIME_UNKNOWN,  Button_getPressTime(myTest_Button));
}

TEST(button, getPressTime_returns_specificValue)
{
	myTest_Button->pressTime = 8;
	TEST_ASSERT_EQUAL(8,  Button_getPressTime(myTest_Button));
}

TEST(button, setPressTime_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(myTest_Button->pressTime, Button_setPressTime(myTest_Button, 5));
}

TEST(button, setPressTime_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(BUTTON_TIME_UNKNOWN, Button_setPressTime(NULL, 7));
}

TEST(button, setPressTime_returnsSpecificValueOnSuccess)
{
	TEST_ASSERT_EQUAL(6, Button_setPressTime(myTest_Button, 6));
}

TEST(button, setPressTime_canSetSpecificValue)
{
	Button_setPressTime(myTest_Button, 9);
	TEST_ASSERT_EQUAL(9, myTest_Button->pressTime);
}

//****  set and get releaseTime  ****************
/**/

TEST(button, getReleaseTime_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(BUTTON_TIME_UNKNOWN,  Button_getReleaseTime(myTest_Button));
}

TEST(button, getReleaseTime_returns_specificValue)
{
	myTest_Button->releaseTime = 8;
	TEST_ASSERT_EQUAL(8,  Button_getReleaseTime(myTest_Button));
}

TEST(button, setReleaseTime_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(myTest_Button->releaseTime, Button_setReleaseTime(myTest_Button, 5));
}

TEST(button, setReleaseTime_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(BUTTON_TIME_UNKNOWN, Button_setReleaseTime(NULL, 7));
}

TEST(button, setReleaseTime_returnsSpecificValueOnSuccess)
{
	TEST_ASSERT_EQUAL(6, Button_setReleaseTime(myTest_Button, 6));
}

TEST(button, setReleaseTime_canSetSpecificValue)
{
	Button_setReleaseTime(myTest_Button, 9);
	TEST_ASSERT_EQUAL(9, myTest_Button->releaseTime);
}

//****  Button_reportReady  ****************

TEST(button, reportReady_returns_NotReadyOnCreate)
{
	TEST_ASSERT_EQUAL(BUTTON_REPORT_NOT_READY, Button_reportReady(myTest_Button));
}

TEST(button, reportReady_returns_ReadyOnValidReport)
{
	Button_setButtonState(myTest_Button, BUTTON_REPORT);
	TEST_ASSERT_EQUAL(BUTTON_REPORT_IS_READY, Button_reportReady(myTest_Button));
}

TEST(button, reportReady_returnsNotReadyOnNullPtr)
{
	TEST_ASSERT_EQUAL(BUTTON_REPORT_NOT_READY, Button_reportReady(NULL));
}

//****  copy/Time_copy  ****************

TEST(button, copy_returnsSelfOnSuccess)
{
	struct Button * masterButton = new(Button);
	Button_setPressTime(masterButton, 1);
	TEST_ASSERT_EQUAL_PTR(myTest_Button, copy(myTest_Button, masterButton));
	//free(masterButton);
	masterButton = safeDelete(masterButton);
}

TEST(button, copy_AllItemsCopiedToSelf)
{
	struct Button * masterButton = new(Button);
	Button_setButtonState(masterButton, BUTTON_STATE_UNKNOWN);
	Button_setPressCount(masterButton, 1);
	Button_setPressType (masterButton, BUTTON_PRESS_TYPE_UNKNOWN);
	Button_setPressTime  (masterButton, 2);
	Button_setReleaseTime(masterButton, 3);
	copy(myTest_Button, masterButton);

	TEST_ASSERT_EQUAL(BUTTON_STATE_UNKNOWN,      myTest_Button->buttonState);
	TEST_ASSERT_EQUAL(1,                         myTest_Button->pressCount);
	TEST_ASSERT_EQUAL(BUTTON_PRESS_TYPE_UNKNOWN, myTest_Button->pressType);
	TEST_ASSERT_EQUAL(2,                         myTest_Button->pressTime);
	TEST_ASSERT_EQUAL(3,                         myTest_Button->releaseTime);

	masterButton = safeDelete(masterButton);
}

TEST(button, copy_returnsNullOnNullSelf)
{
	struct Button * masterButton = new(Button);
	Button_setPressCount(masterButton, 1);
	TEST_ASSERT_EQUAL_PTR(NULL, copy(NULL, masterButton)  );
	masterButton = safeDelete(masterButton);
}

TEST(button, copy_returnsNullOnNullMaster)
{
	struct Button * masterButton = new(Button);
	Button_setPressCount(masterButton, 1);
	TEST_ASSERT_EQUAL_PTR(NULL, copy(myTest_Button, NULL) );
	masterButton = safeDelete(masterButton);
}

//****  equal/Button_equal  ********************

TEST(button, myTest_Button_IsEqualTo_myTest_Button)
{
	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_Button, myTest_Button));
}

TEST(button, equal_UnequalButtonStateReturn_Unequal)
{
	struct Button * masterButton = new(Button);
	Button_setButtonState(masterButton, BUTTON_IDLE);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Button, masterButton) );
	masterButton = safeDelete(masterButton);
}

TEST(button, equal_UnequalPressCountReturn_Unequal)
{
	struct Button * masterButton = new(Button);
	Button_setPressCount(masterButton, 1);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Button, masterButton) );
	masterButton = safeDelete(masterButton);
}

TEST(button, equal_UnequalPressTypeReturn_Unequal)
{
	struct Button * masterButton = new(Button);
	Button_setPressType (masterButton, BUTTON_FIVE_SECONDS);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Button, masterButton) );
	masterButton = safeDelete(masterButton);
}

TEST(button, equal_UnequalPressTiimeReturn_Unequal)
{
	struct Button * masterButton = new(Button);
	Button_setPressTime  (masterButton, 2);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Button, masterButton) );
	masterButton = safeDelete(masterButton);
}

TEST(button, equal_UnequalReleaseTimeReturn_Unequal)
{
	struct Button * masterButton = new(Button);
	Button_setReleaseTime(masterButton, 3);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Button, masterButton) );
	masterButton = safeDelete(masterButton);
}

TEST(button, equal_NullReturns_Null)
{
	struct Button * masterButton = new(Button);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Button, NULL));
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(NULL, masterButton ));
	masterButton = safeDelete(masterButton);
}

TEST(button, equal_CopiedButtonReturnsEqual)
{
	struct Button * masterButton = new(Button);
	Button_setButtonState(masterButton, BUTTON_STATE_UNKNOWN);
	Button_setPressCount(masterButton, 1);
	Button_setPressType (masterButton, BUTTON_PRESS_TYPE_UNKNOWN);
	Button_setPressTime  (masterButton, 2);
	Button_setReleaseTime(masterButton, 3);
	copy(myTest_Button, masterButton);
	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_Button, masterButton));
	masterButton = safeDelete(masterButton);
}


/***********  callback  **************/

TEST(button, Button_setButtonReport_cb_returns_Button_PostOnReportReadyCallback)
{
	TEST_ASSERT_EQUAL(Button_PostOnReportReadyCallback, Button_getButtonReport_cb(myTest_Button));
}

TEST(button, Button_setButtonReport_cb_returns_new_cb)
{
	TEST_ASSERT_EQUAL_PTR(button_test_general_cb,
			    Button_setButtonReport_cb(myTest_Button, button_test_general_cb));
}

TEST(button, Button_setButtonReport_cb_sets_new_cb)
{
	Button_setButtonReport_cb(myTest_Button, button_test_general_cb);
	TEST_ASSERT_EQUAL(button_test_general_cb, myTest_Button->button_OnReportReady_cb);
}

TEST(button, Button_reportCallback_firesDesignatedCallback)
{
	button_test_cb_count = 0;
	Button_setButtonReport_cb(myTest_Button, button_test_general_cb);
	myTest_Button->button_OnReportReady_cb(myTest_Button);
	TEST_ASSERT_EQUAL(1, button_test_cb_count);
}




/***********  config  **************/
/**/
TEST(button, Config_copiesState)
{
	struct Button * masterButton = new(Button);
	Button_setPressType(masterButton, BUTTON_SHORT_PRESS);
	Button_setPressTime(masterButton, 7);
	config(myTest_Button, masterButton);
	TEST_ASSERT_EQUAL(BUTTON_SHORT_PRESS, Button_getPressType(myTest_Button));
	TEST_ASSERT_EQUAL(7,                  Button_getPressTime(myTest_Button));
	masterButton = safeDelete(masterButton);
}

/***********  init  **************/
/**/
TEST(button, CanCall_Button_Init_MulitpleTimes)
{
	const void * originalButton = Button;
	const void * originalButtonClass = ButtonClass;
	Button_init(1000);
	Button_init(1000);
	Button_init(1000);

	// Verify that the class pointers are unchanged
	TEST_ASSERT_EQUAL_PTR(Button, originalButton);
	TEST_ASSERT_EQUAL_PTR(ButtonClass, originalButtonClass);

	char outputFilename[] = "button_puto.txt";
	FILE * fp = fopen(outputFilename, "a");
	puto(myTest_Button, fp);
	fclose(fp);
}

/***********  API  **************/

TEST(button, Button_reset_setsDataMembersToDefaults)
{
	Button_reset(myTest_Button);
	TEST_ASSERT_EQUAL(BUTTON_STATE_UNKNOWN,      Button_getButtonState(myTest_Button));
	TEST_ASSERT_EQUAL(0,                         Button_getPressCount(myTest_Button));
	TEST_ASSERT_EQUAL(BUTTON_PRESS_TYPE_UNKNOWN, Button_getPressType(myTest_Button));
	TEST_ASSERT_EQUAL(BUTTON_TIME_UNKNOWN,       Button_getPressTime(myTest_Button));
	TEST_ASSERT_EQUAL(BUTTON_TIME_UNKNOWN,       Button_getReleaseTime(myTest_Button));
}

TEST(button, Button_reset_returnsSelfOnSuccess)
{
	TEST_ASSERT_EQUAL_PTR(myTest_Button, Button_reset(myTest_Button));
}

TEST(button, Button_reset_returnsNullOnNullPtr)
{
	TEST_ASSERT_EQUAL_PTR(NULL, Button_reset(NULL));
}

//++++++++++++++++++//

TEST(button, Button_enable_resetsDataMembers)
{
	Button_start(myTest_Button);
	// Confirmation of transition to BUTTON_IDLE is handled in next test
	//TEST_ASSERT_EQUAL(BUTTON_IDLE,               Button_getButtonState(myTest_Button));
	TEST_ASSERT_EQUAL(0,                         Button_getPressCount(myTest_Button));
	TEST_ASSERT_EQUAL(BUTTON_PRESS_TYPE_UNKNOWN, Button_getPressType(myTest_Button));
	TEST_ASSERT_EQUAL(BUTTON_TIME_UNKNOWN,       Button_getPressTime(myTest_Button));
	TEST_ASSERT_EQUAL(BUTTON_TIME_UNKNOWN,       Button_getReleaseTime(myTest_Button));
}

TEST(button, Button_enable_transistionsToIdleState)
{
	Button_start(myTest_Button);
	TEST_ASSERT_EQUAL(BUTTON_IDLE, Button_getButtonState(myTest_Button));
}

TEST(button, Button_enable_returnsSelfOnSuccess)
{
	TEST_ASSERT_EQUAL(myTest_Button, Button_start(myTest_Button));
}

TEST(button, Button_enable_returnsNullOnNullPtr)
{
	TEST_ASSERT_EQUAL(NULL, Button_start(NULL));
}

//++++++++++++++++++//

TEST(button, Button_updateState_UnknownStaysInUnknown)
{
	Button_setButtonState(myTest_Button, BUTTON_STATE_UNKNOWN);
	Button_updateState(myTest_Button, 5);
	Button_updateState(myTest_Button, 6);
	Button_updateState(myTest_Button, 7);
	TEST_ASSERT_EQUAL(BUTTON_STATE_UNKNOWN, Button_getButtonState(myTest_Button) );
}

TEST(button, Button_updateState_ResetTransitionsToIdle)
{
	Button_setButtonState(myTest_Button, BUTTON_RESET);
	Button_updateState(myTest_Button, 5);
	TEST_ASSERT_EQUAL(BUTTON_IDLE, Button_getButtonState(myTest_Button) );
}

TEST(button, Button_updateState_IdleStaysInIdle)
{
	Button_setButtonState(myTest_Button, BUTTON_IDLE);
	Button_updateState(myTest_Button, 5);
	Button_updateState(myTest_Button, 6);
	Button_updateState(myTest_Button, 7);
	TEST_ASSERT_EQUAL(BUTTON_IDLE, Button_getButtonState(myTest_Button) );
}

TEST(button, Button_updateState_PressTransistionsToWaitRelease)
{
	Button_setButtonState(myTest_Button, BUTTON_PRESSED);
	Button_updateState(myTest_Button, 5);
	TEST_ASSERT_EQUAL(BUTTON_WAITING_RELEASE, Button_getButtonState(myTest_Button) );
}

TEST(button, Button_updateState_WaitReleaseStaysInWaitRelease)
{
	Button_setButtonState(myTest_Button, BUTTON_WAITING_RELEASE);
	Button_updateState(myTest_Button, 5);
	Button_updateState(myTest_Button, 6);
	Button_updateState(myTest_Button, 7);
	TEST_ASSERT_EQUAL(BUTTON_WAITING_RELEASE, Button_getButtonState(myTest_Button) );
}

TEST(button, Button_updateState_ReleaseTransistionsToWaiting)
{
	Button_setButtonState(myTest_Button, BUTTON_RELEASED);
	Button_updateState(myTest_Button, 5);
	TEST_ASSERT_EQUAL(BUTTON_WAITING, Button_getButtonState(myTest_Button) );
}

TEST(button, Button_updateState_WaitingStaysInWaiting)
{
	Button_setButtonState(myTest_Button, BUTTON_WAITING);
	Button_updateState(myTest_Button, 5);
	Button_updateState(myTest_Button, 6);
	Button_updateState(myTest_Button, 7);
	TEST_ASSERT_EQUAL(BUTTON_WAITING, Button_getButtonState(myTest_Button) );
}

TEST(button, Button_updateState_ReportStaysInReport)
{
	Button_setButtonState(myTest_Button, BUTTON_REPORT);
	Button_updateState(myTest_Button, 5);
	Button_updateState(myTest_Button, 6);
	Button_updateState(myTest_Button, 7);
	TEST_ASSERT_EQUAL(BUTTON_REPORT, Button_getButtonState(myTest_Button) );
}

//++++++++++++++++++//

TEST(button, Button_pressCallback_SetsPressTime)
{
	Button_start(myTest_Button);
	Button_pressCallback(myTest_Button, 5);
	TEST_ASSERT_EQUAL(5, Button_getPressTime(myTest_Button) );
}

TEST(button, Button_pressCallback_ReturnsSelfOnSuccess)
{
	Button_start(myTest_Button);
	TEST_ASSERT_EQUAL(myTest_Button, Button_pressCallback(myTest_Button, 5) );
}

TEST(button, Button_pressCallback_ReturnsNullOnNull)
{
	Button_start(myTest_Button);
	TEST_ASSERT_EQUAL(NULL, Button_pressCallback(NULL, 5) );
}

TEST(button, Button_pressCallback_ReturnsNullOnNegativeTimeStamp)
{
	Button_start(myTest_Button);
	TEST_ASSERT_EQUAL(NULL, Button_pressCallback(myTest_Button, -2) );
}

//++++++++++++++++++//

TEST(button, Button_releaseCallback_SetsPressTime)
{
	Button_start(myTest_Button);
	Button_pressCallback(myTest_Button, 0);
	Button_releaseCallback(myTest_Button, 5);
	TEST_ASSERT_EQUAL(5, Button_getReleaseTime(myTest_Button) );
}

TEST(button, Button_releaseCallback_ReturnsSelfOnSuccess)
{
	Button_start(myTest_Button);
	Button_pressCallback(myTest_Button, 0);
	TEST_ASSERT_EQUAL(myTest_Button, Button_releaseCallback(myTest_Button, 5) );
}

TEST(button, Button_releaseCallback_ReturnsNullOnNull)
{
	Button_start(myTest_Button);
	TEST_ASSERT_EQUAL(NULL, Button_releaseCallback(NULL, 5) );
}

TEST(button, Button_releaseCallback_ReturnsNullOnNegativeTimeStamp)
{
	Button_start(myTest_Button);
	TEST_ASSERT_EQUAL(NULL, Button_releaseCallback(myTest_Button, -2) );
}

TEST(button, Button_releaseCallback_ShortTransistionsToWait)
{
	Button_start(myTest_Button);
	Button_pressCallback(myTest_Button, 0);
	Button_releaseCallback(myTest_Button, 50);
	TEST_ASSERT_EQUAL(BUTTON_WAITING, myTest_Button->buttonState );
}

TEST(button, Button_releaseCallback_LongTransistionsToWait)
{
	Button_start(myTest_Button);
	Button_pressCallback(myTest_Button, 0);
	Button_releaseCallback(myTest_Button, 100);
	TEST_ASSERT_EQUAL(BUTTON_WAITING, myTest_Button->buttonState );
}

TEST(button, Button_releaseCallback_FiveSecondsTransistionsToReport)
{
	Button_start(myTest_Button);
	Button_pressCallback(myTest_Button, 0);
	Button_releaseCallback(myTest_Button, 500);
	TEST_ASSERT_EQUAL(BUTTON_REPORT, myTest_Button->buttonState );
}




//++++++++++++++++++
//
TEST(button, Button_reportCallback_PressTypeReturnsShortPress)
{
	Button_start(myTest_Button);
	Button_pressCallback  (myTest_Button, 10);
	Button_releaseCallback(myTest_Button, 50);
	Button_reportCallback(myTest_Button);
	TEST_ASSERT_EQUAL(BUTTON_REPORT,      Button_getButtonState(myTest_Button) );
	TEST_ASSERT_EQUAL(BUTTON_SHORT_PRESS, Button_getPressType  (myTest_Button) );
}

TEST(button, Button_reportCallback_PressTypeReturnsLongPress)
{
	Button_start(myTest_Button);
	Button_pressCallback  (myTest_Button, 0);
	Button_releaseCallback(myTest_Button, 101);
	Button_reportCallback(myTest_Button);
	TEST_ASSERT_EQUAL(BUTTON_REPORT,     Button_getButtonState(myTest_Button) );
	TEST_ASSERT_EQUAL(BUTTON_LONG_PRESS, Button_getPressType  (myTest_Button) );
}

TEST(button, Button_reportCallback_PressTypeReturnsFiveSecondPress)
{
	Button_start(myTest_Button);
	Button_pressCallback  (myTest_Button, 0);
	Button_releaseCallback(myTest_Button, 500);
	Button_reportCallback(myTest_Button);
	TEST_ASSERT_EQUAL(BUTTON_REPORT,       Button_getButtonState(myTest_Button) );
	TEST_ASSERT_EQUAL(BUTTON_FIVE_SECONDS, Button_getPressType  (myTest_Button) );
}

TEST(button, Button_reportCallback_releaseCallback_ReturnsSelfOnSuccess)
{
	Button_start(myTest_Button);
	Button_pressCallback  (myTest_Button, 0);
	Button_releaseCallback(myTest_Button, 101);
	Button_reportCallback(myTest_Button);
	TEST_ASSERT_EQUAL(myTest_Button, Button_reportCallback(myTest_Button) );
}

TEST(button, Button_reportCallback_ReturnsNullOnNull)
{
	Button_start(myTest_Button);
	Button_pressCallback  (myTest_Button, 0);
	Button_releaseCallback(myTest_Button, 101);
	Button_reportCallback(myTest_Button);
	TEST_ASSERT_EQUAL(NULL, Button_reportCallback(NULL) );
}

//++++++++++++++++++//

TEST(button, Button_TwoPresses_incrementsCountToTwo)
{
	Button_start(myTest_Button);
	Button_pressCallback  (myTest_Button, 0);
	Button_releaseCallback(myTest_Button, 101);

	Button_pressCallback  (myTest_Button, 200);
	Button_releaseCallback(myTest_Button, 301);

	Button_reportCallback(myTest_Button);
	TEST_ASSERT_EQUAL(BUTTON_REPORT,    Button_getButtonState     (myTest_Button) );
	TEST_ASSERT_EQUAL(BUTTON_LONG_PRESS, Button_getPressType(myTest_Button) );
	TEST_ASSERT_EQUAL(2, Button_getPressCount(myTest_Button) );
}

TEST(button, Button_FourPresses_incrementsCountToFour)
{
	Button_start(myTest_Button);
	Button_pressCallback  (myTest_Button, 0);
	Button_releaseCallback(myTest_Button, 1);

	Button_pressCallback  (myTest_Button, 2);
	Button_releaseCallback(myTest_Button, 3);

	Button_pressCallback  (myTest_Button, 4);
	Button_releaseCallback(myTest_Button, 5);

	Button_pressCallback  (myTest_Button, 6);
	Button_releaseCallback(myTest_Button, 7);

	Button_reportCallback(myTest_Button);
	TEST_ASSERT_EQUAL(4, Button_getPressCount(myTest_Button) );
}

//++++++++++++++++++//

TEST(button, Button_ReleaseAfterReleaseReturnsToIdle)
{
	Button_start          (myTest_Button);
	Button_pressCallback  (myTest_Button, 0);
	Button_releaseCallback(myTest_Button, 2);
	Button_releaseCallback(myTest_Button, 4);
	TEST_ASSERT_EQUAL(BUTTON_IDLE, Button_getButtonState(myTest_Button) );
}

TEST(button, Button_PressAfterPressReturnsToIdle)
{
	Button_start          (myTest_Button);
	Button_pressCallback  (myTest_Button, 0);
	Button_pressCallback  (myTest_Button, 2);
	TEST_ASSERT_EQUAL(BUTTON_IDLE, Button_getButtonState(myTest_Button) );
}

TEST(button, Button_ReportAfterReportStaysInReport)
{
	Button_start          (myTest_Button);
	Button_pressCallback  (myTest_Button, 0);
	Button_releaseCallback(myTest_Button, 2);
	Button_reportCallback (myTest_Button);
	Button_reportCallback (myTest_Button);
	TEST_ASSERT_EQUAL(1,                  Button_getPressCount (myTest_Button) );
	TEST_ASSERT_EQUAL(BUTTON_SHORT_PRESS, Button_getPressType  (myTest_Button) );
	TEST_ASSERT_EQUAL(BUTTON_REPORT,      Button_getButtonState(myTest_Button) );

}


//****  Support Methods  ****************


struct Button * button_test_general_cb(struct Button * _self)
{
	button_test_cb_count++;
	//printf("  XXX times_test_general_cb\n");
	return _self;
}

