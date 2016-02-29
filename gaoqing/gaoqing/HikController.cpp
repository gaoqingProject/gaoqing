#include "StdAfx.h"
#include "HikController.h"
#define channelNO 0


HikController::HikController(void): play_no(-1)
{
	
}
void HikController::initHik(void)
{
	//=================================HIK Operation=======================
	NET_DVR_Init();
	
}
DWORD HikController::playtimeHik()
{
	DWORD time = PlayM4_GetFileTime(play_no);
	LONG pt = PlayM4_GetPort(&pt);
	if (!PlayM4_OpenFile(pt, file.GetBuffer(file.GetLength())))
	{
		//g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "PlayM4_OpenFile %d", PlayM4_GetLastError(USED_PORT));
		return 0;
	}
	time = PlayM4_GetFileTime(pt);

	PlayM4_CloseFile(pt);
	PlayM4_FreePort(pt);
	return time;
}
DWORD HikController::playedtimeHik()
{
	return PlayM4_GetPlayedTimeEx(play_no);
}
int HikController::playposHik(DWORD pos)
{
	if(play_no == -1  || pos ==0)
		return SUCESS;
	if (!PlayM4_SetPlayedTimeEx(play_no, pos))
	{
		return FAIL;
	}else{
		return SUCESS;
	}
}
bool HikController::resumeHik()
{
	if(play_no == -1)
		return SUCESS;
	if (!PlayM4_Pause(play_no,FALSE))
	{
		return FAIL;
	}else{
		return SUCESS;
	}
}
bool HikController::pauseHik()
{
	if(play_no == -1)
		return SUCESS;
	if (!PlayM4_Pause(play_no,TRUE))
	{
		return FAIL;
	}else{
		return SUCESS;
	}
}
bool HikController::fastHik()
{
	if(play_no == -1)
		return SUCESS;
	if (!PlayM4_Fast(play_no))
	{
		return FAIL;
	}else{
		return SUCESS;
	}
}
bool HikController::slowHik()
{
	if(play_no == -1)
		return SUCESS;
	if (!PlayM4_Slow(play_no))
	{
		return FAIL;
	}else{
		return SUCESS;
	}
}
bool HikController::playHik()
{
	if(play_no == -1)
		return SUCESS;
	if (!PlayM4_Play(play_no,wind))
	{
		return FAIL;
	}else{
		return SUCESS;
	}
}
void CALLBACK HikController::FileRefDoneCB(DWORD nPort,void* nUser)
{
	HikController *ct = static_cast<HikController*>((void*)nUser);
       ct->g_bRefDone = TRUE;
}

void CALLBACK HikController::test(
  long        nPort,
  char        *pBuf,
  long        nSize,
 FRAME_INFO  *pFrameInfo,
  void*        nUser,
  void*        nReserved2
  ){
	  if(nSize != 0){
	  
		  HikController *ct = static_cast<HikController*>((void*)nUser);
		ct->g_test=TRUE;
	  }

}
/*********************************************************
  Function:	PlayBack
  Desc:		播放视频
  Input:	
  Output:	
  Return:	SUCESS 成功
            ERROR_FAIL_OPEN_FILE   文件打开失败
			ERROR_FAIL_OPEN_WINDOW 视频输出到窗口失败
**********************************************************/
int HikController::playbackHik(CString filename)
{

	int iFilePosSel = 0;
	//CString csFileName="test.mp4";//path of video file
	//CString csDeviceIP;
	//CString csTemp;
	//CString sTemp1;
	//int nPos = 0;
	//char szLan[128] = {0};
	file=filename;
	PlayM4_Stop(play_no);
	PlayM4_CloseFile(play_no);


	g_bRefDone = FALSE;
	g_test = FALSE;
	PlayM4_SetFileRefCallBack(play_no,&HikController::FileRefDoneCB,(void *)this);
	//PlayM4_SetDecCallBackMend(play_no,&HikController::test,(void *)this);

	if (!PlayM4_OpenFile(play_no, filename.GetBuffer(filename.GetLength())))
	{
		//g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "PlayM4_OpenFile %d", PlayM4_GetLastError(USED_PORT));
		return ERROR_FAIL_OPEN_FILE;
	}
	

	if (PlayM4_Play(play_no,wind)==FALSE)
	{
		
		PlayM4_CloseFile(play_no);
		//g_pMainDlg->AddLog(m_iDeviceIndex, PLAY_FAIL_T, "PlayM4_Play err[%d]", PlayM4_GetLastError(USED_PORT));
		return ERROR_FAIL_OPEN_WINDOW;
	}

	while (g_bRefDone == FALSE)
    {
		Sleep(2);
	}

	/*while (g_test == FALSE)
    {
		Sleep(2);
	}*/

	/*DWORD frame;
	do{
		frame = PlayM4_GetCurrentFrameNum(play_no);
	}while(!frame);*/

	Sleep(500);
	return SUCESS;
}
void HikController::playslowHik()
{
	PlayM4_Slow(play_no);
	//PlayM4_Slow(play_no);
	//PlayM4_Slow(play_no);
	//PlayM4_Slow(play_no);
}

