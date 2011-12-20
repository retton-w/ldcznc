/*
** img_buf.c
** Login : <frank@frank-desktop>
** Started on  Mon Jul 27 15:20:17 2009 frank
** $Id$
**
** Copyright (C) 2009 frank
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "image_buffer.h"
#include "buf_list.h"


static struct env_image_buffer env_image_buffer;

static class BufList *image_buffer_list     = NULL; 
static class BufList *capture_buffer_list   = NULL; 
static class BufList *transport_buffer_list = NULL;
static pthread_mutex_t mutex;


int  image_buffer_init(struct env_image_buffer *env)
{
        image_buffer_list     = new BufFilo(
                //env->buf_len + sizeof(struct image_buffer_description),
                env->buf_len,
                env->buf_num);

        capture_buffer_list   = new BufFifo(0, 0);
        transport_buffer_list = new BufFifo(0, 0);

        pthread_mutex_init(&mutex, NULL);
        memcpy(&env_image_buffer, env, sizeof(*env));
        return 0;
}

int image_buffer_deinit(void)
{
        if (image_buffer_list) {
                delete image_buffer_list;
                image_buffer_list = NULL;
        }
        if (capture_buffer_list) {
                delete capture_buffer_list;
                capture_buffer_list = NULL;
        }
        if (transport_buffer_list) {
                delete transport_buffer_list;
                transport_buffer_list = NULL;
        }
        pthread_mutex_destroy(&mutex);
        return 0;
}

/* capture buffer operation */
int  capture_buffer_alloc(struct image_buffer_description **desc)
{
        void *buffer = NULL;
        struct image_buffer_description *d = NULL;
        pthread_mutex_lock(&mutex);
        if ((image_buffer_list->get_len() > env_image_buffer.resd_buf_num_max) &&
            (capture_buffer_list->get_len() < env_image_buffer.capt_buf_num_max)) {
			
			//printf("capture_buffer_alloc length is %d\n", image_buffer_list->get_len());
			buffer = image_buffer_list->GetBuffer();
        }
        pthread_mutex_unlock(&mutex);

        if (buffer == NULL) {
                return -1;
        }
        d = (struct image_buffer_description *)buffer;
        d->info.vir_addr = (unsigned int)buffer + sizeof(*d);
        *desc = d;

        return 0;
}

int  capture_buffer_free(struct image_buffer_description *desc)
{
        int ret = 0;
        pthread_mutex_lock(&mutex);
        ret = image_buffer_list->PutBuffer((void *)desc);
        pthread_mutex_unlock(&mutex);
        return ret;
}

int  capture_buffer_get(struct image_buffer_description **desc)
{
        void *buffer = NULL;
        struct image_buffer_description *d = NULL;
        pthread_mutex_lock(&mutex);
        buffer = capture_buffer_list->GetBuffer();
        pthread_mutex_unlock(&mutex);

        if (buffer == NULL) {
                return -1;
        }
        d = (struct image_buffer_description *)buffer;
        d->info.vir_addr = (unsigned int)buffer + sizeof(*d);
        *desc = d;

        return 0;
}

int capture_buffer_put(struct image_buffer_description *desc)
{
        int ret = 0;
        pthread_mutex_unlock(&mutex);
        ret = capture_buffer_list->PutBuffer((void *)desc);
        pthread_mutex_unlock(&mutex);
        return ret;
}

/* transport buffer operation */
int transport_buffer_alloc(struct image_buffer_description **desc)
{
        void *buffer = NULL;
        struct image_buffer_description *d = NULL;
        pthread_mutex_lock(&mutex);
        if ((image_buffer_list->get_len() > env_image_buffer.resd_buf_num_max) &&
            (transport_buffer_list->get_len() < env_image_buffer.tran_buf_num_max)) {
                buffer = image_buffer_list->GetBuffer();
        }
        pthread_mutex_unlock(&mutex);

        if (buffer == NULL) {
                return -1;
        }
        d = (struct image_buffer_description *)buffer;
        d->info.vir_addr = (unsigned int)buffer + sizeof(*d);
        *desc = d;

        return 0;
}

int  transport_buffer_free(struct image_buffer_description *desc)
{
        int ret = 0;
        pthread_mutex_lock(&mutex);
        ret = image_buffer_list->PutBuffer((void *)desc);
        pthread_mutex_unlock(&mutex);
        return ret;
}

int transport_buffer_get(struct image_buffer_description **desc)
{
        void *buffer = NULL;
        struct image_buffer_description *d = NULL;
        pthread_mutex_lock(&mutex);
        buffer = transport_buffer_list->GetBuffer();
        pthread_mutex_unlock(&mutex);

        if (buffer == NULL) {
                return -1;
        }
        d = (struct image_buffer_description *)buffer;
        d->info.vir_addr = (unsigned int)buffer + sizeof(*d);
        *desc = d;

        return 0;
}

int transport_buffer_put(struct image_buffer_description *desc)
{
        int ret = 0;
        pthread_mutex_lock(&mutex);
        ret = transport_buffer_list->PutBuffer((void *)desc);
        pthread_mutex_unlock(&mutex);
        return ret;
}

/*
 * return value :
 *     -1 : length smaller than minimal length
 *     1  : length larger than maximal length
 *     0
 */
int  transport_buffer_get_level(void)
{
        int len = 0;
        int level = 0;
        pthread_mutex_lock(&mutex);
        len = transport_buffer_list->get_len();
        if (len < env_image_buffer.tran_buf_num_min) {
                level = -1;
        } else if (len > env_image_buffer.tran_buf_num_max) {
                level = 1;
        } else {
                level = 0;
        }
        pthread_mutex_unlock(&mutex);

        return level;
}


