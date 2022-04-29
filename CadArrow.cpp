#include "pch.h"

CCadArrow::CCadArrow():CCadObject()
{
	m_pPen = 0;
	m_pBrFill = 0;
	m_RotationAngle = 0.0;
	SetType(ObjectType::ARROW);
	GetName().Format(_T("Arrow%d"), ++CCadArrow::m_ArrowCount);
	if (!m_AttributesGood)
	{
		m_AttributesGood = TRUE;
		m_LastAttributes.CopyFrom(GETAPP.GetArrowAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}

CCadArrow::CCadArrow(CCadArrow &ca) :CCadObject()
{
	m_ArrowTip = ca.GetArrowTip();
	m_DefineAngle = ca.GetDefineAnglePoint();
	GetAttributes().CopyFrom(ca.GetAttributesPointer());
	m_pPen = 0;
	m_pBrFill = 0;
	SetType(ObjectType::ARROW);
	GetName().Format(_T("Arrow%d"), ++CCadArrow::m_ArrowCount);
}

CCadArrow::~CCadArrow()
{
	if (m_pPen) delete m_pPen;
	if (m_pBrFill) delete m_pBrFill;
}

void CCadArrow::Move(CDoubleSize Diff)
{
	//***************************************************
	//	Move
	//		This Method is used to move the object
	// by the amount that is passed.
	//
	// parameters:
	//	p.......amount to move the object by
	//
	// return value: none
	//--------------------------------------------------
	m_ArrowTip += Diff;
	m_DefineAngle += Diff;
}

void CCadArrow::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
{
	//***************************************************
	// Save
	//		This Method save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
	char* IndentString = new char[256];

	GETAPP.MkIndentString(IndentString, Indent, ' ');
	fprintf(pO, "%s%s(\n",
		IndentString,
		CLexer::TokenToString(DocFileParseToken::ARROW)
	);
	m_ArrowTip.Save(pO, DocFileParseToken::START, Indent + 1, flags);
	m_DefineAngle.Save(pO, DocFileParseToken::END, Indent + 1, flags);
	GetAttributes().Save(pO, DocFileParseToken::ARROW, Indent + 1, flags);
	fprintf(pO, ")\n");
	delete[] IndentString;
}

void CCadArrow::SetVertex(int Vi, CDoubleSize sz)
{
	//***************************************************
	// SetVertex
	//	This Method is used to change the position of
	// a vertex.
	//
	// parameters:
	// Vi......index of the vertex
	// p......Amnount to change the vertex by
	//
	// return value: none
	//--------------------------------------------------
}

int CCadArrow::GrabPoint(CDoublePoint p)
{
	//***************************************************
	// GrabPoint
	//	This Method checks for a vertex at point p
	//
	// parameters:
	//	p.....point to check for presence of a vertex
	//	scale....scale factor
	//
	// return value:
	//	returns index of vertex if succesful
	//	returns -1 on fail
	//--------------------------------------------------
	return -1;
}


void CCadArrow::Draw(CDC* pDC, MODE mode, CDoublePoint& ULHC, CScale& Scale)
{
	//***************************************************
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
	CPen *pOldPen;
	CBrush *pOldBr;
	CRect rect;
	CPoint ArrowPoints[4];

	if (CCadArrow::IsRenderEnabled())
	{
		if ((!IsLastModeSame(mode)) || IsDirty())
		{
			SetDirty(FALSE);
			switch (mode.DrawMode)
			{
			case ObjectDrawMode::FINAL:
				if (m_pPen) delete m_pPen;
				m_pPen = new CPen();
				m_pPen->CreatePen(PS_SOLID, 1, GetAttributes().m_colorLine);
				if (m_pBrFill) delete m_pBrFill;
				m_pBrFill = new CBrush(GetAttributes().m_colorFill);
				break;
			case ObjectDrawMode::SELECTED:
				if (m_pPen) delete m_pPen;
				m_pPen = new CPen();
				m_pPen->CreatePen(PS_SOLID, 1, GetAttributes().m_colorLine ^ 0x00ff00);
				if (m_pBrFill) delete m_pBrFill;
				m_pBrFill = new CBrush(GetAttributes().m_colorFill ^ 0x00ff00);
				break;
			case ObjectDrawMode::SKETCH:
				if (m_pPen) delete m_pPen;
				m_pPen = new CPen();
				m_pPen->CreatePen(PS_SOLID, 1, GetAttributes().m_colorLine);
				if (m_pBrFill) delete m_pBrFill;
				m_pBrFill = new CBrush(GetAttributes().m_colorFill);
				break;
			}
		}
		switch (mode.DrawMode)
		{
		case ObjectDrawMode::FINAL:
		case ObjectDrawMode::SELECTED:
			pOldPen = pDC->SelectObject(m_pPen);
			pOldBr = pDC->SelectObject(m_pBrFill);
			pDC->Polygon(GETAPP.MakePolygonFromDoublePolygon(ArrowPoints, GetArrowPoints(), 4, ULHC, Scale), 4);
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBr);
			break;
		case ObjectDrawMode::SKETCH:
			pOldPen = pDC->SelectObject(m_pPen);
			pOldBr = pDC->SelectObject(m_pBrFill);
			pDC->Polygon(GETAPP.MakePolygonFromDoublePolygon(ArrowPoints, GetArrowPoints(), 4, ULHC, Scale), 4);
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBr);
			break;
		}
		SetLastMode(mode);
	}
}

