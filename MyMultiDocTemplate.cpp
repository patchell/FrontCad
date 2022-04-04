#include "stdafx.h"
#include "JamCadDefines.h"
#include "JamCad.h"
#include "MyMultiDocTemplate.h"

/*
CMyMultiDocTemplate::CMyMultiDocTemplate()
{

}
*/

CMyMultiDocTemplate::CMyMultiDocTemplate(UINT nIDResource,
			CRuntimeClass* pDocClass,
			CRuntimeClass* pFrameClass,
			CRuntimeClass* pViewClass):
	CMultiDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
{
}


CMyMultiDocTemplate::~CMyMultiDocTemplate()
{
}

CDocument * CMyMultiDocTemplate::MakeNewDocument()
{
	CDocument* pDocument = CreateNewDocument();
	if (pDocument == NULL)
	{
		TRACE(traceAppMsg, 0, "CDocTemplate::CreateNewDocument returned NULL.\n");
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		return NULL;
	}
	ASSERT_VALID(pDocument);

	BOOL bAutoDelete = pDocument->m_bAutoDelete;
	pDocument->m_bAutoDelete = FALSE;   // don't destroy if something goes wrong
	CFrameWnd* pFrame = CreateNewFrame(pDocument, NULL);
	pDocument->m_bAutoDelete = bAutoDelete;
	if (pFrame == NULL)
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		delete pDocument;       // explicit delete on error
		return NULL;
	}
	ASSERT_VALID(pFrame);
	if (!pDocument->OnNewDocument())
	{
		// user has be alerted to what failed in OnNewDocument
		TRACE(traceAppMsg, 0, "CDocument::OnNewDocument returned FALSE.\n");
		pFrame->DestroyWindow();
		return NULL;
	}
	InitialUpdateFrame(pFrame, pDocument, true);
	return pDocument;

}
