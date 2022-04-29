// MoveObjects.cpp: implementation of the CMoveObjects class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMoveObjects::CMoveObjects()
{
	m_pHead = 0;
	m_pTail = 0;
	m_Ref = CPoint(0, 0);
	m_nObjs = 0;
}

CMoveObjects::~CMoveObjects()
{

}

void CMoveObjects::AddObject(CCadObject *pObj)
{
	if(m_pHead)
	{
		///--------------------------
		///add object to end of list
		///--------------------------
		m_pTail->SetNext(pObj);
		pObj->SetPrev(m_pTail);
		m_pTail = pObj;
	}
	else	///first object
	{
		m_pHead = pObj;
		m_pTail = pObj;
	}
	m_nObjs++;
}

CCadObject * CMoveObjects::RemoveObject(CCadObject *pLO)
{
	///------------------------------------------
	/// RemoveObject
	///			This Method removes an object
	/// from the move buffer.  This is a DELETE
	///
	///	parameters:
	///		pLO.....pointer to object to remove
	///------------------------------------------
	if(pLO == m_pHead)
	{
		m_pHead = (CCadObject *)m_pHead->GetNext();
		if(m_pHead) m_pHead->SetPrev(0);
	}
	else if (pLO == m_pTail)
	{
		m_pTail = (CCadObject*)m_pTail->GetPrev();
		if(m_pTail)m_pTail->SetNext(0);
	}
	else
	{
		pLO->GetPrev()->SetNext(pLO->GetNext());
		pLO->GetNext()->SetPrev(pLO->GetPrev());
	}
	pLO->SetPrev(0);
	pLO->SetNext(0);
	m_nObjs--;
	return pLO;
}

void CMoveObjects::Draw(CDC *pDC, CDoublePoint& ULHC, CScale& Scale)
{
	CCadObject *pCO = m_pHead;
	MODE mode;
	mode.DrawMode = ObjectDrawMode::SELECTED;
	while(pCO)
	{
		pCO->Draw(pDC,mode, ULHC, Scale);
		pCO = pCO->GetNext();
	}
}

void CMoveObjects::SetRef(CDoublePoint Ref)
{
	CCadObject *pCO = m_pHead;
	while(pCO)
	{
		pCO->Move(Ref);
		pCO = pCO->GetNext();
	}
	m_Ref = Ref;
}

void CMoveObjects::Clear(int nDeleteFlag)
{
	CCadObject *pCO = m_pHead;
	CCadObject *pNext;
	while(pCO && nDeleteFlag)
	{
		pNext = pCO->GetNext();
		delete pCO;
		pCO = pNext;
	}
	this->m_pHead = 0;
	this->m_pTail = 0;
	m_nObjs = 0;
}

// Copy a MoveObjects class
void CMoveObjects::Copy(CMoveObjects & Src)
{
	CCadObject *pObj = Src.GetHead();
	while (pObj)
	{
		AddObject(pObj->CopyObject());
		pObj = pObj->GetNext();
	}
}

void CMoveObjects::Copy(CMoveObjects *pSrc)
{
	CCadObject *pObj = pSrc->GetHead();
	while (pObj)
	{
		AddObject(pObj->CopyObject());
		pObj = pObj->GetNext();
	}
}

// Removes object from front of list
CCadObject * CMoveObjects::RemoveFromHead()
{
	CCadObject *pObj;

	pObj = m_pHead;
	if (pObj)
	{
		m_pHead = pObj->GetNext();
		if(m_pHead)
			m_pHead->SetPrev(0);
		pObj->SetNext(0);
	}
	return pObj;
}


CCadObject * CMoveObjects::RemoveFromTail()
{
	CCadObject *pObj;

	pObj = m_pTail;
	if (pObj)
	{
		m_pTail = pObj->GetPrev();
		if(m_pTail)
			m_pTail->SetNext(0);
		pObj->SetPrev(0);
	}
	return pObj;
}
