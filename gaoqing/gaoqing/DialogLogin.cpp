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
Desc:	   初始化对话框
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
Desc:	   对话框大小改变函数
Input:     UINT nType 
		   int cx 宽度
		   int cy 高度
Return:	   void
**************************************************/
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
}
