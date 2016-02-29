// gaoqingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "gaoqing.h"
#include "gaoqingDlg.h"
#include "afxmt.h"
#include "Mmsystem.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BUFSIZE MAX_PATH
#define LIVE_TIMER				8		//local playback status timer
CgaoqingDlg::CgaoqingDlg(CWnd* pParent /*=NULL*/)
: CDialog(CgaoqingDlg::IDD, pParent),direction(0),car_n(0),pFile(INVALID_HANDLE_VALUE)
	, mHik_a(0)
	, mHik_b(0)
	, mHik_c(0)
	, mHik_d(0)
	,s_Play(FALSE)
	,train_id("1")
	,maxScreen(0)
	,maxScreen_isMax(false)
	,playSpeed(0)
	,is_minimized(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


}

void CgaoqingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, tabSettings);
	DDX_Control(pDX, IDC_SLIDER_PLAY, playslider);
}

BEGIN_MESSAGE_MAP(CgaoqingDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CgaoqingDlg::OnTcnSelchangeTab)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_PLAY, &CgaoqingDlg::OnNMReleasedcaptureSliderPlay)
	ON_BN_CLICKED(IDC_BUTTON_REC_PLAY, &CgaoqingDlg::OnBnClickedButtonRecPlay)
	ON_BN_CLICKED(IDC_BUTTON_REC_FAST, &CgaoqingDlg::OnBnClickedButtonRecFast)
	ON_BN_CLICKED(IDC_BUTTON_REC_SLOW, &CgaoqingDlg::OnBnClickedButtonRecSlow)
	ON_WM_SYSCOMMAND()
	ON_WM_SIZE()
	ON_STN_DBLCLK(IDC_STATIC_PLAY_A, &CgaoqingDlg::OnStnDblclickStaticPlayA)
	
	ON_STN_DBLCLK(IDC_STATIC_PLAY_B, &CgaoqingDlg::OnStnDblclickStaticPlayB)
	ON_STN_DBLCLK(IDC_STATIC_PLAY_C, &CgaoqingDlg::OnStnDblclickStaticPlayC)
	ON_STN_DBLCLK(IDC_STATIC_PLAY_D, &CgaoqingDlg::OnStnDblclickStaticPlayD)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CgaoqingDlg message handlers



// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CgaoqingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CgaoqingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CgaoqingDlg::OnClose() 
{
	CDialog::OnClose();
}
//==========================================play record ========================
#define TIMER_INTERVAL 500//local play back timer refresh interval

