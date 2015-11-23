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
	doesNot,
	SENSOR_WAITING_MEASUREMENT,
	SENSOR_GET_RAW_DATA,
	SENSOR_DISABLE_POWER,
	SENSOR_PROCESS_RAW_DATA,
	SENSOR_CHECK_ALARMS,
	SENSOR_REPORT
} sensorState_t;

typedef enum sensorAsyncFlag_t {
	SENSOR_ASYNC_FLAG_UNKNOWN = -1,
	SENSOR_ASYNC_START_REQUEST,
	SENSOR_ASYNC_START_DONE,
	SENSOR_ASYNC_MEASURE_REQUEST,
	SENSOR_ASYNC_MEASURE_IN_PROCESS,
	SENSOR_ASYNC_MEASURE_DONE
} sensorAsyncFlag_t;

typedef enum miniState_t {
	SENSOR_MINI_STATE_UNKNOWN = -1, SENSOR_MINI_STATE_START_0, SENSOR_MINI_STATE_1,
	SENSOR_MINI_STATE_2, SENSOR_MINI_STATE_3, SENSOR_MINI_STATE_4, SENSOR_MINI_STATE_5,
	SENSOR_MINI_STATE_6, SENSOR_MINI_STATE_7, SENSOR_MINI_STATE_8, SENSOR_MINI_STATE_9,
} miniState_t;

typedef enum sensorDelayTicks_t {
	SENSOR_DELAY_TICKS_UNKNOWN = -1, SENSOR_DELAY_TICKS_ZERO = 0
} sensorDelayTicks_t;

typedef enum alarmType_t {
	ALARM_TYPE_UNKNOWN = -1, ALARM_TRIGGERED, ALARM_NONE,
	ALARM_BELOW, ALARM_BELOW_OR_EQUAL, ALARM_EQUAL, ALARM_ABOVE, ALARM_ABOVE_OR_EQUAL,
	ALARM_BETWEEN, ALARM_OUTSIDE
} alarmType_t;

typedef enum sensorReportStatus_t {
	SENSOR_REPORT_NOT_READY = 0, SENSOR_REPORT_IS_READY = 1
} sensorReportStatus_t;

typedef struct Sensor * (* sensor_cb_fnct)(struct Sensor * _sensor);

/*! Maximum number of commands that can be pre-loaded into the command buffer.
 *  Commands are 8-bit char types.  Note that address registers that might be
 *  needed in other communications protocols like I2C also count in this total.
 */
#define MAX_COMMANDS 16
typedef char command_t;

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
* // Initialize Sensor classes structures
* Sensor_init();
*
* // create a new sensor
* struct Sensor * mySensorPointer = new(Sensor);
*
* // Set recurrent scheduler task specifying a suitable period in ticks
* scheduler_AddTask_wPTR(Sensor_update,
*                        mySensorPointer,
*                        _ticksOfDelay,
*						 _period);
*
* // Load sensor access struct defaults
* Sensor_start(mySensorPointer);
*
* // call update() once to guarantee that the selected defaults are loaded once
* Sensor_update(mySensorPointer);
*
* // Set timed delay callbacks if needed for the sensor
* Sensor_setPowerUpDelayTicks(mySensorPointer, int _delayTicks);
* Sensor_setResetDelayTicks(mySensorPointer, int _delayTicks);
* Sensor_setMeasurementDelayTicks(mySensorPointer, int _delayTicks);
*
* // Initiate automated measurement cycle
* Sensor_measureAndProcess(mySensorPointer);
*
* // report and alarm callbacks will fire if they are set
*
* // Deleting a sensor
* // safeDelete sets sensor pointer to NULL while deleting sensor structure
* mySensorPointer = safeDelete(mySensorPointer);
* @endcode
*
*
*/
void Sensor_init(void);

