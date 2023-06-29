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
	m_SubType = SubTypes::ANY;
	m_SubSubType = 0;
	m_Selected = FALSE;	//selected flag
	m_bAttributesValid = FALSE;
	//----------- Main Drawing Links --------------
	m_pNext = 0;
	m_pPrev = 0;
	m_pHead = 0;
	m_pTail = 0;
	m_nTotalObjects = 0;
	//-------------------------------
	m_pParentObject = 0;
	m_pOrigin = 0;
	//---------------------------
	// Generate Unique ID
	//---------------------------
	m_Id = GETAPP.GetUniqueID();
}

CCadObject::CCadObject(ObjectType Type)
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
	m_Type = Type;
	m_SubType = SubTypes::ANY;
	m_SubSubType = 0;
	m_Selected = FALSE;	//selected flag
	m_bAttributesValid = FALSE;
	//----------- Main Drawing Links --------------
	m_pNext = 0;
	m_pPrev = 0;
	m_pHead = 0;
	m_pTail = 0;
	m_nTotalObjects = 0;
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
	CCadObject* pObj, *pNextObj;

	pObj = GetHead();
	while (pObj)
	{
		pNextObj = pObj->GetNext();
		RemoveObject(pObj);
		delete pObj;
		pObj = pNextObj;
	}
	//TODO implement handling of dependent objects
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
	return pObjNext;
}

int CCadObject::CheckSelected(DOUBLEPOINT p, CCadObject** ppSelList, int& n, int flag)
{
	//--------------------------------------
	// CheckSelected
	//
	// This function is used to see if
	// there are any objects under the
	// point.  This function returns the
	// first 'n' objects it finds.  It does
	// not actually select anything.
	//
	// parameters:
	//		p......point of interest
	//		ppSelList...pointer to an array of selected objects
	//		n.....maximum number of objects to check for
	//		flag..what kind of objects are inlcuded in list
	//				0-All Objects
	//				1-Unselected Objects
	//				2-Selected Objects
	//	returns:
	//		number of objects that are under the point
	//--------------------------------------------
	int count = 0;
	CCadObject* pCO = GetHead();

	while (pCO && ((count < n) || !n))
	{
		if (pCO->CheckSelected(p, ppSelList, n, flag))
		{
			switch (flag)
			{
			case CADDRAWUBG_CHECKSEL_FLAG_ALL:
				if (ppSelList) ppSelList[count] = pCO;
				count++;	///increment count
				break;
			case CADDRAWUBG_CHECKSEL_FLAG_UNSEL:
				if (!pCO->IsSelected())
				{
					if (ppSelList) ppSelList[count] = pCO;
					count++;	///increment count
				}
				break;
			case CADDRAWUBG_CHECKSEL_FLAG_SEL:
				if (pCO->IsSelected())
				{
					if (ppSelList) ppSelList[count] = pCO;
					count++;	///increment count
				}
				break;
			}
		}
		pCO = pCO->GetNext();
	}
	n = n - count;
	return n;
}

void CCadObject::SelectAll(UINT ObjectKind)
{
	CCadObject* pObj;

	if (IsNotSelected() && IsItThisKind(ObjectKind))
	{
		SetSelected(TRUE);
		pObj = GetHead();
		while (pObj)
		{
			pObj->SelectAll(OBJKIND_ALL);
			pObj = pObj->GetNext();
		}
	}
}

void CCadObject::UnSelectAll(UINT ObjectKind)
{
	CCadObject* pObj;

	if (IsSelected() && IsItThisKind(ObjectKind))
	{
		SetSelected(FALSE);
		pObj = GetHead();
		while (pObj)
		{
			pObj->UnSelectAll(OBJKIND_ALL);
			pObj = pObj->GetNext();
		}
	}
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
	pObj = GetHead();
	csType = GetTypeString();
	csSubType = GetSubTypeString(GetSubType());
	printf("%s%s SubType:%s SubSubType:%d\n", 
		GETAPP.IndentString(sI, Indent++),
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

	pObj = GetHead();
	while (pObj)
	{
		pObj->Move(Diff);
		pObj = pObj->GetNext();
	}
}

void CCadObject::Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale)
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
		pObj = GetHead();	//get head of children
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