void CgaoqingDlg::initView()
{
	tabSettings.InsertItem(0,_T("直播"));//“选项卡1”可更改，是sheet页的名字；
	tabSettings.InsertItem(1,_T("回放"));
	tabSettings.InsertItem(2,_T("设置")); 

	dLogin.Create(IDD_DIALOG_LOGIN,GetDlgItem(IDC_TAB));
	dFind.Create(IDD_DIALOG_FIND,GetDlgItem(IDC_TAB));
	dSetting.Create(IDD_DIALOG_SETTING,GetDlgItem(IDC_TAB));

	CRect rs;
	tabSettings.GetClientRect(&rs);
	rs.left +=10;
	rs.right -=10;
	rs.top +=30;
	rs.bottom -=30;
	dLogin.MoveWindow(&rs);
	dFind.MoveWindow(&rs);
	dSetting.MoveWindow(&rs);

	dLogin.ShowWindow(true);
	dFind.ShowWindow(false);
	dSetting.ShowWindow(false);
	tabSettings.SetCurSel(0);

}
BOOL CgaoqingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	NET_DVR_Init();
	// TODO: Add extra initialization here
	initParam();
	
	//=================================HIK Operation=======================
	if(loginCamera()){

			startUART();
			
	}
	//=================================Init================================

	initView();


	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CgaoqingDlg::setConfig(CString category,CString item,CString val)
{
	CString add;
	TCHAR dir[BUFSIZE];
	::GetCurrentDirectory(sizeof(dir),dir);
	sprintf_s(add.GetBuffer(BUFSIZE),BUFSIZE,"%s\\config.ini", dir);
	add.ReleaseBuffer();
	::WritePrivateProfileString(category,item,val,add);
}
CString CgaoqingDlg::getConfig(CString category,CString item,CString def)
{
	CString add;
	CString str;
	TCHAR dir[BUFSIZE];
	::GetCurrentDirectory(sizeof(dir),dir);
	sprintf_s(add.GetBuffer(BUFSIZE),BUFSIZE,"%s\\config.ini", dir);
	add.ReleaseBuffer();
	::GetPrivateProfileString(category,item,def,str.GetBuffer(MAX_PATH),MAX_PATH,add);
	return str;
}
void CgaoqingDlg::initParam()
{
	

	direction         = 0;
	dLogin.parent = this;
	dFind.parent = this;
	dSetting.parent = this;

	camer_a = atoi(getConfig("CAMA","Enable","1"));
	camer_b = atoi(getConfig("CAMB","Enable","1"));
	camer_c = atoi(getConfig("CAMC","Enable","1"));
	camer_d = atoi(getConfig("CAMD","Enable","1"));

	path = getConfig("CONFIG","PATH",".\\");
	/*
	ser_com = new CSerialPort;
	ser_com->InitPort(Port_N, Port_BAUD, Port_PARITY , Port_DBITS, Port_STOP, EV_RXCHAR);
	ser_com->ReadFun= &CgaoqingDlg::ser_com_read;
	ser_com->OpenListenThread();
	ser_com->pWnd = this;*/

	LONG nPort;
	if(camer_a){

		mHik_a = new HikController();
		mHik_a->wind = GetDlgItem(IDC_STATIC_PLAY_A)->m_hWnd;
		PlayM4_GetPort(&nPort);
		mHik_a->play_no = nPort;
	} 
	if(camer_b){

		mHik_b = new HikController();
		mHik_b->wind = GetDlgItem(IDC_STATIC_PLAY_B)->m_hWnd;
		PlayM4_GetPort(&nPort);
		mHik_b->play_no = nPort;
	} 
	if(camer_c){

		mHik_c = new HikController();
		mHik_c->wind = GetDlgItem(IDC_STATIC_PLAY_C)->m_hWnd;
		PlayM4_GetPort(&nPort);
		mHik_c->play_no = nPort;
	} 
	if(camer_d){

		mHik_d = new HikController();
		mHik_d->wind = GetDlgItem(IDC_STATIC_PLAY_D)->m_hWnd;
		PlayM4_GetPort(&nPort);
		mHik_d->play_no = nPort;
	} 
	/*
	mHik_b->wind = GetDlgItem(IDC_STATIC_PLAY_B)->m_hWnd;
	mHik_c->wind = GetDlgItem(IDC_STATIC_PLAY_C)->m_hWnd;
	mHik_d->wind = GetDlgItem(IDC_STATIC_PLAY_D)->m_hWnd;

	
	
	PlayM4_GetPort(&nPort);
	mHik_b->play_no = nPort;
	PlayM4_GetPort(&nPort);
	mHik_c->play_no = nPort;
	PlayM4_GetPort(&nPort);
	mHik_d->play_no = nPort;

*/
	TRY   
	{      
		//		m_db.OpenEx(_T("DSN = gaoqingtest;UID = sa;PWD =gaoqing1"),CDatabase::noOdbcDialog);//连接到一个名为Test的数据源   
		//m_db.OpenEx(_T("Driver={SQL Server};Server=mySQLServer;UID=sa; PWD=myPassword;Database=Northwind;"); 
		m_db.OpenEx(_T("DSN=gaoqing;UID=sa;PWD=gaoqing;"),CDatabase::noOdbcDialog);//连接到一个名为Test的数据源   
		//m_db.OpenEx(_T("Driver={SQL Server};Server=mySQLServer;UID=sa; PWD=myPassword;Database=Northwind;"); 
		//m_db.OpenEx(_T("DSN=gaoqing;"),CDatabase::noOdbcDialog);//连接到一个名为Test的数据源   
		rs_train.m_pDatabase = &m_db;   
		//rs_details.m_pDatabase = &m_db;  
		//rs_train.Open(CRecordset::snapshot,_T("tbl_train"));  
		//rs_details.Open(CRecordset::snapshot,_T("tbl_train_details")); 
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

}
void CgaoqingDlg::startUART()
{
	Port_N        = atoi(getConfig("UART","Port","1"));
	Port_BAUD     = 38400;
	Port_PARITY   = 'N';
	Port_DBITS    = 8;
	Port_STOP     = 1;

	ser_com = new CSerialPort;
	ser_com->InitPort(Port_N, Port_BAUD, Port_PARITY , Port_DBITS, Port_STOP, EV_RXCHAR);
	ser_com->ReadFun= &CgaoqingDlg::ser_com_read;
	ser_com->OpenListenThread();
	ser_com->pWnd = this;
}

void CgaoqingDlg::stopUART()
{
	delete ser_com;

}
char CgaoqingDlg::rx_ser[50]={};
int  CgaoqingDlg::count_ser=0;
int  CgaoqingDlg::longth_ser=0;
char CgaoqingDlg::command_ser=0;
void CgaoqingDlg::ser_com_read(char rd)
{
	if((rd == 0x02)||(rx_ser[0]==0x02))
	{
		//#ifdef TEST_PROCESS
		//seb[sei++]=rd;sei%=500;
		//#endif
		switch (rd)
		{
		case 0x02:
			rx_ser[0]=rd;
			count_ser=0;
			break;
			//==============================================================================
		case 'S':
			if(count_ser>1) {rx_ser[count_ser++]=rd;break;}
			rx_ser[1]=rd;
			count_ser=2;
			longth_ser=1;
			command_ser='S';
			break;
		case 'T':
			if(count_ser>1) {rx_ser[count_ser++]=rd;break;}
			rx_ser[1]=rd;
			count_ser=2;
			longth_ser=12;
			command_ser='T';
			break;
		case 'C':
			if(count_ser>1) {rx_ser[count_ser++]=rd;break;}
			rx_ser[1]=rd;
			count_ser=2;
			longth_ser=2;
			command_ser='C';
			break;
		case 'N':
			if(count_ser>1) {rx_ser[count_ser++]=rd;break;}
			rx_ser[1]=rd;
			count_ser=2;
			longth_ser=20;
			command_ser='N';
			break;
		case 'A':
			if(count_ser>1) {rx_ser[count_ser++]=rd;break;}
			rx_ser[1]=rd;
			count_ser=2;
			longth_ser=5;
			command_ser='A';
			break;
		case 'J':
			if(count_ser>1) {rx_ser[count_ser++]=rd;break;}
			rx_ser[1]=rd;
			count_ser=2;
			longth_ser=0;
			command_ser='J';
			break;
		case 's':
			if(count_ser>1) {rx_ser[count_ser++]=rd;break;}
			rx_ser[1]=rd;
			count_ser=2;
			longth_ser=0;
			command_ser='s';
			break;
		case 'F':
			if(count_ser>1) {rx_ser[count_ser++]=rd;break;}
			rx_ser[1]=rd;
			count_ser=2;
			longth_ser=0;
			command_ser='F';
			break;
		case 'X':
			if(count_ser>1) {rx_ser[count_ser++]=rd;break;}
			rx_ser[1]=rd;
			count_ser=2;
			longth_ser=19;
			command_ser='X';
			break;
		case 'D':
			if(count_ser>1) {rx_ser[count_ser++]=rd;break;}
			rx_ser[1]=rd;
			count_ser=2;
			longth_ser=4;
			command_ser='D';
			break;
			//=================================================================================
		case 0x03:
			rx_ser[0]=0;
			rx_ser[count_ser]=rd;
			if ((count_ser == (longth_ser+2))||(count_ser == (longth_ser+3)))
			{
				Command_Disposal_Ser(command_ser);
			}
			break;
		default:
			rx_ser[count_ser]=rd;
			count_ser++;
			break;
		}
	}
}
void CgaoqingDlg::OnOK(){

	return;
}
void CgaoqingDlg::Command_Disposal_Ser(char command)
{
	//unsigned int hour;
	
	switch (command)
	{   
	case 'S':
		car_n=0;
		if (rx_ser[2] == '0') direction = 0;
		else direction = 1;
		dLogin.output_list_.ResetContent();
		post_message(_T("列车到达...\r\n"));

		if(camer_a && mHik_a->displayHik() != FAIL){

		}
		if(camer_b && mHik_b->displayHik() != FAIL){

		}
		if(camer_c && mHik_c->displayHik() != FAIL){

		}
		if(camer_d && mHik_d->displayHik() != FAIL){

		}

		stime = timeGetTime();

		tabSettings.SetCurSel(0);

		dLogin.ShowWindow(true);
		dFind.ShowWindow(false);
		dSetting.ShowWindow(false);

		GetDlgItem(IDC_SLIDER_PLAY)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_REC_PLAY)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_REC_FAST)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_REC_SLOW)->EnableWindow(false);

		if (stopVideo())
		{
			
		}
		CStatic *wd;
		s_Play=FALSE;
		GetDlgItem(IDC_BUTTON_REC_PLAY)->SetWindowText("播放");
		wd = (CStatic *)GetDlgItem(IDC_STATIC_PLAY_A);
		wd->SetBitmap(NULL);
		wd = (CStatic *)GetDlgItem(IDC_STATIC_PLAY_B);
		wd->SetBitmap(NULL);
		wd = (CStatic *)GetDlgItem(IDC_STATIC_PLAY_C);
		wd->SetBitmap(NULL);
		wd = (CStatic *)GetDlgItem(IDC_STATIC_PLAY_D);
		wd->SetBitmap(NULL);
		Invalidate(TRUE); 

		tabSettings.EnableWindow(false);
		break;
	case 'T':
		{
			timename = "20"+CString(rx_ser+2,12);
			foldername.Format(_T("%s%s"),path,timename);
			//hour=(rx_ser[8]-0x30)*10+(rx_ser[9]-0x30);
			_mkdir(foldername);
			pFile = ::CreateFile(_T(foldername+"\\index.txt"),
				GENERIC_WRITE,
				1,
				NULL,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

			if (pFile == INVALID_HANDLE_VALUE)  
			{  
				AfxMessageBox("Invalid handle when read file create/n");  
			}else{

				//rs_train.AddNew();
				//rs_train.tran_name = timename; 
				CString sql;
				sql.Format( _T("INSERT INTO tbl_train (tran_name) VALUES (\'%s\')"),timename);
					
					
					
					//%d\r\n%d\t%d\t%s\t%d\tJ\r\n",direction,car_n,car_speed,CString(car_number,20),pos);

				//CString sql = _T("INSERT UserInfo(tran_name)VALUES(" 123, 'Bob');

				//try{   
				//rs_train.Open();
				//rs_train.AddNew();
				m_db.ExecuteSQL(sql);
				CString train_sql = _T("SELECT top 1 tran_id from tbl_train order by tran_id desc");
				TRY        
				{       
					rs_train.Open(AFX_DB_USE_DEFAULT_TYPE,train_sql);  
					  
					rs_train.GetFieldValue(_T("tran_id"),train_id);
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
				//rs_train.Update();
				//}


				//if(!rs_train.Update())  
				//{  
				//	AfxMessageBox(_T("Add New failed!"));  
				//}  

			}

			if(camer_a && mHik_a->saveFileHik(foldername+"\\"+timename+"lt.mp4")==SUCESS){


			}	
			if(camer_b && mHik_b->saveFileHik(foldername+"\\"+timename+"rt.mp4")==SUCESS){


			}
			if(camer_c && mHik_c->saveFileHik(foldername+"\\"+timename+"lb.mp4")==SUCESS){


			}
			if(camer_d && mHik_d->saveFileHik(foldername+"\\"+timename+"rb.mp4")==SUCESS){


			}

			post_message(_T("时间[%s]\r\n"),("20"+CString(rx_ser+2,12)));
			post_message(_T("方向[%s]\r\n"),direction == 0 ? _T("正向"): _T("反向"));

			break;
		}
	case 'C':	
		{
			CString str;
			DWORD pos = timeGetTime() - stime;
			if(pFile == INVALID_HANDLE_VALUE) break;

			if (foldername=="")  break;
			if(rx_ser[4]==0x03?1:0) car_n=(rx_ser[3]-0x30)+(rx_ser[2]-0x30)*10;
			else car_n= (rx_ser[4]-0x30)+(rx_ser[3]-0x30)*10+ (rx_ser[2]-0x30)*100;

			//post_message(_T("车位...%d\r\n"),car_n);
			post_message(_T("%d---%s---%s\r\n"),car_speed,CString(car_number,7),CString(car_number+7,13));
			if(car_n == 1) 
			{
				//str.Format("%d\r\n%d\t%d\t%s\t%d\tJ\r\n",direction,car_n,car_speed,CString(car_number,20),pos);
				str.Format("%d\r\n%d\t%d\t%s\t%d\tJ\r\n",direction,car_n,car_speed,CString(car_number,20),pos);

			}
			else 
			{
				//str.Format("%d\t%d\t%s\t%d\t \r\n",car_n,car_speed,CString(car_number,20),pos);

				if(car_n == 5) {
					int re=0;
					if(camer_a && mHik_a->playbackHik(foldername+"\\"+timename+"lt.mp4")==SUCESS){

						mHik_a->playslowHik();
						re=1;
					}
					if(camer_b && mHik_b->playbackHik(foldername+"\\"+timename+"rt.mp4")==SUCESS){

						mHik_b->playslowHik();
						re=1;
					}
					if(camer_c && mHik_c->playbackHik(foldername+"\\"+timename+"lb.mp4")==SUCESS){

						mHik_c->playslowHik();
						re=1;
					}
					if(camer_d && mHik_d->playbackHik(foldername+"\\"+timename+"rb.mp4")==SUCESS){

						mHik_d->playslowHik();
						re=1;
					}
					if(re==1){
						SetTimer(LIVE_TIMER,500,NULL);
					}
				}

			}
			index_str+=str;
			CString sql;
			sql.Format( _T("INSERT INTO tbl_train_details (tran_id,car_speed,car_type,car_number,car_time) VALUES (%s,%d,\'%s\','\%s'\,%d)"),train_id,car_speed,CString(car_number,7),CString(car_number+7,13),pos);
			m_db.ExecuteSQL(sql);

			break;
		}
	case 'N':
		memcpy(car_number,rx_ser+2,20);
		break;
	case 'A':
		break;
	case 'J':
		break;
	case 's':

		tabSettings.EnableWindow(true);
		if(camer_a){
			//mHik_a->stopplaybackHik();
			mHik_a->stopsaveFileHik();
			mHik_a->stopdisplayHik();
			//mHik_a->stopplaybackHik();
		}
		if(camer_b){
			//mHik_b->stopplaybackHik();
			mHik_b->stopsaveFileHik();
			mHik_b->stopdisplayHik();
			//mHik_b->stopplaybackHik();
		}
		if(camer_c){
			//mHik_c->stopplaybackHik();
			mHik_c->stopsaveFileHik();
			mHik_c->stopdisplayHik();
			//mHik_c->stopplaybackHik();
		}
		if(camer_d){
			//mHik_d->stopplaybackHik();
			mHik_d->stopsaveFileHik();
			mHik_d->stopdisplayHik();
			//mHik_d->stopplaybackHik();
		}

		rs_train.Close();
		memset(car_number,'X',20);

		post_message(_T("列车已通过..."));
		if(pFile != INVALID_HANDLE_VALUE) 
		{

			DWORD retVal = 0,outBytes = 0;  
			retVal = ::SetFilePointer(pFile, 0, NULL, FILE_END);
			if (retVal == 0xFFFFFFFF)  
			{  
				retVal = GetLastError();  
				AfxMessageBox("LastError = %d/n", retVal);   
			}  


			if (WriteFile(pFile, index_str.GetBuffer(), index_str.GetLength(), &outBytes, NULL) == FALSE)  
			{  
				AfxMessageBox("Write file failed/n");  
			}  
			if (0 == FlushFileBuffers(pFile))  
			{  
				AfxMessageBox("Flush failed/n");  
			} 

			::CloseHandle(pFile);
			pFile = INVALID_HANDLE_VALUE;
		}
		break;
	case 'F':
		post_message(_T("握手...\r\n"));
		if(pFile != INVALID_HANDLE_VALUE) {::CloseHandle(pFile);pFile = INVALID_HANDLE_VALUE;}
		memset(car_number,'X',20);
		car_n=0;
		tabSettings.EnableWindow(true);
		break;
	case 'X':
		break;
	case 'D':
		car_speed=(rx_ser[2]-0x30)*1000+(rx_ser[3]-0x30)*100+(rx_ser[4]-0x30)*10+rx_ser[5]-0x30;
		break;
	default:
		break;
	}
}
CRect CgaoqingDlg::getSize(CWnd *pWnd)
{
	CRect rect;
	pWnd->GetWindowRect(&rect);  
	ScreenToClient(&rect);
	return rect;
}
void CgaoqingDlg::saveSize()
{

	CWnd *pWnd;   
	pWnd = GetDlgItem(IDC_STATIC_PLAY_A);  
	screenA = getSize(pWnd);  
	pWnd = GetDlgItem(IDC_STATIC_PLAY_B);  
	screenB = getSize(pWnd);  
	pWnd = GetDlgItem(IDC_STATIC_PLAY_C);  
	screenC = getSize(pWnd); 
	pWnd = GetDlgItem(IDC_STATIC_PLAY_D);  
	screenD = getSize(pWnd); 


	pWnd = GetDlgItem(IDC_SLIDER_PLAY);  
	screenProcess = getSize(pWnd); 

	pWnd = GetDlgItem(IDC_BUTTON_REC_PLAY);  
	screenPlay = getSize(pWnd); 
	pWnd = GetDlgItem(IDC_BUTTON_REC_FAST);  
	screenFast = getSize(pWnd); 
	pWnd = GetDlgItem(IDC_BUTTON_REC_SLOW);  
	screenSlow = getSize(pWnd); 
}
void CgaoqingDlg::restoreSize()
{
	CWnd *pWnd;   
	pWnd = GetDlgItem(IDC_STATIC_PLAY_A);  
	if(pWnd){
		ChangeSize(pWnd, screenA.left, screenA.top,screenA.right-screenA.left,screenA.bottom-screenA.top);  
		pWnd->ModifyStyle(SS_NOTIFY,0);
	}
	pWnd = GetDlgItem(IDC_STATIC_PLAY_B);  
	if(pWnd){
		ChangeSize(pWnd, screenB.left, screenB.top,screenB.right-screenB.left,screenB.bottom-screenB.top);  
		pWnd->ModifyStyle(SS_NOTIFY,0);
	}
	pWnd = GetDlgItem(IDC_STATIC_PLAY_C);  
	if(pWnd){
		ChangeSize(pWnd, screenC.left, screenC.top,screenC.right-screenC.left,screenC.bottom-screenC.top); 
		pWnd->ModifyStyle(SS_NOTIFY,0);
	}
	pWnd = GetDlgItem(IDC_STATIC_PLAY_D);  
	if(pWnd){
		ChangeSize(pWnd, screenD.left, screenD.top,screenD.right-screenD.left,screenD.bottom-screenD.top);
		pWnd->ModifyStyle(SS_NOTIFY,0);
	}

	pWnd = GetDlgItem(IDC_SLIDER_PLAY);  
	ChangeSize(pWnd, screenProcess.left, screenProcess.top,screenProcess.right-screenProcess.left,screenProcess.bottom-screenProcess.top);

	pWnd = GetDlgItem(IDC_BUTTON_REC_PLAY);  
	ChangeSize(pWnd, screenPlay.left, screenPlay.top,screenPlay.right-screenPlay.left,screenPlay.bottom-screenPlay.top);
	pWnd = GetDlgItem(IDC_BUTTON_REC_FAST);  
	ChangeSize(pWnd, screenFast.left, screenFast.top,screenFast.right-screenFast.left,screenFast.bottom-screenFast.top);
	pWnd = GetDlgItem(IDC_BUTTON_REC_SLOW);  
	ChangeSize(pWnd, screenSlow.left, screenSlow.top,screenSlow.right-screenSlow.left,screenSlow.bottom-screenSlow.top);
	
}
CRect CgaoqingDlg::ChangeSize(CWnd *pWnd, int left, int top, int width, int height)  
{  
	if(pWnd)  //判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建   
	{  
		CRect old_rect,new_rect;   //获取控件变化前的大小  
		old_rect = getSize(pWnd);
		//pWnd->GetWindowRect(&old_rect);  
		//ScreenToClient(&old_rect);//将控件大小转换为在对话框中的区域坐标   
		//rect.left=rect.left*cx/m_rect.Width();//调整控件大小  
		//rect.right=rect.right*cx/m_rect.Width();  
		//rect.top=rect.top*cy/m_rect.Height();  
		//rect.bottom=rect.bottom*cy/m_rect.Height();  
		new_rect.left=left;//调整控件大小  
		new_rect.right=left+width;  
		new_rect.top=top;  
		new_rect.bottom=top+height;  
		pWnd->MoveWindow(new_rect);//设置控件大小  
		return old_rect;
	}  
	return NULL;
} 
//afx_msg void OnSysCommand(UINT nID, LPARAM lParam)



