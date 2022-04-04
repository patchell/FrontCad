#pragma once
#include "afxwin.h"
class CMyMultiDocTemplate:public CMultiDocTemplate
{
public:
	CMyMultiDocTemplate();
	CMyMultiDocTemplate(UINT nIDResource,
			CRuntimeClass* pDocClass,
		CRuntimeClass* pFrameClass,
		CRuntimeClass* pViewClass);
	virtual ~CMyMultiDocTemplate();
	CDocument * MakeNewDocument();
};