/*********************************************************
  Function:	stopplaybackHik
  Desc:		停止播放视频
  Input:	
  Output:	
  Return:	SUCESS 成功
            ERROR_FAIL_OPEN_FILE   文件打开失败
			ERROR_FAIL_OPEN_WINDOW 视频输出到窗口失败
**********************************************************/
int HikController::stopplaybackHik()
{
		PlayM4_Stop(play_no);
		PlayM4_CloseFile(play_no);
		return SUCESS;
}
















/*************************************************
函数名:    	stopPlayHik
函数描述:	停止播放
输入参数:   
输出参数:   			
返回值:		SUCESS 成功
            FAIL   失败
**************************************************/
bool HikController::stopPlayHik()
{
	if(handle != -1)
	{
		
		NET_DVR_StopRealPlay(handle);
		handle=-1;
		return SUCESS;
	}
	return FAIL;
}
/*************************************************
函数名:    	stopFileHik
函数描述:	停止录像
输入参数:   
输出参数:   			
返回值:		SUCESS 成功
            FAIL   失败
**************************************************/
bool HikController::stopFileHik()
{
    if(!NET_DVR_StopSaveRealData(handle))
	{
		//MessageBox("停止录像失败");
		return FAIL;
	}
	return SUCESS;
	
}
/*************************************************
函数名:    	DoGetDeviceResoureCfg
函数描述:	获得设备资源配置
输入参数:   设备info
            channel号
输出参数:   			
返回值:		
**************************************************/
void HikController::DoGetDeviceResoureCfg(LOCAL_DEVICE_INFO *info,LONG channel)
{
	NET_DVR_IPPARACFG_V40 IpAccessCfg;
	memset(&IpAccessCfg,0,sizeof(IpAccessCfg));	
	DWORD  dwReturned;

	

	info->bIPRet = NET_DVR_GetDVRConfig( \
		(*info).lLoginID, \
		NET_DVR_GET_IPPARACFG_V40, \
		channel, \
		&IpAccessCfg, \
		sizeof(NET_DVR_IPPARACFG_V40), \
		&dwReturned \
		);

	int i;
    if(!info->bIPRet)   //不支持ip接入,9000以下设备不支持禁用模拟通道
	{
		for(i=0; i<MAX_ANALOG_CHANNUM; i++)
		{
			if (i < info->iDeviceChanNum)
			{
				sprintf(info->struChanInfo[i].chChanName,"camera%d",i+info->iStartChan);
				info->struChanInfo[i].iChanIndex=i+info->iStartChan;  //通道号
				info->struChanInfo[i].bEnable = TRUE;
				
			}
			else
			{
				info->struChanInfo[i].iChanIndex = -1;
				info->struChanInfo[i].bEnable = FALSE;
				sprintf(info->struChanInfo[i].chChanName, "");	
			}
		}
	}
	else        //支持IP接入，9000设备
	{
		for(i=0; i<MAX_ANALOG_CHANNUM; i++)  //模拟通道
		{
			if (i < info->iDeviceChanNum)
			{
				sprintf(info->struChanInfo[i].chChanName,"camera%d",i+info->iStartChan);
				info->struChanInfo[i].iChanIndex=i+info->iStartChan;
				if(IpAccessCfg.byAnalogChanEnable[i])
				{
					info->struChanInfo[i].bEnable = TRUE;
				}
				else
				{
					info->struChanInfo[i].bEnable = FALSE;
				}
				
			}
			else//clear the state of other channel
			{
				info->struChanInfo[i].iChanIndex = -1;
				info->struChanInfo[i].bEnable = FALSE;
				sprintf(info->struChanInfo[i].chChanName, "");	
			}		
		}

		//数字通道
		for(i=0; i<MAX_IP_CHANNEL; i++)
		{
			if(IpAccessCfg.struStreamMode[i].uGetStream.struChanInfo.byEnable)  //ip通道在线
			{
				info->struChanInfo[i+MAX_ANALOG_CHANNUM].bEnable = TRUE;
                info->struChanInfo[i+MAX_ANALOG_CHANNUM].iChanIndex = i+IpAccessCfg.dwStartDChan;
				sprintf(info->struChanInfo[i+MAX_ANALOG_CHANNUM].chChanName,"IP Camera %d",i+1);

			}
			else
			{
               info->struChanInfo[i+MAX_ANALOG_CHANNUM].bEnable = FALSE;
			   info->struChanInfo[i+MAX_ANALOG_CHANNUM].iChanIndex = -1;
			}
		}
		
		
	}

}
/*************************************************
函数名:    	GetDecoderCfg
函数描述:	获得解码器配置
输入参数:   设备info
输出参数:   			
返回值:		
**************************************************/
void HikController::GetDecoderCfg(LOCAL_DEVICE_INFO *info)
{
     NET_DVR_DECODERCFG_V30 DecoderCfg;
	 DWORD  dwReturned;
	 BOOL bRet;
		
      
	 //获取通道解码器信息
	 for(int i=0; i<MAX_CHANNUM_V30; i++)
	 {
		 if(info->struChanInfo[i].bEnable)
		 {
			 memset(&DecoderCfg,0,sizeof(NET_DVR_DECODERCFG_V30));
			 bRet = NET_DVR_GetDVRConfig(info->lLoginID,NET_DVR_GET_DECODERCFG_V30 , \
				info->struChanInfo[i].iChanIndex,&DecoderCfg,sizeof(NET_DVR_DECODERCFG_V30),&dwReturned);
			 if(!bRet)
			 {
				 TRACE("Get DecderCfg failed,Chan:%d\n",info->struChanInfo[i].iChanIndex);
				 continue;
			 }

			 memcpy(&info->struChanInfo[i].struDecodercfg,&DecoderCfg,sizeof(NET_DVR_DECODERCFG_V30));
		 }
		 
	 }

}

