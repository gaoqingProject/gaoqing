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
}

void CgaoqingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CgaoqingDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
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

