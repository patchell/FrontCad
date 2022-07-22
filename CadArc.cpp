#include "pch.h"

CCadArc::CCadArc():CCadObject()
{
	SetType(ObjectType::ARC);
	GetName().Format(_T("ARC_%d"), ++m_ArcCount);
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetArcAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}

CCadArc::~CCadArc()
{
}

void CCadArc::Create()
{
	CADObjectTypes Obj;

	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create();
	Obj.pCadPoint->SetSubType(SubType::CENTERPOINT);
	Obj.pCadPoint->SetSubSubType(0);
	AddObjectAtChildTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create();
	Obj.pCadPoint->SetSubType(SubType::STARTPOINT);
	Obj.pCadPoint->SetSubSubType(0);
	AddObjectAtChildTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create();
	Obj.pCadPoint->SetSubType(SubType::ENDPOINT);
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

BOOL CCadArc::Destroy(CCadObject* pDependentObjects)
{
	return 0;
}

void CCadArc::Move(CDoubleSize Diff)
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

void CCadArc::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
{
	//--------------------------------------------------
	// Save
	//		This Method save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
	char* pIndentString1 = new char[256];

//	fprintf(pO, "%s%s(%s(%8.3lf,%8.3lf),%s(%8.3lf,%8.3lf))\n",
//		GETAPP.MkIndentString(pIndentString1, Indent, ' '),
//		CLexer::TokenToString(DocFileParseToken::ARC),
//		CLexer::TokenToString(DocFileParseToken::POINT),
//		m_pointStart.dX, m_pointStart.dY,
//		CLexer::TokenToString(DocFileParseToken::POINT),
//		m_pointEnd.dX, m_pointEnd.dY
//	);
	GetAttributes().Save(pO,Indent+1,flags);
}

void CCadArc::Draw(CDC* pDC, MODE mode, CCadPoint ULHC, CScale Scale)
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
	CPen *pOldPen = 0, penLine;
	CRect rect;
	CADObjectTypes ObjP1, ObjP2, ObjStart, ObjEnd, ObjCenter;
	int Lw;

	if (IsRenderEnabled())
	{
		Lw = GETAPP.RoundDoubleToInt(GetLineWidth() * Scale.m_ScaleX);
		if (Lw < 1)
			Lw = 1;
		ObjP1.pCadObject = FindChildObject(ObjectType::POINT, SubType::RECTSHAPE, 1);
		ObjP2.pCadObject = FindChildObject(ObjectType::POINT, SubType::RECTSHAPE, 2);
		ObjStart.pCadObject = FindChildObject(ObjectType::POINT, SubType::STARTPOINT, 0);
		ObjEnd.pCadObject = FindChildObject(ObjectType::POINT, SubType::ENDPOINT, 0);

		switch (mode.DrawMode)
		{
		case ObjectDrawMode::FINAL:
			penLine.CreatePen(PS_SOLID, Lw, GetAttributes().m_colorLine);
			pOldPen = pDC->SelectObject(&penLine);
			rect.SetRect(
				ObjP1.pCadPoint->ToPixelPoint(ULHC, Scale),
				ObjP2.pCadPoint->ToPixelPoint(ULHC, Scale)
			);
			pDC->Arc(
				&rect, 
				ObjStart.pCadPoint->ToPixelPoint(ULHC,Scale),
				ObjEnd.pCadPoint->ToPixelPoint(ULHC,Scale)
			);
			pDC->SelectObject(pOldPen);
			break;
		case ObjectDrawMode::SELECTED:
			pOldPen = pDC->SelectObject(&penLine);
			rect.SetRect(
				ObjP1.pCadPoint->ToPixelPoint(ULHC, Scale),
				ObjP2.pCadPoint->ToPixelPoint(ULHC, Scale)
			);
			pDC->Arc(
				&rect,
				ObjStart.pCadPoint->ToPixelPoint(ULHC, Scale),
				ObjEnd.pCadPoint->ToPixelPoint(ULHC, Scale)
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
			pDC->MoveTo(rect.CenterPoint());
			pDC->LineTo(ObjStart.pCadPoint->ToPixelPoint(ULHC,Scale));
			pDC->SelectObject(pOldPen);
			break;
		case ObjectDrawMode::ARCEND:
			pOldPen = pDC->SelectObject(&penLine);
			pDC->Arc(
				&rect, 
				ObjStart.pCadPoint->ToPixelPoint(ULHC,Scale),
				ObjEnd.pCadPoint->ToPixelPoint(ULHC,Scale)
			);
			pDC->MoveTo(rect.CenterPoint());
			pDC->LineTo(ObjEnd.pCadPoint->ToPixelPoint(ULHC, Scale));
			pDC->SelectObject(pOldPen);
			break;
		}
	}
}

BOOL CCadArc::PointInThisObject(DOUBLEPOINT point)
{
	BOOL rV = FALSE;
	double StartAngle, EndAngle, Angle;
	CCadRect* pRect;
	CADObjectTypes P1, P2, ObjCenter;

	//------------------------------
	// See if the point is in the
	// shape rectangle
	//------------------------------
	P1.pCadObject = FindChildObject(ObjectType::POINT, SubType::RECTSHAPE, 1);
	P2.pCadObject = FindChildObject(ObjectType::POINT, SubType::RECTSHAPE, 2);
	ObjCenter.pCadObject = FindChildObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
	pRect = new CCadRect;
	pRect->Create();
	pRect->SetPoints(DOUBLEPOINT(*P1.pCadPoint), DOUBLEPOINT(*P2.pCadPoint), DOUBLEPOINT(*P1.pCadPoint));
	if (pRect->PointInThisObject(point))
	{
		//------------------------------
		// Is the angle between the
		// Start and End anble?
		//------------------------------
		StartAngle = GETAPP.ArcTan(
			P1.pCadPoint->GetX() - ObjCenter.pCadPoint->GetX(),
			ObjCenter.pCadPoint->GetY() - P1.pCadPoint->GetY()
		);
		EndAngle = GETAPP.ArcTan(
			P2.pCadPoint->GetX() - ObjCenter.pCadPoint->GetX(),
			ObjCenter.pCadPoint->GetY() - P2.pCadPoint->GetY()
		);
		Angle = GETAPP.ArcTan(
			point.dX - ObjCenter.pCadPoint->GetX(),
			ObjCenter.pCadPoint->GetY() - point.dX
		);
		if (GETAPP.CheckAngle(StartAngle, EndAngle, Angle))
		{
			//----------------------------
			// Check to see if the radius
			// is at least sort of close
			// to the actual radius.
			//----------------------------
			// 1 = x^2/A^2 + y^2/B^2
			//----------------------------
			double A, B;

			A = pRect->GetWidth() / 2.0;
			B = pRect->GetHeight() / 2.0;
			rV = GETAPP.TestEllipsePoint(A, B, point, DOUBLEPOINT(*ObjCenter.pCadPoint), TOLERANCE_10_PERCENT);
		}
	}
	delete pRect;
	return rV;
}

int CCadArc::PointInObjectAndSelect(
	DOUBLEPOINT p,
	CCadObject ** ppSelList , 
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

	if (index < n || n == 0)
	{
		//---------------------------------------
		// is point in the Arc?
		//---------------------------------------
		if (PointInThisObject(p))
		{
			if(IsItThisKind(nKinds))
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


CString& CCadArc::GetTypeString(void)
{
	//--------------------------------------------------
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csTypeName = CString(_T("Arc"));
	return csTypeName;
}

CString& CCadArc::GetObjDescription()
{
	GetDescription().Format(_T("Base Obj Class"));
	return GetDescription();
}

CCadObject * CCadArc::CopyObject(void)
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
	newObj.pCadArc = new CCadArc;
	newObj.pCadArc->Create();
	CCadObject::CopyObject(newObj.pCadObject);
	newObj.pCadArc->CopyAttributesFrom(GetPtrToAttributes());
	return newObj.pCadObject;
}

CDoubleSize CCadArc::GetSize()
{
	//--------------------------------------------------
	// GetSize
	//	Get the size of the object.  Reutrns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	CADObjectTypes P1, P2;
	CDoubleSize size;

	P1.pCadObject = FindChildObject(ObjectType::POINT, SubType::RECTSHAPE, 1);
	P2.pCadObject = FindChildObject(ObjectType::POINT, SubType::RECTSHAPE, 2);
	size = CDoubleSize(
		P1.pCadPoint->GetX() - P2.pCadPoint->GetX(),
		P2.pCadPoint->GetY() - P1.pCadPoint->GetY()
	);
	return size;
}

DocFileParseToken CCadArc::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
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

void CCadArc::CopyAttributesTo(SArcAttributes *pAttrib)
{
	//---------------------------------------------------
	//	GetAttributes
	//		This Method is used to copy the
	//	attributes from this object into one pointed 
	//	to by the parameter.
	//
	// Parameters:
	//	pAttrb.....pointer to attributes structure to copy into
	//----------------------------------------------------
	GetAttributes().CopyTo(pAttrib);
}

void CCadArc::CopyAttributesFrom(SArcAttributes *pAttrib)
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

ObjectDrawState CCadArc::ProcessDrawMode(ObjectDrawState DrawState)
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
	CADObjectTypes Obj1, Obj2, Center;

	switch (DrawState)
	{
	case ObjectDrawState::START_DRAWING:
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
		CopyAttributesFrom(&m_CurrentAttributes);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("ARC:Place First Corner of Circle Shape"));
		GETVIEW->EnableAutoScroll(TRUE);
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
		Obj1.pCadObject = FindChildObject(ObjectType::POINT, SubType::RECTSHAPE, 1);
		Obj1.pCadPoint->SetPoint(MousePos);
		Obj2.pCadObject = FindChildObject(ObjectType::POINT, SubType::RECTSHAPE, 2);
		Obj2.pCadPoint->SetPoint(MousePos);
		Center.pCadObject = FindChildObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
		Center.pCadPoint ->SetPoint(
			GETAPP.CalcCenter(
				DOUBLEPOINT(*Obj1.pCadPoint), 
				DOUBLEPOINT(*Obj2.pCadPoint)
			)
		);
		DrawState = ObjectDrawState::PLACE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Place Second Corner of Circle"));
		GETVIEW->Invalidate();
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		DrawState = ObjectDrawState::PLACE_LBUTTON_UP;
		break;
	case ObjectDrawState::PLACE_LBUTTON_UP:
		Obj2.pCadObject = FindChildObject(ObjectType::POINT, SubType::RECTSHAPE, 2);
		Obj2.pCadPoint->SetPoint(MousePos);
		Center.pCadObject = FindChildObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
		Center.pCadPoint->SetPoint(
			GETAPP.CalcCenter(
				DOUBLEPOINT(*Obj1.pCadPoint),
				DOUBLEPOINT(*Obj2.pCadPoint)
			)
		);
		DrawState = ObjectDrawState::ARCSTART_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("ARC:Place Begining of ARC"));
		break;
	case ObjectDrawState::ARCSTART_LBUTTON_DOWN:
		DrawState = ObjectDrawState::ARCSTART_LBUTTON_UP;
		break;
	case ObjectDrawState::ARCSTART_LBUTTON_UP:
		Obj1.pCadObject = FindChildObject(ObjectType::POINT, SubType::STARTPOINT, 0);
		Obj1.pCadPoint->SetPoint(MousePos);
		DrawState = ObjectDrawState::ARCEND_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("ARC:Place End of ARC"));
		GETVIEW->Invalidate();
		break;
	case ObjectDrawState::ARCEND_LBUTTON_DOWN:
		DrawState = ObjectDrawState::ARCEND_LBUTTON_UP;
		break;
	case ObjectDrawState::ARCEND_LBUTTON_UP:
		Obj1.pCadObject = FindChildObject(ObjectType::POINT, SubType::ENDPOINT, 0);
		Obj1.pCadPoint->SetPoint(MousePos);
		GETVIEW->GetDocument()->AddObjectAtTail(this);
		Obj1.pCadArc = new CCadArc;
		Obj1.pCadArc->Create();
		GETVIEW->SetObjectTypes(Obj1.pCadObject);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("ARC:Place First Corner of Circle Shape"));
		GETVIEW->Invalidate();
		break;
	}
	return DrawState;
}

