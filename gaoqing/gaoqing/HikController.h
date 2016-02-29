#pragma once
#include "hik/GeneralDef.h"
#include "winnt.h"
#include "common.h"

#define FAIL -1
#define SUCESS 0

#define ERROR_FAIL_OPEN_FILE   1
#define ERROR_FAIL_OPEN_WINDOW 2

class HikController
{
public:
	HikController(void);
	static void initHik(void);
	CString ip;
	UINT port;
	CString name;
	CString pwd;
	LONG handle;
	HWND wind;
	LONG play_no;
	BOOL g_bRefDone;
	BOOL g_test;


	bool displayHik();
	void stopdisplayHik();
	bool stopPlayHik();
	bool saveFileHik(CString file);
	bool stopsaveFileHik();
	bool stopFileHik();
	int loginHik();
	int logoutHik();
	int playbackHik(CString filename);
	int stopplaybackHik();
	DWORD playedtimeHik();
	int saveParamHik(BYTE bright,BYTE contrast,BYTE sharpness,BYTE saturation,int exp_mode,int exp_time,int daynight);
	PARAM_STRU getParamHik();

	void playslowHik();
	int playposHik(DWORD pos);
	DWORD playtimeHik();
	static void CALLBACK FileRefDoneCB(DWORD nPort,void* nUser);
	bool pauseHik();
	bool resumeHik();
	bool slowHik();
	bool fastHik();
	bool playHik();

private:
	LOCAL_DEVICE_INFO m_struDeviceInfo;
	void DoGetDeviceResoureCfg(LOCAL_DEVICE_INFO *info,LONG channel);
	void GetDecoderCfg(LOCAL_DEVICE_INFO *info);

	static void CALLBACK HikController::test(
  long        nPort,
  char        *pBuf,
  long        nSize,
 FRAME_INFO  *pFrameInfo,
  void*        nUser,
  void*        nReserved2
 
  );
	 CString file;

public:
	~HikController(void);
};
