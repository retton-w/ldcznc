#ifndef __LDSPMIL_H__
#define __LDSPMIL_H__


#ifdef __cplusplus
extern "C" {
#endif

        /*
         * 参数说明:
         *     au_code      : 认证码
         *     au_code_len  : 认证码长度[1-99]
         *     wm_code      : 防伪码
         *     wm_code_len  : 防伪码长度[1-99]
         *     jpeg_buf     : 存储的JPG数据
         *     jpeg_buf_len : JPG图像数据大小
         * 说明 :
         *     jpeg_buf_cap 必须不小于
         *     (*jpeg_buf_len + au_code_len + wm_code_len)
         */
        int EmbedAntiCounterfeitInfo(const unsigned char *au_code, unsigned int au_code_len,
        const unsigned char *wm_code, unsigned int wm_code_len,
        unsigned char *jpeg_buf,
        unsigned long *jpeg_buf_len);
        /*
         * 参数说明:
         *     jpeg_buf     : 存储的JPG数据
         *     jpeg_buf_len : JPG图像数据大小
         * 说明 :
         *     jpeg_buf_cap 必须不小于
         *     (*jpeg_buf_len + 20)
         */
        int CreateJPGFileHash(unsigned char *jpeg_buf,
                              unsigned long *jpeg_buf_len);

#ifdef __cplusplus
}
#endif

#endif
