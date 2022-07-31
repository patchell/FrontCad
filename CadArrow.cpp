#include "pch.h"

CCadArrow::CCadArrow():CCadObject()
{
	SetType(ObjectType::ARROW);
	GetName().Format(_T("Arrow%d"), ++CCadArrow::m_ArrowCount);
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetArrowAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}

CCadArrow::~CCadArrow()
{
}

BOOL CCadArrow::Create(CCadObject* pParent, CCadObject* pOrigin)
{
	CADObjectTypes Obj;

	CCadObject::Create(pParent, pOrigin);
	if (pParent == NULL)
		pParent = this;
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, pOrigin);
	Obj.pCadPoint->SetSubType(SubType::ARROW_TIP);
	AddObjectAtChildTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, pOrigin);
	Obj.pCadPoint->SetSubType(SubType::ARROW_TOP);
	AddObjectAtChildTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, pOrigin);
	Obj.pCadPoint->SetSubType(SubType::ARROW_END);
	AddObjectAtChildTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, pOrigin);
	Obj.pCadPoint->SetSubType(SubType::ARROW_BOT);
	AddObjectAtChildTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, pOrigin);
	Obj.pCadPoint->SetSubType(SubType::ARROW_ROTATION);
	AddObjectAtChildTail(Obj.pCadObject);

	return TRUE;
}

BOOL CCadArrow::Destroy(CCadObject* pDependentObjects)
{
	BOOL rV = TRUE;
	return rV;
}

void CCadArrow::Move(CDoubleSize Diff)
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

void CCadArrow::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
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

void CCadArrow::Draw(CDC* pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale)
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
	CPoint ArrowPoints[4];
	CADObjectTypes TIP, TOP, END, BOT, ROT;
	int Lw;

	if (IsRenderEnabled())
	{
		MakeCPointArray(ArrowPoints, pDC, mode, ULHC, Scale);
		Lw = GETAPP.RoundDoubleToInt(GetAttributes().m_LineWidth * Scale.GetScaleX());
		if (Lw < 1)
			Lw = 1;
		TIP.pCadObject = FindChildObject(ObjectType::POINT, SubType::ARROW_TIP, 0);
		TOP.pCadObject = FindChildObject(ObjectType::POINT, SubType::ARROW_TOP, 0);
		END.pCadObject = FindChildObject(ObjectType::POINT, SubType::ARROW_END, 0);
		BOT.pCadObject = FindChildObject(ObjectType::POINT, SubType::ARROW_END, 0);
		ROT.pCadObject = FindChildObject(ObjectType::POINT, SubType::ARROW_ROTATION, 0);
		switch (mode.DrawMode)
		{
		case ObjectDrawMode::FINAL:
			penLine.CreatePen(PS_SOLID, Lw, GetAttributes().m_colorLine);
			brushFill.CreateSolidBrush(GetAttributes().m_colorFill);
			pOldPen = pDC->SelectObject(&penLine);
			pOldBr = pDC->SelectObject(&brushFill);
			pDC->Polygon(ArrowPoints, 4);
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBr);
			break;
		case ObjectDrawMode::SELECTED:
			penLine.CreatePen(PS_SOLID, Lw, GetAttributes().m_colorSelected);
			brushFill.CreateStockObject(NULL_BRUSH);
			pOldPen = pDC->SelectObject(&penLine);
			pOldBr = pDC->SelectObject(&brushFill);
			pDC->Polygon(ArrowPoints, 4);
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBr);
			break;
		case ObjectDrawMode::SKETCH:
			penLine.CreatePen(PS_SOLID, 1, GetAttributes().m_colorSelected);
			pOldPen = pDC->SelectObject(&penLine);
			brushFill.CreateStockObject(NULL_BRUSH);
			pOldBr = pDC->SelectObject(&brushFill);
			pDC->Polygon(ArrowPoints, 4);
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBr);
			break;
		}
	}
}

BOOL CCadArrow::PointInThisObject(DOUBLEPOINT point)
{
	DOUBLEPOINT Points[4];
	CADObjectTypes Obj;
	BOOL rV;

	Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::ARROW_TIP, 0);
	Points[ARROW_TIP] = DOUBLEPOINT(*Obj.pCadPoint);
	Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::ARROW_TOP, 0);
	Points[ARROW_TOP] = DOUBLEPOINT(*Obj.pCadPoint);
	Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::ARROW_END, 0);
	Points[ARROW_BACK] = DOUBLEPOINT(*Obj.pCadPoint);
	Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::ARROW_BOT, 0);
	Points[ARROW_BOT] = DOUBLEPOINT(*Obj.pCadPoint);
	rV = GETAPP.PtEnclosedInPolygon(point, Points,4);
	return rV;
}

