#include "pch.h"


CCadHoleRnd1Flat::CCadHoleRnd1Flat():CCadObject(ObjectType::HOLE_RND1FLAT)
{
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

BOOL CCadHoleRnd1Flat::Create(CCadObject* pParent, SubTypes type)
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
	SolveIntersection();
	return TRUE;
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

void CCadHoleRnd1Flat::Save(
	CFile* pcfFile,
	int Indent,
	int flags
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

void CCadHoleRnd1Flat::Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale)
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
	CBrush brushFill, * pOldBrush;
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
		dS = Radius / 3.0;	// for cross hairs
		//Radius double to int
		nRadius = GETAPP.RoundDoubleToInt(
			Radius * Scale.GetScaleX()
		);
		//Get object that define the hole
		ObjCenter.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0);
		ObjStart.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::STARTPOINT, 0);
		ObjEnd.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ENDPOINT, 0);
		//convert to view coordinates
		start = ObjStart.pCadPoint->ToPixelPoint(LLHC, Scale);
		end = ObjEnd.pCadPoint->ToPixelPoint(LLHC, Scale);
		//make a view rectangle
		rect.SetRect(
			ObjCenter.pCadPoint->ToPixelPoint(LLHC, Scale) - CSize(nRadius, nRadius),
			ObjCenter.pCadPoint->ToPixelPoint(LLHC, Scale) + CSize(nRadius, nRadius)
		);
		Lw = GETAPP.RoundDoubleToInt(Scale.GetScaleX() * GetAttributes().m_LineWidth);
		if (Lw < 1) Lw = 1;

		CreateThePen(mode, &penLine, Lw);
		brushFill.CreateStockObject(NULL_BRUSH);
		switch (mode.PaintMode)
		{
		case MODE::ObjectPaintMode::FINAL:
		case MODE::ObjectPaintMode::SKETCH:
			pOldBrush = pDC->SelectObject(&brushFill);
			pOldPen = pDC->SelectObject(&penLine);
			pDC->Arc(
				&rect, 
				ObjStart.pCadPoint->ToPixelPoint(LLHC, Scale), 
				ObjEnd.pCadPoint->ToPixelPoint(LLHC, Scale)
			);
			ObjStart.pCadPoint->LineFromHereToThere(ObjEnd.pCadPoint, pDC, LLHC, Scale);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(dS, dS), pDC, LLHC, Scale);;
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(-dS, dS), pDC, LLHC, Scale);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(dS, -dS), pDC, LLHC, Scale);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(-dS, -dS), pDC, LLHC, Scale);
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBrush);
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

	ObjCenter.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0);
	ObjStart.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::STARTPOINT, 0);
	ObjEnd.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ENDPOINT, 0);
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

BOOL CCadHoleRnd1Flat::IsPointEnclosed(DOUBLEPOINT p)
{
	return 0;
}

BOOL CCadHoleRnd1Flat::PointInThisObject(DOUBLEPOINT point)
{
	return 0;
}

int CCadHoleRnd1Flat::PointInObjectAndSelect(
	DOUBLEPOINT p,
	CCadObject* pExcludeObject,
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

	if (index < n)
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

CString& CCadHoleRnd1Flat::GetTypeString()
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

CCadObject * CCadHoleRnd1Flat::Copy()
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

void CCadHoleRnd1Flat::CopyAttributes(CCadObject* pToObj)
{
	((CCadHoleRnd1Flat*)pToObj)->CopyAttributesFrom(GetPtrToAttributes());
}

int CCadHoleRnd1Flat::Parse(
	CFile* pcfInFile,
	int Token,	// Lookahead Token
	CFileParser* pParser,	// pointer to parser
	int TypeToken// Token type to save object as
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
		SolveIntersection();
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
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0);
		Obj.pCadPoint->SetPoint(MousePos);
		SolveIntersection();
		GETVIEW->Invalidate();
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0);
		Obj.pCadPoint->SetPoint(MousePos);
		SolveIntersection();
		GetParent()->AddObjectAtTail(this);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		Obj.pCadHoleRnd1Flat = new CCadHoleRnd1Flat;
		Obj.pCadHoleRnd1Flat->Create(GetParent(),GetSubType());
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
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0);
		Obj.pCadPoint->SetPoint(MousePos);
		SolveIntersection();
		GETVIEW->Invalidate();
		break;
	default:
		SolveIntersection();
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
	return Id;
}

void CCadHoleRnd1Flat::CreateThePen(MODE mode, CPen* pen, int Lw)
{
	switch (mode.PaintMode)
	{
	case MODE::ObjectPaintMode::FINAL:
		if(IsSelected())
			pen->CreatePen(PS_SOLID, Lw, GetAttributes().m_colorLine);
		else
			pen->CreatePen(PS_SOLID, Lw, GetAttributes().m_colorSelected);
		break;
	case MODE::ObjectPaintMode::SKETCH:
		pen->CreatePen(PS_DOT, Lw, GetAttributes().m_colorSelected);
		break;
	}
}

void CCadHoleRnd1Flat::Print(const char* pTitle)
{
	CADObjectTypes ObjStart, ObjCenter, ObjEnd;
	char* s = new char[256];

	ObjCenter.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, SUBSUBTYPE_ANY);
	ObjStart.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::STARTPOINT, SUBSUBTYPE_ANY);
	ObjEnd.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ENDPOINT, SUBSUBTYPE_ANY);

	printf("%hs%hs  LW=%5.3lf  Flat=%6.3lf  Rad=%7.3lf\n", 
		pTitle, 
		GETAPP.ConvertCStringToChar(s, GetName()),
		GetAttributes().m_LineWidth,
		GetAttributes().m_FlatDistanceFromCenter,
		GetAttributes().m_HoleRadius
	);
	ObjCenter.pCadPoint->Print("  Center:");
	ObjStart.pCadPoint-> Print("   Start:");
	ObjEnd.pCadPoint->Print("     End:");
	delete[] s;

}
