#include "pch.h"

CCadOrigin::CCadOrigin():CCadObject()
{
	SetType(ObjectType::ORIGIN);
	GetName().Format(_T("Origin_%d"), ++m_OriginCount);
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetOriginAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}

CCadOrigin::~CCadOrigin()
{
}


void CCadOrigin::Create()
{
	CADObjectTypes Obj;

	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create();
	Obj.pCadPoint->SetSubType(SubType::ORIGIN_LOCATION);
	Obj.pCadPoint->SetSubSubType(0);
	AddObjectAtTail(Obj.pCadObject);
}

BOOL CCadOrigin::Destroy(CCadObject* pDependentObject)
{
	BOOL rV = TRUE;
	return rV;
}

void CCadOrigin::Move(CDoubleSize Diff)
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

DOUBLEPOINT CCadOrigin::GetCenterPoint()
{
	CADObjectTypes Obj;

	Obj.pCadObject = FindObject(ObjectType::POINT, SubType::ORIGIN_LOCATION, 0);
	return DOUBLEPOINT(*Obj.pCadPoint);
}

void CCadOrigin::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
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
void CCadOrigin::Draw(CDC* pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale)
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
	CCadObject* pObj = GetHead(); 
	CPen LinePen, * OldPen = 0;
	CBrush fillBrush, * oldBrush = 0;
	CADObjectTypes Obj;
	int LineWidth;
	CRect rect;
	int rectHalfWidth, CrossHairLen;
	CPoint pointUL, pointLR, pointCenter;;
	double Radius;
	//---------------------------------------
	// Draw this Origin Object
	//---------------------------------------

	if (IsRenderEnabled())
	{
		LineWidth = GETAPP.RoundDoubleToInt(GetAttributes().m_LineWidth * Scale.GetScaleX());
		if (LineWidth < 1)
			LineWidth = 1;
		Radius = GetAttributes().m_Radius * Scale.GetScaleX();
		CrossHairLen = GETAPP.RoundDoubleToInt(GetAttributes().m_Radius * 1.50 * Scale.GetScaleX());
		if (CrossHairLen < 15)
			CrossHairLen = 15;
		rectHalfWidth = GETAPP.RoundDoubleToInt(Radius);
		if (rectHalfWidth < 10)
		{
			rectHalfWidth = 10;
			Radius = double(rectHalfWidth);
		}
		Obj.pCadObject = FindObject(ObjectType::POINT, SubType::ORIGIN_LOCATION, 0);
		pointLR = Obj.pCadPoint->ToPixelPoint(ULHC,Scale) + CSize(rectHalfWidth, rectHalfWidth);
		pointUL = Obj.pCadPoint->ToPixelPoint(ULHC, Scale) - CSize(rectHalfWidth, rectHalfWidth);
		rect.SetRect(pointUL, pointLR);
		rect.NormalizeRect();
		switch (mode.DrawMode)
		{
		case ObjectDrawMode::FINAL:
			LinePen.CreatePen(PS_SOLID, LineWidth, GetAttributes().m_colorLine);
			OldPen = pDC->SelectObject(&LinePen);
			break;
		case ObjectDrawMode::SELECTED:
			LinePen.CreatePen(PS_SOLID, LineWidth, GetAttributes().m_colorLine);
			OldPen = pDC->SelectObject(&LinePen);
			break;
		case ObjectDrawMode::SKETCH:
			LinePen.CreatePen(PS_DOT, LineWidth, GetAttributes().m_colorLine);
			OldPen = pDC->SelectObject(&LinePen);
			break;
		}
		fillBrush.CreateStockObject(NULL_BRUSH);
		oldBrush = pDC->SelectObject(&fillBrush);
		pDC->Ellipse(&rect);
		pointCenter = Obj.pCadPoint->ToPixelPoint(ULHC, Scale);
		pDC->MoveTo(pointCenter.x + CrossHairLen,pointCenter.y);
		pDC->LineTo(pointCenter.x - CrossHairLen, pointCenter.y);
		pDC->MoveTo(pointCenter.x,pointCenter.y + CrossHairLen);
		pDC->LineTo(pointCenter.x, pointCenter.y - CrossHairLen);
		pDC->SelectObject(oldBrush);
		if (OldPen)
			pDC->SelectObject(OldPen);
	}
	//---------------------------------------
	// Draw the child objects of this origin
	//---------------------------------------
	while (pObj)
	{
		pObj->Draw(pDC, mode, ULHC, Scale);
		pObj = pObj->GetNext();
	}
}

