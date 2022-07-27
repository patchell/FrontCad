#include "pch.h"

CCadHoleRnd2Flat::CCadHoleRnd2Flat():CCadObject()
{
	SetType(ObjectType::HOLE_RND2FLAT);
	GetName().Format(_T("RoundHole2Flat_%d"), ++m_RoundHole2FlatCount);
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetRoundHole2FlatAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}

CCadHoleRnd2Flat::~CCadHoleRnd2Flat()
{
}

BOOL CCadHoleRnd2Flat::Create(CCadObject* pParent, CCadObject* pOrigin)
{
	CADObjectTypes Obj;

	CCadObject::Create(pParent, pOrigin);
	if (pParent == NULL)
		pParent = this;
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, pOrigin);
	Obj.pCadPoint->SetSubType(SubType::CENTERPOINT);
	Obj.pCadPoint->SetSubSubType(0);
	AddObjectAtChildTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, pOrigin);
	Obj.pCadPoint->SetSubType(SubType::STARTPOINT);
	Obj.pCadPoint->SetSubSubType(1);
	AddObjectAtChildTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, pOrigin);
	Obj.pCadPoint->SetSubType(SubType::ENDPOINT);
	Obj.pCadPoint->SetSubSubType(1);
	AddObjectAtChildTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, pOrigin);
	Obj.pCadPoint->SetSubType(SubType::STARTPOINT);
	Obj.pCadPoint->SetSubSubType(2);
	AddObjectAtChildTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, pOrigin);
	Obj.pCadPoint->SetSubType(SubType::ENDPOINT);
	Obj.pCadPoint->SetSubSubType(2);
	AddObjectAtChildTail(Obj.pCadObject);
	SolveIntersection();
	return TRUE;
}

BOOL CCadHoleRnd2Flat::Destroy(CCadObject* pDependentObject)
{
	BOOL rV = TRUE;
	return rV;
}

void CCadHoleRnd2Flat::Move(CDoubleSize Diff)
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

void CCadHoleRnd2Flat::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
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

void CCadHoleRnd2Flat::Draw(CDC* pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale)
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
	CPen* pOldPen, penLine;
	CRect rect;
	CADObjectTypes ObjCenter;
	CADObjectTypes ObjStart1, ObjEnd1;
	CADObjectTypes ObjStart2, ObjEnd2;
	CPoint start1, end1;
	CPoint start2, end2;
	double Radius;
	int nRadius;
	int Lw;
	double dS;


	if (IsRenderEnabled())
	{
		Radius = GetAttributes().m_HoleRadius;
		dS = Radius / 3.0;
		nRadius = GETAPP.RoundDoubleToInt(Radius * Scale.GetScaleX());
		ObjCenter.pCadObject = FindChildObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
		ObjStart1.pCadObject = FindChildObject(ObjectType::POINT, SubType::STARTPOINT, 1);
		ObjEnd1.pCadObject = FindChildObject(ObjectType::POINT, SubType::ENDPOINT, 1);
		ObjStart2.pCadObject = FindChildObject(ObjectType::POINT, SubType::STARTPOINT, 2);
		ObjEnd2.pCadObject = FindChildObject(ObjectType::POINT, SubType::ENDPOINT, 2);
		start1 = ObjStart1.pCadPoint->ToPixelPoint(ULHC, Scale);
		end1 = ObjEnd1.pCadPoint->ToPixelPoint(ULHC, Scale);
		start2 = ObjStart2.pCadPoint->ToPixelPoint(ULHC, Scale);
		end2 = ObjEnd2.pCadPoint->ToPixelPoint(ULHC, Scale);
		rect.SetRect(
			ObjCenter.pCadPoint->ToPixelPoint(ULHC, Scale) - CSize(nRadius, nRadius),
			ObjCenter.pCadPoint->ToPixelPoint(ULHC, Scale) + CSize(nRadius, nRadius)
		);
		Lw = GETAPP.RoundDoubleToInt(GetAttributes().m_LineWidth * Scale.GetScaleX());
		if (Lw < 1) Lw = 1;

		CreateThePen(mode, &penLine, Lw);
		switch (mode.DrawMode)
		{
		case ObjectDrawMode::FINAL:
		case ObjectDrawMode::SELECTED:
		case ObjectDrawMode::SKETCH:
			pOldPen = pDC->SelectObject(&penLine);
			pDC->Arc(rect, start1, end1);
			pDC->Arc(rect, start2, end2);
			pDC->MoveTo(start1);
			pDC->LineTo(start2);
			pDC->MoveTo(end1);
			pDC->LineTo(end2);

			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(dS, dS), pDC, ULHC, Scale);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(-dS, dS), pDC, ULHC, Scale);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(dS, -dS), pDC, ULHC, Scale);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(-dS, -dS), pDC, ULHC, Scale);

			pDC->SelectObject(pOldPen);
			break;
		}
	}
}

BOOL CCadHoleRnd2Flat::PointInThisObject(DOUBLEPOINT point)
{
	return 0;
}

