#pragma once


class CFrontCadView;

class CBaseDocument : public CDocument
{
	CString m_csFileName;
	CString csDocName;
	CFrontCadView* m_pDocView;
	//------------- Drawing --------------------
	CCadObject* m_pDocObject;
	//------------------------------------------
	DECLARE_DYNCREATE(CBaseDocument)
public:
	CBaseDocument();
	virtual ~CBaseDocument();
	//---------------------------------
	//---------------------------------
	void SetFileName(CString& csName);
	CString& GetFileName() {return m_csFileName;}
	//---------------------------------
	// Selection
	//---------------------------------
	virtual void UnSelectAll(
		UINT nKinds		//kinds of object to select
	);
	virtual int PointInObjectAndSelect(
		DOUBLEPOINT p,
		CCadObject** ppL,
		CCadObject* pExcludeObject,
		int n,
		UINT nKinds		//kinds of object to select
	);
	void SetDocView(CFrontCadView* pFCV) { m_pDocView = pFCV; }
	CFrontCadView* GetDocView() { return m_pDocView; }
	CCadObject* GetDocObject() { return m_pDocObject; }
	void SetDocObject(CCadObject* pDocObj) { m_pDocObject = pDocObj; }
	//-----------------------------------------------------
	CString& GetDocumentName() { return csDocName; }
	void SetDocumentName(CString& csName) { csDocName = csName; }
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
protected:
	virtual BOOL OnNewDocument();
	DECLARE_MESSAGE_MAP()
};
