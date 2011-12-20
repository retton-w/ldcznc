/**
 * @file	sensor.cpp
 * @brief	FPGA控制接口
 * @author	hrh <huangrh@landuntec.com>
 * @version	1.0.0
 * @date	2011-10-24
 * @note	通过单体模式实现FPGA控制接口
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

#include <iomanip>

/* 标准Linux头文件 */
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

/* DaVinci 特有的内核头文件 */
#include <media/davinci/videohd.h>

#include "sensor.h"


Sensor *Sensor::_Sensor = 0;

Sensor::Sensor()
:_sensorHandle(-1)
{

}

Sensor::~Sensor()
{
	_sensorHandle = -1;
}


bool Sensor::SetSensorHandle(int handle)
{
	_sensorHandle = handle;

	return true;
}


void Sensor::Release()
{

	if (_Sensor != 0) {

		delete _Sensor;
		_Sensor = 0;
	}

}


Sensor *Sensor::GetInstance()
{
	if (_Sensor == 0) {

		_Sensor = new Sensor();
	}

	return _Sensor;
}


int Sensor::SetSensorExposure(unsigned int HD)
{
	printf("func is %s\n", __func__);
	if (HD > 1023)
		HD = 1023;

	unsigned char val = HD & 0xFF;
	if (SetFPGAParam(0xab, val))
		return -1;

	val = (HD >> 8) & 0xFF;
	if (SetFPGAParam(0xaa, val))
		return -1;

	return 0;

}


int Sensor::SetSensorGain(unsigned int gain)
{
	printf("func is %s\n", __func__);
	if (gain > 1023)
		gain = 1023;

	unsigned char val = gain & 0xFF;
	if (SetFPGAParam(0x19, val))
		return -1;

	val = (gain >> 8) & 0xFF;
	if (SetFPGAParam(0x18, val))
		return -1;

	return 0;
	
}


int Sensor::SetSensorFlashOn()
{
	printf("func is %s\n", __func__);

	if (SetFPGAParam(0xbb, 0x01))
		return -1;

	return 0;

}


int Sensor::SetSensorFlashOff()
{
	printf("func is %s\n", __func__);

	if (SetFPGAParam(0xbb, 0x00))
		return -1;

	return 0;

}


int Sensor::SetSensorVideo()
{
	printf("func is %s\n", __func__);

	if (SetFPGAParam(0x55, 0x03))
		return -1;

	return 0;

}


int Sensor::SetSensorCapture()
{
	printf("func is %s\n", __func__);
	
	if (SetFPGAParam(0x55, 0x04))
		return -1;

	return 0;

}


int Sensor::SetSensorRgain(unsigned int Rgain)
{
	printf("func is %s\n", __func__);

	if (Rgain > 255)
		Rgain = 255;
	
	unsigned char val = Rgain & 0xFF;
	if (SetFPGAParam(0xdd, val))
		return -1;

	return 0;

}


int Sensor::SetSensorBgain(unsigned int Bgain)
{
	printf("func is %s\n", __func__);

	if (Bgain > 255)
		Bgain = 255;
	
	unsigned char val = Bgain & 0xFF;
	if (SetFPGAParam(0xff, val))
		return -1;

	return 0;

}


int Sensor::SetSensorFlashDelay(unsigned int time)
{
	printf("func is %s\n", __func__);
	
	if (time > 5)
		time = 5;
	
	unsigned char val = time & 0xFF;
	if (SetFPGAParam(0x22, val))
		return -1;

	return 0;

}


int Sensor::SetSensorContinousCapNum(unsigned int num)
{
	printf("func is %s\n", __func__);
	
	if (num > 10)
		num = 10;
	
	unsigned char val = num & 0xFF;
	if (SetFPGAParam(0x33, val))
		return -1;

	return 0;

}


int Sensor::SetSensorContinousCapDuration(unsigned int time)
{

	printf("func is %s\n", __func__);
	if (time > 1023)
		time = 1023;

	unsigned char val = time & 0xFF;
	if (SetFPGAParam(0x4b, val))
		return -1;

	val = (time >> 8) & 0xFF;
	if (SetFPGAParam(0x4a,val))
		return -1;

	return 0;

}


int Sensor::SetSensorAEAuto()
{
	
	printf("func is %s\n", __func__);
	if (SetFPGAParam(0x01, 0x00))
		return -1;

	return 0;
	
}


