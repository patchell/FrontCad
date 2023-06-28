#include "pch.h"


CCadEllipse::CCadEllipse():CCadObject(ObjectType::ELIPSE)
{
	GetName().Format(_T("Elipse_%d"), ++m_ElipseCount);
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetEllipseAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}

CCadEllipse::~CCadEllipse()
{
}

BOOL CCadEllipse::Create(CCadObject* pParent, SubTypes type)
{
	CADObjectTypes Obj;

	CCadObject::Create(pParent, type);
	if (pParent == NULL)
		pParent = this;
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::CENTERPOINT);
	AddObjectAtTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::RECTSHAPE);
	Obj.pCadPoint->SetSubSubType(1);
	AddObjectAtTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::RECTSHAPE);
	Obj.pCadPoint->SetSubSubType(2);
	AddObjectAtTail(Obj.pCadObject);
	return TRUE;
}

void CCadEllipse::Move(CDoubleSize Diff)
{
	//-----------------------------------------------
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

void CCadEllipse::Save(FILE * pO, CLexer::Tokens Token, int Indent, int flags)
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

void CCadEllipse::Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale)
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
	CPen *pOldPen,penLine;
	CBrush *pOldBr, brushFill;
	CRect rect;
	CADObjectTypes ObjP1, ObjP2;
	int Lw;

	if (m_RenderEnable)
	{
		printf("Draw\n");
		ObjP1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::RECTSHAPE, 1);
		ObjP2.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::RECTSHAPE, 2);
		Lw = GETAPP.RoundDoubleToInt(Scale.dSX * GetLineWidth());
		if (Lw < 1) Lw = 1;

		rect.SetRect(
			ObjP1.pCadPoint->ToPixelPoint(LLHC, Scale),
			ObjP2.pCadPoint->ToPixelPoint(LLHC, Scale)
		);
		switch (mode.PaintMode)
		{
		case MODE::ObjectPaintMode::FINAL:
			if (IsSelected())
			{
				penLine.CreatePen(PS_SOLID, Lw, GetAttributes().m_colorLineSelected);
				brushFill.CreateStockObject(NULL_BRUSH);
			}
			else
			{
				penLine.CreatePen(PS_SOLID, Lw, GetLineColor());
				if (GetTransparent())
					brushFill.CreateStockObject(NULL_BRUSH);
				else
					brushFill.CreateSolidBrush(GetFillColor());
			}
			pOldPen = pDC->SelectObject(&penLine);
			pOldBr = pDC->SelectObject(&brushFill);
			pDC->Ellipse(&rect);
			pDC->SelectObject(pOldBr);
			pDC->SelectObject(pOldPen);
			break;
		case MODE::ObjectPaintMode::SKETCH:
			penLine.CreatePen(PS_DOT, 1, GetAttributes().m_colorLineSelected);
			brushFill.CreateStockObject(NULL_BRUSH);
			pOldPen = pDC->SelectObject(&penLine);
			pOldBr = pDC->SelectObject(&brushFill);
			pDC->Ellipse(&rect);
			pDC->Rectangle(&rect);
			pDC->SelectObject(pOldBr);
			pDC->SelectObject(pOldPen);
			break;
		}
	}
}

BOOL CCadEllipse::PointInThisObject(DOUBLEPOINT point)
{
	BOOL rV = FALSE;
	double a, b;
	CADObjectTypes ObjP1, ObjP2, ObjCenter;

	ObjP1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::RECTSHAPE, 1);
	ObjP2.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::RECTSHAPE, 2);
	ObjCenter.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0);
	a = ObjP1.pCadPoint->GetX() - ObjCenter.pCadPoint->GetX();
	b = ObjP1.pCadPoint->GetY() - ObjCenter.pCadPoint->GetY();
	rV = GETAPP.PointInEllipse(a, b, point, DOUBLEPOINT(*ObjCenter.pCadPoint));
	return rV;
}

