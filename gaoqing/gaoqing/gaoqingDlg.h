// gaoqingDlg.h : header file
//

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

// Dialog Data
	enum { IDD = IDD_GAOQING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	bool direction;
	unsigned int car_n;

	char car_number[20]; 
	CString index_str;
	unsigned int car_speed;

	bool is_minimized;










	

	CString foldername;
	CString timename;
	HANDLE pFile;
	//static bool grab_exist;



//===============HIK Variables==================
	LONG m_lPlayHandle;

	CIPAddressCtrl	m_ctrlDevIp;
	UINT	m_nDevPort;
	CString	m_csUser;
	CString	m_csPWD;
	HTREEITEM m_hDevItem;
	//function
	BOOL DoLogin();
	//void DoGetDeviceResoureCfg();
	//void GetDecoderCfg();
	afx_msg void OnClose();
	void StopPlay();
	void StopRecord();
	void StartPlay(int iChanIndex);
	void StartRecord();
	void PlayBack();
	HWND m_hPlayWnd;
	void initView();
	void post_message(LPCTSTR format, ...);
	void OnOK();

	HikController *mHik_a;
	HikController *mHik_b;
	HikController *mHik_c;
	HikController *mHik_d;
	
public:
	CString getConfig(CString category,CString item,CString def);
	static void setConfig(CString category,CString item,CString val);
	bool playVideo(CString patha,CString pathb,CString pathc,CString pathd);
	int stopVideo(bool isRec);
	int pauseVideo();
	int resumeVideo();
	int fastVideo();
	int slowVideo();
	int saveParams(BYTE bright,BYTE contrast,BYTE sharpness,BYTE saturation,int exp_mode,int exp_time,int daynight);
	PARAM_STRU getParams();

	DWORD getPlayedTime(bool isRec);

	DWORD getVideoTime(bool isRec);
	int playVideoPos(DWORD pos);
	CString getCStringFromConfig(CString cam,CString sub);
	bool loginCamera();
	void logoutCamera();

	void startUART();
	void stopUART();
	CSerialPort *ser_com;

	static char rx_ser[50];
	static int  count_ser;
	static int  longth_ser;
	static char command_ser;

	void ser_com_read(char rd);
	void Command_Disposal_Ser(char command);
	unsigned int Port_N        ;
	unsigned int Port_BAUD     ;
    char         Port_PARITY   ;
	unsigned int Port_DBITS    ;
	unsigned int Port_STOP     ;
 
	//void OnSysCommand(UINT nID, LPARAM lParam);
	CRect ChangeSize(CWnd *pWnd, int left, int right, int width, int height);
	CRect getSize(CWnd *pWnd);
	void saveSize();
	void restoreSize();

	void ShowLivePlayBackState();
	int playLive(bool display);



	bool camer_a;
	bool camer_b;
	bool camer_c;
	bool camer_d;
	BOOL s_Play;
	CRect screenA,screenB,screenC,screenD,screenMax,screenProcess,screenPlay,screenFast,screenSlow;
	CString path;
	int playSpeed;
	int slowtimes;
	bool adjust;
//==============================================
private:

	CString train_id;
	DWORD stime;
	void initParam();
	CDatabase m_db; 
	CRecordset rs_train;//,rs_details;
	int maxScreen;
	bool maxScreen_isMax;
	
	
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
public:
	afx_msg void OnBnClickedButton3();
public:
	afx_msg void OnEnChangeEditPort();
public:
	afx_msg void OnBnClickedButton1();
public:
	CTabCtrl tabSettings;
	CDialogLogin dLogin;
	CDialogFind dFind;
	CDialogSetting dSetting;
public:
	afx_msg void OnBnDoubleclickedButton2();
public:
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);

public:
	CMySliderCtrl playslider;
public:
	afx_msg void OnNMReleasedcaptureSliderPlay(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnBnClickedButtonRecPlay();
public:
	afx_msg void OnBnClickedButtonRecFast();
public:
	afx_msg void OnBnClickedButtonRecSlow();


public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnStnDblclickStaticPlayA();
public:
	afx_msg void OnStnDblclickStaticPlayB();
public:
	afx_msg void OnStnDblclickStaticPlayC();
public:
	afx_msg void OnStnDblclickStaticPlayD();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
 

};
