// DialogLogin.cpp : implementation file
//

#include "stdafx.h"
#include "gaoqing.h"
#include "DialogLogin.h"
#include "gaoqingDlg.h"
#define TIMER_INTERVAL 500//local play back timer refresh interval

// CDialogLogin dialog

IMPLEMENT_DYNAMIC(CDialogLogin, CDialog)

CDialogLogin::CDialogLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogLogin::IDD, pParent)

{

	//===============HIK==================
	m_bIsLogin = FALSE;
	m_bIsPlaying = FALSE;
	m_bIsRecording = FALSE;
	//====================================
}

CDialogLogin::~CDialogLogin()
{
}

void CDialogLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, output_list_);
}
BOOL CDialogLogin::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	
	

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CDialogLogin, CDialog)
	
	
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CDialogLogin message handlers

void CDialogLogin::OnBnClickedButtonx()
{
	// TODO: Add your control notification handler code here
}
void CDialogLogin::OnOK(){

	return;
}
/*
void CDialogLogin::OnPlayClicked()
{

	
	if(mHik->playbackHik("test.mp4")==SUCCESS){
	}
}*/
/*
void CDialogLogin::OnLoginClicked()
{
	loginCamera(IDC_STATIC_PLAY_A);

}*/


//=========================================================================================





/*
void CDialogLogin::OnLogoutClicked()
{
	if(mHik->stopFileHik()==SUCCESS){
		m_bIsRecording = FALSE;
		if(mHik->stopplayliveHik()==SUCCESS){

		}
		mHik->logoutHik();
	}
}
*/
/*void CDialogLogin::OnStartClick()
{
	// TODO: Add your control notification handler code here

	if(!m_bIsLogin){
		if(loginCamera()){

			CgaoqingDlg *p = (CgaoqingDlg *)parent;
			p->startUART();
			GetDlgItem(IDC_BUTTON_START)->SetWindowText("停止");

			
		}
	}else{
		CgaoqingDlg *p = (CgaoqingDlg *)parent;
		logoutCamera();
		p->stopUART();
		GetDlgItem(IDC_BUTTON_START)->SetWindowText("开始");
	}
	
}*/


void CDialogLogin::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CWnd *pWnd; 
	pWnd = GetDlgItem(IDC_LIST1);
	if(pWnd != NULL){
		CRect new_rect;
		new_rect.left=10;//调整控件大小  
		new_rect.right=cx-10;  
		new_rect.top=0;  
		new_rect.bottom=cy;  
		pWnd->MoveWindow(new_rect);//设置控件大小  
	}
	// TODO: Add your message handler code here
}
