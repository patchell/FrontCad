#pragma once


// CBaseDocument class

class CBaseDocument : public CDocument
{
	CString csDocName;
	CCadObject* m_pHead;
	CCadObject* m_pTail;
	UINT m_nTotalObjects;
	CCadObject* m_pCurrentReference;
	CCadObject* m_pReferenceHead;
	CCadObject* m_pReferenceTail;
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
	virtual void AddObjectAtFront(CCadObject* pObj);
	virtual void AddObjectAtEnd(CCadObject* pObj);
	virtual void RemoveObject(CCadObject* pObj);
	virtual void SetReference(CPoint Ref) {};
	virtual void DrawReference(CDC* pDC, CSize Offset, CScale Scale, CRect& clientrect) {};
	virtual void UpdateReference(CPoint newRef) {};
	virtual void SetAllDirty(void);
	virtual void SetDirty(BOOL f) {}
	CCadObject* GetHead() { return m_pHead; }
	void SetHead(CCadObject* pObj) { m_pHead = pObj; }
	CCadObject* GetTail() { return m_pTail; }
	void SetTail(CCadObject* pObj) { m_pTail = pObj; }
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