//=======================================================11.5======================================================

/*************************************************
函数名:    	displayHik
函数描述:	显示输出
输入参数:   设备info
输出参数:   			
返回值:		handle 成功
            FAIL 失败
**************************************************/
bool HikController::displayHik()
{
	/*NET_DVR_CLIENTINFO ClientInfo;
	ClientInfo.hPlayWnd     = (HWND)0;
									//handle of display window
	ClientInfo.lChannel     = 1;	//channel no
	ClientInfo.lLinkMode    = 0;	//bit 31 ----0:main stream 
									//bit 0-30 ----0:tcp 1:udp 2:multi cast
    ClientInfo.sMultiCastIP = NULL; //multi cast ip
*/

	NET_DVR_PREVIEWINFO ClientInfo = {0};
	//ClientInfo.hPlayWnd     = GetDlgItem(IDC_STATIC_PLAY)->m_hWnd;
	ClientInfo.lChannel     = 1;
	ClientInfo.dwStreamType    = 0;
    ClientInfo.dwLinkMode = 0;



	TRACE("Channel number:%d\n",ClientInfo.lChannel);
	//handle = NET_DVR_RealPlay_V30(m_struDeviceInfo.lLoginID,&ClientInfo,NULL,NULL,TRUE);
	handle = NET_DVR_RealPlay_V40(m_struDeviceInfo.lLoginID,&ClientInfo,NULL,NULL);
	if(-1 == handle)
	{
		DWORD err=NET_DVR_GetLastError();

		return FAIL;
	}
	return SUCESS;

}
void HikController::stopdisplayHik()
{
	NET_DVR_StopRealPlay(handle);
	handle=-1;
}
/*************************************************
函数名:    	saveFileHik
函数描述:	保存录像
输入参数:   设备info
输出参数:   			
返回值:		SUCESS 成功
            FAIL 失败
**************************************************/
bool HikController::saveFileHik(CString file)
{

	/*char RecName[256] = {0};
	
	CTime CurTime = CTime::GetCurrentTime();;
	sprintf( \
		RecName, \
		"%04d%02d%02d%02d%02d%02d_ch%02d.mp4", \
		CurTime.GetYear(), \
		CurTime.GetMonth(), \
		CurTime.GetDay(), \
		CurTime.GetHour(), \
		CurTime.GetMinute(), \
		CurTime.GetSecond(), \
		info.struChanInfo[0].iChanIndex \
		);*/
	char *RecName=file.GetBuffer(file.GetLength());
	if(NET_DVR_SaveRealData(handle,RecName))
	{
		return SUCESS;
	}else{
		return FAIL;
	}

}
bool HikController::stopsaveFileHik()
{
	if(!NET_DVR_StopSaveRealData(handle))
	{
		return FAIL;
	}
	return SUCESS;
}
/*************************************************
函数名:    	loginHik
函数描述:	注册设备
输入参数:   设备ip 端口 用户名 密码 设备info
输出参数:   			
返回值:		SUCESS 成功
            FAIL 失败
**************************************************/
int HikController::loginHik()
{
	
	NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;
	memset(&DeviceInfoTmp,0,sizeof(NET_DVR_DEVICEINFO_V30));
	
	LONG lLoginID = NET_DVR_Login_V30( \
		ip.GetBuffer(ip.GetLength()), \
		port, \
		name.GetBuffer(name.GetLength()), \
		pwd.GetBuffer(pwd.GetLength()), \
		&DeviceInfoTmp \
		);
	if(lLoginID == -1)
	{
		//MessageBox("Login to Device failed!\n");
		return FAIL;
	}
	m_struDeviceInfo.lLoginID = lLoginID;
	m_struDeviceInfo.iDeviceChanNum = DeviceInfoTmp.byChanNum;
	m_struDeviceInfo.iIPChanNum = DeviceInfoTmp.byIPChanNum;
	m_struDeviceInfo.iStartChan  = DeviceInfoTmp.byStartChan;
    m_struDeviceInfo.iIPStartChan  = DeviceInfoTmp.byStartDChan;

	DoGetDeviceResoureCfg(&m_struDeviceInfo,channelNO);  //获取设备资源信息	
	GetDecoderCfg(&m_struDeviceInfo);                           //获取云台解码器信息 

	DWORD returnLen;
	NET_DVR_COMPRESSIONCFG_V30 para = {0};
	int re = NET_DVR_GetDVRConfig(m_struDeviceInfo.lLoginID, NET_DVR_SET_COMPRESSCFG_V30, DeviceInfoTmp.byStartChan, &para, sizeof(NET_DVR_COMPRESSIONCFG_V30), &returnLen);
	if(re){
		para.struNormHighRecordPara.byVideoEncType = 1; //standard h264
		para.struNormHighRecordPara.byResolution = 27;  //1920x1080p
		para.struNormHighRecordPara.byBitrateType = 1;  //fixed bit rate
		para.struNormHighRecordPara.byPicQuality = 0;   //qulity hight
		para.struNormHighRecordPara.dwVideoFrameRate = 22;//50Hz
		para.struNormHighRecordPara.dwVideoBitrate = 27; //16384K bps
		para.struNormHighRecordPara.byStreamSmooth = 50; //smooth 50

		NET_DVR_SetDVRConfig(m_struDeviceInfo.lLoginID, NET_DVR_SET_COMPRESSCFG_V30, DeviceInfoTmp.byStartChan, &para, sizeof(NET_DVR_COMPRESSIONCFG_V30));
	
	
	}


	return SUCESS;
}
int HikController::saveParamHik(BYTE brightness,BYTE contrast,BYTE sharpness,BYTE saturation,int exp_mode,int exp_time,int daynight)
{
	/*
	NET_DVR_MULTI_STREAM_COMPRESSIONCFG_COND m_struMSCond;
	NET_DVR_MULTI_STREAM_COMPRESSIONCFG m_struMSCompressionCfg;
	DWORD dwStatus;

	//memset(&m_struMSCond, 0, sizeof(NET_DVR_MULTI_STREAM_COMPRESSIONCFG_COND));
	memset(&m_struMSCond,0,sizeof(NET_DVR_MULTI_STREAM_COMPRESSIONCFG_COND));
	m_struMSCond.dwSize = sizeof(NET_DVR_MULTI_STREAM_COMPRESSIONCFG);

	m_struMSCond.struStreamInfo.dwChannel = m_struDeviceInfo.iIPStartChan;
	m_struMSCond.struStreamInfo.dwSize = sizeof(NET_DVR_STREAM_INFO);
	m_struMSCond.dwStreamType = 0;
	if (!NET_DVR_SetDeviceConfig(m_struDeviceInfo.lLoginID, NET_DVR_SET_MULTI_STREAM_COMPRESSIONCFG, 0,
				&m_struMSCond, sizeof(NET_DVR_MULTI_STREAM_COMPRESSIONCFG_COND), &dwStatus, &m_struMSCompressionCfg, sizeof(NET_DVR_MULTI_STREAM_COMPRESSIONCFG)))	
	{

		return FAIL;
	}else{
		return SUCESS;
	
	}*/
	DWORD returnLen;
	NET_DVR_CAMERAPARAMCFG para = {0};
	int re = NET_DVR_GetDVRConfig(m_struDeviceInfo.lLoginID, NET_DVR_GET_CCDPARAMCFG, m_struDeviceInfo.iStartChan, &para, sizeof(NET_DVR_CAMERAPARAMCFG), &returnLen);
	if(re){


		para.struVideoEffect.byBrightnessLevel = brightness; //standard h264
		para.struVideoEffect.byContrastLevel = contrast; //standard h264
		para.struVideoEffect.bySharpnessLevel = sharpness; //standard h264
		para.struVideoEffect.bySaturationLevel = saturation; //standard h264

		para.struExposure.byExposureMode = exp_mode; //0 手动曝光 1自动曝光
		para.struExposure.dwVideoExposureSet = exp_time; //us

		para.struDayNight.byDayNightFilterType =daynight; //日夜切换：0-白天，1-夜晚，2-自动，3-定时，4-报警输入触发

		if(NET_DVR_SetDVRConfig(m_struDeviceInfo.lLoginID, NET_DVR_SET_CCDPARAMCFG, m_struDeviceInfo.iStartChan, &para, sizeof(NET_DVR_CAMERAPARAMCFG))){
	
			return SUCESS;
		}else{
			return FAIL;
		}
	
	}else{
		return FAIL;
	}
}
PARAM_STRU HikController::getParamHik()
{
	/*
	NET_DVR_MULTI_STREAM_COMPRESSIONCFG_COND m_struMSCond;
	NET_DVR_MULTI_STREAM_COMPRESSIONCFG m_struMSCompressionCfg;
	DWORD dwStatus;

	//memset(&m_struMSCond, 0, sizeof(NET_DVR_MULTI_STREAM_COMPRESSIONCFG_COND));
	memset(&m_struMSCond,0,sizeof(NET_DVR_MULTI_STREAM_COMPRESSIONCFG_COND));
	m_struMSCond.dwSize = sizeof(NET_DVR_MULTI_STREAM_COMPRESSIONCFG);

	m_struMSCond.struStreamInfo.dwChannel = m_struDeviceInfo.iIPStartChan;
	m_struMSCond.struStreamInfo.dwSize = sizeof(NET_DVR_STREAM_INFO);
	m_struMSCond.dwStreamType = 0;
	if (!NET_DVR_SetDeviceConfig(m_struDeviceInfo.lLoginID, NET_DVR_SET_MULTI_STREAM_COMPRESSIONCFG, 0,
				&m_struMSCond, sizeof(NET_DVR_MULTI_STREAM_COMPRESSIONCFG_COND), &dwStatus, &m_struMSCompressionCfg, sizeof(NET_DVR_MULTI_STREAM_COMPRESSIONCFG)))	
	{

		return FAIL;
	}else{
		return SUCESS;
	
	}*/
	DWORD returnLen;
	NET_DVR_CAMERAPARAMCFG para = {0};
	int re = NET_DVR_GetDVRConfig(m_struDeviceInfo.lLoginID, NET_DVR_GET_CCDPARAMCFG, m_struDeviceInfo.iStartChan, &para, sizeof(NET_DVR_CAMERAPARAMCFG), &returnLen);
	PARAM_STRU str;
	if(re){

		
		str.brightness = para.struVideoEffect.byBrightnessLevel; //standard h264
		str.contrast = para.struVideoEffect.byContrastLevel; //standard h264
		str.sharpness = para.struVideoEffect.bySharpnessLevel; //standard h264
		str.saturation = para.struVideoEffect.bySaturationLevel; //standard h264

		str.exposuremode = para.struExposure.byExposureMode; /*0 手动曝光 1自动曝光*/
		str.exposuretime = para.struExposure.dwVideoExposureSet; //us

		str.daynight = para.struDayNight.byDayNightFilterType; /*日夜切换：0-白天，1-夜晚，2-自动，3-定时，4-报警输入触发*/ 

		//NET_DVR_SetDVRConfig(m_struDeviceInfo.lLoginID, NET_DVR_SET_CCDPARAMCFG, m_struDeviceInfo.iStartChan, &para, sizeof(NET_DVR_CAMERAPARAMCFG));
	
		
	
	}else{
		str.brightness = 0; //standard h264
		str.contrast = 0; //standard h264
		str.sharpness = 0; //standard h264
		str.saturation = 0; //standard h264

		str.exposuremode = 0; /*0 手动曝光 1自动曝光*/
		str.exposuretime = 1; //us

		str.daynight = 0; 
	
	
	}
	return str;
}


/*************************************************
函数名:    	logoutHik
函数描述:	注册设备
输入参数:   设备info
输出参数:   			
返回值:		SUCESS 成功
            FAIL 失败
**************************************************/
int HikController::logoutHik()
{
	if(NET_DVR_Logout_V30(m_struDeviceInfo.lLoginID)){
		return SUCESS;

	}else{
		return FAIL;
	} 
}










HikController::~HikController(void)
{
}
