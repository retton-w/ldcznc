#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include "gpio.h"

#define FPGA_CTL_DEVICE "/dev/cfpga"
#define MANUAL_SNAP 0x4F01


int GpioCtl::MannualSnap()
{
	int fpga_ctl_handle = open(FPGA_CTL_DEVICE, O_RDWR);

	if (fpga_ctl_handle < 0) {

		printf("func is %s, open dm368 io to trig fpga failed : %s\n", __func__, strerror(errno));
		return -1;
	}

	if (ioctl(fpga_ctl_handle, MANUAL_SNAP, NULL) < 0) {

		printf("func is %s, set trig fpga failed : %s\n", __func__, strerror(errno));
		close(fpga_ctl_handle);
		return -1;
	}

	close(fpga_ctl_handle);
	return 0;
}


