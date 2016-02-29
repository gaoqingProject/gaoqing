// DialogSetting.cpp : implementation file
//

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
CString CDialogSetting::getCStringFromConfig(CString cam,CString sub)
{
	CgaoqingDlg *dlg = (CgaoqingDlg *)GetParent()->GetParent();
	CString str = _T(dlg->getConfig(cam,sub,""));

	return str;
}
void CDialogSetting::setCStringToConfig(CString cam,CString sub,CString str)
{
	CgaoqingDlg *dlg = (CgaoqingDlg *)GetParent()->GetParent();
	dlg->setConfig(cam,sub,str);

}
void CDialogSetting::saveParams(BYTE bright,BYTE contrast,BYTE sharpness,BYTE saturation,int exp_mode,int exp_time,int daynight)
{
	CgaoqingDlg *dlg = (CgaoqingDlg *)GetParent()->GetParent();
	dlg->saveParams(bright,contrast,sharpness,saturation,exp_mode,exp_time,daynight);

}
PARAM_STRU CDialogSetting::getParams()
{
	CgaoqingDlg *dlg = (CgaoqingDlg *)GetParent()->GetParent();
	return dlg->getParams();

}
CDialogSetting::~CDialogSetting()
{
}
BOOL CDialogSetting::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString str;
	int i;
	for(i=1;i<=100;i++){
		str.Format("%d",i);
		c_Brightness.AddString(str);
		str="";
	}
	for(i=1;i<=100;i++){
		str.Format("%d",i);
		c_Contrast.AddString(str);
		str="";
	}
	for(i=1;i<=100;i++){
		str.Format("%d",i);
		c_Sharpness.AddString(str);
		str="";
	}
	for(i=1;i<=100;i++){
		str.Format("%d",i);
		c_Saturation.AddString(str);
		str="";
	}

	c_ExpMode.AddString(_T("自动"));
	c_ExpMode.AddString(_T("手动"));


	for(i=1;i<=100;i++){
		str.Format("%d",i*100);
		c_ExpTime.AddString(str);
		str="";
	}
	
	c_DayNight.AddString(_T("白天"));
	c_DayNight.AddString(_T("夜晚"));
	c_DayNight.AddString(_T("自动"));
	c_DayNight.AddString(_T("定时"));
	c_DayNight.AddString(_T("报警输入触发"));

	PARAM_STRU pm = getParams();

	str.Format("%d",pm.brightness);
	//str = getCStringFromConfig("CAMS","Brightness");
	c_Brightness.SetWindowTextA(str);
	setCStringToConfig("CAMS","Brightness",str);
	str="";

	str.Format("%d",pm.contrast);
	//str = getCStringFromConfig("CAMS","Contrast");
	c_Contrast.SetWindowTextA(str);
	setCStringToConfig("CAMS","Contrast",str);
	str="";

	str.Format("%d",pm.sharpness);
	//str = getCStringFromConfig("CAMS","Sharpness");
	c_Sharpness.SetWindowTextA(str);
	setCStringToConfig("CAMS","Sharpness",str);
	str="";

	str.Format("%d",pm.saturation);
	//str = getCStringFromConfig("CAMS","Saturation");
	c_Saturation.SetWindowTextA(str);
	setCStringToConfig("CAMS","Saturation",str);
	str="";

	str.Format("%d",pm.exposuremode);

	if(pm.exposuremode ==0){
		str = "手动曝光";
	}else{
		str = "自动曝光";
	}

	//str = getCStringFromConfig("CAMS","ExposureMode");
	c_ExpMode.SetWindowTextA(str);
	setCStringToConfig("CAMS","ExposureMode",str);
	str="";

	str.Format("%d",pm.exposuretime);
	//str = getCStringFromConfig("CAMS","ExposureTime");
	c_ExpTime.SetWindowTextA(str);
	setCStringToConfig("CAMS","ExposureTime",str);
	str="";

	
	if(pm.daynight ==0){
		str = "白天";
	}else if(pm.daynight ==1){
		str = "夜晚";
	}else if(pm.daynight ==2){
		str = "自动";
	}else if(pm.daynight ==3){
		str = "定时";
	}else if(pm.daynight ==4){
		str = "报警输入触发";
	}

	//str.Format("%d",pm.daynight);
	//str = getCStringFromConfig("CAMS","DayNight");
	c_DayNight.SetWindowTextA(str);
	setCStringToConfig("CAMS","DayNight",str);
	str="";


	initParam();
	return TRUE;  // return TRUE  unless you set the focus to a control

}
CString CDialogSetting::getIp()
{
	UpdateData(TRUE);
	//prepare for ip
	CString DeviceIp;
	BYTE nField0,nField1,nField2,nField3; 
	ip_a.GetAddress(nField0,nField1,nField2,nField3);
	DeviceIp.Format("%d.%d.%d.%d",nField0,nField1,nField2,nField3);
	return DeviceIp;
	
}/*
int CDialogSetting::getPort()
{
	return port_a;
	
}*/
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
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDialogSetting::OnBnClickedButtonSave)
	ON_CBN_SELCHANGE(IDC_CONTRAST, &CDialogSetting::OnCbnSelchangeContrast)
	ON_CBN_SELCHANGE(IDC_SHARPNESS, &CDialogSetting::OnCbnSelchangeSharpness)
	ON_CBN_SELCHANGE(IDC_SATURATION, &CDialogSetting::OnCbnSelchangeSaturation)
	ON_CBN_SELCHANGE(IDC_EXP_MODE, &CDialogSetting::OnCbnSelchangeExpMode)
	ON_CBN_SELCHANGE(IDC_EXP_TIME, &CDialogSetting::OnCbnSelchangeExpTime)
	ON_CBN_SELCHANGE(IDC_DAYNIGHT, &CDialogSetting::OnCbnSelchangeDaynight)
	ON_CBN_SELCHANGE(IDC_BRIGHTNESS, &CDialogSetting::OnCbnSelchangeBrightness)




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
END_MESSAGE_MAP()
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
void CDialogSetting::OnOK(){

	return;
}
int CDialogSetting::transCStringToInt(CString addstr)
{
	DWORD add;
	vector<CString> ips = SplitCString(addstr,".");
	add = atoi(ips[0])*256*256*256+atoi(ips[1])*256*256+atoi(ips[2])*256+atoi(ips[3]);

	return add;
}
#include "gaoqingDlg.h"
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
void CDialogSetting::OnEnChangeEditPortA()
{
	UpdateData(TRUE);
	CString str;
	str.Format("%d", port_a);
	setCStringToConfig("CAMA","Port",str);
}

