#include "pch.h"

CCadBitmap::CCadBitmap():CCadObject(ObjectType::BITMAPTYPE)
{
	m_pBM = 0;
	GetName().Format(_T("Bitmap_%d"), ++m_BitmapCount);
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetBitmapAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
CopyAttributesFrom(&m_CurrentAttributes);
}

CCadBitmap::~CCadBitmap()
{
	if (m_pBM) delete m_pBM;
}

BOOL CCadBitmap::Create(CCadObject* pParent, SubTypes type)
{
	CADObjectTypes Obj;

	CCadObject::Create(pParent, type);
	if (pParent == NULL)
		pParent = this;

	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::VERTEX);
	Obj.pCadPoint->SetSubSubType(1);
	AddObjectAtTail(Obj.pCadObject);

	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::VERTEX);
	Obj.pCadPoint->SetSubSubType(2);
	AddObjectAtTail(Obj.pCadObject);
	return TRUE;
}

void CCadBitmap::Move(CDoubleSize Diff)
{
	//--------------------------------------------------
	//	Move
	//		This Method is used to move the object
	// by the amount that is passed.
	//
	// parameters:
	//	p.......amount to move the object by
	//
	// return value: none
	//--------------------------------------------------
	CCadObject::Move(Diff);
}

void CCadBitmap::Save(CFile * pcfFile, int Indent, int flags)
{
	//--------------------------------------------------
	// Save
	//		This Method save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
}


void CCadBitmap::Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale)
{
	//--------------------------------------------------
	// Draw
	//	This Method draws the document to the device
	// parameters:
	//	pDC.....pointer to the device context
	//	mode....drawing mode
	//	Offset..Offset to draw objects at
	//	Scale..Scale factor to draw objects at
	//
	// return value:none
	//--------------------------------------------------
	CADObjectTypes ObjP1, ObjP2;
	CPen penLine, * ppenOld;
	CBrush brushFill, * pbrushOld;
	CRect rect;
	CDC bmDC;
	CBitmap* bitmapOld;

	if (IsRenderEnabled())
	{
		ObjP1.pCadObject = FindObject(ObjectType::RECT, CCadObject::SubTypes::VERTEX, 1);
		ObjP2.pCadObject = FindObject(ObjectType::RECT, CCadObject::SubTypes::VERTEX, 2);
		ObjP1.pCadPoint->ToPixelRect(ObjP2.pCadPoint, rect, LLHC, Scale);

		switch (mode.PaintMode)
		{
		case MODE::ObjectPaintMode::FINAL:
			bmDC.CreateCompatibleDC(pDC);
			bitmapOld = bmDC.SelectObject(this->m_pBM);
			pDC->StretchBlt(
				rect.left,
				rect.top,
				rect.Width(),
				rect.Height(),
				&bmDC,
				0,
				0,
				GetAttributes().m_BitmapSize.ToCSize().cx,
				GetAttributes().m_BitmapSize.ToCSize().cy,
				SRCCOPY
			);
			bmDC.SelectObject(bitmapOld);
			if (IsSelected())
			{
				penLine.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				ppenOld = pDC->SelectObject(&penLine);
				brushFill.CreateStockObject(NULL_BRUSH);
				pbrushOld = pDC->SelectObject(&brushFill);
				pDC->Rectangle(&rect);
				pDC->SelectObject(pbrushOld);
				pDC->SelectObject(ppenOld);
			}
			break;
		case MODE::ObjectPaintMode::SKETCH:
			penLine.CreatePen(PS_DOT, 1, RGB(255, 0, 0));
			ppenOld = pDC->SelectObject(&penLine);
			pDC->DrawDragRect(&rect, CSize(1, 1), NULL, CSize(1, 1));
			pDC->SelectObject(ppenOld);
			break;
		}
	}
}

BOOL CCadBitmap::IsPointEnclosed(DOUBLEPOINT p)
{
	return 0;
}

