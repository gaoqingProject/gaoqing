#include "StdAfx.h"
#include "HikController.h"
#define channelNO 0
HikController::HikController(void)
{
	
}
void HikController::initHik(void)
{
	//=================================HIK Operation=======================
	NET_DVR_Init();
	
}
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
BOOL HikController::loginHik(CString ip,UINT port,CString name,CString pwd,LOCAL_DEVICE_INFO *info)
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
		return FALSE;
	}
	info->lLoginID = lLoginID;
	info->iDeviceChanNum = DeviceInfoTmp.byChanNum;
	info->iIPChanNum = DeviceInfoTmp.byIPChanNum;
	info->iStartChan  = DeviceInfoTmp.byStartChan;
    info->iIPStartChan  = DeviceInfoTmp.byStartDChan;

	DoGetDeviceResoureCfg(info,channelNO);  //获取设备资源信息	
	GetDecoderCfg(info);                           //获取云台解码器信息 

	return TRUE;
}
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

void HikController::logoutHik(LOCAL_DEVICE_INFO *info)
{
        NET_DVR_Logout_V30(info->lLoginID); 
}
void HikController::displayHik(LOCAL_DEVICE_INFO *info)
{
	//=================step 1 ===================
	NET_DVR_CLIENTINFO ClientInfo;
	//ClientInfo.hPlayWnd     = GetDlgItem(IDC_STATIC_PLAYb)->m_hWnd;
	ClientInfo.lChannel     = 1;
	ClientInfo.lLinkMode    = 0;
    ClientInfo.sMultiCastIP = NULL;
	TRACE("Channel number:%d\n",ClientInfo.lChannel);
	LONG handle = NET_DVR_RealPlay_V30(info->lLoginID,&ClientInfo,NULL,NULL,TRUE);
	if(-1 == handle)
	{
	}

}
HikController::~HikController(void)
{
}
