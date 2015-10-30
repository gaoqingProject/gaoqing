#pragma once
#include "hik/GeneralDef.h"
#include "winnt.h"
class HikController
{
public:
	HikController(void);
	void initHik(void);
	void DoGetDeviceResoureCfg(LOCAL_DEVICE_INFO *info,LONG channel);
	BOOL loginHik(CString ip,UINT port,CString name,CString pwd,LOCAL_DEVICE_INFO *info);
	void GetDecoderCfg(LOCAL_DEVICE_INFO *info);
	void logoutHik(LOCAL_DEVICE_INFO *info);
	void displayHik(LOCAL_DEVICE_INFO *info);
public:
	~HikController(void);
};
