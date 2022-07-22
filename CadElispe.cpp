#include "pch.h"


CCadElispe::CCadElispe():CCadObject()
{
	SetType(ObjectType::ELIPSE);
	GetName().Format(_T("Elipse_%d"), ++m_ElipseCount);
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetEllipseAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}

CCadElispe::~CCadElispe()
{
}

void CCadElispe::Create()
{
	CADObjectTypes Obj;

	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create();
	Obj.pCadPoint->SetSubType(SubType::CENTERPOINT);
	Obj.pCadPoint->SetSubSubType(0);
	AddObjectAtChildTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create();
	Obj.pCadPoint->SetSubType(SubType::RECTSHAPE);
	Obj.pCadPoint->SetSubSubType(1);
	AddObjectAtChildTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create();
	Obj.pCadPoint->SetSubType(SubType::RECTSHAPE);
	Obj.pCadPoint->SetSubSubType(2);
	AddObjectAtChildTail(Obj.pCadObject);
}


BOOL CCadElispe::Destroy(CCadObject* pDependentObjects)
{
	BOOL rV = TRUE;
	return rV;
}

void CCadElispe::Move(CDoubleSize Diff)
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

void CCadElispe::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
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

void CCadElispe::Draw(CDC* pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale)
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

	ObjP1.pCadObject = FindChildObject(ObjectType::POINT, SubType::RECTSHAPE, 1);
	ObjP2.pCadObject = FindChildObject(ObjectType::POINT, SubType::RECTSHAPE, 2);
	Lw = GETAPP.RoundDoubleToInt(Scale.m_ScaleX * GetLineWidth());
	if (Lw < 1) Lw = 1;

	rect.SetRect(
		ObjP1.pCadPoint->ToPixelPoint(ULHC, Scale),
		ObjP1.pCadPoint->ToPixelPoint(ULHC, Scale)
	);
	switch (mode.DrawMode)
	{
	case ObjectDrawMode::FINAL:
		penLine.CreatePen(PS_SOLID, Lw, GetLineColor());
		if (GetTransparent())
			brushFill.CreateStockObject(NULL_BRUSH);
		else
			brushFill.CreateSolidBrush(GetFillColor());
		pOldPen = pDC->SelectObject(&penLine);
		pOldBr = pDC->SelectObject(&brushFill);
		pDC->Ellipse(&rect);
		pDC->SelectObject(pOldBr);
		pDC->SelectObject(pOldPen);
		break;
	case ObjectDrawMode::SELECTED:
		penLine.CreatePen(PS_SOLID, Lw, GetAttributes().m_colorLineSelected);
		brushFill.CreateStockObject(NULL_BRUSH);
		pOldPen = pDC->SelectObject(&penLine);
		pOldBr = pDC->SelectObject(&brushFill);
		pDC->Ellipse(&rect);
		pDC->SelectObject(pOldBr);
		pDC->SelectObject(pOldPen);
		break;
	case ObjectDrawMode::SKETCH:
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

BOOL CCadElispe::PointInThisObject(DOUBLEPOINT point)
{
	BOOL rV = FALSE;
	double a, b;
	CADObjectTypes ObjP1, ObjP2, ObjCenter;

	ObjP1.pCadObject = FindChildObject(ObjectType::POINT, SubType::RECTSHAPE, 1);
	ObjP2.pCadObject = FindChildObject(ObjectType::POINT, SubType::RECTSHAPE, 2);
	ObjCenter.pCadObject = FindChildObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
	a = ObjP1.pCadPoint->GetX() - ObjCenter.pCadPoint->GetX();
	b = ObjP1.pCadPoint->GetY() - ObjCenter.pCadPoint->GetY();
	rV = GETAPP.PointInEllipse(a, b, point, DOUBLEPOINT(*ObjCenter.pCadPoint));
	return rV;
}

int CCadElispe::PointInObjectAndSelect(
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
	int ix;

	if (index < n)
	{
		//---------------------------------------
		// is point in the Arc?
		//---------------------------------------
		if (PointInThisObject(p))
		{
			ppSelList[index++] = this;
			ix = CCadObject::PointInObjectAndSelect(
				p,
				ppSelList,
				index,
				n,
				nKinds
			);
			index += ix;
		}
	}
	return index;
}


CString& CCadElispe::GetTypeString(void)
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

CString& CCadElispe::GetObjDescription()
{
	GetDescription().Format(_T("Base Obj Class"));
	return GetDescription();
}

CCadObject * CCadElispe::CopyObject(void)
{
	//-----------------------------------------------
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadElispe *pCE = new CCadElispe;
	*pCE = *this;
	return pCE;
}

CDoubleSize CCadElispe::GetSize()
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

	Obj.pCadObject = FindChildObject(ObjectType::RECT, SubType::RECTSHAPE, 0);
	return Obj.pCadRect->GetSize();
}

DocFileParseToken CCadElispe::Parse(
	DocFileParseToken Token, 
	CLexer *pLex, 
	DocFileParseToken TypeToken
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

void CCadElispe::CopyAttributesTo(SEllipseAttributes *pAttrib)
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


void CCadElispe::CopyAttributesFrom(SEllipseAttributes *pAttrib)
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

ObjectDrawState CCadElispe::ProcessDrawMode(ObjectDrawState DrawState)
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
		Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::RECTSHAPE, 1);
		Obj.pCadPoint->SetPoint(MousePos);
		Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::RECTSHAPE, 2);
		Obj.pCadPoint->SetPoint(MousePos);
		DrawState = ObjectDrawState::PLACE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Ellipse:Place Second Point Defining Shape"));
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		DrawState = ObjectDrawState::PLACE_LBUTTON_UP;
		break;
	case ObjectDrawState::PLACE_LBUTTON_UP:
		Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::RECTSHAPE, 2);
		pP2 = Obj.pCadPoint;
		Obj.pCadPoint->SetPoint(MousePos);
		Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::RECTSHAPE, 1);
		pP1 = Obj.pCadPoint;
		Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
		Obj.pCadPoint->SetPoint(GETAPP.CalcCenter(pP1, pP2));
		GETVIEW->GetDocument()->AddObjectAtTail(this);
		Obj.pCadElispe = new CCadElispe;
		Obj.pCadElispe->Create();
		GETVIEW->SetObjectTypes(Obj.pCadElispe);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Ellipse::Place First Point Defining Shape"));
		GETVIEW->Invalidate();
		break;
	}
	return DrawState;
}

ObjectDrawState CCadElispe::MouseMove(ObjectDrawState DrawState)
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
			Obj1.pCadObject = FindChildObject(ObjectType::POINT, SubType::RECTSHAPE, 1);
			Obj2.pCadObject = FindChildObject(ObjectType::POINT, SubType::RECTSHAPE, 2);
			ObjCenter.pCadObject = FindChildObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
			Obj2.pCadPoint->SetPoint(MousePos);
			ObjCenter.pCadPoint->SetPoint(GETAPP.CalcCenter(Obj1.pCadPoint, Obj2.pCadPoint));
			break;
	}
	GETVIEW->Invalidate();
	return DrawState;
}


int CCadElispe::EditProperties()
{
	CDlgEllispeProperties Dlg;
	int Id;

	Dlg.SetEllipse(this);
	Id = Dlg.DoModal();
	return Id;
}