BOOL CCadOrigin::PointInThisObject(DOUBLEPOINT point)
{
	return 0;
}

int CCadOrigin::PointInObjectAndSelect(
	DOUBLEPOINT p,
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
	int iX;
	CCadObject* pObj;
	CADObjectTypes Obj;

	if (index < n)
	{
		if (PointInThisObject(p))
		{
			Obj.pCadObject = FindObject(ObjectType::POINT, SubType::ORIGIN_LOCATION, 0);
			ppSelList[index++] = Obj.pCadObject;
		}
		pObj = GetHead();
		while (pObj)
		{
			iX = pObj->PointInObjectAndSelect(
				p, 
				ppSelList, 
				index, 
				n,
				nKinds
			);
			if (iX)
			{
				index += iX;
			}
		}
	}
	return index;
}

CString& CCadOrigin::GetTypeString(void)
{
	//---------------------------------------------------
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csName = _T("Origin");
	return csName;
}

CString& CCadOrigin::GetObjDescription()
{
	CADObjectTypes Obj;

	Obj.pCadObject = FindObject(ObjectType::POINT, SubType::ORIGIN_LOCATION, 0);
	GetDescription().Format(_T("Origin(%7.3lf,%7.3lf)"),Obj.pCadPoint->GetX(),Obj.pCadPoint->GetY());
	return GetDescription();
}

CCadObject * CCadOrigin::CopyObject(void)
{
	//---------------------------------------------------
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadOrigin *pCO = new CCadOrigin;
	pCO->Create();
	CCadObject::CopyObject(pCO);
	return pCO;
}

DocFileParseToken CCadOrigin::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
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

void CCadOrigin::CopyAttributesTo(SOriginAttributes* pAttrib)
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

void CCadOrigin::CopyAttributesFrom(SOriginAttributes * pAttrib)
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

ObjectDrawState CCadOrigin::MouseMove(ObjectDrawState DrawState)
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
	DOUBLEPOINT MousePosition = GETVIEW->GetCurrentMousePosition();
	CADObjectTypes Obj;

	switch (DrawState)
	{
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		Obj.pCadObject = FindObject(ObjectType::POINT, SubType::ORIGIN_LOCATION, 0);
		Obj.pCadPoint->SetPoint(MousePosition);
	}
	GETVIEW->Invalidate();
	return DrawState;
}

ObjectDrawState CCadOrigin::ProcessDrawMode(ObjectDrawState DrawState)
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
	CPoint MouseScreenCoordinate;
	CADObjectTypes Obj;

	switch (DrawState)
	{
	case ObjectDrawState::START_DRAWING:
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
		CopyAttributesFrom(&m_CurrentAttributes);
		DrawState = ObjectDrawState::SET_ATTRIBUTES;
		GETAPP.UpdateStatusBar(_T("Origin:Place Origin Point"));
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
		GETVIEW->GetCursorPosition(&MouseScreenCoordinate);
		Id = EditProperties();
		if (IDOK == Id)
		{
			if(m_AttributesDirty)
				CopyAttributesTo(&m_CurrentAttributes);
			DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
			GETVIEW->EnableAutoScroll(1);
		}
		else
			DrawState = ObjectDrawState::END_DRAWING;
		GETVIEW->SetCursorPosition(MouseScreenCoordinate);
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		Obj.pCadObject = FindObject(ObjectType::POINT, SubType::ORIGIN_LOCATION, 0);
		Obj.pCadPoint->SetPoint(MousePos);
		DrawState = ObjectDrawState::SET_ATTRIBUTES;
		GETVIEW->AddOriginAtTail(this);
		GETVIEW->EnableAutoScroll(0);
		GETVIEW->SetObjectTypes(new CCadOrigin);
		GETAPP.UpdateStatusBar(_T("Origin:Set Origin Name"));
		break;
	}
	return DrawState;
}

int CCadOrigin::EditProperties()
{
	int Id;
	CDlgOriginAttributes Dlg;

	Dlg.SetOrigin(this);
	Id = Dlg.DoModal();
	return Id;
}
