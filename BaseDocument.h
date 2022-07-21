#pragma once


// CBaseDocument class
class CFrontCadView;

class CBaseDocument : public CDocument
{
	CString csDocName;
	CFrontCadView* m_pDocView;
	//------- Drawing Objects --------
	CCadObject* m_pHead;
	CCadObject* m_pTail;
	UINT m_nTotalObjects;
	//----------------------------------
	// Object Selection Variables
	//---------------------------------
	UINT m_nTotalSelections;	//count of selected objects
	CCadObject* m_pSelObjHead;	//list of selected objects
	CCadObject* m_pSelObjTail;	//end of selected objects list
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
	virtual int PointInObjectAndSelect(
		DOUBLEPOINT p,
		CCadObject** ppL, 
		int n,
		UINT nKinds		//kinds of object to select
	) 
	{ 
		return 0;
	};
	void SetDocView(CFrontCadView* pFCV) { m_pDocView = pFCV; }
	CFrontCadView* GetDocView() { return m_pDocView; }
	//-----------------------------------------------
	//-------------- Drawing Objects ----------------
	//-----------------------------------------------
	virtual void AddObjectAtHead(CCadObject* pObj);
	virtual void AddObjectAtTail(CCadObject* pObj);
	virtual void RemoveObject(CCadObject* pObj);
	CCadObject* GetHead() { return m_pHead; }
	void SetHead(CCadObject* pObj) { m_pHead = pObj; }
	CCadObject* GetTail() { return m_pTail; }
	void SetTail(CCadObject* pObj) { m_pTail = pObj; }
	//-----------------------------------------------------
	//----------------- Origin Objects --------------------
	//-----------------------------------------------------
	virtual void AddOriginAtHead(CCadObject* pObj);
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
	CString& GetDocumentName() { return csDocName; }
	void SetDocumentName(CString& csName) { csDocName = csName; }
	//-------------------------------------------------
	// Selection of objects methods
	//-------------------------------------------------
	CCadObject* GetSelListHead() { return m_pSelObjHead; }
	void SetSelListHead(CCadObject* pObj) { m_pSelObjHead = pObj; }
	CCadObject* GetSelListTail() { return m_pSelObjTail; }
	void SetSelListTail(CCadObject* pObj) { m_pSelObjTail = pObj; }
	void AddToSelListHead(CCadObject* pO);
	void AddToSelListTail(CCadObject* pObj);
	void RemoveUnselectedObjects();
	void RemoveSelectedObject(CCadObject* pO);
	CCadObject* RemoveSelectedObjectFromHead();
	int UnSelectAll();
	int SelectAnObject(CCadObject** ppObj, int n, DOUBLEPOINT p);
	UINT GetTotalSelectedItems() { return m_nTotalSelections; }
	void SetTotalSelectedItems(UINT nT) { m_nTotalSelections = nT; }
	UINT IncTotalSelectedItems() { return --m_nTotalSelections; }
	UINT DecTotalSelectedItems() { return --m_nTotalSelections; }
	//-------------------------------------------
	// Clipboard Methods
	//-------------------------------------------
	int CutToClipBoard();
	int CopyToClipBoard();
	int PasteFromClipBoard();
	//---------------------------------
	// Process Zooms
	//---------------------------------
	virtual void ProcessZoom(CScale& InchesPerPixel);
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