BOOL CCadBitmap::PointInThisObject(DOUBLEPOINT point)
{
	BOOL rV;
	CADObjectTypes Obj;

	Obj.pCadObject = FindObject(ObjectType::RECT, CCadObject::SubTypes::RECTSHAPE, 0);
	rV = Obj.pCadRect->PointInThisObject(point);
	return rV;
}

int CCadBitmap::PointInObjectAndSelect(
	DOUBLEPOINT p,
	CCadObject* pExcludeObject,
	CCadObject** ppSelList,
	int index,
	int n,
	UINT nKinds
)
{
	//--------------------------------------------------
	// PointInObjectAndSelect
	//	This Method is used to see if an object can
	// be selected at point p.
	//
	// parameters:
	//	p...........point to check at
	//	Offset......Offset of drawing
	//	ppSelList...pointer to list of selected objects
	//	index.......current index into the selection list
	//	n...........Total number of spaces in selection list
	//
	// return value:
	//	returns true if point is within object
	//	otherwise, false
	//--------------------------------------------------

	if (index < n)
	{
		//---------------------------------------
		// is point in the Arc?
		//---------------------------------------
		if (PointInThisObject(p))
		{
			if (IsItThisKind(nKinds) && DontExclude(pExcludeObject))
			{
				ppSelList[index++] = this;
				ShouldWeSelectThisObjectAndDidIt(nKinds);
			}
		}
		index = CCadObject::PointInObjectAndSelect(
			p,
			pExcludeObject,
			ppSelList,
			index,
			n,
			nKinds
		);
	}
	return index;
}

CString& CCadBitmap::GetTypeString()
{
	//--------------------------------------------------
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csName = _T("Bitmap");
	return csName;
}

CString& CCadBitmap::GetObjDescription()
{
	GetDescription().Format(_T("Base Obj Class"));
	return GetDescription();
}

CCadObject * CCadBitmap::Copy()
{
	//--------------------------------------------------
	// Copy
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadBitmap *pBM = new CCadBitmap;
	pBM->Create(GetParent(), GetSubType());
	CopyObject(pBM);
	return pBM ;
}

void CCadBitmap::CopyAttributes(CCadObject* pToObj)
{
	((CCadBitmap*)pToObj)->CopyAttributesFrom(GetPtrToAttributes());
}


CDoubleSize CCadBitmap::GetSize()
{
	//--------------------------------------------------
	// GetSize
	//	Get the size of the object.  Returns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	CADObjectTypes Obj;

	Obj.pCadObject = FindObject(ObjectType::RECT, CCadObject::SubTypes::RECTSHAPE, 0);

	return Obj.pCadRect->GetSize();
}

int CCadBitmap::Parse(
	CFile* pcfInFile,
	int Token,	// Lookahead Token
	CFileParser* pParser,	// pointer to parser
	int TypeToken// Token type to save object as
)
{
	//--------------------------------------------------
	// Parse
	//	This Method is used to parse this 
	// object out of an input stream
	//
	// parameters:
	//	Token....Token for this object
	//	pLex.....Pointer to Lexer for the data stream
	//
	// return value:
	//	returns lookahead token on success, or
	//			negative value on error
	//--------------------------------------------------
	return Token;
}

void CCadBitmap::CopyAttributesTo(SBitmapAttributes*pAttrib)
{
	//---------------------------------------------------
	//	CopyAttributesTo
	//		This Method is used to copy the
	//	attributes from this object into one pointed
	//	to by the parameter.
	//
	// Parameters:
	//	pAttrb.....pointer to attributes structure to copy into
	//-------------------------------------------------/
	GetAttributes().CopyTo(pAttrib);
}

void CCadBitmap::CopyAttributesFrom(SBitmapAttributes*pAttrib)
{
	//---------------------------------------------------
	//	CopyAttributesFrom
	//		This Method is used to copy the
	//	attributes pointed to by the parameter into
	//	this object
	//
	// Parameters:
	//	pAttrb.....pointer to attributes structure to copy
	//---------------------------------------------------/
	GetAttributes().CopyFrom(pAttrib);
	SetAttributesValid();
}

