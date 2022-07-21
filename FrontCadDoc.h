#pragma once

// CFrontCadDoc document

class CFrontCadDoc : public CBaseDocument
{
	CDoubleSize m_DocSize;
	CDoubleSize m_PaperSize;
	DECLARE_DYNCREATE(CFrontCadDoc)
public:
	CFrontCadDoc();
	virtual ~CFrontCadDoc();
	virtual int PointInObjectAndSelect(
		DOUBLEPOINT p,
		CCadObject** ppSelList,
		int n,
		UINT nKinds		//kinds of object to select
	);
	virtual void SetDirty(UINT flag);
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	void SetDocSize(CDoubleSize szDoc) { m_DocSize = szDoc; }
	CDoubleSize& GetDocSize() { return m_DocSize; }
	double GetDocWidth() { return m_DocSize.dCX; }
	double GetDocHieght() { return m_DocSize.dCY; }
	void SetPaperSize(CDoubleSize szPaper) { m_PaperSize = szPaper; }
	CDoubleSize GetPaperSize() { return m_PaperSize; }
protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnModifySwitchreference();
};
