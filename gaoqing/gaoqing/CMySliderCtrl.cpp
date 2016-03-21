/*
File name:   CMySliderCtrl.cpp
File Author: Li Weichen
Date:        2016.3.1
*/
#include "stdafx.h"
#include "CMySliderCtrl.h"

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

void CMySliderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	*sign = true;
	CSliderCtrl::OnLButtonDown(nFlags, point);
}


 

