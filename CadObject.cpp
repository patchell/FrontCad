/****************************************************************
* CCadObject
*		This is the base class for all of the drawing objects
* They are all the same for this simple reason that any of
* them may exist in any document.  The idea is that they behave
* the same so as to make things just a bit easier on me while
* writing the program.  This class was derived from an earlier
* work that I called FrontCad.  This version is supposedely 
* much more object oriented.
****************************************************************/
#include "pch.h"

CCadObject::CCadObject()
{
	//***************************************************
	// CCadObject
	//	This is the default constructor
	// parameters:
	//	none
	// return value:
	//	none
	//--------------------------------------------------
	m_Type = ObjectType::BASE;
	m_Dirty = FALSE;	// Dirty flag
	m_Selected = FALSE;	//selected flag
	m_pReferenceObj = 0;
	//----------- Main Drawing Links --------------
	m_pNext = 0;
	m_pPrev = 0;
	//--------- Selected object Links -------------
	m_pNextSel = 0;
	m_pPrevSel = 0;
	//---------- Drawing Object Child List -------
	m_pHead = 0;
	m_pTail = 0;
	m_nTotalChildObjects = 0;
	//--------Dependent Child list --------------
	m_pHeadDependentChildren = 0;
	m_pTailDependentChildren = 0;
	m_pNextDependentChild = 0;
	m_pPrevDependentChild = 0;
	m_nTotalDependentChildren = 0;
	// -------Dependent Parent List -------------
	m_pHeadDependentParents = 0;
	m_pTailDependentParents = 0;
	m_pNextDependentParent = 0;
	m_pPrevDependentParent = 0;
	m_nTotalDependentParents = 0;
	//---------------------------
	// Origin List links
	//---------------------------
	m_pNextOrigin = 0;
	m_pPrevOrigin = 0;
	//---------------------------
	// Generate Unique ID
	//---------------------------
	m_Id = GETAPP.GetUniqueID();
}

CCadObject::~CCadObject()
{
	//***************************************************
	// ~CCadObject
	//		This is the base object destructor
	//
	// parameters:
	//	none
	// return value:
	//	none
	//--------------------------------------------------
}

void CCadObject::Draw(CDC* pDC, MODE mode, CDoublePoint& ULHC, CScale& Scale)
{
	//***************************************************
	// Draw
	//	This Method draws the document to the device
	// parameters:
	//	pDC.....pointer to the device context
	//	mode....drawing mode
	//	Offset..Offset to draw objects at
	//	Scale..Scale factor to draw objects at
	//
	// return value:none
	//--------------------------------------------------
}


CString& CCadObject::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csTypeName = CString(_T("CCadOject Base Class"));
	return csTypeName;
}

CCadObject CCadObject::operator=(CCadObject &v)
{
	//***************************************************
	// operator=
	//		Provides the Methodality when one object
	// value is assigned to another
	// parameters:
	//	v......reference to object to get value(s) from
	//
	// return value:this
	//--------------------------------------------------
	return CCadObject();
}

CCadObject * CCadObject::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	return new CCadObject;
}

//-------------------------------------------------
// Child Object List
//-------------------------------------------------

void CCadObject::AddObjectAtHead(CCadObject* pObj)
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
	++m_nTotalChildObjects;
}

void CCadObject::AddObjectAtTail(CCadObject* pObj)
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
	++m_nTotalChildObjects;
}

void CCadObject::RemoveObject(CCadObject* pObj)
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
	--m_nTotalChildObjects;
}

//-----------------------------------------
// Dependent Children lists
//-----------------------------------------

void CCadObject::AddDepChildObjectAtHead(CCadObject* pObj)
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
	++m_nTotalDependentChildren;
}

void CCadObject::AddDepChildObjectAtTail(CCadObject* pObj)
{
	if (GetDependentChildrenHead() == 0)
	{
		SetDependentChildrenHead(pObj);
		SetDependentChildrenTail(pObj);
	}
	else
	{
		pObj->SetPrevDependentChild(GetDependentChildrenTail());
		GetDependentChildrenTail()->SetNextDependentChild(pObj);
		SetDependentChildrenTail(pObj);
	}
	++m_nTotalDependentChildren;
}

void CCadObject::RemoveDepChildObject(CCadObject* pObj)
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
	--m_nTotalDependentChildren;
}

//-----------------------------------------
// Dependent parent list management
//-----------------------------------------
void CCadObject::AddDepParentObjectAtHead(CCadObject* pObj)
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
	++m_nTotalDependentParents;
}

void CCadObject::AddDepParentObjectAtTail(CCadObject* pObj)
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
	++m_nTotalDependentParents;
}

void CCadObject::RemoveDepParentObject(CCadObject* pObj)
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
	--m_nTotalDependentParents;
}
