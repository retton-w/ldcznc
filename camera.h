/**
 * @file	camera.h
 * @brief	
 * @author	hrh <huangrh@landuntec.com>
 * @version	1.0.0
 * @date	2011-11-29
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
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "cecodec.h"

typedef struct _VideoSize {

	int width;
	int height;
	int stride;
	int line;

} VideoSize;


class Camera
{

public:
	Camera();
	~Camera();
	virtual bool CameraCleanup();

	virtual bool SetVideoFormat() = 0;
	virtual bool InitDevice() = 0;
	virtual bool CameraInit() = 0;

	virtual bool GetCaptureBuffer(struct cmemBuffer **pCaptureBufs) = 0;
	virtual bool PutCaptureBuffer(struct cmemBuffer *pCaptureBufs) = 0;

	int _CameraFd;
	int _width;
	int _height;
	int _stride;
	int _line;
	unsigned int _maxNumBufs;
	
	struct cmemBuffer *_pCmemBuffer;

	bool SetCaptureSize(VideoSize size);
	bool StopCapture();

	bool OpenDevice(const char *deviceName);
	bool StartCapture();
	bool UserPtrInit();
	
	int  GetCameraHandle();
};


#endif

