
// MFCSignal.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCSignalApp:
// �йش����ʵ�֣������ MFCSignal.cpp
//

class CMFCSignalApp : public CWinAppEx
{
public:
	CMFCSignalApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCSignalApp theApp;