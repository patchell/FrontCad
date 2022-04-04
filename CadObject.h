#pragma once

class CCadObject
{
	//--------------------------------------
	// Object Properties
	//--------------------------------------

	CString m_csName;
	ObjectType m_Type;		//specifies the type of object
	int m_Dirty;	//flag that indicates object needs to be redrawn
	int m_Selected;	//indicates that object is in the selected state
	MODE m_LastMode;	//last display mode object was in
	CCadObject *m_pNext;	//next object in drawing
	CCadObject *m_pPrev;	//previous object in drawing
	CCadObject *m_pNextSel;	//next selected object
	CCadObject* m_pPrevSel;	//previous selected object
public:
	//---------------------------------
	// Object Creation Methods
	//---------------------------------
	CCadObject();
	virtual ~CCadObject();
	CString& GetName() { return m_csName; }
	virtual DocFileParseToken Parse(
		DocFileParseToken Token,
		CLexer*
		pLex
	) {
		return Token;
	}
	ObjectType GetType(void) { return m_Type; }
	void SetType(ObjectType type) { m_Type = type; }
	BOOL IsLastModeSame(MODE m) { 
		BOOL Result = FALSE;
		if (m_LastMode.DrawMode == m.DrawMode &&
			m_LastMode.LinesMode == m.LinesMode &&
			m_LastMode.PointsMode == m.PointsMode
			)
		{
			Result = TRUE;
		}
		return (Result); 
	}
	void SetLastMode(MODE m) {
		m_LastMode.DrawMode = m.DrawMode;
		m_LastMode.LinesMode = m.LinesMode;
		m_LastMode.PointsMode = m.PointsMode;
	}
	MODE GetLastMode(void) { return m_LastMode; }
	//------------------------------------------
	// Linked List Methods
	//------------------------------------------
	CCadObject *GetNext(void) { return m_pNext; }
	void SetNext(CCadObject *pO) { m_pNext = pO; }
	CCadObject *GetPrev(void) { return m_pPrev; }
	void SetPrev(CCadObject *pO) { m_pPrev = pO; }
	CCadObject *GetNextSel(void) { return m_pNextSel; }
	void SetNextSel(CCadObject *pO) { m_pNextSel = pO; }
	CCadObject* GetPrevSel(void) { return m_pPrevSel; }
	void SetPrevSel(CCadObject* pO) { m_pPrevSel = pO; }
	//------------------------------------
	// Drawing Manipulation Methods
	//------------------------------------
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE * pO, DocFileParseToken Token, int Indent=0,int flags=0);
	virtual void SetVertex(int v, CDoubleSize sz);
	virtual int GrabPoint(CDoublePoint p);
	virtual void Draw(CDC* pDC, MODE mode, CSize Offset = CSize(0, 0), CScale Scale = CScale(0.1, 0.1));
	//------------------------------------------
	// Get things about the object
	//------------------------------------------
	virtual int PointInObjectAndSelect(
		CDoublePoint p, 
		CCadObject** ppSelList = 0, 
		int index = 0, 
		int n = 0, 
		DrawingCheckSelectFlags flag = DrawingCheckSelectFlags::FLAG_ALL) {
		return index;
	}
	virtual CDoublePoint GetReference() { return CDoublePoint(0.0, 0.0); };
	virtual int IsDirty(void);
	virtual int IsSelected(void);
	virtual void SetDirty(int d);
	virtual void SetSelected(int Flag = 0);
	virtual void AdjustReference(CDoubleSize Ref);
	virtual CDoubleRect& GetRect(CDoubleRect& rect) {
		return rect; 
	}
	virtual CString& GetTypeString(void);
	virtual CString& GetObjectTypeName(void) {
		return m_csName; 
	}
	CCadObject operator=(CCadObject &v);
	virtual CCadObject * CopyObject(void);
	virtual void Copy(CCadObject* pObj) {}
	virtual void SetRect(CDoubleRect & rect, CDoublePoint P1, CDoublePoint P2, CDoubleSize Lw);
	virtual CDoublePoint& GetCenter(CDoublePoint& point) { 
		point = CDoublePoint(0.0, 0.0);
		return point; 
	};
	virtual void ChangeCenter(CSize p) {};
	virtual CDoubleSize& GetSize(CDoubleSize& size) { 
		size = CDoubleSize(0.0, 0.0);
		return size; 
	};
	virtual void ChangeSize(CDoubleSize Sz){};
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState) { return DrawState; }
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	virtual void Flip(CDoublePoint Pivot, int Direction){}
	virtual void *GetDefaultAttributes() { return GETAPP.GetObjectDefaultAttributes(GetType()); }
	//--------------------------------------------------------
	// Methods for managing object properites
	//--------------------------------------------------------
	virtual int EditProperties(void);
	CString& GetObjectName() {return m_csName;}
	virtual BOOL NeedsAttributes() { return FALSE; }
	virtual void ClearNeedsAttributes() {}
};
