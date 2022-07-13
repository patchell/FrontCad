#include "pch.h"


CCadHoleRound::CCadHoleRound():CCadObject()
{
	SetType(ObjectType::HOLE_ROUND);
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

void CCadHoleRound::Create()
{
	CADObjectTypes Obj;

	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create();
	Obj.pCadPoint->SetSubType(SubType::CENTERPOINT);
	Obj.pCadPoint->SetSubSubType(0);
	AddObjectAtTail(Obj.pCadObject);
}

BOOL CCadHoleRound::Destroy(CCadObject* pDependentObject)
{
	BOOL rV = TRUE;
	return rV;
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

void CCadHoleRound::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
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

void CCadHoleRound::Draw(CDC* pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale)
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
			pOldPen = pDC->SelectObject(&penLine);
			pDC->Ellipse(&rect);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(dS, dS), pDC, ULHC, Scale);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(-dS, dS), pDC, ULHC, Scale);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(dS, -dS), pDC, ULHC, Scale);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(-dS,- dS), pDC, ULHC, Scale);
			pDC->SelectObject(pOldPen);
			break;
		}
	}
}

BOOL CCadHoleRound::PointInThisObject(DOUBLEPOINT point)
{
	return 0;
}

int CCadHoleRound::PointInObjectAndSelect(
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

	ObjCenter.pCadObject = FindObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
	GetDescription().Format(_T("Round Hole(%7.3lf,%7.3lf)"),
		ObjCenter.pCadPoint->GetX(),
		ObjCenter.pCadPoint->GetY()
	);
	return GetDescription();
}

CCadObject * CCadHoleRound::CopyObject(void)
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
	pHR->Create();
	CCadObject::CopyObject(pHR);
	return pHR;
}

DocFileParseToken CCadHoleRound::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
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
	return Token;
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
		m_Center = MousePos;
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		Obj.pCadObject = FindObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
		Obj.pCadPoint->SetPoint(MousePos);
		GETVIEW->GetDocument()->AddObjectAtTail(this);
		Obj.pCadHoleRound = new CCadHoleRound;
		Obj.pCadHoleRound->Create();
		GETVIEW->SetObjectTypes(Obj.pCadHoleRound);
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

	switch (DrawState)
	{
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		m_Center = MousePos;
		GETVIEW->Invalidate();
		break;
	}
	return DrawState;
}

int CCadHoleRound::EditProperties(void)
{
	int Id;
	CDlgRoundHoleProp Dlg;

	Dlg.SetRoundHole(this);
	Id = Dlg.DoModal();
	return Id;

}


void CCadHoleRound::CreateThePen(MODE mode, CPen* pen, int Lw)
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