CString& CCadObject::GetSubTypeString(SubTypes SubTypeEnum)
{
	int i;
	BOOL Loop = TRUE;

	for (i = 0; Loop && SubTypeStringsLUT[i].m_SubType > SubTypes::NONE; ++i)
	{
		if (SubTypeEnum == SubTypeStringsLUT[i].m_SubType)
		{
			Loop = FALSE;
		}
	}
	return SubTypeStringsLUT[i].m_csName;
}

char* CCadObject::GetSubTypeString(char* pDest, SubTypes SubTypeEnum)
{
	return GETAPP.ConvertCStringToChar(pDest, GetSubTypeString(SubTypeEnum));;
}

char* CCadObject::GetCharSubTypeString(char* pDest, SubTypes SubTypeEnum)
{
	CString csST = GetSubTypeString(SubTypeEnum);
	return GETAPP.ConvertCStringToChar(pDest, csST);
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
	pObj = GetHead();
	while (pObj)
	{
		pNew = pObj->CopyObject();
		pObjCopy->AddObjectAtTail(pNew);
		pObj = pObj->GetNext();
	}
}

CCadObject* CCadObject::GetVertex(UINT VertexNumber)
{
	CCadObject* pObj;

	pObj = FindObject(ObjectType::POINT, SubTypes::VERTEX, VertexNumber);
	return pObj;
}


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
	++m_nTotalObjects;
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
	++m_nTotalObjects;
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
	pObj->SetNext(0);
	pObj->SetPrev(0);
	--m_nTotalObjects;
}

CCadObject* CCadObject::RemoveObjectFromHead()
{
	CCadObject* pObj;

	pObj = GetHead();
	SetHead(pObj->GetNext());
	pObj->SetNext(0);
	pObj->SetPrev(0);
	return pObj;
}

CCadObject* CCadObject::RemoveObjectFromTail()
{
	CCadObject* pObj;

	pObj = GetTail();
	SetTail(pObj->GetPrev());
	pObj->SetNext(0);
	pObj->SetPrev(0);
	return pObj;
}

void CCadObject::RemoveAndDestroyAll()
{
	CCadObject* pObj;

	pObj = RemoveObjectFromHead();
	while (pObj)
	{
		pObj->RemoveAndDestroyAll();
		delete pObj;
		pObj = RemoveObjectFromHead();
	}
}


CCadObject* CCadObject::DeleteObject(CCadObject* pObj)
{
	CCadObject* pObjNext;

	pObjNext = pObj->GetNext();	//Get the next child Object
	RemoveObject(pObj);	//Remove pObj from list of children
	delete pObj;				// Say6 Bye Bye
	return pObjNext;			// Return the next object
}

CCadObject* CCadObject::FindObject(ObjectType Type, SubTypes SubType, UINT SubSubType)
{
	// Is this the real one?
	CCadObject* pResult = 0;
	CCadObject* pObj;
	BOOL bLoop = TRUE;

	pObj = GetHead();
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

//---------------------------------------------------
// Selected Object Methods
//---------------------------------------------------

void CCadObject::RemoveAndDestroySelectedObjects()
{
	CCadObject* pObj, * pNextObj;;

	pObj = GetHead();
	while (pObj)
	{
		pNextObj = pObj->GetNext();
		if (pObj->IsSelected())
		{
			pObj->RemoveAndDestroyAll();
			delete pObj;
		}
		pObj = pNextObj;
	}
}

int CCadObject::GetTotalNumberOfSelectedItems()
{
	int rV = 0;
	return rV;
}



