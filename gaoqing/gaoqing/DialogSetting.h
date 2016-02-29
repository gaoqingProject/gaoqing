#pragma once
#include "afxwin.h"
#include "common.h"
#include <vector>
using namespace std;
// CDialogSetting dialog
class CgaoqingDlg;

class CDialogSetting : public CDialog
{
	DECLARE_DYNAMIC(CDialogSetting)

public:
	CDialogSetting(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogSetting();
	BOOL OnInitDialog();
	void Logout();
	CString getCStringFromConfig(CString cam,CString sub);
	void setCStringToConfig(CString cam,CString sub,CString str);
	void saveParams(BYTE bright,BYTE contrast,BYTE sharpness,BYTE saturation,int exp_mode,int exp_time,int daynight);
	PARAM_STRU getParams();

	CgaoqingDlg *parent;

	void OnOK();
	void initParam();
int transCStringToInt(CString addstr);
CString getIp();
vector<CString> SplitCString(CString strSource, CString ch);
//int getPort();

	CIPAddressCtrl ip_a;
	CIPAddressCtrl ip_b;
	CIPAddressCtrl ip_c;
	CIPAddressCtrl ip_d;


	int port_a;
	int port_b;
	int port_c;
	int port_d;


public:
	CString pwd_a;
	CString pwd_b;
	CString pwd_c;
	CString pwd_d;
public:
	CString user_a;
	CString user_b;
	CString user_c;
	CString user_d;

public:
	CButton isAExist;
public:
	CButton isBExist;
public:
	CButton isCExist;
public:
	CButton isDExist;




// Dialog Data
	enum { IDD = IDD_DIALOG_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSave();
public:
	CComboBox c_Brightness;
public:
	CComboBox c_Contrast;
public:
	CComboBox c_Sharpness;
public:
	CComboBox c_Saturation;
public:
	CComboBox c_ExpMode;
public:
	CComboBox c_ExpTime;
public:
	CComboBox c_DayNight;
public:
	afx_msg void OnCbnSelchangeBright();
public:
	afx_msg void OnCbnSelchangeContrast();
public:
	afx_msg void OnCbnSelchangeSharpness();
public:
	afx_msg void OnCbnSelchangeSaturation();
public:
	afx_msg void OnCbnSelchangeExpMode();
public:
	afx_msg void OnCbnSelchangeExpTime();
public:
	afx_msg void OnCbnSelchangeDaynight();
public:
	afx_msg void OnCbnSelchangeBrightness();

public:
	afx_msg void OnCameraACheck();

public:
	afx_msg void OnCameraBCheck();
public:
	afx_msg void OnCameraCCheck();
public:
	afx_msg void OnCameraDCheck();

public:
	afx_msg void OnIpnFieldchangedIpaddressA(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnIpnFieldchangedIpaddressC(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnIpnFieldchangedIpaddressB(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnIpnFieldchangedIpaddressD(NMHDR *pNMHDR, LRESULT *pResult);

public:
	afx_msg void OnEnChangeEditPortA();
public:
	afx_msg void OnEnChangeEditPortB();
public:
	afx_msg void OnEnChangeEditPortC();
public:
	afx_msg void OnEnChangeEditPortD();
public:
	afx_msg void OnEnChangeEditUserA();
public:
	afx_msg void OnEnChangeEditPwdA();
public:
	afx_msg void OnEnChangeEditUserC();
public:
	afx_msg void OnEnChangeEditPwdC();
public:
	afx_msg void OnEnChangeEditUserB();
public:
	afx_msg void OnEnChangeEditPwdB();
public:
	afx_msg void OnEnChangeEditUserD();
public:
	afx_msg void OnEnChangeEditPwdD();














public:
	afx_msg void OnBnClickedButtonLogin();
public:
	CButton loginSet;
public:
	CEdit passwordInput;
};