/*! Initializes sensor access structure defaults.  Sets the data members of a
 *  specific sensor to their defaults values.  Returns self on success,
 *  otherwise NULL.  Callbacks and other individualized settings should be
 *  set AFTER start().  Required callback delays are powerUp, reset and
 *  measurement, which should be greater than '0' ticks.
*/
void * Sensor_start(void * _self);

/****************************************/
/*** interface manager for callbacks  ***/

// TODO:  Move delay callback materials to the private header file
/*! Arms the asynchronous callbacks from the scheduler.  The callback function
 *  will be fired with the sensor access structure pointer _self as a parameter.
 *  Requires ... include scheduler.h
 */
sensor_cb_fnct Sensor_armDelayedCallback(void *         _self,
		                                 sensor_cb_fnct _callback,
										 int            _ticksOfDelay);

// Required callbacks to be setup as tasks on the scheduler using above
// _ticksOfDelay setting depends on duration of the scheduler tick
void Sensor_postEnablePower(void * _self);
void Sensor_postAlignAndConfig(void * _self);
void Sensor_postStartMeasurement(void * _self);

// returns current callback function on key sensor state machine events
// can be used to create a callback chains that fire on reportReady or alarm
sensor_cb_fnct Sensor_setOnReportReady_cb(const void * _self,
		                                  sensor_cb_fnct _cb);
sensor_cb_fnct Sensor_getOnReportReady_cb(const void * _self);


sensor_cb_fnct Sensor_setOnAlarmTriggered_cb(const void * _self,
		                                     sensor_cb_fnct _cb);
sensor_cb_fnct Sensor_getOnAlarmTriggered_cb(const void * _self);

/*******************************************/
/*** Post sensor configuration controls  ***/

/*! Performs complete sensor power up and measurement cycle, processes data
 *  and automatically disables power.
*/
void * Sensor_measureAndProcess(void * _self);

/*! Manual method to disable sensor power.  Power is automatically disabled
 *  if Sensor_disablePower() is populated.  Use this manual method if
 *  Sensor_disablePower() method is left blank.  Does not affect the sensor
 *  access structure data.  The sensor can be re-powered immediately, or a new
 *  automatic measurement can be initiated.
 */
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

// add comment
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
void * Sensor_writeDataToSensor(void * _self,
		                        void * _dataPointer,
								int count);
void * Sensor_readDataFromSensor(void * _self,
		                         void * _dataPointer,
								 int count);


/******************************************/
/*** Sensor data member access methods  ***/

/*!  Primary state machine variable.  This would not normally be accessed
 *   by code calling into a sensor driver.  Access is provided to facilitate
 *   the detailed code needed to implement other sensor drivers.
 */
sensorState_t Sensor_getSensorState(const void  * _self);
sensorState_t Sensor_setSensorState(void * _self, sensorState_t _sensorState);

/*!  Secondary state machine variable.  This would not normally be accessed
 *   by code calling into a sensor driver.  Access is provided to facilitate
 *   the detailed code needed to implement other sensor drivers, where the
 *   primary state machine variable proves to be too course and additional
 *   states are needed.
 */
miniState_t Sensor_getMiniState(const void  * _self);
miniState_t Sensor_setMiniState(void * _self, miniState_t _miniState);


/*!  Asynchronous sensor state machine state change requested are captured
 *   by setting this flag.  The state will initially be unknown.  An
 *   asynchronous request to start, will set it to SENSOR_ASYNC_START_REQUEST.
 *   A value of SENSOR_ASYNC_START_DONE indicates that the start process is
 *   complete and that the sensor methods can be called.  A request for
 *   measurement using SENSOR_ASYNC_MEASURE_REQUEST, will initiate a
 *   measurement if the sensor has previously been started, or it will
 *   initiate a start followed by a measurement if the sensor is un-started.
 *   SENSOR_ASYNC_MEASURE_DONE indicates that the prior measure request has
 *   been completed.  Only one measurement request at a time will be monitored.
 *   Measurement request are not individually counted and processed.
 */
