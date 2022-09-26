//----------------------------------------------------------------
// CCadObject
//		This is the base class for all of the drawing objects
// They are all the same for this simple reason that any of
// them may exist in any document.  The idea is that they behave
// the same so as to make things just a bit easier on me while
// writing the program.  This class was derived from an earlier
// work that I called FrontCad.  This version is supposedely 
// much more object oriented.
//----------------------------------------------------------------
#include "pch.h"

CCadObject::CCadObject()
{
	//--------------------------------------------------
	// CCadObject
	//	This is the default constructor
	// parameters:
	//	none
	// return value:
	//	none
	//--------------------------------------------------
	m_CurrentDrawState = ObjectDrawState::NULL_STATE;
	m_Type = ObjectType::BASE;
	m_SubType = SubType::ANY;
	m_SubSubType = 0;
	m_Selected = FALSE;	//selected flag
	m_bAttributesValid = FALSE;
	//----------- Main Drawing Links --------------
	m_pNext = 0;
	m_pPrev = 0;
	//--------- Selected object Links -------------
	m_pNextSel = 0;
	m_pPrevSel = 0;
	//---------- Drawing Object Child List -------
	m_pChildHead = 0;
	m_pChildTail = 0;
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
	// Clipboard Links
	//---------------------------
	m_pNextClipBoard = 0;
	m_pPrevClipBoard = 0;
	//---------------------------
	// Origin List links
	//---------------------------
	m_pNextOrigin = 0;
	m_pPrevOrigin = 0;
	//-------------------------------
	m_pParentObject = 0;
	m_pOrigin = 0;
	//---------------------------
	// Generate Unique ID
	//---------------------------
	m_Id = GETAPP.GetUniqueID();
}

CCadObject::~CCadObject()
{
	//--------------------------------------------------
	// ~CCadObject
	//		This is the base object destructor
	//
	// parameters:
	//	none
	// return value:
	//	none
	//--------------------------------------------------
	CCadObject* pObj;

	pObj = GetChildrenHead();
	while (pObj)
	{
		//----------------------------------------
		// Delete the child object and update
		// pObj with the next object in the list
		// of child objects
		//----------------------------------------
		pObj = DeleteChildObject(pObj);
	}
	pObj = GetDependentChildrenHead();
	while (pObj)
	{
		pObj = pObj->Destroy(pObj);
	}
}

CCadObject* CCadObject::Destroy(CCadObject* pDependentObject)
{
	//--------------------------------------------------------------
	// Destroy
	// 
	// This is all confusing enough that it makes my head spin.
	// If this object has any dependent children, such as a line
	// that has a dimension associated with it, we delete the
	// dimension object along with the line.  Keeps things from
	// getting cluttered up.  This is diffent from ChildObjects in
	// that Child objectsw are part of the object, where a
	// dpendent ofbject just does something with another object
	// 
	// parameters:
	//	pDependentObject....dependent object to delete
	// 
	// returns:
	// TRUE on success, FALSE on fail
	// 
	//--------------------------------------------------------------
	CCadObject* pObjNext = NULL;
	BOOL rV = TRUE;

	if (pDependentObject)	//Any thing to destroy?
	{
		pObjNext = pDependentObject->GetNextDependentChild();
		RemoveDepChildObject(pDependentObject);
		delete pDependentObject;
	}
	return pObjNext;
}

int CCadObject::Print(int Indent)
{
	char* s = new char[256];
	char* s1 = new char[256];
	char* sI = new char[256];
	CCadObject* pObj;
	CString csType;
	CString csSubType;

//	CCadObject* pParent;
//	pParent = GetParent();
//	if (pParent) Indent = pParent->Print(Indent);
	GETAPP.MkIndentString(sI, Indent++);
	pObj = GetChildrenHead();
	csType = GetTypeString();
	csSubType = GetSubTypeString(GetSubType());
	printf("%s%s SubType:%s SubSubType:%d\n", 
		sI, 
		GETAPP.ConvertCStringToChar(s, csType),
		GETAPP.ConvertCStringToChar(s1, csSubType),
		GetSubSubType()
	);
	while (pObj)
	{
		pObj->Print(++Indent);
		pObj = pObj->GetNext();
	}
	delete[] sI;
	delete[] s1;
	delete[] s;
	Indent += 2;
	return Indent;
}

void CCadObject::Move(CDoubleSize Diff)
{
	//--------------------------------------------------
	//	Move
	//		This Method is used to move the object
	// by the point that is passed.
	//
	// parameters:
	//	p.......point to move the object by
	//
	// return value: none
	//--------------------------------------------------
	CCadObject* pObj;

	pObj = GetChildrenHead();
	while (pObj)
	{
		pObj->Move(Diff);
		pObj = pObj->GetNext();
	}
}

