/*
File name:   DialogFind.cpp
File Author: Li Weichen
Date:        2016.3.1
*/
#include "stdafx.h"
#include "gaoqing.h"
#include "DialogFind.h"
#include "gaoqingDlg.h"
#include "afx.h"
#include <shlwapi.h>

#pragma comment(lib,"Shlwapi.lib") 

// CDialogFind dialog
#define PLAY_TIMER				7		//local playback status timer
IMPLEMENT_DYNAMIC(CDialogFind, CDialog)

CDialogFind::CDialogFind(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogFind::IDD, pParent)
	,len(0)
{

}
/*************************************************
Function:  OnOK
Desc:	   避免回车BUG
Input:     void
Return:	   void
**************************************************/
void CDialogFind::OnOK(){

	return;
}
/*************************************************
Function:  findTrains
Desc:	   从数据库中寻找车辆
Input:     CString train_sql 车号查询sql
		   CString carno 车号
Return:	   void
**************************************************/
void CDialogFind::findTrains(CString train_sql,CString carno)
{
	hRoot = tran_list.InsertItem(_T("录像文件"), 0, 0);
	TRY        
	{       
		rs_train.Open(AFX_DB_USE_DEFAULT_TYPE,train_sql);  
		while(rs_train.IsEOF()==0){

			CString name,id;  
			rs_train.GetFieldValue("tran_id",id);
			rs_train.GetFieldValue(_T("tran_name"),name);
			CString details_sql = "";
			if(carno.IsEmpty())
				details_sql.Format(_T("SELECT * FROM tbl_train_details WHERE tran_id=%s"),id);  
			else
				details_sql.Format(_T("SELECT * FROM tbl_train_details WHERE tran_id=%s AND car_number LIKE '%%%s%%'"),id,carno);  
			
			rs_details.Open(AFX_DB_USE_DEFAULT_TYPE,details_sql);  

			CString filename = "";
			if(rs_details.IsEOF()==0){
				CString path = parent->getConfig("CONFIG","PATH",".\\");
				filename.Format(_T("%s%s\\"),path,name);
				hCataItem = tran_list.InsertItem(_T(name), 1, 1, hRoot, TVI_LAST);
			}
			CAR_PLAY *item = new CAR_PLAY;
			hArtItem = tran_list.InsertItem(_T("起始"), 2, 2, hCataItem, TVI_LAST); 
			if (PathFileExists(_T(filename+name+"lt.mp4"))){
					item->patha = new CString(_T(filename+name+"lt.mp4"));
			}else{
					item->patha = NULL;
			}
			if (PathFileExists(_T(filename+name+"lb.mp4"))){
					item->pathb = new CString(_T(filename+name+"lb.mp4"));
			}else{
					item->pathb = NULL;
			}
			if (PathFileExists(_T(filename+name+"rt.mp4"))){
					item->pathc = new CString(_T(filename+name+"rt.mp4"));
			}else{
					item->pathc = NULL;
			}
			if (PathFileExists(_T(filename+name+"rb.mp4"))){
					item->pathd = new CString(_T(filename+name+"rb.mp4"));
			}else{
					item->pathd = NULL;
			}

			item->time =0;
			tran_list.SetItemData(hArtItem,(DWORD_PTR)item);

			while(rs_details.IsEOF()==0){
			
				CString number,time;  
				rs_details.GetFieldValue(_T("car_number"),number);
				rs_details.GetFieldValue(_T("car_time"),time);

				item = new CAR_PLAY;
				//CAR_NO car = *it;
				hArtItem = tran_list.InsertItem(_T(number), 2, 2, hCataItem, TVI_LAST); 
				
				if (PathFileExists(_T(filename+name+"lt.mp4"))){
					item->patha = new CString(_T(filename+name+"lt.mp4"));
				}else{
					item->patha = NULL;
				}
				if (PathFileExists(_T(filename+name+"lb.mp4"))){
					item->pathb = new CString(_T(filename+name+"lb.mp4"));
				}else{
					item->pathb = NULL;
				}
				if (PathFileExists(_T(filename+name+"rt.mp4"))){
					item->pathc = new CString(_T(filename+name+"rt.mp4"));
				}else{
					item->pathc = NULL;
				}
				if (PathFileExists(_T(filename+name+"rb.mp4"))){
					item->pathd = new CString(_T(filename+name+"rb.mp4"));
				}else{
					item->pathd = NULL;
				}

				item->time = _ttoi(time);
				tran_list.SetItemData(hArtItem,(DWORD_PTR)item);
				rs_details.MoveNext();
			}
			rs_details.Close();

			rs_train.MoveNext();

		}
		rs_train.Close();
		                       
	}       
	CATCH(CDBException,ex)        
	{            
		AfxMessageBox(ex->m_strError);   
		AfxMessageBox(ex->m_strStateNativeOrigin);        
	}           
	AND_CATCH(CMemoryException,pEx)   
	{            
		pEx->ReportError();            
		AfxMessageBox(_T("memory exception"));     
	}  
	END_CATCH 
}
/*************************************************
Function:  OnInitDialog
Desc:	   初始化对话框
Input:     void
Return:	   true
**************************************************/
BOOL CDialogFind::OnInitDialog()
{
	CDialog::OnInitDialog();
	
     
	TRY   
	{   m_db.OpenEx(_T("DSN=gaoqing;UID=sa;PWD=gaoqing;"),CDatabase::noOdbcDialog); 
		rs_train.m_pDatabase = &m_db;   
		rs_details.m_pDatabase = &m_db;  
	}  
	CATCH(CDBException,ex)  
	{      
		AfxMessageBox(ex->m_strError);  
		AfxMessageBox(ex->m_strStateNativeOrigin);   
	}   
	AND_CATCH(CMemoryException,pEx)   
	{    pEx->ReportError();   
		AfxMessageBox(_T("memory exception"));   
	} AND_CATCH(CException,ex)   
	{      
		TCHAR szError[100];      
		ex->GetErrorMessage(szError,100);      
		AfxMessageBox(szError);   
	}   
	END_CATCH 
	
	findTrains(_T("SELECT * FROM tbl_train"),"");

	return TRUE;

	
}
CDialogFind::~CDialogFind()
{
}