void CDialogSetting::OnEnChangeEditPortB()
{
	UpdateData(TRUE);
	CString str;
	str.Format("%d", port_b);
	setCStringToConfig("CAMB","Port",str);
}

void CDialogSetting::OnEnChangeEditPortC()
{
	UpdateData(TRUE);
	CString str;
	str.Format("%d", port_c);
	setCStringToConfig("CAMC","Port",str);
}

void CDialogSetting::OnEnChangeEditPortD()
{
	UpdateData(TRUE);
	CString str;
	str.Format("%d", port_d);
	setCStringToConfig("CAMD","Port",str);
}

void CDialogSetting::OnEnChangeEditUserA()
{
	UpdateData(TRUE);
	setCStringToConfig("CAMA","User",user_d);
}

void CDialogSetting::OnEnChangeEditPwdA()
{
	UpdateData(TRUE);
	setCStringToConfig("CAMA","Password",pwd_d);
}

void CDialogSetting::OnEnChangeEditUserC()
{
	UpdateData(TRUE);
	setCStringToConfig("CAMC","User",user_d);
}

void CDialogSetting::OnEnChangeEditPwdC()
{
	UpdateData(TRUE);
	setCStringToConfig("CAMC","Password",pwd_d);
}

void CDialogSetting::OnEnChangeEditUserB()
{
	UpdateData(TRUE);
	setCStringToConfig("CAMB","User",user_d);
}

void CDialogSetting::OnEnChangeEditPwdB()
{
	UpdateData(TRUE);
	setCStringToConfig("CAMB","Password",pwd_d);
}

void CDialogSetting::OnEnChangeEditUserD()
{
	UpdateData(TRUE);
	setCStringToConfig("CAMD","User",user_d);
}

void CDialogSetting::OnEnChangeEditPwdD()
{
	UpdateData(TRUE);
	setCStringToConfig("CAMD","Password",pwd_d);
}

// CDialogSetting message handlers
void CDialogSetting::initParam()
{


		isAExist.SetCheck(parent->camer_a);OnCameraACheck();
		isBExist.SetCheck(parent->camer_b);OnCameraBCheck();
		isCExist.SetCheck(parent->camer_c);OnCameraCCheck();
		isDExist.SetCheck(parent->camer_d);OnCameraDCheck();



		//ip_a.SetAddress(0xc0a877FB);
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




/*
	
	*/
}
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
void CDialogSetting::OnCbnSelchangeBrightness()
{
	
}

void CDialogSetting::OnCbnSelchangeContrast()
{
	
}

void CDialogSetting::OnCbnSelchangeSharpness()
{
	
}

void CDialogSetting::OnCbnSelchangeSaturation()
{
	
}

void CDialogSetting::OnCbnSelchangeExpMode()
{
	
}

void CDialogSetting::OnCbnSelchangeExpTime()
{
	
}

void CDialogSetting::OnCbnSelchangeDaynight()
{
	
}



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
