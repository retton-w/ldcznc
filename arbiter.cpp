#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sensor.h"
#include "arbiter.h"
#include "image_buffer.h"
#include "common.h"

//抓拍方式标记位(手动抓拍、视频、外触发抓拍)
const unsigned int position = CAMERA_500W_LINE * CAMERA_500W_HEIGHT; 

enum {
	INVALID_MODE	= 0,
	VIDEO_MODE	= 1,
	MANUAL_SNAP	= 2,
	HARD_TRIGGER	= 3
};


Arbiter::Arbiter()
{
	Init();
}


Arbiter::~Arbiter()
{
	delete _image_processor;
}


void Arbiter::Init()
{
	_image_processor = new ImageProcess();

	_pCamera = CameraFactory::Create500WCamera();
	_pCamera->CameraInit();					//初始化相机
	int handle = _pCamera->GetCameraHandle();		//获取相机设备文件句柄
	Sensor::GetInstance()->SetSensorHandle(handle);		//向sensor模块设置相机设备文件句柄
	Sensor::GetInstance()->SetSensorCapture();		//初始化相机为触发模式	
}


void Arbiter::Run()
{
	struct cmemBuffer *pBuffer = NULL;
	struct image_buffer_description *image = NULL;
	char snapMode = INVALID_MODE;
	_pCamera->StartCapture();	//开始采集数据

	while (!IsTerminated()) {
		
		if (!_pCamera->GetCaptureBuffer(&pBuffer)) {
			usleep(15000);
			//printf("can't get Buffer from driver\n");
			continue;
		}
		printf("get Buffer from driver\n");

		if (capture_buffer_alloc(&image)) {
			_pCamera->PutCaptureBuffer(pBuffer);
			printf("Fun:%s, Line:%d, get image buffer from list failed\n", __func__, __LINE__);
			usleep(15000);
			continue;
		}

		//snapMode = *((char *)pBuffer->vir_addr + position);
		snapMode = MANUAL_SNAP;
		switch (snapMode) {
		case VIDEO_MODE:	//视频模式处理
			_image_processor->VideoProcess(pBuffer, image);
			break;
		case MANUAL_SNAP:	//手动抓拍处理
			printf("manual snap\n");
			_image_processor->CaptureProcess(pBuffer, image);
			break;
		case HARD_TRIGGER:
			_image_processor->CaptureProcess(pBuffer, image);
		default:
			break;
		}
		//capture_buffer_free(image);
		transport_buffer_put(image);
		image = NULL;

		_pCamera->PutCaptureBuffer(pBuffer);
	}	

	_pCamera->CameraCleanup();	//释放相机资源
	return;
}

