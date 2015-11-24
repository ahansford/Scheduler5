/*
 * sensors-private.h
 *
 *  Created on: Aug 15, 2015
 *      Author: alhansfo
 */

#ifndef SRC_SENSORS_SENSORS_PRIVATE_H_
#define SRC_SENSORS_SENSORS_PRIVATE_H_

struct Sensor {
	const struct Object _;	// should the superclass
	sensorState_t      sensorState;  //! The primary state variable
	int				   miniState;    //! A generic state variable for sub-states
	sensorAsyncFlag_t  asyncFlag;   //! registers external asynchronous state machine change requests
	sensorDelayTicks_t powerUpDelayTicks;  //! Post enable power CB delay in ticks
	int 			   configDelayTicks;    //! Post align & config CB delay in ticks
	//! Post measurement CB delay in ticks
	int 			measurementDelayTicks;
	//command_t *		commandPointer;  //! Pointer to the command buffer
	void * 			rawDataPointer;  //! Pointer to the unprocessed data buffer
	void * 			finalDataPointer;  //! Pointer to the processed data buffer
	void * 			alarmLevelsPointer;  //! Pointer alarm level(s) (nodes)
	alarmType_t 	alarmState;          //! Current alarm state
	alarmType_t		normalState;         //! Expected (normal) alarm state
	//! Callback to middleware when sensor data is ready to be reported out
	struct Sensor * (*Sensor_onReportReady_cb)    (struct Sensor * _sensor);
	//! Callback to middleware when alarm state does not match the expected
	struct Sensor * (*Sensor_onAlarmTriggered_cb) (struct Sensor * _sensor);
	struct IO *		ioStructPtr // pointer to the IO object managing sensor I/O
};

struct SensorClass {
	const struct Class	_;	// should be superclass: Class or "somethingClass"
	void * (*writeDataToSensor)	(void * _self, void * _dataPointer, int count);
	void * (*readDataFromSensor)(void * _self, void * _dataPointer, int count);

	void * (*loadDefaults)    (void * _self);
	void * (*enablePower)     (void * _self);
	void * (*alignAndConfig)  (void * _self);
	void * (*startMeasurement)(void * _self);
	void * (*storeRawData)    (void * _self);
	void * (*disablePower)    (void * _self);
	void * (*processRawData)  (void * _self);
	void * (*checkAlarms)     (void * _self);
};

/*****************************/
/****** Constructors  ********/

void * SensorClass_ctor(void * _self, va_list * app);
void * Sensor_default_ctor     (void * _self, va_list * app);

/***********************************/
/****** Overload Functions  ********/

void *      Sensor_default_dtor  (void * _self);
void *      Sensor_default_copy  (void * _copyTo, const void * _copyFrom);
equal_t     Sensor_default_equal (const void * _self, const void * _comparisonObject);
void *      Sensor_config(const void * _self, const void * _master);
puto_return_t Sensor_default_puto(const void * _self, FILE * _fp);


/**********************************/
/**********  new methods  *********/

void * super_Sensor_writeDataToSensor(const void * _class,
		                        void * _self, void * _dataPointer, int count);
void * Sensor_default_writeDataToSensor(void * _self, void * _dataPointer, int count);

void * super_Sensor_default_readDataFromSensor(const void * _class,
		                       void * _self, void * _dataPointer, int count);
void * Sensor_default_readDataFromSensor(void * _self, void * _dataPointer, int count);

void * super_Sensor_loadDefaults(const void * _class, void * _self);
void * Sensor_default_loadDefaults(void * _self);

void * super_Sensor_enablePower(const void * _class, void * _self);
void * Sensor_default_enablePower(void * _self);

void * super_Sensor_enablePower(const void * _class, void * _self);
void * Sensor_default_enablePower(void * _self);

void * super_Sensor_alignAndConfig(const void * _class, void * _self);
void * Sensor_default_alignAndConfig(void * _self);

void * super_Sensor_startMeasurement(const void * _class, void * _self);
void * Sensor_default_startMeasurement(void * _self);

void * super_Sensor_storeRawData(const void * _class, void * _self);
void * Sensor_default_storeRawData(void * _self);

void * super_Sensor_disablePower(const void * _class, void * _self);
void * Sensor_default_disablePower(void * _self);

void * super_Sensor_processRawData(const void * _class, void * _self);
void * Sensor_default_processRawData(void * _self);

void * super_Sensor_checkSensorAlarms(const void * _class, void * _self);
void * Sensor_default_checkSensorAlarms(void * _self);

void * Sensor_default_sendDisablePowerCommands(void * _self);
void   Sensor_update(void * _self);

struct Sensor * Sensor_emptyReportReadyCallback(struct Sensor * _self);
struct Sensor * Sensor_emptyAlarmTriggeredCallback(struct Sensor * _self);

#endif /* SRC_SENSORS_SENSORS_PRIVATE_H */
