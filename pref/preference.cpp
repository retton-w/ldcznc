
#include <unistd.h>
#include "pref.h"
#include "preference_items.h"
#include "preference.h"
#include "xmlParser.h"

#define INI_FILE_NAME    "/more/param/ParaSet.xml"


#define SEC_MAIN		"MAIN"
#define KEY_DEVICE_NUM		"DeviceNumber"		/* 设备编号		*/
#define KEY_DEVICE_MODE		"DeviceMode"		/* 设备工作模式 	*/
#define KEY_DEVICE_SITE		"DeviceSite"		/* 设备安装地点 	*/
#define KEY_DIRECTION_CODE	"DirectionCode"		/* 行驶方向代码 	*/
#define KEY_DIRECTION_TEXT	"DirectionText"		/* 行驶方向		*/
#define KEY_ROAD_CODE		"RoadCode"		/* 路段代码		*/
#define KEY_DEPARTMENT_CODE	"DepartmentCode"	/* 所属部门代码 	*/
#define KEY_LATITUE		"Latitue"		/* 经度			*/
#define KEY_LONGTITUE		"Longtitue"		/* 纬度			*/
#define KEY_LANE		"Lane"			/* 车道			*/
#define KEY_STATSPERIOD		"StatsPeriod"		/* 统计周期		*/
#define KEY_NTP_SERVER		"NtpServer"		/* NTP对时服务器地址 	*/


static inline int preference_init_main(struct param_main *p)
{
        p->DeviceNumber		= new pref_string(SEC_MAIN, KEY_DEVICE_NUM, "LDCZNC001", 32);
        p->DeviceMode		= new pref_int(SEC_MAIN, KEY_DEVICE_MODE, 6);
        p->DeviceSite		= new pref_string(SEC_MAIN, KEY_DEVICE_SITE, "深南大道", 128);
        p->DirectionCode	= new pref_int_range(SEC_MAIN, KEY_DIRECTION_CODE, 1, 8, 1);
	p->DirectionText	= new pref_string(SEC_MAIN, KEY_DIRECTION_TEXT, "由东向西", 32);
	p->RoadCode		= new pref_string(SEC_MAIN, KEY_ROAD_CODE, "000001", 32);
	p->DepartmentCode	= new pref_string(SEC_MAIN, KEY_DEPARTMENT_CODE, "53000000", 32);
	p->Latitue		= new pref_string(SEC_MAIN, KEY_LATITUE, "104.000000", 32);
	p->Longtitue		= new pref_string(SEC_MAIN, KEY_LONGTITUE, "25.000000", 32);
	p->Lane			= new pref_int_range(SEC_MAIN, KEY_LANE, 0, 9, 1);
	p->StatsPeriod		= new pref_int(SEC_MAIN, KEY_STATSPERIOD, 5);
	p->NtpServer		= new pref_string(SEC_MAIN, KEY_NTP_SERVER, "192.168.0.222", 64);

        return 0;
}


static inline int preference_deinit_main(struct param_main *p)
{
        delete p->DeviceNumber;
        delete p->DeviceMode;
        delete p->DeviceSite;
        delete p->DirectionCode;
        delete p->DirectionText;
        delete p->RoadCode;
        delete p->DepartmentCode;
        delete p->Latitue;
        delete p->Longtitue;
        delete p->Lane;
        delete p->StatsPeriod;
        delete p->NtpServer;

        return 0;
}


static inline int preference_load_main(struct param_main *p, XMLNode &root_node)
{
	p->DeviceNumber->load(root_node);
        p->DeviceMode->load(root_node);
        p->DeviceSite->load(root_node);
        p->DirectionCode->load(root_node);
        p->DirectionText->load(root_node);
        p->RoadCode->load(root_node);
        p->DepartmentCode->load(root_node);
        p->Latitue->load(root_node);
        p->Longtitue->load(root_node);
        p->Lane->load(root_node);
        p->StatsPeriod->load(root_node);
        p->NtpServer->load(root_node);

        return 0;
}


static inline int preference_save_main(struct param_main *p, XMLNode &root_node)
{
	p->DeviceNumber->save(root_node);
        p->DeviceMode->save(root_node);
        p->DeviceSite->save(root_node);
        p->DirectionCode->save(root_node);
        p->DirectionText->save(root_node);
        p->RoadCode->save(root_node);
        p->DepartmentCode->save(root_node);
        p->Latitue->save(root_node);
        p->Longtitue->save(root_node);
        p->Lane->save(root_node);
        p->StatsPeriod->save(root_node);
        p->NtpServer->save(root_node);

        return 0;
}


