#pragma once

class CDocumentFrontCad : public CBaseDocument
{
	CDoubleSize m_DocSize;
	CDoubleSize m_PaperSize;
	CLexerFrontCad* m_pLexer;
	DECLARE_DYNCREATE(CDocumentFrontCad)
public:
	CDocumentFrontCad();
	virtual ~CDocumentFrontCad();
	BOOL Create(CArchive& ar);
	virtual int PointInObjectAndSelect(
		DOUBLEPOINT p,
		CCadObject* pExcludeObject,
		CCadObject** ppSelList,
		int n,
		UINT nKinds		//kinds of object to select
	);
	virtual void SetDirty(UINT flag);
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	CCadDrawing* GetDrawing() {
		return (CCadDrawing*)CBaseDocument::GetDocObject();
	}
	void SetDrawing(CCadDrawing* pDrawing) {
		CBaseDocument::SetDocObject(pDrawing);
	}
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

