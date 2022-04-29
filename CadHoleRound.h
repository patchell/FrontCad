#pragma once

class CCadHoleRound:public CCadObject
{
	inline static BOOL m_AttributesDirty = FALSE;
	inline static int m_RoundHoleCount;
	inline static SRoundHoleAttributes m_LastAttributes;
	inline static SRoundHoleAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	CPen *m_pPenLine;
	SRoundHoleAttributes m_Attrib;
	CDoublePoint m_Center;
public:
	CCadHoleRound();
	virtual ~CCadHoleRound();
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE * pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	virtual void SetVertex(int v, CDoubleSize sz);
	virtual int GrabPoint(CDoublePoint p);
	virtual void Draw(CDC* pDC, MODE mode, CDoublePoint& ULHC, CScale& Scale);
	virtual int PointInObjectAndSelect(
		CDoublePoint p,
		CCadObject** ppSelList = 0,
		int index = 0,
		int n = 0,
		DrawingCheckSelectFlags flag = DrawingCheckSelectFlags::FLAG_ALL
	);
	virtual CDoublePoint GetReference();
	virtual CDoubleRect& GetRect(CDoubleRect& rect);
	virtual CString& GetTypeString(void);
	CCadHoleRound operator=(CCadHoleRound &v);
	virtual CCadObject * CopyObject(void);
	virtual void SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw);
	virtual void RenderEnable(int e);
	virtual CDoublePoint GetCenter();
	virtual CDoubleSize GetSize();
	virtual DocFileParseToken Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken);
	void CopyAttributesTo(SRoundHoleAttributes *pAttrb);
	void CopyAttributesFrom(SRoundHoleAttributes *pAttrb);
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	virtual int EditProperties(void);
	//---------------------------------------------
	// Object Attributes
	//---------------------------------------------
	SRoundHoleAttributes& GetAttributes() { return m_Attrib; }
	COLORREF GetLineColor() { return GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF color) { GetAttributes().m_colorLine = color; }
	double GetLineWidth() { return GetAttributes().m_LineWidth; }
	void SetLineWidth(double w) { GetAttributes().m_LineWidth; }
	double GetHoleRadius() { return GetAttributes().m_HoleRadius; }
	void SetHoleRadius(double d) { GetAttributes().m_HoleRadius = d; }
	virtual BOOL NeedsAttributes();
	virtual void ClearNeedsAttributes();
	//--------------------------------------------
	static void SetRenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	SRoundHoleAttributes* GetLastAttributes() const {
		return &m_LastAttributes;
	}
};

