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

	
	if(mHik->playbackHik("test.mp4")==SUCESS){
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
	if(mHik->stopFileHik()==SUCESS){
		m_bIsRecording = FALSE;
		if(mHik->stopPlayHik()==SUCESS){

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
			GetDlgItem(IDC_BUTTON_START)->SetWindowText("Í£Ö¹");

			
		}
	}else{
		CgaoqingDlg *p = (CgaoqingDlg *)parent;
		logoutCamera();
		p->stopUART();
		GetDlgItem(IDC_BUTTON_START)->SetWindowText("¿ªÊ¼");
	}
	
}*/

