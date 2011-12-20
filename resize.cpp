#include "resize.h"

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

#include <media/davinci/videohd.h>
#include <media/davinci/vpfe_capture.h>
#include <media/davinci/dm365_ccdc.h>
#include <media/davinci/dm365_ipipe.h>
#include <media/davinci/imp_previewer.h>
#include <media/davinci/imp_resizer.h>


#define RESIZER_DEVICE   "/dev/davinci_resizer"


Resize_Handle Resize::ResizeCreate()
{

	Resize_Handle hResize;
        unsigned long oper_mode;

	hResize = (Resize_Handle)calloc(1, sizeof(Resize_Object));
	if (hResize == NULL) {
                printf("Failed to allocate space for Framecopy Object\n");
                return NULL;
        }

	/* Open resizer device */
        hResize->fd = open(RESIZER_DEVICE, O_RDWR);

        if (hResize->fd == -1) {
                printf("Failed to open %s\n", RESIZER_DEVICE);
                return NULL;
        }

	oper_mode = IMP_MODE_SINGLE_SHOT;
	if (ioctl(hResize->fd,RSZ_S_OPER_MODE, &oper_mode) < 0) {
                printf("Resizer can't set operation mode\n");
                goto fac_error;
        }
	
	if (ioctl(hResize->fd,RSZ_G_OPER_MODE, &oper_mode) < 0) {
                printf("Resizer can't get operation mode\n");
                goto fac_error;
        }

	if (oper_mode != IMP_MODE_SINGLE_SHOT) {
                printf("Resizer can't set operation mode single shot, the mode is %lu\n", oper_mode);
                goto fac_error;
        }

	return hResize;

fac_error:
	close(hResize->fd);
	hResize->fd = 0;
	return NULL;

}
	

int Resize::ResizeDelete(Resize_Handle handle)
{

        if (handle) {
		
                close(handle->fd);
                free(handle);
        }

        return 0;
}


int Resize::ResizeConfig(Resize_Handle handle,
			struct cmemBuffer *source_buf,
			struct cmemBuffer *destination_buf,
			struct cmemBuffer *destination_buf2)
{
	
	unsigned int                  hDif;
	unsigned int                  vDif;
	struct rsz_channel_config     rsz_chan_config;
        struct rsz_single_shot_config rsz_ss_config;

	/* Make sure our input parameters are valid */
        if (!handle) {
                printf("Resize_Handle parameter must not be NULL\n");
                return -1;
        }

	if (!source_buf) {
                printf("Source buffer parameter must not be NULL\n");
                return -1;
        }

	if (!destination_buf) {
                printf("Destination buffer parameter must not be NULL\n");
                return -1;
        }

	if (destination_buf->width <= 0) {
                printf("Destination buffer width must be greater than zero\n");
                return -1;
        }

	if (destination_buf->height <= 0) {
                printf("Destination buffer height must be greater than zero\n");
                return -1;
        }

	if ((source_buf->stride & 0x1F) != 0) {
                printf("Source buffer pitch must be a multiple of 32 bytes\n");
                return -1;
        }

        if ((destination_buf->stride & 0x1F) != 0) {
                printf("Destination buffer pitch must be a multiple of 32 bytes\n");
                return -1;
        }

	/* Check for valid buffer scaling */
        hDif = source_buf->width  * 256 / destination_buf->width;
        vDif = source_buf->height * 256 / destination_buf->height;

        if (hDif < 32) {
                printf("Horizontal up-scaling must not exceed 8x\n");
                return -1;
        }

        if (hDif > 4096) {
                printf("Horizontal down-scaling must not exceed 1/16x\n");
                return -1;
        }

        if (vDif < 32) {
                printf("Vertical up-scaling must not exceed 8x\n");
                return -1;
        }

        if (vDif > 4096) {
                printf("Vertical down-scaling must not exceed 1/16x\n");
                return -1;
        }

	/* Setting default configuration in Resizer */
        memset(&rsz_ss_config, 0, sizeof(rsz_ss_config));
        rsz_chan_config.oper_mode = IMP_MODE_SINGLE_SHOT;
        rsz_chan_config.chain     = 0;
        rsz_chan_config.len       = 0;
        rsz_chan_config.config = NULL; /* to set defaults in driver */
        if (ioctl(handle->fd, RSZ_S_CONFIG, &rsz_chan_config) < 0) {
                printf("Error in setting default configuration for single shot mode\n");
                goto faco_error;
        }
	
