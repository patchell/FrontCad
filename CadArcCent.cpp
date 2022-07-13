#include "pch.h"


CCadArcCent::CCadArcCent():CCadObject()
{
	SetType(ObjectType::ARCCENTERED);
	GetName().Format(_T("ARCCENTER_%d"), ++m_ArcCentCount);
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetArcCenterAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}

CCadArcCent::~CCadArcCent()
{
}

void CCadArcCent::Create(void)
{
	CADObjectTypes Obj;

	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create();
	Obj.pCadPoint->SetSubType(SubType::CENTERPOINT);
	Obj.pCadPoint->SetSubSubType(0);
	AddObjectAtTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create();
	Obj.pCadPoint->SetSubType(SubType::STARTPOINT);
	Obj.pCadPoint->SetSubSubType(0);
	AddObjectAtTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create();
	Obj.pCadPoint->SetSubType(SubType::ENDPOINT);
	Obj.pCadPoint->SetSubSubType(0);
	AddObjectAtTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create();
	Obj.pCadPoint->SetSubType(SubType::RECTSHAPE);
	Obj.pCadPoint->SetSubSubType(1);
	AddObjectAtTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create();
	Obj.pCadPoint->SetSubType(SubType::RECTSHAPE);
	Obj.pCadPoint->SetSubSubType(2);
	AddObjectAtTail(Obj.pCadObject);
}

BOOL CCadArcCent::Destroy(CCadObject* pDepndentObjects)
{
	return 0;
}


void CCadArcCent::Move(CDoubleSize Diff)
{
	//--------------------------------------------------
	//	Move
	//		This Method is used to move the object
	// by the point that is passed.
	//
	// parameters:
	//	p.......point to move the object by
	//
	// return value: none
	//--------------------------------------------------
	CCadObject::Move(Diff);
}

void CCadArcCent::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
{
	//--------------------------------------------------
	// Save
	//		This Method save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
	char* TempString = new char[256];
	char* IndentString = new char[256];

	GETAPP.MkIndentString(IndentString, Indent);
	fprintf(pO, "%s%s(\n",
		IndentString,
		CLexer::TokenToString(DocFileParseToken::ARCCENTERED)
	);
//	m_rectShape.Save(pO, DocFileParseToken::SHAPE, Indent + 1, flags);
//	GetCenter()->Save(pO, DocFileParseToken::CENTER, Indent + 1, flags);
//	GetStartPoint().Save(pO, DocFileParseToken::START, Indent + 1, flags);
//	GetEndPoint().Save(pO, DocFileParseToken::END, Indent + 1, flags);
	GetAttributes().Save(pO, Indent + 1, flags);
	delete[] TempString;
}

