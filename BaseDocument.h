#pragma once


// CBaseDocument class

class CBaseDocument : public CDocument
{
	CString csDocName;
	//------- Drawing Objects --------
	CCadObject* m_pHead;
	CCadObject* m_pTail;
	UINT m_nTotalObjects;
	//------------ Origin Objects --------------
	CCadObject* m_pCurrentOrigin;
	CCadObject* m_pOriginHead;
	CCadObject* m_pOriginTail;
	UINT m_nTotalOrigins;
	//------------------------------------------
	DECLARE_DYNCREATE(CBaseDocument)
public:
	CBaseDocument();
	virtual ~CBaseDocument();
	virtual UINT PointInObjectAndSelect(
		CDoublePoint p, 
		CCadObject** ppL, 
		int n, 
		DrawingCheckSelectFlags flag) 
	{ 
		return 0; 
	};
	//-------------- Drawing Objects ----------------
	virtual void AddObjectAtFront(CCadObject* pObj);
	virtual void AddObjectAtEnd(CCadObject* pObj);
	virtual void RemoveObject(CCadObject* pObj);
	CCadObject* GetHead() { return m_pHead; }
	void SetHead(CCadObject* pObj) { m_pHead = pObj; }
	CCadObject* GetTail() { return m_pTail; }
	void SetTail(CCadObject* pObj) { m_pTail = pObj; }
	//----------------- Origin Objects --------------------
	virtual void AddOriginAtFront(CCadObject* pObj);
	virtual void AddOriginAtTail(CCadObject* pObj);
	virtual void RemoveOrigin(CCadObject* pObj);
	void SetOriginHead(CCadObject* pObj) { m_pOriginHead = pObj; }
	CCadObject* GetOriginHead() { return m_pOriginHead; }
	void SetOriginTail(CCadObject* pObj) { m_pOriginTail = pObj; }
	CCadObject* GetOriginTail() { return m_pOriginTail; }

	CCadObject* GetCurrentOrigin() {
		return m_pCurrentOrigin;
	}
	void SetCurrentOrigin(CCadObject* pObj) { m_pCurrentOrigin = pObj; }
	//-----------------------------------------------------
	virtual void SetAllDirty(void);
	virtual void SetDirty(BOOL f) {}
	CString& GetDocumentName() { return csDocName; }
	void SetDocumentName(CString& csName) { csDocName = csName; }

#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
public:
protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
