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
	if (GetCurrentOrigin())
	{
		GetCurrentOrigin()->AddObjectAtHead(pObj);
	}
	else
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
}

void CBaseDocument::AddObjectAtEnd(CCadObject* pObj)
{
	if (GetCurrentOrigin())
	{
		GetCurrentOrigin()->AddObjectAtTail(pObj);
	}
	else
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
}

void CBaseDocument::RemoveObject(CCadObject * pObj)
{
	if (GetCurrentOrigin())
	{
		GetCurrentOrigin()->RemoveObject(pObj);
	}
	else
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
		--m_nTotalObjects;
	}
}

//----------------------------------------------
// List of Origin Objects
//---------------------------------------------

void CBaseDocument::AddOriginAtFront(CCadObject* pObj)
{
	if (GetOriginHead() == 0)
	{
		SetOriginHead(pObj);
		SetOriginTail(pObj);;
	}
	else
	{
		pObj->SetNextOrigin(GetOriginHead());
		GetOriginHead()->SetPrevOrigin(pObj);
		SetOriginHead(pObj);
	}
	++m_nTotalOrigins;
}

void CBaseDocument::AddOriginAtTail(CCadObject* pObj)
{
	if (GetOriginHead() == 0)
	{
		SetOriginHead(pObj);
		SetOriginTail(pObj);
	}
	else
	{
		pObj->SetPrevOrigin(GetOriginTail());
		GetOriginTail()->SetNextOrigin(pObj);
		SetOriginTail(pObj);
	}
	++m_nTotalOrigins;
}

void CBaseDocument::RemoveOrigin(CCadObject* pObj)
{
	if (pObj == GetOriginHead())
	{
		SetOriginHead(pObj->GetNextOrigin());
		if (GetOriginHead())
			GetOriginHead()->SetPrevOrigin(0);
		else
			SetOriginTail(0);
	}
	else if (pObj == GetOriginTail())
	{
		SetOriginTail(pObj->GetPrevOrigin());
		if (GetOriginTail())
			GetOriginTail()->SetNextOrigin(0);
		else
			SetOriginHead(0);
	}
	else
	{
		pObj->GetNextOrigin()->SetPrevOrigin(pObj->GetPrevOrigin());
		pObj->GetPrevOrigin()->SetNextOrigin(pObj->GetNextOrigin());
	}
	--m_nTotalOrigins;
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
