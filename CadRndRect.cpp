#include "pch.h"

CCadRndRect::CCadRndRect():CCadObject(ObjectType::ROUNDEDRECT)
{
	GetName().Format(_T("RoundedRect_%d"), ++m_RoundedRectCount);
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetRoundedRectangleAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}


CCadRndRect::~CCadRndRect()
{
}

BOOL CCadRndRect::Create(CCadObject* pParent, SubTypes type)
{
	CADObjectTypes Obj;
	int i;

	CCadObject::Create(pParent, type);
	if (pParent == NULL)
		pParent = this;
	for (i = 0; i < 2; ++i)
	{
		Obj.pCadPoint = new CCadPoint;
		Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::VERTEX);
		Obj.pCadPoint->SetSubSubType(i + 1);
		AddObjectAtTail(Obj.pCadObject);
	}
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::CENTERPOINT);
	AddObjectAtTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::CORNER_RADIUS);
	AddObjectAtTail(Obj.pCadObject);
	return TRUE;
}

void CCadRndRect::Move(CDoubleSize Diff)
{
	//---------------------------------------------------
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

void CCadRndRect::Save(FILE * pO, CLexer::Tokens Token, int Indent, int flags)
{
	//---------------------------------------------------
	// Save
	//		This Method save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
}

void CCadRndRect::Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale)
{
	//---------------------------------------------------
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
	CPen *pOldPen, penLine;
	CBrush* pOldBr, brushFill;
	CADObjectTypes ObjP1, ObjP2, ObjCornerRadius;
	CRect rect;
	int Lw;

	if (IsRenderEnabled())
	{
		Lw = GETAPP.RoundDoubleToInt(GetAttributes().m_LineWidth * Scale.dSX);
		if (Lw < 1)
			Lw = 1;
		ObjP1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 1);
		ObjP2.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 2);
		ObjCornerRadius.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CORNER_RADIUS, 0);
		CreateThePen(mode, &penLine, Lw);
		CreateTheBrush(mode, &brushFill);
		pOldPen = pDC->SelectObject(&penLine);
		pOldBr = pDC->SelectObject(&brushFill);
		rect.SetRect(
			ObjP1.pCadPoint->ToPixelPoint(LLHC, Scale),
			ObjP2.pCadPoint->ToPixelPoint(LLHC, Scale)
		);
		switch (mode.PaintMode)
		{
		case MODE::ObjectPaintMode::FINAL:
		case MODE::ObjectPaintMode::SKETCH:
			pDC->RoundRect(&rect, ObjCornerRadius.pCadPoint->ToPixelPoint(LLHC,Scale));
			break;
		}
		pDC->SelectObject(pOldBr);
		pDC->SelectObject(pOldPen);
	}
}

BOOL CCadRndRect::PointInThisObject(DOUBLEPOINT point)
{
	return 0;
}

int CCadRndRect::PointInObjectAndSelect(
	DOUBLEPOINT p,
	CCadObject* pExcludeObject,
	CCadObject** ppSelList,
	int index,
	int n,
	UINT nKinds
)
{
	//---------------------------------------------------
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

CString& CCadRndRect::GetTypeString(void)
{
	//---------------------------------------------------
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csName = _T("Rounded Rectangle");
	return csName;
}

CString& CCadRndRect::GetObjDescription()
{
	CADObjectTypes Obj;

	Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 1);
	GetDescription().Format(_T("Rnd Rect(%7.3lf,%7.3lf)"), Obj.pCadPoint->GetX(), Obj.pCadPoint->GetY());
	return GetDescription();
}

CCadObject * CCadRndRect::CopyObject(void)
{
	//---------------------------------------------------
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadRndRect *pCR = new CCadRndRect;
	pCR->Create(GetParent(), GetSubType());
	CCadObject::CopyObject(pCR);
	return pCR;
}

CDoubleSize CCadRndRect::GetSize()
{
	//---------------------------------------------------
	// GetSize
	//	Get the size of the object.  Reutrns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	CADObjectTypes ObjP1, ObjP2;
	double CX, CY;

	ObjP1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 1);
	ObjP2.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 2);
	CX = abs(ObjP2.pCadPoint->GetX() - ObjP1.pCadPoint->GetX());
	CY = abs(ObjP2.pCadPoint->GetY() - ObjP1.pCadPoint->GetY());
	return CDoubleSize(CX,CY);
}

