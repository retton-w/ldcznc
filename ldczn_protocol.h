#ifndef	_LDCZN_PROTOCOL_H_
#define	_LDCZN_PROTOCOL_H_

#define PROTOCOL_MAGIC {'S', 'Z', 'L', 'D'}

#define PROTOCOL_MAJOR 0
#define PROTOCOL_MINOR 1


/**
 * @brief	数据信息打包方式
 */
enum ENCODEING_TYPE {
        ENCODING_TYPE_RAW    = 0,
        ENCODING_TYPE_PB     = 1, /* Google Protocol Buffer */
        ENCODING_TYPE_XML    = 2
};


/**
 * @brief	数据包类型
 */
enum MESSAGE_TYPE {
        MESSAGE_TYPE_REQ = 0, /* 请求 */
        MESSAGE_TYPE_ACK = 1  /* 应答 */
};


/**
 * @brief	传输类型
 */
enum TRANSPORT_TYPE {

        TRANSPORT_TYPE_UDP = 0, /* UDP */
        TRANSPORT_TYPE_TCP = 1, /* TCP */
};


#define REQ_TYPE_MASK     0xFF000000
#define REQ_TYPE_SUB_MASK 0x00FF0000
#define REQ_TYPE_CMD_MASK 0x0000FFFF


enum REQ_TYPE {

        REQ_TYPE_HEARTBEAT   = 0x01000000, /* 心跳	*/
        REQ_TYPE_RUNNING     = 0x02000000, /* 运行	*/
        REQ_TYPE_MANUFACTURE = 0x03000000, /* 生产	*/
        REQ_TYPE_HW_DEBUG    = 0x04000000, /* 硬件调试	*/
        REQ_TYPE_SW_DEBUG    = 0x05000000, /* 软件调试	*/
        REQ_TYPE_V_DETETC    = 0x06000000, /* 视频分析	*/
        REQ_TYPE_IMAGE       = 0x07000000, /* 图像	*/
	REQ_TYPE_PLATE	     = 0x08000000, /* 车牌参数	*/
	REQ_TYPE_PERIPHERRAL = 0x09000000, /* 外设参数	*/
	REQ_TYPE_PROCESSIMAGE= 0x0A000000, /* 图像处理	*/
	REQ_TYPE_SEARCH	     = 0x0B000000, /* 设备查找  */
};


enum REQ_IMG_ {

        REQ_IMG_START        = 0x00010000, /* 使能视频传输 */
        REQ_IMG_STOP         = 0x00020000, /* 停止视频传输 */
        REQ_IMG_TRANS        = 0x00030000, /* 图像传输 */
        REQ_IMG_GPARM        = 0x00040000, /* 获取图像参数 */
        REQ_IMG_SPARM        = 0x00050000, /* 设置图像参数 */
        REQ_IMG_SMODE        = 0x00060000, /* 设置工作模式 */
        REQ_IMG_AEWMODE      = 0x00070000, /* 设置AEW工作模式 */
        REQ_IMG_SNAP         = 0x00080000, /* 内触发抓拍 */
};


/**
 * @brief	连接相机
 */
enum REQ_RUN_ {

	REQ_RUN_CONNECT		= 0x00000001,	/* 跟相机建立连接 */
	REQ_RUN_DISCON		= 0x00000002,	/* 跟相机断开连接 */
};


/**
 * @brief	相机工作模式(抓拍/视频)
 */
enum REQ_IMG_MODE {
        REQ_IMG_MODE_LIVE    = 0x00000001, /* LIVE 模式 */
        REQ_IMG_MODE_TRIG    = 0x00000002, /* TRIG 模式 */
};


enum {
        AEWMODE_GAIN  = 0x00000001, /* 调整增益 */
        AEWMODE_EXP   = 0x00000002, /* 调整曝光 */
};


/**
 * @brief	AEW工作模式
 */
enum REQ_IMG_AEWMODE_ {

        REQ_IMG_AEWMODE_DISABLE = 0,
        REQ_IMG_AEWMODE_GAIN    = AEWMODE_GAIN,
        REQ_IMG_AEWMODE_EXP     = AEWMODE_EXP,
        REQ_IMG_AEWMODE_AUTO    = AEWMODE_GAIN | AEWMODE_EXP,
};


/**
 * @brief	图像参数
 */
enum REQ_IMG_PARM {

        REQ_IMG_PARM_EXP     = 0x00000001, /* 曝光时间		*/
	REQ_IMG_PARM_EXP_MIN = 0x00000002, /* 曝光下限		*/
	REQ_IMG_PARM_EXP_MAX = 0x00000003, /* 曝光上限		*/
        REQ_IMG_PARM_GAIN    = 0x00000004, /* 增益		*/
	REQ_IMG_PARM_GAIN_MIN= 0x00000005, /* 增益下限		*/
	REQ_IMG_PARM_GAIN_MAX= 0x00000006, /* 增益上限		*/
        REQ_IMG_PARM_WB_R    = 0x00000007, /* 白平衡增益 : R 	*/
        REQ_IMG_PARM_WB_B    = 0x00000008, /* 白平衡增益 : B 	*/
	REQ_IMG_PARM_V_TARGET= 0x00000009, /* 视频目标灰度	*/
	REQ_IMG_PARM_T_TARGET= 0x0000000A, /* 触发目标灰度	*/
	REQ_IMG_PARM_AEZONE  = 0x0000000B, /* AE调整区域	*/

};


