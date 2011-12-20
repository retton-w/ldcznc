#ifndef _CAMERA_500W_H_
#define _CAMERA_500W_H_

#include "camera.h"

class Camera500W:public Camera
{

public:
	
	Camera500W();
	~Camera500W();

	bool CameraInit();
	bool InitDevice();

	bool SetVideoFormat();
	bool GetCaptureBuffer(struct cmemBuffer **pCaptureBufs);
	bool PutCaptureBuffer(struct cmemBuffer *pCaptureBufs);


private:

	
};

#endif

