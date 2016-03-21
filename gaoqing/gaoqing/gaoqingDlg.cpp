/*
File name:   gaoqingDlg.cpp
File Author: Li Weichen
Date:        2016.3.1
*/
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

char CgaoqingDlg::rx_ser[50]={};
int  CgaoqingDlg::count_ser=0;
int  CgaoqingDlg::longth_ser=0;
char CgaoqingDlg::command_ser=0;


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
	,adjust(false)
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
//  the minimized mWindow.
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
/*************************************************
Function:   initView
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
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
	rs.top +=30;
	rs.bottom -=30;

	playslider.sign = &adjust;
	playslider.SetPageSize(20);

	dLogin.MoveWindow(&rs);
	dFind.MoveWindow(&rs);
	dSetting.MoveWindow(&rs);

	dLogin.ShowWindow(true);
	dFind.ShowWindow(false);
	dSetting.ShowWindow(false);
	tabSettings.SetCurSel(0);
}
/*************************************************
Function:   OnInitDialog
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
BOOL CgaoqingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int cx;// = GetSystemMetrics(SM_CXFULLSCREEN); 
	int cy;// = GetSystemMetrics(SM_CYFULLSCREEN); 
	CRect rt;

	SystemParametersInfo(SPI_GETWORKAREA,0,&rt,0);
	cx = rt.right;
	cy = rt.bottom;
	SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE); 

	
	GetWindowRect(&rt);  
	ScreenToClient(&rt);
	cx = rt.right;
	cy = rt.bottom;

	int gap = 20;

	int wtab = 300;
	int htab = cy;

	int wsld = (cx-wtab);
	int hsld = 40;

	int wbtn = (cx-wtab-gap-gap-gap-gap)/3;
	int hbtn = 50;

	int wdis = (cx-wtab)/2;
	int hdis = (cy-gap-hsld-gap-hbtn-gap)/2;


	CRect new_rect;
	CWnd *pWnd; 
	
	//GetDlgItem(IDC_SLIDER_PLAY)->GetWindowRect(&new_rect);
	pWnd = GetDlgItem(IDC_TAB);
	new_rect.left=wdis*2;//调整控件大小  
	new_rect.right=cx;  
	new_rect.top=0;  
	new_rect.bottom=htab;  
	pWnd->MoveWindow(new_rect);//设置控件大小  


	pWnd = GetDlgItem(IDC_STATIC_PLAY_A); 
	new_rect.left=0;//调整控件大小  
	new_rect.right=wdis;  
	new_rect.top=0;  
	new_rect.bottom=hdis;  
	pWnd->MoveWindow(new_rect);//设置控件大小  


	pWnd = GetDlgItem(IDC_STATIC_PLAY_B); 
	new_rect.left=wdis;//调整控件大小  
	new_rect.right=wdis*2;  
	new_rect.top=0;  
	new_rect.bottom=hdis;  
	pWnd->MoveWindow(new_rect);//设置控件大小  


	pWnd = GetDlgItem(IDC_STATIC_PLAY_C); 
	new_rect.left=0;//调整控件大小  
	new_rect.right=wdis;  
	new_rect.top=hdis;  
	new_rect.bottom=hdis*2;  
	pWnd->MoveWindow(new_rect);//设置控件大小  

	pWnd = GetDlgItem(IDC_STATIC_PLAY_D); 
	new_rect.left=wdis;//调整控件大小  
	new_rect.right=wdis*2;  
	new_rect.top=hdis;  
	new_rect.bottom=hdis*2;  
	pWnd->MoveWindow(new_rect);//设置控件大小  

	pWnd = GetDlgItem(IDC_SLIDER_PLAY);
	new_rect.left=0;//调整控件大小  
	new_rect.right=wsld;  
	new_rect.top=new_rect.bottom+gap;  
	new_rect.bottom=new_rect.top+hsld;  
	pWnd->MoveWindow(new_rect);//设置控件大小  

	pWnd = GetDlgItem(IDC_BUTTON_REC_PLAY);
	new_rect.left=gap;//调整控件大小  
	new_rect.right=gap+wbtn;  
	new_rect.top=new_rect.bottom+gap;  
	new_rect.bottom=new_rect.top+hbtn;  
	pWnd->MoveWindow(new_rect);//设置控件大小  

	pWnd = GetDlgItem(IDC_BUTTON_REC_FAST);
	new_rect.left=new_rect.right+gap;//调整控件大小  
	new_rect.right=new_rect.left+wbtn;  
	//new_rect.top=hdis*2+gap;  
	//new_rect.bottom=hdis*2+gap+hsld;  
	pWnd->MoveWindow(new_rect);//设置控件大小  

	pWnd = GetDlgItem(IDC_BUTTON_REC_SLOW);
	new_rect.left=new_rect.right+gap;//调整控件大小  
	new_rect.right=new_rect.left+wbtn;  
	//new_rect.top=hdis*2+gap;  
	//new_rect.bottom=hdis*2+gap+hsld;  
	pWnd->MoveWindow(new_rect);//设置控件大小  

	//SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	HikController::initHik();
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
/*************************************************
Function:   setConfig
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CgaoqingDlg::setConfig(CString category,CString item,CString val)
{
	CString add;
	TCHAR dir[BUFSIZE];
	::GetCurrentDirectory(sizeof(dir),dir);
	sprintf_s(add.GetBuffer(BUFSIZE),BUFSIZE,"%s\\config.ini", dir);
	add.ReleaseBuffer();
	::WritePrivateProfileString(category,item,val,add);
}
/*************************************************
Function:   getConfig
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
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
/*************************************************
Function:   initParam
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
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
	slowtimes = atoi(getConfig("CONFIG","SlowTimes","1"));

	LONG nPort;

	mHik_a = new HikController();
	mHik_a->mWind = GetDlgItem(IDC_STATIC_PLAY_A)->m_hWnd;
	PlayM4_GetPort(&nPort);
	mHik_a->mPlay_no = nPort;

	mHik_b = new HikController();
	mHik_b->mWind = GetDlgItem(IDC_STATIC_PLAY_B)->m_hWnd;
	PlayM4_GetPort(&nPort);
	mHik_b->mPlay_no = nPort;

	mHik_c = new HikController();
	mHik_c->mWind = GetDlgItem(IDC_STATIC_PLAY_C)->m_hWnd;
	PlayM4_GetPort(&nPort);
	mHik_c->mPlay_no = nPort;

	mHik_d = new HikController();
	mHik_d->mWind = GetDlgItem(IDC_STATIC_PLAY_D)->m_hWnd;
	PlayM4_GetPort(&nPort);
	mHik_d->mPlay_no = nPort;
	TRY   
	{      
		m_db.OpenEx(_T("DSN=gaoqing;UID=sa;PWD=gaoqing;"),CDatabase::noOdbcDialog);//连接到一个名为Test的数据源   
		rs_train.m_pDatabase = &m_db;   
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

}
/*************************************************
Function:   startUART
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
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
/*************************************************
Function:   stopUART
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CgaoqingDlg::stopUART()
{
	delete ser_com;

}
/*************************************************
Function:   ser_com_read
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CgaoqingDlg::ser_com_read(char rd)
{
	if((rd == 0x02)||(rx_ser[0]==0x02))
	{
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
/*************************************************
Function:   Command_Disposal_Ser
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
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
		playLive(false);

		stime = timeGetTime();

		tabSettings.SetCurSel(0);

		dLogin.ShowWindow(true);
		dFind.ShowWindow(false);
		dSetting.ShowWindow(false);

		GetDlgItem(IDC_SLIDER_PLAY)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_REC_PLAY)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_REC_FAST)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_REC_SLOW)->EnableWindow(false);

		if (stopVideo(true))
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
				CString sql;
				sql.Format( _T("INSERT INTO tbl_train (tran_name) VALUES (\'%s\')"),timename);
					
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

			}

			if(camer_a && mHik_a->savefileHik(foldername+"\\"+timename+"lt.mp4")==SUCCESS){


			}	
			if(camer_b && mHik_b->savefileHik(foldername+"\\"+timename+"rt.mp4")==SUCCESS){


			}
			if(camer_c && mHik_c->savefileHik(foldername+"\\"+timename+"lb.mp4")==SUCCESS){


			}
			if(camer_d && mHik_d->savefileHik(foldername+"\\"+timename+"rb.mp4")==SUCCESS){


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

			post_message(_T("%d---%s---%s\r\n"),car_speed,CString(car_number,7),CString(car_number+7,13));
			if(car_n == 1) 
			{
				str.Format("%d\r\n%d\t%d\t%s\t%d\tJ\r\n",direction,car_n,car_speed,CString(car_number,20),pos);

			}
			else 
			{
				if(car_n == 5) {
					int re=0;

					if(camer_a && mHik_a->playbackHik(foldername+"\\"+timename+"lt.mp4")==SUCCESS){

						if(camer_a && mHik_a->slowHik(slowtimes)==SUCCESS)
							re=1;
					}
					if(camer_b && mHik_b->playbackHik(foldername+"\\"+timename+"rt.mp4")==SUCCESS){

						if(camer_b && mHik_b->slowHik(slowtimes)==SUCCESS)
							re=1;
					}
					if(camer_c && mHik_c->playbackHik(foldername+"\\"+timename+"lb.mp4")==SUCCESS){

						if(camer_c && mHik_c->slowHik(slowtimes)==SUCCESS)
							re=1;
					}
					if(camer_d && mHik_d->playbackHik(foldername+"\\"+timename+"rb.mp4")==SUCCESS){

						if(camer_d && mHik_d->slowHik(slowtimes)==SUCCESS)
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
			mHik_a->stopsavefileHik();
			mHik_a->stopplayliveHik();
		}
		if(camer_b){
			mHik_b->stopsavefileHik();
			mHik_b->stopplayliveHik();
		}
		if(camer_c){
			mHik_c->stopsavefileHik();
			mHik_c->stopplayliveHik();
		}
		if(camer_d){
			mHik_d->stopsavefileHik();
			mHik_d->stopplayliveHik();
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
/*************************************************
Function:   getSize
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
CRect CgaoqingDlg::getSize(CWnd *pWnd)
{
	CRect rect;
	pWnd->GetWindowRect(&rect);  
	ScreenToClient(&rect);
	return rect;
}
/*************************************************
Function:   saveSize
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
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
/*************************************************
Function:   restoreSize
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
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
/*************************************************
Function:   ChangeSize
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
CRect CgaoqingDlg::ChangeSize(CWnd *pWnd, int left, int top, int width, int height)  
{  
	if(pWnd)  //判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建   
	{  
		CRect old_rect,new_rect;   //获取控件变化前的大小  
		old_rect = getSize(pWnd);
		new_rect.left=left;//调整控件大小  
		new_rect.right=left+width;  
		new_rect.top=top;  
		new_rect.bottom=top+height;  
		pWnd->MoveWindow(new_rect);//设置控件大小  
		return old_rect;
	}  
	return NULL;
} 
/*************************************************
Function:   getVideoTime
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/

DWORD CgaoqingDlg::getVideoTime(bool isRec)
{
	DWORD len=0;
	if(isRec?mHik_a->rec:camer_a)
		len = mHik_a->playtimeHik();
	if(len ==0){
		if(isRec?mHik_b->rec:camer_b){
			len = mHik_a->playtimeHik();
		}
	}else{
		if(isRec?mHik_b->rec:camer_b){
			len =min(len,mHik_b->playtimeHik());
		}
	}
		

	if(len ==0){
		if(isRec?mHik_c->rec:camer_c){
			len = mHik_c->playtimeHik();
		}
	}else{
		if(isRec?mHik_c->rec:camer_c){
			len =min(len,mHik_c->playtimeHik());
		}
	}

	if(len ==0){
		if(isRec?mHik_d->rec:camer_d){
			len = mHik_d->playtimeHik();
		}
	}else{
		if(isRec?mHik_d->rec:camer_d){
			len =min(len,mHik_d->playtimeHik());
		}
	}
	if(len ==0)
		len = -1;


	return len;
}
/*************************************************
Function:   getPlayedTime
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
DWORD CgaoqingDlg::getPlayedTime(bool isRec)
{
	DWORD len=0;
	if(isRec?mHik_a->rec:camer_a)
		return mHik_a->playedtimeHik();
	if(isRec?mHik_b->rec:camer_b)
		return mHik_a->playedtimeHik();
	if(isRec?mHik_c->rec:camer_c)
		return mHik_c->playedtimeHik();
	if(isRec?mHik_d->rec:camer_d)
		return mHik_d->playedtimeHik();
	return 0;
}
/*************************************************
Function:   playVideoPos
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
int CgaoqingDlg::playVideoPos(DWORD pos)
{
	if(
		(mHik_a->rec ?mHik_a->playposHik(pos)==SUCCESS:true) &
		(mHik_b->rec ?mHik_b->playposHik(pos)==SUCCESS:true) &
		(mHik_c->rec ?mHik_c->playposHik(pos)==SUCCESS:true) &
		(mHik_d->rec ?mHik_d->playposHik(pos)==SUCCESS:true)){
		return SUCCESS;
	}
	return FAIL;
}
/*************************************************
Function:   playLive
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
int CgaoqingDlg::playLive(bool display)
{
	if(
		(camer_a ?mHik_a->playliveHik(display)==SUCCESS:true) &
		(camer_b ?mHik_b->playliveHik(display)==SUCCESS:true) &
		(camer_c ?mHik_c->playliveHik(display)==SUCCESS:true) &
		(camer_d ?mHik_d->playliveHik(display)==SUCCESS:true)){

			if(!display){
				CStatic *wd = (CStatic *)GetDlgItem(IDC_STATIC_PLAY_A);
				wd->SetBitmap(NULL);
				wd = (CStatic *)GetDlgItem(IDC_STATIC_PLAY_B);
				wd->SetBitmap(NULL);
				wd = (CStatic *)GetDlgItem(IDC_STATIC_PLAY_C);
				wd->SetBitmap(NULL);
				wd = (CStatic *)GetDlgItem(IDC_STATIC_PLAY_D);
				wd->SetBitmap(NULL);
				Invalidate(TRUE);
			}



		return SUCCESS;
	}
	return FAIL;
}
/*************************************************
Function:   stopVideo
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
int CgaoqingDlg::stopVideo(bool isRec)
{
	if(
		(isRec?mHik_a->rec:camer_a ?mHik_a->stopplaybackHik()==SUCCESS:true) &
		(isRec?mHik_b->rec:camer_b ?mHik_b->stopplaybackHik()==SUCCESS:true) &
		(isRec?mHik_c->rec:camer_c ?mHik_c->stopplaybackHik()==SUCCESS:true) &
		(isRec?mHik_d->rec:camer_d ?mHik_d->stopplaybackHik()==SUCCESS:true)){
		return SUCCESS;
	}
	return FAIL;
}
/*************************************************
Function:   resumeVideo
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
int CgaoqingDlg::resumeVideo()
{
	if(
		(mHik_a->rec ?mHik_a->resumeHik()==SUCCESS:true) &
		(mHik_b->rec ?mHik_b->resumeHik()==SUCCESS:true) &
		(mHik_c->rec ?mHik_c->resumeHik()==SUCCESS:true) &
		(mHik_d->rec ?mHik_d->resumeHik()==SUCCESS:true)){
		return SUCCESS;
	}
	
	
	return FAIL;
}
/*************************************************
Function:   pauseVideo
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
int CgaoqingDlg::pauseVideo()
{
	if(
		(mHik_a->rec ?mHik_a->pauseHik()==SUCCESS:true) &
		(mHik_b->rec ?mHik_b->pauseHik()==SUCCESS:true) &
		(mHik_c->rec ?mHik_c->pauseHik()==SUCCESS:true) &
		(mHik_d->rec ?mHik_d->pauseHik()==SUCCESS:true)){
		return SUCCESS;
	}
	
	
	return FAIL;
}
/*************************************************
Function:   fastVideo
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
int CgaoqingDlg::fastVideo()
{
	if(
		(mHik_a->rec ?mHik_a->fastHik()==SUCCESS:true) &
		(mHik_b->rec ?mHik_b->fastHik()==SUCCESS:true) &
		(mHik_c->rec ?mHik_c->fastHik()==SUCCESS:true) &
		(mHik_d->rec ?mHik_d->fastHik()==SUCCESS:true)){
		return SUCCESS;
	}
	
	if(	mHik_a->rec)  mHik_a->resumeHik();
	if(	mHik_b->rec)  mHik_b->resumeHik();
	if(	mHik_c->rec)  mHik_c->resumeHik();
	if(	mHik_d->rec)  mHik_d->resumeHik();
	
	return FAIL;
}
/*************************************************
Function:   slowVideo
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
int CgaoqingDlg::slowVideo()
{
	if(
		(mHik_a->rec ?mHik_a->slowHik(1)==SUCCESS:true) &
		(mHik_b->rec ?mHik_b->slowHik(1)==SUCCESS:true) &
		(mHik_c->rec ?mHik_c->slowHik(1)==SUCCESS:true) &
		(mHik_d->rec ?mHik_d->slowHik(1)==SUCCESS:true)){
		return SUCCESS;
	}
	
	if(	mHik_a->rec)  mHik_a->resumeHik();
	if(	mHik_b->rec)  mHik_b->resumeHik();
	if(	mHik_c->rec)  mHik_c->resumeHik();
	if(	mHik_d->rec)  mHik_d->resumeHik();
	
	return FAIL;
}
/*************************************************
Function:   saveParams
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
int CgaoqingDlg::saveParams(BYTE bright,BYTE contrast,BYTE sharpness,BYTE saturation,int exp_mode,int exp_time,int daynight)
{
	if(
		(camer_a ?mHik_a->saveParamHik(bright,contrast,sharpness,saturation,exp_mode,exp_time,daynight)==SUCCESS:true) &
		(camer_b ?mHik_b->saveParamHik(bright,contrast,sharpness,saturation,exp_mode,exp_time,daynight)==SUCCESS:true) &
		(camer_c ?mHik_c->saveParamHik(bright,contrast,sharpness,saturation,exp_mode,exp_time,daynight)==SUCCESS:true) &
		(camer_d ?mHik_d->saveParamHik(bright,contrast,sharpness,saturation,exp_mode,exp_time,daynight)==SUCCESS:true)){
		return SUCCESS;
	}else
		return FAIL;
}
/*************************************************
Function:   getParams
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
PARAM_STRU CgaoqingDlg::getParams()
{
	if(camer_a){
		return mHik_a->getParamHik();
	}else if(camer_b){
		return mHik_b->getParamHik();
	}else if(camer_c){
		return mHik_c->getParamHik();
	}else if(camer_d){
		return mHik_d->getParamHik();
	}else {
		PARAM_STRU str;
		return str;
	}
		//return (PARAM_STRU)null;
}
/*************************************************
Function:   playVideo
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
int CgaoqingDlg::playVideo(CString patha,CString pathb,CString pathc,CString pathd)
{
	int result=FAIL;
	if(!patha.IsEmpty()){

		mHik_a->rec = true;
		if(mHik_a->playbackHik(patha)==SUCCESS){
			result = SUCCESS;
		}else
			result = FAIL;
	}else{
		mHik_a->rec = false;
	}

	if(!pathb.IsEmpty()){ 
		mHik_b->rec = true;
		if(mHik_b->playbackHik(pathb)==SUCCESS){
			result = SUCCESS;
		}else
			result = FAIL;
	}else{
		mHik_b->rec = false;
	}

	if(!pathc.IsEmpty()){
		mHik_c->rec = true;
		if(mHik_c->playbackHik(pathc)==SUCCESS){

			result = SUCCESS;
		}else
			result = FAIL;
	}else{
		mHik_c->rec = false;
	}

	if(!pathd.IsEmpty()){ 
		mHik_d->rec = true;
		if(mHik_d->playbackHik(pathd)==SUCCESS){
			result = SUCCESS;
		}else
			result = FAIL;
	}else{
		mHik_d->rec = false;
	}

	return result;
}
/*************************************************
Function:   post_message
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
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
/*************************************************
Function:   OnTcnSelchangeTab
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
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

		if (stopVideo(true))
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

		dSetting.SetFocus();

		GetDlgItem(IDC_SLIDER_PLAY)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_REC_PLAY)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_REC_FAST)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_REC_SLOW)->EnableWindow(false);

		if (stopVideo(true))
		{
			
		}

		break; 

	default: break; 

	} 

	*pResult = 0;
}
/*************************************************
Function:   getCStringFromConfig
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
CString CgaoqingDlg::getCStringFromConfig(CString cam,CString sub)
{
	return _T(getConfig(cam,sub,""));
}
/*************************************************
Function:   loginCamera
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
bool CgaoqingDlg::loginCamera()
{
	//UpdateData(TRUE);
	//prepare for ip
	//CString DeviceIp;
	//BYTE nField0,nField1,nField2,nField3; 
	bool re=true;
	
	if(camer_a){
		
		mHik_a->mIp = getCStringFromConfig("CAMA","Ip");
		mHik_a->mPort =atoi(getCStringFromConfig("CAMA","Port"));
		mHik_a->mName = getCStringFromConfig("CAMA","User");
		mHik_a->mPwd = getCStringFromConfig("CAMA","Password");


		if(mHik_a->loginHik()!=SUCCESS){
			re=false;
		}
	}
	if(camer_b && re){

		mHik_b->mIp = getCStringFromConfig("CAMB","Ip");
		mHik_b->mPort =atoi(getCStringFromConfig("CAMB","Port"));
		mHik_b->mName = getCStringFromConfig("CAMB","User");
		mHik_b->mPwd = getCStringFromConfig("CAMB","Password");

		if(mHik_b->loginHik()!=SUCCESS){
			re=false;
		}
	}
	if(camer_c && re){

		mHik_c->mIp = getCStringFromConfig("CAMC","Ip");
		mHik_c->mPort =atoi(getCStringFromConfig("CAMC","Port"));
		mHik_c->mName = getCStringFromConfig("CAMC","User");
		mHik_c->mPwd = getCStringFromConfig("CAMC","Password");

		if(mHik_c->loginHik()!=SUCCESS){
			re=false;
		}
	}
	if(camer_d && re){


		mHik_d->mIp = getCStringFromConfig("CAMD","Ip");
		mHik_d->mPort =atoi(getCStringFromConfig("CAMD","Port"));
		mHik_d->mName = getCStringFromConfig("CAMD","User");
		mHik_d->mPwd = getCStringFromConfig("CAMD","Password");

		if(mHik_d->loginHik()!=SUCCESS){
			re=false;
		}
	}
	if(!re)
	{
		logoutCamera();
	}else{
		playLive(true);
		
	}
	return re;
}
/*************************************************
Function:   OnBnClickedButtonRecPlay
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CgaoqingDlg::OnBnClickedButtonRecPlay()
{
	if (s_Play)
	{
		if (pauseVideo()==SUCCESS)
		{
			s_Play=FALSE;
			GetDlgItem(IDC_BUTTON_REC_PLAY)->SetWindowText("播放");
			GetDlgItem(IDC_BUTTON_REC_FAST)->EnableWindow(false);
			GetDlgItem(IDC_BUTTON_REC_SLOW)->EnableWindow(false);
		}
	}
	else
	{
		if (resumeVideo()==SUCCESS)
		{
			s_Play=TRUE;
			GetDlgItem(IDC_BUTTON_REC_PLAY)->SetWindowText("暂停");
			GetDlgItem(IDC_BUTTON_REC_FAST)->EnableWindow(true);
			GetDlgItem(IDC_BUTTON_REC_SLOW)->EnableWindow(true);
		}
		
	}
}
/*************************************************
Function:   OnBnClickedButtonRecFast
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CgaoqingDlg::OnBnClickedButtonRecFast()
{
	if(playSpeed <4){
	{
		if (fastVideo()==SUCCESS)
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
/*************************************************
Function:   OnBnClickedButtonRecSlow
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CgaoqingDlg::OnBnClickedButtonRecSlow()
{
	if(playSpeed >-4){
	{
		if (slowVideo()==SUCCESS)
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
/*************************************************
Function:   OnNMReleasedcaptureSliderPlay
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CgaoqingDlg::OnNMReleasedcaptureSliderPlay(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nPos = 0;
	nPos = playslider.GetPos();
	//m_sliderPlayProgress.SetPos(nPos);
	if(playVideoPos(nPos)==SUCCESS)
	{
		*pResult = 0;
	}
	adjust = false;
	*pResult = 0;
}
/*************************************************
Function:   logoutCamera
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CgaoqingDlg::logoutCamera()
{
	if(camer_a && mHik_a->stopsavefileHik()==SUCCESS){
			
		if(mHik_a->stopplayliveHik()==SUCCESS){
			if(mHik_a->logoutHik()==SUCCESS){
					
			}
				

		}
			
	}

	if(camer_b && mHik_b->stopsavefileHik()==SUCCESS){
			
		if(mHik_b->stopplayliveHik()==SUCCESS){
			if(mHik_b->logoutHik()==SUCCESS){
					
			}
				

		}
			
	}

	if(camer_c && mHik_c->stopsavefileHik()==SUCCESS){
			
		if(mHik_c->stopplayliveHik()==SUCCESS){
			if(mHik_c->logoutHik()==SUCCESS){
					
			}
				

		}
			
	}

	if(camer_d && mHik_d->stopsavefileHik()==SUCCESS){
			
		if(mHik_d->stopplayliveHik()==SUCCESS){
			if(mHik_d->logoutHik()==SUCCESS){
					
			}
				

		}
			
	}
}
/*************************************************
Function:   OnSysCommand
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
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
/*************************************************
Function:   OnSize
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CgaoqingDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
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
/*************************************************
Function:   OnStnDblclickStaticPlayA
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
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
/*************************************************
Function:   OnStnDblclickStaticPlayB
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
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
/*************************************************
Function:   OnStnDblclickStaticPlayC
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
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
/*************************************************
Function:   OnStnDblclickStaticPlayD
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
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

		CRect rt;
		GetClientRect(&rt);

		int cx = rt.right - rt.left;
		int cy = rt.bottom - rt.top;
		pWnd = GetDlgItem(IDC_STATIC_PLAY_D);  
		screenMax = ChangeSize(pWnd, cx*0.01, cy*0.01,cx*0.98,cy*0.94); 

	}
}
/*************************************************
Function:   OnTimer
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CgaoqingDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (nIDEvent==LIVE_TIMER)
	{
		ShowLivePlayBackState();
	}
	CDialog::OnTimer(nIDEvent);
}
/*************************************************
Function:   ShowLivePlayBackState
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CgaoqingDlg::ShowLivePlayBackState()
{
	DWORD len =	getVideoTime(false)*1000;
	if (len == 0)
	{
		return;
	}

	DWORD nCurTime=getPlayedTime(false);
	if (nCurTime > len-100)
	{
		stopVideo(false);
		CStatic *wd = (CStatic *)GetDlgItem(IDC_STATIC_PLAY_A);
		wd->SetBitmap(NULL);
		wd = (CStatic *)GetDlgItem(IDC_STATIC_PLAY_B);
		wd->SetBitmap(NULL);
		wd = (CStatic *)GetDlgItem(IDC_STATIC_PLAY_C);
		wd->SetBitmap(NULL);
		wd = (CStatic *)GetDlgItem(IDC_STATIC_PLAY_D);
		wd->SetBitmap(NULL);
		Invalidate(TRUE);

		playLive(true);
		KillTimer(LIVE_TIMER);

	}


	//GetDlgItem(IDC_STATIC_LOCAL_PLAY_STATUS)->SetWindowText(csStatus);
}
/*************************************************
Function:   OnOK
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CgaoqingDlg::OnOK(){

	return;
}