void CCadHoleRnd2Flat::SolveIntersection()
{
	CADObjectTypes ObjCenter;
	CADObjectTypes ObjStart1;
	CADObjectTypes ObjEnd1;
	CADObjectTypes ObjStart2;
	CADObjectTypes ObjEnd2;
	double Radius;
	double FlatDist;

	Radius = GetAttributes().m_HoleRadius;
	FlatDist = GetAttributes().m_FlatDistanceFromCenter;

	ObjCenter.pCadObject = FindChildObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
	ObjStart1.pCadObject = FindChildObject(ObjectType::POINT, SubType::STARTPOINT, 0);
	ObjEnd1.pCadObject = FindChildObject(ObjectType::POINT, SubType::ENDPOINT, 0);
	ObjStart2.pCadObject = FindChildObject(ObjectType::POINT, SubType::STARTPOINT, 0);
	ObjEnd2.pCadObject = FindChildObject(ObjectType::POINT, SubType::ENDPOINT, 0);
	//--------------------------------
	// Start First Flat
	//--------------------------------
	ObjStart1.pCadPoint->SetX(ObjCenter.pCadPoint->GetX() + FlatDist);
	ObjStart1.pCadPoint->SetY(ObjCenter.pCadPoint->GetY() - sqrt(Radius * Radius - FlatDist * FlatDist));
	//--------------------------------
	//	First End Point
	//--------------------------------
	ObjEnd1.pCadPoint->SetX(ObjCenter.pCadPoint->GetX() + FlatDist);
	ObjEnd1.pCadPoint->SetY(ObjCenter.pCadPoint->GetY() + sqrt(Radius * Radius - FlatDist * FlatDist));

	//--------------------------------
	// Start Second Flat
	//--------------------------------
	ObjStart2.pCadPoint->SetX(ObjCenter.pCadPoint->GetX() - FlatDist);
	ObjStart2.pCadPoint->SetY(ObjCenter.pCadPoint->GetY() + sqrt(Radius * Radius - FlatDist * FlatDist));
	//--------------------------------
	//	Second End Point
	//--------------------------------
	ObjEnd2.pCadPoint->SetX(ObjCenter.pCadPoint->GetX() - FlatDist);
	ObjEnd2.pCadPoint->SetY(ObjCenter.pCadPoint->GetY() - sqrt(Radius * Radius - FlatDist * FlatDist));
}

int CCadHoleRnd2Flat::PointInObjectAndSelect(
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

	if (index < n)
	{
		//---------------------------------------
		// is point in the Arc?
		//---------------------------------------
		if (PointInThisObject(p))
		{
			if (IsItThisKind(nKinds))
				ppSelList[index++] = this;
		}
		index = CCadObject::PointInObjectAndSelect(
			p,
			ppSelList,
			index,
			n,
			nKinds
		);
	}
	return index;
}

CString& CCadHoleRnd2Flat::GetTypeString(void)
{
	//---------------------------------------------------
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csName = _T("Rnd Hole 2 Flats");
	return csName;
}

CString& CCadHoleRnd2Flat::GetObjDescription()
{
	GetDescription().Format(_T("Base Obj Class"));
	return GetDescription();
}

CCadObject * CCadHoleRnd2Flat::CopyObject(void)
{
	//---------------------------------------------------
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadHoleRnd2Flat* pHR2F = new CCadHoleRnd2Flat;
	CCadObject::CopyObject(pHR2F);
	return pHR2F;
}

CDoubleSize CCadHoleRnd2Flat::GetSize()
{
	//---------------------------------------------------
	// GetSize
	//	Get the size of the object.  Reutrns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	double Radius = GetHoleRadius();
	return CDoubleSize(Radius,Radius);
}

DocFileParseToken CCadHoleRnd2Flat::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
{
	//---------------------------------------------------
	// Parse
	//	This Method is used to parse this 
	// object out of an input stream
	//
	// parameters:
	//	Token....Lookahead Token for this object
	//	pLex.....Pointer to Lexer for the data stream
	//	TypeToken..Token type for this parse
	// return value:
	//	returns lookahead token on success, or
	//			negative value on error
	//--------------------------------------------------
	return Token;
}

void CCadHoleRnd2Flat::CopyAttributesTo(SRndHole2FlatAttributes *pAttrib)
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

void CCadHoleRnd2Flat::CopyAttributesFrom(SRndHole2FlatAttributes *pAttrib)
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

ObjectDrawState CCadHoleRnd2Flat::ProcessDrawMode(ObjectDrawState DrawState)
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
		GETAPP.UpdateStatusBar(_T("Round Hole with Two Flat Side"));
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
		Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
		Obj.pCadPoint->SetPoint(MousePos);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETVIEW->GetDocument()->AddObjectAtTail(this);
		Obj.pCadHoleRnd2Flat = new CCadHoleRnd2Flat;
		Obj.pCadHoleRnd2Flat->Create(NULL, GETVIEW->GetDocument()->GetCurrentOrigin());
		GETVIEW->SetObjectTypes(Obj.pCadObject);
		GETAPP.UpdateStatusBar(_T("Round Hole with Two Flat Side"));
		GETVIEW->Invalidate();
		break;
	}
	return DrawState;
}

ObjectDrawState CCadHoleRnd2Flat::MouseMove(ObjectDrawState DrawState)
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
		Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
		Obj.pCadPoint->SetPoint(MousePos);
		GETVIEW->Invalidate();
		break;
	}
	return DrawState;
}

int CCadHoleRnd2Flat::EditProperties()
{
	int Id;
	CDlgRndHole2FlatProp Dlg;

	Dlg.SetRndHole2Flat(this);
	Id = Dlg.DoModal();
	return Id;
}

void CCadHoleRnd2Flat::CreateThePen(MODE mode, CPen* pen, int Lw)
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