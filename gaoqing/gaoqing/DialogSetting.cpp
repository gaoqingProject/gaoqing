/*
File name:   DialogSetting.cpp
File Author: Li Weichen
Date:        2016.3.1
*/
#include "stdafx.h"
#include "gaoqing.h"
#include "DialogSetting.h"
#include "gaoqingDlg.h"
// CDialogSetting dialog

IMPLEMENT_DYNAMIC(CDialogSetting, CDialog)

CDialogSetting::CDialogSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSetting::IDD, pParent)
{


}
CDialogSetting::~CDialogSetting()
{
}

void CDialogSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BRIGHTNESS, c_Brightness);
	DDX_Control(pDX, IDC_CONTRAST, c_Contrast);
	DDX_Control(pDX, IDC_SHARPNESS, c_Sharpness);
	DDX_Control(pDX, IDC_SATURATION, c_Saturation);
	DDX_Control(pDX, IDC_EXP_MODE, c_ExpMode);
	DDX_Control(pDX, IDC_EXP_TIME, c_ExpTime);
	DDX_Control(pDX, IDC_DAYNIGHT, c_DayNight);

	DDX_Control(pDX, IDC_IPADDRESS_A, ip_a);
	DDX_Text(pDX, IDC_EDIT_PORT_A, port_a);
	DDX_Text(pDX, IDC_EDIT_PWD_A, pwd_a);
	DDX_Text(pDX, IDC_EDIT_USER_A, user_a);

	DDX_Control(pDX, IDC_IPADDRESS_B, ip_b);
	DDX_Text(pDX, IDC_EDIT_PORT_B, port_b);
	DDX_Text(pDX, IDC_EDIT_PWD_B, pwd_b);
	DDX_Text(pDX, IDC_EDIT_USER_B, user_b);

	DDX_Control(pDX, IDC_IPADDRESS_C, ip_c);
	DDX_Text(pDX, IDC_EDIT_PORT_C, port_c);
	DDX_Text(pDX, IDC_EDIT_PWD_C, pwd_c);
	DDX_Text(pDX, IDC_EDIT_USER_C, user_c);

	DDX_Control(pDX, IDC_IPADDRESS_D, ip_d);
	DDX_Text(pDX, IDC_EDIT_PORT_D, port_d);
	DDX_Text(pDX, IDC_EDIT_PWD_D, pwd_d);
	DDX_Text(pDX, IDC_EDIT_USER_D, user_d);

	DDX_Control(pDX, IDC_CHECK_A, isAExist);
	DDX_Control(pDX, IDC_CHECK_B, isBExist);
	DDX_Control(pDX, IDC_CHECK_C, isCExist);
	DDX_Control(pDX, IDC_CHECK_D, isDExist);
	DDX_Control(pDX, IDC_BUTTON_LOGIN, loginSet);
	DDX_Control(pDX, IDC_EDIT2, passwordInput);
}


