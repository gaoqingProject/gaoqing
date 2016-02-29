#pragma once
#include "afxcmn.h"
#include "afxdb.h"
#include <vector>
#include "afxdtctl.h"
#include "afxwin.h"
//#include "gaoqingDlg.h"
using namespace std;
class CgaoqingDlg;
typedef struct STRU_CAR_NO
{

	CString no;
	int time;
}CAR_NO;

// CDialogFind dialog
class CDialogFind : public CDialog
{
	DECLARE_DYNAMIC(CDialogFind)

public:
	CDialogFind(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogFind();
	BOOL OnInitDialog();
	void ShowPlayBackState();
// Dialog Data
	enum { IDD = IDD_DIALOG_FIND };

	void findTrains(CString sql,CString carno);
	CDatabase m_db; 
	CRecordset rs_train,rs_details;
	HTREEITEM hRoot;     // 树的根节点的句柄   
    HTREEITEM hCataItem; // 可表示任一分类节点的句柄   
    HTREEITEM hArtItem;  // 可表示任一文章节点的句柄 
	void OnOK();
private:
	DWORD len;
	



	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	vector<CAR_NO> findCars(CString uri);
	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl tran_list;
	CgaoqingDlg *parent;
public:
	afx_msg void OnTvnSelchangedTrainList(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnTvnGetInfoTipTrainList(NMHDR *pNMHDR, LRESULT *pResult);

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnBnClickedButtonFind();
public:
	CDateTimeCtrl startTime;
public:
	CDateTimeCtrl endTime;
public:
	CEdit mEditCarNo;
public:
	afx_msg void OnNMDblclkTrainList(NMHDR *pNMHDR, LRESULT *pResult);
};