#define SEC_UPLOAD		"UPLOAD"
#define KEY_UPLOAD_METHOD	"UploadMethod"		/* 数据上传方式		*/
#define	KEY_UPLOAD_SERVER	"UploadServer"		/* 数据上传服务器	*/
#define KEY_UPLOAD_PORT		"UploadPort"		/* 数据上传端口		*/
#define KEY_UPLOAD_USER		"UploadUser"		/* ftp登陆用户名	*/
#define KEY_UPLOAD_PASSWD	"UploadPasswd"		/* ftp登陆密码		*/
#define KEY_UPLOAD_TYPE		"UploadType"		/* ftp上传类型		*/


static inline int preference_init_upload(struct param_upload *p)
{
	p->UploadMethod		= new pref_int_range(SEC_UPLOAD, KEY_UPLOAD_METHOD, 0, 1, 0);
        p->UploadServer		= new pref_string(SEC_UPLOAD, KEY_UPLOAD_SERVER, "192.168.0.222", 64);
	p->UploadPort		= new pref_int(SEC_UPLOAD, KEY_UPLOAD_PORT, 21);
        p->UploadUser		= new pref_string(SEC_UPLOAD, KEY_UPLOAD_USER, "", 32);
	p->UploadPasswd		= new pref_string(SEC_UPLOAD, KEY_UPLOAD_PASSWD, "", 32);
	p->UploadType		= new pref_int_range(SEC_UPLOAD, KEY_UPLOAD_TYPE, 0, 1, 0);

        return 0;
}

static inline int preference_deinit_upload(struct param_upload *p)
{
        delete p->UploadMethod;
	delete p->UploadServer;
	delete p->UploadPort;
	delete p->UploadUser;
	delete p->UploadPasswd;
	delete p->UploadType;

        return 0;
}

static inline int preference_load_upload(struct param_upload *p, XMLNode &root_node)
{
        p->UploadMethod->load(root_node);
	p->UploadServer->load(root_node);
	p->UploadPort->load(root_node);
	p->UploadUser->load(root_node);
	p->UploadPasswd->load(root_node);
	p->UploadType->load(root_node);

        return 0;
}


static inline int preference_save_upload(struct param_upload *p, XMLNode &root_node)
{
        p->UploadMethod->save(root_node);
	p->UploadServer->save(root_node);
	p->UploadPort->save(root_node);
	p->UploadUser->save(root_node);
	p->UploadPasswd->save(root_node);
	p->UploadType->save(root_node);

        return 0;
}


#define SEC_AEW			"AEW"
#define KEY_DEFAULT_GAIN	"DefaultGain"		/* 默认增益		*/
#define	KEY_MIN_GAIN		"MinGain"		/* 最小增益		*/
#define KEY_MAX_GAIN		"MaxGain"		/* 最大增益		*/
#define KEY_DEFAULT_EXP		"DefaultExposure"	/* 默认曝光		*/
#define KEY_MIN_EXP		"MinExposure"		/* 最小曝光		*/
#define KEY_MAX_EXP		"MaxExposure"		/* 最大曝光		*/
#define KEY_V_TARGETGRAY	"VideoTargetGray"	/* 视频目标灰度		*/
#define	KEY_T_TARGETGRAY	"TriggerTargetGray"	/* 抓拍目标灰度		*/
#define KEY_R_GAIN		"Rgain"			/* 白平衡Rgain		*/
#define KEY_B_GAIN		"Bgain"			/* 白平衡Bgain		*/
#define KEY_START_MIN		"DaytimeStartMin"	/* 白天开始时间(分钟)	*/
#define KEY_START_HOUR		"DaytimeStartHour"	/* 白天开始时间(小时)	*/
#define KEY_END_MIN		"DaytimeEndMin"		/* 白天结束时间(分钟)	*/
#define KEY_END_HOUR		"DaytimeEndHour"	/* 白天结束时间(小时)	*/