enum REQ_PERIPHERRAL_ {

	REQ_PERIPHERRAL_FLASH_OFF	= 0x00000000, /* 关闪光灯	*/
	REQ_PERIPHERRAL_FLASH_ON	= 0x00000001, /* 开闪光灯	*/
	REQ_PERIPHERRAL_FLASH_DELAY	= 0x00000002, /* 闪光灯提前量	*/
	REQ_PERIPHERRAL_LED_OFF		= 0x00000003, /* 关LED补光灯	*/
	REQ_PERIPHERRAL_LED_ON		= 0x00000004, /* 开LED补光灯	*/
	REQ_PERIPHERRAL_LED_RATE	= 0x00000005, /* LED灯倍频值	*/
	REQ_PERIPHERRAL_LED_TIME	= 0x00000006, /* LED灯占空比	*/
	REQ_PERIPHERRAL_SNAP_TIMES	= 0x00000007, /* 连拍图像张数	*/
	REQ_PERIPHERRAL_SNAP_DURATION	= 0x00000008, /* 连拍时间间隔	*/
};


enum REQ_MAN_ {

        REQ_MAN_FMT          = 0x00010000, /* 格式化硬盘 */
        REQ_MAN_UPG          = 0x00020000, /* 更新 */
        REQ_MAN_NPRF         = 0x00030000, /* 获取参数个数 */
        REQ_MAN_GPRF         = 0x00040000, /* 获取参数 */
        REQ_MAN_SPRF         = 0x00050000, /* 设置参数 */
        REQ_MAN_REBOOT       = 0x00060000, /* 重启 */
        REQ_MAN_RESET        = 0x00070000, /* 恢复出厂设置 */
        REQ_MAN_COMP_TIME    = 0x00080000, /* 对时 */
        REQ_MAN_CLR          = 0x00090000, /* 清除分区 */
};


enum REQ_MAN_UPG_ {

        REQ_MAN_UPG_BACK     = 0x00000001, /* 备份分区 */
        REQ_MAN_UPG_APP      = 0x00000002, /* 应用程序 */
        REQ_MAN_UPG_PREF     = 0x00000003, /* 配置文件 */
        REQ_MAN_UPG_DRV      = 0x00000004, /* 设备驱动 */
        REQ_MAN_UPG_ALG_MOD  = 0x00000005, /* DSP算法支持模块 */
        REQ_MAN_UPG_LIB      = 0x00000006, /* 共享库 */
        REQ_MAN_UPG_ALG      = 0x00000007, /* DSP算法 */
        REQ_MAN_UPG_MON      = 0x00000008, /* 监控程序 */
        REQ_MAN_UPG_MCU      = 0x00000009, /* MCU固件程序 */
        REQ_MAN_UPG_FPGA     = 0x0000000A, /* 清除分区 */
};


enum REQ_MAN_CLR_ {

        REQ_MAN_CLR_BACK     = 0x00000001, /* 清除备份分区 */
        REQ_MAN_CLR_DATA     = 0x00000002, /* 清除数据分区 */
};


/* 升级文件类型 */
enum UPG_FILE_TYPE {
        UPG_FILE_TYPE_STANDALONE = 0,
        UPG_FILE_TYPE_TAR        = 1,
        UPG_FILE_TYPE_TGZ        = 2,
};


enum ACK_STATUS {

        ACK_SUCCESS          = 0x00000000, /* 成功 */
        ACK_FAILED           = 0x80000000, /* 失败 */
};


#pragma pack(push)
#pragma pack(4)

struct header_std {
        char         magic[4];       /* "SZLD" */
        char         protocol_major; /* 0 */
        char         protocol_minor; /* 1 */
        char         encoding;
        char         auth_code[16];
        char         msg_type;
        unsigned int msg_size;
};


struct payload_req {
        unsigned int id;   /* 请求ID */
        unsigned int type; /* 请求类型 */
};


struct payload_ack {
        unsigned int id;   /* 请求ID */
        unsigned int type; /* 请求类型 */
        unsigned int sts;  /* 请求状态 */
};


struct payload_heartbeat {

        char           ser_nbr[33];
        unsigned char  transport_type;
        unsigned short data_port;
        unsigned short heartbeat_period;
};


struct packet_heartbeat {

        struct header_std        head;
        struct payload_req       req;
        //struct payload_heartbeat hb;
};


struct payload_search {

	char	serial_number[32];	/* 相机序列号					*/
	char	site[128];		/* 安装地点(便于后台多台相机时，区分设备)	*/
};


struct packet_search_req {

