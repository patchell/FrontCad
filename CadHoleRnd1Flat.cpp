#include "pch.h"


CCadHoleRnd1Flat::CCadHoleRnd1Flat():CCadObject()
{
	SetType(ObjectType::HOLE_RND1FLAT);
	GetName().Format(_T("RoundHole1Flat_%d"), ++m_RndHole1FlatCount);
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetRoundHole1FlatAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}

CCadHoleRnd1Flat::~CCadHoleRnd1Flat()
{
}

void CCadHoleRnd1Flat::Create()
{
	CADObjectTypes Obj;

	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create();
	Obj.pCadPoint->SetSubType(SubType::CENTERPOINT);
	Obj.pCadPoint->SetSubSubType(0);
	AddObjectAtTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create();
	Obj.pCadPoint->SetSubType(SubType::STARTPOINT);;
	Obj.pCadPoint->SetSubSubType(0);
	AddObjectAtTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create();
	Obj.pCadPoint->SetSubType(SubType::ENDPOINT);
	Obj.pCadPoint->SetSubSubType(0);
	AddObjectAtTail(Obj.pCadObject);
	SolveIntersection();
}


BOOL CCadHoleRnd1Flat::Destroy(CCadObject* pDependentObject)
{
	BOOL rV = TRUE;
	return rV;
}

void CCadHoleRnd1Flat::Move(CDoubleSize Diff)
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

void CCadHoleRnd1Flat::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
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

void CCadHoleRnd1Flat::Draw(CDC* pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale)
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
	CRect rect;
	CADObjectTypes ObjCenter, ObjStart, ObjEnd;
	CPoint start, end;
	double Radius;
	int nRadius;
	int Lw;
	double dS;

	if (IsRenderEnabled())
	{
		Radius = GetAttributes().m_HoleRadius;
		dS = Radius / 3.0;
		nRadius = GETAPP.RoundDoubleToInt(Radius * Scale.GetScaleX());
		ObjCenter.pCadObject = FindObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
		ObjStart.pCadObject = FindObject(ObjectType::POINT, SubType::STARTPOINT, 0);
		ObjEnd.pCadObject = FindObject(ObjectType::POINT, SubType::ENDPOINT, 0);
		start = ObjStart.pCadPoint->ToPixelPoint(ULHC, Scale);
		end = ObjEnd.pCadPoint->ToPixelPoint(ULHC, Scale);
		rect.SetRect(
			ObjCenter.pCadPoint->ToPixelPoint(ULHC, Scale) - CSize(nRadius, nRadius),
			ObjCenter.pCadPoint->ToPixelPoint(ULHC, Scale) + CSize(nRadius, nRadius)
		);
		Lw = GETAPP.RoundDoubleToInt(Scale.GetScaleX() * GetAttributes().m_LineWidth);
		if (Lw < 1) Lw = 1;

		CreateThePen(mode, &penLine, Lw);
		switch (mode.DrawMode)
		{
		case ObjectDrawMode::FINAL:
		case ObjectDrawMode::SELECTED:
		case ObjectDrawMode::SKETCH:
			pDC->SelectStockObject(NULL_BRUSH);
			pOldPen = pDC->SelectObject(&penLine);
			pDC->Arc(
				&rect, 
				ObjStart.pCadPoint->ToPixelPoint(ULHC, Scale), 
				ObjEnd.pCadPoint->ToPixelPoint(ULHC, Scale)
			);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(dS, dS), pDC, ULHC, Scale);;
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(-dS, dS), pDC, ULHC, Scale);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(dS, -dS), pDC, ULHC, Scale);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(-dS, -dS), pDC, ULHC, Scale);
			pDC->SelectObject(pOldPen);
			break;
		}
	}
}