	/* default configuration setting in Resizer successfull */
        memset(&rsz_ss_config, 0, sizeof(rsz_ss_config));
        rsz_chan_config.oper_mode = IMP_MODE_SINGLE_SHOT;
        rsz_chan_config.chain     = 0;
        rsz_chan_config.len       = sizeof(rsz_ss_config);
        rsz_chan_config.config    = &rsz_ss_config;
        if (ioctl(handle->fd, RSZ_G_CONFIG, &rsz_chan_config) < 0) {
                printf("Error in getting resizer channel configuration from driver\n");
                goto faco_error;
        }

	/* input params are set at the resizer */
        //rsz_ss_config.input.pix_fmt      = IMP_UYVY;
        rsz_ss_config.input.pix_fmt      = IPIPE_UYVY;
        rsz_ss_config.input.image_width  = source_buf->width;
        rsz_ss_config.input.image_height = source_buf->height;//rsz_ss_config.input.image_height;
        rsz_ss_config.input.ppln         = rsz_ss_config.input.image_width + 8;
        rsz_ss_config.input.lpfr         = rsz_ss_config.input.image_height + 10;
	rsz_ss_config.input.line_length  = source_buf->line;

	rsz_ss_config.output1.enable     = 1;
        //rsz_ss_config.output1.pix_fmt    = IMP_YUV420SP;
        //rsz_ss_config.output1.pix_fmt    = IMP_UYVY;
        rsz_ss_config.output1.pix_fmt    = IPIPE_UYVY;
        rsz_ss_config.output1.width      = destination_buf->width;
        rsz_ss_config.output1.height     = destination_buf->height;
	
	if (destination_buf2 == NULL) {
                rsz_ss_config.output2.enable     = 0;
        } else {
                rsz_ss_config.output2.enable     = 1;
                //rsz_ss_config.output2.pix_fmt    = IMP_YUV420SP;
                rsz_ss_config.output2.pix_fmt    = IPIPE_YUV420SP;
                rsz_ss_config.output2.width      = destination_buf2->width;
                rsz_ss_config.output2.height     = destination_buf2->height;
        }

	rsz_chan_config.oper_mode = IMP_MODE_SINGLE_SHOT;
        rsz_chan_config.chain     = 0;
        rsz_chan_config.len       = sizeof(rsz_ss_config);
        if (ioctl(handle->fd, RSZ_S_CONFIG, &rsz_chan_config) < 0) {
                printf("Error in setting default configuration for single shot mode\n");
                goto faco_error;
        }

	/* read again and verify */
        rsz_chan_config.oper_mode = IMP_MODE_SINGLE_SHOT;
        rsz_chan_config.chain     = 0;
        rsz_chan_config.len       = sizeof(rsz_ss_config);
        if (ioctl(handle->fd, RSZ_G_CONFIG, &rsz_chan_config) < 0) {
                printf("Error in getting configuration from driver\n");
                goto faco_error;
        }

	return 0;

faco_error:
        close(handle->fd);
        handle->fd = 0;
        return -1;

}


int Resize::ResizeExecute(Resize_Handle handle,
			struct cmemBuffer *source_buf,
			struct cmemBuffer *destination_buf,
			struct cmemBuffer *destination_buf2)
{
	struct imp_convert   rsz;

	memset(&rsz, 0, sizeof(rsz));
	rsz.in_buff.index     = -1;
	rsz.in_buff.buf_type  = IMP_BUF_IN;
	rsz.in_buff.offset    = source_buf->vir_addr + source_buf->offset;	//add offset
	rsz.in_buff.size      = source_buf->height * source_buf->line;

	rsz.out_buff1.index    = -1;
	rsz.out_buff1.buf_type = IMP_BUF_OUT1;
	rsz.out_buff1.offset   = destination_buf->vir_addr + destination_buf->offset;	//add offset
	rsz.out_buff1.size     = destination_buf->height * destination_buf->line;
	if (destination_buf2 != NULL) {
                rsz.out_buff2.index    = -1;
                rsz.out_buff2.buf_type = IMP_BUF_OUT2;
                rsz.out_buff2.offset   = destination_buf2->vir_addr;
                rsz.out_buff2.size     = destination_buf2->height * destination_buf2->stride;
        }

	/*
         * The IPIPE requires that the memory offsets of the input and output
         * buffers start on 32-byte boundaries.
         */
        if ((rsz.in_buff.offset  & 0x1F) != 0) {
                printf("in offset\n");
                return -1;
        }

        if ((rsz.out_buff1.offset & 0x1F) != 0) {
                printf("out offset\n");
                return -1;
        }

        if ((rsz.out_buff2.offset & 0x1F) != 0) {
                printf("out offset\n");
                return -1;
        }

	/* Start IPIPE operation */
        if (ioctl(handle->fd, RSZ_RESIZE, &rsz) == -1) {
                printf("Failed RSZ_RESIZE\n");
                return -1;
        }

        return 0;

}

