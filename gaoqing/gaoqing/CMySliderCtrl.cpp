
// MySliderCtrl.cpp : ʵ���ļ�
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



// CMySliderCtrl ��Ϣ�������


void CMySliderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	*sign = true;
	CSliderCtrl::OnLButtonDown(nFlags, point);
}


 

