#include "pch.h"

CCadHoleRnd2Flat::CCadHoleRnd2Flat():CCadObject(ObjectType::HOLE_RND2FLAT)
{
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

BOOL CCadHoleRnd2Flat::Create(CCadObject* pParent, SubTypes type)
{
	CADObjectTypes Obj;

	CCadObject::Create(pParent, type);
	if (pParent == NULL)
		pParent = this;
	//------------------ Center Point --------------------
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::CENTERPOINT);
	AddObjectAtTail(Obj.pCadObject);
	//---------------------- Start Point 1 -----------------------
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::STARTPOINT);
	Obj.pCadPoint->SetSubSubType(1);
	AddObjectAtTail(Obj.pCadObject);
	//------------------- End Point 1 ---------------------------
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::ENDPOINT);
	Obj.pCadPoint->SetSubSubType(1);
	AddObjectAtTail(Obj.pCadObject);
	//--------------- Start Point 2 -----------------------------
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::STARTPOINT);
	Obj.pCadPoint->SetSubSubType(2);
	AddObjectAtTail(Obj.pCadObject);
	//-------------------- End Point 2 -----------------------------
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::ENDPOINT);
	Obj.pCadPoint->SetSubSubType(2);
	AddObjectAtTail(Obj.pCadObject);
	return TRUE;
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

void CCadHoleRnd2Flat::Save(
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

void CCadHoleRnd2Flat::Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale)
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
	CADObjectTypes ObjCenter;
	CADObjectTypes ObjStart1, ObjEnd1;
	CADObjectTypes ObjStart2, ObjEnd2;
	double Radius;
	int Lw;
	double dS;


	if (IsRenderEnabled())
	{
		Radius = GetAttributes().m_HoleRadius;
		dS = Radius / 3.0;
		//----- Get Points that define this object --------------------------
		ObjCenter.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0);
		ObjStart1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::STARTPOINT, 1);
		ObjEnd1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ENDPOINT, 1);
		ObjStart2.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::STARTPOINT, 2);
		ObjEnd2.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ENDPOINT, 2);
		//----------------------------------------------------------
		Lw = GETAPP.RoundDoubleToInt(GetAttributes().m_LineWidth * Scale.GetScaleX());
		if (Lw < 1) Lw = 1;

		CreateThePen(mode, &penLine, Lw);
		switch (mode.PaintMode)
		{
		case MODE::ObjectPaintMode::FINAL:
		case MODE::ObjectPaintMode::SKETCH:
			pOldPen = pDC->SelectObject(&penLine);
			ObjCenter.pCadPoint->ToPixelArc(
				Radius,
				Radius,
				ObjStart1.pCadPoint,
				ObjEnd1.pCadPoint,
				pDC,
				LLHC,
				Scale
			);
			ObjCenter.pCadPoint->ToPixelArc(
				Radius,
				Radius,
				ObjEnd2.pCadPoint,
				ObjStart2.pCadPoint,
				pDC,
				LLHC,
				Scale
			);
			ObjEnd1.pCadPoint->LineFromHereToThere(
				ObjEnd2.pCadPoint,
				pDC,
				LLHC,
				Scale
			);
			ObjStart1.pCadPoint->LineFromHereToThere(
				ObjStart2.pCadPoint,
				pDC,
				LLHC,
				Scale
			);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(dS, dS), pDC, LLHC, Scale);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(-dS, dS), pDC, LLHC, Scale);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(dS, -dS), pDC, LLHC, Scale);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(-dS, -dS), pDC, LLHC, Scale);

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
	double Yoff;

	Radius = GetAttributes().m_HoleRadius;
	FlatDist = GetAttributes().m_FlatDistanceFromCenter;
	Yoff = sqrt(Radius * Radius - FlatDist * FlatDist);
	ObjCenter.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0);
	ObjStart1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::STARTPOINT, 1);
	ObjEnd1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ENDPOINT, 1);
	ObjStart2.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::STARTPOINT, 2);
	ObjEnd2.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ENDPOINT, 2);
	//--------------------------------
	// Start First Flat
	//--------------------------------
	ObjStart1.pCadPoint->SetX(ObjCenter.pCadPoint->GetX() + FlatDist);
	ObjStart1.pCadPoint->SetY(ObjCenter.pCadPoint->GetY() - Yoff);
	//--------------------------------
	//	First End Point
	//--------------------------------
	ObjEnd1.pCadPoint->SetX(ObjCenter.pCadPoint->GetX() - FlatDist);
	ObjEnd1.pCadPoint->SetY(ObjCenter.pCadPoint->GetY() - Yoff);

	//--------------------------------
	// Start Second Flat
	//--------------------------------
	ObjStart2.pCadPoint->SetX(ObjCenter.pCadPoint->GetX() + FlatDist);
	ObjStart2.pCadPoint->SetY(ObjCenter.pCadPoint->GetY() + Yoff);
	//--------------------------------
	//	Second End Point
	//--------------------------------
	ObjEnd2.pCadPoint->SetX(ObjCenter.pCadPoint->GetX() - FlatDist);
	ObjEnd2.pCadPoint->SetY(ObjCenter.pCadPoint->GetY() + Yoff);
}

BOOL CCadHoleRnd2Flat::IsPointEnclosed(DOUBLEPOINT p)
{
	return 0;
}

int CCadHoleRnd2Flat::PointInObjectAndSelect(
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

CString& CCadHoleRnd2Flat::GetTypeString()
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

CCadObject * CCadHoleRnd2Flat::Copy()
{
	//---------------------------------------------------
	// Copy
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadHoleRnd2Flat* pHR2F = new CCadHoleRnd2Flat;
	pHR2F->Create(GetParent(), GetSubType());
	CCadObject::CopyObject(pHR2F);
	return pHR2F;
}

void CCadHoleRnd2Flat::CopyAttributes(CCadObject* pToObj)
{
	((CCadHoleRnd2Flat*)pToObj)->CopyAttributesFrom(GetPtrToAttributes());
}

CDoubleSize CCadHoleRnd2Flat::GetSize()
{
	//---------------------------------------------------
	// GetSize
	//	Get the size of the object.  Returns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	double Radius = GetHoleRadius();
	return CDoubleSize(Radius,Radius);
}

int CCadHoleRnd2Flat::Parse(
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
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0);
		Obj.pCadPoint->SetPoint(MousePos);
		SolveIntersection();
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GetParent()->AddObjectAtTail(this);
		Obj.pCadHoleRnd2Flat = new CCadHoleRnd2Flat;
		Obj.pCadHoleRnd2Flat->Create(GetParent(),GetSubType());
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
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0);
		Obj.pCadPoint->SetPoint(MousePos);
		GETVIEW->Invalidate();
		break;
	}
	SolveIntersection();
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
	switch (mode.PaintMode)
	{
	case MODE::ObjectPaintMode::FINAL:
		if(IsSelected())
			pen->CreatePen(PS_SOLID, Lw, GetAttributes().m_colorSelected);
		else
			pen->CreatePen(PS_SOLID, Lw, GetAttributes().m_colorLine);
		break;
	case MODE::ObjectPaintMode::SKETCH:
		pen->CreatePen(PS_DOT, Lw, GetAttributes().m_colorSelected);
		break;
	}
}