void CCadObject::Draw(CDC* pDC, MODE mode, DOUBLEPOINT& ULHC, CScale& Scale)
{
	//--------------------------------------------------
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

int CCadObject::PointInObjectAndSelect(
	DOUBLEPOINT p, 
	CCadObject* pExcludeObject,
	CCadObject** ppSelList,
	int index, 
	int n,
	UINT nKinds
)
{
	//--------------------------------------------------
	// PointInObjectAndSelect
	//	This Method is used to see if an object can
	// be selected at point p.
	//
	// parameters:
	//	p...........point to check at
	//	ppSelList...pointer to list of selected objects
	//	index.......current index into the selection list
	//	n...........Total number of spaces in slection list
	//	nKinds........Determines what sort of objects selected
	//
	// return value:
	//	returns true if point is within object
	//	otherwise, false
	//--------------------------------------------------
	CCadObject* pObj;

	if (index < n)	//Is there room?
	{
		pObj = GetChildrenHead();	//get head of children
		while (pObj)	//take a look at the children
		{
			if (pObj != pExcludeObject)
			{
				index = pObj->PointInObjectAndSelect(
					p,
					pExcludeObject,
					ppSelList,
					index,
					n,
					nKinds
				);
			}
			if (index < n)
				pObj = pObj->GetNext();
			else
				pObj = NULL;
		}
	}
	return index;
}

CString& CCadObject::GetTypeString(void)
{
	//--------------------------------------------------
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csTypeName = CString(_T("BASE"));
	return csTypeName;
}

CString& CCadObject::GetObjDescription()
{
	GetDescription().Format(_T("Base Obj Class"));
	return m_csDescription;
}

void CCadObject::CopyObject(CCadObject* pObjCopy)
{
	//--------------------------------------------------
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------

	CCadObject* pObj;
	CCadObject* pNew;

	pObjCopy->m_Type = m_Type;
	pObjCopy->m_SubType = m_SubType;
	pObjCopy->m_SubSubType = m_SubSubType;
	pObjCopy->m_Selected = m_Selected;
	pObjCopy->m_pParentObject = m_pParentObject;
	//------------------------------
	// copy children
	//------------------------------
	pObj = GetChildrenHead();
	while (pObj)
	{
		pNew = pObj->CopyObject();
		pObjCopy->AddObjectAtChildTail(pNew);
		pObj = pObj->GetNext();
	}
}

CCadObject* CCadObject::GetVertex(UINT VertexNumber)
{
	CCadObject* pObj;

	pObj = FindChildObject(ObjectType::POINT, SubType::VERTEX, VertexNumber);
	return pObj;
}

//-------------------------------------------------
// Child Object List
//-------------------------------------------------

void CCadObject::AddObjectAtChildHead(CCadObject* pObj)
{
	pObj->SetDependentParentHead(this);
	if (GetChildrenHead() == 0)
	{
		SetChildrenHead(pObj);
		SetChildrenTail(pObj);;
	}
	else
	{
		pObj->SetNext(GetChildrenHead());
		GetChildrenHead()->SetPrev(pObj);
		SetChildrenHead(pObj);
	}
	++m_nTotalChildObjects;
}

void CCadObject::AddObjectAtChildTail(CCadObject* pObj)
{
	pObj->SetDependentParentHead(this);
	if (GetChildrenHead() == 0)
	{
		SetChildrenHead(pObj);
		SetChildrenTail(pObj);
	}
	else
	{
		pObj->SetPrev(GetChildrenTail());
		GetChildrenTail()->SetNext(pObj);
		SetChildrenTail(pObj);
	}
	++m_nTotalChildObjects;
}

void CCadObject::RemoveChildObject(CCadObject* pObj)
{
	if (pObj == GetChildrenHead())
	{
		SetChildrenHead(pObj->GetNext());
		if (GetChildrenHead())
			GetChildrenHead()->SetPrev(0);
		else
			SetChildrenTail(0);
	}
	else if (pObj == GetChildrenTail())
	{
		SetChildrenTail(pObj->GetPrev());
		if (GetChildrenTail())
			GetChildrenTail()->SetNext(0);
		else
			SetChildrenHead(0);
	}
	else
	{
		pObj->GetNext()->SetPrev(pObj->GetPrev());
		pObj->GetPrev()->SetNext(pObj->GetNext());
	}
	pObj->SetNext(0);
	pObj->SetPrev(0);
	pObj->SetParent(0);
	--m_nTotalChildObjects;
}

CCadObject* CCadObject::DeleteChildObject(CCadObject* pObj)
{
	CCadObject* pObjNext;

	pObjNext = pObj->GetNext();	//Get the next child Object
	RemoveChildObject(pObj);	//Remove pObj from list of children
	delete pObj;				// Say6 Bye Bye
	return pObjNext;			// Return the next object
}

CCadObject* CCadObject::FindChildObject(ObjectType Type, SubType SubType, UINT SubSubType)
{
	// Is this the real one?
	CCadObject* pResult = 0;
	CCadObject* pObj;
	BOOL bLoop = TRUE;

	pObj = GetChildrenHead();
	while (pObj && bLoop)
	{
		if (pObj->m_Type == Type &&
			pObj->m_SubType == SubType &&
			SubSubType == pObj->m_SubSubType)
		{
			bLoop = FALSE;
			pResult = pObj;
		}
		else
		{
			pObj = pObj->GetNext();
		}
	}
	return pResult;
}

//-----------------------------------------
// Dependent Children lists
//-----------------------------------------

void CCadObject::AddDepChildObjectAtHead(CCadObject* pObj)
{
	if (GetDependentChildrenHead() == 0)
	{
		SetDependentChildrenHead(pObj);
		SetDependentChildrenTail(pObj);;
	}
	else
	{
		pObj->SetNextDependentChild(GetDependentChildrenHead());
		GetDependentChildrenHead()->SetPrevDependentChild(pObj);
		SetDependentChildrenHead(pObj);
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
	if (pObj == GetDependentChildrenHead())
	{
		SetDependentChildrenHead(pObj->GetNextDependentChild());
		if (GetDependentChildrenHead())
			GetDependentChildrenHead()->SetPrevDependentChild(0);
		else
			SetDependentChildrenTail(0);
	}
	else if (pObj == GetDependentChildrenTail())
	{
		SetDependentChildrenTail(pObj->GetPrevDependentChild());
		if (GetDependentChildrenTail())
			GetDependentChildrenTail()->SetNextDependentChild(0);
		else
			SetDependentChildrenHead(0);
	}
	else
	{
		pObj->GetNextDependentChild()->SetPrevDependentChild(pObj->GetPrevDependentChild());
		pObj->GetPrevDependentChild()->SetNextDependentChild(pObj->GetNextDependentChild());
	}
	SetPrevDependentChild(0);
	SetNextDependentChild(0);
	--m_nTotalDependentChildren;
}

CCadObject* CCadObject::FindDepChildObject(ObjectType Type, SubType SubType, UINT SubSubType)
{
	CCadObject* pResult = 0;;
	CCadObject* pObj;
	BOOL bLoop = TRUE;

	pObj = GetDependentChildrenHead();
	while (pObj && bLoop)
	{
		if (pObj->m_Type == Type &&
			pObj->m_SubType == SubType &&
			SubSubType > 0)
		{
			bLoop = FALSE;
			pResult = pObj;
		}
		else if (pObj->m_Type == Type && pObj->m_SubType == SubType)
		{
			bLoop = FALSE;
			pResult = pObj;
		}
		else
		{
			pObj = pObj->GetNextDependentChild();
		}
	}
	return pResult;
}

//-----------------------------------------
// Dependent parent list management
//-----------------------------------------
void CCadObject::AddDepParentObjectAtHead(CCadObject* pObj)
{
	if (GetDependentParentsHead() == 0)
	{
		SetDependentParentHead(pObj);
		SetDependentParentTail(pObj);;
	}
	else
	{
		pObj->SetNextDependentParent(GetDependentParentsHead());
		GetDependentParentsHead()->SetPrevDependentParent(pObj);
		SetDependentParentHead(pObj);
	}
	++m_nTotalDependentParents;
}

void CCadObject::AddDepParentObjectAtTail(CCadObject* pObj)
{
	if (GetDependentParrentsTail() == 0)
	{
		SetDependentParentHead(pObj);
		SetDependentParentHead(pObj);
	}
	else
	{
		pObj->SetPrevDependentParent(GetDependentParrentsTail());
		GetDependentParrentsTail()->SetNextDependentParent(pObj);
		SetDependentParentTail(pObj);
	}
	++m_nTotalDependentParents;
}

void CCadObject::RemoveDepParentObject(CCadObject* pObj)
{
	if (pObj == GetDependentParentsHead())
	{
		SetDependentParentHead(pObj->GetNextDependentParent());
		if (GetDependentParentsHead())
			GetDependentParentsHead()->SetPrevDependentParent(0);
		else
			SetDependentParentTail(0);
	}
	else if (pObj == GetDependentParrentsTail())
	{
		SetDependentParentTail(pObj->GetPrevDependentParent());
		if (GetDependentParrentsTail())
			GetDependentParrentsTail()->SetNextDependentParent(0);
		else
			SetDependentParentHead(0);
	}
	else
	{
		pObj->GetNextDependentParent()->SetPrevDependentParent(pObj->GetPrevDependentParent());
		pObj->GetPrevDependentParent()->SetNextDependentParent(pObj->GetNextDependentParent());
	}
	SetNextDependentParent(0);
	SetPrevDependentParent(0);
	--m_nTotalDependentParents;
}

CCadObject* CCadObject::FindDepParentObject(ObjectType Type, SubType SubType, UINT SubSubType)
{
	CCadObject* pObj = 0 , *pResult = 0;
	BOOL bLoop = TRUE;

	pObj = GetDependentParentsHead();
	while (pObj && bLoop)
	{
		if (pObj->m_Type == Type &&
			pObj->m_SubType == SubType &&
			SubSubType > 0)
		{
			bLoop = FALSE;
			pResult = pObj;
		}
		else if (pObj->m_Type == Type && pObj->m_SubType == SubType)
		{
			bLoop = FALSE;
			pResult = pObj;
		}
		else
		{
			pObj = pObj->GetNextDependentParent();
		}
	}
	return pResult;
}