void CCadArcCent::Draw(CDC* pDC, MODE mode, CCadPoint ULHC, CScale Scale)
{
	//--------------------------------------------------
	// Draw
	//	This Method draws the document to the device
	// parameters:
	//	pDC.....pointer to the device context
	//	mode....drawing mode
	//	Offset..Offset to draw objects at
	//	Scale..Scale factor to draw objects in Pixels/Inch
	//
	// return value:none
	//--------------------------------------------------
	CPen *pOldPen, penLine;
	CRect rect;
	CADObjectTypes P1, P2, PC, PS, PE;
	int Lw;

	if (IsRenderEnabled())
	{
		P1.pCadObject = FindObject(ObjectType::POINT, SubType::RECTSHAPE, 1);
		P2.pCadObject = FindObject(ObjectType::POINT, SubType::RECTSHAPE, 2);
		PC.pCadObject = FindObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
		PS.pCadObject = FindObject(ObjectType::POINT, SubType::STARTPOINT, 0);
		PE.pCadObject = FindObject(ObjectType::POINT, SubType::ENDPOINT, 0);
		rect.SetRect(
			P1.pCadPoint->ToPixelPoint(ULHC, Scale),
			P2.pCadPoint->ToPixelPoint(ULHC, Scale)
		);
		Lw = GETAPP.RoundDoubleToInt(GetAttributes().m_LineWidth * Scale.GetScaleX());
		if (Lw < 1)
			Lw = 1;

		switch (mode.DrawMode)
		{
		case ObjectDrawMode::FINAL:
			penLine.CreatePen(PS_SOLID, Lw, GetAttributes().m_colorLine);
			pOldPen = pDC->SelectObject(&penLine);
			pDC->Arc(
				&rect, 
				PS.pCadPoint->ToPixelPoint(ULHC,Scale), 
				PS.pCadPoint->ToPixelPoint(ULHC, Scale)
			);
			pDC->SelectObject(pOldPen);
			break;
		case ObjectDrawMode::SELECTED:
			penLine.CreatePen(PS_SOLID, Lw, GetAttributes().m_colorSelected);
			pOldPen = pDC->SelectObject(&penLine);
			pDC->Arc(
				&rect,
				PS.pCadPoint->ToPixelPoint(ULHC, Scale),
				PS.pCadPoint->ToPixelPoint(ULHC, Scale)
			);
			pDC->SelectObject(pOldPen);
			break;
		case ObjectDrawMode::SKETCH:
			pOldPen = pDC->SelectObject(&penLine);
			pDC->Rectangle(&rect);
			pDC->Ellipse(&rect);
			pDC->SelectObject(pOldPen);
			break;
		case ObjectDrawMode::ARCSTART:
			pOldPen = pDC->SelectObject(&penLine);
			pDC->Ellipse(&rect);
			pDC->MoveTo(PC.pCadPoint->ToPixelPoint(ULHC,Scale));
			pDC->LineTo(PS.pCadPoint->ToPixelPoint(ULHC,Scale));
			pDC->SelectObject(pOldPen);
			break;
		case ObjectDrawMode::ARCEND:
			pOldPen = pDC->SelectObject(&penLine);
			pDC->Arc(
				&rect,
				PS.pCadPoint->ToPixelPoint(ULHC, Scale),
				PS.pCadPoint->ToPixelPoint(ULHC, Scale)
			);
			pDC->MoveTo(PC.pCadPoint->ToPixelPoint(ULHC,Scale));
			pDC->LineTo(PE.pCadPoint->ToPixelPoint(ULHC,Scale));
			pDC->SelectObject(pOldPen);
			break;
		}
	}
}

BOOL CCadArcCent::PointInThisObject(DOUBLEPOINT point)
{
	//-----------------------------------------------------
	//	PtInArc
	// Determine if the point is near (on) the arc that is
	// of interest
	//
	// parameter:
	//	p.......point of interest
	//
	// Returns:
	//	TRUE if point is on arc
	//-----------------------------------------------------
	double StartAngle, EndAngle, Angle;
	double A, B;
	BOOL rV = FALSE;
	CCadRect rect;
	CADObjectTypes P1, P2, PC;

	//-------------------------------
	// do a rough test to cull
	// this object out
	//------------------------------
	P1.pCadObject = FindObject(ObjectType::POINT, SubType::RECTSHAPE, 1);
	P2.pCadObject = FindObject(ObjectType::POINT, SubType::RECTSHAPE, 2);
	PC.pCadObject = FindObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
	rect.Create();
	rect.SetPoints(DOUBLEPOINT(*P1.pCadPoint), DOUBLEPOINT(*P2.pCadPoint), DOUBLEPOINT(*P1.pCadPoint));
	if (rect.PointInThisObject(point))
	{
		//-------------------------------------
		// first, check to see if the angle
		// formed by p is between the start
		// and end angle
		//-------------------------------------
		StartAngle = GETAPP.ArcTan(
			P1.pCadPoint->GetX() - PC.pCadPoint->GetX(),
			PC.pCadPoint->GetY() - P1.pCadPoint->GetY()
		);
		EndAngle = GETAPP.ArcTan(
			P2.pCadPoint->GetX() - PC.pCadPoint->GetX(),
			PC.pCadPoint->GetY() - P2.pCadPoint->GetY()
		);
		Angle = GETAPP.ArcTan(
			point.dX - PC.pCadPoint->GetX(),
			PC.pCadPoint->GetY() - point.dX
		);
		if(GETAPP.CheckAngle(StartAngle,EndAngle,Angle))
		{
			//----------------------------------------
			// check to see if the point is within
			// the radius of the arc
			//----------------------------------------

			A = rect.GetWidth() / 2.0;
			B = rect.GetHeight() / 2.0;
			rV = GETAPP.TestEllipsePoint(
				A, 
				B, 
				point, 
				DOUBLEPOINT(*PC.pCadPoint), 
				TOLERANCE_10_PERCENT);
		}
	}
	return rV;
}

int CCadArcCent::EditProperties()
{
	CDlgArcCentProperies Dlg;
	int Id;

	Dlg.SetArcCentered(this);
	Id = Dlg.DoModal();
	return Id;
}

