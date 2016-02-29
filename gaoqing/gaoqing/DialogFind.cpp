// DialogFind.cpp : implementation file
//

#include "stdafx.h"
#include "gaoqing.h"
#include "DialogFind.h"
#include "gaoqingDlg.h"
#include "afx.h"
#include <shlwapi.h>

#pragma comment(lib,"Shlwapi.lib") //如果没有这行，会出现link错误





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
	//要执行的SQL语句          
	    
	//存放查询结果   
	hRoot = tran_list.InsertItem(_T("录像文件"), 0, 0);
	TRY        
	{       
		rs_train.Open(AFX_DB_USE_DEFAULT_TYPE,train_sql);  
		//打开查询记录        
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
		         
		//得到数据                     
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
	// 插入根节点   
	
     
	TRY   
	{   m_db.OpenEx(_T("DSN=gaoqing;UID=sa;PWD=gaoqing;"),CDatabase::noOdbcDialog);//连接到一个名为Test的数据源   
		   
		//m_db.OpenEx(_T("DSN=gaoqing;"),CDatabase::noOdbcDialog);//连接到一个名为Test的数据源   
		rs_train.m_pDatabase = &m_db;   
		rs_details.m_pDatabase = &m_db;  
	}  
	//////////////////////////处理异常消息   
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

    // 在根节点下插入子节点   
    hCataItem =tran_list.InsertItem(_T("IT互联网"), 1, 1, hRoot, TVI_LAST);   
    // 为“IT互联网”节点添加附加的编号数据，在鼠标划过该节点时显示   
    tran_list.SetItemData(hCataItem, 1);   
    // 在“IT互联网”节点下插入子节点   
    hArtItem = tran_list.InsertItem(_T("百度文章1"), 2, 2, hCataItem, TVI_LAST);   
    // 为“百度文章1”节点添加附加的编号数据，在鼠标划过该节点时显示   
    tran_list.SetItemData(hArtItem, 2);   
    // 在“IT互联网”节点下插入另一子节点   
    hArtItem = tran_list.InsertItem(_T("谷歌文章2"), 2, 2, hCataItem, TVI_LAST);   
    // 为“谷歌文章2”节点添加附加的编号数据，在鼠标划过该节点时显示   
    tran_list.SetItemData(hArtItem, 3);   
    // 在根节点下插入第二个子节点   
    hCataItem = tran_list.InsertItem(_T("数码生活"), 1, 1, hRoot, TVI_LAST);   
    // 为“数码生活”节点添加附加的编号数据，在鼠标划过该节点时显示   
    tran_list.SetItemData(hCataItem, 4);   
    // 在“数码生活”节点下插入子节点   
    hArtItem = tran_list.InsertItem(_T("智能手机文章1"), 2, 2, hCataItem, TVI_LAST);   
    // 为“智能手机文章1”节点添加附加的编号数据，在鼠标划过该节点时显示   
    tran_list.SetItemData(hArtItem, 5);   
    // 在“数码生活”节点下插入另一子节点   
    hArtItem = tran_list.InsertItem(_T("平板电脑文章2"), 2, 2, hCataItem, TVI_LAST);   
    // 为“平板电脑文章2”节点添加附加的编号数据，在鼠标划过该节点时显示   
    tran_list.SetItemData(hArtItem, 6);   
    // 在根节点下插入第三个子节点   
    hCataItem = tran_list.InsertItem(_T("软件开发"), 1, 1, hRoot, TVI_LAST);   
    // 为“软件开发”节点添加附加的编号数据，在鼠标划过该节点时显示   
    tran_list.SetItemData(hCataItem, 7);   
    // 在“软件开发”节点下插入子节点   
    hArtItem = tran_list.InsertItem(_T("C++编程入门系列1"), 2, 2, hCataItem, TVI_LAST);   
    // 为“C++编程入门系列1”节点添加附加的编号数据，在鼠标划过该节点时显示   
    tran_list.SetItemData(hArtItem, 8);   
    // 在“软件开发”节点下插入另一子节点   
    hArtItem = tran_list.InsertItem(_T("VS2010/MFC编程入门2"), 2, 2, hCataItem, TVI_LAST);   
    // 为“VS2010/MFC编程入门2”节点添加附加的编号数据，在鼠标划过该节点时显示   
    tran_list.SetItemData(hArtItem, 9);   
    // 在根节点下插入第四个子节点   
    hCataItem = tran_list.InsertItem(_T("娱乐休闲"), 1, 1, hRoot, TVI_LAST);   
    // 为“娱乐休闲”节点添加附加的编号数据，在鼠标划过该节点时显示   
    tran_list.SetItemData(hCataItem, 10);   
    // 在“娱乐休闲”节点下插入子节点   
    hArtItem = tran_list.InsertItem(_T("玛雅文明文章1"), 2, 2, hCataItem, TVI_LAST);   
    // 为“玛雅文明文章1”节点添加附加的编号数据，在鼠标划过该节点时显示   
    tran_list.SetItemData(hArtItem, 11);   
    // 在“娱乐休闲”节点下插入另一子节点   
    hArtItem = tran_list.InsertItem(_T("IT笑话2"), 2, 2, hCataItem, TVI_LAST);   
    // 为“IT笑话2”节点添加附加的编号数据，在鼠标划过该节点时显示   
    tran_list.SetItemData(hArtItem, 12);   

*/


	//dlg = (CgaoqingDlg *)GetParent()->GetParent();

	//CDialog *gg = (CDialog *)dlg;
	






/*

	//CString m_cstrFileList="";
    CFileFind tempFindl1;
    BOOL bFoundl1; //判断是否成功找到文件
	bFoundl1=tempFindl1.FindFile("C:\\test\\*.*");   //修改" "内内容给限定查找文件类型
    //CString strTmp;   //如果找到的是文件夹存放文件夹路径
    while(bFoundl1)      //遍历所有文件
    {
        bFoundl1=tempFindl1.FindNextFile(); //第一次执行FindNextFile是选择到第一个文件，以后执行为选择
                                                              //到下一个文件
           if(tempFindl1.IsDots()) continue; //如果找到的是返回上层的目录 则结束本次查找
          
            if(tempFindl1.IsDirectory())   //找到的是文件夹，则遍历该文件夹下的文件
            {
                //strTmp="";
                //strTmp=tempFind.GetFilePath();
				CString foldername = tempFindl1.GetFileName();
				hCataItem =tran_list.InsertItem(_T(foldername), 1, 1, hRoot, TVI_LAST);

				CString folderpath = tempFindl1.GetFilePath();


				CFileFind tempFindl2;
				BOOL bFoundl2; //判断是否成功找到文件
				bFoundl2=tempFindl2.FindFile(folderpath+"\\*.*");   //修改" "内内容给限定查找文件类型
				
				CString *patha=NULL;
				CString *pathb=NULL;
				CString *pathc=NULL;
				CString *pathd=NULL;
				CString *index = NULL;
    //CString strTmp;   //如果找到的是文件夹存放文件夹路径
				while(bFoundl2)      //遍历所有文件
				{
					bFoundl2=tempFindl2.FindNextFile(); //第一次执行FindNextFile是选择到第一个文件，以后执行为选择
                                                              //到下一个文件
					if(tempFindl2.IsDots()) continue; //如果找到的是返回上层的目录 则结束本次查找
          
					if(tempFindl2.IsDirectory())   //找到的是文件夹，则遍历该文件夹下的文件
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
                CString foldername=tempFindl1.GetFileName(); //保存文件名，包括后缀名
                 // 在此处添加对找到文件的处理
               
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
	
	//CString *strText; // 树节点的标签文本字符串   
       
    // 获取当前选中节点的句柄   
    HTREEITEM hItem = tran_list.GetSelectedItem();   
    // 获取选中节点的标签文本字符串   
    //strText = (CString *)tran_list.GetItemData(hItem);   
    // 将字符串显示到编辑框中   
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
		parent->GetDlgItem(IDC_BUTTON_REC_PLAY)->SetWindowText("暂停");

		SetTimer(PLAY_TIMER,500,NULL);
	}*/

}

