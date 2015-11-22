/*
 * sensors-test.c
 *
 *  Created on: Sept, 2015
 *      Author: Alan
 */


#include "..\..\src\cross_compiler_defs.h"
#include "..\unity\unity_fixture.h"
//#include "..\unity\unity.h"
#include "..\..\src\mocks\RuntimeErrorStub.h"

#include "..\..\src\sensors\sensors.h"
//#include "..\..\src\sensors\sensors-private.h"

/****************************************/
/*** Basic Object and Class Testing ****/
struct       Sensor *      myTest_Sensor;
const struct SensorClass * myTest_SensorClass_PTR;
const struct Class *       myTest_Sensor_class_PTR;

struct Sensor * memoryLeakPointer;

struct_task_t testTASKS_sensors[SCHEDULER_MAX_TASKS];

struct Sensor * Sensor_test_general_cb(struct Sensor * _self);
struct Sensor * Sensor_test_general_cb2(struct Sensor * _self);
int sensor_test_cb_count;
int sensor_test_cb_count2;

int i;

/****************************************/
/*********** Start of Testing ***********/
TEST_GROUP(sensor);

TEST_SETUP(sensor)
{
	RuntimeErrorStub_Reset();
	Sensor_init();

	sensor_test_cb_count  = 0;
	sensor_test_cb_count2 = 0;

	myTest_Sensor = new(Sensor);
	if ( myTest_Sensor == NULL ) {printf("failed to allocate memory for new(Sensor)\n"); }
	myTest_SensorClass_PTR  = classOf(myTest_Sensor);
	myTest_Sensor_class_PTR = Sensor;
	scheduler_Create(testTASKS_sensors);
}

TEST_TEAR_DOWN(sensor)
{
	myTest_Sensor = safeDelete(myTest_Sensor);

	for ( i = 0; i < SCHEDULER_MAX_TASKS; i++)
		{ scheduler_DeleteTask(i); }

	//myTest_Sensor = new(Sensor);
	//if ( myTest_Sensor != memoryLeakPointer )
	//	{ printf("\nPossible memory leak in Sensors\n"); }
	//myTest_Sensor = safeDelete(myTest_Sensor);

	RuntimeErrorStub_Reset();
}

/**************************************/
/**************  New  *****************/

TEST(sensor, myTest_Sensor_IsNotNullOnCreate)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  myTest_Sensor);
}

TEST(sensor, Sensor_sensorState_isUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(SENSOR_STATE_UNKNOWN,  myTest_Sensor->sensorState);
}

TEST(sensor, Sensor_powerUpDelayTicks_isUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(SENSOR_DELAY_TICKS_UNKNOWN,  myTest_Sensor->powerUpDelayTicks);
}

TEST(sensor, Sensor_resetDelayTicks_isUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(SENSOR_DELAY_TICKS_UNKNOWN,  myTest_Sensor->resetDelayTicks);
}

TEST(sensor, Sensor_measurementDelayTicks_isUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(SENSOR_DELAY_TICKS_UNKNOWN,  myTest_Sensor->measurementDelayTicks);
}

TEST(sensor, Sensor_commandPointer_isNullOnCreate)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  myTest_Sensor->commandPointer);
}

TEST(sensor, Sensor_rawDataPointer_isNullOnCreate)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  myTest_Sensor->rawDataPointer);
}

TEST(sensor, Sensor_finalDataPointer_isNullOnCreate)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  myTest_Sensor->finalDataPointer);
}

TEST(sensor, Sensor_alarmLevelsPointer_isUnknownOnCreate)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  myTest_Sensor->alarmLevelsPointer);
}

TEST(sensor, Sensor_alarmState_isUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(ALARM_TYPE_UNKNOWN,  myTest_Sensor->alarmState);
}

TEST(sensor, Sensor_normalState_isUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(ALARM_TYPE_UNKNOWN,  myTest_Sensor->normalState);
}
/**/

/**************************************/
/*************  Sensor  ***************/
/**/
TEST(sensor, init_Sensor_ClassReturnsNonNull)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  myTest_Sensor->_.class);
}

TEST(sensor, init_classOf_myTest_Sensor_is_Sensor)
{
	TEST_ASSERT_EQUAL_PTR(Sensor,  myTest_Sensor->_.class);
}

TEST(sensor, init_classOf_Sensor_is_SensorClass)
{
	TEST_ASSERT_EQUAL_PTR(SensorClass,  myTest_Sensor->_.class->_.class);
}

TEST(sensor, init_ctor_pointsTo_SensorClass_ctor)
{
	TEST_ASSERT_EQUAL_PTR(Sensor_default_ctor,  myTest_Sensor->_.class->ctor);
	TEST_ASSERT_EQUAL_PTR(SensorClass_ctor,     myTest_Sensor->_.class->_.class->ctor);
}

TEST(sensor, init_ctor_pointsTo_Sensor_ctor)
{
	TEST_ASSERT_EQUAL_PTR(Sensor_default_ctor,  myTest_Sensor->_.class->ctor);
}

TEST(sensor, init_dtor_pointsTo_Sensor_dtor)
{
	TEST_ASSERT_EQUAL_PTR(Sensor_default_dtor,  ((struct Class *)myTest_SensorClass_PTR)->dtor );
	TEST_ASSERT_EQUAL_PTR(Sensor_default_dtor,  myTest_Sensor->_.class->dtor);
}

TEST(sensor, init_copy_pointsTo_Sensor_copy)
{
	TEST_ASSERT_EQUAL_PTR(Sensor_default_copy,  myTest_Sensor->_.class->copy);
}

TEST(sensor, init_equal_pointsTo_Sensor_equal)
{
	TEST_ASSERT_EQUAL_PTR(Sensor_default_equal,  myTest_Sensor->_.class->equal);
}

TEST(sensor, init_config_pointsTo_Time_config)
{
	TEST_ASSERT_EQUAL_PTR(Sensor_config,  myTest_Sensor->_.class->config);
}

TEST(sensor, init_SensorClass_NameIs_SensorClass)
{
	TEST_ASSERT_EQUAL_STRING("SensorClass",  myTest_Sensor->_.class->_.class->className);
}


/**************************************/
/*******  SensorClass Methods  *********/

/**/
TEST(sensor, writeDataToSensor_pointsTo_Sensor_writeDataToSensor)
{
	TEST_ASSERT_EQUAL_PTR(Sensor_default_writeDataToSensor,  myTest_SensorClass_PTR->writeDataToSensor );
}

TEST(sensor, readDataToSensor_pointsTo_Sensor_readDataToSensor)
{
	TEST_ASSERT_EQUAL_PTR(Sensor_default_readDataFromSensor,  myTest_SensorClass_PTR->readDataFromSensor );
}

TEST(sensor, enableSensorPower_pointsTo_Sensor_enableSensorPower)
{
	TEST_ASSERT_EQUAL_PTR(Sensor_default_enablePower,  myTest_SensorClass_PTR->enablePower );
}

TEST(sensor, resetAndConfigSensor_pointsTo_Sensor_resetAndConfigSensor)
{
	TEST_ASSERT_EQUAL_PTR(Sensor_default_alignAndConfig,  myTest_SensorClass_PTR->alignAndConfig );
}
/**/
TEST(sensor, startSensorMeasurement_pointsTo_Sensor_startSensorMeasurement)
{
	TEST_ASSERT_EQUAL_PTR(Sensor_default_startMeasurement,  myTest_SensorClass_PTR->startMeasurement );
}

TEST(sensor, storeRawSensorData_pointsTo_Sensor_storeRawSensorData)
{
	TEST_ASSERT_EQUAL_PTR(Sensor_default_storeRawData,  myTest_SensorClass_PTR->storeRawData );
}

TEST(sensor, disableSensorPower_pointsTo_Sensor_disableSensorPower)
{
	TEST_ASSERT_EQUAL_PTR(Sensor_default_disablePower,  myTest_SensorClass_PTR->disablePower );
}

