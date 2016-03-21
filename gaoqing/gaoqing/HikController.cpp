/*
File name:   HikController.cpp
File Author: Li Weichen
Date:        2016.3.1
*/
#include "StdAfx.h"
#include "HikController.h"
//====================== Camera Variables Definition ===================
#define channelNO 0
//====================== Definition End ================================

HikController::HikController(void): mPlay_no(-1),handle(-1),rec(false)
{

}
/*************************************************
Function:  initHik
Desc:	   初始化sdk
Input:     void
Return:	   void
**************************************************/
void HikController::initHik(void)
{
	NET_DVR_Init();
}
/*************************************************
Function:   playtimeHik
Desc:	    获取当前文件的播放长度
Input:      void
Return:		0 -- FAIL
            others -- 文件播放长度
**************************************************/
DWORD HikController::playtimeHik()
{
	DWORD time;
	LONG pt; 
	PlayM4_GetPort(&pt);//get a port
	if (!PlayM4_OpenFile(pt, file.GetBuffer(file.GetLength())))//open file
	{
		return 0;//open failed
	}
	time = PlayM4_GetFileTime(pt); // get time in unit of second

	PlayM4_CloseFile(pt);//close file
	PlayM4_FreePort(pt);// release port
	return time;
}
/*************************************************
Function:   playedtimeHik
Desc:	    获取播放时间
Input:      void
Return:		播放时间（ms）
**************************************************/
DWORD HikController::playedtimeHik()
{
	return PlayM4_GetPlayedTimeEx(mPlay_no);
}
/*************************************************
Function:   playposHik
Desc:	    设置播放位置
Input:      播放时间位置（ms）
Return:		FAIL -- 失败
            SUCCESS -- 成功
**************************************************/
int HikController::playposHik(DWORD pos)
{
	if(mPlay_no == -1)
		return SUCCESS;
	if (!PlayM4_SetPlayedTimeEx(mPlay_no, pos))
	{
		return FAIL;
	}else{
		return SUCCESS;
	}
}
/*************************************************
Function:   resumeHik
Desc:	    暂停播放
Input:      void
Return:		FAIL -- 失败
            SUCCESS -- 成功
**************************************************/
bool HikController::resumeHik()
{
	if(mPlay_no == -1)
		return SUCCESS;
	if (!PlayM4_Pause(mPlay_no,FALSE))
	{
		return FAIL;
	}else{
		return SUCCESS;
	}
}
/*************************************************
Function:   pauseHik
Desc:	    暂停播放
Input:      void
Return:		FAIL -- 失败
            SUCCESS -- 成功
**************************************************/
bool HikController::pauseHik()
{
	if(mPlay_no == -1)
		return SUCCESS;
	if (!PlayM4_Pause(mPlay_no,TRUE))
	{
		return FAIL;
	}else{
		return SUCCESS;
	}
}
/*************************************************
Function:   fastHik
Desc:	    播放快进
Input:      void
Return:		FAIL -- 失败
            SUCCESS -- 成功
**************************************************/
bool HikController::fastHik()
{
	if(mPlay_no == -1)
		return SUCCESS;
	if (!PlayM4_Fast(mPlay_no))
	{
		return FAIL;
	}else{
		return SUCCESS;
	}
}
/*************************************************
Function:   slowHik
Desc:	    播放慢放
Input:      times 慢放倍数 0<=times<=4
Return:		FAIL -- 失败
            SUCCESS -- 成功
**************************************************/
bool HikController::slowHik(int times)
{
	if(mPlay_no == -1)
		return SUCCESS;
	int cn = times>4?4:times;
	if(cn<0)
		times=0;

	for(int i=0;i<times;i++)
	{
		if (!PlayM4_Slow(mPlay_no))
		{
			return FAIL;
		}else{
			return SUCCESS;
		}
	}
}
/*************************************************
Function:   playHik
Desc:	    继续播放
Input:      void
Return:		FAIL -- 失败
            SUCCESS -- 成功
**************************************************/
/*
bool HikController::playHik()
{
	if(mPlay_no == -1)
		return SUCCESS;
	if (!PlayM4_Play(mPlay_no,wind))
	{
		return FAIL;
	}else{
		return SUCCESS;
	}
}*/
/*************************************************
Function:   FileRefDoneCB
Desc:	    建立索引文件回调函数
Input:      DWORD nPort 端口
            void* nUser 用户数据
Return:		void
**************************************************/
void CALLBACK HikController::FileRefDoneCB(DWORD nPort,void* nUser)
{
	HikController *ct = static_cast<HikController*>((void*)nUser);
	ct->g_bRefDone = TRUE; // assign mark to have loaded file
}
/*********************************************************
Function:	PlayBack
Desc:		开始播放视频
Input:	    播放文件名
Return:	    SUCCESS -- 成功
            ERROR_FAIL_OPEN_FILE -- 文件打开失败
            ERROR_FAIL_OPEN_WINDOW -- 视频输出到窗口失败
**********************************************************/
int HikController::playbackHik(CString filename)
{

	int iFilePosSel = 0;
	file=filename;
	PlayM4_Stop(mPlay_no);
	PlayM4_CloseFile(mPlay_no);


	g_bRefDone = FALSE;
	g_test = FALSE;
	PlayM4_SetFileRefCallBack(mPlay_no,&HikController::FileRefDoneCB,(void *)this);

	if (!PlayM4_OpenFile(mPlay_no, filename.GetBuffer(filename.GetLength())))
	{
		return ERROR_FAIL_OPEN_FILE;
	}


	if (PlayM4_Play(mPlay_no,mWind)==FALSE)
	{

		PlayM4_CloseFile(mPlay_no);
		return ERROR_FAIL_OPEN_WINDOW;
	}

	while (g_bRefDone == FALSE)
	{
		Sleep(2);
	}
	Sleep(500);
	return SUCCESS;
}