DWORD CgaoqingDlg::getVideoTime()
{
	DWORD len=0;
	if(mHik_a != NULL)
		len = mHik_a->playtimeHik();
	if(len ==0){
		if(mHik_b != NULL){
			len = mHik_a->playtimeHik();
		}
	}else{
		if(mHik_b != NULL){
			len =min(len,mHik_b->playtimeHik());
		}
	}
		

	if(len ==0){
		if(mHik_c != NULL){
			len = mHik_c->playtimeHik();
		}
	}else{
		if(mHik_c != NULL){
			len =min(len,mHik_c->playtimeHik());
		}
	}

	if(len ==0){
		if(mHik_d != NULL){
			len = mHik_d->playtimeHik();
		}
	}else{
		if(mHik_d != NULL){
			len =min(len,mHik_d->playtimeHik());
		}
	}
	if(len ==0)
		len = -1;


	return len;
}
DWORD CgaoqingDlg::getPlayedTime()
{
	DWORD len=0;
	if(mHik_a != NULL)
		return mHik_a->playedtimeHik();
	if(mHik_b != NULL)
		return mHik_a->playedtimeHik();
	if(mHik_c != NULL)
		return mHik_c->playedtimeHik();
	if(mHik_d != NULL)
		return mHik_d->playedtimeHik();
	return 0;
}
int CgaoqingDlg::playVideoPos(DWORD pos)
{
	if(
		mHik_a !=NULL ?mHik_a->playposHik(pos)==SUCESS:SUCESS &&
		mHik_b !=NULL ?mHik_b->playposHik(pos)==SUCESS:SUCESS &&
		mHik_c !=NULL ?mHik_c->playposHik(pos)==SUCESS:SUCESS &&
		mHik_d !=NULL ?mHik_d->playposHik(pos)==SUCESS:SUCESS){
		return SUCESS;
	}
	return FAIL;
}
int CgaoqingDlg::stopVideo()
{
	if(
		mHik_a !=NULL ?mHik_a->stopplaybackHik()==SUCESS:SUCESS &&
		mHik_b !=NULL ?mHik_b->stopplaybackHik()==SUCESS:SUCESS &&
		mHik_c !=NULL ?mHik_c->stopplaybackHik()==SUCESS:SUCESS &&
		mHik_d !=NULL ?mHik_d->stopplaybackHik()==SUCESS:SUCESS){
		return SUCESS;
	}
	return FAIL;
}
int CgaoqingDlg::resumeVideo()
{
	if(
		mHik_a !=NULL ?mHik_a->resumeHik()==SUCESS:SUCESS &&
		mHik_b !=NULL ?mHik_b->resumeHik()==SUCESS:SUCESS &&
		mHik_c !=NULL ?mHik_c->resumeHik()==SUCESS:SUCESS &&
		mHik_d !=NULL ?mHik_d->resumeHik()==SUCESS:SUCESS){
		return SUCESS;
	}
	
	if(	mHik_a !=NULL)  mHik_a->pauseHik();
	if(	mHik_b !=NULL)  mHik_b->pauseHik();
	if(	mHik_c !=NULL)  mHik_c->pauseHik();
	if(	mHik_d !=NULL)  mHik_d->pauseHik();
	
	return FAIL;
}
int CgaoqingDlg::pauseVideo()
{
	if(
		mHik_a !=NULL ?mHik_a->pauseHik()==SUCESS:SUCESS &&
		mHik_b !=NULL ?mHik_b->pauseHik()==SUCESS:SUCESS &&
		mHik_c !=NULL ?mHik_c->pauseHik()==SUCESS:SUCESS &&
		mHik_d !=NULL ?mHik_d->pauseHik()==SUCESS:SUCESS){
		return SUCESS;
	}
	
	if(	mHik_a !=NULL)  mHik_a->playHik();
	if(	mHik_b !=NULL)  mHik_b->playHik();
	if(	mHik_c !=NULL)  mHik_c->playHik();
	if(	mHik_d !=NULL)  mHik_d->playHik();
	
	return FAIL;
}
int CgaoqingDlg::fastVideo()
{
	if(
		mHik_a !=NULL ?mHik_a->fastHik()==SUCESS:SUCESS &&
		mHik_b !=NULL ?mHik_b->fastHik()==SUCESS:SUCESS &&
		mHik_c !=NULL ?mHik_c->fastHik()==SUCESS:SUCESS &&
		mHik_d !=NULL ?mHik_d->fastHik()==SUCESS:SUCESS){
		return SUCESS;
	}
	
	if(	mHik_a !=NULL)  mHik_a->playHik();
	if(	mHik_b !=NULL)  mHik_b->playHik();
	if(	mHik_c !=NULL)  mHik_c->playHik();
	if(	mHik_d !=NULL)  mHik_d->playHik();
	
	return FAIL;
}
int CgaoqingDlg::slowVideo()
{
	if(
		mHik_a !=NULL ?mHik_a->slowHik()==SUCESS:SUCESS &&
		mHik_b !=NULL ?mHik_b->slowHik()==SUCESS:SUCESS &&
		mHik_c !=NULL ?mHik_c->slowHik()==SUCESS:SUCESS &&
		mHik_d !=NULL ?mHik_d->slowHik()==SUCESS:SUCESS){
		return SUCESS;
	}
	
	if(	mHik_a !=NULL)  mHik_a->resumeHik();
	if(	mHik_b !=NULL)  mHik_b->resumeHik();
	if(	mHik_c !=NULL)  mHik_c->resumeHik();
	if(	mHik_d !=NULL)  mHik_d->resumeHik();
	
	return FAIL;
}
int CgaoqingDlg::saveParams(BYTE bright,BYTE contrast,BYTE sharpness,BYTE saturation,int exp_mode,int exp_time,int daynight)
{
	if(
		mHik_a !=NULL ?mHik_a->saveParamHik(bright,contrast,sharpness,saturation,exp_mode,exp_time,daynight)==SUCESS:SUCESS &&
		mHik_b !=NULL ?mHik_b->saveParamHik(bright,contrast,sharpness,saturation,exp_mode,exp_time,daynight)==SUCESS:SUCESS &&
		mHik_c !=NULL ?mHik_c->saveParamHik(bright,contrast,sharpness,saturation,exp_mode,exp_time,daynight)==SUCESS:SUCESS &&
		mHik_d !=NULL ?mHik_d->saveParamHik(bright,contrast,sharpness,saturation,exp_mode,exp_time,daynight)==SUCESS:SUCESS){
		return SUCESS;
	}else
		return FAIL;
}
PARAM_STRU CgaoqingDlg::getParams()
{
	if(mHik_a !=NULL){
		return mHik_a->getParamHik();
	}else if(mHik_b !=NULL){
		return mHik_b->getParamHik();
	}else if(mHik_c !=NULL){
		return mHik_c->getParamHik();
	}else if(mHik_d !=NULL){
		return mHik_d->getParamHik();
	}else {
		PARAM_STRU str;
		return str;
	}
		//return (PARAM_STRU)null;
}
bool CgaoqingDlg::playVideo(CString patha,CString pathb,CString pathc,CString pathd)
{
	bool result=FAIL;
	if(!patha.IsEmpty()){
		if(mHik_a->playbackHik(patha)==SUCESS){
			result = SUCESS;
		}else
			result = FAIL;
	}

	if(!pathb.IsEmpty()){ 
		if(mHik_b->playbackHik(pathb)==SUCESS){
			result = SUCESS;
		}else
			result = FAIL;
	}
	if(!pathc.IsEmpty()){
		if(mHik_c->playbackHik(pathc)==SUCESS){

			result = SUCESS;
		}else
			result = FAIL;
	}
	if(!pathd.IsEmpty()){ \
		if(mHik_d->playbackHik(pathd)==SUCESS){
			result = SUCESS;
		}else
			result = FAIL;
	}
	return result;
}
void CgaoqingDlg::post_message(LPCTSTR format, ...)
{
	if (dLogin.output_list_)
	{
		CString msg;
		va_list arg_list = 0;
		va_start(arg_list, format);		
		msg.FormatV(format, arg_list);
		va_end(arg_list);

		dLogin.output_list_.AddString(msg);
		dLogin.output_list_.SetCurSel(dLogin.output_list_.GetCount() - 1);
	}
}
//================================11.5===========================================





void CgaoqingDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int CurSel; 
	CStatic *wd;

	CurSel=tabSettings.GetCurSel(); 

	switch(CurSel) 

	{

	case 0: 

		dLogin.ShowWindow(true);
		dFind.ShowWindow(false);
		dSetting.ShowWindow(false);

		GetDlgItem(IDC_SLIDER_PLAY)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_REC_PLAY)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_REC_FAST)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_REC_SLOW)->EnableWindow(false);

		if (stopVideo())
		{
			
		}
			
		s_Play=FALSE;
		GetDlgItem(IDC_BUTTON_REC_PLAY)->SetWindowText("播放");
		wd = (CStatic *)GetDlgItem(IDC_STATIC_PLAY_A);
		wd->SetBitmap(NULL);
		wd = (CStatic *)GetDlgItem(IDC_STATIC_PLAY_B);
		wd->SetBitmap(NULL);
		wd = (CStatic *)GetDlgItem(IDC_STATIC_PLAY_C);
		wd->SetBitmap(NULL);
		wd = (CStatic *)GetDlgItem(IDC_STATIC_PLAY_D);
		wd->SetBitmap(NULL);
		Invalidate(TRUE); 
		dSetting.Logout();
		break; 

	case 1: 

		dLogin.ShowWindow(false);
		dFind.ShowWindow(true);
		dSetting.ShowWindow(false);

		GetDlgItem(IDC_SLIDER_PLAY)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_REC_PLAY)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_REC_FAST)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_REC_SLOW)->EnableWindow(false);

		dSetting.Logout();
		break; 

	case 2: 

		dLogin.ShowWindow(false);
		dFind.ShowWindow(false);
		dSetting.ShowWindow(true);

		GetDlgItem(IDC_SLIDER_PLAY)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_REC_PLAY)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_REC_FAST)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_REC_SLOW)->EnableWindow(false);

		if (stopVideo())
		{
			
		}

		break; 

	default: break; 

	} 

	*pResult = 0;
}
CString CgaoqingDlg::getCStringFromConfig(CString cam,CString sub)
{
	return _T(getConfig(cam,sub,""));
}
bool CgaoqingDlg::loginCamera()
{
	//UpdateData(TRUE);
	//prepare for ip
	//CString DeviceIp;
	//BYTE nField0,nField1,nField2,nField3; 
	bool re=true;
	
	if(camer_a){
		
		mHik_a->ip = getCStringFromConfig("CAMA","Ip");
		mHik_a->port =atoi(getCStringFromConfig("CAMA","Port"));
		mHik_a->name = getCStringFromConfig("CAMA","User");
		mHik_a->pwd = getCStringFromConfig("CAMA","Password");


		if(mHik_a->loginHik()!=SUCESS){
			re=false;
		}
	}
	if(camer_b && re){

		mHik_b->ip = getCStringFromConfig("CAMB","Ip");
		mHik_b->port =atoi(getCStringFromConfig("CAMB","Port"));
		mHik_b->name = getCStringFromConfig("CAMB","User");
		mHik_b->pwd = getCStringFromConfig("CAMB","Password");

		if(mHik_b->loginHik()!=SUCESS){
			re=false;
		}
	}
	if(camer_c && re){

		mHik_c->ip = getCStringFromConfig("CAMC","Ip");
		mHik_c->port =atoi(getCStringFromConfig("CAMC","Port"));
		mHik_c->name = getCStringFromConfig("CAMC","User");
		mHik_c->pwd = getCStringFromConfig("CAMC","Password");

		if(mHik_c->loginHik()!=SUCESS){
			re=false;
		}
	}
	if(camer_d && re){


		mHik_d->ip = getCStringFromConfig("CAMD","Ip");
		mHik_d->port =atoi(getCStringFromConfig("CAMD","Port"));
		mHik_d->name = getCStringFromConfig("CAMD","User");
		mHik_d->pwd = getCStringFromConfig("CAMD","Password");

		if(mHik_d->loginHik()!=SUCESS){
			re=false;
		}
	}
	if(!re)
	{
		logoutCamera();
	}
	return re;
}
void CgaoqingDlg::OnBnClickedButtonRecPlay()
{
	if (s_Play)
	{
		if (pauseVideo())
		{
			
		}
			
		s_Play=FALSE;
		GetDlgItem(IDC_BUTTON_REC_PLAY)->SetWindowText("播放");
		GetDlgItem(IDC_BUTTON_REC_FAST)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_REC_SLOW)->EnableWindow(false);
	}
	else
	{
		if (resumeVideo())
		{
			
		}
		s_Play=TRUE;
		GetDlgItem(IDC_BUTTON_REC_PLAY)->SetWindowText("暂停");
		GetDlgItem(IDC_BUTTON_REC_FAST)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_REC_SLOW)->EnableWindow(true);
	}
}

