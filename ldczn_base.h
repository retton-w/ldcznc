#ifndef _BASE_H_
#define _BASE_H_

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief	相机状态
 */
enum {

        NORMAL			= 0x00,         /* 正常				*/
        RADAR_ERROR		= 0x01,         /* 雷达异常			*/
        DETECTOR_ERROR		= 0x02,         /* 信号检测器异常		*/
        COIL_DETECTOR_ERROR	= 0x04,         /* 线圈卡异常(无线圈卡心跳包)	*/
        COIL_ERROR		= 0x08,         /* 线圈异常(线圈短路或断开)	*/
};



/**
 * @brief	图像类型
 */
enum {

	IMAGE_TYPE_BAYER_BG,
	IMAGE_TYPE_BAYER_GB,
	IMAGE_TYPE_BAYER_RG,
	IMAGE_TYPE_BAYER_GR,
	IMAGE_TYPE_JPEG,
	IMAGE_TYPE_BMP,
	IMAGE_TYPE_RGB888,
	IMAGE_TYPE_RGB565,
	IMAGE_TYPE_YUV420,
	IMAGE_TYPE_YUYV,
	IMAGE_TYPE_UYVY,
};


/**
 * @brief	相机图像参数值
 */
typedef struct _camera_param {

	unsigned char	camera_SN[32];		/* 相机序列号			*/
	unsigned int	max_exposure_time;	/* 最大曝光时间			*/
	unsigned int	min_exposure_time;	/* 最小曝光时间			*/
	unsigned int	max_gain;		/* 最大增益			*/
	unsigned int	min_gain;		/* 最小增益			*/
	unsigned int	def_exposure_time;	/* 默认及夜间曝光时间		*/
	unsigned int 	def_gain;		/* 默认及夜间增益		*/
	unsigned int	awb_blue_gain;		/* 蓝色增益初始值		*/
	unsigned int	awb_red_gain;		/* 红色增益初始值		*/
	unsigned int	flash_mode;		/* 闪光灯模式			*/
	unsigned int	max_gray;		/* 最小灰度			*/
	unsigned int	min_gray;		/* 最大灰度			*/
	unsigned int	video_target_gray;	/* 视频目标灰度值		*/
	unsigned int	trigger_target_gray;	/* 抓拍目标灰度值		*/

} camera_param;


/**
 * @brief	相机当前状态信息
 */
typedef struct _camera_status {

	unsigned int	current_exposure_time;	/* 当前快门值		*/
	unsigned int	current_gain;		/* 当前增益值		*/
	unsigned int	flash_status;		/* 闪光灯状态		*/
	unsigned int	awb_blue_gain;		/* 当前蓝色增益值	*/
	unsigned int	awb_red_gain;		/* 当前红色增益值	*/
	unsigned int	current_temperature;	/* 相机当前温度		*/
	unsigned int	contrast;		/* 对比度		*/
	unsigned int	saturation;		/* 饱和度		*/
	unsigned int	black_levle;		/* 黑电平		*/
	unsigned int	acutance;		/* 锐度			*/
	unsigned int	AEW_zone;		/* 2A数据统计区域	*/

} camera_status;


/**
 * @brief	存储器相关信息
 */
typedef struct _storage_info {

	unsigned int	total_capability;	/* 存储器总容量(单位:Mb)*/
	unsigned int	remain_capability;	/* 未使用容量		*/
	unsigned int	unuploaded_info_num;	/* 未上传数据数量	*/

} storage_info;


/**
 * @brief	雷达参数
 */
typedef struct _radar_param {

	unsigned char	radar_SN[32];		/* 雷达序列号		*/
	unsigned char	work_mode;		/* 工作模式(同/反向)	*/
	unsigned char	car_capture_speed;	/* 小车起拍速度		*/
	unsigned char	vehicle_capture_speed;	/* 大车起拍速度		*/
	unsigned char	min_capture_speed;	/* 最低速起拍值		*/
	unsigned char	discard_speed;		/* 超高速丢弃速度值	*/
	unsigned char	radar_delay_time;	/* 雷达延时抓拍时间	*/
	unsigned char	radar_gain;		/* 雷达增益		*/
	unsigned char	radar_angle;		/* 雷达角度		*/
	unsigned char	radar_sense;		/* 雷达同向灵敏度	*/
	unsigned char	radar_reverse_sense;	/* 雷达反向灵敏度	*/
	unsigned char	poweroff_interval;	/* 关机时长		*/

} radar_param;


