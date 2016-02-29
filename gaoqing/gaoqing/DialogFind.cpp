// DialogFind.cpp : implementation file
//

#include "stdafx.h"
#include "gaoqing.h"
#include "DialogFind.h"
#include "gaoqingDlg.h"
#include "afx.h"
#include <shlwapi.h>

#pragma comment(lib,"Shlwapi.lib") //���û�����У������link����





// CDialogFind dialog
#define PLAY_TIMER				7		//local playback status timer
IMPLEMENT_DYNAMIC(CDialogFind, CDialog)

CDialogFind::CDialogFind(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogFind::IDD, pParent)
	,len(0)
{

}
vector<CAR_NO> CDialogFind::findCars(CString uri)
{
	CStdioFile file;
	file.Open(uri,CFile::modeRead);
	CString strline;
	int index,speed,time;
	CString no,letter;
	file.ReadString(strline);
	vector<CAR_NO> cars;
	while(file.ReadString(strline)){
		vector<CString> vecString;
		int iPos = 0;
		CString strTmp;
		strline.ReleaseBuffer();
		strTmp = strline.Tokenize("\t",iPos);
		while(strTmp.Trim() != _T(""))
		{
			vecString.push_back(strTmp);
			strTmp = strline.Tokenize("\t",iPos);
		}


		//sscanf(strline,"%d\t%d\t%s\t%d\t%s",&index,&speed,no,&time,letter);
		CAR_NO car;
		car.no = vecString[2];
		car.time = atoi(vecString[3]);
		cars.push_back(car);

	
	}
	file.Close();
	return cars;
}
void CDialogFind::OnOK(){

	return;
}
void CDialogFind::findTrains(CString train_sql,CString carno)
{
	//"SELECT * FROM tbl_train WHERE (number=1)"
	//CString train_sql = _T("SELECT * FROM tbl_train");      
	//Ҫִ�е�SQL���          
	    
	//��Ų�ѯ���   
	hRoot = tran_list.InsertItem(_T("¼���ļ�"), 0, 0);
	TRY        
	{       
		rs_train.Open(AFX_DB_USE_DEFAULT_TYPE,train_sql);  
		//�򿪲�ѯ��¼        
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
			hArtItem = tran_list.InsertItem(_T("��ʼ"), 2, 2, hCataItem, TVI_LAST); 
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
		         
		//�õ�����                     
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
BOOL CDialogFind::OnInitDialog()
{
	CDialog::OnInitDialog();

	//CTime timeTime(1998, 4, 3, 0, 0, 0);
   //VERIFY(pCtrl->SetTime(&timeTime));

    //CTime time = CTime::GetCurrentTime();
	//startTime.SetTime(&time);
	//endTime.SetTime(&time);
	// ������ڵ�   
	
     
	TRY   
	{   m_db.OpenEx(_T("DSN=gaoqing;UID=sa;PWD=gaoqing;"),CDatabase::noOdbcDialog);//���ӵ�һ����ΪTest������Դ   
		   
		//m_db.OpenEx(_T("DSN=gaoqing;"),CDatabase::noOdbcDialog);//���ӵ�һ����ΪTest������Դ   
		rs_train.m_pDatabase = &m_db;   
		rs_details.m_pDatabase = &m_db;  
	}  
	//////////////////////////�����쳣��Ϣ   
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


	 
	

	//CString *tt = new CString("");
	//tran_list.SetItemData(hRoot,(DWORD_PTR)tt);
/*

    // �ڸ��ڵ��²����ӽڵ�   
    hCataItem =tran_list.InsertItem(_T("IT������"), 1, 1, hRoot, TVI_LAST);   
    // Ϊ��IT���������ڵ���Ӹ��ӵı�����ݣ�����껮���ýڵ�ʱ��ʾ   
    tran_list.SetItemData(hCataItem, 1);   
    // �ڡ�IT���������ڵ��²����ӽڵ�   
    hArtItem = tran_list.InsertItem(_T("�ٶ�����1"), 2, 2, hCataItem, TVI_LAST);   
    // Ϊ���ٶ�����1���ڵ���Ӹ��ӵı�����ݣ�����껮���ýڵ�ʱ��ʾ   
    tran_list.SetItemData(hArtItem, 2);   
    // �ڡ�IT���������ڵ��²�����һ�ӽڵ�   
    hArtItem = tran_list.InsertItem(_T("�ȸ�����2"), 2, 2, hCataItem, TVI_LAST);   
    // Ϊ���ȸ�����2���ڵ���Ӹ��ӵı�����ݣ�����껮���ýڵ�ʱ��ʾ   
    tran_list.SetItemData(hArtItem, 3);   
    // �ڸ��ڵ��²���ڶ����ӽڵ�   
    hCataItem = tran_list.InsertItem(_T("��������"), 1, 1, hRoot, TVI_LAST);   
    // Ϊ����������ڵ���Ӹ��ӵı�����ݣ�����껮���ýڵ�ʱ��ʾ   
    tran_list.SetItemData(hCataItem, 4);   
    // �ڡ���������ڵ��²����ӽڵ�   
    hArtItem = tran_list.InsertItem(_T("�����ֻ�����1"), 2, 2, hCataItem, TVI_LAST);   
    // Ϊ�������ֻ�����1���ڵ���Ӹ��ӵı�����ݣ�����껮���ýڵ�ʱ��ʾ   
    tran_list.SetItemData(hArtItem, 5);   
    // �ڡ���������ڵ��²�����һ�ӽڵ�   
    hArtItem = tran_list.InsertItem(_T("ƽ���������2"), 2, 2, hCataItem, TVI_LAST);   
    // Ϊ��ƽ���������2���ڵ���Ӹ��ӵı�����ݣ�����껮���ýڵ�ʱ��ʾ   
    tran_list.SetItemData(hArtItem, 6);   
    // �ڸ��ڵ��²���������ӽڵ�   
    hCataItem = tran_list.InsertItem(_T("�������"), 1, 1, hRoot, TVI_LAST);   
    // Ϊ������������ڵ���Ӹ��ӵı�����ݣ�����껮���ýڵ�ʱ��ʾ   
    tran_list.SetItemData(hCataItem, 7);   
    // �ڡ�����������ڵ��²����ӽڵ�   
    hArtItem = tran_list.InsertItem(_T("C++�������ϵ��1"), 2, 2, hCataItem, TVI_LAST);   
    // Ϊ��C++�������ϵ��1���ڵ���Ӹ��ӵı�����ݣ�����껮���ýڵ�ʱ��ʾ   
    tran_list.SetItemData(hArtItem, 8);   
    // �ڡ�����������ڵ��²�����һ�ӽڵ�   
    hArtItem = tran_list.InsertItem(_T("VS2010/MFC�������2"), 2, 2, hCataItem, TVI_LAST);   
    // Ϊ��VS2010/MFC�������2���ڵ���Ӹ��ӵı�����ݣ�����껮���ýڵ�ʱ��ʾ   
    tran_list.SetItemData(hArtItem, 9);   
    // �ڸ��ڵ��²�����ĸ��ӽڵ�   
    hCataItem = tran_list.InsertItem(_T("��������"), 1, 1, hRoot, TVI_LAST);   
    // Ϊ���������С��ڵ���Ӹ��ӵı�����ݣ�����껮���ýڵ�ʱ��ʾ   
    tran_list.SetItemData(hCataItem, 10);   
    // �ڡ��������С��ڵ��²����ӽڵ�   
    hArtItem = tran_list.InsertItem(_T("������������1"), 2, 2, hCataItem, TVI_LAST);   
    // Ϊ��������������1���ڵ���Ӹ��ӵı�����ݣ�����껮���ýڵ�ʱ��ʾ   
    tran_list.SetItemData(hArtItem, 11);   
    // �ڡ��������С��ڵ��²�����һ�ӽڵ�   
    hArtItem = tran_list.InsertItem(_T("ITЦ��2"), 2, 2, hCataItem, TVI_LAST);   
    // Ϊ��ITЦ��2���ڵ���Ӹ��ӵı�����ݣ�����껮���ýڵ�ʱ��ʾ   
    tran_list.SetItemData(hArtItem, 12);   

*/


	//dlg = (CgaoqingDlg *)GetParent()->GetParent();

	//CDialog *gg = (CDialog *)dlg;
	






/*

	//CString m_cstrFileList="";
    CFileFind tempFindl1;
    BOOL bFoundl1; //�ж��Ƿ�ɹ��ҵ��ļ�
	bFoundl1=tempFindl1.FindFile("C:\\test\\*.*");   //�޸�" "�����ݸ��޶������ļ�����
    //CString strTmp;   //����ҵ������ļ��д���ļ���·��
    while(bFoundl1)      //���������ļ�
    {
        bFoundl1=tempFindl1.FindNextFile(); //��һ��ִ��FindNextFile��ѡ�񵽵�һ���ļ����Ժ�ִ��Ϊѡ��
                                                              //����һ���ļ�
           if(tempFindl1.IsDots()) continue; //����ҵ����Ƿ����ϲ��Ŀ¼ ��������β���
          
            if(tempFindl1.IsDirectory())   //�ҵ������ļ��У���������ļ����µ��ļ�
            {
                //strTmp="";
                //strTmp=tempFind.GetFilePath();
				CString foldername = tempFindl1.GetFileName();
				hCataItem =tran_list.InsertItem(_T(foldername), 1, 1, hRoot, TVI_LAST);

				CString folderpath = tempFindl1.GetFilePath();


				CFileFind tempFindl2;
				BOOL bFoundl2; //�ж��Ƿ�ɹ��ҵ��ļ�
				bFoundl2=tempFindl2.FindFile(folderpath+"\\*.*");   //�޸�" "�����ݸ��޶������ļ�����
				
				CString *patha=NULL;
				CString *pathb=NULL;
				CString *pathc=NULL;
				CString *pathd=NULL;
				CString *index = NULL;
    //CString strTmp;   //����ҵ������ļ��д���ļ���·��
				while(bFoundl2)      //���������ļ�
				{
					bFoundl2=tempFindl2.FindNextFile(); //��һ��ִ��FindNextFile��ѡ�񵽵�һ���ļ����Ժ�ִ��Ϊѡ��
                                                              //����һ���ļ�
					if(tempFindl2.IsDots()) continue; //����ҵ����Ƿ����ϲ��Ŀ¼ ��������β���
          
					if(tempFindl2.IsDirectory())   //�ҵ������ļ��У���������ļ����µ��ļ�
					{
					}
					else
					{
						
						CString filename = tempFindl2.GetFileName();
						if(filename.Right(6)=="lt.mp4"){
							patha = new CString(folderpath+"\\"+filename);
     					}else if(filename.Right(6)=="lb.mp4"){
							pathb = new CString(folderpath+"\\"+filename);
     					}else if(filename.Right(6)=="rt.mp4"){
							pathc = new CString(folderpath+"\\"+filename);
     					}else if(filename.Right(6)=="rb.mp4"){
							pathd = new CString(folderpath+"\\"+filename);
     					}else if(filename.Right(4)==".txt"){
							index = new CString(folderpath+"\\"+filename);	
						}
               
					}
				}
				tempFindl2.Close();

				if(index != NULL && (patha != NULL || pathb != NULL || pathc != NULL || pathd != NULL)){
					vector<CAR_NO> cars = findCars(*index);
					for(vector<CAR_NO>::iterator it = cars.begin();it!=cars.end();it++){

						CAR_PLAY *item = new CAR_PLAY;
						CAR_NO car = *it;
						hArtItem = tran_list.InsertItem(_T(car.no), 2, 2, hCataItem, TVI_LAST); 
						item->patha = patha;
						item->pathb = pathb;
						item->pathc = pathc;
						item->pathd = pathd;
						item->time = car.time;
						tran_list.SetItemData(hArtItem,(DWORD_PTR)item);
					}
				}





                //FindFile(strTmp);
            }
            else
            {
                CString foldername=tempFindl1.GetFileName(); //�����ļ�����������׺��
                 // �ڴ˴���Ӷ��ҵ��ļ��Ĵ���
               
            }
    }
    tempFindl1.Close();


*/





























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
	ON_NOTIFY(TVN_SELCHANGED, IDC_TRAIN_LIST, &CDialogFind::OnTvnSelchangedTrainList)
	ON_NOTIFY(TVN_GETINFOTIP, IDC_TRAIN_LIST, &CDialogFind::OnTvnGetInfoTipTrainList)

	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_FIND, &CDialogFind::OnBnClickedButtonFind)
	ON_NOTIFY(NM_DBLCLK, IDC_TRAIN_LIST, &CDialogFind::OnNMDblclkTrainList)
END_MESSAGE_MAP()


// CDialogFind message handlers
#include "afxcmn.h"
void CDialogFind::OnTvnSelchangedTrainList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

/*
	
	//CString *strText; // ���ڵ�ı�ǩ�ı��ַ���   
       
    // ��ȡ��ǰѡ�нڵ�ľ��   
    HTREEITEM hItem = tran_list.GetSelectedItem();   
    // ��ȡѡ�нڵ�ı�ǩ�ı��ַ���   
    //strText = (CString *)tran_list.GetItemData(hItem);   
    // ���ַ�����ʾ���༭����   
    //SetDlgItemText(IDC_ITEM_SEL_EDIT, strText); 

	DWORD_PTR pt = tran_list.GetItemData(hItem);
	if(pt != 0){
		CAR_PLAY *item = (CAR_PLAY *)tran_list.GetItemData(hItem); 
	
	//CgaoqingDlg *dlg = (CgaoqingDlg *)GetParent()->GetParent();

		//CString *st = item->path;
	//CDialog *gg = (CDialog *)dlg;
		parent->playVideo(item->patha==NULL?"":(*(item->patha)),item->pathb==NULL?"":(*(item->pathb)),item->pathc==NULL?"":(*(item->pathc)),item->pathd==NULL?"":(*(item->pathd)));


		len =	parent->getVideoTime()*1000;

		parent->playslider.SetRange(0,len);
		parent->playslider.SetPos(item->time);

		parent->playVideoPos(item->time);

		parent->s_Play=TRUE;
		parent->GetDlgItem(IDC_SLIDER_PLAY)->EnableWindow(true);
		parent->GetDlgItem(IDC_BUTTON_REC_PLAY)->EnableWindow(true);
		parent->GetDlgItem(IDC_BUTTON_REC_FAST)->EnableWindow(true);
		parent->GetDlgItem(IDC_BUTTON_REC_SLOW)->EnableWindow(true);
		parent->GetDlgItem(IDC_BUTTON_REC_PLAY)->SetWindowText("��ͣ");

		SetTimer(PLAY_TIMER,500,NULL);
	}*/

}

void CDialogFind::OnTvnGetInfoTipTrainList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVGETINFOTIP pGetInfoTip = reinterpret_cast<LPNMTVGETINFOTIP>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	
NMTVGETINFOTIP* pTVTipInfo = (NMTVGETINFOTIP*)pNMHDR;   // �������pNMHDRת��ΪNMTVGETINFOTIPָ������   
    HTREEITEM hRoot = tran_list.GetRootItem();      // ��ȡ���ĸ��ڵ�   
    CString strText;     // ÿ�����ڵ����ʾ��Ϣ   
  
    if (pTVTipInfo->hItem == hRoot)   
    {   
        // �����껮���Ľڵ��Ǹ��ڵ㣬����ʾ��ϢΪ��   
        strText = _T("");   
    }   
    else  
    {   
        // �����껮���Ľڵ㲻�Ǹ��ڵ㣬�򽫸ýڵ�ĸ���32λ���ݸ�ʽ��Ϊ�ַ���   
        strText.Format(_T("%d"), pTVTipInfo->lParam);   
    }   
  
    // ��strText�ַ���������pTVTipInfo�ṹ�������pszText��Ա�У�����������ʾ����ΪstrText����ʾ��Ϣ   
    //wcscpy(pTVTipInfo->pszText, strText); 


}