void CCadHoleRnd1Flat::SolveIntersection()
{
	CADObjectTypes ObjCenter;
	CADObjectTypes ObjStart;
	CADObjectTypes ObjEnd;
	double Radius;
	double FlatDist;

	Radius = GetAttributes().m_HoleRadius;
	FlatDist = GetAttributes().m_FlatDistanceFromCenter;

	ObjCenter.pCadObject = FindObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
	ObjStart.pCadObject = FindObject(ObjectType::POINT, SubType::STARTPOINT, 0);
	ObjEnd.pCadObject = FindObject(ObjectType::POINT, SubType::ENDPOINT, 0);
	//--------------------------------
	// Start
	//--------------------------------
	ObjStart.pCadPoint->SetX(ObjCenter.pCadPoint->GetX() + FlatDist);
	ObjStart.pCadPoint->SetY(ObjCenter.pCadPoint->GetY() - sqrt(Radius * Radius - FlatDist * FlatDist));
	//--------------------------------
	//	End Point
	//--------------------------------
	ObjEnd.pCadPoint->SetX(ObjCenter.pCadPoint->GetX() + FlatDist);
	ObjEnd.pCadPoint->SetY(ObjCenter.pCadPoint->GetY() + sqrt(Radius * Radius - FlatDist * FlatDist));
}

BOOL CCadHoleRnd1Flat::PointInThisObject(DOUBLEPOINT point)
{
	return 0;
}

int CCadHoleRnd1Flat::PointInObjectAndSelect(
	DOUBLEPOINT p,
	CCadObject** ppSelList,
	int index,
	int n
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
				n
			);
			index += ix;
		}
	}
	return index;
}

CString& CCadHoleRnd1Flat::GetTypeString(void)
{
	//---------------------------------------------------
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csTypeName = CString(_T("Round Hole 1 Flat"));
	return csTypeName;
}

CString& CCadHoleRnd1Flat::GetObjDescription()
{
	GetDescription().Format(_T("Base Obj Class"));
	return GetDescription();
}

CCadObject * CCadHoleRnd1Flat::CopyObject(void)
{
	//---------------------------------------------------
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadHoleRnd1Flat *pHR1F = new CCadHoleRnd1Flat;
	CCadObject::CopyObject(pHR1F);
	return pHR1F;
}

DocFileParseToken CCadHoleRnd1Flat::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
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

void CCadHoleRnd1Flat::CopyAttributesTo(SRndHole1FlatAttributes *pAttrib)
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
	GetAttributes().CopyFrom(pAttrib);
}

void CCadHoleRnd1Flat::CopyAttributesFrom(SRndHole1FlatAttributes *pAttrib)
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

ObjectDrawState CCadHoleRnd1Flat::ProcessDrawMode(ObjectDrawState DrawState)
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
		GETAPP.UpdateStatusBar(_T("Round Hole with One Flat:Place Center Point"));
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
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		Obj.pCadObject = FindObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
		Obj.pCadPoint->SetPoint(MousePos);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		Obj.pCadHoleRnd1Flat = new CCadHoleRnd1Flat;
		Obj.pCadHoleRnd1Flat->Create();
		GETVIEW->SetObjectTypes(Obj.pCadObject);
		GETAPP.UpdateStatusBar(_T("Round Hole with One Flat Side:Place Center Point"));
		GETVIEW->Invalidate();
		break;
	}
	return DrawState;
}

ObjectDrawState CCadHoleRnd1Flat::MouseMove(ObjectDrawState DrawState)
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
		Obj.pCadObject = FindObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
		Obj.pCadPoint->SetPoint(MousePos);
		GETVIEW->Invalidate();
		break;
	}
	return DrawState;
}

int CCadHoleRnd1Flat::EditProperties()
{
	int Id;
	CDlgRndHole1Flat Dlg;

	Dlg.SetRndHole1Flat(this);
	Id = Dlg.DoModal();
	if (IDOK == Id)
	{
		if (Dlg.IsDirty())
		{
			CopyAttributesTo(&m_CurrentAttributes);
			m_AttributesDirty = TRUE;
		}
	}
	return Id;
}

void CCadHoleRnd1Flat::CreateThePen(MODE mode, CPen* pen, int Lw)
{
	switch (mode.DrawMode)
	{
	case ObjectDrawMode::FINAL:
		pen->CreatePen(PS_SOLID, Lw, GetAttributes().m_colorLine);
		break;
	case ObjectDrawMode::SELECTED:
		pen->CreatePen(PS_SOLID, Lw, GetAttributes().m_colorLine);
		break;
	case ObjectDrawMode::SKETCH:
		pen->CreatePen(PS_DOT, Lw, GetAttributes().m_colorLine);
		break;
	}
}
