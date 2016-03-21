/*
File name:   DialogLogin.h
File Author: Li Weichen
Date:        2016.3.1
*/
#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "HikController.h"


// CDialogLogin dialog
class CgaoqingDlg;
class CDialogLogin : public CDialog
{
	DECLARE_DYNAMIC(CDialogLogin)

public:
	CDialogLogin(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogLogin();

	//===== member variables ====
	CgaoqingDlg *parent;
	CListBox output_list_;

	//===== message fuction =====
	void OnOK();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonx();

// Dialog Data
	enum { IDD = IDD_DIALOG_LOGIN };
	
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	
};