void CDialogFind::ShowPlayBackState()
{
	//DWORD totaltime = dlg->getVideoTime();
	if (len == 0)
	{
		return;
	}

	DWORD nCurTime=parent->getPlayedTime();
	parent->playslider.SetPos(nCurTime);
	/*CString csStatus;
	csStatus.Format("%02d:%02d:%02d/%02d:%02d:%02d",\
		nCurSecond/3600,(nCurSecond%3600)/60,nCurSecond%60,\
		m_iTotalSeconds/3600,(m_iTotalSeconds%3600)/60,m_iTotalSeconds%60);
	float fCurPos = PlayM4_GetPlayPos(USED_PORT);
	g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "PlayM4_GetPlayPos %f", fCurPos);
	*/
	if (nCurTime > len-100)
	{
		parent->stopVideo();
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
		parent->GetDlgItem(IDC_BUTTON_REC_PLAY)->SetWindowText("����");
		parent->GetDlgItem(IDC_BUTTON_REC_FAST)->SetWindowText("���");
		parent->GetDlgItem(IDC_BUTTON_REC_SLOW)->SetWindowText("����");
		parent->GetDlgItem(IDC_SLIDER_PLAY)->EnableWindow(false);
		parent->GetDlgItem(IDC_BUTTON_REC_PLAY)->EnableWindow(false);
		parent->GetDlgItem(IDC_BUTTON_REC_FAST)->EnableWindow(false);
		parent->GetDlgItem(IDC_BUTTON_REC_SLOW)->EnableWindow(false);
		KillTimer(PLAY_TIMER);

	}


	//GetDlgItem(IDC_STATIC_LOCAL_PLAY_STATUS)->SetWindowText(csStatus);
}