/*********************************************************
Function:	stopplaybackHik
Desc:		停止播放视频
Input:	
Return:  	SUCCESS 成功
            ERROR_FAIL_OPEN_FILE   文件打开失败
            ERROR_FAIL_OPEN_WINDOW 视频输出到窗口失败
**********************************************************/
int HikController::stopplaybackHik()
{
	PlayM4_Stop(mPlay_no);
	PlayM4_CloseFile(mPlay_no);
	return SUCCESS;
}
/*************************************************
Function:   stopplayliveHik
Desc:	    停止播放
Input:   	
Return:		SUCCESS 成功
            FAIL   失败
**************************************************/
bool HikController::stopplayliveHik()
{
	if(handle != -1)
	{

		NET_DVR_StopRealPlay(handle);
		handle=-1;
		return SUCCESS;
	}
	return FAIL;
}
/*************************************************
Function:   stopFileHik
Desc:	    停止录像
Input:   
Return:		SUCCESS 成功
            FAIL   失败
**************************************************/
/*bool HikController::stopFileHik()
{
	if(!NET_DVR_StopSaveRealData(handle))
	{
		//MessageBox("停止录像失败");
		return FAIL;
	}
	return SUCCESS;

}*/
/*************************************************
Function:   DoGetDeviceResoureCfg
Desc:	    获得设备资源配置
Input:      设备info
            channel号	
Return:		void
**************************************************/
void HikController::dogetdeviceresourecfgHik(LOCAL_DEVICE_INFO *info,LONG channel)
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
Function:   GetDecoderCfg
Desc:	    获得解码器配置
Input:      设备info
Return:		void
**************************************************/
void HikController::getdecodercfgHik(LOCAL_DEVICE_INFO *info)
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


/*************************************************
Function:   playliveHik
Desc:	    直播录像
Input:      设备info
Return:		handle 成功
            FAIL 失败
**************************************************/
bool HikController::playliveHik(bool isPlay)
{

	NET_DVR_PREVIEWINFO ClientInfo = {0};
	ClientInfo.lChannel     = 1;
	ClientInfo.dwStreamType    = 0;
	ClientInfo.dwLinkMode = 0;

	if(handle != -1){
		NET_DVR_StopRealPlay(handle);
	}
	

	if(isPlay){
		ClientInfo.hPlayWnd = mWind;
	}else{
		ClientInfo.hPlayWnd = NULL;
	}


	TRACE("Channel number:%d\n",ClientInfo.lChannel);
	handle = NET_DVR_RealPlay_V40(m_struDeviceInfo.lLoginID,&ClientInfo,NULL,NULL);
		
	if(-1 == handle)
	{
		DWORD err=NET_DVR_GetLastError();

		return FAIL;
	}
	return SUCCESS;

}

/*************************************************
Function:   savefileHik
Desc:	    保存录像
Input:      设备info
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
int HikController::savefileHik(CString file)
{
	char *RecName=file.GetBuffer(file.GetLength());
	if(NET_DVR_SaveRealData(handle,RecName))
	{
		return SUCCESS;
	}else{
		return FAIL;
	}

}
/*************************************************
Function:   stopsavefileHik
Desc:	    停止保存录像
Input:      设备info
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
int HikController::stopsavefileHik()
{
	if(!NET_DVR_StopSaveRealData(handle))
	{
		return FAIL;
	}
	return SUCCESS;
}
/*************************************************
Function:   loginHik
Desc:	    注册设备
Input:      设备ip 端口 用户名 密码 设备info
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
int HikController::loginHik()
{

	NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;
	memset(&DeviceInfoTmp,0,sizeof(NET_DVR_DEVICEINFO_V30));

	LONG lLoginID = NET_DVR_Login_V30( \
		mIp.GetBuffer(mIp.GetLength()), \
		mPort, \
		mName.GetBuffer(mName.GetLength()), \
		mPwd.GetBuffer(mPwd.GetLength()), \
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

	dogetdeviceresourecfgHik(&m_struDeviceInfo,channelNO);  //获取设备资源信息	
	getdecodercfgHik(&m_struDeviceInfo);                           //获取云台解码器信息 

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


	return SUCCESS;
}
/*************************************************
Function:   saveParamHik
Desc:	    保存参数到相机
Input:      亮度 对比度 锐度 饱和度 曝光模式 曝光时间 日夜模式
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
int HikController::saveParamHik(BYTE brightness,BYTE contrast,BYTE sharpness,BYTE saturation,int exp_mode,int exp_time,int daynight)
{
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

			return SUCCESS;
		}else{
			return FAIL;
		}

	}else{
		return FAIL;
	}
}
/*************************************************
Function:   saveParamHik
Desc:	    从相机获取参数
Input:      void
Return:		亮度 对比度 锐度 饱和度 曝光模式 曝光时间 日夜模式
			0--失败
**************************************************/
PARAM_STRU HikController::getParamHik()
{
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
Function:   logoutHik
Desc:	    注册设备
Input:      设备info
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
int HikController::logoutHik()
{
	if(NET_DVR_Logout_V30(m_struDeviceInfo.lLoginID)){
		return SUCCESS;

	}else{
		return FAIL;
	} 
}


HikController::~HikController(void)
{
}
