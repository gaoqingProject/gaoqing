/*
File name:   CgaoqingApp.h
File Author: Li Weichen
Date:        2016.3.1
*/
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CgaoqingApp:
// See gaoqing.cpp for the implementation of this class
//

class CgaoqingApp : public CWinApp
{
public:
	CgaoqingApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CgaoqingApp theApp;