/**
 * @brief	信号检测器参数
 */
typedef struct _detector_param {

	unsigned char	detector_SN[32];	/* 车检器序列号	*/
	unsigned int	baund_rate;		/* 串口波特率	*/
	unsigned int	date_bits;		/* 数据位	*/
	unsigned int	stop_bits;		/* 停止位	*/
	unsigned int	verifty_bit;		/* 校验位	*/

} detector_param;




/**
 * @brief	网络参数
 */
typedef struct _network_param {
	
	unsigned char	mac_addr[32];		/* MAC地址	*/
	unsigned char	device_ip[32];		/* 设备IP地址	*/
	unsigned char	netmask[32];		/* 子网掩码	*/
	unsigned char	gateway[32];		/* 网关		*/
	unsigned char	dns_server_main[32];	/* DNS服务器1	*/
	unsigned char	dns_server_bak[32];	/* DNS服务器2	*/

} network_param;


/**
 * @brief	抓拍信息上传参数
 *
 * 		卡口,固定点,区间测速可以配置是否传输图片,默认上传信息
 * 		闯红灯默认上传三张图片
 *
 */
typedef struct _upload_param {

	unsigned char	server_ip[128];		/* 服务器ip/域名	*/
	unsigned long	upload_port;		/* 信息上传端口		*/
	unsigned long	control_port;		/* 控制端口		*/
	unsigned char	capture_info_enable;	/* 是否上传抓拍信息 	*/
	unsigned char	image_info_enable;	/* 是否上传抓拍图像	*/

} upload_param;


/**
 * @brief	程序版本号
 */
typedef struct _program_version {

	unsigned char	MCU_version[64];	/* 单片机程序版本 	*/
	unsigned char	FPGA_version[64];	/* FPGA程序版本	  	*/
	unsigned char	DSP_version[64];	/* DSP算法版本	  	*/
	unsigned char	KERN_version[64];	/* 内核版本		*/
	unsigned char	FILESYS_version[64];	/* 文件系统版本		*/
	unsigned char	APP_version[64];	/* 应用程序版本	  	*/

} program_version;


/**
 * @brief	设备状态
 */
typedef struct _device_status {

	camera_status 	status;			
	unsigned int	peripheral_status;	/* 外设状态	   */
	unsigned int	total_passing;		/* 通行车辆总数    */

} device_status;


/**
 * @brief	设备信息
 */
typedef struct _device_param {
	
	unsigned char	device_SN[64];		/* 设备编号 	*/
	unsigned char	device_name[64];	/* 设备名称	*/
	unsigned int	device_model;		/* 设备工作模式 */
	unsigned int	lane;			/* 安装车道 	*/
	unsigned int	car_speed_limit;	/* 小车限速 	*/
	unsigned int	vehicle_speed_limit;	/* 大车限速 	*/
	unsigned int	min_speed_limit;	/* 最小限速 	*/
	unsigned int	capture_limit;		/* 起拍速度	*/
	unsigned char	site[128];		/* 安装地点 	*/
	unsigned char	direction_code[32];	/* 行驶方向代码	*/
	unsigned char	direction_text[128];	/* 行驶方向 	*/
	unsigned char	road_code[32];		/* 路段代码	*/
	unsigned char	department_code[32];	/* 单位代码	*/
	unsigned int	road_kilometer;		/* 路段地点信息	*/
	unsigned int	road_meter;		/* 路段地点信息	*/
	unsigned char	latitue[32];		/* 经度		*/
	unsigned char	longtitue[32];		/* 纬度		*/
	unsigned char	police_code[32];	/* 警员代码	*/
	unsigned char 	reserve[128];		/* 保留字段	*/

} device_param;



/**
 * @brief	车流量信息
 */
