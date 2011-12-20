
#ifndef	_PREFERENCE_H_
#define _PREFERENCE_H_

#include "pref.h"

struct param_main
{
        class pref *DeviceNumber;
        class pref *DeviceMode;
        class pref *DeviceSite;
        class pref *DirectionCode;
	class pref *DirectionText;
	class pref *RoadCode;
	class pref *DepartmentCode;
	class pref *Latitue;
	class pref *Longtitue;
	class pref *Lane;
	class pref *StatsPeriod;
	class pref *NtpServer;
};

struct param_upload
{
	class pref *UploadMethod;
	class pref *UploadServer;
	class pref *UploadPort;
	class pref *UploadUser;
	class pref *UploadPasswd;
	class pref *UploadType;
};

struct param_aew
{
	class pref *DefaultGain;
	class pref *MinGain;
	class pref *MaxGain;
	class pref *DefaultExposure;
	class pref *MinExposure;
	class pref *MaxExposure;
	class pref *VideoTargetGray;
	class pref *TriggerTargetGray;
	class pref *RGain;
	class pref *BGain;
	class pref *DaytimeStartMin;
	class pref *DaytimeStartHour;
	class pref *DaytimeEndMin;
	class pref *DaytimeEndHour;
};

struct param_peripherral {
        class pref *FlashMode;
	class pref *LedMode;
	class pref *LedRatio;
	class pref *LedMutiple;
	class pref *PeripherralType;
};


struct app_param {
        struct param_main		main;
        struct param_upload		upload;
        struct param_aew		aew;
        struct param_peripherral	peripherral;
};

#ifdef __cplusplus
extern "C" {
#endif
int preference_init(void);
int preference_deinit(void);
int preference_load(void);
int preference_save(void);
int preference_get(char *buf);
int preference_set(char *buf, int len);

char *preference_get_log_name(void);
char *preference_get_inifile_name(void);

#ifdef __cplusplus
}
#endif

#endif 	    /* !PREFERENCE_H_ */

