#pragma once

// CFrontCadDoc document

class CFrontCadDoc : public CBaseDocument
{
	CDoubleSize m_DocSize;
	DECLARE_DYNCREATE(CFrontCadDoc)
public:
	CFrontCadDoc();
	virtual ~CFrontCadDoc();
	virtual UINT PointInObjectAndSelect(
		CDoublePoint p,
		CCadObject** ppSelList = 0,
		int n = 0,
		DrawingCheckSelectFlags flag = DrawingCheckSelectFlags::FLAG_ALL
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
	CDoubleSize GetDocSize() { return m_DocSize; }
protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};