BOOL CCadArrow::PointInObjectAndSelect(
	CDoublePoint p, 
	CCadObject ** ppSelList , 
	int index, 
	int n, 
	DrawingCheckSelectFlags flag
)
{
	//***************************************************
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
	//	flag........Determines what sort of objects selected
	//
	// return value:
	//	returns true if point is within object
	//	otherwise, false
	//--------------------------------------------------

	if (index < n || n == 0)
	{
		//---------------------------------------
		// is point in polygon/arrow
		//---------------------------------------
		if (GETAPP.PtEnclosedInPolygon(
			p,
			GetAttributes().m_PolyPointsShape.GetPoints(),
			GetAttributes().m_PolyPointsShape.GetSize()
			)
		)
		{
			if (ppSelList)
			{
				switch (flag)
				{
					case DrawingCheckSelectFlags::FLAG_ALL:
						ppSelList[index++] = this;
						break;
					case DrawingCheckSelectFlags::FLAG_UNSEL:
						if (!IsSelected())
							ppSelList[index++] = this;
						break;
					case DrawingCheckSelectFlags::FLAG_SEL:
						if (IsSelected())
							ppSelList[index++] = this;
						break;
				}
			}
			else
			{
				switch (flag)
				{
					case DrawingCheckSelectFlags::FLAG_ALL:
						index = 1;
						break;
					case DrawingCheckSelectFlags::FLAG_UNSEL:
						if (!IsSelected())
							index = 1;
						break;
					case DrawingCheckSelectFlags::FLAG_SEL:
						if (IsSelected())
							index = 1;
						break;
				}

			}
		}
	}
	return index;
}

CDoublePoint CCadArrow::GetReference()
{
	//***************************************************
	// GetReference
	//	This Method returns the reference point for
	// the object
	// parameters:none
	//
	// return value:reference point
	//--------------------------------------------------
	return m_ArrowTip;
}