int CCadArrow::PointInObjectAndSelect(
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

	if (index < n || n == 0)
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

CString& CCadArrow::GetTypeString(void)
{
	//--------------------------------------------------
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csName = CString(_T("Arrow"));
	return csName;
}

CString& CCadArrow::GetObjDescription()
{
	GetDescription().Format(_T("Base Obj Class"));
	return GetDescription();
}

CCadObject * CCadArrow::CopyObject(void)
{
	//--------------------------------------------------
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadArrow *pArrow = new CCadArrow;
	pArrow->Create(GetParent(), GetOrigin());
	CCadObject::CopyObject(pArrow);
	return pArrow;
}

CDoubleSize CCadArrow::GetSize()
{
	//---------------------------------------
	// GetSize
	// 
	// Get the size of the rectangle that
	// encloses the arrow
	//---------------------------------------
	double MinX, MaxX, MinY, MaxY;
	CDoubleSize Size;

	GETAPP.GetPolyMinMax(GetAttributes().m_aArrowShape, 4, MinX, MaxX, MinY, MaxY);
	Size = CDoubleSize(MaxX - MinX, MinY - MaxY);
	return Size;
}


DocFileParseToken CCadArrow::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
{
	//--------------------------------------------------
	// Parse
	//	This Method is used to parse this 
	// object out of an input stream
	//
	// parameters:
	//	Token......Token for this object
	//	pLex.......Pointer to Lexer for the data stream
	//	TypeToken..Alternative type other than the default
	// 
	// return value:
	//	returns lookahead token on success, or
	//			throws exception on error
	//--------------------------------------------------
	return Token;
}

void CCadArrow::CopyAttributesTo(SArrowAttributes *pAttrib)
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

void CCadArrow::CopyAttributesFrom(SArrowAttributes *pAttrib)
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

ObjectDrawState CCadArrow::ProcessDrawMode(ObjectDrawState DrawState)
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
		GETAPP.UpdateStatusBar(_T("Arrow:Locate Arrow Tip Point"));
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
		Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::ARROW_TIP, 0);
		Obj.pCadPoint->SetPoint(MousePos);
		Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::ARROW_ROTATION, 0);
		Obj.pCadPoint->SetPoint(MousePos);
		DrawState = ObjectDrawState::ROTATE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Arrow:Place Rotation Point"));
		break;
	case ObjectDrawState::ROTATE_LBUTTON_DOWN:
		DrawState = ObjectDrawState::ROTATE_LBUTTON_UP;
		break;
	case ObjectDrawState::ROTATE_LBUTTON_UP:
		Rotate(MousePos);
		GETVIEW->GetDocument()->AddObjectAtTail(this);
		Obj.pCadArrow = new CCadArrow;
		Obj.pCadArrow->Create(GetParent(), GetOrigin());
		GETVIEW->SetObjectTypes(Obj.pCadObject);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Arrow:Locate Arrow Tip Point"));
		GETVIEW->Invalidate();
		break;
	}
	return DrawState;
}



ObjectDrawState CCadArrow::MouseMove(ObjectDrawState DrawState)
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

	switch (DrawState)
	{
		case ObjectDrawState::ROTATE_LBUTTON_DOWN:
			Rotate(MousePos);
			break;
	}
	GETVIEW->Invalidate();
	return DrawState;
}