void CDialogFind::OnTvnGetInfoTipTrainList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVGETINFOTIP pGetInfoTip = reinterpret_cast<LPNMTVGETINFOTIP>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	
NMTVGETINFOTIP* pTVTipInfo = (NMTVGETINFOTIP*)pNMHDR;   // 将传入的pNMHDR转换为NMTVGETINFOTIP指针类型   
    HTREEITEM hRoot = tran_list.GetRootItem();      // 获取树的根节点   
    CString strText;     // 每个树节点的提示信息   
  
    if (pTVTipInfo->hItem == hRoot)   
    {   
        // 如果鼠标划过的节点是根节点，则提示信息为空   
        strText = _T("");   
    }   
    else  
    {   
        // 如果鼠标划过的节点不是根节点，则将该节点的附加32位数据格式化为字符串   
        strText.Format(_T("%d"), pTVTipInfo->lParam);   
    }   
  
    // 将strText字符串拷贝到pTVTipInfo结构体变量的pszText成员中，这样就能显示内容为strText的提示信息   
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
		parent->GetDlgItem(IDC_BUTTON_REC_PLAY)->SetWindowText("播放");
		parent->GetDlgItem(IDC_BUTTON_REC_FAST)->SetWindowText("快放");
		parent->GetDlgItem(IDC_BUTTON_REC_SLOW)->SetWindowText("慢放");
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


	//CString *strText; // 树节点的标签文本字符串   
       
    // 获取当前选中节点的句柄   
    HTREEITEM hItem = tran_list.GetSelectedItem();   
    // 获取选中节点的标签文本字符串   
    //strText = (CString *)tran_list.GetItemData(hItem);   
    // 将字符串显示到编辑框中   
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
		parent->GetDlgItem(IDC_BUTTON_REC_PLAY)->SetWindowText("暂停");

		SetTimer(PLAY_TIMER,500,NULL);
	}
}