TEST(sensor, processRawSensorData_pointsTo_Sensor_processRawSensorData)
{
	TEST_ASSERT_EQUAL_PTR(Sensor_default_processRawData,  myTest_SensorClass_PTR->processRawData );
}

TEST(sensor, checkSensorAlarms_pointsTo_Sensor_checkSensorAlarms)
{
	TEST_ASSERT_EQUAL_PTR(Sensor_default_checkSensorAlarms,  myTest_SensorClass_PTR->checkAlarms );
}


/****  delete/Time_dtor  ****************/
/**/
TEST(sensor, delete_returns_SelfOnSuccess)
{
	TEST_ASSERT_EQUAL_PTR(myTest_Sensor,  delete(myTest_Sensor));
	myTest_Sensor = NULL;
}

TEST(sensor, delete_safeDelete_returnsNullOnSuccess)
{
	TEST_ASSERT_EQUAL_PTR(NULL,  safeDelete(myTest_Sensor));
	myTest_Sensor = NULL;
}


/****  set and get sensorState  ****************/
/**/
TEST(sensor, Sensor_getSensorState_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(SENSOR_STATE_UNKNOWN,  Sensor_getSensorState(myTest_Sensor) );
}

TEST(sensor, Sensor_getSensorState_returns_specificValue)
{
	myTest_Sensor->sensorState = SENSOR_START_MEASUREMENT;
	TEST_ASSERT_EQUAL(SENSOR_START_MEASUREMENT,  Sensor_getSensorState(myTest_Sensor) );
}

TEST(sensor, Sensor_setSensorState_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(myTest_Sensor->sensorState,  Sensor_setSensorState(myTest_Sensor, 5));
}

TEST(sensor, Sensor_setSensorState_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(SENSOR_STATE_UNKNOWN,  Sensor_setSensorState(NULL, SENSOR_ALIGN_CONFIG));
}

TEST(sensor, Sensor_setSensorState_returnsSpecificValueOnSuccess)
{
	TEST_ASSERT_EQUAL(SENSOR_ALIGN_CONFIG,  Sensor_setSensorState(myTest_Sensor, SENSOR_ALIGN_CONFIG));
}

TEST(sensor, Sensor_setSensorState_canSetSpecificValue)
{
	Sensor_setSensorState(myTest_Sensor, SENSOR_ALIGN_CONFIG);
	TEST_ASSERT_EQUAL(SENSOR_ALIGN_CONFIG,  myTest_Sensor->sensorState);
}

/****  set and get AsyncFlag  ****************/
/**/
TEST(sensor, Sensor_getAsyncFlag_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(SENSOR_ASYNC_FLAG_UNKNOWN,  Sensor_getAsyncFlag(myTest_Sensor) );
}

TEST(sensor, Sensor_getAsyncFlag_returns_specificValue)
{
	myTest_Sensor->asyncFlag = SENSOR_ASYNC_START_REQUEST;
	TEST_ASSERT_EQUAL(SENSOR_ASYNC_START_REQUEST,  Sensor_getAsyncFlag(myTest_Sensor) );
}

TEST(sensor, Sensor_setAsyncFlag_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(myTest_Sensor->asyncFlag,  Sensor_setAsyncFlag(myTest_Sensor, 2));
}

TEST(sensor, Sensor_setAsyncFlag_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(SENSOR_STATE_UNKNOWN,  Sensor_setAsyncFlag(NULL, SENSOR_ASYNC_START_REQUEST));
}

TEST(sensor, Sensor_setAsyncFlag_returnsSpecificValueOnSuccess)
{
	TEST_ASSERT_EQUAL(SENSOR_ASYNC_START_REQUEST,  Sensor_setAsyncFlag(myTest_Sensor, SENSOR_ASYNC_START_REQUEST));
}

TEST(sensor, Sensor_setAsyncFlag_canSetSpecificValue)
{
	Sensor_setAsyncFlag(myTest_Sensor, SENSOR_ASYNC_START_REQUEST);
	TEST_ASSERT_EQUAL(SENSOR_ASYNC_START_REQUEST,  myTest_Sensor->asyncFlag);
}

/****  Set/Get setPowerUpDelayTicks  ****************/
/**/
TEST(sensor, Sensor_getPowerUpDelayTicks_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(SENSOR_DELAY_TICKS_UNKNOWN,  Sensor_getEnablePowerDelayTicks(myTest_Sensor) );
}

TEST(sensor, Sensor_getPowerUpDelayTicks_returns_specificValue)
{
	myTest_Sensor->powerUpDelayTicks = 5;
	TEST_ASSERT_EQUAL(5,  Sensor_getEnablePowerDelayTicks(myTest_Sensor) );
}

TEST(sensor, Sensor_setPowerUpDelayTicks_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(4,  Sensor_setPowerUpDelayTicks(myTest_Sensor, 4));
}

TEST(sensor, Sensor_setPowerUpDelayTicks_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(SENSOR_DELAY_TICKS_UNKNOWN,  Sensor_setPowerUpDelayTicks(NULL, 5));
}

TEST(sensor, Sensor_setPowerUpDelayTicks_returnsSpecificValueOnSuccess)
{
	TEST_ASSERT_EQUAL(6,  Sensor_setPowerUpDelayTicks(myTest_Sensor, 6));
}

TEST(sensor, Sensor_setPowerUpDelayTicks_canSetSpecificValue)
{
	Sensor_setPowerUpDelayTicks(myTest_Sensor, 7);
	TEST_ASSERT_EQUAL(7,  myTest_Sensor->powerUpDelayTicks);
}

/****  Set/Get resetDelayTicks  ****************/
/**/
TEST(sensor, Sensor_getResetDelayTicks_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(SENSOR_DELAY_TICKS_UNKNOWN,  Sensor_getAlignConfigDelayTicks(myTest_Sensor) );
}

TEST(sensor, Sensor_getResetDelayTicks_returns_specificValue)
{
	myTest_Sensor->resetDelayTicks = 6;
	TEST_ASSERT_EQUAL(6,  Sensor_getAlignConfigDelayTicks(myTest_Sensor) );
}

TEST(sensor, Sensor_setResetDelayTicks_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(4,  Sensor_setAlignConfigDelayTicks(myTest_Sensor, 4));
}

TEST(sensor, Sensor_setResetDelayTicks_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(SENSOR_DELAY_TICKS_UNKNOWN,  Sensor_setAlignConfigDelayTicks(NULL, 5));
}

TEST(sensor, Sensor_setResetDelayTicks_returnsSpecificValueOnSuccess)
{
	TEST_ASSERT_EQUAL(6,  Sensor_setAlignConfigDelayTicks(myTest_Sensor, 6));
}

TEST(sensor, Sensor_setResetDelayTicks_canSetSpecificValue)
{
	Sensor_setAlignConfigDelayTicks(myTest_Sensor, 7);
	TEST_ASSERT_EQUAL(7,  myTest_Sensor->resetDelayTicks);
}


/****  Set/Get MeasurementDelayTicks  ****************/
/**/
TEST(sensor, Sensor_getMeasurementDelayTicks_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(SENSOR_DELAY_TICKS_UNKNOWN,  Sensor_getMeasurementDelayTicks(myTest_Sensor) );
}

TEST(sensor, Sensor_getMeasurementDelayTicks_returns_specificValue)
{
	myTest_Sensor->measurementDelayTicks = 6;
	TEST_ASSERT_EQUAL(6,  Sensor_getMeasurementDelayTicks(myTest_Sensor) );
}

TEST(sensor, Sensor_setMeasurementDelayTicks_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(4,  Sensor_setMeasurementDelayTicks(myTest_Sensor, 4));
}

TEST(sensor, Sensor_setMeasurementDelayTicks_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(SENSOR_DELAY_TICKS_UNKNOWN,  Sensor_setMeasurementDelayTicks(NULL, 5));
}

