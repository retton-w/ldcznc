#ifndef _IMAGE_PROCESS_H_
#define _IMAGE_PROCESS_H_

#include "cecodec.h"
#include "resize.h"


enum CaptureStatus {

	VIDEO,
	CAPTURE
};


class ImageProcess
{

public:
	ImageProcess();
	~ImageProcess();

	void SetJpegQvalue(int value);
	void CaptureProcess(struct cmemBuffer *buffer,
			    struct image_buffer_description *image);
	void VideoProcess(struct cmemBuffer *buffer,
			  struct image_buffer_description *image);

private:
	
	void Init();	
	void SetJpegFullSize();
	void SetJpegHalfSize();
	
	bool OverlayText(struct cmemBuffer &imageInfo, const struct overlay_text *text, int len);
	bool JpegEncodeProcess(struct cmemBuffer &imageInfo, struct cmemBuffer *outBuffer);
	bool Resize(struct cmemBuffer *imageInfo, struct cmemBuffer *outBuffer);
	bool EmbedWatermark();

	void GetWatermark(char* pWmcode);
	Resize_Handle rsz_handle;
	struct jpeg_params _param;

	CaptureStatus _status;
	struct cmemBuffer resize_buffer;
};

#endif

