/*
File name:   gaoqingDlg.h
File Author: Li Weichen
Date:        2016.3.1
*/
#pragma once
//==============================HIK INCLUDE=====================
//#include "hik/GeneralDef.h"
//==============================================================
#include "HikController.h"
#include "DialogLogin.h"
#include "DialogFind.h"
#include "DialogSetting.h"
#include "afxcmn.h"
#include "SerialPort.h"
#include <direct.h>
#include "common.h"
#include "CMySliderCtrl.h"


#define USED_PORT_A 90		//card decode port
#define USED_PORT_B 91		//card decode port
#define USED_PORT_C 92		//card decode port
#define USED_PORT_D 93		//card decode port

// CgaoqingDlg dialog
class CgaoqingDlg : public CDialog
{
// Construction
public:
	CgaoqingDlg(CWnd* pParent = NULL);	// standard constructor
	
public:
	//===== member variables ====
	CMySliderCtrl playslider;
	bool adjust;
	bool camer_a;
	bool camer_b;
	bool camer_c;
	bool camer_d;
	int playSpeed;
	BOOL s_Play;

	//===== member function ====
	CString getConfig(CString category,CString item,CString def);
	DWORD getPlayedTime(bool isRec);
	int stopVideo(bool isRec);
	static void setConfig(CString category,CString item,CString val);
	PARAM_STRU getParams();
	int saveParams(BYTE bright,BYTE contrast,BYTE sharpness,BYTE saturation,int exp_mode,int exp_time,int daynight);
	int playVideo(CString patha,CString pathb,CString pathc,CString pathd);
	DWORD getVideoTime(bool isRec);
	int playVideoPos(DWORD pos);

private:
	
	//===== member variables ====
	
	CTabCtrl tabSettings;
	CDialogLogin dLogin;
	CDialogFind dFind;
	CDialogSetting dSetting;


	CString train_id;
	DWORD stime;
	int maxScreen;
	bool maxScreen_isMax;
	CString path;
	int slowtimes;
	bool direction;
	unsigned int car_n;
	char car_number[20]; 
	CString index_str;
	unsigned int car_speed;
	bool is_minimized;
	CString foldername;
	CString timename;
	HANDLE pFile;

	LONG m_lPlayHandle;
	CIPAddressCtrl	m_ctrlDevIp;
	UINT	m_nDevPort;
	CString	m_csUser;
	CString	m_csPWD;
	HTREEITEM m_hDevItem;
	HWND m_hPlayWnd;

	//===== camera variables ====
	HikController *mHik_a;
	HikController *mHik_b;
	HikController *mHik_c;
	HikController *mHik_d;

	//===== UART variables ====
	unsigned int Port_N        ;
	unsigned int Port_BAUD     ;
    char         Port_PARITY   ;
	unsigned int Port_DBITS    ;
	unsigned int Port_STOP     ;

	CSerialPort *ser_com;
	static char rx_ser[50];
	static int  count_ser;
	static int  longth_ser;
	static char command_ser;
	
	//===== db variables ====
	CDatabase m_db; 
	CRecordset rs_train;//,rs_details;
	

	//===== layout variables ====
	CRect screenA,screenB,screenC,screenD,screenMax,screenProcess,screenPlay,screenFast,screenSlow;
	

	//===== member function ====
	void initParam();
	int pauseVideo();
	int resumeVideo();
	int fastVideo();
	int slowVideo();
	CString getCStringFromConfig(CString cam,CString sub);
	bool loginCamera();
	void logoutCamera();
	void startUART();
	void stopUART();
	void ser_com_read(char rd);
	void Command_Disposal_Ser(char command);
	CRect ChangeSize(CWnd *pWnd, int left, int right, int width, int height);
	CRect getSize(CWnd *pWnd);
	void saveSize();
	void restoreSize();
	void ShowLivePlayBackState();
	int playLive(bool display);
	void StopPlay();
	void StopRecord();
	void StartPlay(int iChanIndex);
	void StartRecord();
	void PlayBack();
	void initView();
	void post_message(LPCTSTR format, ...);

	//===== message fuction =====
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeEditPort();
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderPlay(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonRecPlay();
	afx_msg void OnBnClickedButtonRecFast();
	afx_msg void OnBnClickedButtonRecSlow();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnStnDblclickStaticPlayA();
	afx_msg void OnStnDblclickStaticPlayB();
	afx_msg void OnStnDblclickStaticPlayC();
	afx_msg void OnStnDblclickStaticPlayD();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void OnOK();
 
// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_GAOQING_DIALOG };
	HICON m_hIcon;

	// Generated message map functions
	
	
	DECLARE_MESSAGE_MAP()

};
