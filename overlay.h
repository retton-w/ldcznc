#ifndef  OVERLAY_H_
#define  OVERLAY_H_

#ifdef __cplusplus
extern "C" {
#endif

//#include "common.h"
#include "ce_buffer.h"


/**
* @brief	freetype字符叠加参数	
*/
struct overlay_font {
        const char          *name;    /* font name */
        int                 size;     /* font size */
};

/**
 * @brief	文字叠加结构体
 */
struct overlay_text {

        const char          *text;	/* text to overlay */
        int                 x;		/* x position */
        int                 y;		/* y position */
};


/**
 * @fn		int overlay_init(struct overlay_font *font)
 *
 * @brief	初始化字符叠加库
 *
 * @param	[in] font	叠加字符参数
 *
 * @return	
 */
int overlay_init(struct overlay_font *font);



/**
 * @fn		int overlay_deinit(void)
 *
 * @brief	释放字符叠加资源
 *
 * @return	
 */
int overlay_deinit(void);



/**
 * @fn		int overlay_multi(sturct _ImageBuffer *info,
 * 				  const struct overlay_text *text,
 * 				  int text_num);
 *
 * @param	info			要叠加的图像数据
 * @param	[in]	text		要叠加的文本信息
 * @param	[in] 	text_num	叠加文本的个数
 *
 * @return	
 */
int overlay_multi(struct cmemBuffer *info, const struct overlay_text *text, int text_num);


#ifdef __cplusplus
}
#endif

#endif

