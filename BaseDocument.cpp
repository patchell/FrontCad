// BaseDocument.cpp : implementation file
//

#include "pch.h"


// CBaseDocument

IMPLEMENT_DYNCREATE(CBaseDocument, CDocument)

CBaseDocument::CBaseDocument()
{
	m_pHead = 0;
	m_pTail = 0;
	m_nTotalObjects = 0;
}

CBaseDocument::~CBaseDocument()
{
}

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

#ifndef _WIN32_WCE
// CBaseDocument serialization

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
#endif


void CBaseDocument::AddObjectAtFront(CCadObject * pObj)
{
	if (GetHead() == 0)
	{
		SetHead(pObj);
		SetTail(pObj);;
	}
	else
	{
		pObj->SetNext(GetHead());
		GetHead()->SetPrev(pObj);
		SetHead(pObj); 
	}
	++m_nTotalObjects;
}

void CBaseDocument::AddObjectAtEnd(CCadObject* pObj)
{
	if (GetHead() == 0)
	{
		SetHead(pObj);
		SetTail(pObj);
	}
	else
	{
		pObj->SetPrev(GetTail());
		GetTail()->SetNext(pObj);
		SetTail(pObj);
	}
	++m_nTotalObjects;
}

void CBaseDocument::RemoveObject(CCadObject * pObj)
{
	if (pObj == GetHead())
	{
		SetHead(pObj->GetNext());
		if (GetHead())
			GetHead()->SetPrev(0);
		else
			SetTail(0);
	}
	else if (pObj == GetTail())
	{
		SetTail(pObj->GetPrev());
		if (GetTail())
			GetTail()->SetNext(0);
		else
			SetHead(0);
	}
	else
	{
		pObj->GetNext()->SetPrev(pObj->GetPrev());
		pObj->GetPrev()->SetNext(pObj->GetNext());
	}
}

void CBaseDocument::SetAllDirty(void)
{
	CCadObject* pObject = GetHead();

	while (pObject)
	{
		pObject->SetDirty(TRUE);
		pObject = pObject->GetNext();
	}
}

// CBaseDocument commands
