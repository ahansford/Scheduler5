/*
 * times-test-runner.c
 *
 *  Created on: August, 2015
 *      Author: Alan
 */


#include "..\unity\unity_fixture.h"
//#include "..\unity\unity.h"

TEST_GROUP_RUNNER(button)
{
	/**************************************/
	/**************  New  *****************/
	/**/
	RUN_TEST_CASE(button, myTest_Button_IsNotNullOnCreate);
	RUN_TEST_CASE(button, Button_buttonState_isUnknownOnCreate);
	RUN_TEST_CASE(button, Button_pressCount_isUnknownOnCreate);
	RUN_TEST_CASE(button, Button_pressType_isUnknownOnCreate);
	RUN_TEST_CASE(button, Button_pressTime_isUnknownOnCreate);
	RUN_TEST_CASE(button, Button_releaseTime_isUnknownOnCreate);

	/**************************************/
	/*************  Button  ***************/
	/**/
	RUN_TEST_CASE(button, init_Button_ClassReturnsNonNull);
	RUN_TEST_CASE(button, init_classOf_myTest_Button_is_Button);
	RUN_TEST_CASE(button, init_classOf_Button_is_ButtonClass);
	RUN_TEST_CASE(button, init_ctor_pointsTo_ButtonClass_ctor);
	RUN_TEST_CASE(button, init_ctor_pointsTo_Button_ctor);
	RUN_TEST_CASE(button, init_dtor_pointsTo_Button_dtor);
	RUN_TEST_CASE(button, init_copy_pointsTo_Button_copy);
	RUN_TEST_CASE(button, init_equal_pointsTo_Button_equal);
	RUN_TEST_CASE(button, init_config_pointsTo_Time_config);

	RUN_TEST_CASE(button, init_ButtonClass_NameIs_ButtonClass);

	/**************************************/
	/*******  ButtonClass Methods  *********/
	/**/
	//RUN_TEST_CASE(button, init_setSecond_pointsTo_Time_setSecond);
	//RUN_TEST_CASE(button, init_getSecond_pointsTo_Time_getSecond);


	//****  delete/Button_dtor  ****************
	/**/
	RUN_TEST_CASE(button, delete_returns_SelfOnSuccess);
	RUN_TEST_CASE(button, delete_safeDelete_returnsNullOnSuccess);

	//****  Set/Get buttonState  ****************
	/**/
	RUN_TEST_CASE(button, getButtonState_returns_UnknownOnCreate);
	RUN_TEST_CASE(button, getButtonState_returns_specificValue);
	RUN_TEST_CASE(button, setButtonState_returnsSpecificValue);
	RUN_TEST_CASE(button, setButtonState_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(button, setButtonState_returnsSpecificValueOnSuccess);
	RUN_TEST_CASE(button, setButtonState_canSetSpecificValue);

	//****  Set/Get pressCount  ****************
	/**/
	RUN_TEST_CASE(button, getPressCount_returns_UnknownOnCreate);
	RUN_TEST_CASE(button, getPressCount_returns_specificValue);
	RUN_TEST_CASE(button, setPressCount_returnsSpecificValue);
	RUN_TEST_CASE(button, setPressCount_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(button, setPressCount_returnsSpecificValueOnSuccess);
	RUN_TEST_CASE(button, setPressCount_canSetSpecificValue);

	//****  Set/Get pressType  ****************
	/**/
	RUN_TEST_CASE(button, getPressType_returns_UnknownOnCreate);
	RUN_TEST_CASE(button, getPressType_returns_specificValue);
	RUN_TEST_CASE(button, setPressType_returnsSpecificValue);
	RUN_TEST_CASE(button, setPressType_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(button, setPressType_returnsSpecificValueOnSuccess);
	RUN_TEST_CASE(button, setPressType_canSetSpecificValue);

	//****  Set/Get releaseTime  ****************
	/**/
	RUN_TEST_CASE(button, getReleaseTime_returns_UnknownOnCreate);
	RUN_TEST_CASE(button, getReleaseTime_returns_specificValue);
	RUN_TEST_CASE(button, setReleaseTime_returnsSpecificValue);
	RUN_TEST_CASE(button, setReleaseTime_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(button, setReleaseTime_returnsSpecificValueOnSuccess);
	RUN_TEST_CASE(button, setReleaseTime_canSetSpecificValue);

	//****  Button_reportReady  ****************
	/**/
	RUN_TEST_CASE(button, reportReady_returns_NotReadyOnCreate);
	RUN_TEST_CASE(button, reportReady_returns_ReadyOnValidReport);
	RUN_TEST_CASE(button, reportReady_returnsNotReadyOnNullPtr);

	//****  copy/Time_copy  ****************
	/**/
	RUN_TEST_CASE(button, copy_returnsSelfOnSuccess);
	RUN_TEST_CASE(button, copy_AllItemsCopiedToSelf);
	RUN_TEST_CASE(button, copy_returnsNullOnNullSelf);
	RUN_TEST_CASE(button, copy_returnsNullOnNullMaster);

	//****  equal/Button_equal  ********************
	/**/
	RUN_TEST_CASE(button, myTest_Button_IsEqualTo_myTest_Button);
	RUN_TEST_CASE(button, equal_UnequalButtonStateReturn_Unequal);
	RUN_TEST_CASE(button, equal_UnequalPressCountReturn_Unequal);
	RUN_TEST_CASE(button, equal_UnequalPressTypeReturn_Unequal);
	RUN_TEST_CASE(button, equal_UnequalPressTiimeReturn_Unequal);
	RUN_TEST_CASE(button, equal_UnequalReleaseTimeReturn_Unequal);

	RUN_TEST_CASE(button, equal_NullReturns_Null);
	RUN_TEST_CASE(button, equal_CopiedButtonReturnsEqual);


	/***********  callback  **************/
	/**/
	RUN_TEST_CASE(button, Button_setButtonReport_cb_returns_Button_PostOnReportReadyCallback);
	RUN_TEST_CASE(button, Button_setButtonReport_cb_returns_new_cb);
	RUN_TEST_CASE(button, Button_setButtonReport_cb_sets_new_cb);
	RUN_TEST_CASE(button, Button_reportCallback_firesDesignatedCallback);


	/***********  config  **************/
	RUN_TEST_CASE(button, Config_copiesState);

	/***********  init  **************/
	RUN_TEST_CASE(button, CanCall_Button_Init_MulitpleTimes);

	/***********  API  **************/
	RUN_TEST_CASE(button, Button_reset_setsDataMembersToDefaults);
	RUN_TEST_CASE(button, Button_reset_returnsSelfOnSuccess);
	RUN_TEST_CASE(button, Button_reset_returnsNullOnNullPtr);
	//++++++++++++++++++//
	RUN_TEST_CASE(button, Button_enable_resetsDataMembers);
	RUN_TEST_CASE(button, Button_enable_transistionsToIdleState);
	RUN_TEST_CASE(button, Button_enable_returnsSelfOnSuccess);
	RUN_TEST_CASE(button, Button_enable_returnsNullOnNullPtr);
	//++++++++++++++++++//
	RUN_TEST_CASE(button, Button_updateState_UnknownStaysInUnknown);
	RUN_TEST_CASE(button, Button_updateState_ResetTransitionsToIdle);
	RUN_TEST_CASE(button, Button_updateState_IdleStaysInIdle);
	RUN_TEST_CASE(button, Button_updateState_PressTransistionsToWaitRelease);
	RUN_TEST_CASE(button, Button_updateState_WaitReleaseStaysInWaitRelease);
	RUN_TEST_CASE(button, Button_updateState_ReleaseTransistionsToWaiting);
	RUN_TEST_CASE(button, Button_updateState_WaitingStaysInWaiting);
	RUN_TEST_CASE(button, Button_updateState_ReportStaysInReport);
	//++++++++++++++++++//
	RUN_TEST_CASE(button, Button_pressCallback_SetsPressTime);
	RUN_TEST_CASE(button, Button_pressCallback_ReturnsSelfOnSuccess);
	RUN_TEST_CASE(button, Button_pressCallback_ReturnsNullOnNull);
	RUN_TEST_CASE(button, Button_pressCallback_ReturnsNullOnNegativeTimeStamp);
	//++++++++++++++++++//
	RUN_TEST_CASE(button, Button_releaseCallback_SetsPressTime);
	RUN_TEST_CASE(button, Button_releaseCallback_ReturnsSelfOnSuccess);
	RUN_TEST_CASE(button, Button_releaseCallback_ReturnsNullOnNull);
	RUN_TEST_CASE(button, Button_releaseCallback_ReturnsNullOnNegativeTimeStamp);
	RUN_TEST_CASE(button, Button_releaseCallback_ShortTransistionsToWait);
	RUN_TEST_CASE(button, Button_releaseCallback_LongTransistionsToWait);
	RUN_TEST_CASE(button, Button_releaseCallback_FiveSecondsTransistionsToReport);
	//++++++++++++++++++//
	RUN_TEST_CASE(button, Button_reportCallback_PressTypeReturnsShortPress);
	RUN_TEST_CASE(button, Button_reportCallback_PressTypeReturnsLongPress);
	RUN_TEST_CASE(button, Button_reportCallback_PressTypeReturnsFiveSecondPress);
	RUN_TEST_CASE(button, Button_reportCallback_releaseCallback_ReturnsSelfOnSuccess);
	RUN_TEST_CASE(button, Button_reportCallback_ReturnsNullOnNull);
	//++++++++++++++++++//
	RUN_TEST_CASE(button, Button_TwoPresses_incrementsCountToTwo);
	RUN_TEST_CASE(button, Button_FourPresses_incrementsCountToFour);
	//++++++++++++++++++//
	RUN_TEST_CASE(button, Button_ReleaseAfterReleaseReturnsToIdle);
	RUN_TEST_CASE(button, Button_PressAfterPressReturnsToIdle);
	RUN_TEST_CASE(button, Button_ReportAfterReportStaysInReport);

}