int CCadArcCent::PointInObjectAndSelect(
	DOUBLEPOINT p,
	CCadObject** ppSelList,
	int index,
	int n
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

	if (index < n || n == 0)
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
				n
			);
			index += ix;
		}
	}
	return index;
}

CString& CCadArcCent::GetTypeString(void)
{
	//--------------------------------------------------
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csTypeName = CString(_T("Arc Centered"));
	return csTypeName;
}

CString& CCadArcCent::GetObjDescription()
{
	CADObjectTypes Obj;

	Obj.pCadObject = FindObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
	GetDescription().Format(_T("CenteredArc(%7.3lf,%7.3lf)"),Obj.pCadPoint->GetX(), Obj.pCadPoint->GetY());
	return GetDescription();
}

CCadObject * CCadArcCent::CopyObject(void)
{
	//--------------------------------------------------
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CADObjectTypes newObj;
	
	newObj.pCadArcCent = new CCadArcCent;
	newObj.pCadArcCent->Create();
	CCadObject::CopyObject(newObj.pCadObject);
	newObj.pCadArcCent->CopyAttributesFrom(GetPtrToAttributes());
	return newObj.pCadObject;
}

DocFileParseToken CCadArcCent::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
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

void CCadArcCent::CopyAttributesTo(SArcCenterAttributes *pAttrib)
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

void CCadArcCent::CopyAttributesFrom(SArcCenterAttributes*pAttrib)
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

ObjectDrawState CCadArcCent::ProcessDrawMode(ObjectDrawState DrawState)
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
	CADObjectTypes P1, P2, PC, PS, PE;

	switch (DrawState)
	{
	case ObjectDrawState::START_DRAWING:
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
		CopyAttributesFrom(&m_CurrentAttributes);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETVIEW->EnableAutoScroll(TRUE);
		GETAPP.UpdateStatusBar(_T("ARC CENTER:Place Center Point of Circle Shape"));
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
		GETAPP.UpdateStatusBar(_T(""));
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
		PC.pCadObject = FindObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
		PC.pCadPoint->SetPoint(MousePos);
		P1.pCadObject = FindObject(ObjectType::POINT, SubType::RECTSHAPE, 1);
		P1.pCadPoint->SetPoint(MousePos);
		P2.pCadObject = FindObject(ObjectType::POINT, SubType::RECTSHAPE, 2);
		P2.pCadPoint->SetPoint(MousePos);
		DrawState = ObjectDrawState::PLACE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("ARC CENTERED:Place Corner of Circle Shape"));
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		DrawState = ObjectDrawState::PLACE_LBUTTON_UP;
		break;
	case ObjectDrawState::PLACE_LBUTTON_UP:
		PC.pCadObject = FindObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
		P1.pCadObject = FindObject(ObjectType::POINT, SubType::RECTSHAPE, 1);
		P1.pCadPoint->SetPoint(MousePos);
		P2.pCadObject = FindObject(ObjectType::POINT, SubType::RECTSHAPE, 2);
		P2.pCadPoint->SetPoint(CalculateP2(PC.pCadPoint,P1.pCadPoint));
		DrawState = ObjectDrawState::ARCSTART_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("ARC CENTERED:Locate Start of Arc"));
		break;
	case ObjectDrawState::ARCSTART_LBUTTON_DOWN:
		break;
	case ObjectDrawState::ARCSTART_LBUTTON_UP:
		PS.pCadObject = FindObject(ObjectType::POINT, SubType::STARTPOINT, 0);
		PS.pCadPoint->SetPoint(MousePos);
		DrawState = ObjectDrawState::ARCEND_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("ARC CENTERED:Locate End of Arc"));
		break;
	case ObjectDrawState::ARCEND_LBUTTON_DOWN:
		DrawState = ObjectDrawState::ARCEND_LBUTTON_UP;
		break;
	case ObjectDrawState::ARCEND_LBUTTON_UP:
		PE.pCadObject = FindObject(ObjectType::POINT, SubType::ENDPOINT, 0);
		PE.pCadPoint->SetPoint(MousePos);
		GETVIEW->GetDocument()->AddObjectAtTail(this);
		GETVIEW->SetObjectTypes(new CCadArcCent);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("ARC CENTERED:Locate Center Point of Arc"));
		GETVIEW->Invalidate();
		break;
	}
	return DrawState;
}