CLexer::Tokens CCadRndRect::Parse(
	CLexer::Tokens Token,	// Lookahead Token
	CFileParser* pParser,	// pointer to parser
	CLexer::Tokens TypeToken// Token type to save object as
)
{
	//---------------------------------------------------
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

void CCadRndRect::CopyAttributesTo(SRoundedRectAttributes* pAttrib)
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

void CCadRndRect::CopyAttributesFrom(SRoundedRectAttributes*pAttrib)
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

int CCadRndRect::EditProperties()
{
	int Id;
	CDlgRndRectProperties Dlg;
	Dlg.SetRndRect(this);
	Id = Dlg.DoModal();
	return Id;
}

ObjectDrawState CCadRndRect::ProcessDrawMode(ObjectDrawState DrawState)
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
	CADObjectTypes Obj;

	switch (DrawState)
	{
	case ObjectDrawState::START_DRAWING:
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
		CopyAttributesFrom(&m_CurrentAttributes);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETVIEW->EnableAutoScroll(TRUE);
		GETAPP.UpdateStatusBar(_T("Rounded Rectangle:Place First Point"));
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
			CopyAttributesTo(&m_CurrentAttributes);
			m_AttributesDirty = TRUE;
		}
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		GETVIEW->Invalidate();
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 1);
		Obj.pCadPoint->SetPoint(MousePos);
		DrawState = ObjectDrawState::PLACE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Rounded Rectangle:Place Second Point"));
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		DrawState = ObjectDrawState::PLACE_LBUTTON_UP;
		break;
	case ObjectDrawState::PLACE_LBUTTON_UP:
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 2);
		Obj.pCadPoint->SetPoint(MousePos);
		DrawState = ObjectDrawState::PLACE_RADIUS_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Rounded Rectangle:Place Corner Radius Point"));
		GETVIEW->Invalidate();
		break;
	case ObjectDrawState::PLACE_RADIUS_LBUTTON_DOWN:
		DrawState = ObjectDrawState::PLACE_RADIUS_LBUTTON_UP;
		break;
	case ObjectDrawState::PLACE_RADIUS_LBUTTON_UP:
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CORNER_RADIUS, 0);
		Obj.pCadPoint->SetPoint(MousePos);
		GETVIEW->EnableAutoScroll(FALSE);
		GetParent()->AddObjectAtTail(this);
		GETVIEW->SetObjectTypes(new CCadRndRect);
		GETAPP.UpdateStatusBar(_T("Rounded Rectangle:Place First Point"));
		GETVIEW->Invalidate();
		break;
	}
	return DrawState;
}


ObjectDrawState CCadRndRect::MouseMove(ObjectDrawState DrawState)
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
	DOUBLEPOINT MousePos = GETVIEW->GetCurrentMousePosition();
	CADObjectTypes Obj;

	switch (DrawState)
	{
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 1);
		Obj.pCadPoint->SetPoint(MousePos);
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 2);
		Obj.pCadPoint->SetPoint(MousePos);
	case ObjectDrawState::PLACE_RADIUS_LBUTTON_DOWN:
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CORNER_RADIUS, 0);
		Obj.pCadPoint->SetPoint(MousePos);
		break;
	}
	GETVIEW->Invalidate();
	return DrawState;
}


COLORREF CCadRndRect::CreateThePen(MODE mode, CPen* pen, int Lw)
{
	COLORREF rColor = RGB(192, 192, 192);

	switch (mode.PaintMode)
	{
	case MODE::ObjectPaintMode::FINAL:
		if (IsSelected())
		{
			rColor = GetAttributes().m_colorLineSelected;
			pen->CreatePen(PS_SOLID, Lw, rColor);
		}
		else
		{
			rColor = GetAttributes().m_colorLine;
			pen->CreatePen(PS_SOLID, Lw, rColor);
		}
		break;
	case MODE::ObjectPaintMode::SKETCH:
		rColor = GetAttributes().m_colorLineSelected;
		pen->CreatePen(PS_DOT, Lw, rColor);
		break;
	}

	return rColor;
}

void CCadRndRect::CreateTheBrush(MODE mode, CBrush* brushFill)
{
	switch (mode.PaintMode)
	{
	case MODE::ObjectPaintMode::FINAL:
		if (IsSelected())
			brushFill->CreateStockObject(NULL_BRUSH);
		else
			brushFill->CreateSolidBrush(GetAttributes().m_colorFill);
		break;
	case MODE::ObjectPaintMode::SKETCH:
		brushFill->CreateStockObject(NULL_BRUSH);
		break;
	}
}

