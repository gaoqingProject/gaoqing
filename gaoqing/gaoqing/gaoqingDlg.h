// gaoqingDlg.h : header file
//

#pragma once
//==============================HIK INCLUDE=====================
#include "hik/GeneralDef.h"
//==============================================================

// CgaoqingDlg dialog
class CgaoqingDlg : public CDialog
{
// Construction
public:
	CgaoqingDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GAOQING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
//===============HIK Variables==================
	LOCAL_DEVICE_INFO m_struDeviceInfo;
	LONG m_lPlayHandle;
//==============================================

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
};
