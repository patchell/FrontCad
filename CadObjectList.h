#pragma once
class CCadObjectList
{
	CCadObject* m_pHead;
	CCadObject* m_pTail;
	CCadObject* (CCadObject::*m_GetNext)();
public:
	CCadObjectList() {
		m_pHead = 0;
		m_pTail = 0;
		m_GetNext = 0;
	}
	void SetGetNext(CCadObject* (CCadObject::* GetNext)()) {
		m_GetNext = GetNext;
	}
};

