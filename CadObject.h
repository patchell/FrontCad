#pragma once

class CCadPoint;

class CCadObject
{
	//--------------------------------------
	// Object Properties
	//--------------------------------------

	CString m_csName;
	CString m_csDescription;
	UINT m_Id;	//unique ID number
	ObjectType m_Type;		//specifies the type of object
	SubType m_SubType;		//Specifies the subtype of the object
	UINT m_SubSubType;		//Doffertiates between different objects of same subtype
	BOOL m_Selected;	//indicates that object is in the selected state
	BOOL m_bAttributesValid;
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
	// List of Origin Objects Links
	//---------------------------------
	CCadObject* m_pNextOrigin;
	CCadObject* m_pPrevOrigin;
	//------------------------------------
	// Clipboard Linked List
	//------------------------------------
	CCadObject* m_pNextClipBoard;
	CCadObject* m_pPrevClipBoard;
	//------------------------------------
	// Parrent Object
	//------------------------------------
	CCadObject* m_pParentObject;
public:
	//-------------------------------------
	// Object Creation/Destruction Methods
	//-------------------------------------
	CCadObject();
	virtual ~CCadObject();
	virtual BOOL Destroy(CCadObject* pDependentObject) { return TRUE; }
	virtual void SetParent(CCadObject* pP) { m_pParentObject = pP; }
	virtual CCadObject* GetParent() const { return m_pParentObject; }
	UINT GetId() { return m_Id; }
	ObjectType GetType(void) { return m_Type; }
	void SetType(ObjectType type) { m_Type = type; }
	SubType GetSubType() { return m_SubType; }
	void SetSubType(SubType subtype) { m_SubType = subtype; }
	UINT GetSubSubType() { return m_SubSubType; }
	void SetSubSubType(UINT SST) { m_SubSubType = SST; }
	BOOL AttributesAreValid() {
		return m_bAttributesValid;
	}
	void SetAttributesValid() { m_bAttributesValid = TRUE; }
	//------------------------------------
	// Drawing Manipulation Methods
	//------------------------------------
	virtual void Move(CDoubleSize Diff);
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
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale);
	//------------------------------------------
	// Get things about the object
	//------------------------------------------
	virtual BOOL PointInThisObject(DOUBLEPOINT point) { return FALSE; }
	virtual int PointInObjectAndSelect(
		DOUBLEPOINT p,
		CCadObject** ppSelList,
		int index,
		int n
	);
	virtual BOOL IsSelected(void) const { return m_Selected; }
	virtual void SetSelected(BOOL Flag) { m_Selected = Flag; }
	CString& GetName() { return m_csName; }
	void SetName(CString& csName) { m_csName = csName; }
	CString& GetDescription() { return m_csDescription; }
	virtual CString& GetTypeString(void);
	virtual CString& GetObjDescription();
	void CopyObject(CCadObject* pObjDestination);
	virtual CCadObject* CopyObject() { return NULL; }
	virtual CDoubleSize GetSize() { 
		return CDoubleSize(0.0, 0.0);
	};
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState) { return DrawState; }
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState) { return DrawState; }
	virtual void Flip(CCadPoint* Pivot, int Direction){}
	virtual void ProcessZoom(CScale& InchesPerPixel) {}
	//--------------------------------------------------------
	// Methods for managing object properites
	//--------------------------------------------------------
	virtual int EditProperties(void) { return IDCANCEL; }
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
	CCadObject* DeleteObject(CCadObject* pObj) {
		CCadObject* pObjNext;

		pObjNext = pObj->GetNext();
		Destroy(pObj);
		delete pObj;
		return pObjNext;
	}
	CCadObject* FindObject(ObjectType Type, SubType SubType, UINT SubSubType = 0);
	//-------------------------------------------------------------
	//				List of Dependent Children ------------
	//-------------------------------------------------------------
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
	CCadObject* FindDepChildObject(ObjectType Type, SubType SubType, UINT SubSubType = 0);
	//-----------------------------------------------------------------
	//			List of dependent parents 
	//-----------------------------------------------------------------
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
	CCadObject* FindDepParentObject(ObjectType Type, SubType SubType, UINT SubSubType = 0);
	//---------------- Origin List Links -----------------------
	void SetNextOrigin(CCadObject* pObj) { m_pNextOrigin = pObj; }
	CCadObject* GetNextOrigin() { return m_pNextOrigin; }
	void SetPrevOrigin(CCadObject* pObj) { m_pPrevOrigin = pObj; }
	CCadObject* GetPrevOrigin() { return m_pPrevOrigin; }
	//---------------------------------------------------
	// Clip Board Linked List Methods
	//--------------------------------------------------
	CCadObject* GetNextClipBoard() { return m_pNextClipBoard; }
	void SetNextClipBoard(CCadObject* pObj) { m_pNextClipBoard = pObj; }
	CCadObject* GetPrevClipBoard() { return m_pPrevClipBoard; }
	void SetPrevClipBoard(CCadObject* pObj) { m_pPrevClipBoard = pObj; }
};
