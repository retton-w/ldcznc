
#include "camera_500w.h"
#include <iostream>
#include <errno.h>
#include <sys/ioctl.h>

#include <linux/videodev2.h>

#include <media/davinci/videohd.h>


const char *deviceName	= "/dev/video0";
const unsigned int CAPTURE_NUM_BUFFER = 3;
const unsigned int NUM_IOCTL_RETIES = 100;


Camera500W::Camera500W()
{
	int extra = 64;

	VideoSize size;
	size.width	= 2432;
	size.height	= 2048 + extra;
	size.stride	= 2432;
	size.line	= size.stride * 2;
	SetCaptureSize(size);

	_maxNumBufs	= CAPTURE_NUM_BUFFER;
}


Camera500W::~Camera500W()
{

}


bool Camera500W::CameraInit()
{
	if (!OpenDevice(deviceName)) {
		
		return false;
	}

	if (!InitDevice()) {

		return false;
	}

	if (!SetVideoFormat()) {

		return false;
	}
	
	if (!UserPtrInit()) {

		return false;
	}
	
	//StartCapture();
	//sleep(1);

	return true;
	
}


bool Camera500W::InitDevice()
{

	struct v4l2_capability cap;
	if (ioctl(_CameraFd, VIDIOC_QUERYCAP, &cap) == -1) {
		return false;
	}

	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		return false;
	}

	if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
		return false;
	}

	struct v4l2_input input;
	unsigned int tmp_input;
	input.type = V4L2_INPUT_TYPE_CAMERA;
	input.index = 0;

	while(1) {

		if (ioctl(_CameraFd, VIDIOC_ENUMINPUT, &input) < 0) {

			return false;
		}

		std::cout << "input.name = " << input.name << std::endl;

		if (!strcasecmp((char *)(input.name), "ldccdmod")) {
			break;
		}

		input.index++;
	}

	if (ioctl(_CameraFd, VIDIOC_S_INPUT, &input.index) == -1) {
		return false;
	}

	if (ioctl(_CameraFd, VIDIOC_G_INPUT, &tmp_input) == -1) {
		return false;
	}

	if (tmp_input != input.index) {
		return false;
	}

	//sleep(1);

	unsigned int failCount = 0;
	v4l2_std_id standard;
	int ret = -1;

	do {

		ret = ioctl(_CameraFd, VIDIOC_QUERYSTD, &standard);

		if (ret == -1 && errno == EAGAIN) {
			failCount++;
		}
		
	} while (ret == -1 && errno == EAGAIN && failCount < NUM_IOCTL_RETIES);

	switch (standard) {

		case V4L2_STD_NTSC:
			std::cout << "Input video standard is NTSC." << std::endl;
			break;
		case V4L2_STD_PAL:
			std::cout << "Input video standard is PAL." << std::endl;
			break;
		case V4L2_STD_200W_15:
			std::cout << "Input video standard is 200W-15." << std::endl;
			break;
		case V4L2_STD_500W_15:
			std::cout << "Input video standard is 500W-15." << std::endl;
			break;
		default:
			std::cout << "standard is not supported" << " standard = " << standard << std::endl;
			return false;
	}
	
	if (ioctl(_CameraFd, VIDIOC_S_STD, &standard) == -1) {
		return false;
	}


	struct v4l2_requestbuffers req;
	memset(&req, 0, sizeof(req));
	req.count  = _maxNumBufs;
        req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory = V4L2_MEMORY_USERPTR;

        /* 获取采集设备驱动的缓存(设备内存) */
        if (ioctl(_CameraFd, VIDIOC_REQBUFS, &req) == -1) {
		std::cout <<"VIDIOC_REQBUFS failed." << strerror(errno) << std::endl;
                return false;
        }

        if (req.count < _maxNumBufs) {
		std::cout << "Insufficient buffer memory" << std::endl;
                return false;
        }

	return true;

}


bool Camera500W::SetVideoFormat()
{
	struct v4l2_format fmt;
	memset(&fmt, 0, sizeof(struct v4l2_format));
	fmt.type			= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width		= _width;
	fmt.fmt.pix.height		= _height;
	fmt.fmt.pix.pixelformat		= V4L2_PIX_FMT_UYVY;
	fmt.fmt.pix.field		= V4L2_FIELD_NONE;
	
	if (ioctl(_CameraFd, VIDIOC_S_FMT, &fmt) == -1) {
		return false;
	}
	
	std::cout << "set format width: " << _width << " height: " << _height << std::endl;
	
	return true;
}


bool Camera500W::GetCaptureBuffer(struct cmemBuffer **pCaptureBufs)
{
	struct v4l2_buffer v4l2buf;
	memset(&v4l2buf, 0, sizeof(v4l2buf));

	v4l2buf.type	= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	v4l2buf.memory	= V4L2_MEMORY_USERPTR;

	if (ioctl(_CameraFd, VIDIOC_DQBUF, &v4l2buf) < 0) {
		
		if (errno == EAGAIN || errno == EINTR) {

			usleep(2000);
			return false;
		}

		std::cout << "VIDIOC_DQBUF error" << errno << strerror(errno) << std::endl;
		return false;
	}
	
	*pCaptureBufs = &_pCmemBuffer[v4l2buf.index];

	return true;
}


bool Camera500W::PutCaptureBuffer(struct cmemBuffer *pCaptureBufs)
{
	struct v4l2_buffer v4l2buf;
	memset(&v4l2buf, 0, sizeof(v4l2buf));

	v4l2buf.type		= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	v4l2buf.memory		= V4L2_MEMORY_USERPTR;
	v4l2buf.index		= pCaptureBufs->index;
	v4l2buf.m.userptr	= pCaptureBufs->vir_addr;
	v4l2buf.length		= pCaptureBufs->size;

	if (ioctl(_CameraFd, VIDIOC_QBUF, &v4l2buf) < 0) {

		std::cout << "VIDIOC_QBUF error" << errno << strerror(errno) << std::endl;
		return false;
	}

	//std::cout << "put an image" << std::endl;

	return true;

}


