#include "pch.h"

CCadArc::CCadArc():CCadObject(CCadObject::ObjectType::ARC)
{
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

BOOL CCadArc::Create(CCadObject* pParent, SubTypes type)
{
	CADObjectTypes Obj;

	CCadObject::Create(pParent, type);
	if (pParent == NULL)
		pParent = this;
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::CENTERPOINT);
	AddObjectAtTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::STARTPOINT);
	AddObjectAtTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::ENDPOINT);
	AddObjectAtTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::ARC_RECTSHAPE);
	Obj.pCadPoint->SetSubSubType(1);
	AddObjectAtTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::ARC_RECTSHAPE);
	Obj.pCadPoint->SetSubSubType(2);
	AddObjectAtTail(Obj.pCadObject);
	return TRUE;
}

void CCadArc::UpdateEnclosure()
{
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

void CCadArc::Save(
	CFile* pcfFile,
	int Indent
)
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

	///------------------------------------------
	/// Save
	///		This function is called whenever the
	/// user saves a document of library.
	///
	/// parameter:
	///		pO.....pointer to Output File Stream
	///------------------------------------------
	char* IndentString = new char[256];
	char* s1 = new char[1024];
	int BuffLen;
	CString csOut;

	GETAPP.IndentString(IndentString, 256, Indent, ' ');
	csOut.Format(
		_T("%hs%hs(\n"),
		IndentString,
		GETAPP.ConvertCStringToChar(s1, this->GetTypeString())
	);
	BuffLen = sprintf_s(s1, 1024, "%hs", GETAPP.ConvertCStringToChar(s1, csOut));
	pcfFile->Write(s1, BuffLen);
	FindObject(ObjectType::POINT, CCadObject::SubTypes::ARC_RECTSHAPE, 1)->Save(pcfFile, Indent + 2);
	FindObject(ObjectType::POINT, CCadObject::SubTypes::ARC_RECTSHAPE, 2)->Save(pcfFile, Indent + 2);
	FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0)->Save(pcfFile, Indent + 2);
	FindObject(ObjectType::POINT, CCadObject::SubTypes::STARTPOINT, 0)->Save(pcfFile, Indent + 2);
	FindObject(ObjectType::POINT, CCadObject::SubTypes::ENDPOINT, 0)->Save(pcfFile, Indent + 2);
	GetAttributes().Save(pcfFile,Indent+2);
	csOut.Format( _T("%hs)"), IndentString);
	BuffLen = sprintf_s(s1, 1024, "%hs", GETAPP.ConvertCStringToChar(s1, csOut));
	pcfFile->Write(s1, BuffLen);
	delete[] s1;
	delete[] IndentString;
}

void CCadArc::Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale)
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
	CBrush brushFill, * pBrushOld;
	CADObjectTypes ObjP1, ObjP2, ObjStart, ObjEnd, ObjCenter;
	int Lw;

	if (IsRenderEnabled())
	{
		ObjP1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARC_RECTSHAPE, 1);
		ObjP2.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARC_RECTSHAPE, 2);
		ObjStart.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::STARTPOINT, 0);
		ObjEnd.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ENDPOINT, 0);
		ObjCenter.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0);
		brushFill.CreateStockObject(NULL_BRUSH);

		switch (mode.PaintMode)
		{
		case MODE::ObjectPaintMode::FINAL:
			Lw = GETAPP.RoundDoubleToInt(GetLineWidth() * Scale.dSX);
			if (Lw < 1)
				Lw = 1;
			if (IsSelected())
				penLine.CreatePen(PS_SOLID, Lw, GetAttributes().m_colorSelected);
			else
				penLine.CreatePen(PS_SOLID, Lw, GetAttributes().m_colorLine);
			pOldPen = pDC->SelectObject(&penLine);
			ObjP1.pCadPoint->ToPixelArc(ObjP2.pCadPoint, ObjStart.pCadPoint, ObjEnd.pCadPoint, pDC, LLHC, Scale);
			pDC->SelectObject(pOldPen);
			break;
		case MODE::ObjectPaintMode::SKETCH:
			pBrushOld = pDC->SelectObject(&brushFill);
			penLine.CreatePen(PS_SOLID, 1, GetAttributes().m_colorSelected);
			pOldPen = pDC->SelectObject(&penLine);
			ObjP1.pCadPoint->ToPixelRect(ObjP2.pCadPoint, pDC, LLHC, Scale);
			ObjP1.pCadPoint->ToPixelEllipse(ObjP2.pCadPoint, pDC, LLHC, Scale);
			pDC->SelectObject(pBrushOld);
			pDC->SelectObject(pOldPen);
			break;
		case MODE::ObjectPaintMode::ARCSTART:
			penLine.CreatePen(PS_SOLID, 1, GetAttributes().m_colorSelected);
			pOldPen = pDC->SelectObject(&penLine);
			pBrushOld = pDC->SelectObject(&brushFill);
			ObjP1.pCadPoint->ToPixelEllipse(ObjP2.pCadPoint, pDC, LLHC, Scale);
			ObjCenter.pCadPoint->MoveTo(pDC, LLHC, Scale);
			ObjStart.pCadPoint->LineTo(pDC, LLHC, Scale);
			pDC->SelectObject(pBrushOld);
			pDC->SelectObject(pOldPen);
			break;
		case MODE::ObjectPaintMode::ARCEND:
			penLine.CreatePen(PS_SOLID, 1, GetAttributes().m_colorSelected);
			pOldPen = pDC->SelectObject(&penLine);
			ObjP1.pCadPoint->ToPixelArc(
				ObjP2.pCadPoint,
				ObjStart.pCadPoint,
				ObjEnd.pCadPoint,
				pDC,
				LLHC,
				Scale
			);
			ObjCenter.pCadPoint->MoveTo(pDC,LLHC, Scale);
			ObjEnd.pCadPoint->LineTo(pDC, LLHC, Scale);
			pDC->SelectObject(pOldPen);
			break;
		}
	}
}