TEST(sensor, Sensor_setMeasurementDelayTicks_returnsSpecificValueOnSuccess)
{
	TEST_ASSERT_EQUAL(6,  Sensor_setMeasurementDelayTicks(myTest_Sensor, 6));
}

TEST(sensor, Sensor_setMeasurementDelayTicks_canSetSpecificValue)
{
	Sensor_setMeasurementDelayTicks(myTest_Sensor, 7);
	TEST_ASSERT_EQUAL(7,  myTest_Sensor->measurementDelayTicks);
}

/****  Set/Get CommandPointer  ****************/
/**/
TEST(sensor, Sensor_getCommandPointer_returns_UnknownOnCreate)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  Sensor_getCommandPointer(myTest_Sensor) );
}

TEST(sensor, Sensor_getCommandPointer_returns_specificValue)
{
	// WARNING: object will be deleted on completion of test ... MUST return correct PTR
	void * originalPTR = myTest_Sensor->commandPointer;
	myTest_Sensor->commandPointer = (void *)6;
	TEST_ASSERT_EQUAL_PTR((void *)6,  Sensor_getCommandPointer(myTest_Sensor) );
	myTest_Sensor->commandPointer = originalPTR;
}

TEST(sensor, Sensor_setCommandPointer_returnsSpecificValue)
{
	// WARNING: object will be deleted on completion of test ... MUST return correct PTR
	void * originalPTR = myTest_Sensor->commandPointer;
	TEST_ASSERT_EQUAL_PTR((void *)4,  Sensor_setCommandPointer(myTest_Sensor, (void *)4));
	myTest_Sensor->commandPointer = originalPTR;
}

TEST(sensor, Sensor_setCommandPointer_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL_PTR(NULL,  Sensor_setCommandPointer(NULL, (void *)5));
}


/****  Set/Get RawDataPointer  ****************/
/**/
TEST(sensor, Sensor_getRawDataPointer_returns_UnknownOnCreate)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  Sensor_getRawDataPointer(myTest_Sensor) );
}

TEST(sensor, Sensor_getRawDataPointer_returns_specificValue)
{
	// WARNING: object will be deleted on completion of test ... MUST return correct PTR
	void * originalPTR = myTest_Sensor->rawDataPointer;
	myTest_Sensor->rawDataPointer = (void *)6;
	TEST_ASSERT_EQUAL_PTR((void *)6,  Sensor_getRawDataPointer(myTest_Sensor) );
	myTest_Sensor->rawDataPointer = originalPTR;
}

TEST(sensor, Sensor_setRawDataPointer_returnsSpecificValue)
{
	// WARNING: object will be deleted on completion of test ... MUST return correct PTR
	void * originalPTR = myTest_Sensor->rawDataPointer;
	TEST_ASSERT_EQUAL_PTR((void *)4,  Sensor_setRawDataPointer(myTest_Sensor, (void *)4));
	myTest_Sensor->rawDataPointer = originalPTR;
}

TEST(sensor, Sensor_setRawDataPointer_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL_PTR(NULL,  Sensor_setRawDataPointer(NULL, (void *)5));
}

/****  Set/Get FinalDataPointer  ****************/
/**/
TEST(sensor, Sensor_getFinalDataPointer_returns_UnknownOnCreate)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  Sensor_getFinalDataPointer(myTest_Sensor) );
}

TEST(sensor, Sensor_getFinalDataPointer_returns_specificValue)
{
	// WARNING: object will be deleted on completion of test ... MUST return correct PTR
	void * originalPTR = myTest_Sensor->finalDataPointer;
	myTest_Sensor->finalDataPointer = (void *)6;
	TEST_ASSERT_EQUAL_PTR((void *)6,  Sensor_getFinalDataPointer(myTest_Sensor) );
	myTest_Sensor->finalDataPointer = originalPTR;
}

TEST(sensor, Sensor_setFinalDataPointer_returnsSpecificValue)
{
	// WARNING: object will be deleted on completion of test ... MUST return correct PTR
	void * originalPTR = myTest_Sensor->finalDataPointer;
	TEST_ASSERT_EQUAL_PTR((void *)4,  Sensor_setFinalDataPointer(myTest_Sensor, (void *)4));
	myTest_Sensor->finalDataPointer = originalPTR;
}

TEST(sensor, Sensor_setFinalDataPointer_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL_PTR(NULL,  Sensor_setFinalDataPointer(NULL, (void *)5));
}


/****  Set/Get AlarmLevelsPointer  ****************/
/**/
TEST(sensor, Sensor_getAlarmLevelsPointer_returns_UnknownOnCreate)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  Sensor_getAlarmLevelsPointer(myTest_Sensor) );
}

TEST(sensor, Sensor_getAlarmLevelsPointer_returns_specificValue)
{
	// WARNING: object will be deleted on completion of test ... MUST return correct PTR
	void * originalPTR = myTest_Sensor->alarmLevelsPointer;
	myTest_Sensor->alarmLevelsPointer = (void *)6;
	TEST_ASSERT_EQUAL_PTR((void *)6,  Sensor_getAlarmLevelsPointer(myTest_Sensor) );
	myTest_Sensor->alarmLevelsPointer = originalPTR;
}

TEST(sensor, Sensor_setAlarmLevelsPointer_returnsSpecificValue)
{
	// WARNING: object will be deleted on completion of test ... MUST return correct PTR
	void * originalPTR = myTest_Sensor->alarmLevelsPointer;
	TEST_ASSERT_EQUAL_PTR((void *)4,  Sensor_setAlarmLevelsPointer(myTest_Sensor, (void *)4));
	myTest_Sensor->alarmLevelsPointer = originalPTR;
}

TEST(sensor, Sensor_setAlarmLevelsPointer_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL_PTR(NULL,  Sensor_setAlarmLevelsPointer(NULL, (void *)5));
}

TEST(sensor, Sensor_getLowerPrimaryAlarmLevel_returnsUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(VALUE_UNKNOWN,  Sensor_getLowerPrimaryAlarmLevel(myTest_Sensor));
}

TEST(sensor, Sensor_setLowerPrimaryAlarmLevel_setsSpecficValue)
{
	TEST_ASSERT_EQUAL(myTest_Sensor,  Sensor_setLowerPrimaryAlarmLevel(myTest_Sensor, 5));
	TEST_ASSERT_EQUAL(5,  ((struct Node *)myTest_Sensor->alarmLevelsPointer)->nodeValue);
}

TEST(sensor, Sensor_getLowerPrimaryAlarmLevel_returnsSpecificValue)
{
	((struct Node *)myTest_Sensor->alarmLevelsPointer)->nodeValue = 6;
	TEST_ASSERT_EQUAL(6,  Sensor_getLowerPrimaryAlarmLevel(myTest_Sensor));
}

TEST(sensor, Sensor_getUpperSecondaryAlarmLevel_returnsUnknownOnCreate)
{
	TEST_ASSERT_EQUAL(VALUE_UNKNOWN,  Sensor_getUpperSecondaryAlarmLevel(myTest_Sensor));
}

TEST(sensor, Sensor_setUpperSecondaryAlarmLevel_setsSpecficValue)
{
	TEST_ASSERT_EQUAL(myTest_Sensor,  Sensor_setUpperSecondaryAlarmLevel(myTest_Sensor, 5));
	TEST_ASSERT_EQUAL(5,  ((struct Node *)myTest_Sensor->alarmLevelsPointer)->linkedNode->nodeValue);
}

TEST(sensor, Sensor_getUpperSecondaryAlarmLevel_returnsSpecificValue)
{
	((struct Node *)myTest_Sensor->alarmLevelsPointer)->linkedNode->nodeValue = 6;
	TEST_ASSERT_EQUAL(6,  Sensor_getUpperSecondaryAlarmLevel(myTest_Sensor));
}