static inline int preference_init_aew(struct param_aew *p)
{
        p->DefaultGain		= new pref_int(SEC_AEW, KEY_DEFAULT_GAIN, 400);
        p->MinGain		= new pref_int(SEC_AEW, KEY_MIN_GAIN, 350);
        p->MaxGain		= new pref_int(SEC_AEW, KEY_MAX_GAIN, 700);
        p->DefaultExposure	= new pref_int(SEC_AEW, KEY_DEFAULT_EXP, 16);
	p->MinExposure		= new pref_int(SEC_AEW, KEY_MIN_EXP, 1);
	p->MaxExposure		= new pref_int(SEC_AEW, KEY_MAX_EXP, 16);
	p->VideoTargetGray	= new pref_int(SEC_AEW, KEY_V_TARGETGRAY, 80);
        p->TriggerTargetGray	= new pref_int(SEC_AEW, KEY_T_TARGETGRAY, 80);
        p->RGain		= new pref_int(SEC_AEW, KEY_R_GAIN, 400);
        p->BGain		= new pref_int(SEC_AEW, KEY_B_GAIN, 400);
        p->DaytimeStartMin	= new pref_int_range(SEC_AEW, KEY_START_MIN, 0, 59, 30);
        p->DaytimeStartHour	= new pref_int_range(SEC_AEW, KEY_START_HOUR, 0, 23, 6);
        p->DaytimeEndMin	= new pref_int_range(SEC_AEW, KEY_END_MIN, 0, 59, 30);
        p->DaytimeEndHour	= new pref_int_range(SEC_AEW, KEY_END_HOUR, 0, 23, 18);

        return 0;
}

static inline int preference_deinit_aew(struct param_aew *p)
{
	delete p->DefaultGain;
	delete p->MinGain;
	delete p->MaxGain;
	delete p->DefaultExposure;
	delete p->MinExposure;
	delete p->MaxExposure;
	delete p->VideoTargetGray;
	delete p->TriggerTargetGray;
	delete p->RGain;
	delete p->BGain;
	delete p->DaytimeStartMin;
	delete p->DaytimeStartHour;
	delete p->DaytimeEndMin;
	delete p->DaytimeEndHour;

        return 0;
}

static inline int preference_load_aew(struct param_aew *p, XMLNode &root_node)
{
	p->DefaultGain->load(root_node);
	p->MinGain->load(root_node);
	p->MaxGain->load(root_node);
	p->DefaultExposure->load(root_node);
	p->MinExposure->load(root_node);
	p->MaxExposure->load(root_node);
	p->VideoTargetGray->load(root_node);
	p->TriggerTargetGray->load(root_node);
	p->RGain->load(root_node);
	p->BGain->load(root_node);
	p->DaytimeStartMin->load(root_node);
	p->DaytimeStartHour->load(root_node);
	p->DaytimeEndMin->load(root_node);
	p->DaytimeEndHour->load(root_node);

        return 0;
}


static inline int preference_save_aew(struct param_aew *p, XMLNode &root_node)
{
	p->DefaultGain->save(root_node);
	p->MinGain->save(root_node);
	p->MaxGain->save(root_node);
	p->DefaultExposure->save(root_node);
	p->MinExposure->save(root_node);
	p->MaxExposure->save(root_node);
	p->VideoTargetGray->save(root_node);
	p->TriggerTargetGray->save(root_node);
	p->RGain->save(root_node);
	p->BGain->save(root_node);
	p->DaytimeStartMin->save(root_node);
	p->DaytimeStartHour->save(root_node);
	p->DaytimeEndMin->save(root_node);
	p->DaytimeEndHour->save(root_node);
       
        return 0;
}


#define SEC_PERIPHERRAL		"PERIPHERRAL"
#define KEY_FLASH_MODE		"FlashMode"		/* 闪光灯工作模式	*/
#define KEY_LED_MODE		"LedMode"		/* LED灯工作模式	*/
#define KEY_LED_RATIO		"LedRatio"		/* LED灯占空比		*/
#define KEY_LED_MUTIPLE		"LedMutiple"		/* LED灯倍频		*/
#define KEY_PERIPHERRAL_TYPE	"PeripherralType"	/* 外设类型		*/

static inline int preference_init_peripherral(struct param_peripherral *p)
{
        p->FlashMode		= new pref_int_range(SEC_PERIPHERRAL, KEY_FLASH_MODE, 0, 2, 1);
        p->LedMode		= new pref_int_range(SEC_PERIPHERRAL, KEY_LED_MODE, 0, 2, 1);
        p->LedRatio		= new pref_int(SEC_PERIPHERRAL, KEY_LED_RATIO, 2);
        p->LedMutiple		= new pref_int(SEC_PERIPHERRAL, KEY_LED_MUTIPLE, 3);
	p->PeripherralType	= new pref_int(SEC_PERIPHERRAL, KEY_PERIPHERRAL_TYPE, 0);

        return 0;
}

static inline int preference_deinit_peripherral(struct param_peripherral *p)
{
	delete p->FlashMode;
	delete p->LedMode;
	delete p->LedRatio;
	delete p->LedMutiple;
	delete p->PeripherralType;

        return 0;
}

static inline int preference_load_peripherral(struct param_peripherral *p, XMLNode &root_node)
{
	p->FlashMode->load(root_node);
	p->LedMode->load(root_node);
	p->LedRatio->load(root_node);
	p->LedMutiple->load(root_node);
	p->PeripherralType->load(root_node);

        return 0;
}

