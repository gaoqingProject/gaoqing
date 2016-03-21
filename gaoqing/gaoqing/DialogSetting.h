/*
File name:   DialogSetting.h
File Author: Li Weichen
Date:        2016.3.1
*/
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
	
	

	//===== member variables ====
	CgaoqingDlg *parent;

	//===== member function ====
	void Logout();
	
private:
	
	//===== member variables ====
	
	CIPAddressCtrl ip_a;
	CIPAddressCtrl ip_b;
	CIPAddressCtrl ip_c;
	CIPAddressCtrl ip_d;
	int port_a;
	int port_b;
	int port_c;
	int port_d;
	CString pwd_a;
	CString pwd_b;
	CString pwd_c;
	CString pwd_d;
	CString user_a;
	CString user_b;
	CString user_c;
	CString user_d;
	CButton isAExist;
	CButton isBExist;
	CButton isCExist;
	CButton isDExist;

	CComboBox c_Brightness;
	CComboBox c_Contrast;
	CComboBox c_Sharpness;
	CComboBox c_Saturation;
	CComboBox c_ExpMode;
	CComboBox c_ExpTime;
	CComboBox c_DayNight;

	CButton loginSet;
	CEdit passwordInput;

	//===== member function ====
	
	void initParam();
	int transCStringToInt(CString addstr);
	CString getIp();
	vector<CString> SplitCString(CString strSource, CString ch);

	
	CString getCStringFromConfig(CString cam,CString sub);
	void setCStringToConfig(CString cam,CString sub,CString str);
	void saveParams(BYTE bright,BYTE contrast,BYTE sharpness,BYTE saturation,int exp_mode,int exp_time,int daynight);
	PARAM_STRU getParams();

	//===== message fuction =====
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnCameraACheck();
	afx_msg void OnCameraBCheck();
	afx_msg void OnCameraCCheck();
	afx_msg void OnCameraDCheck();
	afx_msg void OnIpnFieldchangedIpaddressA(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnIpnFieldchangedIpaddressC(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnIpnFieldchangedIpaddressB(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnIpnFieldchangedIpaddressD(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditPortA();
	afx_msg void OnEnChangeEditPortB();
	afx_msg void OnEnChangeEditPortC();
	afx_msg void OnEnChangeEditPortD();
	afx_msg void OnEnChangeEditUserA();
	afx_msg void OnEnChangeEditPwdA();
	afx_msg void OnEnChangeEditUserC();
	afx_msg void OnEnChangeEditPwdC();
	afx_msg void OnEnChangeEditUserB();
	afx_msg void OnEnChangeEditPwdB();
	afx_msg void OnEnChangeEditUserD();
	afx_msg void OnEnChangeEditPwdD();
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	void OnOK();
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_DIALOG_SETTING };
	DECLARE_MESSAGE_MAP()
};
