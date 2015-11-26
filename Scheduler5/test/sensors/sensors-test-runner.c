/*
 * sensors-test-runner.c
 *
 *  Created on: August, 2015
 *      Author: Alan
 */


#include "..\unity\unity_fixture.h"
//#include "..\unity\unity.h"

TEST_GROUP_RUNNER(sensor)
{
	/**************************************/
	/**************  New  *****************/
	/**/
	RUN_TEST_CASE(sensor, myTest_Sensor_IsNotNullOnCreate);
	RUN_TEST_CASE(sensor, Sensor_sensorState_isUnknownOnCreate);
	RUN_TEST_CASE(sensor, Sensor_powerUpDelayTicks_isUnknownOnCreate);
	RUN_TEST_CASE(sensor, Sensor_resetDelayTicks_isUnknownOnCreate);
	RUN_TEST_CASE(sensor, Sensor_measurementDelayTicks_isUnknownOnCreate);

	RUN_TEST_CASE(sensor, Sensor_rawDataPointer_isNullOnCreate);
	RUN_TEST_CASE(sensor, Sensor_finalDataPointer_isNullOnCreate);
	RUN_TEST_CASE(sensor, Sensor_alarmLevelsPointer_isUnknownOnCreate);
	RUN_TEST_CASE(sensor, Sensor_alarmState_isUnknownOnCreate);
	RUN_TEST_CASE(sensor, Sensor_normalState_isUnknownOnCreate);
	RUN_TEST_CASE(sensor, Sensor_ioStructPointer_isNullOnCreate);

	/**************************************/
	/*************  Sensors  ***************/
	/**/
	RUN_TEST_CASE(sensor, init_Sensor_ClassReturnsNonNull);
	RUN_TEST_CASE(sensor, init_classOf_myTest_Sensor_is_Sensor);
	RUN_TEST_CASE(sensor, init_classOf_Sensor_is_SensorClass);
	RUN_TEST_CASE(sensor, init_ctor_pointsTo_SensorClass_ctor);
	RUN_TEST_CASE(sensor, init_ctor_pointsTo_Sensor_ctor);
	RUN_TEST_CASE(sensor, init_dtor_pointsTo_Sensor_dtor);
	RUN_TEST_CASE(sensor, init_copy_pointsTo_Sensor_copy);
	RUN_TEST_CASE(sensor, init_equal_pointsTo_Sensor_equal);
	RUN_TEST_CASE(sensor, init_config_pointsTo_Time_config);

	RUN_TEST_CASE(sensor, init_SensorClass_NameIs_SensorClass);

	/**************************************/
	/*******  SensorClass Methods  *********/
	/**/
	RUN_TEST_CASE(sensor, writeDataToSensor_pointsTo_Sensor_writeDataToSensor);
	RUN_TEST_CASE(sensor, readDataToSensor_pointsTo_Sensor_readDataToSensor);
	RUN_TEST_CASE(sensor, enableSensorPower_pointsTo_Sensor_enableSensorPower);
	RUN_TEST_CASE(sensor, resetAndConfigSensor_pointsTo_Sensor_resetAndConfigSensor);
	RUN_TEST_CASE(sensor, startSensorMeasurement_pointsTo_Sensor_startSensorMeasurement);
	RUN_TEST_CASE(sensor, storeRawSensorData_pointsTo_Sensor_storeRawSensorData);
	RUN_TEST_CASE(sensor, disableSensorPower_pointsTo_Sensor_disableSensorPower);
	RUN_TEST_CASE(sensor, processRawSensorData_pointsTo_Sensor_processRawSensorData);
	RUN_TEST_CASE(sensor, checkSensorAlarms_pointsTo_Sensor_checkSensorAlarms);

	/****  delete/Sensor_dtor  ****************/
	/**/
	RUN_TEST_CASE(sensor, delete_returns_SelfOnSuccess);
	RUN_TEST_CASE(sensor, delete_safeDelete_returnsNullOnSuccess);

	/****  Set/Get sensorState  ****************/
	/**/
	RUN_TEST_CASE(sensor, Sensor_getSensorState_returns_UnknownOnCreate);
	RUN_TEST_CASE(sensor, Sensor_getSensorState_returns_specificValue);
	RUN_TEST_CASE(sensor, Sensor_setSensorState_returnsSpecificValue);
	RUN_TEST_CASE(sensor, Sensor_setSensorState_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(sensor, Sensor_setSensorState_returnsSpecificValueOnSuccess);
	RUN_TEST_CASE(sensor, Sensor_setSensorState_canSetSpecificValue);

	/****  Set/Get AsyncFlag  ****************/
	/**/
	RUN_TEST_CASE(sensor, Sensor_getAsyncFlag_returns_UnknownOnCreate);
	RUN_TEST_CASE(sensor, Sensor_getAsyncFlag_returns_specificValue);
	RUN_TEST_CASE(sensor, Sensor_setAsyncFlag_returnsSpecificValue);
	RUN_TEST_CASE(sensor, Sensor_setAsyncFlag_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(sensor, Sensor_setAsyncFlag_returnsSpecificValueOnSuccess);
	RUN_TEST_CASE(sensor, Sensor_setAsyncFlag_canSetSpecificValue);

	/****  Set/Get powerUpDelayTicks  ****************/
	/**/
	RUN_TEST_CASE(sensor, Sensor_getPowerUpDelayTicks_returns_UnknownOnCreate);
	RUN_TEST_CASE(sensor, Sensor_getPowerUpDelayTicks_returns_specificValue);
	RUN_TEST_CASE(sensor, Sensor_setPowerUpDelayTicks_returnsSpecificValue);
	RUN_TEST_CASE(sensor, Sensor_setPowerUpDelayTicks_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(sensor, Sensor_setPowerUpDelayTicks_returnsSpecificValueOnSuccess);
	RUN_TEST_CASE(sensor, Sensor_setPowerUpDelayTicks_canSetSpecificValue);

	/****  Set/Get ResetDelayTicks  ****************/
	/**/
	RUN_TEST_CASE(sensor, Sensor_getResetDelayTicks_returns_UnknownOnCreate);
	RUN_TEST_CASE(sensor, Sensor_getResetDelayTicks_returns_specificValue);
	RUN_TEST_CASE(sensor, Sensor_setResetDelayTicks_returnsSpecificValue);
	RUN_TEST_CASE(sensor, Sensor_setResetDelayTicks_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(sensor, Sensor_setResetDelayTicks_returnsSpecificValueOnSuccess);
	RUN_TEST_CASE(sensor, Sensor_setResetDelayTicks_canSetSpecificValue);

	/****  Set/Get MesurementDelayTicks  ****************/
	/**/
	RUN_TEST_CASE(sensor, Sensor_getMeasurementDelayTicks_returns_UnknownOnCreate);
	RUN_TEST_CASE(sensor, Sensor_getMeasurementDelayTicks_returns_specificValue);
	RUN_TEST_CASE(sensor, Sensor_setMeasurementDelayTicks_returnsSpecificValue);
	RUN_TEST_CASE(sensor, Sensor_setMeasurementDelayTicks_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(sensor, Sensor_setMeasurementDelayTicks_returnsSpecificValueOnSuccess);
	RUN_TEST_CASE(sensor, Sensor_setMeasurementDelayTicks_canSetSpecificValue);

		/****  Set/Get RawDataPointer  ****************/
	/**/
	RUN_TEST_CASE(sensor, Sensor_getRawDataPointer_returns_UnknownOnCreate);
	RUN_TEST_CASE(sensor, Sensor_getRawDataPointer_returns_specificValue);
	RUN_TEST_CASE(sensor, Sensor_setRawDataPointer_returnsSpecificValue);
	RUN_TEST_CASE(sensor, Sensor_setRawDataPointer_returnsUnknownOnNullPtr);

	/****  Set/Get FinalDataPointer  ****************/
	/**/
	RUN_TEST_CASE(sensor, Sensor_getFinalDataPointer_returns_UnknownOnCreate);
	RUN_TEST_CASE(sensor, Sensor_getFinalDataPointer_returns_specificValue);
	RUN_TEST_CASE(sensor, Sensor_setFinalDataPointer_returnsSpecificValue);
	RUN_TEST_CASE(sensor, Sensor_setFinalDataPointer_returnsUnknownOnNullPtr);

	/****  Set/Get AlarmLevelsPointer  ****************/
	/**/
	RUN_TEST_CASE(sensor, Sensor_getAlarmLevelsPointer_returns_UnknownOnCreate);
	RUN_TEST_CASE(sensor, Sensor_getAlarmLevelsPointer_returns_specificValue);
	RUN_TEST_CASE(sensor, Sensor_setAlarmLevelsPointer_returnsSpecificValue);
	RUN_TEST_CASE(sensor, Sensor_setAlarmLevelsPointer_returnsUnknownOnNullPtr);

	RUN_TEST_CASE(sensor, Sensor_getLowerPrimaryAlarmLevel_returnsUnknownOnCreate);
	RUN_TEST_CASE(sensor, Sensor_setLowerPrimaryAlarmLevel_setsSpecficValue);
	RUN_TEST_CASE(sensor, Sensor_getLowerPrimaryAlarmLevel_returnsSpecificValue);

	RUN_TEST_CASE(sensor, Sensor_getUpperSecondaryAlarmLevel_returnsUnknownOnCreate);
	RUN_TEST_CASE(sensor, Sensor_setUpperSecondaryAlarmLevel_setsSpecficValue);
	RUN_TEST_CASE(sensor, Sensor_getUpperSecondaryAlarmLevel_returnsSpecificValue);

	/****  Set/Get AlarmState  ****************/
	/**/
	RUN_TEST_CASE(sensor, Sensor_getAlarmState_returns_UnknownOnCreate);
	RUN_TEST_CASE(sensor, Sensor_getAlarmState_returns_specificValue);
	RUN_TEST_CASE(sensor, Sensor_setAlarmState_returnsSpecificValue);
	RUN_TEST_CASE(sensor, Sensor_setAlarmState_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(sensor, Sensor_setAlarmState_returnsSpecificValueOnSuccess);
	RUN_TEST_CASE(sensor, Sensor_setAlarmState_canSetSpecificValue);

	/****  Set/Get NormalState  ****************/
	/**/
	RUN_TEST_CASE(sensor, Sensor_getNormalState_returns_UnknownOnCreate);
	RUN_TEST_CASE(sensor, Sensor_getNormalState_returns_specificValue);
	RUN_TEST_CASE(sensor, Sensor_setNormalState_returnsSpecificValue);
	RUN_TEST_CASE(sensor, Sensor_setNormalState_returnsUnknownOnNullPtr);
	RUN_TEST_CASE(sensor, Sensor_setNormalState_returnsSpecificValueOnSuccess);
	RUN_TEST_CASE(sensor, Sensor_setNormalState_canSetSpecificValue);

	/****  check alarm state  ****************/
	/**/
	RUN_TEST_CASE(sensor, Sensor_checkAlarms_detects_Below);
	RUN_TEST_CASE(sensor, Sensor_checkAlarms_detects_BelowEqual);
	RUN_TEST_CASE(sensor, Sensor_checkAlarms_detects_Equal);
	RUN_TEST_CASE(sensor, Sensor_checkAlarms_detects_AboveEqual);
	RUN_TEST_CASE(sensor, Sensor_checkAlarms_detects_Above);

	RUN_TEST_CASE(sensor, Sensor_checkAlarms_detects_Between);
	RUN_TEST_CASE(sensor, Sensor_checkAlarms_detects_Outside);

	RUN_TEST_CASE(sensor, Sensor_checkAlarms_Returns_UnknownOnMissingThresholds);
	RUN_TEST_CASE(sensor, Sensor_checkAlarms_Returns_UnknownOnMissingUpperThreshold);
	RUN_TEST_CASE(sensor, Sensor_checkAlarms_Returns_UnknownOnMissingLowerThreshold);

	/****  Sensor_dataReady  ****************/
	/**/
	RUN_TEST_CASE(sensor, Sensor_reportReady_returns_NotReadyOnCreate);
	RUN_TEST_CASE(sensor, Sensor_reportReady_returns_ReadyOnValidReport);
	RUN_TEST_CASE(sensor, Sensor_reportReady_returnsNotReadyOnNullPtr);

	/****  Set/Get IoStructPointer  ****************/
	/**/
	RUN_TEST_CASE(sensor, Sensor_getIoStructPointer_returns_UnknownOnCreate);
	RUN_TEST_CASE(sensor, Sensor_getIoStructPointer_returns_specificValue);
	RUN_TEST_CASE(sensor, Sensor_setIoStructPointer_returnsSpecificValue);
	RUN_TEST_CASE(sensor, Sensor_setIoStructPointer_returnsUnknownOnNullPtr);

	/****  getIoCommandBufPointer  ****************/
	/**/
	RUN_TEST_CASE(sensor, Sensor_getIoCommandBufPointer_returns_UnknownOnCreate);
	RUN_TEST_CASE(sensor, Sensor_getIoCommandBufPointer_returns_specificValue);

	//****  copy/Time_copy  ****************
	/**/
	RUN_TEST_CASE(sensor, copy_returnsSelfOnSuccess);
	RUN_TEST_CASE(sensor, copy_AllItemsCopiedToSelf);
	RUN_TEST_CASE(sensor, copy_returnsNullOnNullSelf);
	RUN_TEST_CASE(sensor, copy_returnsNullOnNullMaster);

	//****  equal/Button_equal  ********************
	/**/
	RUN_TEST_CASE(sensor, myTest_Sensor_IsEqualTo_myTest_Sensor);
	RUN_TEST_CASE(sensor, equal_UnequalSensorStateReturn_Unequal);
	RUN_TEST_CASE(sensor, equal_UnequalResetDelayTicksReturn_Unequal);
	RUN_TEST_CASE(sensor, equal_UnequalMeasurementDelayTicksReturn_Unequal);
	RUN_TEST_CASE(sensor, equal_UnequalAlarmStateReturn_Unequal);
	RUN_TEST_CASE(sensor, equal_UnequalNormalStateReturn_Unequal);
	RUN_TEST_CASE(sensor, equal_UnequalNormalReportReadyCB_Unequal);

	RUN_TEST_CASE(sensor, equal_NullReturns_Null);
	RUN_TEST_CASE(sensor, equal_CopiedSensorReturnsEqual);


	/***********  onReportReady callback  **************/
	/**/
	RUN_TEST_CASE(sensor, getSensor_onReportReady_cb_returns_emptyReportReadyCallback_onCreate);
	RUN_TEST_CASE(sensor, getSensor_onReportReady_cb_returns_new_cb);
	RUN_TEST_CASE(sensor, setSensor_onReportReady_cb_sets_new_cb);
	RUN_TEST_CASE(sensor, Sensor_reportReady_firesDesignatedCallback);

	/***********  onReportReady callback  **************/
	/**/
	RUN_TEST_CASE(sensor, getSensor_onAlarmTriggered_cb_returns_emptyReportReadyCallback_onCreate);
	RUN_TEST_CASE(sensor, getSensor_onAlarmTriggered_cb_returns_new_cb);
	RUN_TEST_CASE(sensor, setSensor_onAlarmTriggered_cb_sets_new_cb);
	RUN_TEST_CASE(sensor, Sensor_AlarmTriggered_firesDesignatedCallback);

	/***********  config  **************/
	//RUN_TEST_CASE(sensor, Config_copiesState);


	/***********  init  **************/
	/**/
	RUN_TEST_CASE(sensor, CanCall_Sensor_Init_MulitpleTimes);
	RUN_TEST_CASE(sensor, memory_leak);

	/***********  API  **************/
	/**/

	RUN_TEST_CASE(sensor, Sensor_start_setsAsyncFlagToStartRequest);
	RUN_TEST_CASE(sensor, Sensor_start_setsAsyncFlagToStartDoneAfterUpdate);
	RUN_TEST_CASE(sensor, Sensor_start_doesNotModifiyAsyncFlagIfAlreadyStarted);

	RUN_TEST_CASE(sensor, Sensor_measure_triggeresStartWhenNotStarted);
	RUN_TEST_CASE(sensor, Sensor_measure_triggeresMeasureWhenReadyIdle);
	RUN_TEST_CASE(sensor, Sensor_measure_triggeresMeasureWhenReport);
	RUN_TEST_CASE(sensor, Sensor_measure_secondMeasureDoesNotChangeMainSM);

	RUN_TEST_CASE(sensor, Sensor_start_setsAlarmTypeToUnknown);
	RUN_TEST_CASE(sensor, Sensor_start_setsNormalTypeToUnknown);
	RUN_TEST_CASE(sensor, Sensor_start_stateEndsInIdle);
	RUN_TEST_CASE(sensor, Sensor_start_returnsSelfOnSuccess);
	RUN_TEST_CASE(sensor, Sensor_start_returnsNullOnNullPtr);

	RUN_TEST_CASE(sensor, Sensor_armDelayedCallback_addsTask);
	RUN_TEST_CASE(sensor, Sensor_armDelayedCallback_returnsCallBackFnOnSuccess);
	RUN_TEST_CASE(sensor, Sensor_armDelayedCallback_returnsNullOnNullSensorPTR);
	RUN_TEST_CASE(sensor, Sensor_armDelayedCallback_returnsNullOnNullCallback);
	RUN_TEST_CASE(sensor, Sensor_armDelayedCallback_returnsNullOnTickLessThanOne);

	RUN_TEST_CASE(sensor, Sensor_measureAndProcess_sendsAllCommands);

	RUN_TEST_CASE(sensor, Sensor_enablePower_armsPowerUpCallback);
	RUN_TEST_CASE(sensor, Sensor_configAndAlign_armsConfigAlignCallback);
	RUN_TEST_CASE(sensor, Sensor_measure_armsMeasureCallback);

	RUN_TEST_CASE(sensor, Sensor_measureAndProcess_stateEndsInReport);
	RUN_TEST_CASE(sensor, Sensor_measureAndProcess_returnsSelfOnSuccess);
	RUN_TEST_CASE(sensor, Sensor_measureAndProcess_returnsNullOnNullPtr);

	RUN_TEST_CASE(sensor, Sensor_stop_endsInUnpoweredIdle);

	RUN_TEST_CASE(sensor, Sensor_stop_endsInUnpoweredIdle);
	//RUN_TEST_CASE(sensor, Sensor_stop_sendsPowerDownCommands);

	RUN_TEST_CASE(sensor, Sensor_resetMiniState_setsToStateZero);
	RUN_TEST_CASE(sensor, Sensor_incrementMiniState_movesToNextState);


	/***********  Sensor_postXXXXcallbacks  **************/
	/**/
	RUN_TEST_CASE(sensor, Sensor_postEnableSensorPower_stateEndsInAlignConfig);
	RUN_TEST_CASE(sensor, Sensor_postEnableSensorPower_NoStateChangeOnNullPtr);

	RUN_TEST_CASE(sensor, Sensor_postAlignAndConfig_stateEndsInStartMeasure);
	RUN_TEST_CASE(sensor, Sensor_postAlignAndConfig_NoStateChangeOnNullPtr);

	RUN_TEST_CASE(sensor, Sensor_postStartMeasurement_stateEndsInGetRawData);
	RUN_TEST_CASE(sensor, Sensor_postStartMeasurement_NoStateChangeOnNullPtr);

}