ObjectDrawState CCadBitmap::ProcessDrawMode(ObjectDrawState DrawState)
{
	//-------------------------------------------------------
	//	ProcessDrawMode
	//		This is the state machine for creating this
	//	object on the screen.  
	//
	//	parameters:
	//		DrawState.Current state of drawing process
	//
	//	Returns:
	//		Next Draw State
	//-------------------------------------------------------
	UINT Id;
	DOUBLEPOINT MousePos = GETVIEW->GetCurrentMousePosition();
	CADObjectTypes ObjP1, ObjP2;

	switch (DrawState)
	{
	case ObjectDrawState::START_DRAWING:
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
		CopyAttributesFrom(&m_CurrentAttributes);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		break;
	case ObjectDrawState::END_DRAWING:
		if (m_AttributesDirty)
		{
			Id = GETVIEW->MessageBoxW(_T("Do you want to keep\nThe current\nAttributes?"), _T("Keep Or Toss"), MB_YESNO);
			if (IDYES == Id)
			{
				m_CurrentAttributes.CopyTo(&m_LastAttributes);
			}
			m_AttributesDirty = FALSE;
		}
		break;
	case ObjectDrawState::SET_ATTRIBUTES:
		Id = EditProperties();
		if (IDOK == Id)
		{
			m_AttributesDirty = TRUE;
			CopyAttributesTo(&m_CurrentAttributes);
		}
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		GETVIEW->EnableAutoScroll(TRUE);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		ObjP1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 1);
		ObjP2.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 2);
		ObjP2.pCadPoint->SetPoint(MousePos);
		ObjP1.pCadPoint->SetPoint(MousePos);
		DrawState = ObjectDrawState::ROTATE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Arrow:Place Rotation Point"));
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		DrawState = ObjectDrawState::PLACE_LBUTTON_UP;
		break;
	case ObjectDrawState::PLACE_LBUTTON_UP:
		ObjP2.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 2);
		ObjP2.pCadPoint->SetPoint(MousePos);
		GETVIEW->EnableAutoScroll(FALSE);
		GetParent()->AddObjectAtTail(this);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Arrow:Locate Arrow Tip Point"));
		GETVIEW->Invalidate();
		break;
	}
	return DrawState;
}

ObjectDrawState CCadBitmap::MouseMove(ObjectDrawState DrawState)
{
	//-------------------------------------------------------
	// MouseMove
	//		This is the state machine for creating this
	//	object on the screen.  This Method is for when
	//	the left mouse is moved.
	//
	//	parameters:
	//		pASV......pointer to view that is creating object
	//		DrawState.Current state of drawing process
	//
	//	Returns:
	//		Next Draw State
	//-------------------------------------------------------
	DOUBLEPOINT MousePos;
	CADObjectTypes Obj;

	switch (DrawState)
	{
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 2);
		Obj.pCadPoint->SetPoint(MousePos);
		break;
	}
	GETVIEW->Invalidate();
	return DrawState;
}

int CCadBitmap::EditProperties()
{
	CDlgBitmapProperties Dlg;
	int Id;

	Dlg.SetBitmapData(this);
	Id = Dlg.DoModal();
	return Id;
}


void CCadBitmap::LoadBitmapImage(CString csPath)
{
	if (m_pBM) delete m_pBM;
	m_pBM = new CMyBitmap;
	m_pBM->LoadImageBitmap(csPath);
	m_csBMFileName = csPath;
	BITMAP bm;
	m_pBM->GetBitmap(&bm);
	GetAttributes().m_BitmapSize = CDoubleSize(int(bm.bmWidth), int(bm.bmHeight));
}

void CCadBitmap::RestoreAspectRatio()
{
	CADObjectTypes Obj;
	double AspectRatioBM;

	Obj.pCadObject = FindObject(ObjectType::RECT, CCadObject::SubTypes::RECTSHAPE, 0);
	AspectRatioBM = GetAttributes().m_BitmapSize.dCY / GetAttributes().m_BitmapSize.dCX;
	Obj.pCadRect->SetHeight(AspectRatioBM * Obj.pCadRect->GetWidth());
	GetAttributes().m_MaintainAspectRatio = TRUE;
}