void CgaoqingDlg::OnBnClickedButtonRecFast()
{
	if(playSpeed <4){
	{
		if (fastVideo()==SUCESS)
			playSpeed++;	

			CString str;
			switch(playSpeed){
				case -4:
					GetDlgItem(IDC_BUTTON_REC_SLOW)->SetWindowText("慢放X16");
					break;
				case -3:
					GetDlgItem(IDC_BUTTON_REC_SLOW)->SetWindowText("慢放X8");
					break;
				case -2:
					GetDlgItem(IDC_BUTTON_REC_SLOW)->SetWindowText("慢放X4");
					break;
				case -1:
					GetDlgItem(IDC_BUTTON_REC_SLOW)->SetWindowText("慢放X2");
					break;
				case 0:
					GetDlgItem(IDC_BUTTON_REC_FAST)->SetWindowText("快放");
					GetDlgItem(IDC_BUTTON_REC_SLOW)->SetWindowText("慢放");
					break;

				case 1:
					GetDlgItem(IDC_BUTTON_REC_FAST)->SetWindowText("快放X2");
					break;
				case 2:
					GetDlgItem(IDC_BUTTON_REC_FAST)->SetWindowText("快放X4");
					break;
				case 3:
					GetDlgItem(IDC_BUTTON_REC_FAST)->SetWindowText("快放X8");
					break;
				case 4:
					GetDlgItem(IDC_BUTTON_REC_FAST)->SetWindowText("快放X16");
					break;
			}
			
		}
	}
}

