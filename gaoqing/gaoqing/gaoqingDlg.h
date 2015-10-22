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
	BOOL m_bIsLogin;

	CIPAddressCtrl	m_ctrlDevIp;
	UINT	m_nDevPort;
	CString	m_csUser;
	CString	m_csPWD;
	HTREEITEM m_hDevItem;
	BOOL m_bIsPlaying;
	BOOL m_bIsRecording;
	//function
	BOOL DoLogin();
	void DoGetDeviceResoureCfg();
	void GetDecoderCfg();
	afx_msg void OnClose();
	void StopPlay();
	void StopRecord();
	void StartPlay(int iChanIndex);
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
public:
	afx_msg void OnLogin();
public:
	afx_msg void OnBnClickedButton2();
};
