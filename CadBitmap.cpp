#include "pch.h"

CCadBitmap::CCadBitmap():CCadObject()
{
	m_pBM = 0;
	SetType(ObjectType::BITMAP);
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

void CCadBitmap::Create()
{
	CADObjectTypes Obj;

	Obj.pCadRect = new CCadRect;
	Obj.pCadRect->Create();
	Obj.pCadRect->SetSubType(SubType::RECTSHAPE);
	AddObjectAtChildTail(Obj.pCadObject);
}

BOOL CCadBitmap::Destroy(CCadObject* pDependentObject)
{
	BOOL rV = TRUE;
	return rV;
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

void CCadBitmap::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
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


void CCadBitmap::Draw(CDC* pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale)
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
	CADObjectTypes Obj;
	CPen penLine, *ppenOld;
	CBrush brushFill, * pbrushOld;
	CRect rect;
	CDC bmDC;
	CBitmap *bitmapOld;

	if (IsRenderEnabled())
	{
		Obj.pCadObject = FindChildObject(ObjectType::RECT, SubType::RECTSHAPE,0);
		rect = Obj.pCadRect->ToCRect(ULHC, Scale);

		switch (mode.DrawMode)
		{
		case ObjectDrawMode::FINAL:
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
			break;
		case ObjectDrawMode::SELECTED:
			penLine.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			ppenOld = pDC->SelectObject(&penLine);
			brushFill.CreateStockObject(NULL_BRUSH);
			pbrushOld = pDC->SelectObject(&brushFill);
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
			pDC->Rectangle(&rect);
			pDC->SelectObject(pbrushOld);
			pDC->SelectObject(ppenOld);
			break;
		case ObjectDrawMode::SKETCH:
			penLine.CreatePen(PS_DOT, 1, RGB(255, 0, 0));
			ppenOld = pDC->SelectObject(&penLine);
			pDC->DrawDragRect(&rect, CSize(1, 1), NULL, CSize(1, 1));
			pDC->SelectObject(ppenOld);
			break;
		}
	}
}

BOOL CCadBitmap::PointInThisObject(DOUBLEPOINT point)
{
	BOOL rV;
	CADObjectTypes Obj;

	Obj.pCadObject = FindChildObject(ObjectType::RECT, SubType::RECTSHAPE, 0);
	rV = Obj.pCadRect->PointInThisObject(point);
	return rV;
}

int CCadBitmap::PointInObjectAndSelect(
	DOUBLEPOINT p,
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
	//	n...........Total number of spaces in slection list
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
			if (IsItThisKind(nKinds))
				ppSelList[index++] = this;
		}
		index = CCadObject::PointInObjectAndSelect(
			p,
			ppSelList,
			index,
			n,
			nKinds
		);
	}
	return index;
}

CString& CCadBitmap::GetTypeString(void)
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

CCadObject * CCadBitmap::CopyObject(void)
{
	//--------------------------------------------------
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadBitmap *pBM = new CCadBitmap;
	*pBM = *this;
	pBM->CopyAttributesFrom(&m_LastAttributes);
	return pBM ;
}


CDoubleSize CCadBitmap::GetSize()
{
	//--------------------------------------------------
	// GetSize
	//	Get the size of the object.  Reutrns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	CADObjectTypes Obj;

	Obj.pCadObject = FindChildObject(ObjectType::RECT, SubType::RECTSHAPE, 0);

	return Obj.pCadRect->GetSize();
}

DocFileParseToken CCadBitmap::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
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
		DrawState = ObjectDrawState::ROTATE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Arrow:Place Rotation Point"));
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		DrawState = ObjectDrawState::PLACE_LBUTTON_UP;
		break;
	case ObjectDrawState::PLACE_LBUTTON_UP:
		GETVIEW->EnableAutoScroll(FALSE);
		GETVIEW->GetDocument()->AddObjectAtTail(this);
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
		Obj.pCadObject = FindChildObject(ObjectType::RECT, SubType::RECTSHAPE, 0);
		Obj.pCadRect->SetSecondPoint(MousePos);
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

	Obj.pCadObject = FindChildObject(ObjectType::RECT, SubType::RECTSHAPE, 0);
	AspectRatioBM = GetAttributes().m_BitmapSize.dCY / GetAttributes().m_BitmapSize.dCX;
	Obj.pCadRect->SetHeight(AspectRatioBM * Obj.pCadRect->GetWidth());
	GetAttributes().m_MaintainAspectRatio = TRUE;
}
