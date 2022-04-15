#pragma once

class CCadObject
{
	//--------------------------------------
	// Object Properties
	//--------------------------------------

	CString m_csName;
	UINT m_Id;	//unique ID number
	ObjectType m_Type;		//specifies the type of object
	BOOL m_Dirty;	//flag that indicates object needs to be redrawn
	BOOL m_Selected;	//indicates that object is in the selected state
	MODE m_LastMode;	//last display mode object was in
	//------------ Links for Main List of Drawing Objects ------------------
	CCadObject *m_pNext;	//next object in drawing
	CCadObject *m_pPrev;	//previous object in drawing
	//------------ Links for List of Selected Objects --------------------------------
	CCadObject *m_pNextSel;	//next selected object
	CCadObject* m_pPrevSel;	//previous selected object
	//----- child object List for compound objects -----------------------
	CCadObject* m_pHead;	//head pointer of child object 
	CCadObject* m_pTail;	//tail pointer of child object 
	UINT m_nTotalChildObjects;	//total number of child objects
	//------------------------------------------------
	//             dependent objects  
	//------------------------------------------------
	//----------- Dependent Children ----------------
	CCadObject* m_pHeadDependentChildren;
	CCadObject* m_pTailDependentChildren;
	CCadObject* m_pNextDependentChild;
	CCadObject* m_pPrevDependentChild;
	UINT m_nTotalDependentChildren;
	//---------- Dependent Parent List -------------
	CCadObject* m_pHeadDependentParents;
	CCadObject* m_pTailDependentParents;
	CCadObject* m_pNextDependentParent;
	CCadObject* m_pPrevDependentParent;
	UINT m_nTotalDependentParents;
	//---------------------------------
	// List of Referece Objects Links
	//---------------------------------
	CCadObject* m_pNextRef;
	CCadObject* m_pPrevRef;
	//------------------------------------
	// reference object
	//------------------------------------
	CCadObject* m_pReferenceObj;
public:
	//-------------------------------------
	// Object Creation/Destruction Methods
	//-------------------------------------
	CCadObject();
	virtual ~CCadObject();
	virtual BOOL OnCreate(CCadObject* pRefObj) {
		m_pReferenceObj = pRefObj;
		return TRUE;
	}
	virtual BOOL Destroy(CCadObject* pDependentObject) { return TRUE; }
	CCadObject* GetReferenceObject() { return m_pReferenceObj; }
	CString& GetName() { return m_csName; }
	UINT GetId() { return m_Id; }
	ObjectType GetType(void) { return m_Type; }
	void SetType(ObjectType type) { m_Type = type; }
	//------------------------------------
	// Drawing Manipulation Methods
	//------------------------------------
	virtual void Move(CDoubleSize Diff) {}
	virtual int GrabPoint(CDoublePoint p) { return -1; };
	virtual void SetVertex(int VertexId, CDoublePoint p) {};
	//------------------------------------
	// load and save files
	//------------------------------------
	virtual void Save(FILE* pO, DocFileParseToken Token, int Indent = 0, int flags = 0) {}
	virtual DocFileParseToken Parse(
		DocFileParseToken Token,
		CLexer* pLex
	)
	{
		return Token;
	}
	//-----------------------------------------
	// Draw The Drawing
	//-----------------------------------------
	virtual void Draw(CDC* pDC, MODE mode, CSize Offset = CSize(0, 0), CScale Scale = CScale(0.1, 0.1));
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
	virtual BOOL IsDirty(void) { return m_Dirty; };
	virtual void SetDirty(BOOL d) { m_Dirty = d; }
	virtual BOOL IsSelected(void) { return m_Selected; }
	virtual void SetSelected(BOOL Flag) { m_Selected = Flag; }
	virtual CDoubleRect& GetRect(CDoubleRect& rect) {
		return rect; 
	}
	virtual CString& GetTypeString(void);
	CCadObject operator=(CCadObject &v);
	virtual CCadObject * CopyObject(void);
	virtual void Copy(CCadObject* pObj) {}
	virtual CDoublePoint& GetCenter(CDoublePoint& point) { 
		point = CDoublePoint(0.0, 0.0);
		return point; 
	};
	virtual CDoubleSize& GetSize(CDoubleSize& size) { 
		size = CDoubleSize(0.0, 0.0);
		return size; 
	};
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState) { return DrawState; }
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState) { return DrawState; }
	virtual void Flip(CDoublePoint Pivot, int Direction){}
	//--------------------------------------------------------
	// Methods for managing object properites
	//--------------------------------------------------------
	virtual int EditProperties(void) { return IDCANCEL; }
	virtual BOOL NeedsAttributes() { return FALSE; }
	virtual void ClearNeedsAttributes() {}
	//------------------------------------------
	// Linked List Methods
	// -----------------------------------------
	// Main Links
	//------------------------------------------
	CCadObject* GetNext(void) { return m_pNext; }
	void SetNext(CCadObject* pO) { m_pNext = pO; }
	CCadObject* GetPrev(void) { return m_pPrev; }
	void SetPrev(CCadObject* pO) { m_pPrev = pO; }
	//-------- Selected Object Links ----------------
	CCadObject* GetNextSel(void) { return m_pNextSel; }
	void SetNextSel(CCadObject* pO) { m_pNextSel = pO; }
	CCadObject* GetPrevSel(void) { return m_pPrevSel; }
	void SetPrevSel(CCadObject* pO) { m_pPrevSel = pO; }
	//------------------------------------------------------
	// Manage list of child drawing objects
	//------------------------------------------------------
	CCadObject* GetHead() { return m_pHead; }
	void SetHead(CCadObject* pObj) { m_pHead = pObj; }
	CCadObject* GetTail() { return m_pTail; }
	void SetTail(CCadObject* pObj) { m_pTail = pObj; }
	//-- For the Children
	void AddObjectAtHead(CCadObject* pObj);
	void AddObjectAtTail(CCadObject* pObj);
	void RemoveObject(CCadObject* pObj);
	//------- Manage List of Dependent Children ------------
	CCadObject* GetDependentChildrenHead() { return m_pHeadDependentChildren; }
	void SetDependentChildrenHead(CCadObject* pObj) { m_pHeadDependentChildren = pObj; }
	CCadObject* GetDependentChildrenTail() { return m_pTailDependentChildren; }
	void SetDependentChildrenTail(CCadObject* pObj) { m_pTailDependentChildren = pObj; }
	//--Links--
	CCadObject* GetNextDependentChild() { return m_pNextDependentChild; }	//get next
	void SetNextDependentChild(CCadObject* pObj) { m_pNextDependentChild = pObj; } //set next
	CCadObject* GetPrevDependentChild() { return m_pPrevDependentChild; }	//get previous
	void SetPrevDependentChild(CCadObject* pObj) { m_pPrevDependentChild = pObj; }
	//-- Manage List
	void AddDepChildObjectAtHead(CCadObject* pObj);
	void AddDepChildObjectAtTail(CCadObject* pObj);
	void RemoveDepChildObject(CCadObject* pObj);
	//------- Manage list of dependent parents ------------------------
	CCadObject* GetDependentParentsHead() { return m_pHeadDependentParents; }
	void SetDependentParentHead(CCadObject* pH) { m_pHeadDependentParents = pH; }
	CCadObject* GetDependentParrentsTail() { return m_pTailDependentParents; }
	void SetDependentParentTail(CCadObject* pT) { m_pTailDependentParents = pT; }
	//-- Links --
	CCadObject* GetNextDependentParent() { return m_pNextDependentParent; }
	void SetNextDependentParent(CCadObject* pN) { m_pNextDependentParent = pN; }
	CCadObject* GetPrevDependentParent() { return m_pPrevDependentParent; }
	void SetPrevDependentParent(CCadObject* pP) { m_pPrevDependentParent = pP; }
	//-- Manage List --
	void AddDepParentObjectAtHead(CCadObject* pObj);
	void AddDepParentObjectAtTail(CCadObject* pObj);
	void RemoveDepParentObject(CCadObject* pObj);
};
