/*
** preference_items.h
** Login : <frank@frank-desktop>
** Started on  Sat Aug  1 09:58:16 2009 frank
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

#ifndef	_PREFERENCE_ITEMS_H_
#define _PREFERENCE_ITEMS_H_

enum preference_item_index {

	min_id0 = 0,
	DeviceNumber,
        DeviceMode,
        DeviceSite,
        DirectionCode,
	DirectionText,
	RoadCode,
	DepartmentCode,
	Latitue,
	Longtitue,
	Lane,
	StatsPeriod,
	NtpServer,
	max_id0,

	min_id1 = 100,
	UploadMethod,
	UploadServer,
	UploadPort,
	UploadUser,
	UploadPasswd,
	UploadType,
	max_id1,

	min_id2 = 200,
	DefaultGain,
	MinGain,
	MaxGain,
	DefaultExposure,
	MinExposure,
	MaxExposure,
	VideoTargetGray,
	TriggerTargetGray,
	RGain,
	BGain,
	DaytimeStartMin,
	DaytimeStartHour,
	DaytimeEndMin,
	DaytimeEndHour,
	max_id2,

	min_id3 = 300,
	FlashMode,
	LedMode,
	LedRatio,
	LedMutiple,
	PeripherralType,
	max_id3,
	
	Max_Idx
};

#ifdef __cplusplus
extern "C" {
#define ARRAY_SIZE(a)   (sizeof(a) / sizeof((a)[0]))
#define CLEAR(x)        MEMSET(&(x), 0, sizeof(x))	
#endif

int preference_items_init(void);
int preference_items_get(int type, void *value);
int preference_items_set(int type, void *value);
int preference_items_get_string(int type, char *value);
int preference_items_set_string(int type, char *value);

#ifdef __cplusplus
}
#endif

#endif 	    /* !PREFERENCE_ITEMS_H_ */

