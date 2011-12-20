/*
** preference_items.c
** Login : <frank@frank-desktop>
** Started on  Sat Aug  1 09:59:15 2009 frank
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


#include "pref.h"
#include "preference_items.h"
#include "preference.h"
#include <pthread.h>

static pthread_mutex_t Preference_mutex;

struct preference_item {
        enum preference_item_index idx;
        class pref                 *pref;
};


class preference_items {
public :
        preference_items(struct preference_item *items, int len)
        {
                this->items = items;
                this->len   = len;
        }
        ~preference_items(void){}
public :
        int get(int index, void *value)
        {
                int i = 0;
                for (i = 0; i < this->len; ++i) {
                        if (this->items[i].idx == index) {
                                return items[i].pref->get(value);
                        }
                }
                return -2;
        }
        int set(int index, void *value)
        {
                int i = 0;
                int tmp = 0;
                char IniFileName[256];
                char *pInifileName = IniFileName;

                for (i = 0; i < this->len; ++i) {
                        if (this->items[i].idx == index) {
                                XMLNode root_node;
                                XMLResults result;

                                tmp = items[i].pref->set(value);

								pInifileName = preference_get_inifile_name();
                                root_node = XMLNode::parseFile(pInifileName, "xml", &result);
                                if (root_node.getName() == NULL) {
                                        return -1;
                                }
								items[i].pref->save(root_node);
                                root_node.writeToFile(pInifileName);
								return tmp;
                        }
                }
                return -2;
        }
        int get_string(int index, char *value)
        {
                int i = 0;
                for (i = 0; i < this->len; ++i) {
                        if (this->items[i].idx == index) {
                                return items[i].pref->get_string(value);
                        }
                }
                return -2;
        }
        int set_string(int index, char *value)
        {
                int i = 0;
				int tmp = 0;
				char IniFileName[256];
				char *pInifileName = IniFileName;

                for (i = 0; i < this->len; ++i) {
                        if (this->items[i].idx == index) {
                                XMLNode root_node;
                                XMLResults result;

								tmp = items[i].pref->set_string(value);
								pInifileName = preference_get_inifile_name();

                                root_node = XMLNode::parseFile(pInifileName, "xml", &result);
                                if (root_node.getName() == NULL) {
                                        return -1;
                                }

								items[i].pref->save(root_node);
                                root_node.writeToFile(pInifileName);
								return tmp;
                        }
                }
                return -2;
        }
private :
        struct preference_item *items;
        int                    len;
};


static struct preference_item preference_items_main[max_id0];
static struct preference_item preference_items_upload[max_id1 - min_id1 - 1];
static struct preference_item preference_items_aew[max_id2 - min_id2 - 1];
static struct preference_item preference_items_peripherral[max_id3 - min_id3 - 1];


int preference_items_get(int type, void *value)
{
	class preference_items items_main(preference_items_main,
						ARRAY_SIZE(preference_items_main));
	class preference_items items_upload(preference_items_upload,
						ARRAY_SIZE(preference_items_upload));
	class preference_items items_aew(preference_items_aew,
						ARRAY_SIZE(preference_items_aew));
	class preference_items items_peripherral(preference_items_peripherral,
						ARRAY_SIZE(preference_items_peripherral));
	
	int ret;
	pthread_mutex_lock(&Preference_mutex);
        if ((ret = items_main.get(type, value)) == 0) {
		pthread_mutex_unlock(&Preference_mutex);
                return ret;
        } else if (ret != -2) {
		pthread_mutex_unlock(&Preference_mutex);
                return ret;
        }
        if ((ret = items_upload.get(type, value)) == 0) {
        }

        if ((ret = items_aew.get(type, value)) == 0) {
        }
        if ((ret = items_peripherral.get(type, value)) == 0) {
        }
	
	pthread_mutex_unlock(&Preference_mutex);
        return ret;
}

int preference_items_set(int type, void *value)
{
	class preference_items items_main(preference_items_main,
						ARRAY_SIZE(preference_items_main));
	class preference_items items_upload(preference_items_upload,
						ARRAY_SIZE(preference_items_upload));
	class preference_items items_aew(preference_items_aew,
						ARRAY_SIZE(preference_items_aew));
	class preference_items items_peripherral(preference_items_peripherral,
						ARRAY_SIZE(preference_items_peripherral));
        int ret;
	pthread_mutex_lock(&Preference_mutex);
        if ((ret = items_main.set(type, value)) == 0) {
			pthread_mutex_unlock(&Preference_mutex);
            return ret;
	} else if (ret != -2) {
		pthread_mutex_unlock(&Preference_mutex);
		return ret;
	}
	if ((ret = items_upload.get(type, value)) == 0) {
        }

        if ((ret = items_aew.get(type, value)) == 0) {
        }
        if ((ret = items_peripherral.get(type, value)) == 0) {
        }

	pthread_mutex_unlock(&Preference_mutex);

        return ret;
}

int preference_items_get_string(int type, char *value)
{
	class preference_items items_main(preference_items_main,
						ARRAY_SIZE(preference_items_main));
	class preference_items items_upload(preference_items_upload,
						ARRAY_SIZE(preference_items_upload));
	class preference_items items_aew(preference_items_aew,
						ARRAY_SIZE(preference_items_aew));
	class preference_items items_peripherral(preference_items_peripherral,
						ARRAY_SIZE(preference_items_peripherral));
        int ret;
	pthread_mutex_lock(&Preference_mutex);
        if ((ret = items_main.get_string(type, value)) == 0) {
		pthread_mutex_unlock(&Preference_mutex);
                return ret;
        } else if (ret != -2) {
		pthread_mutex_unlock(&Preference_mutex);
                return ret;
        }
	if ((ret = items_upload.get(type, value)) == 0) {
        }

        if ((ret = items_aew.get(type, value)) == 0) {
        }
        if ((ret = items_peripherral.get(type, value)) == 0) {
        }

	pthread_mutex_unlock(&Preference_mutex);
        return ret;
}

int preference_items_set_string(int type, char *value)
{
	class preference_items items_main(preference_items_main,
						ARRAY_SIZE(preference_items_main));
	class preference_items items_upload(preference_items_upload,
						ARRAY_SIZE(preference_items_upload));
	class preference_items items_aew(preference_items_aew,
						ARRAY_SIZE(preference_items_aew));
	class preference_items items_peripherral(preference_items_peripherral,
						ARRAY_SIZE(preference_items_peripherral));
        int ret;
	pthread_mutex_lock(&Preference_mutex);
        if ((ret = items_main.set_string(type, value)) == 0) {
		pthread_mutex_unlock(&Preference_mutex);
		return ret;
        } else if (ret != -2) {
		pthread_mutex_unlock(&Preference_mutex);
		return ret;
        }
	if ((ret = items_upload.get(type, value)) == 0) {
        }

        if ((ret = items_aew.get(type, value)) == 0) {
        }
        if ((ret = items_peripherral.get(type, value)) == 0) {
        }

	pthread_mutex_unlock(&Preference_mutex);

        return ret;
}


extern struct app_param app_param;

static inline int preference_items_init_main(void)
{
        struct app_param *p = &app_param;
        preference_items_main[ 0].idx  = DeviceNumber;
        preference_items_main[ 0].pref = p->main.DeviceNumber;
        preference_items_main[ 1].idx  = DeviceMode;
        preference_items_main[ 1].pref = p->main.DeviceMode;
        preference_items_main[ 2].idx  = DeviceSite;
        preference_items_main[ 2].pref = p->main.DeviceSite;
        preference_items_main[ 3].idx  = DirectionCode;
        preference_items_main[ 3].pref = p->main.DirectionCode;
        preference_items_main[ 4].idx  = DirectionText;
        preference_items_main[ 4].pref = p->main.DirectionText;
        preference_items_main[ 5].idx  = RoadCode;
        preference_items_main[ 5].pref = p->main.RoadCode;
        preference_items_main[ 6].idx  = DepartmentCode;
        preference_items_main[ 6].pref = p->main.DepartmentCode;
        preference_items_main[ 7].idx  = Latitue;
        preference_items_main[ 7].pref = p->main.Latitue;
        preference_items_main[ 8].idx  = Longtitue;
        preference_items_main[ 8].pref = p->main.Longtitue;
        preference_items_main[ 9].idx  = Lane;
        preference_items_main[ 9].pref = p->main.Lane;
        preference_items_main[10].idx  = StatsPeriod;
        preference_items_main[10].pref = p->main.StatsPeriod;
        preference_items_main[11].idx  = NtpServer;
        preference_items_main[11].pref = p->main.NtpServer;
                                   
        return 0;                  
}                                  
                                   
static inline int preference_items_init_upload(void)
{                                  
        struct app_param *p = &app_param;
	
        preference_items_upload[ 0].idx  = UploadMethod;
        preference_items_upload[ 0].pref = p->upload.UploadMethod;
        preference_items_upload[ 1].idx  = UploadServer;
        preference_items_upload[ 1].pref = p->upload.UploadServer;
        preference_items_upload[ 2].idx  = UploadPort;
        preference_items_upload[ 2].pref = p->upload.UploadPort;
        preference_items_upload[ 3].idx  = UploadUser;
        preference_items_upload[ 3].pref = p->upload.UploadUser;
        preference_items_upload[ 4].idx  = UploadPasswd;
        preference_items_upload[ 4].pref = p->upload.UploadPasswd;
        preference_items_upload[ 5].idx  = UploadType;
        preference_items_upload[ 5].pref = p->upload.UploadType;

	return 0;
}

static inline int preference_items_init_aew(void)
{
	struct app_param *p = &app_param;

	preference_items_aew[ 0].idx= DefaultGain;
        preference_items_aew[ 0].pref = p->aew.DefaultGain;
        preference_items_aew[ 1].idx= MinGain;
        preference_items_aew[ 1].pref = p->aew.MinGain;
        preference_items_aew[ 2].idx= MaxGain;
        preference_items_aew[ 2].pref = p->aew.MaxGain;
        preference_items_aew[ 3].idx= DefaultExposure;
        preference_items_aew[ 3].pref = p->aew.DefaultExposure;
        preference_items_aew[ 4].idx= MinExposure;
        preference_items_aew[ 4].pref = p->aew.MinExposure;
	preference_items_aew[ 5].idx= MaxExposure;
        preference_items_aew[ 5].pref = p->aew.MaxExposure;
        preference_items_aew[ 6].idx= VideoTargetGray;
        preference_items_aew[ 6].pref = p->aew.VideoTargetGray;
        preference_items_aew[ 7].idx= TriggerTargetGray;
        preference_items_aew[ 7].pref = p->aew.TriggerTargetGray;
        preference_items_aew[ 8].idx= RGain;
        preference_items_aew[ 8].pref = p->aew.RGain;
        preference_items_aew[ 9].idx= BGain;
        preference_items_aew[ 9].pref = p->aew.BGain;
        preference_items_aew[10].idx= DaytimeStartMin;
        preference_items_aew[10].pref = p->aew.DaytimeStartMin;
	preference_items_aew[11].idx= DaytimeStartHour;
        preference_items_aew[11].pref = p->aew.DaytimeStartHour;
        preference_items_aew[12].idx= DaytimeEndMin;
        preference_items_aew[12].pref = p->aew.DaytimeEndMin;
	preference_items_aew[13].idx= DaytimeEndHour;
        preference_items_aew[13].pref = p->aew.DaytimeEndHour;
		
        return 0;
}


static inline int preference_items_init_peripherral(void)
{                                  
        struct app_param *p = &app_param;
	
        preference_items_peripherral[ 0].idx  = FlashMode;
        preference_items_peripherral[ 0].pref = p->peripherral.FlashMode;
        preference_items_peripherral[ 1].idx  = LedMode;
        preference_items_peripherral[ 1].pref = p->peripherral.LedMode;
        preference_items_peripherral[ 2].idx  = LedRatio;
        preference_items_peripherral[ 2].pref = p->peripherral.LedRatio;
        preference_items_peripherral[ 3].idx  = LedMutiple;
        preference_items_peripherral[ 3].pref = p->peripherral.LedMutiple;
        preference_items_peripherral[ 4].idx  = PeripherralType;
        preference_items_peripherral[ 4].pref = p->peripherral.PeripherralType;

	return 0;
}



int preference_items_init(void)
{
	preference_items_init_main();
	preference_items_init_upload();
	preference_items_init_aew();
	preference_items_init_peripherral();
	pthread_mutex_init(&Preference_mutex,NULL);
	return 0;
}