sensorAsyncFlag_t Sensor_getAsyncFlag(const void  * _self);
sensorAsyncFlag_t Sensor_setAsyncFlag(void * _self,
		                              sensorAsyncFlag_t _asyncFlag);

/*!  Post enable power CB delay in system scheduler ticks.  The specific
 *   number of ticks depends on the system wake up frequency.  The specified
 *   callback fires after the appropriate period to allow the sensor power
 *   system to stabilize.  This assumes that sensor power is removed when
 *   the sensor is not in use.  No communication with the sensor should be
 *   attempted until the sensor is powered AND stable.
 */
sensorDelayTicks_t Sensor_getEnablePowerDelayTicks(const void * _self);
sensorDelayTicks_t Sensor_setPowerUpDelayTicks(void * _self,
		                                       sensorDelayTicks_t _delayTicks);

/*!  Post reset and align CB delay in system scheduler ticks.  The specific
 *   number of ticks depends on the system wake up frequency.  The specified
 *   callback fires after the appropriate period to allow the sensor to
 *   stabilize after it has been configured.  This assumes that sensor requires
 *   some amount of time to stabilize.  No sensor measurements should be
 *   attempted until the sensor is stable.
 */
sensorDelayTicks_t Sensor_getAlignConfigDelayTicks(const void * _self);
sensorDelayTicks_t Sensor_setAlignConfigDelayTicks(void * _self,
		                                     sensorDelayTicks_t _delayTicks);

/*!  Post measurement delay in system scheduler ticks.  The specific
 *   number of ticks depends on the system wake up frequency.  The specified
 *   callback fires after the appropriate period indicating that the sensor
 *   measurement is complete.
 */
sensorDelayTicks_t Sensor_getMeasurementDelayTicks(const void * _self);
sensorDelayTicks_t Sensor_setMeasurementDelayTicks(void * _self,
		                                           sensorDelayTicks_t _delayTicks);

//! Pointer to the command buffer used to communicate with sensor
void * Sensor_getCommandPointer(const void * _self);
void * Sensor_setCommandPointer(      void * _self, void * _commandPointer);

//! Pointer to the unprocessed data buffer
void * Sensor_getRawDataPointer(const void * _self);
void * Sensor_setRawDataPointer(      void * _self, void * _rawDataPointer);

//! Pointer to the processed data buffer
void * Sensor_getFinalDataPointer(const void * _self);
void * Sensor_setFinalDataPointer(void * _self, void * _finalDataPointer);

//! Pointer alarm level(s) Nodes
void * Sensor_getAlarmLevelsPointer(const void * _self);
void * Sensor_setAlarmLevelsPointer(void * _self, void * _alarmLevelsPointer);

/*!  Accessor methods for the primary alarm threshold level.
 */
value_t Sensor_getLowerPrimaryAlarmLevel(const void * _self);
void *  Sensor_setLowerPrimaryAlarmLevel(void * _self, value_t _value);

/*!  Accessor methods for the secondary alarm threshold level.  There will always
 *   be a primary alarm level if the secondary level is populated.
 */
value_t Sensor_getUpperSecondaryAlarmLevel(const void * _self);
void *  Sensor_setUpperSecondaryAlarmLevel(void * _self, value_t _value);

/*! Current alarm state.  Note that the current alarm state must be compared
 *  with the expected or normal alarm state to determine if the
 *  onAlarmLevelTriggered_cb should be fired.
 */
alarmType_t Sensor_getAlarmState(const void * _self);
alarmType_t Sensor_setAlarmState(void * _self, alarmType_t _alarmState);

alarmType_t Sensor_getNormalState(const void * _self);
alarmType_t Sensor_setNormalState(void * _self, alarmType_t _normalState);

sensorReportStatus_t Sensor_reportReady(const void * _self);

void Sensor_transitionState(struct Sensor * _self, sensorState_t _state);
void * Sensor_resetMiniState(struct Sensor * _self);
void * Sensor_incrementMiniState(struct Sensor * _self);

#endif /* SRC_BUTTON_BUTTON_H_ */
