#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "image_buffer.h"
#include "image_process.h"
#include "sensor.h"
#include "overlay.h"

#include "ldspmil.h"


ImageProcess::ImageProcess()
{
	Init();
	
	_status = CAPTURE;

	_pCamera = CameraFactory::Create500WCamera();
	_pCamera->CameraInit();
	int handle = _pCamera->GetCameraHandle();
	Sensor::GetInstance()->SetSensorHandle(handle);
	Sensor::GetInstance()->SetSensorCapture();
	
	resize_buffer.width	= 1216;
	resize_buffer.height	= 1024;
	resize_buffer.stride	= 1216;
	resize_buffer.line	= 2432;
	resize_buffer.extra_size= 0;
	buffer_create(&resize_buffer);
}


void ImageProcess::Init()
{
	struct overlay_font overlay_font;
	overlay_font.name      = "simsun.ttc";
        overlay_font.size      = 28;
	overlay_init(&overlay_font);
	
	_param.width	= 2432;
	_param.height	= 2112;
	_param.stride	= 2432; 
	_param.qvalue	= 90;
	jpeg_init(&_param);

	rsz_handle = Resize::ResizeCreate();
	if (rsz_handle == NULL) {
		printf("resize create failed\n");
	}
}


ImageProcess::~ImageProcess()
{
	buffer_delete(&resize_buffer);
	Resize::ResizeDelete(rsz_handle);
	jpeg_exit();
	overlay_deinit();
	printf("deconstruct function: %s\n", __func__);
}


void ImageProcess::SetJpegFullSize()
{
	_param.width	= 2432;
	_param.height	= 2112;
	_param.stride	= 2432; 
	set_jpegenc_param(&_param);	
}


void ImageProcess::SetJpegHalfSize()
{
	_param.width	= 1216;
	_param.height	= 1024;
	_param.stride	= 1216;
	set_jpegenc_param(&_param);
}


void ImageProcess::SetJpegQvalue(int value)
{
	_param.qvalue = value;
	set_jpegenc_param(&_param);
}


void ImageProcess::GetWatermark(char* pWmcode)
{
	char* pcode = pWmcode;
	int bytWmlong = 16;

	int iFlag = 62;
	char allArray[63] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

	clock_t start_time;
	clock_t cur_time;

	int i,k;

	start_time = clock();
	do {
		cur_time = clock();
	} while ((cur_time - start_time) < 3);

	srand((unsigned)time(NULL) + clock());

	for (i = 0; i < bytWmlong; i++) {
		k = rand();
		k = k % iFlag;
		pcode[i] = allArray[k];
	}

	pcode[bytWmlong] = '\0';

}


bool ImageProcess::OverlayText(struct cmemBuffer &imageInfo, const struct overlay_text *text, int len)
{
	
	overlay_multi(&imageInfo, text, len);
	return true;
}


bool ImageProcess::JpegEncodeProcess(struct cmemBuffer &imageInfo, struct cmemBuffer *outBuffer)
{

	jpeg_encode((void *)imageInfo.vir_addr, imageInfo.size, (void *)outBuffer->vir_addr, &outBuffer->size);
	
	outBuffer->width	= imageInfo.width;
	outBuffer->height	= imageInfo.height;
	outBuffer->line		= imageInfo.line;

#if 0
	char deviceNo[8] = "001";
	char watermark[32];
	GetWatermark(watermark);
	EmbedAntiCounterfeitInfo((unsigned char *)deviceNo, strlen(deviceNo),
				 (unsigned char *)watermark, strlen(watermark),
				 (unsigned char *)outBuffer->buffer, (unsigned long *)&outBuffer->length); 

	CreateJPGFileHash((unsigned char *)outBuffer->buffer, (unsigned long *)&outBuffer->length);
#endif

	return true;

}


bool ImageProcess::Resize(struct cmemBuffer *imageInfo, struct cmemBuffer *outBuffer)
{


	int input_height	= imageInfo->height;
	int input_width		= imageInfo->width;
	int output_height	= outBuffer->height;
	int output_width	= outBuffer->width;

	imageInfo->width  = imageInfo->width / 2;
	imageInfo->height = (imageInfo->height > 2048) ? 2048: imageInfo->height;
	imageInfo->size   = imageInfo->height * imageInfo->line;
	imageInfo->offset = 0;

	outBuffer->width  = outBuffer->width / 2;
	outBuffer->offset = 0;
	Resize::ResizeConfig(rsz_handle, imageInfo, outBuffer, NULL);

	Resize::ResizeExecute(rsz_handle, imageInfo, outBuffer, NULL);

	imageInfo->offset = imageInfo->width * 2;
	outBuffer->offset = outBuffer->width * 2;
	Resize::ResizeExecute(rsz_handle, imageInfo, outBuffer, NULL);

	outBuffer->width  = output_width;
	outBuffer->height = output_height; 
	outBuffer->offset = 0;
	imageInfo->width  = input_width;
	imageInfo->height = input_height;
	imageInfo->offset = 0;

	return true;
}


void ImageProcess::Run()
{
	struct cmemBuffer *pBuffer;
	CaptureStatus last_status = CAPTURE;
	FILE *fp;

	_pCamera->StartCapture();

	while (!IsTerminated()) {

		if (last_status != _status) {
			last_status = _status;
			if (_status == VIDEO) {
				printf("set half size\n");
				SetJpegHalfSize();
			} else {
				printf("set full size\n");
				SetJpegFullSize();
			}
			usleep(5000);
			continue;
		}

		if (_pCamera->GetCaptureBuffer(&pBuffer)) {

			printf("get Buffer from driver\n");
			switch (last_status) {
			case VIDEO:
				printf("VIDEO\n");
				VideoProcess(pBuffer);
				break;
			case CAPTURE:
				usleep(10);
				fp = fopen("origin.yuv", "wb");
				fwrite((char *)pBuffer->vir_addr,2432 * 2049 * 2, 1, fp);
				fclose(fp);
				CaptureProcess(pBuffer);
				printf("CAPTURE\n");
				break;
			default:
				break;
			}
			_pCamera->PutCaptureBuffer(pBuffer);

		} else {
			usleep(15000);
			//printf("can't get Buffer from driver\n");
		}
	}

	_pCamera->CameraCleanup();

	return;
}


void ImageProcess::SetStatus(CaptureStatus status)
{
	
	_status = status;
}


void ImageProcess::VideoProcess(struct cmemBuffer *buffer)
{
	
	struct image_buffer_description *image = NULL;

	if (capture_buffer_alloc(&image) == 0) {
	
		printf("get image buffer from list\n");
		Resize(buffer, &resize_buffer);
		JpegEncodeProcess(resize_buffer, &image->info);

		transport_buffer_put(image);
		//capture_buffer_free(image);
		image = NULL;
	}
}


void ImageProcess::CaptureProcess(struct cmemBuffer *buffer)
{
	struct image_buffer_description *image = NULL;
	if (capture_buffer_alloc(&image) == 0) {

		printf("func: %s, get image buffer from list\n", __func__);
		JpegEncodeProcess(*buffer, &image->info);

		//capture_buffer_free(image);
		transport_buffer_put(image);
		image = NULL;
	}
}

