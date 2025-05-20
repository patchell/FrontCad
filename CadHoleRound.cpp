#include "pch.h"


CCadHoleRound::CCadHoleRound():CCadObject(ObjectType::HOLE_ROUND)
{
	GetName().Format(_T("RoundHole_%d"), ++m_RoundHoleCount);
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetRoundHoleAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}

CCadHoleRound::~CCadHoleRound()
{
}

BOOL CCadHoleRound::Create(CCadObject* pParent, SubTypes type)
{
	CADObjectTypes Obj;

	CCadObject::Create(pParent, type);
	if (pParent == NULL)
		pParent = this;
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::CENTERPOINT);
	AddObjectAtTail(Obj.pCadObject);
	return TRUE;
}

void CCadHoleRound::UpdateEnclosure()
{
}

void CCadHoleRound::Move(CDoubleSize Diff)
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

void CCadHoleRound::Save(
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

void CCadHoleRound::Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale)
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
	CBrush* pOldBrush, brushFill;
	CRect rect;
	CADObjectTypes ObjCenter;
	double Radius;
	int nRadius;
	int Lw;
	double dS;


	if (IsRenderEnabled())
	{
		Radius = GetAttributes().m_HoleRadius;
		dS = Radius / 3.0;
		nRadius = GETAPP.RoundDoubleToInt(Radius * Scale.GetScaleX());
		ObjCenter.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0);
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
			pOldPen = pDC->SelectObject(&penLine);
			pOldBrush = pDC->SelectObject(&brushFill);
			pDC->Ellipse(&rect);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(dS, dS), pDC, LLHC, Scale);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(-dS, dS), pDC, LLHC, Scale);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(dS, -dS), pDC, LLHC, Scale);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(-dS,- dS), pDC, LLHC, Scale);
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBrush);
			break;
		}
	}
}

BOOL CCadHoleRound::IsPointEnclosed(DOUBLEPOINT p)
{
	return 0;
}

BOOL CCadHoleRound::PointInThisObject(DOUBLEPOINT point)
{
	return 0;
}

int CCadHoleRound::PointInObjectAndSelect(
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

CString& CCadHoleRound::GetTypeString()
{
	//---------------------------------------------------
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csName = _T("Round Hole");
	return csName;
}

CString& CCadHoleRound::GetObjDescription()
{
	CADObjectTypes ObjCenter;

	ObjCenter.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0);
	GetDescription().Format(_T("Round Hole(%7.3lf,%7.3lf)"),
		ObjCenter.pCadPoint->GetX(),
		ObjCenter.pCadPoint->GetY()
	);
	return GetDescription();
}

CCadObject * CCadHoleRound::Copy()
{
	//---------------------------------------------------
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadHoleRound *pHR = new CCadHoleRound;
	pHR->Create(GetParent(), GetSubType());
	CCadObject::CopyObject(pHR);
	return pHR;
}

void CCadHoleRound::CopyAttributes(CCadObject* pToObj)
{
	((CCadHoleRound*)pToObj)->CopyAttributesFrom(GetPtrToAttributes());
}

void CCadHoleRound::Parse(
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
	//	TypeToken..Specifies Object Toke
	//
	// return value:
	//	returns lookahead token on success, or
	//			negative value on error
	//--------------------------------------------------
}

void CCadHoleRound::CopyAttributesTo(SRoundHoleAttributes *pAttrib)
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

void CCadHoleRound::CopyAttributesFrom(SRoundHoleAttributes *pAttrib)
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

ObjectDrawState CCadHoleRound::ProcessDrawMode(ObjectDrawState DrawState)
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
		GETAPP.UpdateStatusBar(_T("Round Hole:Place Center Point"));
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
		SetCurrentDrawState(DrawState);
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0);
		Obj.pCadPoint->SetPoint(MousePos);
		GetParent()->AddObjectAtTail(this);
		//-------------- Make New Object ----------------------
		Obj.pCadHoleRound = new CCadHoleRound;
		Obj.pCadHoleRound->Create(GetParent(), GetSubType());
		GETVIEW->SetObjectTypes(Obj.pCadHoleRound);
		((CCadPoint *)Obj.pCadHoleRound->FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, SUBSUBTYPE_ANY))->SetPoint(MousePos);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Round Hole:Place Center Point"));
		GETVIEW->Invalidate();
		break;
	}
	return DrawState;
}


ObjectDrawState CCadHoleRound::MouseMove(ObjectDrawState DrawState)
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
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, SUBSUBTYPE_ANY);
		Obj.pCadPoint->SetPoint(MousePos);
		GETVIEW->Invalidate();
		break;
	}
	return DrawState;
}

int CCadHoleRound::EditProperties()
{
	int Id;
	CDlgRoundHoleProp Dlg;

	Dlg.SetRoundHole(this);
	Id = Dlg.DoModal();
	return Id;

}


void CCadHoleRound::CreateThePen(MODE mode, CPen* pen, int Lw)
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