BEGIN_MESSAGE_MAP(CDialogSetting, CDialog)




	ON_BN_CLICKED(IDC_CHECK_A, &CDialogSetting::OnCameraACheck)
	ON_BN_CLICKED(IDC_CHECK_B, &CDialogSetting::OnCameraBCheck)
	ON_BN_CLICKED(IDC_CHECK_C, &CDialogSetting::OnCameraCCheck)
	ON_BN_CLICKED(IDC_CHECK_D, &CDialogSetting::OnCameraDCheck)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_A, &CDialogSetting::OnIpnFieldchangedIpaddressA)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_C, &CDialogSetting::OnIpnFieldchangedIpaddressC)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_B, &CDialogSetting::OnIpnFieldchangedIpaddressB)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_D, &CDialogSetting::OnIpnFieldchangedIpaddressD)
	ON_EN_CHANGE(IDC_EDIT_PORT_A, &CDialogSetting::OnEnChangeEditPortA)
	ON_EN_CHANGE(IDC_EDIT_PORT_B, &CDialogSetting::OnEnChangeEditPortB)
	ON_EN_CHANGE(IDC_EDIT_PORT_C, &CDialogSetting::OnEnChangeEditPortC)
	ON_EN_CHANGE(IDC_EDIT_PORT_D, &CDialogSetting::OnEnChangeEditPortD)
	ON_EN_CHANGE(IDC_EDIT_USER_A, &CDialogSetting::OnEnChangeEditUserA)
	ON_EN_CHANGE(IDC_EDIT_PWD_A, &CDialogSetting::OnEnChangeEditPwdA)
	ON_EN_CHANGE(IDC_EDIT_USER_C, &CDialogSetting::OnEnChangeEditUserC)
	ON_EN_CHANGE(IDC_EDIT_PWD_C, &CDialogSetting::OnEnChangeEditPwdC)
	ON_EN_CHANGE(IDC_EDIT_USER_B, &CDialogSetting::OnEnChangeEditUserB)
	ON_EN_CHANGE(IDC_EDIT_PWD_B, &CDialogSetting::OnEnChangeEditPwdB)
	ON_EN_CHANGE(IDC_EDIT_USER_D, &CDialogSetting::OnEnChangeEditUserD)
	ON_EN_CHANGE(IDC_EDIT_PWD_D, &CDialogSetting::OnEnChangeEditPwdD)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CDialogSetting::OnBnClickedButtonLogin)
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()
/*************************************************
Function:   getCStringFromConfig
Desc:	    获得配置文件中的配置信息
Input:      CString cam 主键 CString sub 子键
Return:		CString 被配置的字符串
**************************************************/
CString CDialogSetting::getCStringFromConfig(CString cam,CString sub)
{
	CgaoqingDlg *dlg = (CgaoqingDlg *)GetParent()->GetParent();
	CString str = _T(dlg->getConfig(cam,sub,""));

	return str;
}
/*************************************************
Function:   setCStringToConfig
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::setCStringToConfig(CString cam,CString sub,CString str)
{
	CgaoqingDlg *dlg = (CgaoqingDlg *)GetParent()->GetParent();
	dlg->setConfig(cam,sub,str);

}
/*************************************************
Function:   saveParams
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::saveParams(BYTE bright,BYTE contrast,BYTE sharpness,BYTE saturation,int exp_mode,int exp_time,int daynight)
{
	CgaoqingDlg *dlg = (CgaoqingDlg *)GetParent()->GetParent();
	dlg->saveParams(bright,contrast,sharpness,saturation,exp_mode,exp_time,daynight);

}
/*************************************************
Function:   getParams
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
PARAM_STRU CDialogSetting::getParams()
{
	CgaoqingDlg *dlg = (CgaoqingDlg *)GetParent()->GetParent();
	return dlg->getParams();

}
/*************************************************
Function:   OnInitDialog
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
BOOL CDialogSetting::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString tempa,tempb;
	int i;
	
	PARAM_STRU pm = getParams();

	tempa.Format("%d",pm.brightness);
	//str = getCStringFromConfig("CAMS","Brightness");
	c_Brightness.SetWindowTextA(tempa);
	for(i=1;i<=100;i++){
		tempb.Format("%d",i);
		c_Brightness.AddString(tempb);
		if(tempa == tempb){
			c_Brightness.SetCurSel(i);
		}
		tempb="";
	}
	setCStringToConfig("CAMS","Brightness",tempa);
	tempa="";


	tempa.Format("%d",pm.contrast);
	//str = getCStringFromConfig("CAMS","Contrast");
	c_Contrast.SetWindowTextA(tempa);
	for(i=1;i<=100;i++){
		tempb.Format("%d",i);
		c_Contrast.AddString(tempb);
		if(tempa == tempb){
			c_Contrast.SetCurSel(i);
		}
		tempb="";
	}
	setCStringToConfig("CAMS","Contrast",tempa);
	tempa="";

	tempa.Format("%d",pm.sharpness);
	//str = getCStringFromConfig("CAMS","Sharpness");
	c_Sharpness.SetWindowTextA(tempa);
	for(i=1;i<=100;i++){
		tempb.Format("%d",i);
		c_Sharpness.AddString(tempb);
		if(tempa == tempb){
			c_Sharpness.SetCurSel(i);
		}
		tempb="";
	}
	setCStringToConfig("CAMS","Sharpness",tempa);
	tempa="";

	tempa.Format("%d",pm.saturation);
	//str = getCStringFromConfig("CAMS","Saturation");
	c_Saturation.SetWindowTextA(tempa);
	for(i=1;i<=100;i++){
		tempb.Format("%d",i);
		c_Saturation.AddString(tempb);
		if(tempa == tempb){
			c_Saturation.SetCurSel(i);
		}
		tempb="";
	}
	setCStringToConfig("CAMS","Saturation",tempa);
	tempa="";

	//str.Format("%d",pm.exposuremode);

	if(pm.exposuremode ==0){
		tempa = "手动曝光";
	}else{
		tempa = "自动曝光";
	}

	//str = getCStringFromConfig("CAMS","ExposureMode");
	c_ExpMode.SetWindowTextA(tempa);

	c_ExpMode.AddString(_T("自动曝光"));
	c_ExpMode.AddString(_T("手动曝光"));

	if(tempa == _T("自动曝光")){
			c_ExpMode.SetCurSel(0);
	}else if(tempa == _T("手动曝光")){
			c_ExpMode.SetCurSel(1);
	}

	setCStringToConfig("CAMS","ExposureMode",tempa);
	tempa="";

	tempa.Format("%d",pm.exposuretime);
	//str = getCStringFromConfig("CAMS","ExposureTime");
	c_ExpTime.SetWindowTextA(tempa);
	for(i=1;i<=100;i++){
		tempb.Format("%d",i*100);
		c_ExpTime.AddString(tempb);
		if(tempa == tempb){
			c_ExpTime.SetCurSel(i);
		}
		tempb="";
	}
	setCStringToConfig("CAMS","ExposureTime",tempa);
	tempa="";

	
	if(pm.daynight ==0){
		tempa = "白天";
	}else if(pm.daynight ==1){
		tempa = "夜晚";
	}else if(pm.daynight ==2){
		tempa = "自动";
	}else if(pm.daynight ==3){
		tempa = "定时";
	}else if(pm.daynight ==4){
		tempa = "报警输入触发";
	}

	//str.Format("%d",pm.daynight);
	//str = getCStringFromConfig("CAMS","DayNight");
	c_DayNight.SetWindowTextA(tempa);

	c_DayNight.AddString(_T("白天"));
	c_DayNight.AddString(_T("夜晚"));
	c_DayNight.AddString(_T("自动"));
	c_DayNight.AddString(_T("定时"));
	c_DayNight.AddString(_T("报警输入触发"));

	if(tempa == _T("白天")){
			c_DayNight.SetCurSel(0);
	}else if(tempa == _T("夜晚")){
			c_DayNight.SetCurSel(1);
	}else if(tempa == _T("自动")){
			c_DayNight.SetCurSel(2);
	}else if(tempa == _T("定时")){
			c_DayNight.SetCurSel(3);
	}else if(tempa == _T("报警输入触发")){
			c_DayNight.SetCurSel(4);
	}


	setCStringToConfig("CAMS","DayNight",tempa);
	tempa="";


	initParam();
	return TRUE;  // return TRUE  unless you set the focus to a control

}
/*************************************************
Function:   getIp
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
CString CDialogSetting::getIp()
{
	UpdateData(TRUE);
	//prepare for ip
	CString DeviceIp;
	BYTE nField0,nField1,nField2,nField3; 
	ip_a.GetAddress(nField0,nField1,nField2,nField3);
	DeviceIp.Format("%d.%d.%d.%d",nField0,nField1,nField2,nField3);
	return DeviceIp;
	
}
/*************************************************
Function:   SplitCString
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
vector<CString> CDialogSetting::SplitCString(CString strSource, CString ch)
{
	vector<CString> vecString;
	int iPos = 0;
	CString strTmp;
	strSource.ReleaseBuffer();
	strTmp = strSource.Tokenize(ch,iPos);
	while(strTmp.Trim() != _T(""))
	{
		vecString.push_back(strTmp);
		strTmp = strSource.Tokenize(ch,iPos);
	}
	return vecString;
}
/*************************************************
Function:   OnOK
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::OnOK(){

	return;
}
/*************************************************
Function:   transCStringToInt
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
int CDialogSetting::transCStringToInt(CString addstr)
{
	DWORD add;
	vector<CString> ips = SplitCString(addstr,".");
	add = atoi(ips[0])*256*256*256+atoi(ips[1])*256*256+atoi(ips[2])*256+atoi(ips[3]);

	return add;
}

/*************************************************
Function:   OnCameraACheck
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::OnCameraACheck()
{
	parent->camer_a = isAExist.GetCheck();
	CString re;
	re.Format("%d", parent->camer_a);
	CgaoqingDlg::setConfig("CAMA","Enable",re);
	if(parent->camer_a == true){
		ip_a.EnableWindow(true);
		GetDlgItem(IDC_EDIT_PORT_A)-> EnableWindow(true);
		GetDlgItem(IDC_EDIT_USER_A)-> EnableWindow(true);
		GetDlgItem(IDC_EDIT_PWD_A)-> EnableWindow(true);
	}else{
		ip_a.EnableWindow(false);
		GetDlgItem(IDC_EDIT_PORT_A)-> EnableWindow(false);
		GetDlgItem(IDC_EDIT_USER_A)-> EnableWindow(false);
		GetDlgItem(IDC_EDIT_PWD_A)-> EnableWindow(false);
	}

}
/*************************************************
Function:   OnCameraBCheck
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::OnCameraBCheck()
{
	parent->camer_b = isBExist.GetCheck();
	CString re;
	re.Format("%d", parent->camer_b);
	CgaoqingDlg::setConfig("CAMB","Enable",re);
	if(parent->camer_b == true){
		ip_b.EnableWindow(true);
		GetDlgItem(IDC_EDIT_PORT_B)-> EnableWindow(true);
		GetDlgItem(IDC_EDIT_USER_B)-> EnableWindow(true);
		GetDlgItem(IDC_EDIT_PWD_B)-> EnableWindow(true);
	}else{
		ip_b.EnableWindow(false);
		GetDlgItem(IDC_EDIT_PORT_B)-> EnableWindow(false);
		GetDlgItem(IDC_EDIT_USER_B)-> EnableWindow(false);
		GetDlgItem(IDC_EDIT_PWD_B)-> EnableWindow(false);
	}
}
/*************************************************
Function:   OnCameraCCheck
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::OnCameraCCheck()
{
	parent->camer_c = isCExist.GetCheck();
	CString re;
	re.Format("%d", parent->camer_c);
	CgaoqingDlg::setConfig("CAMC","Enable",re);
	if(parent->camer_c == true){
		ip_c.EnableWindow(true);
		GetDlgItem(IDC_EDIT_PORT_C)-> EnableWindow(true);
		GetDlgItem(IDC_EDIT_USER_C)-> EnableWindow(true);
		GetDlgItem(IDC_EDIT_PWD_C)-> EnableWindow(true);
	}else{
		ip_c.EnableWindow(false);
		GetDlgItem(IDC_EDIT_PORT_C)-> EnableWindow(false);
		GetDlgItem(IDC_EDIT_USER_C)-> EnableWindow(false);
		GetDlgItem(IDC_EDIT_PWD_C)-> EnableWindow(false);
	}
}
/*************************************************
Function:   OnCameraDCheck
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::OnCameraDCheck()
{
	parent->camer_d = isDExist.GetCheck();
	CString re;
	re.Format("%d", parent->camer_d);
	CgaoqingDlg::setConfig("CAMD","Enable",re);
	if(parent->camer_d == true){

		ip_d.EnableWindow(true);
		GetDlgItem(IDC_EDIT_PORT_D)-> EnableWindow(true);
		GetDlgItem(IDC_EDIT_USER_D)-> EnableWindow(true);
		GetDlgItem(IDC_EDIT_PWD_D)-> EnableWindow(true);
	}else{
		ip_d.EnableWindow(false);
		GetDlgItem(IDC_EDIT_PORT_D)-> EnableWindow(false);
		GetDlgItem(IDC_EDIT_USER_D)-> EnableWindow(false);
		GetDlgItem(IDC_EDIT_PWD_D)-> EnableWindow(false);
	}
}
/*************************************************
Function:   OnIpnFieldchangedIpaddressA
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::OnIpnFieldchangedIpaddressA(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;


	CString DeviceIp;
	BYTE nField0,nField1,nField2,nField3; 
	ip_a.GetAddress(nField0,nField1,nField2,nField3);
	DeviceIp.Format("%d.%d.%d.%d",nField0,nField1,nField2,nField3);

	setCStringToConfig("CAMA","Ip",DeviceIp);
}
/*************************************************
Function:   OnIpnFieldchangedIpaddressC
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::OnIpnFieldchangedIpaddressC(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CString DeviceIp;
	BYTE nField0,nField1,nField2,nField3; 
	ip_c.GetAddress(nField0,nField1,nField2,nField3);
	DeviceIp.Format("%d.%d.%d.%d",nField0,nField1,nField2,nField3);

	setCStringToConfig("CAMC","Ip",DeviceIp);
}
/*************************************************
Function:   OnIpnFieldchangedIpaddressB
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::OnIpnFieldchangedIpaddressB(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CString DeviceIp;
	BYTE nField0,nField1,nField2,nField3; 
	ip_b.GetAddress(nField0,nField1,nField2,nField3);
	DeviceIp.Format("%d.%d.%d.%d",nField0,nField1,nField2,nField3);

	setCStringToConfig("CAMB","Ip",DeviceIp);
}
/*************************************************
Function:   OnIpnFieldchangedIpaddressD
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::OnIpnFieldchangedIpaddressD(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CString DeviceIp;
	BYTE nField0,nField1,nField2,nField3; 
	ip_d.GetAddress(nField0,nField1,nField2,nField3);
	DeviceIp.Format("%d.%d.%d.%d",nField0,nField1,nField2,nField3);

	setCStringToConfig("CAMD","Ip",DeviceIp);
}
/*************************************************
Function:   OnEnChangeEditPortA
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::OnEnChangeEditPortA()
{
	UpdateData(TRUE);
	CString str;
	str.Format("%d", port_a);
	setCStringToConfig("CAMA","Port",str);
}
/*************************************************
Function:   OnEnChangeEditPortB
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::OnEnChangeEditPortB()
{
	UpdateData(TRUE);
	CString str;
	str.Format("%d", port_b);
	setCStringToConfig("CAMB","Port",str);
}
/*************************************************
Function:   OnEnChangeEditPortC
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::OnEnChangeEditPortC()
{
	UpdateData(TRUE);
	CString str;
	str.Format("%d", port_c);
	setCStringToConfig("CAMC","Port",str);
}
/*************************************************
Function:   OnEnChangeEditPortD
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::OnEnChangeEditPortD()
{
	UpdateData(TRUE);
	CString str;
	str.Format("%d", port_d);
	setCStringToConfig("CAMD","Port",str);
}
/*************************************************
Function:   OnEnChangeEditUserA
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::OnEnChangeEditUserA()
{
	UpdateData(TRUE);
	setCStringToConfig("CAMA","User",user_d);
}
/*************************************************
Function:   OnEnChangeEditPwdA
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::OnEnChangeEditPwdA()
{
	UpdateData(TRUE);
	setCStringToConfig("CAMA","Password",pwd_d);
}
/*************************************************
Function:   OnEnChangeEditUserC
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::OnEnChangeEditUserC()
{
	UpdateData(TRUE);
	setCStringToConfig("CAMC","User",user_d);
}
/*************************************************
Function:   OnEnChangeEditPwdC
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::OnEnChangeEditPwdC()
{
	UpdateData(TRUE);
	setCStringToConfig("CAMC","Password",pwd_d);
}
/*************************************************
Function:   OnEnChangeEditUserB
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::OnEnChangeEditUserB()
{
	UpdateData(TRUE);
	setCStringToConfig("CAMB","User",user_d);
}
/*************************************************
Function:   OnEnChangeEditPwdB
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::OnEnChangeEditPwdB()
{
	UpdateData(TRUE);
	setCStringToConfig("CAMB","Password",pwd_d);
}
/*************************************************
Function:   OnEnChangeEditUserD
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::OnEnChangeEditUserD()
{
	UpdateData(TRUE);
	setCStringToConfig("CAMD","User",user_d);
}
/*************************************************
Function:   OnEnChangeEditPwdD
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::OnEnChangeEditPwdD()
{
	UpdateData(TRUE);
	setCStringToConfig("CAMD","Password",pwd_d);
}
/*************************************************
Function:   initParam
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::initParam()
{

	isAExist.SetCheck(parent->camer_a);OnCameraACheck();
	isBExist.SetCheck(parent->camer_b);OnCameraBCheck();
	isCExist.SetCheck(parent->camer_c);OnCameraCCheck();
	isDExist.SetCheck(parent->camer_d);OnCameraDCheck();

	ip_a.SetAddress(transCStringToInt(getCStringFromConfig("CAMA","Ip")));
	user_a=getCStringFromConfig("CAMA","User");//"admin";
	pwd_a=getCStringFromConfig("CAMA","Password");//"admin123";
	port_a=atoi(getCStringFromConfig("CAMA","Port"));//8000;

	ip_b.SetAddress(transCStringToInt(getCStringFromConfig("CAMB","Ip")));
	user_b=getCStringFromConfig("CAMB","User");//"admin";
	pwd_b=getCStringFromConfig("CAMB","Password");//"admin123";
	port_b=atoi(getCStringFromConfig("CAMB","Port"));//8000;

	ip_c.SetAddress(transCStringToInt(getCStringFromConfig("CAMC","Ip")));
	user_c=getCStringFromConfig("CAMC","User");//"admin";
	pwd_c=getCStringFromConfig("CAMC","Password");//"admin123";
	port_c=atoi(getCStringFromConfig("CAMC","Port"));//8000;

	ip_d.SetAddress(transCStringToInt(getCStringFromConfig("CAMD","Ip")));
	user_d=getCStringFromConfig("CAMD","User");//"admin";
	pwd_d=getCStringFromConfig("CAMD","Password");//"admin123";
	port_d=atoi(getCStringFromConfig("CAMD","Port"));//7000;

	UpdateData(false);


	ip_a.EnableWindow(true);
	ip_b.EnableWindow(true);
	ip_c.EnableWindow(true);
	ip_d.EnableWindow(true);



	isAExist.EnableWindow(false);
	isBExist.EnableWindow(false);
	isCExist.EnableWindow(false);
	isDExist.EnableWindow(false);

	ip_a.EnableWindow(false);
	GetDlgItem(IDC_EDIT_PORT_A)-> EnableWindow(false);
	GetDlgItem(IDC_EDIT_USER_A)-> EnableWindow(false);
	GetDlgItem(IDC_EDIT_PWD_A)-> EnableWindow(false);

	ip_b.EnableWindow(false);
	GetDlgItem(IDC_EDIT_PORT_B)-> EnableWindow(false);
	GetDlgItem(IDC_EDIT_USER_B)-> EnableWindow(false);
	GetDlgItem(IDC_EDIT_PWD_B)-> EnableWindow(false);

	ip_c.EnableWindow(false);
	GetDlgItem(IDC_EDIT_PORT_C)-> EnableWindow(false);
	GetDlgItem(IDC_EDIT_USER_C)-> EnableWindow(false);
	GetDlgItem(IDC_EDIT_PWD_C)-> EnableWindow(false);


	ip_d.EnableWindow(false);
	GetDlgItem(IDC_EDIT_PORT_D)-> EnableWindow(false);
	GetDlgItem(IDC_EDIT_USER_D)-> EnableWindow(false);
	GetDlgItem(IDC_EDIT_PWD_D)-> EnableWindow(false);

}
/*************************************************
Function:   OnBnClickedButtonSave
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::OnBnClickedButtonSave()
{
	CString str;
    c_Brightness.GetLBText(c_Brightness.GetCurSel(),str);
	//setCStringToConfig("CAMS","Brightness",strText);

	//CString str = getCStringFromConfig("CAMS","Brightness");
	BYTE brightness = _ttoi(_T(str));
	


	//CString strText;
    c_Contrast.GetLBText(c_Contrast.GetCurSel(),str);
	//setCStringToConfig("CAMS","Contrast",strText);

	//str = getCStringFromConfig("CAMS","Contrast");
	BYTE contrast = _ttoi(_T(str));

	//CString strText;
    c_Sharpness.GetLBText(c_Sharpness.GetCurSel(),str);
	//setCStringToConfig("CAMS","Sharpness",strText);

	//str = getCStringFromConfig("CAMS","Sharpness");
	BYTE sharpness = _ttoi(_T(str));

	//CString strText;
    c_Saturation.GetLBText(c_Saturation.GetCurSel(),str);
	//setCStringToConfig("CAMS","Saturation",strText);

	//str = getCStringFromConfig("CAMS","Saturation");
	BYTE saturation = _ttoi(_T(str));

	//CString strText;
    c_ExpMode.GetLBText(c_ExpMode.GetCurSel(),str);
	//setCStringToConfig("CAMS","ExposureMode",strText);

	//str = getCStringFromConfig("CAMS","ExposureMode");
	BYTE exposuremode;
	if(str == "手动曝光")
		exposuremode = 0;
	else if(str == "自动曝光")
		exposuremode = 1;
	else 
		exposuremode = 0;

	//CString strText;
    c_ExpTime.GetLBText(c_ExpTime.GetCurSel(),str);
	//setCStringToConfig("CAMS","ExposureTime",strText);

	//str = getCStringFromConfig("CAMS","ExposureTime");
	int exposuretime = _ttoi(_T(str));

	//CString strText;
    c_DayNight.GetLBText(c_DayNight.GetCurSel(),str);
	//setCStringToConfig("CAMS","DayNight",strText);

	//str = getCStringFromConfig("CAMS","DayNight");
	BYTE daynight;

	if(str == "白天")
		daynight = 0;
	else if(str == "夜晚")
		daynight = 1;
	else if(str == "自动")
		daynight = 2;
	else if(str == "定时")
		daynight = 3;
	else if(str == "报警输入触发")
		daynight = 4;
	else 
		daynight = 0;

	saveParams(brightness,contrast,sharpness,saturation,exposuremode,exposuretime,daynight);
}
/*************************************************
Function:   OnBnClickedButtonLogin
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::OnBnClickedButtonLogin()
{
	CString pwd,cap;
	passwordInput.GetWindowTextA(pwd);
	if(pwd != "zhjlt")
		return;
	loginSet.GetWindowTextA(cap);

	if(cap == "登录"){

		passwordInput.SetWindowTextA(_T(""));
		isAExist.EnableWindow(true);
		isBExist.EnableWindow(true);
		isCExist.EnableWindow(true);
		isDExist.EnableWindow(true);



		if(parent->camer_a == true){

			ip_a.EnableWindow(true);
			GetDlgItem(IDC_EDIT_PORT_A)-> EnableWindow(true);
			GetDlgItem(IDC_EDIT_USER_A)-> EnableWindow(true);
			GetDlgItem(IDC_EDIT_PWD_A)-> EnableWindow(true);
		}else{
			ip_a.EnableWindow(false);
			GetDlgItem(IDC_EDIT_PORT_A)-> EnableWindow(false);
			GetDlgItem(IDC_EDIT_USER_A)-> EnableWindow(false);
			GetDlgItem(IDC_EDIT_PWD_A)-> EnableWindow(false);
		}

		if(parent->camer_b == true){

			ip_b.EnableWindow(true);
			GetDlgItem(IDC_EDIT_PORT_B)-> EnableWindow(true);
			GetDlgItem(IDC_EDIT_USER_B)-> EnableWindow(true);
			GetDlgItem(IDC_EDIT_PWD_B)-> EnableWindow(true);
		}else{
			ip_b.EnableWindow(false);
			GetDlgItem(IDC_EDIT_PORT_B)-> EnableWindow(false);
			GetDlgItem(IDC_EDIT_USER_B)-> EnableWindow(false);
			GetDlgItem(IDC_EDIT_PWD_B)-> EnableWindow(false);
		}

		if(parent->camer_c == true){

			ip_c.EnableWindow(true);
			GetDlgItem(IDC_EDIT_PORT_C)-> EnableWindow(true);
			GetDlgItem(IDC_EDIT_USER_C)-> EnableWindow(true);
			GetDlgItem(IDC_EDIT_PWD_C)-> EnableWindow(true);
		}else{
			ip_c.EnableWindow(false);
			GetDlgItem(IDC_EDIT_PORT_C)-> EnableWindow(false);
			GetDlgItem(IDC_EDIT_USER_C)-> EnableWindow(false);
			GetDlgItem(IDC_EDIT_PWD_C)-> EnableWindow(false);
		}

		if(parent->camer_d == true){

			ip_d.EnableWindow(true);
			GetDlgItem(IDC_EDIT_PORT_D)-> EnableWindow(true);
			GetDlgItem(IDC_EDIT_USER_D)-> EnableWindow(true);
			GetDlgItem(IDC_EDIT_PWD_D)-> EnableWindow(true);
		}else{
			ip_d.EnableWindow(false);
			GetDlgItem(IDC_EDIT_PORT_D)-> EnableWindow(false);
			GetDlgItem(IDC_EDIT_USER_D)-> EnableWindow(false);
			GetDlgItem(IDC_EDIT_PWD_D)-> EnableWindow(false);
		}

		GetDlgItem(IDC_BRIGHTNESS)-> EnableWindow(true);
		GetDlgItem(IDC_CONTRAST)-> EnableWindow(true);
		GetDlgItem(IDC_SHARPNESS)-> EnableWindow(true);
		GetDlgItem(IDC_SATURATION)-> EnableWindow(true);
		GetDlgItem(IDC_EXP_MODE)-> EnableWindow(true);
		GetDlgItem(IDC_EXP_TIME)-> EnableWindow(true);
		GetDlgItem(IDC_DAYNIGHT)-> EnableWindow(true);
		GetDlgItem(IDC_BUTTON_SAVE)-> EnableWindow(true);

		loginSet.SetWindowTextA(_T("注销"));
	}else{

		Logout();
	}
}
/*************************************************
Function:   Logout
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::Logout()
{
		isAExist.EnableWindow(false);
		isBExist.EnableWindow(false);
		isCExist.EnableWindow(false);
		isDExist.EnableWindow(false);

		ip_a.EnableWindow(false);
		GetDlgItem(IDC_EDIT_PORT_A)-> EnableWindow(false);
		GetDlgItem(IDC_EDIT_USER_A)-> EnableWindow(false);
		GetDlgItem(IDC_EDIT_PWD_A)-> EnableWindow(false);

		ip_b.EnableWindow(false);
		GetDlgItem(IDC_EDIT_PORT_B)-> EnableWindow(false);
		GetDlgItem(IDC_EDIT_USER_B)-> EnableWindow(false);
		GetDlgItem(IDC_EDIT_PWD_B)-> EnableWindow(false);

		ip_c.EnableWindow(false);
		GetDlgItem(IDC_EDIT_PORT_C)-> EnableWindow(false);
		GetDlgItem(IDC_EDIT_USER_C)-> EnableWindow(false);
		GetDlgItem(IDC_EDIT_PWD_C)-> EnableWindow(false);


		ip_d.EnableWindow(false);
		GetDlgItem(IDC_EDIT_PORT_D)-> EnableWindow(false);
		GetDlgItem(IDC_EDIT_USER_D)-> EnableWindow(false);
		GetDlgItem(IDC_EDIT_PWD_D)-> EnableWindow(false);
		
		GetDlgItem(IDC_BRIGHTNESS)-> EnableWindow(false);
		GetDlgItem(IDC_CONTRAST)-> EnableWindow(false);
		GetDlgItem(IDC_SHARPNESS)-> EnableWindow(false);
		GetDlgItem(IDC_SATURATION)-> EnableWindow(false);
		GetDlgItem(IDC_EXP_MODE)-> EnableWindow(false);
		GetDlgItem(IDC_EXP_TIME)-> EnableWindow(false);
		GetDlgItem(IDC_DAYNIGHT)-> EnableWindow(false);
		GetDlgItem(IDC_BUTTON_SAVE)-> EnableWindow(false);

		loginSet.SetWindowTextA(_T("登录"));
}
/*************************************************
Function:   OnVScroll
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
void CDialogSetting::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
	SCROLLINFO scrollinfo;
	GetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
	switch (nSBCode)
	{
	case SB_BOTTOM:
		ScrollWindow(0,(scrollinfo.nPos-scrollinfo.nMax)*10);
		scrollinfo.nPos = scrollinfo.nMax;
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		break;
	case SB_TOP:
		ScrollWindow(0,(scrollinfo.nPos-scrollinfo.nMin)*10);
		scrollinfo.nPos = scrollinfo.nMin;
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		break;
	case SB_LINEUP:
		scrollinfo.nPos -= 20;
		if (scrollinfo.nPos<scrollinfo.nMin)
		{
			scrollinfo.nPos = scrollinfo.nMin;
			break;
		}
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		ScrollWindow(0,15);
		break;
	case SB_LINEDOWN:
		scrollinfo.nPos += 20;
		if (scrollinfo.nPos>scrollinfo.nMax)
		{
			scrollinfo.nPos = scrollinfo.nMax;
			break;
		}
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		ScrollWindow(0,-15);
		break;
	case SB_PAGEUP:
		scrollinfo.nPos -= 50;
		if (scrollinfo.nPos<scrollinfo.nMin)
		{
			scrollinfo.nPos = scrollinfo.nMin;
			SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
			break;
		}
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		ScrollWindow(0,10*5);
		break;
	case SB_PAGEDOWN:
		scrollinfo.nPos += 50;
		if (scrollinfo.nPos>scrollinfo.nMax)
		{
			scrollinfo.nPos = scrollinfo.nMax;
			SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
			break;
		}
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		ScrollWindow(0,-10*5);
		break;
	case SB_ENDSCROLL:
		// MessageBox("SB_ENDSCROLL");
		break;
	case SB_THUMBPOSITION:
		// ScrollWindow(0,(scrollinfo.nPos-nPos)*10);
		// scrollinfo.nPos = nPos;
		// SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		break;
	case SB_THUMBTRACK:
		ScrollWindow(0,(scrollinfo.nPos-nPos));
		scrollinfo.nPos = nPos;
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		break;
	}


}
/*************************************************
Function:   OnMouseWheel
Desc:	    
Input:      
Return:		SUCCESS 成功
            FAIL 失败
**************************************************/
BOOL CDialogSetting::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default


	if(zDelta < 0)
	{
		OnVScroll(SB_LINEDOWN, GetScrollPos(SB_VERT),  GetScrollBarCtrl(SB_VERT));
	}
	else if (zDelta > 0)
	{
		OnVScroll(SB_LINEUP, GetScrollPos(SB_VERT),  GetScrollBarCtrl(SB_VERT));
	}


	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}