/****  Set/Get AlarmState  ****************/
/**/
TEST(sensor, Sensor_getAlarmState_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(ALARM_TYPE_UNKNOWN,  Sensor_getAlarmState(myTest_Sensor) );
}

TEST(sensor, Sensor_getAlarmState_returns_specificValue)
{
	myTest_Sensor->alarmState = 6;
	TEST_ASSERT_EQUAL(6,  Sensor_getAlarmState(myTest_Sensor) );
}

TEST(sensor, Sensor_setAlarmState_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(4,  Sensor_setAlarmState(myTest_Sensor, 4));
}

TEST(sensor, Sensor_setAlarmState_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(ALARM_TYPE_UNKNOWN,  Sensor_setAlarmState(NULL, 5));
}

TEST(sensor, Sensor_setAlarmState_returnsSpecificValueOnSuccess)
{
	TEST_ASSERT_EQUAL(6,  Sensor_setAlarmState(myTest_Sensor, 6));
}

TEST(sensor, Sensor_setAlarmState_canSetSpecificValue)
{
	Sensor_setAlarmState(myTest_Sensor, 7);
	TEST_ASSERT_EQUAL(7,  myTest_Sensor->alarmState);
}

/****  Set/Get NormalState  ****************/
/**/
TEST(sensor, Sensor_getNormalState_returns_UnknownOnCreate)
{
	TEST_ASSERT_EQUAL(ALARM_TYPE_UNKNOWN,  Sensor_getNormalState(myTest_Sensor) );
}

TEST(sensor, Sensor_getNormalState_returns_specificValue)
{
	myTest_Sensor->normalState = 6;
	TEST_ASSERT_EQUAL(6,  Sensor_getNormalState(myTest_Sensor) );
}

TEST(sensor, Sensor_setNormalState_returnsSpecificValue)
{
	TEST_ASSERT_EQUAL(4,  Sensor_setNormalState(myTest_Sensor, 4));
}

TEST(sensor, Sensor_setNormalState_returnsUnknownOnNullPtr)
{
	TEST_ASSERT_EQUAL(ALARM_TYPE_UNKNOWN,  Sensor_setNormalState(NULL, 5));
}

TEST(sensor, Sensor_setNormalState_returnsSpecificValueOnSuccess)
{
	TEST_ASSERT_EQUAL(6,  Sensor_setNormalState(myTest_Sensor, 6));
}

TEST(sensor, Sensor_setNormalState_canSetSpecificValue)
{
	Sensor_setNormalState(myTest_Sensor, 7);
	TEST_ASSERT_EQUAL(7,  myTest_Sensor->normalState);
}

/****  check alarm state  ****************/

TEST(sensor, Sensor_checkAlarms_detects_Below)
{
	//printf("TEST: Sensor_checkAlarms_detects_Below\n");
	struct Node * localFinalDataPtr = Sensor_getFinalDataPointer(myTest_Sensor);
	localFinalDataPtr->nodeValue = 9;
	Sensor_setLowerPrimaryAlarmLevel(myTest_Sensor, 100);
	Sensor_setNormalState(myTest_Sensor, ALARM_BELOW); //  <<--
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TRIGGERED,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 100;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_NONE,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 101;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_NONE,  Sensor_getAlarmState(myTest_Sensor));
}

TEST(sensor, Sensor_checkAlarms_detects_BelowEqual)
{
	struct Node * localFinalDataPtr = Sensor_getFinalDataPointer(myTest_Sensor);
	localFinalDataPtr->nodeValue = 99;
	Sensor_setLowerPrimaryAlarmLevel(myTest_Sensor, 100);
	Sensor_setNormalState(myTest_Sensor, ALARM_BELOW_OR_EQUAL); //  <<--
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TRIGGERED,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 100;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TRIGGERED,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 101;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_NONE,  Sensor_getAlarmState(myTest_Sensor));
}

TEST(sensor, Sensor_checkAlarms_detects_Equal)
{
	struct Node * localFinalDataPtr = Sensor_getFinalDataPointer(myTest_Sensor);
	localFinalDataPtr->nodeValue = 100;
	Sensor_setLowerPrimaryAlarmLevel(myTest_Sensor, 100);
	Sensor_setNormalState(myTest_Sensor, ALARM_EQUAL); //  <<--
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TRIGGERED,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 99;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_NONE,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 101;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_NONE,  Sensor_getAlarmState(myTest_Sensor));
}

TEST(sensor, Sensor_checkAlarms_detects_AboveEqual)
{
	//printf("\nTEST: Sensor_checkAlarms_detects_AboveEqual\n");
	struct Node * localFinalDataPtr = Sensor_getFinalDataPointer(myTest_Sensor);
	localFinalDataPtr->nodeValue = 101;
	Sensor_setLowerPrimaryAlarmLevel(myTest_Sensor, 100);
	Sensor_setNormalState(myTest_Sensor, ALARM_ABOVE_OR_EQUAL); //  <<--
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TRIGGERED,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 100;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TRIGGERED,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 99;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_NONE,  Sensor_getAlarmState(myTest_Sensor));
}

TEST(sensor, Sensor_checkAlarms_detects_Above)
{
	struct Node * localFinalDataPtr = Sensor_getFinalDataPointer(myTest_Sensor);
	Sensor_setLowerPrimaryAlarmLevel(myTest_Sensor, 100);
	Sensor_setNormalState(myTest_Sensor, ALARM_ABOVE); //  <<--

	localFinalDataPtr->nodeValue = 99;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_NONE,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 100;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_NONE,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 101;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TRIGGERED,  Sensor_getAlarmState(myTest_Sensor));
}

TEST(sensor, Sensor_checkAlarms_detects_Between)
{
	Sensor_setLowerPrimaryAlarmLevel(myTest_Sensor, 99);
	Sensor_setUpperSecondaryAlarmLevel(myTest_Sensor, 101);
	Sensor_setNormalState(myTest_Sensor, ALARM_BETWEEN); //  <<--
	struct Node * localFinalDataPtr = myTest_Sensor->finalDataPointer;

	localFinalDataPtr->nodeValue = 98;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_NONE,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 99;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TRIGGERED,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr = Sensor_getFinalDataPointer(myTest_Sensor);
	localFinalDataPtr->nodeValue = 100;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TRIGGERED,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 101;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TRIGGERED,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 102;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_NONE,  Sensor_getAlarmState(myTest_Sensor));
}

TEST(sensor, Sensor_checkAlarms_detects_Outside)
{
	Sensor_setLowerPrimaryAlarmLevel(myTest_Sensor, 99);
	Sensor_setUpperSecondaryAlarmLevel(myTest_Sensor, 101);
	Sensor_setNormalState(myTest_Sensor, ALARM_OUTSIDE); //  <<--
	struct Node * localFinalDataPtr = myTest_Sensor->finalDataPointer;

	localFinalDataPtr->nodeValue = 98;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TRIGGERED,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 99;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_NONE,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr = Sensor_getFinalDataPointer(myTest_Sensor);
	localFinalDataPtr->nodeValue = 100;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_NONE,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 101;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_NONE,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 102;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TRIGGERED,  Sensor_getAlarmState(myTest_Sensor));
}

TEST(sensor, Sensor_checkAlarms_Returns_UnknownOnMissingThresholds)
{
	//Sensor_setLowerPrimaryAlarmLevel(myTest_Sensor, 99);
	//Sensor_setUpperSecondaryAlarmLevel(myTest_Sensor, 101);
	Sensor_setNormalState(myTest_Sensor, ALARM_BETWEEN); //  <<--
	struct Node * localFinalDataPtr = myTest_Sensor->finalDataPointer;

	localFinalDataPtr->nodeValue = 98;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TYPE_UNKNOWN,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 99;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TYPE_UNKNOWN,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr = Sensor_getFinalDataPointer(myTest_Sensor);
	localFinalDataPtr->nodeValue = 100;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TYPE_UNKNOWN,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 101;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TYPE_UNKNOWN,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 102;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TYPE_UNKNOWN,  Sensor_getAlarmState(myTest_Sensor));
}

