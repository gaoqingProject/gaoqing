/*
File name:   DialogLogin.cpp
File Author: Li Weichen
Date:        2016.3.1
*/
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
}

CDialogLogin::~CDialogLogin()
{
}

void CDialogLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, output_list_);
}
/*************************************************
Function:  OnInitDialog
Desc:	   ��ʼ���Ի���
Input:     void
Return:	   true
**************************************************/
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
/*************************************************
Function:  OnSize
Desc:	   �Ի����С�ı亯��
Input:     UINT nType 
		   int cx ���
		   int cy �߶�
Return:	   void
**************************************************/
void CDialogLogin::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CWnd *pWnd; 
	pWnd = GetDlgItem(IDC_LIST1);
	if(pWnd != NULL){
		CRect new_rect;
		new_rect.left=10;//�����ؼ���С  
		new_rect.right=cx-10;  
		new_rect.top=0;  
		new_rect.bottom=cy;  
		pWnd->MoveWindow(new_rect);//���ÿؼ���С  
	}
}
