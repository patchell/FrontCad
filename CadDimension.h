#pragma once

class CCadDimension:public CCadObject
{
	inline static int m_DimensionCount;
	inline static SCadDimAttributes m_LastAttributes;
	inline static SCadDimAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	CString m_csText;
	CDoubleLine m_ExtLine1;		// extension line
	CDoubleLine m_ExtLine2;		// extension line
	CDoubleLine m_DimLine1;		// Dimension line
	CDoubleLine m_DimLine2;		// Dimension line
	CDoublePoint m_P1, m_P2;	//points that are being dimensioned
	CDoublePolygon m_Arrow1, m_Arrow2;	//arrows (really?)
	SCadDimAttributes m_DimensionAttributes;
public:
	CCadDimension();
	virtual ~CCadDimension();
	virtual void OnCreate(void);
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE * pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	virtual void SetVertex(int v, CDoubleSize sz);
	virtual int GrabPoint(CDoublePoint p);
	virtual void Draw(CDC* pDC, MODE mode, CSize Offset = CSize(0, 0), CScale Scale = CScale(0.1, 0.1));
	virtual BOOL PointInObjectAndSelect(CPoint p, CSize Offest = CSize(0, 0), CCadObject ** ppSelList = 0, int index = 0, int n = 0, DrawingCheckSelectFlags flag = DrawingCheckSelectFlags::FLAG_ALL);
	virtual CDoublePoint GetReference();
	virtual void AddObject(CCadObject *pO);
	virtual void RemoveObject(CCadObject *pO);
	virtual CCadObject *GetHead(void);
	virtual int IsDirty(void);
	virtual int IsSelected(void);
	virtual void SetDirty(int d);
	virtual void SetSelected(int Flag = 0);
	virtual void AdjustReference(CDoubleSize Ref);
	virtual CDoubleRect& GetRect(CDoubleRect& rect);
	virtual CString& GetTypeString(void);
	CCadDimension operator=(CCadDimension &v);
	virtual CPoint ScalePoint(CPoint p, int Scale, int ScaleDiv);
	virtual CCadObject * CopyObject(void);
	virtual void SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw);
	CDoublePoint GetCenter();
	virtual void ChangeCenter(CSize p);
	virtual CDoubleSize GetSize();
	virtual void ChangeSize(CSize Sz);
	virtual DocFileParseToken Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken);
	void CopyAttributesFrom(SCadDimAttributes *pAttrib);
	void CopyAttributesTo(SCadDimAttributes* pAttrib);
	virtual BOOL NeedsAttributes();
	virtual void ClearNeedsAttributes();
	int EditProperties();
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	//----------------------------------------------------
	void RenderEnable(int e) { CCadDimension::m_RenderEnable = e; }
	BOOL IsRenderDimensionEnabled() { return CCadDimension::m_RenderEnable; }
	//----------------------------------------------------
	SCadDimAttributes& GetAttributes() { return m_DimensionAttributes; }
	COLORREF GetLineColor() { return GetAttributes().m_colorLine; }
	COLORREF GetTextColor() { return GetAttributes().m_colorText; }
	COLORREF GetTextBkColor() { return GetAttributes().m_colorBKG; }
	double GetLineWidth() { return GetAttributes().m_LineWidth; }
	double GetTextHeight() { return GetAttributes().m_TextHeight; }
	//--------------------------------------------
	static void SetRenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	SCadDimAttributes* GetLastAttributes() const {
		return &m_LastAttributes;
	}
};

