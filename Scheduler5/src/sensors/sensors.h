/*
 * event-controller.h
 *
 *  Created on: Aug 15, 2015
 *      Author: alhansfo
 */

#ifndef SRC_SENSORS_SENSORS_H_
#define SRC_SENSORS_SENSORS_H_

/***********************************************/
/****** superclass and system includes  ********/

//#include "..\cross_compiler_defs.h"
#include "..\..\src\objects\objects.h"
#include "..\..\src\scheduler\scheduler.h"
#include "..\..\src\nodes\nodes.h"     // supports class registration
//#include "..\..\src\node_list\node-list.h"
//#include "..\..\src\times\times.h"     // supports class registration
//#include "..\..\src\events\events.h"     // supports class registration

/***********************************************/
/**** forward declarations and typedefs  *******/

extern const void * Sensor;
extern const void * SensorClass;

typedef enum sensorState_t {
	SENSOR_STATE_UNKNOWN = -1,
	SENSOR_START_DATA_DEFAULTS,
	SENSOR_UNPOWERED_IDLE,
	SENSOR_ENABLE_POWER,
	SENSOR_WAITING_POWER,
	SENSOR_ALIGN_CONFIG,
	SENSOR_WAITING_CONFIG,
	SENSOR_READY_IDLE,
	SENSOR_START_MEASUREMENT,
	SENSOR_WAITING_MEASUREMENT,
	SENSOR_GET_RAW_DATA,
	SENSOR_DISABLE_POWER,
	SENSOR_PROCESS_RAW_DATA,
	SENSOR_CHECK_ALARMS,
	SENSOR_REPORT
} sensorState_t;

typedef enum miniState_t {
	SENSOR_MINI_STATE_UNKNOWN = -1, SENSOR_MINI_STATE_START_0, SENSOR_MINI_STATE_1,
	 SENSOR_MINI_STATE_2, SENSOR_MINI_STATE_3, SENSOR_MINI_STATE_4, SENSOR_MINI_STATE_5,
	 SENSOR_MINI_STATE_6, SENSOR_MINI_STATE_7, SENSOR_MINI_STATE_8, SENSOR_MINI_STATE_9,
} miniState_t;

typedef enum alarmType_t {
	ALARM_TYPE_UNKNOWN = -1, ALARM_TRIGGERED, ALARM_NONE,
	ALARM_BELOW, ALARM_BELOW_OR_EQUAL, ALARM_EQUAL, ALARM_ABOVE, ALARM_ABOVE_OR_EQUAL,
	ALARM_BETWEEN, ALARM_OUTSIDE
} alarmType_t;

typedef enum sensorDelayTicks_t {
	SENSOR_DELAY_TICKS_UNKNOWN = -1, SENSOR_DELAY_TICKS_ZERO = 0
} sensorDelayTicks_t;

typedef enum sensorReportStatus_t {
	SENSOR_REPORT_NOT_READY = 0, SENSOR_REPORT_IS_READY = 1
} sensorReportStatus_t;

typedef struct Sensor * (* sensor_cb_fnct)(struct Sensor * _sensor);



/***********************************************/
/************ protected includes  **************/
/***** must be after externs and typedefs  *****/

#include "sensors-private.h"


/***********************************************/
/****** application interface functions  *******/

/*!
* @brief Generalized sensor manager based on a state machine design.
*
* Processing for various sensors shares a significant amount of commonality,
* even for sensors that use different communication channels.  This sensor
* manager attempts to abstract sensor control and processing for higher level
* program components.
*
* The nominal methods treat the sensor as a simple memory addressed device.
* Nominal reads and writes are executed directly by the base methods.
*
* Other sensor types should overload the base methods with appropriate code
* for the individual sensor.  Many sensors will require some form of hardware
* communication.  The IO manager will normally manage this communication.
* For example, I2C sensors will send specific read and write commands to the IO
* manager.  The IO manager will communicate with the I2C HAL.  Several I2C
* sensors can access the I2C hardware via the IO manager.
*
* Sensor_init() MUST be called before other Sensor_xxxx methods are called.
*
* Higher level program code creates one of more sensors using new().
*
* @code
* Sensor_init();
* struct Sensor * mySensorPointer = new(Sensor);
* Sensor_Start(mySensorPointer);
* Sensor_measureAndProcess(mySensorPointer);
* mySensorPointer = safeDelete(mySensorPointer);
* @endcode
*
*
*/
void Sensor_init(void);

// create a new sensor with new()
// ... struct Sensor mySensor = new(Sensor);

// sets the data members of a specific sensor to their defaults
// returns self on success, otherwise NULL
// callbacks and other individualized settings should be set AFTER start()
// callbacks require powerUp, reset and measurement delays greater than '0'
void * Sensor_start(void * _self);