static inline int preference_save_peripherral(struct param_peripherral *p, XMLNode &root_node)
{
	p->FlashMode->save(root_node);
	p->LedMode->save(root_node);
	p->LedRatio->save(root_node);
	p->LedMutiple->save(root_node);
	p->PeripherralType->save(root_node);
       
        return 0;
}


struct app_param app_param;

static char exe_path[32];
static char exe_name[32];
static char inifilename[256];

char *preference_get_inifile_name(void)
{
	sprintf(inifilename, "%s", INI_FILE_NAME);
	return inifilename;
}

int get_exe_path(char *buf, int size, char **path, char **name)
{
        memset(buf, 0, size);
        if (readlink("/proc/self/exe", buf, size) <= 0) {
                return -1;
        }

        char *temp = NULL;
        if ((temp = (char *)memrchr(buf, '/', size)) == NULL) {
                return -1;
        }
        *temp = '\0';
        ++temp;

        *path = buf;
        *name = temp;

        return 0;
}

int preference_init(void)
{
        struct app_param *p = &app_param;
        preference_init_main(&(p->main));		//初始化运行参数
        preference_init_upload(&(p->upload));		//初始化网络上传参数
        preference_init_aew(&(p->aew));			//初始化AEW参数
        preference_init_peripherral(&(p->peripherral));	//初始化外设参数

        char exe_abs_path[256];
        char *tmp_path;
        char *tmp_name;
        if (get_exe_path(exe_abs_path, sizeof(exe_abs_path),
                         &tmp_path, &tmp_name) < 0) {
        }
        //printf("\nstring path is:%s and length of path is %d\n", tmp_path, strlen(tmp_path));
        memcpy(exe_path, exe_abs_path, strlen(tmp_path) + 1);
        //printf("string name is:%s and length of name is %d\n", tmp_name, strlen(tmp_name));
        memcpy(exe_name, exe_abs_path + strlen(tmp_path) + 1, strlen(tmp_name) + 1);
        /*if (get_exe_path(exe_abs_path, sizeof(exe_abs_path),
                         &exe_path, &exe_name) < 0) {
        }*/
        //sprintf(inifilename, "%s/%s", exe_path, INI_FILE_NAME);
        sprintf(inifilename, "%s", INI_FILE_NAME);

        XMLNode::setGlobalOptions(0, 1, 1);
        preference_load();
        preference_save();

        return 0;
}

int preference_deinit(void)
{
        struct app_param *p = &app_param;
        preference_deinit_main(&(p->main));
        preference_deinit_upload(&(p->upload));
        preference_deinit_aew(&(p->aew));
        preference_deinit_peripherral(&(p->peripherral));

        return 0;
}

int preference_load(void)
{
        struct app_param *p = &app_param;
        
        XMLNode root_node;
        XMLResults result;
        root_node = XMLNode::parseFile(inifilename, "xml", &result);
        if (root_node.getName() == NULL) {
                printf("error %d(%s)\n", result.error, XMLNode::getError(result.error));
                root_node = XMLNode::createXMLTopNode("xml", true);
                root_node.addAttribute("version", "1.0");
                root_node.addAttribute("encoding", "utf-8");
        }

        preference_load_main(&(p->main), root_node);
        preference_load_upload(&(p->upload), root_node);
        preference_load_aew(&(p->aew), root_node);
        preference_load_peripherral(&(p->peripherral), root_node);

        root_node.writeToFile(inifilename);

        return 0;
}

int preference_save(void)
{
        struct app_param *p = &app_param;

        XMLNode root_node;
        XMLResults result;
        root_node = XMLNode::parseFile(inifilename, "xml", &result);
        if (root_node.getName() == NULL) {
                printf("error %d(%s)\n", result.error, XMLNode::getError(result.error));
                root_node = XMLNode::createXMLTopNode("xml", true);
                root_node.addAttribute("version", "1.0");
                root_node.addAttribute("encoding", "utf-8");
        }

        preference_save_main(&(p->main), root_node);
        preference_save_upload(&(p->upload), root_node);
        preference_save_aew(&(p->aew), root_node);
        preference_save_peripherral(&(p->peripherral), root_node);

        root_node.writeToFile(inifilename);
        return 0;
}

int preference_set(char *buf, int len)
{
        FILE *fp = NULL;
        if ((fp = fopen(inifilename, "wb")) == NULL) {
                return -1;
        }
        if (fwrite(buf, len, 1, fp) < 1) {
                fclose(fp);
                return -1;
        }
        fclose(fp);
#if 1
        preference_load();
#else
        system("reboot");
#endif
        return 0;
}