CDoubleRect& CCadArrow::GetRect(CDoubleRect& rect)
{
	//***************************************************
	// GetRect
	//	Returns the rectangle that will enclose the
	//  object
	// parameters:
	//
	// return value:Returns the rectangle that encloses
	// the object
	//--------------------------------------------------
	double MinX = 1e6, MinY = 1e6, MaxX= -1e6, MaxY = -1e6;
	CDoublePoint* PolyPoints = GetAttributes().m_PolyPointsShape.GetPoints();

	for (UINT i = 0; i < GetAttributes().m_PolyPointsShape.GetSize(); ++i)
	{
		if (MinX > PolyPoints[i].dX) MinX = PolyPoints[i].dX;
		if (MinY > PolyPoints[i].dY) MinY = PolyPoints[i].dY;
		if (MaxX < PolyPoints[i].dX) MaxX = PolyPoints[i].dX;
		if (MaxY < PolyPoints[i].dY) MaxY = PolyPoints[i].dY;
	}
	rect = CDoubleRect(
		CDoublePoint(MinX, MinY) + m_ArrowTip,
		CDoublePoint(MaxX, MaxY) + m_ArrowTip
	);
	rect.Rotate(m_ArrowTip, m_RotationAngle, RectPoint::CENTER_RIGHTLINE);
	return rect;
}

CString& CCadArrow::GetTypeString(void)
{
	//***************************************************
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

CCadArrow CCadArrow::operator=(CCadArrow &Ca)
{
	//***************************************************
	// operator=
	//		Provides the Methodality when one object
	// value is assigned to another
	// parameters:
	//	v......reference to object to get value(s) from
	//
	// return value:this
	//--------------------------------------------------
	CCadArrow Result;

	Result.SetArrowTip(Ca.GetArrowTip());
	Result.SetDefineAnglePoint(Ca.GetDefineAnglePoint());
	Result.GetAttributes().CopyFrom(&Ca.GetAttributes());
	return Result;
}

CCadObject * CCadArrow::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadArrow *pArrow = new CCadArrow;
	*pArrow = *this;
	return pArrow;
}

void CCadArrow::SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------

}
	
CDoublePoint& CCadArrow::GetCenter(CDoublePoint& point)
{
	//***************************************************
	// GetCenter
	//	Get the point at the "center" of the object.
	// parameters:
	//
	// return value:the center point
	//--------------------------------------------------
	CDoubleRect rect;
	point = GetRect(rect).GetCenter(point);
	return point;
}


CDoubleSize& CCadArrow::GetSize(CDoubleSize& size)
{
	//***************************************************
	// GetSize
	//	Get the size of the object.  Reutrns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	CDoubleRect rect;
	size = GetRect(rect).GetSize(size);
	return size;
}

DocFileParseToken CCadArrow::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
{
	//***************************************************
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
	Token = pLex->Expect(Token, TypeToken);
	Token = pLex->Expect(Token, DocFileParseToken('('));
	Token = pLex->Point(DocFileParseToken::ARROW_TIP, m_ArrowTip, Token);
	Token = pLex->Expect(Token, DocFileParseToken(','));
	Token = pLex->Point(DocFileParseToken::ARROW_END, m_ArrowTip, Token);
	Token = pLex->Expect(Token, DocFileParseToken(')'));
	Token = GetAttributes().Parse(Token, pLex);
	return Token;
}

void CCadArrow::CopyAttributesTo(SArrowAttributes *pAttrib)
{
	/***************************************************
	*	GetAttributes
	*		This Method is used to copy the
	*	attributes from this object into an
	*	attributes structure pointed to by the parameter
	*
	* Parameters:
	*	pAttrb.....pointer to attributes structure to copy into
	***************************************************/
	GetAttributes().CopyTo(pAttrib);
}