/****************************************/
/*** interface manager for callbacks  ***/

// arms asynchronous callbacks from the scheduler
// requires .... #include "..\..\src\scheduler\scheduler.h"
sensor_cb_fnct Sensor_armDelayedCallback(void *         _self,
		                                 sensor_cb_fnct _callback,
										 int            _ticksOfDelay);

// required callbacks to be setup as tasks on the scheduler using above
// _ticksOfDelay setting depends on duration of the scheduler tick
void Sensor_postEnablePower(void * _self);
void Sensor_postAlignAndConfig(void * _self);
void Sensor_postStartMeasurement(void * _self);

// returns current callback function on key sensor state machine events
// can be used to create a callback chains that fire on reportReady or alarm
sensor_cb_fnct Sensor_setOnReportReady_cb(const void * _self, sensor_cb_fnct _cb);
sensor_cb_fnct Sensor_getOnReportReady_cb(const void * _self);
sensor_cb_fnct Sensor_setOnAlarmTriggered_cb(const void * _self, sensor_cb_fnct _cb);
sensor_cb_fnct Sensor_getOnAlarmTriggered_cb(const void * _self);

/*******************************************/
/*** Post sensor configuration controls  ***/

/*! Performs complete sensor power up and measurement cycle, processes data
 * and automatically disables power.
*/
void * Sensor_measureAndProcess(void * _self);

// power is automatically disabled if Sensor_disablePower() is populated
// use this manual method if Sensor_disablePower() method is left blank
void * Sensor_stopAndRemovePower(void * _self);


/************************************************************/
/*** overload methods used to implement a sensor handler  ***/

// add comment
void * Sensor_loadDefaults(void * _self);

// add comment
void * Sensor_enablePower(void * _self);

// add comment
void * Sensor_alignAndConfig(void * _self);

// add comment
void * Sensor_startMeasurement(void * _self);

void * Sensor_storeRawData(void * _self);

// add comment
void * Sensor_disablePower(void * _self);

// add comment
void * Sensor_storeRawData(void * _self);

// add comment
void * Sensor_processRawData(void * _self);

// add comment
void * Sensor_checkAlarms(void * _self);

/****************************************/
/******** I/O manager interface  ********/

// requires .... #include "..\..\src\i2c\i2c.h"
void * Sensor_writeDataToSensor(void * _self, void * _dataPointer);
void * Sensor_readDataFromSensor (void * _self, void * _dataPointer);


/******************************************/
/*** Sensor data member access methods  ***/

sensorState_t Sensor_getSensorState(const void  * _self);
sensorState_t Sensor_setSensorState(void * _self, sensorState_t _sensorState);

miniState_t Sensor_getMiniState(const void  * _self);
miniState_t Sensor_setMiniState(void * _self, miniState_t _miniState);

int Sensor_getEnablePowerDelayTicks(const void * _self);
int Sensor_setPowerUpDelayTicks(      void * _self, int _delayTicks);

int Sensor_getResetDelayTicks(const void * _self);
int Sensor_setResetDelayTicks(      void * _self, int _delayTicks);

int Sensor_getMeasurementDelayTicks(const void * _self);
int Sensor_setMeasurementDelayTicks(      void * _self, int _delayTicks);

void * Sensor_getRawDataPointer(const void * _self);
void * Sensor_setRawDataPointer(      void * _self, void * _rawDataPointer);

void * Sensor_getFinalDataPointer(const void * _self);
void * Sensor_setFinalDataPointer(void * _self, void * _finalDataPointer);

void * Sensor_getAlarmLevelsPointer(const void * _self);
void * Sensor_setAlarmLevelsPointer(void * _self, void * _alarmLevelsPointer);

value_t Sensor_getLowerPrimaryAlarmLevel(const void * _self);
void *  Sensor_setLowerPrimaryAlarmLevel(void * _self, value_t _value);

value_t Sensor_getUpperSecondaryAlarmLevel(const void * _self);
void *  Sensor_setUpperSecondaryAlarmLevel(void * _self, value_t _value);

alarmType_t Sensor_getAlarmState(const void * _self);
alarmType_t Sensor_setAlarmState(void * _self, alarmType_t _alarmState);

alarmType_t Sensor_getNormalState(const void * _self);
alarmType_t Sensor_setNormalState(void * _self, alarmType_t _normalState);

sensorReportStatus_t Sensor_reportReady(const void * _self);

void Sensor_transitionState(struct Sensor * _self, sensorState_t _state);

#endif /* SRC_BUTTON_BUTTON_H_ */