ObjectDrawState CCadArc::MouseMove(ObjectDrawState DrawState)
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
	CDoubleSize MousePosDiff;
	CADObjectTypes Obj1, Obj2, Center;

	switch (DrawState)
	{
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		Obj1.pCadObject = FindChildObject(ObjectType::POINT, SubType::RECTSHAPE, 1);
		Obj2.pCadObject = FindChildObject(ObjectType::POINT, SubType::RECTSHAPE, 2);
		Obj2.pCadPoint->SetPoint(MousePos);
		Center.pCadObject = FindChildObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
		Center.pCadPoint->SetPoint(
			GETAPP.CalcCenter(
				DOUBLEPOINT(*Obj1.pCadPoint),
				DOUBLEPOINT(*Obj2.pCadPoint)
			)
		);
		break;
	case ObjectDrawState::ARCSTART_LBUTTON_DOWN:
		Obj1.pCadObject = FindChildObject(ObjectType::POINT, SubType::STARTPOINT, 0);
		Obj1.pCadPoint->SetPoint(MousePos);
		break;
	case ObjectDrawState::ARCEND_LBUTTON_DOWN:
		Obj1.pCadObject = FindChildObject(ObjectType::POINT, SubType::ENDPOINT, 0);
		Obj1.pCadPoint->SetPoint(MousePos);
		break;
	}
	GETVIEW->Invalidate();
	return DrawState;
}


/*
void CCadArc::DrawArc(CDC* pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale)
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
*/
double CCadArc::CalcY(double x, double A, double B)
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

BadDelta CCadArc::DeltaIsBad(CPoint P1, CPoint P2)
{
	BadDelta rV = BadDelta::GOOD;
	CSize diff = P1 - P2;
	if (abs(diff.cx) > 1)
		rV = BadDelta::X;
	else if (abs(diff.cy) > 1)
		rV = BadDelta::Y;
	return rV;
}

CDoubleSize CCadArc::SlopeIsOneAt(double Asquared, double Bsquared)
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

int CCadArc::EditProperties(void)
{
	int Id;
	CDlgArcProperties Dlg;

	Dlg.SetArc(this);
	Id = Dlg.DoModal();
	return Id;
}