void CCadArrow::Rotate(DOUBLEPOINT MousePos)
{
	CADObjectTypes TIP, TOP, END, BOT, ROT;
	DOUBLEPOINT R,T,TP, BP;
	double m,x,y, Er;
	double x1, y1;
	double x2, y2;


	TIP.pCadObject = FindChildObject(ObjectType::POINT, SubType::ARROW_TIP, 0);
	TOP.pCadObject = FindChildObject(ObjectType::POINT, SubType::ARROW_TOP, 0);
	END.pCadObject = FindChildObject(ObjectType::POINT, SubType::ARROW_END, 0);
	BOT.pCadObject = FindChildObject(ObjectType::POINT, SubType::ARROW_BOT, 0);
	//------------------------------------------------------------
	// This point defines the angle that the arrow is 
	ROT.pCadObject = FindChildObject(ObjectType::POINT, SubType::ARROW_ROTATION, 0);
	ROT.pCadPoint->SetPoint(MousePos);
	Er = GetAttributes().m_aArrowShape[ARROW_BACK].dX;
	R = DOUBLEPOINT(*ROT.pCadPoint);
	T = DOUBLEPOINT(*TIP.pCadPoint);
	//------------------------------------
	// rotate the End point
	//------------------------------------
	if (0 == (R.dX - T.dX))
	{
		//------------------------------------
		// This is a vertical line
		//-----------------------------------
		END.pCadPoint->SetPoint(T + GetAttributes().m_aArrowShape[ARROW_BACK]);
	}
	else
	{
		m = (R.dY - T.dY) / (R.dX - T.dX);
		x = sqrt(Er * Er / (1 + m * m));
		y = m * x;
		END.pCadPoint->SetPoint(T + DOUBLEPOINT(x, y));
		//------------------------------------------
		// Rotate the top and bottom points
		// These two points will be on a line that
		// goes through a point that is on the axis
		// of the arrow.  Confused?  So am I
		//------------------------------------------
		Er = GetAttributes().m_aArrowShape[ARROW_TOP].dX;
		if (m == 0)
		{
			TP = GetAttributes().m_aArrowShape[ARROW_TOP];
			TOP.pCadPoint->SetPoint(TP);
			BP = GetAttributes().m_aArrowShape[ARROW_BOT];
			BOT.pCadPoint->SetPoint(BP);
		
		}
		else
		{
			x = sqrt(Er * Er / (1 + m * m));
			y = m * x;
			m = -1.0 / m;
			Er = GetAttributes().m_aArrowShape[ARROW_TOP].dY;
			x1 = sqrt(Er * Er / (1 + m * m));
			y1 = m * x1;
			x2 = -x1;
			y2 = -y1;
			TOP.pCadPoint->SetPoint(T + DOUBLEPOINT(x + x1, y + y1));
			BOT.pCadPoint->SetPoint(T + DOUBLEPOINT(x + x2, y + y2));
		}

	}
}

int CCadArrow::EditProperties()
{
	int Id;
	CDlgArrowProperties Dlg;

	Dlg.SetArrow(this);
	Id = Dlg.DoModal();
	return Id;
}

void CCadArrow::MakeCPointArray(CPoint* PolyPoints, CDC* pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale)
{
	CADObjectTypes Obj;

	Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::ARROW_TIP, 0);
	PolyPoints[ARROW_TIP] = Obj.pCadPoint->ToPixelPoint(ULHC, Scale);
	Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::ARROW_TOP, 0);
	PolyPoints[ARROW_TOP] = Obj.pCadPoint->ToPixelPoint(ULHC, Scale);
	Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::ARROW_END, 0);
	PolyPoints[ARROW_BACK] = Obj.pCadPoint->ToPixelPoint(ULHC, Scale);
	Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::ARROW_BOT, 0);
	PolyPoints[ARROW_BOT] = Obj.pCadPoint->ToPixelPoint(ULHC, Scale);
}

//-------------------------------------------
// Methods used to edit the arrow shape
// in the object attributes
//-------------------------------------------
double CCadArrow::GetL1()
{
	double L1;
	double TipX, TopX;
	TipX = GetAttributes().m_aArrowShape[ARROW_TIP].dX;
	TopX = GetAttributes().m_aArrowShape[ARROW_TOP].dX;
	L1 = TipX - TopX;
	return fabs(L1);
}

double CCadArrow::GetL2()
{
	double L2;
	double TipX, EndX;
	TipX = GetAttributes().m_aArrowShape[ARROW_TIP].dX;
	EndX = GetAttributes().m_aArrowShape[ARROW_BACK].dX;
	L2 = TipX - EndX;
	return fabs(L2);
}

double CCadArrow::GetW()
{
	double W;
	double TopY, BotY;

	TopY = GetAttributes().m_aArrowShape[ARROW_TOP].dY;
	BotY = GetAttributes().m_aArrowShape[ARROW_BOT].dY;
	W = TopY - BotY;
	return fabs(W);
}

void CCadArrow::ApplyParameters(double L1, double L2, double W)
{
	//------------------------------------
	// ApplyParameters
	//	This method is used by the 
	// properties editor to define the
	// shape of the Arrow object.
	// 
	// Parameters:
	//	L1.....Over Length
	//	L2.....Length to bend from tip
	//	W......Width, opposite tip
	//------------------------------------
	GetAttributes().m_aArrowShape[ARROW_TIP] = DOUBLEPOINT(0, 0);
	GetAttributes().m_aArrowShape[ARROW_TOP] = DOUBLEPOINT(L1, W / 2.0);
	GetAttributes().m_aArrowShape[ARROW_BACK] = DOUBLEPOINT(L2, 0.0);
	GetAttributes().m_aArrowShape[ARROW_BOT] = DOUBLEPOINT(L1, -W / 2.0);
}

