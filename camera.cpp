/**
 * @file	camera.cpp
 * @brief	
 * @author	hrh <huangrh@landuntec.com>
 * @version	1.0.0
 * @date	2011-11-30
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


#include <iostream>
#include <iomanip>
#include "camera.h"

/* 标准Linux头文件 */
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

/* DaVinci 特有的内核头文件 */
#include <media/davinci/videohd.h>


Camera::Camera()
{

}


Camera::~Camera()
{

}


bool Camera::OpenDevice(const char *deviceName)
{
	_CameraFd = open(deviceName, O_RDWR | O_NONBLOCK, 0);
	if (_CameraFd == -1) {
		
		return false;
	}

	return true;
}


int Camera::GetCameraHandle()
{
	return _CameraFd;
}


bool Camera::StartCapture()
{
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (ioctl(_CameraFd, VIDIOC_STREAMON, &type) == -1) {

		std::cout << "VIDIOC_STREAMON failed" << std::endl;
		return false;
	}

	std::cout << "Stream on success" << std::endl;

	return true;
}


bool Camera::StopCapture(void)
{
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (ioctl(_CameraFd, VIDIOC_STREAMOFF, &type) == -1) {

		std::cout << "VIDIOC_STREAMOFF failed" << std::endl;
		return false;
	}

	std::cout << "Stream off success" << std::endl;
	
	for (unsigned int bufIndex = 0; bufIndex < _maxNumBufs; bufIndex++) {
		buffer_delete(_pCmemBuffer + bufIndex);
	}
	std::cout << "delete buffer" << std::endl;

	delete[] _pCmemBuffer;

	return true;
}


bool Camera::SetCaptureSize(VideoSize size)
{
	_width	= size.width;
	_height	= size.height;
	_stride	= size.stride;
	_line	= size.line;

	return true;
}

bool Camera::UserPtrInit(void)
{

	struct cmemBuffer *pCapBufs = new struct cmemBuffer[_maxNumBufs];
	struct v4l2_buffer buf;

	for (unsigned int bufIndex = 0; bufIndex < _maxNumBufs; bufIndex++) {
	
		pCapBufs[bufIndex].index	= bufIndex;
		pCapBufs[bufIndex].width	= _width;
		pCapBufs[bufIndex].height	= _height;
		pCapBufs[bufIndex].stride	= _stride;
		pCapBufs[bufIndex].line		= _line;
		pCapBufs[bufIndex].extra_size	= 256;

		if (buffer_create(&pCapBufs[bufIndex]) != 0) {
			std::cout << "buffer create failed" << std::endl;
			return false;
		}

		memset(&buf, 0, sizeof(struct v4l2_buffer));
		buf.type	= V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory	= V4L2_MEMORY_USERPTR;
		buf.index	= pCapBufs[bufIndex].index;
		buf.m.userptr	= pCapBufs[bufIndex].vir_addr;
		buf.length	= pCapBufs[bufIndex].size;

		std::cout << "buf length " <<  buf.length << std::endl;
		if (ioctl(_CameraFd, VIDIOC_QBUF, &buf) == -1)
		{
			std::cout << "VIODIOC_QBUF failed." << strerror(errno) << std::endl;
			return false;
		}

	}

	_pCmemBuffer = pCapBufs;
	pCapBufs = NULL;
	return true;
}

bool Camera::CameraCleanup()
{
	StopCapture();
	close(_CameraFd);

	return true;
}