ObjectDrawState CCadArcCent::MouseMove(ObjectDrawState DrawState)
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
	CADObjectTypes PC, P1, P2, PS, PE;

	switch (DrawState)
	{
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		PC.pCadObject = FindObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
		P1.pCadObject = FindObject(ObjectType::POINT, SubType::RECTSHAPE, 1);
		P1.pCadPoint->SetPoint(MousePos);
		P2.pCadObject = FindObject(ObjectType::POINT, SubType::RECTSHAPE, 2);
		P2.pCadPoint->SetPoint(CalculateP2(PC.pCadPoint, P1.pCadPoint));
		break;
	case ObjectDrawState::ARCSTART_LBUTTON_DOWN:
		PS.pCadObject = FindObject(ObjectType::POINT, SubType::STARTPOINT, 0);
		PS.pCadPoint->SetPoint(MousePos);
		break;
	case ObjectDrawState::ARCEND_LBUTTON_DOWN:
		PE.pCadObject = FindObject(ObjectType::POINT, SubType::ENDPOINT, 0);
		PE.pCadPoint->SetPoint(MousePos);
		break;
	}
	GETVIEW->Invalidate();
	return DrawState;
}

DOUBLEPOINT CCadArcCent::CalculateP2(CCadPoint* pPC, CCadPoint* pP1)
{
	DOUBLEPOINT PC, P1, P2;

	P1 = DOUBLEPOINT(*pP1);
	PC = DOUBLEPOINT(*pPC);
	P2 = (PC * 2.0) - P1;
	return P2;
}

/*
void CCadArcCent::DrawArc(CDC* pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale)
{
	//------------------------------
	// For right now, going to use
	// brute force and ignorance
	//-----------------------------

	int RadiusX, RadiusY;
	double A, B, x, y;
	CPoint Point, lastPoint;
	CDoubleSize SlopeIsOne;

	A = m_Shape.dCX;
	A *= A;
	B = m_Shape.dCY;
	B *= B;
	SlopeIsOne = SlopeIsOneAt(A, B);
	RadiusX = GETAPP.RoundDoubleToInt(SlopeIsOne.dCX * PixelsPerInch);
	RadiusY = GETAPP.RoundDoubleToInt(SlopeIsOne.dCY * PixelsPerInch);
	printf("Start\n");
	for (int i = 0; i < RadiusX; i++)
	{
		x = double(i) * 0.01;
		y = CalcY(x, A, B);
		Point = (m_Center + CDoubleSize(x, y)).ToPixelPoint(Offset, Scale);
		printf("<%4d>Point(%4d,%4d)\n", i, Point.x, Point.y);
		pDC->SetPixel(Point, RGB(2550, 64, 255));
	}
	printf("End\n");
	for (int i = 0; i < RadiusY; i++)
	{
		y = double(i) * 0.01;
		x = CalcY(y, B, A);
		Point = (m_Center + CDoubleSize(x, y)).ToPixelPoint(Offset, Scale);
		printf("<%4d>Point(%4d,%4d)\n", i, Point.x, Point.y);
		pDC->SetPixel(Point, RGB(2550, 64, 255));
	}
}

double CCadArcCent::CalcY(double x, double A, double B)
{
	//-------------------------------------
	// CalcY
	// Calculate the y position for x
	// 
	// x....x coordinate
	// A....Major Axis Squared
	// B....Minor Axis Squared
	// reutrns:
	// y
	//-------------------------------------

	double Y;
	Y = sqrt(B * (1.0 - x * x / A));
	return Y;
}

BadDelta CCadArcCent::DeltaIsBad(CPoint P1, CPoint P2)
{
	BadDelta rV = BadDelta::GOOD;
	CSize diff = P1 - P2;
	if (abs(diff.cx) > 1)
		rV = BadDelta::X;
	else if (abs(diff.cy) > 1)
		rV = BadDelta::Y;
	return rV;
}

CDoubleSize CCadArcCent::SlopeIsOneAt(double Asquared, double Bsquared)
{
	//------------------------------
	// SlopeIsOneAt
	//	This method is to find the
	// place on the curve where the
	// slope is one
	// parameters:
	//	Asquared.....Major Axis Squared
	//	Bsquared.....Minor Axis Squared
	//------------------------------

	double x;
	double y;

	x = Asquared * sqrt(1.0 / (Asquared + Bsquared));
	y = Bsquared * sqrt(1.0 / (Asquared + Bsquared));
	return CDoubleSize(x, y);
}
*/
