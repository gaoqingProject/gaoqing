/*
File name:   HikController.h
File Author: Li Weichen
Date:        2016.3.1
*/
#pragma once
#include "hik/GeneralDef.h"
#include "winnt.h"
#include "common.h"

#define FAIL -1
#define SUCCESS 0

#define ERROR_FAIL_OPEN_FILE   1
#define ERROR_FAIL_OPEN_WINDOW 2

class HikController
{
public:
//============================================ Member Functions ===================
	HikController(void);
	~HikController(void);

	//live functions
	bool playliveHik(bool isPlay);
	bool stopplayliveHik();
	
	int savefileHik(CString file);
	int stopsavefileHik();
	
	
	//replay fuctions
	bool pauseHik();
	bool resumeHik();
	bool slowHik(int times);
	bool fastHik();
	int playbackHik(CString filename);
	int stopplaybackHik();
	int playposHik(DWORD pos);
	DWORD playedtimeHik();
	DWORD playtimeHik();

	//login and setting functions
	static void CALLBACK FileRefDoneCB(DWORD nPort,void* nUser);
	static void initHik(void);

	int loginHik();
	int logoutHik();
	int saveParamHik(BYTE bright,BYTE contrast,BYTE sharpness,BYTE saturation,int exp_mode,int exp_time,int daynight);
	PARAM_STRU getParamHik();


	

//============================================ Member Variables ===================
	bool rec; //is record display
	//====config variables
	HWND mWind;
	LONG mPlay_no;
	CString mIp;
	UINT mPort;
	CString mName;
	CString mPwd;
private:
//============================================ Member Functions ===================
	void dogetdeviceresourecfgHik(LOCAL_DEVICE_INFO *info,LONG channel);
	void getdecodercfgHik(LOCAL_DEVICE_INFO *info);
//============================================ Member Variables ===================
	LONG handle;
	BOOL g_bRefDone;
	BOOL g_test;
	CString file;
	LOCAL_DEVICE_INFO m_struDeviceInfo;
};