void CgaoqingDlg::OnBnClickedButtonRecSlow()
{
	if(playSpeed >-4){
	{
		if (slowVideo()==SUCESS)
			playSpeed--;	

			CString str;
			switch(playSpeed){
				case -4:
					GetDlgItem(IDC_BUTTON_REC_SLOW)->SetWindowText("慢放X16");
					break;
				case -3:
					GetDlgItem(IDC_BUTTON_REC_SLOW)->SetWindowText("慢放X8");
					break;
				case -2:
					GetDlgItem(IDC_BUTTON_REC_SLOW)->SetWindowText("慢放X4");
					break;
				case -1:
					GetDlgItem(IDC_BUTTON_REC_SLOW)->SetWindowText("慢放X2");
					break;
				case 0:
					GetDlgItem(IDC_BUTTON_REC_FAST)->SetWindowText("快放");
					GetDlgItem(IDC_BUTTON_REC_SLOW)->SetWindowText("慢放");
					break;

				case 1:
					GetDlgItem(IDC_BUTTON_REC_FAST)->SetWindowText("快放X2");
					break;
				case 2:
					GetDlgItem(IDC_BUTTON_REC_FAST)->SetWindowText("快放X4");
					break;
				case 3:
					GetDlgItem(IDC_BUTTON_REC_FAST)->SetWindowText("快放X8");
					break;
				case 4:
					GetDlgItem(IDC_BUTTON_REC_FAST)->SetWindowText("快放X16");
					break;
			}
			
		}	
	}
}

