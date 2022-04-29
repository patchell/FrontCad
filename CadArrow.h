#pragma once

enum class Arrow{
	TIP,
	TOP,
	END,
	BOT
};

class CCadArrow :public CCadObject
{
	inline static BOOL m_AttributesDirty = FALSE;
	inline static int m_ArrowCount;
	inline static SArrowAttributes m_LastAttributes;
	inline static SArrowAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	SArrowAttributes m_Attrib;
	double m_RotationAngle;
	CDoublePoint m_ArrowTip;
	CDoublePoint m_DefineAngle;
	CDoublePoint m_ArrowPoints[4];
	CPen* m_pPen;
	CBrush* m_pBrFill;
public:
	CCadArrow();
	CCadArrow(CCadArrow& ca);
	virtual ~CCadArrow();
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE* pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	virtual void SetVertex(int v, CDoubleSize sz);
	virtual int GrabPoint(CDoublePoint point);
	virtual void Draw(CDC* pDC, MODE mode, CDoublePoint& ULHC, CScale& Scale);
	virtual BOOL PointInObjectAndSelect(
		CDoublePoint p,
		CCadObject** ppSelList = 0,
		int index = 0,
		int n = 0,
		DrawingCheckSelectFlags flag = DrawingCheckSelectFlags::FLAG_ALL
	);
	virtual CDoublePoint GetReference();
	virtual CDoubleRect& GetRect(CDoubleRect& rect);
	virtual CString& GetTypeString(void);
	CCadArrow operator=(CCadArrow& v);
	virtual CCadObject* CopyObject(void);
	virtual void SetRect(CRect& rect, CPoint P1, CPoint P2, CSize Lw);
	virtual CDoublePoint& GetCenter(CDoublePoint& Center);
	virtual CDoubleSize& GetSize(CDoubleSize& size);
	virtual DocFileParseToken Parse(
		DocFileParseToken Token,
		CLexer* pLex,
		DocFileParseToken TypeToken = DocFileParseToken::ARROW
	);
	void CopyAttributesTo(SArrowAttributes* pAttrb);
	void CopyAttributesFrom(SArrowAttributes* pAttrb);
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	//----------------------------------
	// attributes Methods
	//----------------------------------
	CDoublePoint* GetArrowPoints() { return m_ArrowPoints; }
	virtual int EditProperties();
	SArrowAttributes& GetAttributes() { return m_Attrib; }
	SArrowAttributes* GetAttributesPointer() { return &m_Attrib; }
	CDoublePoint GetArrowTip() { return m_ArrowTip;}
	void SetArrowTip(CDoublePoint Tip);
	CDoublePoint GetDefineAnglePoint() { return m_DefineAngle; }
	void SetDefineAnglePoint(CDoublePoint DAP);
	void ApplyParameters(double L1, double L2, double W);
	CDoublePolygon GetShape() { return GetAttributes().m_PolyPointsShape; }
	COLORREF GetLineColor(void) { GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF c){ GetAttributes().m_colorLine = c; }
	COLORREF GetFillColor(void) { GetAttributes().m_colorFill; }
	void SetFillColor(COLORREF c) { GetAttributes().m_colorFill = c; }
	double GetLineWidth(void) { return GetAttributes().m_LineWidth; }
	void SetLineWidth(double v) { GetAttributes().m_LineWidth = v; }
	CDoublePoint GetArrowPoint(Arrow Point) { 
		CDoublePoint rP;
		switch (Point)
		{
		case Arrow::TIP:
			rP = GetShape().GetPoint(0);
			break;
		case Arrow::END:
			rP = GetShape().GetPoint(2);
			break;
		case Arrow::TOP:
			rP = GetShape().GetPoint(1);
			break;
		case Arrow::BOT:
			rP = GetShape().GetPoint(3);
			break;
		}
		return rP; 
	}
	double GetL1();
	double GetL2();
	double GetW();
	virtual BOOL NeedsAttributes();
	virtual void ClearNeedsAttributes();
	void Recalc();
	//--------------------------------------------
	static void SetRenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	SArrowAttributes* GetLastAttributes() const {
		return &m_LastAttributes;
	}
};

