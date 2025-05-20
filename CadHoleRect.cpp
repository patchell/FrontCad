#include "pch.h"


CCadHoleRect::CCadHoleRect():CCadObject(ObjectType::HOLE_RECTANGLE)
{
	GetName().Format(_T("RectangleHole_%d"), ++m_RectHoleCount);
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetRectHoleAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}
		
CCadHoleRect::~CCadHoleRect()
{
}

BOOL CCadHoleRect::Create(CCadObject* pParent, SubTypes type)
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

void CCadHoleRect::UpdateEnclosure()
{
}

void CCadHoleRect::Move(CDoubleSize Diff)
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

void CCadHoleRect::Save(
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

void CCadHoleRect::Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale)
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
	CADObjectTypes ObjCenter;
	double Delta = 0.075;
	DOUBLEPOINT CENTER;
	int Lw;
	CCadPoint P1, P2;
	double W2, H2;

	if (IsRenderEnabled())
	{
		brushFill.CreateStockObject(NULL_BRUSH);
		W2 = GetAttributes().m_HoleWidth / 2.0;
		H2 = GetAttributes().m_HoleHeight / 2.0;
		ObjCenter.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0);
		P1 = *ObjCenter.pCadPoint + CDoubleSize(-W2, -H2);
		P2 = *ObjCenter.pCadPoint + CDoubleSize(W2, H2);
		Lw = GETAPP.RoundDoubleToInt(Scale.dSX * GetAttributes().m_LineWidth);
		if (Lw < 1) Lw = 1;

		switch (mode.PaintMode)
		{
		case MODE::ObjectPaintMode::FINAL:
		case MODE::ObjectPaintMode::SKETCH:
			pOldBrush = pDC->SelectObject(&brushFill);
			if(IsSelected())
				penLine.CreatePen(PS_SOLID, Lw, GetAttributes().m_colorSelected);
			else
				penLine.CreatePen(PS_SOLID, Lw, GetAttributes().m_colorLine);
			pOldPen = pDC->SelectObject(&penLine);
			P2.ToPixelRect(&P1, pDC, LLHC, Scale);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(Delta,Delta),pDC,LLHC,Scale);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(Delta, -Delta), pDC, LLHC, Scale);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(-Delta, Delta), pDC, LLHC, Scale);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(-Delta, -Delta), pDC, LLHC, Scale);
			pDC->SelectObject(pOldBrush);
			break;
		}
	}
}

BOOL CCadHoleRect::IsPointEnclosed(DOUBLEPOINT p)
{
	return 0;
}

BOOL CCadHoleRect::PointInThisObject(DOUBLEPOINT point)
{
	CADObjectTypes Obj;
	BOOL rV;
	Obj.pCadObject = FindObject(ObjectType::RECT, CCadObject::SubTypes::ARC_RECTSHAPE, 0);
	rV = Obj.pCadRect->PointInThisObject(point);
	return rV;
}

int CCadHoleRect::PointInObjectAndSelect(
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

CString& CCadHoleRect::GetTypeString()
{
	//---------------------------------------------------
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csName = _T("Rect Hole");
	return csName;
}

CString& CCadHoleRect::GetObjDescription()
{
	GetObjDescription().Format(_T("RectHole"));
	return GetObjDescription();
}

CCadObject * CCadHoleRect::Copy()
{
	//---------------------------------------------------
	// Copy
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CADObjectTypes newObj;
	newObj.pCadHoleRect = new CCadHoleRect;
	newObj.pCadHoleRect->Create(GetParent(), GetSubType());
	CopyObject(newObj.pCadObject);
	return newObj.pCadObject;
}

void CCadHoleRect::CopyAttributes(CCadObject* pToObj)
{
	((CCadHoleRect*)pToObj)->CopyAttributesFrom(GetPtrToAttributes());
}


CDoubleSize CCadHoleRect::GetSize()
{
	//---------------------------------------------------
	// GetSize
	//	Get the size of the object.  Returns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	return CSize();
}

void CCadHoleRect::Parse(
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
	//
	// return value:
	//	returns lookahead token on success, or
	//			negative value on error
	//--------------------------------------------------
}

void CCadHoleRect::CopyAttributesTo(SRectHoleAttributes * pAttrib)
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

void CCadHoleRect::CopyAttributesFrom(SRectHoleAttributes *pAttrib)
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

ObjectDrawState CCadHoleRect::ProcessDrawMode(ObjectDrawState DrawState)
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
		//---------------------------------------------
		// Create parts for the Rectangular Hole
		//---------------------------------------------
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Rectangular Hole:Place Center Point"));
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
			m_AttributesDirty = TRUE;
			CopyAttributesTo(&m_CurrentAttributes);
		}
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0);
		Obj.pCadPoint->SetPoint(MousePos);
		GetParent()->AddObjectAtTail(this);
		Obj.pCadHoleRect = new CCadHoleRect;
		GETVIEW->SetObjectTypes(Obj.pCadHoleRect);
		Obj.pCadHoleRect->Create(GetParent(), GetSubType());
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Rectangular Hole:Place Center Point"));
		GETVIEW->Invalidate();
		break;
	}
	return DrawState;
}

ObjectDrawState CCadHoleRect::MouseMove(ObjectDrawState DrawState)
{
	//-------------------------------------------------------
	// MouseMove
	//		This is the state machine for creating this
	//	object on the screen.  This Method is for when
	//	the left mouse is moved.
	//
	//	parameters:
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
	return DrawState;
}

int CCadHoleRect::EditProperties()
{
	int Id;
	CDlgSquareHoleProp Dlg;

	Dlg.SetRectHole(this);
	Id = Dlg.DoModal();
	return Id;
}