void CgaoqingDlg::OnNMReleasedcaptureSliderPlay(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nPos = 0;
	nPos = playslider.GetPos();
	//m_sliderPlayProgress.SetPos(nPos);
	if(playVideoPos(nPos)==SUCESS)
	{
	}
	/*if (!PlayM4_SetPlayedTimeEx(USED_PORT_A, nPos))
	{
	DWORD getVideoTime();
	int playVideoPos(DWORD pos);
	}*/
	*pResult = 0;
}
void CgaoqingDlg::logoutCamera()
{
	if(camer_a && mHik_a->stopFileHik()==SUCESS){
			
		if(mHik_a->stopPlayHik()==SUCESS){
			if(mHik_a->logoutHik()==SUCESS){
					
			}
				

		}
			
	}

	if(camer_b && mHik_b->stopFileHik()==SUCESS){
			
		if(mHik_b->stopPlayHik()==SUCESS){
			if(mHik_b->logoutHik()==SUCESS){
					
			}
				

		}
			
	}

	if(camer_c && mHik_c->stopFileHik()==SUCESS){
			
		if(mHik_c->stopPlayHik()==SUCESS){
			if(mHik_c->logoutHik()==SUCESS){
					
			}
				

		}
			
	}

	if(camer_d && mHik_d->stopFileHik()==SUCESS){
			
		if(mHik_d->stopPlayHik()==SUCESS){
			if(mHik_d->logoutHik()==SUCESS){
					
			}
				

		}
			
	}
}
void CgaoqingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_RESTORE)
	{
		//tabSettings.ShowWindow(SW_SHOW);
	}
	else if(nID == SC_MAXIMIZE)
	{
		//tabSettings.ShowWindow(SW_HIDE);
	}
	CDialog::OnSysCommand(nID, lParam);
}
void CgaoqingDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
/*
	if(maxScreen){

		if(nType==SIZE_MAXIMIZED){

			int id;
			CWnd *pWnd;
			pWnd = GetDlgItem(IDC_STATIC_PLAY_A); 
			pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_STATIC_PLAY_B); 
			pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_STATIC_PLAY_C); 
			pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_STATIC_PLAY_D); 
			pWnd->ShowWindow(SW_HIDE);

			switch(maxScreen)
			{
				case 1:
					id = IDC_STATIC_PLAY_A;  
					
					break;
				case 2:
					id = IDC_STATIC_PLAY_B;
					break;
				case 3:
					id = IDC_STATIC_PLAY_C;
					break;
				case 4:
					id = IDC_STATIC_PLAY_D;
					break;
				default:
					id = IDC_STATIC_PLAY_A;
					break;
			
			}
			   
			pWnd = GetDlgItem(id);  
			pWnd->ShowWindow(SW_SHOW);
			screenMax = ChangeSize(pWnd, cx*0.01, cy*0.01,cx*0.98,cy*0.94); 








			pWnd = GetDlgItem(IDC_SLIDER_PLAY);  
			screenProcess = ChangeSize(pWnd, cx*0.01, cy*0.96,cx*0.70,cy*0.03); 

			pWnd = GetDlgItem(IDC_BUTTON_REC_PLAY);  
			screenPlay = ChangeSize(pWnd, cx*0.73, cy*0.96,cx*0.08,cy*0.03); 
			pWnd = GetDlgItem(IDC_BUTTON_REC_FAST);  
			screenFast = ChangeSize(pWnd, cx*0.82, cy*0.96,cx*0.08,cy*0.03); 
			pWnd = GetDlgItem(IDC_BUTTON_REC_SLOW);  
			screenSlow = ChangeSize(pWnd, cx*0.91, cy*0.96,cx*0.08,cy*0.03); 


		
		}
	}else{*/
		if(nType==SIZE_MAXIMIZED){
			if(!maxScreen){
		tabSettings.ShowWindow(SW_HIDE);
			
		CWnd *pWnd;   
		pWnd = GetDlgItem(IDC_STATIC_PLAY_A);  
		screenA = ChangeSize(pWnd, cx*0.01, cy*0.01,cx*0.49,cy*0.47); 
		pWnd->ModifyStyle(0,SS_NOTIFY);
		pWnd = GetDlgItem(IDC_STATIC_PLAY_B);  
		screenB = ChangeSize(pWnd, cx*0.50, cy*0.01,cx*0.49,cy*0.47);  
		pWnd->ModifyStyle(0,SS_NOTIFY);
		pWnd = GetDlgItem(IDC_STATIC_PLAY_C);  
		screenC = ChangeSize(pWnd, cx*0.01, cy*0.48,cx*0.49,cy*0.47); 
		pWnd->ModifyStyle(0,SS_NOTIFY);
		pWnd = GetDlgItem(IDC_STATIC_PLAY_D);  
		screenD = ChangeSize(pWnd, cx*0.50, cy*0.48,cx*0.49,cy*0.47); 
		pWnd->ModifyStyle(0,SS_NOTIFY);


		pWnd = GetDlgItem(IDC_SLIDER_PLAY);  
		screenProcess = ChangeSize(pWnd, cx*0.01, cy*0.96,cx*0.70,cy*0.03); 

		pWnd = GetDlgItem(IDC_BUTTON_REC_PLAY);  
		screenPlay = ChangeSize(pWnd, cx*0.73, cy*0.96,cx*0.08,cy*0.03); 
		pWnd = GetDlgItem(IDC_BUTTON_REC_FAST);  
		screenFast = ChangeSize(pWnd, cx*0.82, cy*0.96,cx*0.08,cy*0.03); 
		pWnd = GetDlgItem(IDC_BUTTON_REC_SLOW);  
		screenSlow = ChangeSize(pWnd, cx*0.91, cy*0.96,cx*0.08,cy*0.03); 


			}


		}else if(nType==SIZE_RESTORED){
	
			if(is_minimized){
				 is_minimized = false;
			}else{
				//CWnd *pWnd;
				//pWnd = GetDlgItem(IDC_TAB); 
				//pWnd -> ShowWindow(SW_SHOW);
				if(tabSettings)
					tabSettings.ShowWindow(SW_SHOW);
				restoreSize();
			}
		}else if(nType==SIZE_MINIMIZED){

			is_minimized=true;
		}
	//}
	// TODO: Add your message handler code here
}

void CgaoqingDlg::OnStnDblclickStaticPlayA()
{
	// TODO: Add your control notification handler code here
	if(maxScreen){
		maxScreen = 0;

		CWnd *pWnd;   
		pWnd = GetDlgItem(IDC_STATIC_PLAY_A);   
		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_STATIC_PLAY_B);  
		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_STATIC_PLAY_C);   
		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_STATIC_PLAY_D);  
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_STATIC_PLAY_A);  
		ChangeSize(pWnd, screenMax.left, screenMax.top,screenMax.right-screenMax.left,screenMax.bottom-screenMax.top);  
		//pWnd->ModifyStyle(SS_NOTIFY,0);
		ModifyStyle(0,WS_MAXIMIZEBOX|WS_MINIMIZEBOX);
	}else{
		maxScreen = 1;
		ModifyStyle(WS_MAXIMIZEBOX|WS_MINIMIZEBOX,0);

		CWnd *pWnd;   
		//pWnd = GetDlgItem(IDC_STATIC_PLAY_A);   
		//pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_STATIC_PLAY_B);  
		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_STATIC_PLAY_C);   
		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_STATIC_PLAY_D);  
		pWnd->ShowWindow(SW_HIDE);

		CRect rt;
		GetClientRect(&rt);

		int cx = rt.right - rt.left;
		int cy = rt.bottom - rt.top;
		pWnd = GetDlgItem(IDC_STATIC_PLAY_A);  
		screenMax = ChangeSize(pWnd, cx*0.01, cy*0.01,cx*0.98,cy*0.94); 

		//pWnd->ModifyStyle(0,SS_NOTIFY);
	}
}

