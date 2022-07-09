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
	m_nTotalOrigins = 0;
	m_pCurrentOrigin = 0;
	m_pOriginHead = 0;
	m_pOriginTail = 0;
	m_pDocView = 0;
	m_nTotalSelections = 0;
	m_pSelObjHead = 0;
	m_pSelObjTail = 0;
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

int CBaseDocument::CutToClipBoard()
{
	CCadObject* pObj = GetSelListHead();
	int Count = 0;

	GETAPP.GetClipBoard().RemoveAndDestroyALL();
	while (pObj)
	{
		++Count;
		RemoveSelectedObject(pObj);	//remove from selected List
		RemoveObject(pObj);	//remove from document
		GETAPP.GetClipBoard().AddObjectAtTail(pObj);
		pObj = GetSelListHead();
	}
	return Count;
}

int CBaseDocument::CopyToClipBoard()
{
	CCadObject* pObj = GetSelListHead();
	int Count = 0;

	while (pObj)
	{
		++Count;
		GETAPP.GetClipBoard().AddObjectAtTail(pObj->CopyObject());
		pObj = pObj->GetNextSel();
	}
	return Count;
}

int CBaseDocument::PasteFromClipBoard()
{
	CCadObject* pObj = GETAPP.GetClipBoard().GetHead();;
	int Count = 0;

	while (pObj)
	{
		++Count;
		AddObjectAtTail(pObj->CopyObject());
		pObj = pObj->GetNext();
	}
	return Count;
}

void CBaseDocument::ProcessZoom(CScale& InchesPerPixel)
{
	CCadObject* pObj;

	pObj = GetHead();
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
#endif


void CBaseDocument::AddObjectAtHead(CCadObject * pObj)
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

void CBaseDocument::AddObjectAtTail(CCadObject* pObj)
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
		pObj->SetNext(0);
		pObj->SetPrev(0);
		--m_nTotalObjects;
	}
}

//----------------------------------------------
// List of Origin Objects
//---------------------------------------------

void CBaseDocument::AddOriginAtHead(CCadObject* pObj)
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

//-----------------------------------------------
// Methods that manage the selection of objects
//-----------------------------------------------


void CBaseDocument::RemoveUnselectedObjects()
{
	///-------------------------------------
	/// RemoveUnselectedObjects
	///remove all unselected objects
	///--------------------------------------
	CCadObject* pO, * pNext;

	pO = GetSelListHead();
	while (pO)
	{
		if (!pO->IsSelected())
		{
			pNext = pO->GetNextSel();
			RemoveSelectedObject(pO);
			pO = pNext;
		}
		else
		{
			pO = pO->GetNextSel();
		}
	}
}

void CBaseDocument::RemoveSelectedObject(CCadObject* pObj)
{
	///----------------------------------------------
	///	RemoveObject
	/// This fuction removes the specified object
	/// from the selected list
	///		parameters:
	///			pO.....pointer to object to remove
	///----------------------------------------------
	if (pObj == GetSelListHead())
	{
		SetSelListHead(pObj->GetNextSel());
		if (GetSelListHead())
			GetSelListHead()->SetPrevSel(0);
		else
			SetSelListTail(0);
	}
	else if (pObj == GetSelListTail())
	{
		SetSelListTail(pObj->GetPrevSel());
		if (GetSelListTail())
			GetSelListTail()->SetNextSel(0);
		else
			SetSelListHead(0);
	}
	else
	{
		pObj->GetNextSel()->SetPrevSel(pObj->GetPrevSel());
		pObj->GetPrevSel()->SetNextSel(pObj->GetNextSel());
	}
	pObj->SetNextSel(0);
	pObj->SetPrevSel(0);
	DecTotalSelectedItems();
}

CCadObject* CBaseDocument::RemoveSelectedObjectFromHead()
{
	CCadObject* pObj;

	pObj = GetSelListHead();
	SetSelListHead(pObj->GetNext());
	pObj->SetNext(0);
	pObj->SetPrev(0);
	return pObj;
}

int CBaseDocument::UnSelectAll()
{
	CCadObject* pObj, * next;
	int Count = 0;

	pObj = GetSelListHead();
	while (pObj)
	{
		pObj->SetSelected(FALSE);
		next = pObj->GetNextSel();
		RemoveSelectedObject(pObj);
		pObj = next;
		++Count;
	}
	return Count;
}

int CBaseDocument::SelectAnObject(CCadObject** ppObj, int n, DOUBLEPOINT p)
{
	//---------------------------------------------
	// SelectAnObject
	//		This method is called when the user has
	// requested that an object under the mouse be
	// selected, but there are multiple objects
	// there, and FrontCad needs the user to clarify
	// parameters:
	//	ppObj........points to a list of posible objects
	//	n............Number of objects in the list
	//	p............Coordinate of mouse pointer
	// returns:
	//	Index in ppObj where desired object is
	//		returns negative on error
	//---------------------------------------------
	int i, Id;
	CString csString;
	CPoint point = p;
	ClientToScreen(HWND(m_pDocView) ,&point);
	CMenu ConTextMenu;

	ConTextMenu.CreatePopupMenu();
	for (i = 0; i < n; ++i)
	{
		csString = ppObj[i]->GetObjDescription();
		ConTextMenu.AppendMenu(MF_STRING, 3000 + i, csString);
		ConTextMenu.EnableMenuItem(3000 + i, MF_ENABLED);;
	}
	Id = ConTextMenu.TrackPopupMenu(
		TPM_RETURNCMD | TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, 
		point.x, 
		point.y, 
		m_pDocView
	);
	ConTextMenu.DestroyMenu();
	if (Id >= 3000) Id -= 3000;
	else Id = -1;
	return Id;
}


void CBaseDocument::AddToSelListHead(CCadObject* pO)
{
	///----------------------------------------
	///	AddToSelListHead
	///		This Method adds an object to the
	/// selection list.
	///
	///	parameter:
	///		pO.....Object to add to list
	///---------------------------------------
	if (GetSelListHead())
	{
		pO->SetNextSel(GetSelListHead());
		GetSelListHead()->SetPrev(pO);
		SetSelListHead(pO);
	}
	else
	{
		SetSelListHead(pO);
		SetSelListTail(pO);
	}
	IncTotalSelectedItems();
}

void CBaseDocument::AddToSelListTail(CCadObject* pObj)
{
	if (GetSelListTail() == 0)
	{
		SetSelListHead(pObj);
		SetSelListTail(pObj);
	}
	else
	{
		pObj->SetPrevSel(GetSelListTail());
		GetSelListTail()->SetNext(pObj);
		SetSelListTail(pObj);
	}
	IncTotalSelectedItems();
}

// CBaseDocument commands