void CDialogFind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRAIN_LIST, tran_list);

	DDX_Control(pDX, IDC_DATETIME_START, startTime);
	DDX_Control(pDX, IDC_DATETIME_END, endTime);
	DDX_Control(pDX, IDC_EDIT1, mEditCarNo);
}


BEGIN_MESSAGE_MAP(CDialogFind, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_FIND, &CDialogFind::OnBnClickedButtonFind)
	ON_NOTIFY(NM_DBLCLK, IDC_TRAIN_LIST, &CDialogFind::OnNMDblclkTrainList)
	ON_WM_SIZE()
END_MESSAGE_MAP()

/*************************************************
Function:  ShowPlayBackState
Desc:	   根据播放时间设置按钮的状态
Input:     void
Return:	   void
**************************************************/
void CDialogFind::ShowPlayBackState()
{
	if (len == 0)
	{
		return;
	}
	if(parent->adjust)
		return;

	DWORD nCurTime=parent->getPlayedTime(true);
	parent->playslider.SetPos(nCurTime);
	if (nCurTime > len-100)
	{
		parent->stopVideo(true);
		CStatic *wd = (CStatic *)parent->GetDlgItem(IDC_STATIC_PLAY_A);
		wd->SetBitmap(NULL);
		wd = (CStatic *)parent->GetDlgItem(IDC_STATIC_PLAY_B);
		wd->SetBitmap(NULL);
		wd = (CStatic *)parent->GetDlgItem(IDC_STATIC_PLAY_C);
		wd->SetBitmap(NULL);
		wd = (CStatic *)parent->GetDlgItem(IDC_STATIC_PLAY_D);
		wd->SetBitmap(NULL);
		parent->playslider.SetPos(0);
		parent->Invalidate(TRUE);
		parent->playSpeed=0;
		parent->GetDlgItem(IDC_BUTTON_REC_PLAY)->SetWindowText("播放");
		parent->GetDlgItem(IDC_BUTTON_REC_FAST)->SetWindowText("快放");
		parent->GetDlgItem(IDC_BUTTON_REC_SLOW)->SetWindowText("慢放");
		parent->GetDlgItem(IDC_SLIDER_PLAY)->EnableWindow(false);
		parent->GetDlgItem(IDC_BUTTON_REC_PLAY)->EnableWindow(false);
		parent->GetDlgItem(IDC_BUTTON_REC_FAST)->EnableWindow(false);
		parent->GetDlgItem(IDC_BUTTON_REC_SLOW)->EnableWindow(false);
		KillTimer(PLAY_TIMER);

	}
}
/*************************************************
Function:  OnTimer
Desc:	   定时器函数
Input:     UINT_PTR nIDEvent 事件
Return:	   void
**************************************************/
void CDialogFind::OnTimer(UINT_PTR nIDEvent)
{

	if (nIDEvent==PLAY_TIMER)
	{
		ShowPlayBackState();
	}
	CDialog::OnTimer(nIDEvent);
}
/*************************************************
Function:  OnBnClickedButtonFind
Desc:	   查询按钮函数
Input:     void
Return:	   void
**************************************************/
void CDialogFind::OnBnClickedButtonFind()
{
	tran_list.DeleteAllItems();


	CTime timeTime;
    startTime.GetTime(timeTime);

	CString s = timeTime.Format( "%Y%m%d000000" );
	endTime.GetTime(timeTime);
	CString e = timeTime.Format( "%Y%m%d000000" );


	CString sql;
	sql.Format(_T("SELECT * FROM tbl_train WHERE CONVERT(bigint,tran_name) > %s AND CONVERT(bigint,tran_name) < %s"),s,e);

	CString no;
	mEditCarNo.GetWindowText(no);

	findTrains(sql,no);

}
/*************************************************
Function:  OnNMDblclkTrainList
Desc:	   双击list item
Input:     NMHDR *pNMHDR 
           LRESULT *pResult
Return:	   void
**************************************************/
void CDialogFind::OnNMDblclkTrainList(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;


       
    HTREEITEM hItem = tran_list.GetSelectedItem();   
	DWORD_PTR pt = tran_list.GetItemData(hItem);
	if(pt != 0){
		CAR_PLAY *item = (CAR_PLAY *)tran_list.GetItemData(hItem); 
		parent->playVideo(item->patha==NULL?"":(*(item->patha)),item->pathb==NULL?"":(*(item->pathb)),item->pathc==NULL?"":(*(item->pathc)),item->pathd==NULL?"":(*(item->pathd)));


		len =	parent->getVideoTime(true)*1000;

		parent->playslider.SetRange(0,len);
		parent->playslider.SetPos(item->time);

		parent->playVideoPos(item->time);

		parent->s_Play=TRUE;
		parent->GetDlgItem(IDC_SLIDER_PLAY)->EnableWindow(true);
		parent->GetDlgItem(IDC_BUTTON_REC_PLAY)->EnableWindow(true);
		parent->GetDlgItem(IDC_BUTTON_REC_FAST)->EnableWindow(true);
		parent->GetDlgItem(IDC_BUTTON_REC_SLOW)->EnableWindow(true);
		parent->GetDlgItem(IDC_BUTTON_REC_PLAY)->SetWindowText("暂停");

		SetTimer(PLAY_TIMER,500,NULL);
	}
}
/*************************************************
Function:  OnSize
Desc:	   对话框大小改变函数
Input:     UINT nType 
		   int cx 宽度
		   int cy 高度
Return:	   void
**************************************************/
void CDialogFind::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	int gap = 10;
	int wtitle = (cx-gap-gap-gap-gap)/4;
	int htitle = 25;

	int wsel = (cx-gap-gap-gap-gap)*2/4;
	int hsel = htitle;

	int wbtn = (cx-gap-gap-gap-gap)/4;
	int hbtn = htitle+gap+htitle+gap+htitle;

	int wtre = cx-gap-gap;
	int htre = cy-gap-gap-gap-gap-gap-htitle*3;

	CWnd *pWnd; 
	CRect new_rect;
	pWnd = GetDlgItem(IDC_STATIC1);
	if(pWnd != NULL){
		new_rect.left=gap;//调整控件大小  
		new_rect.right=new_rect.left+wtitle;  
		new_rect.top=gap;  
		new_rect.bottom=new_rect.top+htitle;  
		pWnd->MoveWindow(new_rect);//设置控件大小  
	}

	pWnd = GetDlgItem(IDC_EDIT1);
	if(pWnd != NULL){
		new_rect.left=new_rect.right+gap;//调整控件大小  
		new_rect.right=new_rect.left+wsel;  
		//new_rect.top=gap;  
		//new_rect.bottom=new_rect.top+htitle;  
		pWnd->MoveWindow(new_rect);//设置控件大小  
	}

	pWnd = GetDlgItem(IDC_STATIC2);
	if(pWnd != NULL){
		new_rect.left=gap;//调整控件大小  
		new_rect.right=new_rect.left+wtitle;  
		new_rect.top=new_rect.bottom+gap;  
		new_rect.bottom=new_rect.top+htitle;  
		pWnd->MoveWindow(new_rect);//设置控件大小  
	}

	pWnd = GetDlgItem(IDC_DATETIME_START);
	if(pWnd != NULL){
		new_rect.left=new_rect.right+gap;//调整控件大小  
		new_rect.right=new_rect.left+wsel;  
		//new_rect.top=gap;  
		//new_rect.bottom=new_rect.top+htitle;  
		pWnd->MoveWindow(new_rect);//设置控件大小  
	}

	pWnd = GetDlgItem(IDC_STATIC3);
	if(pWnd != NULL){
		new_rect.left=gap;//调整控件大小  
		new_rect.right=new_rect.left+wtitle;  
		new_rect.top=new_rect.bottom+gap;  
		new_rect.bottom=new_rect.top+htitle;  
		pWnd->MoveWindow(new_rect);//设置控件大小  
	}

	pWnd = GetDlgItem(IDC_DATETIME_END);
	if(pWnd != NULL){
		new_rect.left=new_rect.right+gap;//调整控件大小  
		new_rect.right=new_rect.left+wsel;  
		//new_rect.top=gap;  
		//new_rect.bottom=new_rect.top+htitle;  
		pWnd->MoveWindow(new_rect);//设置控件大小  
	}

	pWnd = GetDlgItem(IDC_BUTTON_FIND);
	if(pWnd != NULL){
		new_rect.left=new_rect.right+gap;//调整控件大小  
		new_rect.right=new_rect.left+wbtn;  
		new_rect.top=gap;  
		new_rect.bottom=new_rect.top+hbtn;  
		pWnd->MoveWindow(new_rect);//设置控件大小  
	}
	
	pWnd = GetDlgItem(IDC_TRAIN_LIST);
	if(pWnd != NULL){
		new_rect.left=gap;//调整控件大小  
		new_rect.right=new_rect.left+wtre;  
		new_rect.top=gap*4 + htitle*3;  
		new_rect.bottom=new_rect.top+htre;  
		pWnd->MoveWindow(new_rect);//设置控件大小  
	}
}