#include "pch.h"

CCadOrigin::CCadOrigin():CCadObject(ObjectType::ORIGIN)
{
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


BOOL CCadOrigin::Create(CCadObject* pParent, SubTypes type)
{
	CADObjectTypes Obj;

	CCadObject::Create(pParent,  type);
	if (pParent == NULL)
		pParent = this;
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::ORIGIN_LOCATION);
	AddObjectAtTail(Obj.pCadObject);
	return TRUE;
}

void CCadOrigin::UpdateEnclosure()
{
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

	Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ORIGIN_LOCATION, 0);
	return DOUBLEPOINT(*Obj.pCadPoint);
}

void CCadOrigin::Save(
	CFile* pcfFile,
	int Indent
)
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
void CCadOrigin::Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale)
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
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ORIGIN_LOCATION, 0);
		pointLR = Obj.pCadPoint->ToPixelPoint(LLHC,Scale) + CSize(rectHalfWidth, rectHalfWidth);
		pointUL = Obj.pCadPoint->ToPixelPoint(LLHC, Scale) - CSize(rectHalfWidth, rectHalfWidth);
		rect.SetRect(pointUL, pointLR);
		rect.NormalizeRect();
		CreateThePen(mode, &LinePen, LineWidth);
		OldPen = pDC->SelectObject(&LinePen);
		fillBrush.CreateStockObject(NULL_BRUSH);
		oldBrush = pDC->SelectObject(&fillBrush);
		pDC->Ellipse(&rect);
		pointCenter = Obj.pCadPoint->ToPixelPoint(LLHC, Scale);
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
		pObj->Draw(pDC, mode, LLHC, Scale);
		pObj = pObj->GetNext();
	}
}

BOOL CCadOrigin::IsPointEnclosed(DOUBLEPOINT p)
{
	return 0;
}

BOOL CCadOrigin::PointInThisObject(DOUBLEPOINT point)
{
	return 0;
}

int CCadOrigin::PointInObjectAndSelect(
	DOUBLEPOINT p,
	CCadObject *pExcludeObject,
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
	//	n...........Total number of spaces in selection list
	//
	// return value:
	//	returns true if point is within object
	//	otherwise, false
	//--------------------------------------------------
	int iX;
	CADObjectTypes Obj;

	if (index < n)
	{
		if (PointInThisObject(p))
		{
			if (IsItThisKind(nKinds) && DontExclude(pExcludeObject))
			{
				ppSelList[index++] = this;
				ShouldWeSelectThisObjectAndDidIt(nKinds);
			}
		}
		iX = CCadObject::PointInObjectAndSelect(
			p, 
			pExcludeObject,
			ppSelList, 
			index, 
			n,
			nKinds
		);
		index += iX;
	}
	return index;
}

CString& CCadOrigin::GetTypeString()
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

	Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ORIGIN_LOCATION, 0);
	GetDescription().Format(_T("Origin(%7.3lf,%7.3lf)"),Obj.pCadPoint->GetX(),Obj.pCadPoint->GetY());
	return GetDescription();
}

CCadObject * CCadOrigin::Copy()
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
	pCO->Create(GetParent(), GetSubType());
	CCadObject::CopyObject(pCO);
	return pCO;
}

void CCadOrigin::CopyAttributes(CCadObject* pToObj)
{
	((CCadOrigin*)pToObj)->CopyAttributesFrom(GetPtrToAttributes());
}

void CCadOrigin::Parse(
	CParser* pParser,	// pointer to parser
	Token TypeToken// Token type to save object as
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
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ORIGIN_LOCATION, 0);
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
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ORIGIN_LOCATION, 0);
		Obj.pCadPoint->SetPoint(MousePos);
		DrawState = ObjectDrawState::SET_ATTRIBUTES;
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

void CCadOrigin::CreateThePen(MODE mode, CPen* pen, int Lw)
{
	switch (mode.PaintMode)
	{
	case MODE::ObjectPaintMode::FINAL:
		if (IsSelected())
			pen->CreatePen(PS_SOLID, Lw, GetAttributes().m_colorSelected);
		else
			pen->CreatePen(PS_SOLID, Lw, GetAttributes().m_colorLine);
		break;
	case MODE::ObjectPaintMode::SKETCH:
		pen->CreatePen(PS_DOT, Lw, GetAttributes().m_colorSelected);
		break;
	}
}
