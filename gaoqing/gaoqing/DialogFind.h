/*
File name:   DialogFind.h
File Author: Li Weichen
Date:        2016.3.1
*/
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

	//===== member variables ====
	CTreeCtrl tran_list;
	CgaoqingDlg *parent;
	CDateTimeCtrl startTime;
	CDateTimeCtrl endTime;
	CEdit mEditCarNo;
	//===== db variables ====
	CDatabase m_db; 
	CRecordset rs_train,rs_details;

	//===== tree list variables =====
	HTREEITEM hRoot;     // ���ĸ��ڵ�ľ��   
    HTREEITEM hCataItem; // �ɱ�ʾ��һ���ڽڵ�ľ��   
    HTREEITEM hArtItem;  // �ɱ�ʾ��һ����ڵ�ľ�� 

	//===== message fuction =====
	void OnOK();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonFind();
	afx_msg void OnNMDblclkTrainList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//===== member function ====
	void findTrains(CString sql,CString carno);

	


private:
	//===== member variables ====
	DWORD len;//play length
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()


};
