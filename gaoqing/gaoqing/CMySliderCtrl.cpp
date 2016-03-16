
// MySliderCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "CMySliderCtrl.h"

// CMySliderCtrl

IMPLEMENT_DYNAMIC(CMySliderCtrl, CSliderCtrl)
CMySliderCtrl::CMySliderCtrl()
{
}

CMySliderCtrl::~CMySliderCtrl()
{
}


BEGIN_MESSAGE_MAP(CMySliderCtrl, CSliderCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()



// CMySliderCtrl 消息处理程序


void CMySliderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	*sign = true;
	CSliderCtrl::OnLButtonDown(nFlags, point);
}


 

