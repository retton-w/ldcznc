#ifndef _ARBITER_H_
#define _ARBITER_H_

#include "thread.h"
#include "camera_factory.h"
#include "image_process.h"

class Arbiter : public Thread
{

public:
	Arbiter();
	~Arbiter();

	void CaptureProcess(struct cmemBuffer *buffer);
	void VideoProcess(struct cmemBuffer *buffer);

protected:
	void Run();

private:
	void Init();

	Camera *_pCamera;

	ImageProcess *_image_processor;
};

#endif

