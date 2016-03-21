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
Desc:	   ��ʼ��sdk
Input:     void
Return:	   void
**************************************************/
void HikController::initHik(void)
{
	NET_DVR_Init();
}
/*************************************************
Function:   playtimeHik
Desc:	    ��ȡ��ǰ�ļ��Ĳ��ų���
Input:      void
Return:		0 -- FAIL
            others -- �ļ����ų���
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
Desc:	    ��ȡ����ʱ��
Input:      void
Return:		����ʱ�䣨ms��
**************************************************/
DWORD HikController::playedtimeHik()
{
	return PlayM4_GetPlayedTimeEx(mPlay_no);
}
/*************************************************
Function:   playposHik
Desc:	    ���ò���λ��
Input:      ����ʱ��λ�ã�ms��
Return:		FAIL -- ʧ��
            SUCCESS -- �ɹ�
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
Desc:	    ��ͣ����
Input:      void
Return:		FAIL -- ʧ��
            SUCCESS -- �ɹ�
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
Desc:	    ��ͣ����
Input:      void
Return:		FAIL -- ʧ��
            SUCCESS -- �ɹ�
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
Desc:	    ���ſ��
Input:      void
Return:		FAIL -- ʧ��
            SUCCESS -- �ɹ�
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
Desc:	    ��������
Input:      times ���ű��� 0<=times<=4
Return:		FAIL -- ʧ��
            SUCCESS -- �ɹ�
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
Desc:	    ��������
Input:      void
Return:		FAIL -- ʧ��
            SUCCESS -- �ɹ�
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
Desc:	    ���������ļ��ص�����
Input:      DWORD nPort �˿�
            void* nUser �û�����
Return:		void
**************************************************/
void CALLBACK HikController::FileRefDoneCB(DWORD nPort,void* nUser)
{
	HikController *ct = static_cast<HikController*>((void*)nUser);
	ct->g_bRefDone = TRUE; // assign mark to have loaded file
}
/*********************************************************
Function:	PlayBack
Desc:		��ʼ������Ƶ
Input:	    �����ļ���
Return:	    SUCCESS -- �ɹ�
            ERROR_FAIL_OPEN_FILE -- �ļ���ʧ��
            ERROR_FAIL_OPEN_WINDOW -- ��Ƶ���������ʧ��
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
Desc:		ֹͣ������Ƶ
Input:	
Return:  	SUCCESS �ɹ�
            ERROR_FAIL_OPEN_FILE   �ļ���ʧ��
            ERROR_FAIL_OPEN_WINDOW ��Ƶ���������ʧ��
**********************************************************/
int HikController::stopplaybackHik()
{
	PlayM4_Stop(mPlay_no);
	PlayM4_CloseFile(mPlay_no);
	return SUCCESS;
}
/*************************************************
Function:   stopplayliveHik
Desc:	    ֹͣ����
Input:   	
Return:		SUCCESS �ɹ�
            FAIL   ʧ��
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
Desc:	    ֹͣ¼��
Input:   
Return:		SUCCESS �ɹ�
            FAIL   ʧ��
**************************************************/
/*bool HikController::stopFileHik()
{
	if(!NET_DVR_StopSaveRealData(handle))
	{
		//MessageBox("ֹͣ¼��ʧ��");
		return FAIL;
	}
	return SUCCESS;

}*/
/*************************************************
Function:   DoGetDeviceResoureCfg
Desc:	    ����豸��Դ����
Input:      �豸info
            channel��	
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
	if(!info->bIPRet)   //��֧��ip����,9000�����豸��֧�ֽ���ģ��ͨ��
	{
		for(i=0; i<MAX_ANALOG_CHANNUM; i++)
		{
			if (i < info->iDeviceChanNum)
			{
				sprintf(info->struChanInfo[i].chChanName,"camera%d",i+info->iStartChan);
				info->struChanInfo[i].iChanIndex=i+info->iStartChan;  //ͨ����
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
	else        //֧��IP���룬9000�豸
	{
		for(i=0; i<MAX_ANALOG_CHANNUM; i++)  //ģ��ͨ��
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

		//����ͨ��
		for(i=0; i<MAX_IP_CHANNEL; i++)
		{
			if(IpAccessCfg.struStreamMode[i].uGetStream.struChanInfo.byEnable)  //ipͨ������
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
Desc:	    ��ý���������
Input:      �豸info
Return:		void
**************************************************/
void HikController::getdecodercfgHik(LOCAL_DEVICE_INFO *info)
{
	NET_DVR_DECODERCFG_V30 DecoderCfg;
	DWORD  dwReturned;
	BOOL bRet;

	//��ȡͨ����������Ϣ
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
Desc:	    ֱ��¼��
Input:      �豸info
Return:		handle �ɹ�
            FAIL ʧ��
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
Desc:	    ����¼��
Input:      �豸info
Return:		SUCCESS �ɹ�
            FAIL ʧ��
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
Desc:	    ֹͣ����¼��
Input:      �豸info
Return:		SUCCESS �ɹ�
            FAIL ʧ��
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
Desc:	    ע���豸
Input:      �豸ip �˿� �û��� ���� �豸info
Return:		SUCCESS �ɹ�
            FAIL ʧ��
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

	dogetdeviceresourecfgHik(&m_struDeviceInfo,channelNO);  //��ȡ�豸��Դ��Ϣ	
	getdecodercfgHik(&m_struDeviceInfo);                           //��ȡ��̨��������Ϣ 

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
Desc:	    ������������
Input:      ���� �Աȶ� ��� ���Ͷ� �ع�ģʽ �ع�ʱ�� ��ҹģʽ
Return:		SUCCESS �ɹ�
            FAIL ʧ��
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

		para.struExposure.byExposureMode = exp_mode; //0 �ֶ��ع� 1�Զ��ع�
		para.struExposure.dwVideoExposureSet = exp_time; //us

		para.struDayNight.byDayNightFilterType =daynight; //��ҹ�л���0-���죬1-ҹ��2-�Զ���3-��ʱ��4-�������봥��

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
Desc:	    �������ȡ����
Input:      void
Return:		���� �Աȶ� ��� ���Ͷ� �ع�ģʽ �ع�ʱ�� ��ҹģʽ
			0--ʧ��
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

		str.exposuremode = para.struExposure.byExposureMode; /*0 �ֶ��ع� 1�Զ��ع�*/
		str.exposuretime = para.struExposure.dwVideoExposureSet; //us

		str.daynight = para.struDayNight.byDayNightFilterType; /*��ҹ�л���0-���죬1-ҹ��2-�Զ���3-��ʱ��4-�������봥��*/ 

	}else{
		str.brightness = 0; //standard h264
		str.contrast = 0; //standard h264
		str.sharpness = 0; //standard h264
		str.saturation = 0; //standard h264

		str.exposuremode = 0; /*0 �ֶ��ع� 1�Զ��ع�*/
		str.exposuretime = 1; //us

		str.daynight = 0; 


	}
	return str;
}


/*************************************************
Function:   logoutHik
Desc:	    ע���豸
Input:      �豸info
Return:		SUCCESS �ɹ�
            FAIL ʧ��
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
