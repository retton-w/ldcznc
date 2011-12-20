#include"ldspmil.h"
#include "SHA1.h"

int EmbedAntiCounterfeitInfo(const unsigned char *au_code,  unsigned int au_code_len,
                             const unsigned char *wm_code,  unsigned int wm_code_len,
                             unsigned char *jpeg_buf,
                             unsigned long *jpeg_buf_len)
{
        unsigned int i;
        int len = *jpeg_buf_len;

        if ((au_code == NULL) || (wm_code == NULL)) {
                return -1;
        }
        if (jpeg_buf == NULL) {
                return -1;
        }

        len -= 2;
        /* au code offset is 150 */
        for (i = 0; i < au_code_len; ++i) {
                jpeg_buf[len++] = au_code[i] + 150;
        }

        /* wm code offset is 100 */
        for (i = 0; i < wm_code_len; ++i) {
                jpeg_buf[len++] = wm_code[i] + 100;
        }

        jpeg_buf[len++] = 0xFF;
        jpeg_buf[len++] = 0xD9;

        *jpeg_buf_len = len;

        return 0;
}

int CreateJPGFileHash(unsigned char *jpeg_buf,
                      unsigned long *jpeg_buf_len)
{
        //压缩数据加密
        CSHA1 *pSha1   = NULL;
        int   bCreatOk = 0;

        pSha1 = new CSHA1();
        if (pSha1 == NULL) {
                printf("------------pSha1 is NULL!\n");
                return -1;
        }

        bCreatOk = pSha1->CreateJPGBufferHash(jpeg_buf, jpeg_buf_len);
        if (!bCreatOk) {
                printf("CreateJPGBufferHash failed!\n");

        }



        delete pSha1;

        return bCreatOk;
}