void CCadArrow::CopyAttributesFrom(SArrowAttributes *pAttrib)
{
	/***************************************************
	*	CopyAttributesFrom
	*		This Method is used to copy the
	*	attributes pointed to by the parameter into
	*	this object
	*
	* Parameters:
	*	pAttrb.....pointer to attributes structure to copy
	***************************************************/
	GetAttributes().CopyFrom(pAttrib);
	ClearNeedsAttributes();
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
	CDoublePoint MousePos = GETVIEW()->GetCurrentMousePosition();

	switch (DrawState)
	{
	case ObjectDrawState::START_DRAWING:
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Arrow:Locate Arrow Tip Point"));
		break;
	case ObjectDrawState::END_DRAWING:
		if (m_AttributesDirty)
		{
			Id = GETVIEW()->MessageBoxW(_T("Do you want to keep\nThe current\nAttributes?"), _T("Keep Or Toss"), MB_YESNO);
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
		GETVIEW()->EnableAutoScroll(TRUE);
		m_ArrowTip = m_DefineAngle = MousePos;
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		SetArrowTip(MousePos);
		DrawState = ObjectDrawState::ROTATE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Arrow:Place Rotation Point"));
		break;
	case ObjectDrawState::ROTATE_LBUTTON_DOWN:
		m_DefineAngle = MousePos;
		DrawState = ObjectDrawState::ROTATE_LBUTTON_UP;
		break;
	case ObjectDrawState::ROTATE_LBUTTON_UP:
		GETVIEW()->EnableAutoScroll(FALSE);
		SetDefineAnglePoint(MousePos);
		GETVIEW()->AddObjectAtFrontIntoDoc(this);
		GETVIEW()->SetObjectTypes(new CCadArrow);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Arrow:Locate Arrow Tip Point"));
		GETVIEW()->Invalidate();
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
	CDoublePoint MousePos = GETVIEW()->GetCurrentMousePosition();

	switch (DrawState)
	{
		case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
			SetArrowTip(MousePos);
			break;
		case ObjectDrawState::ROTATE_LBUTTON_DOWN:
			SetDefineAnglePoint(MousePos);
			break;
	}
	GETVIEW()->Invalidate();
	return DrawState;
}

int CCadArrow::EditProperties()
{
	int ID;
	CDlgArrowProperties Dlg;

	Dlg.SetArrow(this);
	ID = Dlg.DoModal();
	return ID;
}

double CCadArrow::GetL1()
{
	double L1;

	L1 = GetArrowPoint(Arrow::TIP).dX - GetArrowPoint(Arrow::TOP).dX;
	return fabs(L1);
}

double CCadArrow::GetL2()
{
	double L2;

	L2 = GetArrowPoint(Arrow::TIP).dX - GetArrowPoint(Arrow::END).dX;
	return fabs(L2);
}

double CCadArrow::GetW()
{
	double W;

	W = GetArrowPoint(Arrow::TOP).dY - GetArrowPoint(Arrow::BOT).dY;
	return fabs(W);
}

BOOL CCadArrow::NeedsAttributes()
{
	return (m_AttributesGood == FALSE);
}

void CCadArrow::ClearNeedsAttributes()
{
	m_AttributesGood = TRUE;
}

void CCadArrow::Recalc()
{
	for (int i = 0; i < 4; ++i)
	{
		GetArrowPoints()[i] = GetAttributes().m_PolyPointsShape.GetPoint(i) + GetArrowTip();
	}
}

void CCadArrow::SetArrowTip(CDoublePoint Tip)
{
	m_ArrowTip = Tip;
	Recalc();
}

void CCadArrow::SetDefineAnglePoint(CDoublePoint DAP)
{
	double Angle;
	int i;

	m_DefineAngle = DAP;
	Angle = (DAP - m_ArrowTip).Angle();
	Recalc();
	for (i = 1; i < 4; ++i)
	{
		GetArrowPoints()[i].Rotate(GetArrowTip(), Angle);
	}
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
	GetShape().SetPoint(int(Arrow::TIP), CDoublePoint(0, 0));
	GetShape().SetPoint(int(Arrow::TOP), CDoublePoint(L1, W / 2.0));
	GetShape().SetPoint(int(Arrow::END), CDoublePoint(L2, 0.0));
	GetShape().SetPoint(int(Arrow::BOT), CDoublePoint(L1, -W / 2.0));

}