TEST(sensor, Sensor_checkAlarms_Returns_UnknownOnMissingUpperThreshold)
{
	Sensor_setLowerPrimaryAlarmLevel(myTest_Sensor, 99);
	//Sensor_setUpperSecondaryAlarmLevel(myTest_Sensor, 101);
	Sensor_setNormalState(myTest_Sensor, ALARM_BETWEEN); //  <<--
	struct Node * localFinalDataPtr = myTest_Sensor->finalDataPointer;

	localFinalDataPtr->nodeValue = 98;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TYPE_UNKNOWN,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 99;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TYPE_UNKNOWN,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr = Sensor_getFinalDataPointer(myTest_Sensor);
	localFinalDataPtr->nodeValue = 100;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TYPE_UNKNOWN,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 101;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TYPE_UNKNOWN,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 102;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TYPE_UNKNOWN,  Sensor_getAlarmState(myTest_Sensor));
}

TEST(sensor, Sensor_checkAlarms_Returns_UnknownOnMissingLowerThreshold)
{
	//Sensor_setLowerPrimaryAlarmLevel(myTest_Sensor, 99);
	Sensor_setUpperSecondaryAlarmLevel(myTest_Sensor, 101);
	Sensor_setNormalState(myTest_Sensor, ALARM_BETWEEN); //  <<--
	struct Node * localFinalDataPtr = myTest_Sensor->finalDataPointer;

	localFinalDataPtr->nodeValue = 98;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TYPE_UNKNOWN,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 99;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TYPE_UNKNOWN,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr = Sensor_getFinalDataPointer(myTest_Sensor);
	localFinalDataPtr->nodeValue = 100;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TYPE_UNKNOWN,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 101;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TYPE_UNKNOWN,  Sensor_getAlarmState(myTest_Sensor));

	localFinalDataPtr->nodeValue = 102;
	Sensor_setSensorState(myTest_Sensor, SENSOR_CHECK_ALARMS);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TYPE_UNKNOWN,  Sensor_getAlarmState(myTest_Sensor));
}

//****  Sensor_reportReady  ****************
/**/
TEST(sensor, Sensor_reportReady_returns_NotReadyOnCreate)
{
	TEST_ASSERT_EQUAL(SENSOR_REPORT_NOT_READY, Sensor_reportReady(myTest_Sensor));
}

TEST(sensor, Sensor_reportReady_returns_ReadyOnValidReport)
{
	Sensor_setSensorState(myTest_Sensor, SENSOR_REPORT);
	TEST_ASSERT_EQUAL(SENSOR_REPORT_IS_READY, Sensor_reportReady(myTest_Sensor));
}

TEST(sensor, Sensor_reportReady_returnsNotReadyOnNullPtr)
{
	TEST_ASSERT_EQUAL(SENSOR_REPORT_NOT_READY, Sensor_reportReady(NULL));
}

//****  copy/Sensor_copy  ****************

TEST(sensor, copy_returnsSelfOnSuccess)
{
	struct Sensor * masterSensor = new(Sensor);
	Sensor_setPowerUpDelayTicks(masterSensor, 1);
	TEST_ASSERT_EQUAL_PTR(myTest_Sensor, copy(myTest_Sensor, masterSensor));
	masterSensor = safeDelete(masterSensor);
}

TEST(sensor, copy_AllItemsCopiedToSelf)
{
	// NOTE: sensorState, and pointers are unique for every sensor
	struct Sensor * masterSensor = new(Sensor);
	Sensor_setSensorState          (masterSensor, SENSOR_ENABLE_POWER );
	Sensor_setPowerUpDelayTicks    (masterSensor, 4 );
	Sensor_setMeasurementDelayTicks(masterSensor, 5 );
	//Sensor_setRawDataPointer       (masterSensor, (void *)6 );
	//Sensor_setFinalDataPointer     (masterSensor, (void *)7 );
	//Sensor_setAlarmLevelsPointer   (masterSensor, (void *)8 );
	Sensor_setAlarmState           (masterSensor, ALARM_ABOVE );
	Sensor_setNormalState          (masterSensor, ALARM_BETWEEN );
	Sensor_setOnReportReady_cb     (masterSensor, Sensor_test_general_cb );
	Sensor_setOnAlarmTriggered_cb  (masterSensor, Sensor_test_general_cb2 );

	copy(myTest_Sensor, masterSensor);

	// NOTE: sensorState, and pointers are unique for every sensor
	//TEST_ASSERT_EQUAL(SENSOR_ENABLE_POWER, myTest_Sensor->sensorState);
	TEST_ASSERT_EQUAL(4,                   myTest_Sensor->powerUpDelayTicks);
	TEST_ASSERT_EQUAL(5,                   myTest_Sensor->measurementDelayTicks);
	//TEST_ASSERT_EQUAL_PTR((void *)6,           myTest_Sensor->rawDataPointer);
	//TEST_ASSERT_EQUAL_PTR((void *)7,           myTest_Sensor->finalDataPointer);
	//TEST_ASSERT_EQUAL_PTR((void *)8,           myTest_Sensor->alarmLevelsPointer);
	TEST_ASSERT_EQUAL(ALARM_ABOVE,         myTest_Sensor->alarmState);
	TEST_ASSERT_EQUAL(ALARM_BETWEEN,       myTest_Sensor->normalState);
	TEST_ASSERT_EQUAL_PTR(Sensor_test_general_cb,  myTest_Sensor->Sensor_onReportReady_cb);
	TEST_ASSERT_EQUAL_PTR(Sensor_test_general_cb2, myTest_Sensor->Sensor_onAlarmTriggered_cb);

	masterSensor = safeDelete(masterSensor);
}

TEST(sensor, copy_returnsNullOnNullSelf)
{
	struct Sensor * masterSensor = new(Sensor);
	Sensor_setPowerUpDelayTicks(masterSensor, 1);
	TEST_ASSERT_EQUAL_PTR(NULL, copy(NULL, masterSensor)  );
	masterSensor = safeDelete(masterSensor);
}
/**/
TEST(sensor, copy_returnsNullOnNullMaster)
{
	struct Sensor * masterSensor = new(Sensor);
	Sensor_setPowerUpDelayTicks(masterSensor, 1);
	TEST_ASSERT_EQUAL_PTR(NULL, copy(myTest_Sensor, NULL) );
	masterSensor = safeDelete(masterSensor);
}

//****  equal/Sensor_equal  ********************
/**/
TEST(sensor, myTest_Sensor_IsEqualTo_myTest_Sensor)
{
	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_Sensor, myTest_Sensor));
}

TEST(sensor, equal_UnequalSensorStateReturn_Unequal)
{
	struct Sensor * masterSensor = new(Sensor);
	Sensor_setPowerUpDelayTicks(masterSensor, 1);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Sensor, masterSensor) );
	masterSensor = safeDelete(masterSensor);
}

TEST(sensor, equal_UnequalResetDelayTicksReturn_Unequal)
{
	struct Sensor * masterSensor = new(Sensor);
	Sensor_setAlignConfigDelayTicks(masterSensor, 2);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Sensor, masterSensor) );
	masterSensor = safeDelete(masterSensor);
}

TEST(sensor, equal_UnequalMeasurementDelayTicksReturn_Unequal)
{
	struct Sensor * masterSensor = new(Sensor);
	Sensor_setMeasurementDelayTicks (masterSensor, 3);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Sensor, masterSensor) );
	masterSensor = safeDelete(masterSensor);
}

TEST(sensor, equal_UnequalAlarmStateReturn_Unequal)
{
	struct Sensor * masterSensor = new(Sensor);
	Sensor_setAlarmState  (masterSensor, ALARM_ABOVE);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Sensor, masterSensor) );
	masterSensor = safeDelete(masterSensor);
}