int CCadEllipse::PointInObjectAndSelect(
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


CString& CCadEllipse::GetTypeString(void)
{
	//----------------------------------------------
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csName = _T("Ellipse");
	return csName;
}

CString& CCadEllipse::GetObjDescription()
{
	GetDescription().Format(_T("Base Obj Class"));
	return GetDescription();
}

CCadObject * CCadEllipse::CopyObject(void)
{
	//-----------------------------------------------
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadEllipse *pCE = new CCadEllipse;
	*pCE = *this;
	return pCE;
}

CDoubleSize CCadEllipse::GetSize()
{
	//---------------------------------------------
	// GetSize
	//	Get the size of the object.  Reutrns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	CADObjectTypes Obj;

	Obj.pCadObject = FindObject(ObjectType::RECT, CCadObject::SubTypes::RECTSHAPE, 0);
	return Obj.pCadRect->GetSize();
}

CLexer::Tokens CCadEllipse::Parse(
	CLexer::Tokens Token, 
	CFileParser* pParser,
	CLexer::Tokens TypeToken
)
{
	//--------------------------------------------
	// Parse
	//	This Method is used to parse this 
	// object out of an input stream
	//
	// parameters:
	//	Token....Look Ahead Token
	//	pLex.....Pointer to Lexer for the data stream
	//	TypeToken..Token that specifies the type of this object
	//
	// return value:
	//	returns lookahead token on success, or
	//			throws an exception
	//--------------------------------------------------
	return Token;
}

void CCadEllipse::CopyAttributesTo(SEllipseAttributes *pAttrib)
{
	//---------------------------------------------------
	//	GetAttributes
	//		This Method is used to copy the
	//	attributes from this object into
	//	an extern attribute structure
	//
	// Parameters:
	//	pAttrb.....pointer to attributes structure to copy into
	//---------------------------------------------------/
	GetAttributes().CopyTo(pAttrib);
}


void CCadEllipse::CopyAttributesFrom(SEllipseAttributes *pAttrib)
{
	//---------------------------------------------------
	//	CopyAttributesFrom
	//		This Method is used to copy the
	//	attributes pointed to by the parameter into
	//	this object
	//
	// Parameters:
	//	pAttrb.....pointer to attributes structure to copy
	//---------------------------------------------------
	GetAttributes().CopyFrom(pAttrib);
	SetAttributesValid();
}

ObjectDrawState CCadEllipse::ProcessDrawMode(ObjectDrawState DrawState)
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
	CCadPoint* pP1, * pP2;

	switch (DrawState)
	{
	case ObjectDrawState::START_DRAWING:
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
		CopyAttributesFrom(&m_CurrentAttributes);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETVIEW->EnableAutoScroll(TRUE);
		GETAPP.UpdateStatusBar(_T("Ellipse::Place First Point Defining Rectangle Shape"));
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
		GETVIEW->EnableAutoScroll(FALSE);
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
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::RECTSHAPE, 1);
		Obj.pCadPoint->SetPoint(MousePos);
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::RECTSHAPE, 2);
		Obj.pCadPoint->SetPoint(MousePos);
		DrawState = ObjectDrawState::PLACE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Ellipse:Place Second Point Defining Shape"));
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		DrawState = ObjectDrawState::PLACE_LBUTTON_UP;
		break;
	case ObjectDrawState::PLACE_LBUTTON_UP:
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::RECTSHAPE, 2);
		pP2 = Obj.pCadPoint;
		Obj.pCadPoint->SetPoint(MousePos);
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::RECTSHAPE, 1);
		pP1 = Obj.pCadPoint;
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0);
		Obj.pCadPoint->SetPoint(GETAPP.CalcCenter(pP1, pP2));
		GetParent()->AddObjectAtTail(this);
		Obj.pCadElispe = new CCadEllipse;
		Obj.pCadElispe->Create(GetParent(), GetSubType());
		GETVIEW->SetObjectTypes(Obj.pCadElispe);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Ellipse::Place First Point Defining Shape"));
		GETVIEW->Invalidate();
		break;
	}
	return DrawState;
}

ObjectDrawState CCadEllipse::MouseMove(ObjectDrawState DrawState)
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
	CADObjectTypes Obj1, Obj2,ObjCenter;

	switch (DrawState)
	{
		case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
			break;
		case ObjectDrawState::PLACE_LBUTTON_DOWN:
			Obj1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::RECTSHAPE, 1);
			Obj2.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::RECTSHAPE, 2);
			ObjCenter.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0);
			Obj2.pCadPoint->SetPoint(MousePos);
			ObjCenter.pCadPoint->SetPoint(GETAPP.CalcCenter(Obj1.pCadPoint, Obj2.pCadPoint));
			break;
	}
	GETVIEW->Invalidate();
	return DrawState;
}


int CCadEllipse::EditProperties()
{
	CDlgEllispeProperties Dlg;
	int Id;

	Dlg.SetEllipse(this);
	Id = Dlg.DoModal();
	return Id;
}
