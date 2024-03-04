// BaseDocument.cpp : implementation file
//

#include "pch.h"


// CBaseDocument

IMPLEMENT_DYNCREATE(CBaseDocument, CDocument)

CBaseDocument::CBaseDocument()
{
	m_pDocView = 0;
	m_pDocObject = 0;

}

CBaseDocument::~CBaseDocument()
{
}

void CBaseDocument::SetFileName(CString& csFileName)
{
	m_csFileName = csFileName;
}

void CBaseDocument::UnSelectAll(UINT nKinds)
{
	GetDocObject()->UnSelectAll(OBJKIND_ALL);
}

int CBaseDocument::PointInObjectAndSelect(
	DOUBLEPOINT p, 
	CCadObject** ppL, 
	CCadObject* pExcludeObject, 
	int n, 
	UINT nKinds
)
{
	n = GetDocObject()->PointInObjectAndSelect(p, pExcludeObject, ppL, 0, n, nKinds);
	return n;
};

BOOL CBaseDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

BEGIN_MESSAGE_MAP(CBaseDocument, CDocument)
END_MESSAGE_MAP()


// CBaseDocument diagnostics

#ifdef _DEBUG
void CBaseDocument::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CBaseDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

int CBaseDocument::CutToClipBoard()
{
	CCadObject* pObj = GetDocObject()->GetHead();
	int Count = 0;

	// Clean out all the old stuff in the CB
	GETAPP.GetClipBoard()->RemoveAndDestroyALL();
	while (pObj)
	{
		if (pObj->IsSelected())
		{
			++Count;
			GetDocObject()->RemoveObject(pObj);	//remove from selected List
			GETAPP.GetClipBoard()->AddObjectAtTail(pObj);
		}
		pObj = pObj->GetNext();
	}
	return Count;
}

int CBaseDocument::CopyToClipBoard()
{
	CCadObject* pObj = GetDocObject()->GetHead();
	int Count = 0;

	while (pObj)
	{
		if (pObj->IsSelected())
		{
			++Count;
			GETAPP.GetClipBoard()->AddObjectAtTail(pObj->Copy());
		}
		pObj = pObj->GetNext();
	}
	return Count;
}

int CBaseDocument::PasteFromClipBoard()
{
	CCadObject* pObj = GETAPP.GetClipBoard()->GetHead();;
	int Count = 0;

	while (pObj)
	{
		++Count;
		GetDocObject()->AddObjectAtTail(pObj->Copy());
		pObj = pObj->GetNext();
	}
	return Count;
}

void CBaseDocument::ProcessZoom(CScale& InchesPerPixel)
{
	CCadObject* pObj;

	printf("Zoom Factor = %d\n", GETVIEW->GetGrid().GetZoomFactor());
	pObj = GetDocObject()->GetHead();
	while (pObj)
	{
		pObj->ProcessZoom(InchesPerPixel);
		pObj = pObj->GetNext();
	}
}

void CBaseDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}



// CBaseDocument commands
