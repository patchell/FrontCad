#include "pch.h"

CClipboard::CClipboard():CCadObject(CCadObject::ObjectType::CLIPBOARD)
{
}

CClipboard::~CClipboard()
{
}

void CClipboard::RemoveAndDestroyALL()
{
	CCadObject* pObj;

	pObj = RemoveObjectFromHead();
	while (pObj)
	{
		delete pObj;
		pObj = RemoveObjectFromHead();
	}
}