int Sensor::SetSensorAEManual()
{
	printf("func is %s\n", __func__);
	
	if (SetFPGAParam(0x01, 0x01))
		return -1;

	return 0;
	
}


int Sensor::SetSensorAEMethod(unsigned int method)
{
	printf("func is %s\n", __func__);
	if (method > 2)
		method = 0;

	unsigned char val = method & 0xFF;
	if (SetFPGAParam(0x0C, val))
		return -1;

	return 0;

}


int Sensor::SetSensorAEZone(unsigned int zone)
{
	printf("func is %s\n", __func__);
	
	unsigned char val = zone & 0xFF;
	if (SetFPGAParam(0x17, val))
		return -1;

	val = (zone >> 8) & 0xFF;
	if (SetFPGAParam(0x16, val))
		return -1;

	val = (zone >> 16) & 0xFF;
	if (SetFPGAParam(0x15, val))
		return -1;

	val = (zone >> 24) & 0xFF;
	if (SetFPGAParam(0x14, val))
		return -1;

	return 0;

}


int Sensor::SetSensorMaxGain(unsigned int gain)
{
	printf("func is %s\n", __func__);

	if (gain > 1023)
		gain = 1023;

	unsigned char val = gain & 0xFF;
	if (SetFPGAParam(0x05, val))
		return -1;

	val = (gain >> 8) & 0xFF;
	if (SetFPGAParam(0x04, val))
		return -1;

	return 0;

}


int Sensor::SetSensorMinGain(unsigned int gain)
{
	printf("func is %s\n", __func__);

	if (gain > 1023)
		gain = 1023;

	unsigned char val = gain & 0xFF;
	if (SetFPGAParam(0x05, val))
		return -1;

	val = (gain >> 8) & 0xFF;
	if (SetFPGAParam(0x04, val))
		return -1;

	return 0;

}


int Sensor::SetSensorMaxExp(unsigned int HD)
{
	printf("func is %s\n", __func__);
	
	if (HD > 1023)
		HD = 1023;

	unsigned char val = HD & 0xFF;
	if (SetFPGAParam(0x09, val))
		return -1;

	val = (HD >> 8) & 0xFF;
	if (SetFPGAParam(0x08, val))
		return -1;

	return 0;

}


int Sensor::SetSensorMinExp(unsigned int HD)
{
	printf("func is %s\n", __func__);
	
	if (HD > 1023)
		HD = 1023;

	unsigned char val = HD & 0xFF;
	if (SetFPGAParam(0x07, val))
		return -1;

	val = (HD >> 8) & 0xFF;
	if (SetFPGAParam(0x06, val))
		return -1;

	return 0;

}


int Sensor::SetSensorVideoTargetGray(unsigned int gray)
{

	printf("func is %s\n", __func__);
	if (gray > 255)
		gray = 255;

	unsigned char val = gray & 0xFF;
	if (SetFPGAParam(0x0A, val))
		return -1;

	return 0;

}


int Sensor::SetSensorCaptureTargetGray(unsigned int gray)
{

	printf("func is %s\n", __func__);
	if (gray > 255)
		gray = 255;

	unsigned char val = gray & 0xFF;
	if (SetFPGAParam(0x0B, val))
		return -1;

	return 0;

}
	

int Sensor::SetSensorSelect8Bit()
{
	printf("func is %s\n", __func__);
	
	if (SetFPGAParam(0xEE, 0x07))
		return -1;

	return 0;

}


int Sensor::SetSensorSelect10Bit()
{

        printf("%s\n", __func__);
	if (SetFPGAParam(0xEE, 0x08))
		return -1;

	return 0;

}


int Sensor::SetFPGAParam(unsigned char addr, unsigned char val)
{
	
	if (_sensorHandle < 0)
		return -1;

	struct v4l2_dbg_register reg;
	reg.match.type	= V4L2_CHIP_MATCH_I2C_ADDR;
	reg.match.addr	= 0x66;
	reg.reg		= addr;
	reg.val		= val;

	if (ioctl(_sensorHandle, VIDIOC_DBG_S_REGISTER, &reg) == -1) {

		printf("func: %s, can't set addr: %02x with val: %02x, errno is %d, %s\n", __func__, addr, val, errno, strerror(errno));
		//printf("func: %s, can't set addr: %02x with val: %02x\n", __func__, addr, val);
		return -1;
	}

	printf("set fpga success\n");
	return 0;

}

