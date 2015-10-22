// gaoqingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "gaoqing.h"
#include "gaoqingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CgaoqingDlg dialog




CgaoqingDlg::CgaoqingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CgaoqingDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//===============HIK==================
	m_bIsLogin = FALSE;
	m_bIsPlaying = FALSE;
	m_bIsRecording = FALSE;
	//====================================
}

void CgaoqingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_IPADDRESS_DEV, m_ctrlDevIp);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nDevPort);
	DDX_Text(pDX, IDC_EDIT_USER, m_csUser);
	DDX_Text(pDX, IDC_EDIT_PWD, m_csPWD);


}

BEGIN_MESSAGE_MAP(CgaoqingDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CgaoqingDlg::OnLogin)
	ON_BN_CLICKED(IDC_BUTTON2, &CgaoqingDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CgaoqingDlg message handlers

BOOL CgaoqingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	//=================================HIK Operation=======================

	//=================step 1 ===================
	NET_DVR_CLIENTINFO ClientInfo;
	ClientInfo.hPlayWnd     = GetDlgItem(IDC_STATIC_PLAY)->m_hWnd;
	ClientInfo.lChannel     = 1;
	ClientInfo.lLinkMode    = 0;
    ClientInfo.sMultiCastIP = NULL;
	TRACE("Channel number:%d\n",ClientInfo.lChannel);
	m_lPlayHandle = NET_DVR_RealPlay_V30(m_struDeviceInfo.lLoginID,&ClientInfo,NULL,NULL,TRUE);
	if(-1 == m_lPlayHandle)
	{
	}

	//==================step 2.========================

	char RecName[256] = {0};
	
	CTime CurTime = CTime::GetCurrentTime();;
	sprintf(
		RecName,
		"%04d%02d%02d%02d%02d%02d_ch%02d.mp4",
		CurTime.GetYear(), \
		CurTime.GetMonth(), \
		CurTime.GetDay(), \
		CurTime.GetHour(), \
		CurTime.GetMinute(), \
		CurTime.GetSecond(), \
		m_struDeviceInfo.struChanInfo[0].iChanIndex
		);

	 if(!NET_DVR_SaveRealData(m_lPlayHandle,RecName))
	 {
	 }
	//=====================================================================

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CgaoqingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CgaoqingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*************************************************
������:    	DoLogin
��������:	���豸ע��
�������:   
�������:   			
����ֵ:		
**************************************************/
BOOL CgaoqingDlg::DoLogin()
{
	UpdateData(TRUE);
	CString DeviceIp;
	BYTE nField0,nField1,nField2,nField3; 
	m_ctrlDevIp.GetAddress(nField0,nField1,nField2,nField3);
	DeviceIp.Format("%d.%d.%d.%d",nField0,nField1,nField2,nField3);
	
	NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;
	memset(&DeviceInfoTmp,0,sizeof(NET_DVR_DEVICEINFO_V30));
	
	LONG lLoginID = NET_DVR_Login_V30(DeviceIp.GetBuffer(DeviceIp.GetLength()),m_nDevPort, \
		m_csUser.GetBuffer(m_csUser.GetLength()),m_csPWD.GetBuffer(m_csPWD.GetLength()),&DeviceInfoTmp);
	if(lLoginID == -1)
	{
		MessageBox("Login to Device failed!\n");
		return FALSE;
	}
    m_struDeviceInfo.lLoginID = lLoginID;
	m_struDeviceInfo.iDeviceChanNum = DeviceInfoTmp.byChanNum;
    m_struDeviceInfo.iIPChanNum = DeviceInfoTmp.byIPChanNum;
    m_struDeviceInfo.iStartChan  = DeviceInfoTmp.byStartChan;
    m_struDeviceInfo.iIPStartChan  = DeviceInfoTmp.byStartDChan;
	return TRUE;
}
/*************************************************
������:    	DoGetDeviceResoureCfg
��������:	��ȡ�豸��ͨ����Դ
�������:   
�������:   			
����ֵ:		
**************************************************/
void CgaoqingDlg::DoGetDeviceResoureCfg()
{
	NET_DVR_IPPARACFG_V40 IpAccessCfg;
	memset(&IpAccessCfg,0,sizeof(IpAccessCfg));	
	DWORD  dwReturned;

	m_struDeviceInfo.bIPRet = NET_DVR_GetDVRConfig(m_struDeviceInfo.lLoginID,NET_DVR_GET_IPPARACFG_V40,0,&IpAccessCfg,sizeof(NET_DVR_IPPARACFG_V40),&dwReturned);

	int i;
    if(!m_struDeviceInfo.bIPRet)   //��֧��ip����,9000�����豸��֧�ֽ���ģ��ͨ��
	{
		for(i=0; i<MAX_ANALOG_CHANNUM; i++)
		{
			if (i < m_struDeviceInfo.iDeviceChanNum)
			{
				sprintf(m_struDeviceInfo.struChanInfo[i].chChanName,"camera%d",i+m_struDeviceInfo.iStartChan);
				m_struDeviceInfo.struChanInfo[i].iChanIndex=i+m_struDeviceInfo.iStartChan;  //ͨ����
				m_struDeviceInfo.struChanInfo[i].bEnable = TRUE;
				
			}
			else
			{
				m_struDeviceInfo.struChanInfo[i].iChanIndex = -1;
				m_struDeviceInfo.struChanInfo[i].bEnable = FALSE;
				sprintf(m_struDeviceInfo.struChanInfo[i].chChanName, "");	
			}
		}
	}
	else        //֧��IP���룬9000�豸
	{
		for(i=0; i<MAX_ANALOG_CHANNUM; i++)  //ģ��ͨ��
		{
			if (i < m_struDeviceInfo.iDeviceChanNum)
			{
				sprintf(m_struDeviceInfo.struChanInfo[i].chChanName,"camera%d",i+m_struDeviceInfo.iStartChan);
				m_struDeviceInfo.struChanInfo[i].iChanIndex=i+m_struDeviceInfo.iStartChan;
				if(IpAccessCfg.byAnalogChanEnable[i])
				{
					m_struDeviceInfo.struChanInfo[i].bEnable = TRUE;
				}
				else
				{
					m_struDeviceInfo.struChanInfo[i].bEnable = FALSE;
				}
				
			}
			else//clear the state of other channel
			{
				m_struDeviceInfo.struChanInfo[i].iChanIndex = -1;
				m_struDeviceInfo.struChanInfo[i].bEnable = FALSE;
				sprintf(m_struDeviceInfo.struChanInfo[i].chChanName, "");	
			}		
		}

		//����ͨ��
		for(i=0; i<MAX_IP_CHANNEL; i++)
		{
			if(IpAccessCfg.struStreamMode[i].uGetStream.struChanInfo.byEnable)  //ipͨ������
			{
				m_struDeviceInfo.struChanInfo[i+MAX_ANALOG_CHANNUM].bEnable = TRUE;
                m_struDeviceInfo.struChanInfo[i+MAX_ANALOG_CHANNUM].iChanIndex = i+IpAccessCfg.dwStartDChan;
				sprintf(m_struDeviceInfo.struChanInfo[i+MAX_ANALOG_CHANNUM].chChanName,"IP Camera %d",i+1);

			}
			else
			{
               m_struDeviceInfo.struChanInfo[i+MAX_ANALOG_CHANNUM].bEnable = FALSE;
			    m_struDeviceInfo.struChanInfo[i+MAX_ANALOG_CHANNUM].iChanIndex = -1;
			}
		}
		
		
	}

}
/*************************************************
������:    	GetDecoderCfg
��������:	��ȡ��̨��������Ϣ
�������:   
�������:   			
����ֵ:		
**************************************************/
void CgaoqingDlg::GetDecoderCfg()
{
     NET_DVR_DECODERCFG_V30 DecoderCfg;
	 DWORD  dwReturned;
	 BOOL bRet;
		
      
	 //��ȡͨ����������Ϣ
	 for(int i=0; i<MAX_CHANNUM_V30; i++)
	 {
		 if(m_struDeviceInfo.struChanInfo[i].bEnable)
		 {
			 memset(&DecoderCfg,0,sizeof(NET_DVR_DECODERCFG_V30));
			 bRet = NET_DVR_GetDVRConfig(m_struDeviceInfo.lLoginID,NET_DVR_GET_DECODERCFG_V30 , \
				 m_struDeviceInfo.struChanInfo[i].iChanIndex,&DecoderCfg,sizeof(NET_DVR_DECODERCFG_V30),&dwReturned);
			 if(!bRet)
			 {
				 TRACE("Get DecderCfg failed,Chan:%d\n",m_struDeviceInfo.struChanInfo[i].iChanIndex);
				 continue;
			 }

			 memcpy(&m_struDeviceInfo.struChanInfo[i].struDecodercfg,&DecoderCfg,sizeof(NET_DVR_DECODERCFG_V30));
		 }
		 
	 }

}
/*************************************************
������:    	StopPlay
��������:	ֹͣ����
�������:   
�������:   			
����ֵ:		
**************************************************/
void CgaoqingDlg::StopPlay()
{
	if(m_lPlayHandle != -1)
	{
		if(m_bIsRecording)  //����¼����ֹͣ
		{
			StopRecord();
		}
		NET_DVR_StopRealPlay(m_lPlayHandle);
		m_lPlayHandle=-1;
		m_bIsPlaying = FALSE;
		GetDlgItem(IDC_STATIC_PLAY)->Invalidate();
	}
}
/*************************************************
������:    	StopRecord
��������:	ֹͣ¼��
�������:   
�������:   			
����ֵ:		
**************************************************/
void CgaoqingDlg::StopRecord()
{
    if(!NET_DVR_StopSaveRealData(m_lPlayHandle))
	{
		MessageBox("ֹͣ¼��ʧ��");
		return;
	}
	m_bIsRecording = FALSE;
	 //GetDlgItem(IDC_BUTTON_RECORD)->SetWindowText("¼��");
}
void CgaoqingDlg::OnLogin()
{
	if(!m_bIsLogin)    //login
	{
		if(!DoLogin())
			return;
		DoGetDeviceResoureCfg();  //��ȡ�豸��Դ��Ϣ	
		//CreateDeviceTree();        //����ͨ����
		GetDecoderCfg();                           //��ȡ��̨��������Ϣ
		//InitDecoderReferCtrl();         //��ʼ����������ؿؼ�      
		GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowText("Logout");
		m_bIsLogin = TRUE;
		
	}
	else      //logout
	{
		
		
		if(m_bIsPlaying|| m_bIsRecording)
		{
			MessageBox("Stop Play or record first!");
			return;
		}
        NET_DVR_Logout_V30(m_struDeviceInfo.lLoginID); 
		GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowText("Login");
		//m_ctrlTreeChan.DeleteAllItems();
		m_struDeviceInfo.lLoginID = -1;
		m_bIsLogin = FALSE;
	}
}



/*************************************************
������:    	StartPlay
��������:	��ʼһ·����
�������:   ChanIndex-ͨ����
�������:   			
����ֵ:		
**************************************************/
void CgaoqingDlg::StartPlay(int iChanIndex)
{
	NET_DVR_CLIENTINFO ClientInfo;
	ClientInfo.hPlayWnd     = GetDlgItem(IDC_STATIC_PLAY)->m_hWnd;
	ClientInfo.lChannel     = 1;
	ClientInfo.lLinkMode    = 0;
    ClientInfo.sMultiCastIP = NULL;
	TRACE("Channel number:%d\n",ClientInfo.lChannel);
	m_lPlayHandle = NET_DVR_RealPlay_V30(m_struDeviceInfo.lLoginID,&ClientInfo,NULL,NULL,TRUE);
	if(-1 == m_lPlayHandle)
	{
		DWORD err=NET_DVR_GetLastError();
		CString m_csErr;
        m_csErr.Format("���ų����������%d",err);
		MessageBox(m_csErr);
	}

	m_bIsPlaying = TRUE;
	//GetDlgItem(IDC_BUTTON_PLAY)->SetWindowText("ֹͣ����");

}
void CgaoqingDlg::OnClose() 
{
	//=================HIK=========================
	if (m_bIsLogin)
	{
        StopPlay();		
        NET_DVR_Logout_V30(m_struDeviceInfo.lLoginID); 
	}
	//==============================================
	CDialog::OnClose();
}

void CgaoqingDlg::OnBnClickedButton2()
{
	 StartPlay(0);
		 m_bIsPlaying = TRUE;
	// TODO: Add your control notification handler code here
}
