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
	CString getIp();
	int getPort();


	CgaoqingDlg *parent;
	
	CString getCStringFromConfig(CString cam,CString sub);
	//void setIntToConfig(CString cam,CString ip);
	void setCStringToConfig(CString cam,CString sub,CString str);

	int transCStringToInt(CString addstr);

	void OnOK();

// Dialog Data
	enum { IDD = IDD_DIALOG_LOGIN };
private:
	BOOL m_bIsLogin;
	BOOL m_bIsPlaying;
	BOOL m_bIsRecording;
	
	
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonx();
public:
	CListBox output_list_;


};
