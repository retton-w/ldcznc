#ifndef _CAMERAFACTORY_H_
#define _CAMERAFACTORY_H_

#include "camera_500w.h"

class CameraFactory
{
public:
	static Camera *Create500WCamera(void);

private:

	CameraFactory();
	~CameraFactory();
};

#endif

