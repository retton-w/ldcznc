#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "image_buffer.h"
#include "image_process.h"
#include "overlay.h"
#include "ldspmil.h"
#include "common.h"


ImageProcess::ImageProcess()
{
	Init();
}


void ImageProcess::Init()
{
	struct overlay_font overlay_font;
	overlay_font.name      = FONT_NAME;
        overlay_font.size      = FONT_SIZE;
	overlay_init(&overlay_font);
	
	_param.width	= CAMERA_500W_WIDTH;
	_param.height	= CAMERA_500W_HEIGHT + CAMERA_OVERLAY_HEIGHT;
	_param.stride	= CAMERA_500W_STRIDE;
	_param.qvalue	= DEFAULT_JPEG_QVALUE;
	jpeg_init(&_param);
	printf("init jpeg\n");

	rsz_handle = Resize::ResizeCreate();
	if (rsz_handle == NULL) {
		printf("resize create failed\n");
	}
	
	_status = CAPTURE;

	resize_buffer.width	= RESIZER_WIDTH;
	resize_buffer.height	= RESIZER_HEIGHT;
	resize_buffer.stride	= RESIZER_STRIDE;
	resize_buffer.line	= RESIZER_LINE;
	resize_buffer.extra_size= 0;
	buffer_create(&resize_buffer);
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
	_param.width	= CAMERA_500W_WIDTH;
	_param.height	= CAMERA_500W_HEIGHT + CAMERA_OVERLAY_HEIGHT;
	_param.stride	= CAMERA_500W_STRIDE; 
	set_jpegenc_param(&_param);	
}


void ImageProcess::SetJpegHalfSize()
{
	_param.width	= RESIZER_WIDTH;
	_param.height	= RESIZER_HEIGHT;
	_param.stride	= RESIZER_STRIDE;
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


/**
 * @fn		bool Resize(struct cmemBuffer *imageInfo,
 * 			    struct cmemBuffer *outBuffer)
 *
 * @brief	将500W的图像缩小成1216 * 1024分辨率
 *
 * @param	[in] imageInfo
 * @param	[out] outBuffer
 *
 * @return	是否resize成功
 * @retval	true | false
 */
bool ImageProcess::Resize(struct cmemBuffer *imageInfo, struct cmemBuffer *outBuffer)
{
	int input_height  = imageInfo->height;
	int input_width	  = imageInfo->width;
	int output_height = outBuffer->height;
	int output_width  = outBuffer->width;

	imageInfo->width  = imageInfo->width / 2;
	imageInfo->height = (imageInfo->height > CAMERA_500W_HEIGHT) ? CAMERA_500W_HEIGHT: imageInfo->height;
	imageInfo->size   = imageInfo->height * imageInfo->line;
	imageInfo->offset = 0;
	outBuffer->width  = outBuffer->width / 2;
	outBuffer->offset = 0;
	Resize::ResizeConfig(rsz_handle, imageInfo, outBuffer, NULL);	//配置resizer

	Resize::ResizeExecute(rsz_handle, imageInfo, outBuffer, NULL);	//左半图像resize

	imageInfo->offset = imageInfo->width * 2;
	outBuffer->offset = outBuffer->width * 2;
	Resize::ResizeExecute(rsz_handle, imageInfo, outBuffer, NULL);	//右半图像resize

	outBuffer->width  = output_width;
	outBuffer->height = output_height; 
	outBuffer->offset = 0;
	imageInfo->width  = input_width;
	imageInfo->height = input_height;
	imageInfo->offset = 0;

	return true;
}


void ImageProcess::VideoProcess(struct cmemBuffer *buffer,
				struct image_buffer_description *image)

{
	if (_status == CAPTURE) {
		_status = VIDEO;
		SetJpegHalfSize();
		usleep(2000);
	}

	Resize(buffer, &resize_buffer);
	JpegEncodeProcess(resize_buffer, &image->info);
}


void ImageProcess::CaptureProcess(struct cmemBuffer *buffer,
				  struct image_buffer_description *image)
{
	if (_status == VIDEO) {
		_status = CAPTURE;
		SetJpegFullSize();
		usleep(2000);
	}

	JpegEncodeProcess(*buffer, &image->info);
}