void CgaoqingDlg::OnStnDblclickStaticPlayB()
{
	// TODO: Add your control notification handler code here
	if(maxScreen){
		maxScreen = 0;

		CWnd *pWnd;   
		pWnd = GetDlgItem(IDC_STATIC_PLAY_A);   
		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_STATIC_PLAY_B);  
		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_STATIC_PLAY_C);   
		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_STATIC_PLAY_D);  
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_STATIC_PLAY_B);  
		ChangeSize(pWnd, screenMax.left, screenMax.top,screenMax.right-screenMax.left,screenMax.bottom-screenMax.top);  
		//pWnd->ModifyStyle(SS_NOTIFY,0);
		ModifyStyle(0,WS_MAXIMIZEBOX|WS_MINIMIZEBOX);
	}else{
		maxScreen = 1;
		ModifyStyle(WS_MAXIMIZEBOX|WS_MINIMIZEBOX,0);

		CWnd *pWnd;   
		pWnd = GetDlgItem(IDC_STATIC_PLAY_A);   
		pWnd->ShowWindow(SW_HIDE);
		//pWnd = GetDlgItem(IDC_STATIC_PLAY_B);  
		//pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_STATIC_PLAY_C);   
		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_STATIC_PLAY_D);  
		pWnd->ShowWindow(SW_HIDE);

		CRect rt;
		GetClientRect(&rt);

		int cx = rt.right - rt.left;
		int cy = rt.bottom - rt.top;
		pWnd = GetDlgItem(IDC_STATIC_PLAY_B);  
		screenMax = ChangeSize(pWnd, cx*0.01, cy*0.01,cx*0.98,cy*0.94); 

	}
}

void CgaoqingDlg::OnStnDblclickStaticPlayC()
{
	// TODO: Add your control notification handler code here
	if(maxScreen){
		maxScreen = 0;

		CWnd *pWnd;   
		pWnd = GetDlgItem(IDC_STATIC_PLAY_A);   
		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_STATIC_PLAY_B);  
		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_STATIC_PLAY_C);   
		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_STATIC_PLAY_D);  
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_STATIC_PLAY_C);  
		ChangeSize(pWnd, screenMax.left, screenMax.top,screenMax.right-screenMax.left,screenMax.bottom-screenMax.top);  
		//pWnd->ModifyStyle(SS_NOTIFY,0);
		ModifyStyle(0,WS_MAXIMIZEBOX|WS_MINIMIZEBOX);
	}else{
		maxScreen = 1;
		ModifyStyle(WS_MAXIMIZEBOX|WS_MINIMIZEBOX,0);

		CWnd *pWnd;   
		pWnd = GetDlgItem(IDC_STATIC_PLAY_A);   
		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_STATIC_PLAY_B);  
		pWnd->ShowWindow(SW_HIDE);
		//pWnd = GetDlgItem(IDC_STATIC_PLAY_C);   
		//pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_STATIC_PLAY_D);  
		pWnd->ShowWindow(SW_HIDE);

		CRect rt;
		GetClientRect(&rt);

		int cx = rt.right - rt.left;
		int cy = rt.bottom - rt.top;
		pWnd = GetDlgItem(IDC_STATIC_PLAY_C);  
		screenMax = ChangeSize(pWnd, cx*0.01, cy*0.01,cx*0.98,cy*0.94); 

	}
}

void CgaoqingDlg::OnStnDblclickStaticPlayD()
{
	// TODO: Add your control notification handler code here
	if(maxScreen){
		maxScreen = 0;

		CWnd *pWnd;   
		pWnd = GetDlgItem(IDC_STATIC_PLAY_A);   
		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_STATIC_PLAY_B);  
		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_STATIC_PLAY_C);   
		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_STATIC_PLAY_D);  
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_STATIC_PLAY_D);  
		ChangeSize(pWnd, screenMax.left, screenMax.top,screenMax.right-screenMax.left,screenMax.bottom-screenMax.top);  
		//pWnd->ModifyStyle(SS_NOTIFY,0);
		ModifyStyle(0,WS_MAXIMIZEBOX|WS_MINIMIZEBOX);
	}else{
		maxScreen = 1;
		ModifyStyle(WS_MAXIMIZEBOX|WS_MINIMIZEBOX,0);

		CWnd *pWnd;   
		pWnd = GetDlgItem(IDC_STATIC_PLAY_A);   
		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_STATIC_PLAY_B);  
		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_STATIC_PLAY_C);   
		pWnd->ShowWindow(SW_HIDE);
		//pWnd = GetDlgItem(IDC_STATIC_PLAY_D);  
		//pWnd->ShowWindow(SW_HIDE);

		CRect rt;
		GetClientRect(&rt);

		int cx = rt.right - rt.left;
		int cy = rt.bottom - rt.top;
		pWnd = GetDlgItem(IDC_STATIC_PLAY_D);  
		screenMax = ChangeSize(pWnd, cx*0.01, cy*0.01,cx*0.98,cy*0.94); 

	}
}

void CgaoqingDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (nIDEvent==LIVE_TIMER)
	{
		ShowLivePlayBackState();
	}
	CDialog::OnTimer(nIDEvent);
}

void CgaoqingDlg::ShowLivePlayBackState()
{
	//DWORD totaltime = dlg->getVideoTime();
	DWORD len =	getVideoTime()*1000;
	if (len == 0)
	{
		return;
	}

	DWORD nCurTime=getPlayedTime();
	//post_message(_T("%d-%d\r\n"),len,nCurTime);
	//parent->playslider.SetPos(nCurTime);
	/*CString csStatus;
	csStatus.Format("%02d:%02d:%02d/%02d:%02d:%02d",\
		nCurSecond/3600,(nCurSecond%3600)/60,nCurSecond%60,\
		m_iTotalSeconds/3600,(m_iTotalSeconds%3600)/60,m_iTotalSeconds%60);
	float fCurPos = PlayM4_GetPlayPos(USED_PORT);
	g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "PlayM4_GetPlayPos %f", fCurPos);
	*/
	if (nCurTime > len-100)
	{
		stopVideo();
		CStatic *wd = (CStatic *)GetDlgItem(IDC_STATIC_PLAY_A);
		wd->SetBitmap(NULL);
		wd = (CStatic *)GetDlgItem(IDC_STATIC_PLAY_B);
		wd->SetBitmap(NULL);
		wd = (CStatic *)GetDlgItem(IDC_STATIC_PLAY_C);
		wd->SetBitmap(NULL);
		wd = (CStatic *)GetDlgItem(IDC_STATIC_PLAY_D);
		wd->SetBitmap(NULL);
		Invalidate(TRUE);
		//GetDlgItem(IDC_BUTTON_REC_PLAY)->SetWindowText("播放");
		//OnBnClickedBtnLocalStop();
		KillTimer(LIVE_TIMER);

	}


	//GetDlgItem(IDC_STATIC_LOCAL_PLAY_STATUS)->SetWindowText(csStatus);
}