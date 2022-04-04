#pragma once

class CCadBitmap :public CCadObject
{
	inline static int m_ArcCount;
	inline static SBitmapAttributes m_LastAttributes;
	inline static SBitmapAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	CDoublePoint m_P1, m_P2;
	CMyBitmap *m_pBM;
	CString m_csBMFileName;
	SBitmapAttributes m_Attributes;
public:
	CCadBitmap();
	CCadBitmap(CCadBitmap &v);
	virtual ~CCadBitmap();
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE * pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	virtual void SetVertex(int v, CDoubleSize sz);
	virtual int GrabPoint(CDoublePoint p);
	virtual void Draw(CDC* pDC, MODE mode, CSize Offset = CSize(0, 0), CScale Scale = CScale(0.1, 0.1));
	virtual int PointInObjectAndSelect(
		CDoublePoint p, 
		CCadObject ** ppSelList = 0, 
		int index = 0, 
		int n = 0, 
		DrawingCheckSelectFlags flag = DrawingCheckSelectFlags::FLAG_ALL
	);
	virtual CDoublePoint GetReference();
	virtual void AdjustReference(CDoubleSize Ref);
	virtual CDoubleRect& GetRect(CDoubleRect& rect);
	virtual CString& GetTypeString(void);
	CCadBitmap operator=(CCadBitmap &v);
	virtual CCadObject * CopyObject(void);
	virtual CDoublePoint& GetCenter(CDoublePoint& Center);
	virtual void ChangeCenter(CDoubleSize p);
	virtual CDoubleSize& GetSize(CDoubleSize& size);
	virtual void ChangeSize(CDoubleSize Sz);
	virtual DocFileParseToken Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken);
	//----------- Attribute Methodes ------------------
	SBitmapAttributes& GetAttributes() { return m_Attributes; }
	SBitmapAttributes* GetPtrToAttributes() { return &m_Attributes; }
	void CopyAttributesTo(SBitmapAttributes* pAttrib);
	void CopyAttributesFrom(SBitmapAttributes* pAttrib);
	virtual BOOL NeedsAttributes();
	virtual void ClearNeedsAttributes();
	BOOL IsAspectRationMaintained() { return GetAttributes().m_MaintainAspectRatio; }
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	//-----------------------------------------------
	virtual int EditProperties();
	void RestoreAspectRatio();
	void LoadBitmapImage(CString csPath);
	BOOL PtInBitmap(CDoublePoint p);
	CString& GetBitMapFileName() { return m_csBMFileName; }
	//--------------------------------------------
	static void SetRenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	SBitmapAttributes* GetLastAttributes() const {
		return &m_LastAttributes;
	}
};

