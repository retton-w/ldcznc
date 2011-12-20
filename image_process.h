#ifndef _IMAGE_PROCESS_H_
#define _IMAGE_PROCESS_H_

#include "cecodec.h"
#include "resize.h"
#include "thread.h"
#include "camera_factory.h"


enum CaptureStatus {

	//IDLE,
	VIDEO,
	CAPTURE
};


class ImageProcess : public Thread
{

public:

	ImageProcess();
	~ImageProcess();

	void SetStatus(CaptureStatus status);

	void SetJpegFullSize();
	void SetJpegHalfSize();
	void SetJpegQvalue(int value);

	bool OverlayText(struct cmemBuffer &imageInfo, const struct overlay_text *text, int len);
	bool JpegEncodeProcess(struct cmemBuffer &imageInfo, struct cmemBuffer *outBuffer);
	bool Resize(struct cmemBuffer *imageInfo, struct cmemBuffer *outBuffer);
	bool EmbedWatermark();

	
	void CaptureProcess(struct cmemBuffer *buffer);
	void VideoProcess(struct cmemBuffer *buffer);

protected:

	void Run();

private:
	
	void Init();
	void GetWatermark(char* pWmcode);
	Resize_Handle rsz_handle;
	struct jpeg_params _param;

	CaptureStatus _status;
	Camera *_pCamera;

	struct cmemBuffer resize_buffer;
};


#endif