void CDialogFind::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (nIDEvent==PLAY_TIMER)
	{
		ShowPlayBackState();
	}
	CDialog::OnTimer(nIDEvent);
}

void CDialogFind::OnBnClickedButtonFind()
{
	tran_list.DeleteAllItems();


	CTime timeTime;
    startTime.GetTime(timeTime);


	//CTime t( 1999, 3, 19, 22, 15, 0 ); 
// 10:15 PM March 19, 1999
	CString s = timeTime.Format( "%Y%m%d000000" );
	endTime.GetTime(timeTime);
	CString e = timeTime.Format( "%Y%m%d000000" );


	CString sql;
	sql.Format(_T("SELECT * FROM tbl_train WHERE CONVERT(bigint,tran_name) > %s AND CONVERT(bigint,tran_name) < %s"),s,e);

	CString no;
	mEditCarNo.GetWindowText(no);

	findTrains(sql,no);

}

void CDialogFind::OnNMDblclkTrainList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;


	//CString *strText; // ���ڵ�ı�ǩ�ı��ַ���   
       
    // ��ȡ��ǰѡ�нڵ�ľ��   
    HTREEITEM hItem = tran_list.GetSelectedItem();   
    // ��ȡѡ�нڵ�ı�ǩ�ı��ַ���   
    //strText = (CString *)tran_list.GetItemData(hItem);   
    // ���ַ�����ʾ���༭����   
    //SetDlgItemText(IDC_ITEM_SEL_EDIT, strText); 

	DWORD_PTR pt = tran_list.GetItemData(hItem);
	if(pt != 0){
		CAR_PLAY *item = (CAR_PLAY *)tran_list.GetItemData(hItem); 
	
	//CgaoqingDlg *dlg = (CgaoqingDlg *)GetParent()->GetParent();

		//CString *st = item->path;
	//CDialog *gg = (CDialog *)dlg;
		parent->playVideo(item->patha==NULL?"":(*(item->patha)),item->pathb==NULL?"":(*(item->pathb)),item->pathc==NULL?"":(*(item->pathc)),item->pathd==NULL?"":(*(item->pathd)));


		len =	parent->getVideoTime()*1000;

		parent->playslider.SetRange(0,len);
		parent->playslider.SetPos(item->time);

		parent->playVideoPos(item->time);

		parent->s_Play=TRUE;
		parent->GetDlgItem(IDC_SLIDER_PLAY)->EnableWindow(true);
		parent->GetDlgItem(IDC_BUTTON_REC_PLAY)->EnableWindow(true);
		parent->GetDlgItem(IDC_BUTTON_REC_FAST)->EnableWindow(true);
		parent->GetDlgItem(IDC_BUTTON_REC_SLOW)->EnableWindow(true);
		parent->GetDlgItem(IDC_BUTTON_REC_PLAY)->SetWindowText("��ͣ");

		SetTimer(PLAY_TIMER,500,NULL);
	}
}
