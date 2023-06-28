#include "pch.h"


CCadDimension::CCadDimension():CCadObject(ObjectType::DIMENSION)
{
	GetName().Format(_T("Dimension_%d"), ++m_DimensionCount);
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetDimensionAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}

void CCadDimension::OnCreate(void)
{
	++CCadDimension::m_DimensionCount;
	GetName().Format(_T("Dimension_%d"), m_DimensionCount);
}

CCadDimension::~CCadDimension()
{
}

void CCadDimension::Move(CDoubleSize Diff)
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

void CCadDimension::Save(FILE * pO, CLexer::Tokens Token, int Indent, int flags)
{
	//--------------------------------------------------
	// Save
	//		This Method save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
}

void CCadDimension::Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale)
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
	if (IsRenderEnabled())
	{
		if (GetHead())
		{
			CCadObject* pObj = GetHead();
			while (pObj)
			{
				pObj->Draw( pDC, mode, LLHC, Scale);
				pObj = pObj->GetNext();
			}
		}
	}
}

BOOL CCadDimension::PointInThisObject(DOUBLEPOINT point)
{
	return 0;
}

int CCadDimension::PointInObjectAndSelect(
	DOUBLEPOINT p,
	CCadObject* pExcludeObject,
	CCadObject** ppSelList,
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

CString& CCadDimension::GetTypeString(void)
{
	//--------------------------------------------------
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csType = _T("Dimension");
	return csType;
}

CString& CCadDimension::GetObjDescription()
{
	GetDescription().Format(_T("Base Obj Class"));
	return GetDescription();
}

CPoint CCadDimension::ScalePoint(CPoint p, int Scale, int ScaleDiv)
{
	//--------------------------------------------------
	// parameters:
	//
	// return value:
	//--------------------------------------------------
	return CPoint();
}

CCadObject * CCadDimension::CopyObject(void)
{
	//--------------------------------------------------
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadDimension *pCD = new CCadDimension;
	*pCD = *this;
	return pCD;
}

CLexer::Tokens CCadDimension::Parse(
	CLexer::Tokens Token,	// Lookahead Token
	CFileParser* pParser,	// pointer to parser
	CLexer::Tokens TypeToken // Token type to save object as
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
	return Token;
}

void CCadDimension::CopyAttributesFrom(SCadDimAttributes *pAttrib)
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

void CCadDimension::CopyAttributesTo(SCadDimAttributes* pAttrib)
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

int CCadDimension::EditProperties()
{
	CDlgDimensionPropertiex Dlg;
	int Id;

	Dlg.SetDimension(this);
	Id = Dlg.DoModal();
	return Id;
}

ObjectDrawState CCadDimension::ProcessDrawMode(ObjectDrawState DrawState)
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

	switch (DrawState)
	{
	case ObjectDrawState::START_DRAWING:
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
		CopyAttributesFrom(&m_CurrentAttributes);
		DrawState = ObjectDrawState::SELECT_OBJECT_TO_DIMENSION_LBUTTON_DOWN;
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
	case ObjectDrawState::SELECT_OBJECT_TO_DIMENSION_LBUTTON_DOWN:

		GETVIEW->EnableAutoScroll(TRUE);
		DrawState = ObjectDrawState::SELECT_OBJECT_TO_DIMENSION_LBUTTON_UP;
		GETAPP.UpdateStatusBar(_T("DRAG to Extend"));
		break;
	case ObjectDrawState::SELECT_OBJECT_TO_DIMENSION_LBUTTON_UP:
		DrawState = ObjectDrawState::EXTENSION_LINES_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("DRAG to Extend"));
		break;
	case ObjectDrawState::EXTENSION_LINES_LBUTTON_DOWN:
		DrawState = ObjectDrawState::EXTENSION_LINES_LBUTTON_UP;
		break;
	case ObjectDrawState::EXTENSION_LINES_LBUTTON_UP:
		DrawState = ObjectDrawState::SELECT_OBJECT_TO_DIMENSION_LBUTTON_DOWN;
		GETVIEW->EnableAutoScroll(FALSE);
		GetParent()->AddObjectAtTail(this);
		GETVIEW->SetObjectTypes(new CCadDimension);
		GETAPP.UpdateStatusBar(_T("Dimension:Select Line To Dimension"));
		GETVIEW->Invalidate();
		break;
	}
	return DrawState;
}

ObjectDrawState CCadDimension::MouseMove(ObjectDrawState DrawState)
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
	case ObjectDrawState::SELECT_OBJECT_TO_DIMENSION_LBUTTON_DOWN:
		break;
	case ObjectDrawState::EXTENSION_LINES_LBUTTON_DOWN:
		break;
	}
	return DrawState;
}

void CDimLine::Create(CCadPoint P1, CCadPoint P2, UINT LineType)
{
}

//----------------------------------------------------------------
// CDimLine, Derived from CCadLine
//----------------------------------------------------------------

void CDimLine::Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale)
{
	//--------------------------------------------------
	// Draw
	//	This Method draws the document to the device
	// parameters:
	//	pDC.....pointer to the device context
	//	mode....drawing mode
	//	LLHC....Offset to draw objects at
	//	Scale..Scale factor to draw objects at
	//
	// return value:none
	//--------------------------------------------------
	CPen* pOldPen, penLine;
	int Lw;
	CRect rect;
	CADObjectTypes ObjP1,ObjP2;

	if (IsRenderEnabled())
	{
		Lw = int(Scale.dSX * GetLineWidth());
		if (Lw < 1) Lw = 1;
		switch (mode.PaintMode)
		{
		case MODE::ObjectPaintMode::FINAL:
			if(IsSelected())
				penLine.CreatePen(PS_SOLID, Lw, GetAttributes().m_colorSelected);
			else
				penLine.CreatePen(PS_SOLID, Lw, GetAttributes().m_colorLine);
			pOldPen = pDC->SelectObject(&penLine);
			ObjP1.pCadObject = FindObject(
				ObjectType::POINT,
				CCadObject::SubTypes::VERTEX,
				1
			);
			ObjP2.pCadObject = FindObject(
				ObjectType::POINT,
				CCadObject::SubTypes::VERTEX,
				2
			);
			ObjP1.pCadPoint->MoveTo(pDC, LLHC, Scale);
			ObjP2.pCadPoint->LineTo(pDC, LLHC, Scale);
			pDC->SelectObject(pOldPen);
			break;
		case MODE::ObjectPaintMode::SKETCH:
			penLine.CreatePen(
				PS_DOT, 
				1, 
				GetAttributes().m_colorSelected
			);
			pOldPen = pDC->SelectObject(&penLine);
			ObjP1.pCadObject = FindObject(
				ObjectType::POINT,
				CCadObject::SubTypes::VERTEX,
				1
			);
			ObjP2.pCadObject = FindObject(
				ObjectType::POINT,
				CCadObject::SubTypes::VERTEX,
				2
			);
			ObjP1.pCadPoint->MoveTo(pDC, LLHC, Scale);
			ObjP2.pCadPoint->LineTo(pDC, LLHC, Scale);
			pDC->SelectObject(pOldPen);
			break;
		}	//end of switch(mode)
	}	//end of if(rederEnabled)
}


