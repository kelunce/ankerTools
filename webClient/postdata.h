
// postdata.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CpostdataApp:
// �йش����ʵ�֣������ postdata.cpp
//

class CpostdataApp : public CWinAppEx
{
public:
	CpostdataApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CpostdataApp theApp;