TEST(sensor, equal_UnequalNormalStateReturn_Unequal)
{
	struct Sensor * masterSensor = new(Sensor);
	Sensor_setNormalState(masterSensor, ALARM_OUTSIDE);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Sensor, masterSensor) );
	masterSensor = safeDelete(masterSensor);
}

TEST(sensor, equal_UnequalNormalReportReadyCB_Unequal)
{
	struct Sensor * masterSensor = new(Sensor);
	Sensor_setOnReportReady_cb(masterSensor, Sensor_test_general_cb);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Sensor, masterSensor) );
	masterSensor = safeDelete(masterSensor);
}

TEST(sensor, equal_NullReturns_Null)
{
	struct Sensor * masterSensor = new(Sensor);
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_Sensor, NULL));
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(NULL, masterSensor ));
	masterSensor = safeDelete(masterSensor);
}

TEST(sensor, equal_CopiedSensorReturnsEqual)
{
	struct Sensor * masterSensor = new(Sensor);
	//Sensor_setSensorState(masterSensor, BUTTON_STATE_UNKNOWN);
	//Sensor_setPressCount(masterSensor, 1);
	//Sensor_setPressType (masterSensor, BUTTON_PRESS_TYPE_UNKNOWN);
	//Sensor_setPressTime  (masterSensor, 2);
	//Sensor_setReleaseTime(masterSensor, 3);
	copy(myTest_Sensor, masterSensor);
	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_Sensor, masterSensor));
	masterSensor = safeDelete(masterSensor);
}


/***********  onReportReady callback  **************/
/**/
TEST(sensor, getSensor_onReportReady_cb_returns_emptyReportReadyCallback_onCreate)
{
	TEST_ASSERT_EQUAL(Sensor_emptyReportReadyCallback,
			                           Sensor_getOnReportReady_cb(myTest_Sensor));
}

TEST(sensor, getSensor_onReportReady_cb_returns_new_cb)
{
	TEST_ASSERT_EQUAL_PTR(Sensor_test_general_cb,
			Sensor_setOnReportReady_cb(myTest_Sensor, Sensor_test_general_cb));
}

TEST(sensor, setSensor_onReportReady_cb_sets_new_cb)
{
	Sensor_setOnReportReady_cb(myTest_Sensor, Sensor_test_general_cb);
	TEST_ASSERT_EQUAL(Sensor_test_general_cb, myTest_Sensor->Sensor_onReportReady_cb);
}

TEST(sensor, Sensor_reportReady_firesDesignatedCallback)
{
	sensor_test_cb_count = 0;
	Sensor_setOnReportReady_cb(myTest_Sensor, Sensor_test_general_cb);
	myTest_Sensor->Sensor_onReportReady_cb(myTest_Sensor);
	TEST_ASSERT_EQUAL(1, sensor_test_cb_count);
}

/***********  onReportReady callback  **************/
/**/
TEST(sensor, getSensor_onAlarmTriggered_cb_returns_emptyReportReadyCallback_onCreate)
{
	TEST_ASSERT_EQUAL(Sensor_emptyAlarmTriggeredCallback,
			                           Sensor_getOnAlarmTriggered_cb(myTest_Sensor));
}

TEST(sensor, getSensor_onAlarmTriggered_cb_returns_new_cb)
{
	TEST_ASSERT_EQUAL_PTR(Sensor_test_general_cb,
			Sensor_setOnAlarmTriggered_cb(myTest_Sensor, Sensor_test_general_cb));
}

TEST(sensor, setSensor_onAlarmTriggered_cb_sets_new_cb)
{
	Sensor_setOnAlarmTriggered_cb(myTest_Sensor, Sensor_test_general_cb);
	TEST_ASSERT_EQUAL(Sensor_test_general_cb, myTest_Sensor->Sensor_onAlarmTriggered_cb);
}

TEST(sensor, Sensor_AlarmTriggered_firesDesignatedCallback)
{
	sensor_test_cb_count = 0;
	Sensor_setOnAlarmTriggered_cb(myTest_Sensor, Sensor_test_general_cb);
	myTest_Sensor->Sensor_onAlarmTriggered_cb(myTest_Sensor);
	TEST_ASSERT_EQUAL(1, sensor_test_cb_count);
}


/***********  config  **************/
/*
TEST(sensor, Config_copiesState)
{
	struct Sensor * masterSensor = new(Sensor);
	Sensor_setPressType(masterSensor, BUTTON_SHORT_PRESS);
	Sensor_setPressTime(masterSensor, 7);
	config(myTest_Sensor, masterSensor);
	TEST_ASSERT_EQUAL(BUTTON_SHORT_PRESS, Sensor_getPressType(myTest_Sensor));
	TEST_ASSERT_EQUAL(7,                  Sensor_getPressTime(myTest_Sensor));
}
*/

/***********  init  **************/
/**/
TEST(sensor, CanCall_Sensor_Init_MulitpleTimes)
{
	const void * originalSensor = Sensor;
	const void * originalSensorClass = SensorClass;
	Sensor_init();
	Sensor_init();
	Sensor_init();

	// Verify that the class pointers are unchanged
	TEST_ASSERT_EQUAL_PTR(Sensor, originalSensor);
	TEST_ASSERT_EQUAL_PTR(SensorClass, originalSensorClass);

	char outputFilename[] = "sensor_puto.txt";
	FILE * fp = fopen(outputFilename, "a");
	puto(myTest_Sensor, fp);
	fclose(fp);
}

TEST(sensor, memory_leak)
{
	// new structure location will move based on other processes
	// create and destroy a large number of objects
	// then look for the base address within an additional 10 new objects

	// get original object location
	struct Event * newItem = new(Sensor);
	struct Event * originalPointer = newItem;
	delete(newItem);
	//printf("originalPointer: %p, i: %i\n", originalPointer, i);

	// create and destroy a large number of objects to cause a memory leak
	for (i = 0; i < 1000; i++) {
		newItem = new(Sensor);
		//printf("newItem: %p, i: %i\n", newItem, i);
		delete(newItem);
	}

	// check for original object location within a small number of additional objects
	int originalPtrCount = 0;
	for (i = 0; i < 40; i++) {
		newItem = new(Sensor);
		//printf("newItem: %p, i: %i\n", newItem, i);
		if (originalPointer == newItem) { originalPtrCount++; }
		delete(newItem);
	}
	//printf("\nSensor-memory-leak-test, originalPtrCount: %i, out of %i trials\n", originalPtrCount, 40);
	TEST_ASSERT_TRUE(originalPtrCount);
}

/***********  API  **************/
/**/

TEST(sensor, Sensor_start_setsAsyncFlagToStartRequest)
{
	Sensor_start(myTest_Sensor);
	TEST_ASSERT_EQUAL(SENSOR_ASYNC_START_REQUEST, myTest_Sensor->asyncFlag);
}

