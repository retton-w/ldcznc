/**
 * @file sensor.h
 * @brief	
 * @author hrh <huangrh@landuntec.com>
 * @version 1.0.0
 * @date 2011-11-28
 * 
 * @verbatim
 * ============================================================================
 * Copyright (c) Shenzhen Landun technology Co.,Ltd. 2011
 * All rights reserved. 
 * 
 * Use of this software is controlled by the terms and conditions found in the
 * license agreenment under which this software has been supplied or provided.
 * ============================================================================
 * 
 * @endverbatim
 * 
 */


#ifndef _SENSOR_H_
#define _SENSOR_H_

/**
 * @brief	前端sensor控制类	
 */
class Sensor
{
public:
	int SetSensorSelect8Bit();
	int SetSensorSelect10Bit();

	int SetSensorExposure(unsigned int HD);
	int SetSensorGain(unsigned int gain);

	int SetSensorFlashOn();
	int SetSensorFlashOff();

	int SetSensorVideo();
	int SetSensorCapture();

	int SetSensorRgain(unsigned int Rgain);
	int SetSensorBgain(unsigned int Bgain);

	int SetSensorFlashDelay(unsigned int time);

	int SetSensorContinousCapNum(unsigned int num);
	int SetSensorContinousCapDuration(unsigned int time);

	int SetSensorAEAuto();
	int SetSensorAEManual();

	int SetSensorMaxGain(unsigned int gain);
	int SetSensorMinGain(unsigned int gain);

	int SetSensorMaxExp(unsigned int HD);
	int SetSensorMinExp(unsigned int HD);

	int SetSensorVideoTargetGray(unsigned int gray);
	int SetSensorCaptureTargetGray(unsigned int gray);

	int SetSensorAEMethod(unsigned int method);
	int SetSensorAEZone(unsigned int zone);

	bool SetSensorHandle(int handle);

	static Sensor *GetInstance();
	static void Release();

private:
	
	Sensor();
	~Sensor();

	int SetFPGAParam(unsigned char addr, unsigned char val);

	int _sensorHandle;

	static Sensor *_Sensor;

};

#endif

