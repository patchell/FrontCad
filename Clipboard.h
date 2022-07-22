#pragma once

class CCadObject;

class CClipboard
{
	CCadObject* m_pHead;
	CCadObject* m_pTail;
	UINT m_TotalClipboardItems;
	DOUBLEPOINT m_Ref;
public:
	CClipboard();
	~CClipboard();
	CCadObject* GetHead() { return m_pHead; }
	void SetHead(CCadObject* pObj) { m_pHead = pObj; }
	CCadObject* GetTail() { return m_pTail; }
	void SetTail(CCadObject* pObj) { m_pTail = pObj; }
	//-- For the Children
	void AddObjectAtHead(CCadObject* pObj);
	void AddObjectAtTail(CCadObject* pObj);
	void RemoveObject(CCadObject* pObj);
	CCadObject* RemoveFromHead();
	CCadObject* FindChildObject(ObjectType Type, SubType SubType, UINT SubSubType);
	void RemoveAndDestroyALL();
	void SetRef(DOUBLEPOINT refpt) { m_Ref = refpt; }
	DOUBLEPOINT GetRef() { return m_Ref; }
};