TEST(sensor, Sensor_start_setsAsyncFlagToStartDoneAfterUpdate)
{
	Sensor_start(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(SENSOR_ASYNC_START_DONE, myTest_Sensor->asyncFlag);
}

TEST(sensor, Sensor_start_doesNotModifiyAsyncFlagIfAlreadyStarted)
{
	Sensor_start(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	sensorAsyncFlag_t currentAsyncFlag = myTest_Sensor->asyncFlag;

	Sensor_start(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(currentAsyncFlag, myTest_Sensor->asyncFlag);
}

TEST(sensor, Sensor_measure_triggeresStartWhenNotStarted)
{
	Sensor_measureAndProcess(myTest_Sensor);
	TEST_ASSERT_EQUAL(SENSOR_STATE_UNKNOWN, myTest_Sensor->sensorState);
	Sensor_update(myTest_Sensor);
	// NOTE:  main state machine transitions to un-powered idle after defaults
	TEST_ASSERT_EQUAL(SENSOR_UNPOWERED_IDLE, myTest_Sensor->sensorState);
}

TEST(sensor, Sensor_measure_triggeresMeasureWhenReadyIdle)
{
	myTest_Sensor->sensorState = SENSOR_READY_IDLE;
	myTest_Sensor->measurementDelayTicks = 5;
	Sensor_measureAndProcess(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(SENSOR_WAITING_MEASUREMENT, myTest_Sensor->sensorState);
}

TEST(sensor, Sensor_measure_triggeresMeasureWhenReport)
{
	myTest_Sensor->sensorState = SENSOR_REPORT;
	myTest_Sensor->measurementDelayTicks = 5;
	Sensor_measureAndProcess(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(SENSOR_WAITING_MEASUREMENT, myTest_Sensor->sensorState);
}

TEST(sensor, Sensor_measure_secondMeasureDoesNotChangeMainSM)
{
	myTest_Sensor->sensorState = SENSOR_WAITING_MEASUREMENT;
	Sensor_setAsyncFlag(myTest_Sensor, SENSOR_ASYNC_MEASURE_IN_PROCESS);

	Sensor_measureAndProcess(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(SENSOR_WAITING_MEASUREMENT, myTest_Sensor->sensorState);
}

TEST(sensor, Sensor_start_setsAlarmTypeToUnknown)
{
	Sensor_start(myTest_Sensor);
	TEST_ASSERT_EQUAL(ALARM_TYPE_UNKNOWN, myTest_Sensor->alarmState);
}

TEST(sensor, Sensor_start_setsNormalTypeToUnknown)
{
	Sensor_start(myTest_Sensor);
	TEST_ASSERT_EQUAL_PTR(ALARM_TYPE_UNKNOWN, myTest_Sensor->normalState);
}

TEST(sensor, Sensor_start_stateEndsInIdle)
{
	Sensor_start(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL_PTR(SENSOR_UNPOWERED_IDLE, myTest_Sensor->sensorState);
}

TEST(sensor, Sensor_start_returnsSelfOnSuccess)
{
	TEST_ASSERT_EQUAL_PTR(myTest_Sensor, Sensor_start(myTest_Sensor));
}

TEST(sensor, Sensor_start_returnsNullOnNullPtr)
{
	TEST_ASSERT_EQUAL_PTR(NULL, Sensor_start(NULL));
}

//++++++++++++++++
/**/
TEST(sensor, Sensor_armDelayedCallback_addsTask)
{
	Sensor_armDelayedCallback(myTest_Sensor, (void *)6, 7 );

	// tests below depend on knowledge of the scheduler implementation
	TEST_ASSERT_EQUAL_PTR(NULL,                     testTASKS_sensors[0].pTask);
	TEST_ASSERT_EQUAL_PTR((void *)6,                testTASKS_sensors[0].pTask_wPTR);
	TEST_ASSERT_EQUAL_PTR(myTest_Sensor,            testTASKS_sensors[0].objectPointer);
	TEST_ASSERT_EQUAL(SCHEDULER_TASK_TYPE_VOID_PTR, testTASKS_sensors[0].taskType);
	TEST_ASSERT_EQUAL(7,                            testTASKS_sensors[0].delay);
	TEST_ASSERT_EQUAL(0,                            testTASKS_sensors[0].period);
	TEST_ASSERT_EQUAL(0,                            testTASKS_sensors[0].runMe);
}

TEST(sensor, Sensor_armDelayedCallback_returnsCallBackFnOnSuccess)
{
	myTest_Sensor->powerUpDelayTicks = 100;
	TEST_ASSERT_EQUAL_PTR(Sensor_postEnablePower,
			                   Sensor_armDelayedCallback(myTest_Sensor,
								                         (sensor_cb_fnct)Sensor_postEnablePower,
								                         myTest_Sensor->powerUpDelayTicks) );
}

TEST(sensor, Sensor_armDelayedCallback_returnsNullOnNullSensorPTR)
{
	myTest_Sensor->powerUpDelayTicks = 100;
	TEST_ASSERT_EQUAL_PTR(Sensor_postEnablePower,
			                   Sensor_armDelayedCallback(NULL,
			                		   	   	   	   	     (sensor_cb_fnct)Sensor_postEnablePower,
								                         myTest_Sensor->powerUpDelayTicks) );
}

TEST(sensor, Sensor_armDelayedCallback_returnsNullOnNullCallback)
{
	myTest_Sensor->powerUpDelayTicks = 100;
	TEST_ASSERT_EQUAL_PTR(NULL, Sensor_armDelayedCallback(myTest_Sensor,
								                          NULL,
								                          myTest_Sensor->powerUpDelayTicks) );
}

TEST(sensor, Sensor_armDelayedCallback_returnsNullOnTickLessThanOne)
{
	myTest_Sensor->powerUpDelayTicks = -1;
	TEST_ASSERT_EQUAL_PTR(NULL, Sensor_armDelayedCallback(myTest_Sensor,
														(sensor_cb_fnct)Sensor_postEnablePower,
								                          myTest_Sensor->powerUpDelayTicks) );
}

//++++++++++++++++
/**/

TEST(sensor, Sensor_measureAndProcess_sendsAllCommands)
{

}

TEST(sensor, Sensor_measureAndProcess_armsSchedulerCallbacks)
{
	// NOTE:  sequences the state machine through all states
	// if errors emerge, try adding additional Sensor_updateState() calls
	TEST_ASSERT_EQUAL(SENSOR_STATE_UNKNOWN, myTest_Sensor->sensorState);
	Sensor_start(myTest_Sensor);  // loads selected defaults
	Sensor_update(myTest_Sensor);

	// can update the delay ticks AFTER the selected defaults are loaded above
	Sensor_setPowerUpDelayTicks    (myTest_Sensor, 1); // >0 triggers callback wait
	Sensor_setAlignConfigDelayTicks(myTest_Sensor, 1); // >0 triggers callback wait
	Sensor_setMeasurementDelayTicks(myTest_Sensor, 1); // >0 triggers callback wait

	// Sensor_update() no longer included in start(), requires separate update call
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(SENSOR_UNPOWERED_IDLE, myTest_Sensor->sensorState);
	Sensor_update(myTest_Sensor);  // safety update
	TEST_ASSERT_EQUAL(SENSOR_UNPOWERED_IDLE, myTest_Sensor->sensorState);

	Sensor_measureAndProcess(myTest_Sensor);  // <<<---  main state machine trigger
	Sensor_update(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(SENSOR_WAITING_POWER, myTest_Sensor->sensorState);

	// tests below depend on knowledge of the scheduler implementation
	// verify that the post power up callback was registered
	TEST_ASSERT_EQUAL_PTR(NULL,                         testTASKS_sensors[0].pTask);
	TEST_ASSERT_EQUAL_PTR(Sensor_postEnablePower,       testTASKS_sensors[0].pTask_wPTR);
	TEST_ASSERT_EQUAL_PTR(myTest_Sensor,                testTASKS_sensors[0].objectPointer);
	TEST_ASSERT_EQUAL(SCHEDULER_TASK_TYPE_VOID_PTR,     testTASKS_sensors[0].taskType);
	TEST_ASSERT_EQUAL(myTest_Sensor->powerUpDelayTicks, testTASKS_sensors[0].delay);
	TEST_ASSERT_EQUAL(0,                                testTASKS_sensors[0].period);
	TEST_ASSERT_EQUAL(0,                                testTASKS_sensors[0].runMe);

	Sensor_postEnablePower(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(SENSOR_WAITING_CONFIG, myTest_Sensor->sensorState);

	Sensor_postAlignAndConfig(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(SENSOR_WAITING_MEASUREMENT, myTest_Sensor->sensorState);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(SENSOR_WAITING_MEASUREMENT, myTest_Sensor->sensorState);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(SENSOR_WAITING_MEASUREMENT, myTest_Sensor->sensorState);

	myTest_Sensor->sensorState = SENSOR_GET_RAW_DATA;
	//TEST_ASSERT_EQUAL(NULL, Sensor_getRawDataPointer(myTest_Sensor));

	// NOTE that the default sensor raw data value is defined as 99 for testing
	((struct Node *)myTest_Sensor->rawDataPointer)->nodeValue = 99;
	Sensor_storeRawData(myTest_Sensor);

	Sensor_update(myTest_Sensor);

	Sensor_postStartMeasurement(myTest_Sensor);

	Sensor_update(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	Sensor_update(myTest_Sensor);

	Sensor_postStartMeasurement(myTest_Sensor);

	Sensor_update(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	Sensor_update(myTest_Sensor);  // safety update
	Sensor_update(myTest_Sensor);  // safety update
	Sensor_update(myTest_Sensor);  // safety update
	Sensor_update(myTest_Sensor);  // safety update
	Sensor_update(myTest_Sensor);  // safety update
	Sensor_update(myTest_Sensor);  // safety update
	TEST_ASSERT_EQUAL(SENSOR_REPORT, myTest_Sensor->sensorState);

	// tests below depend on knowledge of the scheduler implementation
	// verify that the post align and config callback was registered
	TEST_ASSERT_EQUAL_PTR(NULL,                         testTASKS_sensors[1].pTask);
	TEST_ASSERT_EQUAL_PTR(Sensor_postAlignAndConfig,    testTASKS_sensors[1].pTask_wPTR);
	TEST_ASSERT_EQUAL_PTR(myTest_Sensor,                testTASKS_sensors[1].objectPointer);
	TEST_ASSERT_EQUAL(SCHEDULER_TASK_TYPE_VOID_PTR,     testTASKS_sensors[1].taskType);
	TEST_ASSERT_EQUAL(myTest_Sensor->resetDelayTicks,   testTASKS_sensors[1].delay);
	TEST_ASSERT_EQUAL(0,                                testTASKS_sensors[1].period);
	TEST_ASSERT_EQUAL(0,                                testTASKS_sensors[1].runMe);

	// tests below depend on knowledge of the scheduler implementation
	// verify that the post measurement callback was registered
	TEST_ASSERT_EQUAL_PTR(NULL,                         testTASKS_sensors[2].pTask);
	TEST_ASSERT_EQUAL_PTR(Sensor_postStartMeasurement,  testTASKS_sensors[2].pTask_wPTR);
	TEST_ASSERT_EQUAL_PTR(myTest_Sensor,                testTASKS_sensors[2].objectPointer);
	TEST_ASSERT_EQUAL(SCHEDULER_TASK_TYPE_VOID_PTR,     testTASKS_sensors[2].taskType);
	TEST_ASSERT_EQUAL(myTest_Sensor->measurementDelayTicks, testTASKS_sensors[2].delay);
	TEST_ASSERT_EQUAL(0,                                testTASKS_sensors[2].period);
	TEST_ASSERT_EQUAL(0,                                testTASKS_sensors[2].runMe);
/**/
}


TEST(sensor, Sensor_measureAndProcess_stateEndsInReport)
{
	//printf("TEST:Sensor_measureAndProcess_stateEndsInReport, line: %i\n", __LINE__);

	// NOTE:  sequences the state machine through all states
	// if errors emerge, try adding additional Sensor_updateState() calls
	Sensor_start(myTest_Sensor);
	Sensor_update(myTest_Sensor);  // safety update
	Sensor_update(myTest_Sensor);  // safety update

	Sensor_setOnReportReady_cb(myTest_Sensor, Sensor_test_general_cb);
	Sensor_setOnAlarmTriggered_cb(myTest_Sensor, Sensor_test_general_cb2);
	Sensor_setLowerPrimaryAlarmLevel  (myTest_Sensor,  90);
	Sensor_setUpperSecondaryAlarmLevel(myTest_Sensor, 110);
	Sensor_setNormalState(myTest_Sensor, ALARM_BETWEEN);

	// set the raw data value to 100 for testing, base code does not access an actual sensor
	struct Node * localRawDataPtr = Sensor_getRawDataPointer(myTest_Sensor);
	setValue(localRawDataPtr, 100 );


	// on first pass, Sensor_measureAndProcess() will drop back
	// to SENSOR_START_DATA_DEFAULTS state
	Sensor_measureAndProcess(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	Sensor_update(myTest_Sensor);

	Sensor_postEnablePower(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	Sensor_update(myTest_Sensor);

	Sensor_postAlignAndConfig(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	Sensor_update(myTest_Sensor);

	Sensor_postStartMeasurement(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	Sensor_update(myTest_Sensor);

	Sensor_update(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	Sensor_update(myTest_Sensor);  // Safety update
	Sensor_update(myTest_Sensor);
	Sensor_update(myTest_Sensor);

TEST_ASSERT_EQUAL(100, getValue(localRawDataPtr) );

	TEST_ASSERT_EQUAL(1, Sensor_reportReady(myTest_Sensor) );

	struct Node * localFinalDataPtr = Sensor_getFinalDataPointer(myTest_Sensor);
	TEST_ASSERT_EQUAL(100, getValue(localFinalDataPtr) );
	TEST_ASSERT_EQUAL(100, ((struct Node *)(myTest_Sensor->finalDataPointer))->nodeValue );

	TEST_ASSERT_EQUAL_PTR(Sensor_test_general_cb, myTest_Sensor->Sensor_onReportReady_cb);

	TEST_ASSERT_EQUAL(ALARM_BETWEEN, Sensor_getNormalState(myTest_Sensor));
	TEST_ASSERT_EQUAL_PTR(Sensor_test_general_cb2, myTest_Sensor->Sensor_onAlarmTriggered_cb);
	TEST_ASSERT_TRUE(sensor_test_cb_count  >= 1);
	TEST_ASSERT_TRUE(sensor_test_cb_count2 >= 1);
}

TEST(sensor, Sensor_measureAndProcess_returnsSelfOnSuccess)
{
	TEST_ASSERT_EQUAL(myTest_Sensor, Sensor_measureAndProcess(myTest_Sensor));
}

TEST(sensor, Sensor_measureAndProcess_returnsNullOnNullPtr)
{
	TEST_ASSERT_EQUAL(NULL, Sensor_measureAndProcess(NULL));
}

TEST(sensor, Sensor_stop_endsInUnpoweredIdle)
{
	Sensor_start(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	Sensor_stopAndRemovePower(myTest_Sensor);
	TEST_ASSERT_EQUAL(SENSOR_UNPOWERED_IDLE, myTest_Sensor->sensorState);
}

TEST(sensor, Sensor_stop_sendsPowerDownCommands)
{
	Sensor_start(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	Sensor_stopAndRemovePower(myTest_Sensor);
	Sensor_update(myTest_Sensor);
	TEST_ASSERT_EQUAL(SENSOR_UNPOWERED_IDLE, myTest_Sensor->sensorState);
}

TEST(sensor, Sensor_resetMiniState_setsToStateZero)
{
	Sensor_resetMiniState(myTest_Sensor);
	TEST_ASSERT_EQUAL(SENSOR_MINI_STATE_START_0, Sensor_getMiniState(myTest_Sensor));
}

TEST(sensor, Sensor_irincrementMiniState_movesToNextState)
{
	Sensor_setMiniState(myTest_Sensor, SENSOR_MINI_STATE_3);
	Sensor_incrementMiniState(myTest_Sensor);
	TEST_ASSERT_EQUAL(SENSOR_MINI_STATE_4, Sensor_getMiniState(myTest_Sensor));
}

//****  Support Methods  ****************

/**/
struct Sensor * Sensor_test_general_cb(struct Sensor * _self)
{
	sensor_test_cb_count++;
	//printf("  XXX Sensor_test_general_cb\n");
	return _self;
}

struct Sensor * Sensor_test_general_cb2(struct Sensor * _self)
{
	sensor_test_cb_count2++;
	//printf("  XXX Sensor_test_general_cb2\n");
	return _self;
}

