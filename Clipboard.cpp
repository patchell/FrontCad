#include "pch.h"

CClipboard::CClipboard()
{
	m_pHead = 0;
	m_pTail = 0;
	m_TotalClipboardItems = 0;
}

CClipboard::~CClipboard()
{
}


void CClipboard::AddObjectAtHead(CCadObject* pObj)
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
	++m_TotalClipboardItems;
}

void CClipboard::AddObjectAtTail(CCadObject* pObj)
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
	++m_TotalClipboardItems;
}

void CClipboard::RemoveObject(CCadObject* pObj)
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
	pObj->SetParent(0);
	--m_TotalClipboardItems;
}

CCadObject* CClipboard::RemoveFromHead()
{
	CCadObject* pResult;

	pResult = GetHead();
	if (pResult)
		RemoveObject(pResult);
	return pResult;
}

CCadObject* CClipboard::FindObject(ObjectType Type, SubType SubType, UINT SubSubType)
{
	CCadObject* pResult = 0;
	CCadObject* pObj;
	BOOL bLoop = TRUE;

	pObj = GetHead();
	while (pObj && bLoop)
	{
		if (pObj->GetType() == Type &&
			pObj->GetSubType() == SubType &&
			SubSubType > 0)
		{
			bLoop = FALSE;
			pResult = pObj;
		}
		else if (pObj->GetType() == Type && pObj->GetSubType() == SubType)
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

void CClipboard::RemoveAndDestroyALL()
{
	if (GetHead())	//is there anytyhing to destroy?
	{
		CCadObject* pObj;
		CCadObject* pNextObj;

		pObj = GetHead();
		while (pObj)
		{
			pNextObj = pObj->GetNext();
			RemoveObject(pObj);
			delete pObj;
			pObj = pNextObj;
		}
	}
}