	struct header_std	head;
	struct payload_req	req;
};


struct packet_search_ack {

	struct header_std	head;
	struct payload_ack	ack;
	struct payload_search	search;
};


struct packet_running_req {

	struct header_std	head;
	struct payload_req	req;
};


struct packet_runnng_ack {

	struct header_std	head;
	struct payload_ack	ack;
};


struct packet_man_reboot_req {

        struct header_std      head;
        struct payload_req     req;
};


struct packet_man_reboot_ack {
        struct header_std      head;
        struct payload_ack     ack;
};


struct payload_man_upgrade {
        unsigned short         listen_port;
        unsigned int           total_length;
        unsigned short         file_type;
        char                   file_name[33];
};


struct packet_man_upgrade_req {
        struct header_std          head;
        struct payload_req         req;
        struct payload_man_upgrade upg;
};


struct packet_man_upgrade_ack {
        struct header_std       head;
        struct payload_ack      ack;
};


struct packet_man_fmt_req {
        struct header_std       head;
        struct payload_req      req;
};


struct packet_man_fmt_ack {
        struct header_std       head;
        struct payload_ack      ack;
};


struct packet_man_clr_req {
        struct header_std       head;
        struct payload_req      req;
};

struct packet_man_clr_ack {
        struct header_std       head;
        struct payload_ack      ack;
};

struct payload_man_pref {
        int  index;
        char name[32];
        char value[32];
};

struct packet_man_pref_ack {
        struct header_std       head;
        struct payload_ack      ack;
        struct payload_man_pref pref;
};

struct packet_man_pref_req {
        struct header_std       head;
        struct payload_req      req;
        struct payload_man_pref pref;
};

struct ldczn_time {
        unsigned short year; /* 0 ~ 65535 */
        unsigned char  mon;  /* 1 ~ 12 */
        unsigned char  day;  /* 1 ~ 31 */
        unsigned char  hour; /* 0 ~ 23 */
        unsigned char  min;  /* 0 ~ 59 */
        unsigned char  sec;  /* 0 ~ 59 */
        unsigned char  msec; /* 0 ~ 999 */
        unsigned char  wday; /* 0 ~ 6 */
};

struct packet_man_comp_time_req {
        struct header_std        head;
        struct payload_req       req;
        struct ldczn_time        time;
};

struct packet_man_comp_time_ack {
        struct header_std        head;
        struct payload_ack       ack;
};

struct payload_img_start {
        unsigned short port;
};

struct packet_img_start_req {
        struct header_std        head;
        struct payload_req       req;
        struct payload_img_start start;
};

struct packet_img_start_ack {
        struct header_std        head;
        struct payload_ack       ack;
};

struct packet_img_stop_req {
        struct header_std        head;
        struct payload_req       req;
};

struct packet_img_stop_ack {
        struct header_std        head;
        struct payload_ack       ack;
};

struct payload_img_trans {
        unsigned int info_len;
        unsigned int data_len;
};

struct packet_img_trans_req {
        struct header_std        head;
        struct payload_req       req;
        struct payload_img_trans trans;
};

struct packet_img_trans_ack {
        struct header_std        head;
        struct payload_ack       req;
};

struct payload_img_parm {
        unsigned int value;
};

struct packet_img_sparm_req {
        struct header_std        head;
        struct payload_req       req;
        struct payload_img_parm  parm;
};

struct packet_img_sparm_ack {
        struct header_std        head;
        struct payload_ack       ack;
};

struct packet_img_gparm_req {
        struct header_std        head;
        struct payload_req       req;
};

struct packet_img_gparm_ack {
        struct header_std        head;
        struct payload_ack       ack;
        struct payload_img_parm  parm;
};


struct payload_peripherral_parm {
	unsigned int value;
};


struct packet_peripherral_sparm_req {
	struct header_std		head;
	struct payload_req		req;
	struct payload_peripherral_parm param;
};


struct packet_peripherral_sparm_ack {
	struct header_std		head;
	struct payload_ack		ack;
};


struct packet_peripherral_gparm_req {
	struct header_std		head;
	struct payload_req		req;
};


struct packet_peripherral_gparam_ack {
	struct header_std		head;
	struct payload_ack		ack;
	struct payload_peripherral_parm parm;
};

#pragma pack(pop)

static inline void fill_std_header(struct header_std *head,
                                   char encoding,
                                   char *auth_code,
                                   char msg_type,
                                   unsigned int msg_size)
{
        const char magic[] = PROTOCOL_MAGIC;
        memcpy(head->magic, magic, sizeof(magic));
        head->protocol_major = PROTOCOL_MAJOR;
        head->protocol_minor = PROTOCOL_MINOR;
        head->encoding       = encoding;
        memcpy(head->auth_code, auth_code, sizeof(head->auth_code));
        head->msg_type       = msg_type;
        head->msg_size       = msg_size;
}

//extern unsigned int get_req_id(void);

#endif 	    /* !LDCZN_PROTOCOL_H_ */