typedef struct _traffic_status {
	
	unsigned char	start_time[32];	        /* 统计开始时间			*/
	unsigned char	end_time[32];	        /* 统计结束时间			*/
	unsigned char	camera_SN[32];	        /* 相机ID			*/
	unsigned char   device_SN[32];		/* 设备编号			*/
	unsigned char 	road_code[32];	        /* 路段代码			*/
        unsigned char	drive_direction[128];   /* 行驶方向			*/
	unsigned int	stats_period;	        /* 统计周期(单位:分钟)  	*/
	unsigned int	lane;			/* 通行车道			*/
	unsigned int    total_num;	        /* 统计时间内车辆总数		*/
	unsigned int    avg_len;                /* 平均车长(单位:分米)		*/
	unsigned int    highspeed_num;        	/* 超速车辆数			*/
	unsigned int    lowspeed_num;         	/* 低速车辆数			*/
	unsigned int    bigcar_num;	        /* 大车数量			*/
	unsigned int    midcar_num;	        /* 中型车数量			*/
	unsigned int    smallcar_num;	        /* 小车数量			*/
	unsigned int    motorcar_num;	        /* 摩托车数量			*/
	unsigned int    hugecar_num;	        /* 超长车数量			*/
	float           avg_speed;              /* 平均车速 公里/小时		*/
	float           time_usage;             /* 时间占用率			*/
	float           space_usage;            /* 空间占用率			*/
	float           carnum_per_meter;       /* 车头间距   米/辆		*/
	float           carnum_per_second;      /* 车头时距  秒/辆		*/
	float           carnum_per_kilometer;   /* 每公里的车辆数,车辆密度 	*/

} traffic_status;


/**
 * @var		typedef struct _vehicle_info vehicle_info
 *
 * @brief	抓拍车辆信息
 *
 * 		抓拍方式: 0 - 不区分车头车尾; 1 - 车头; 2 - 车尾
 *
 */
typedef struct _trigger_info {

	unsigned char	snapNbr[32];			/* 抓拍编号		*/
	unsigned char	plateNbr[32];			/* 车牌号码		*/
	unsigned char	plate_type;			/* 车牌类型		*/
	unsigned char	plate_color;			/* 车牌颜色		*/
	unsigned char	vehicle_type;			/* 车辆类型		*/
	unsigned char	lane;				/* 车道			*/
	unsigned int	drive_mode;			/* 通行方式		*/
	unsigned int	vehicle_speed;			/* 目标车速		*/
	unsigned int	vehicle_length;			/* 目标车长		*/
	unsigned int	base_speed;			/* 本车速度		*/
	unsigned int	snap_mode;			/* 抓拍方式(移动、固定)	*/
	unsigned int	snap_verify_ID;			/* 抓拍校验码(图片合成)	*/
	struct timeval	passing_time;			/* 通过时刻		*/
	struct timeval	redlight_time;			/* 红灯开始时间		*/
	unsigned char	offence_type;			/* 违法行为		*/
	void		*reserver;			/* 保留字段		*/

} trigger_info;



/**
 * @brief	抓拍图像信息
 */
struct _vehicle_image {

	unsigned int	image_type;			/* 图像类型	*/
	unsigned int	image_width;			/* 有效像素宽度 */
	unsigned int	image_height;			/* 有效像素高度	*/
	unsigned int	image_stride;			/* 图像宽度	*/
	unsigned int	image_length;			/* 数据长度	*/
	unsigned int	image_gray;			/* 图像灰度	*/
	struct timeval	capture_time;			/* 抓拍时间	*/
	unsigned char	*image_buffer;			/* 数据缓存地址	*/

};


/**
 * @brief	图像信息
 */
typedef struct _vehicle_image_info {

	struct _vehicle_image	vehicle_image[3];	
	unsigned int		frame;			/* 图像帧数	*/

} vehicle_image_info;


/**
 * @brief	抓拍信息
 */
typedef struct _capture_info {

	trigger_info 		*pTrigger_info;		/* 触发信息	*/
	vehicle_image_info	*pVehicle_image_info;	/* 图像信息	*/
	device_param		*pDevice_info;		/* 设备信息	*/

} capture_info;


/**
 * @brief	图像处理参数
 */
typedef struct _PictureParams {

	unsigned char	Jpeg_quality;	/* jpeg压缩比	*/
	unsigned char	brightness;	/* 亮度		*/
	unsigned char	contrast;	/* 对比度	*/
	unsigned char	saturation;	/* 饱和度	*/
	unsigned char	hue;		/* 色调		*/

} PictureParams;


#ifdef __cplusplus
}
#endif

#endif