BOOL CCadArc::IsPointEnclosed(DOUBLEPOINT p)
{
	return 0;
}

BOOL CCadArc::PointInThisObject(DOUBLEPOINT point)
{
	BOOL rV = FALSE;
	double StartAngle, EndAngle, Angle;
	CCadRect Rect;
	CADObjectTypes P1, P2, ObjCenter;

	//------------------------------
	// See if the point is in the
	// shape rectangle
	//------------------------------
	P1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARC_RECTSHAPE, 1);
	P2.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARC_RECTSHAPE, 2);
	ObjCenter.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0);
	Rect.Create(NULL, CCadObject::SubTypes::DEFAULT);
	Rect.SetRect(P1.pCadPoint, P2.pCadPoint);
	if (Rect.PointInThisObject(point))
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

			A = Rect.GetWidth() / 2.0;
			B = Rect.GetHeight() / 2.0;
			rV = GETAPP.TestEllipsePoint(A, B, point, DOUBLEPOINT(*ObjCenter.pCadPoint), TOLERANCE_10_PERCENT);
		}
	}
	return rV;
}

int CCadArc::PointInObjectAndSelect(
	DOUBLEPOINT p,
	CCadObject* pExcludeObject,
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
	//	n...........Total number of spaces in selection list
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


CString& CCadArc::GetTypeString()
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

CCadObject * CCadArc::Copy()
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
	newObj.pCadArc->Create(GetParent(), CCadObject::SubTypes::DEFAULT);
	CCadObject::CopyObject(newObj.pCadObject);
	newObj.pCadArc->CopyAttributesFrom(GetPtrToAttributes());
	return newObj.pCadObject;
}

void CCadArc::CopyAttributes(CCadObject* pToObj)
{
	((CCadArc*)pToObj)->CopyAttributesFrom(GetPtrToAttributes());
}

CDoubleSize CCadArc::GetSize()
{
	//--------------------------------------------------
	// GetSize
	//	Get the size of the object.  Returns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	CADObjectTypes P1, P2;
	CDoubleSize size;

	P1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARC_RECTSHAPE, 1);
	P2.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARC_RECTSHAPE, 2);
	size = CDoubleSize(
		P1.pCadPoint->GetX() - P2.pCadPoint->GetX(),
		P2.pCadPoint->GetY() - P1.pCadPoint->GetY()
	);
	return size;
}

void CCadArc::Parse(
	CParser *pParser,
	Token TypeToken// Token type to save object as
)
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
		Obj1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARC_RECTSHAPE, 1);
		Obj1.pCadPoint->SetPoint(MousePos);
		Obj2.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARC_RECTSHAPE, 2);
		Obj2.pCadPoint->SetPoint(MousePos);
		Center.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0);
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
		Obj1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARC_RECTSHAPE, 1);
		Obj2.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARC_RECTSHAPE, 2);
		Obj2.pCadPoint->SetPoint(MousePos);
		Center.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0);
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
		Obj1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::STARTPOINT, 0);
		Obj1.pCadPoint->SetPoint(MousePos);
		DrawState = ObjectDrawState::ARCEND_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("ARC:Place End of ARC"));
		GETVIEW->Invalidate();
		break;
	case ObjectDrawState::ARCEND_LBUTTON_DOWN:
		DrawState = ObjectDrawState::ARCEND_LBUTTON_UP;
		break;
	case ObjectDrawState::ARCEND_LBUTTON_UP:
		Obj1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ENDPOINT, 0);
		Obj1.pCadPoint->SetPoint(MousePos);
		GetParent()->AddObjectAtTail(this);
		Obj1.pCadArc = new CCadArc;
		Obj1.pCadArc->Create(GetParent(), CCadObject::SubTypes::DEFAULT);
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
		Obj1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARC_RECTSHAPE, 1);
		Obj2.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARC_RECTSHAPE, 2);
		Obj2.pCadPoint->SetPoint(MousePos);
		Center.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0);
		Center.pCadPoint->SetPoint(
			GETAPP.CalcCenter(
				DOUBLEPOINT(*Obj1.pCadPoint),
				DOUBLEPOINT(*Obj2.pCadPoint)
			)
		);
		break;
	case ObjectDrawState::ARCSTART_LBUTTON_DOWN:
		Obj1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::STARTPOINT, 0);
		Obj1.pCadPoint->SetPoint(MousePos);
		break;
	case ObjectDrawState::ARCEND_LBUTTON_DOWN:
		Obj1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ENDPOINT, 0);
		Obj1.pCadPoint->SetPoint(MousePos);
		break;
	}
	GETVIEW->Invalidate();
	return DrawState;
}

double CCadArc::CalcY(double x, double A, double B)
{
	//-------------------------------------
	// CalcY
	// Calculate the y position for x
	// 
	// x....x coordinate
	// A....Major Axis Squared
	// B....Minor Axis Squared
	// returns:
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

int CCadArc::EditProperties()
{
	INT_PTR Id;
	CDlgArcProperties Dlg;

	Dlg.SetArc(this);
	Id = Dlg.DoModal();
	return int(Id);
}
