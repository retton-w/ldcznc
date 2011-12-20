#include "camera_factory.h"

Camera *CameraFactory::Create500WCamera(void)
{
	return new Camera500W();
}

