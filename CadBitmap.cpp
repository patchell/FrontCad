#include "pch.h"

CCadBitmap::CCadBitmap():CCadObject()
{
	m_pBM = 0;
	SetType(ObjectType::BITMAP);
	if (!m_AttributesGood)
	{
		m_AttributesGood = TRUE;
		m_LastAttributes.CopyFrom(GETAPP.GetBitmapAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}

CCadBitmap::CCadBitmap(CCadBitmap &v) :CCadObject()
{
	m_pBM = 0;
	m_P1 = v.m_P1;
	m_P2 = v.m_P2;
	m_csBMFileName = v.m_csBMFileName;
	GetAttributes().m_MaintainAspectRatio = TRUE;
//	LoadImageW(m_csBMFileName);
	SetType(ObjectType::BITMAP);
}

CCadBitmap::~CCadBitmap()
{
	if (m_pBM) delete m_pBM;
}

void CCadBitmap::Move(CDoubleSize Diff)
{
	//***************************************************
	//	Move
	//		This Method is used to move the object
	// by the amount that is passed.
	//
	// parameters:
	//	p.......amount to move the object by
	//
	// return value: none
	//--------------------------------------------------
	m_P1 += Diff;
	m_P2 += Diff;
}

void CCadBitmap::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
{
	//***************************************************
	// Save
	//		This Method save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
	char* Indent_1 = new char[256];
	char* String = new char[256];
	fprintf(pO, "%s%s(\n",
		GETAPP.MkIndentString(Indent_1, Indent, ' '),
		CLexer::TokenToString(DocFileParseToken::BITMAP)
	);
	m_P1.Save(pO, DocFileParseToken::POINT, Indent + 1, flags);
	fprintf(pO, ",");
	m_P2.Save(pO, DocFileParseToken::POINT, Indent + 1, flags);
	fprintf(pO, ",\n");
	fprintf(pO, "%s(\"%s\"))\n",
		CLexer::TokenToString(DocFileParseToken::STRING),
		GETAPP.ConvertCStringToChar(String, GetBitMapFileName())
	);
	m_Attributes.Save(
		pO, 
		DocFileParseToken::ATTRIB_BITMAP,
		Indent + 1,
		flags
	);
}

void CCadBitmap::SetVertex(int Vi, CDoubleSize sz)
{
	//***************************************************
	// SetVertex
	//	This Method is used to change the position of
	// a vertex.
	//
	// parameters:
	// Vi.....index of the vertex
	// p......Amnount to change the vertex by
	//
	// return value: none
	//--------------------------------------------------
	if (Vi)
		m_P2 += sz;
	else
		m_P1 += sz;
	if (IsAspectRationMaintained())
		RestoreAspectRatio();
}


int CCadBitmap::GrabPoint(CDoublePoint  point)
{
	//***************************************************
	// GrabPoint
	//	This Method checks for a vertex at point p
	//
	// parameters:
	//	point.....point to check for presence of a vertex
	//
	// return value:
	//	returns index of vertex if succesful
	//	returns -1 on fail
	//--------------------------------------------------
	int rV = -1;

	if (m_P1.IsPointOnTarget(point)) rV = 0;
	else if (m_P1.IsPointOnTarget(point)) rV = 1;
	return rV;
}


void CCadBitmap::Draw(CDC* pDC, MODE mode, CSize Offset, CScale Scale)
{
	//***************************************************
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
	CPen pen, *oldpen;
	CRect rect;
	CPoint P1, P2;

	if (CCadBitmap::IsRenderEnabled())
	{
		if (mode.DrawMode == ObjectDrawMode::SKETCH || GetLastMode().DrawMode == ObjectDrawMode::SKETCH)
		{
			double AspectRatioBM;
			AspectRatioBM = GetAttributes().m_BitmapSize.dCY / GetAttributes().m_BitmapSize.dCX;
			m_P2.dY = AspectRatioBM * abs(m_P2.dX - m_P1.dX) + m_P1.dY;
		}
		P1 = m_P1.ToPixelPoint(Offset, Scale);
		P2 = m_P1.ToPixelPoint(Offset, Scale);
		rect.SetRect(P1, P2);

		switch (mode.DrawMode)
		{
		case ObjectDrawMode::FINAL:
		{
			CDC bmDC;
			CBitmap *pOld;
			bmDC.CreateCompatibleDC(pDC);
			pOld = bmDC.SelectObject(this->m_pBM);
			pDC->StretchBlt(
				P1.x, 
				P1.y, 
				rect.Width(), 
				rect.Height(), 
				&bmDC, 
				0, 
				0, 
				GetAttributes().m_BitmapSize.ToCSize().cx,
				GetAttributes().m_BitmapSize.ToCSize().cy, 
				SRCCOPY
			);
			bmDC.SelectObject(pOld);
		}
		break;
		case ObjectDrawMode::SELECTED:
		{
			CPen bPen;
			CRect Vrect;
			bPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			pen.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
			oldpen = pDC->SelectObject(&pen);
			CDC bmDC;
			CBitmap *pOld;
			bmDC.CreateCompatibleDC(pDC);
			pOld = bmDC.SelectObject(this->m_pBM);
			pDC->StretchBlt(
				P1.x, 
				P1.y, 
				rect.Width(), 
				rect.Height(), 
				&bmDC, 
				0, 
				0, 
				GetAttributes().m_BitmapSize.ToCSize().cx,
				GetAttributes().m_BitmapSize.ToCSize().cy,
				SRCCOPY
			);
			pDC->MoveTo(P1);
			pDC->LineTo(P1.x, P2.y);
			pDC->LineTo(P2);
			pDC->LineTo(P2.x, P1.y);
			pDC->LineTo(P1);
			pDC->SelectObject(&bPen);
			Vrect.SetRect(P1 + CSize(6, 6), P1 + CSize(-6, -6));
			pDC->Rectangle(Vrect);
			Vrect.SetRect(P2 + CSize(6, 6), P2 + CSize(-6, -6));
			pDC->Rectangle(Vrect);
			bmDC.SelectObject(pOld);
			pDC->SelectObject(oldpen);
		}
		break;
		case ObjectDrawMode::SKETCH:
			pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			oldpen = pDC->SelectObject(&pen);
			pDC->DrawDragRect(&rect, CSize(1, 1), NULL, CSize(1, 1));
			pDC->SelectObject(oldpen);
			break;
		}
		SetLastMode(mode);
	}
}

BOOL CCadBitmap::PtInBitmap(CDoublePoint p)
{
	BOOL rV;

	rV = CDoubleRect(m_P1, m_P2).PointInRectangle(p);
	return rV;
}

int CCadBitmap::PointInObjectAndSelect(
	CDoublePoint p, 
	CCadObject ** ppSelList , 
	int index, 
	int n, 
	DrawingCheckSelectFlags flag
)
{
	//***************************************************
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
	//	flag........Determines what sort of objects selected
	//
	// return value:
	//	returns true if point is within object
	//	otherwise, false
	//--------------------------------------------------
	if (index < n || n == 0)
	{
		//---------------------------------------
		// is point in Ellipse
		//---------------------------------------
		if (PtInBitmap(p))
		{
			if (ppSelList)
			{
				switch (flag)
				{
				case DrawingCheckSelectFlags::FLAG_ALL:
					ppSelList[index++] = this;
					break;
				case DrawingCheckSelectFlags::FLAG_UNSEL:
					if (!IsSelected())
						ppSelList[index++] = this;
					break;
				case DrawingCheckSelectFlags::FLAG_SEL:
					if (IsSelected())
						ppSelList[index++] = this;
					break;
				}
			}
			else
			{
				switch (flag)
				{
				case DrawingCheckSelectFlags::FLAG_ALL:
					index = 1;
					break;
				case DrawingCheckSelectFlags::FLAG_UNSEL:
					if (!IsSelected())
						index = 1;
					break;
				case DrawingCheckSelectFlags::FLAG_SEL:
					if (IsSelected())
						index = 1;
					break;
				}

			}
		}
	}
	return index;
}

CDoublePoint CCadBitmap::GetReference()
{
	//***************************************************
	// GetReference
	//	This Method returns the reference point for
	// the object
	// parameters:none
	//
	// return value:reference point
	//--------------------------------------------------
	return m_P1;
}

void CCadBitmap::AdjustReference(CDoubleSize Ref)
{
	//***************************************************
	// AdjustReference
	//	Change the reference point for an object.  This
	// operation needs to change everything else that
	// is referenced to this ppoint as well.
	// parameters:
	//	Ref.......How much to change reference by
	//
	// return value:
	//--------------------------------------------------
	m_P1 -= Ref;
	m_P2 -= Ref;
}

CDoubleRect& CCadBitmap::GetRect(CDoubleRect& rect)
{
	//***************************************************
	// GetRect
	//	Returns the rectangle that will enclose the
	// the object
	// parameters:
	//
	// return value:Returns the rectangle that encloses
	// the object
	//--------------------------------------------------
	rect = CDoubleRect(m_P1, m_P2);
	return rect;
}

CString& CCadBitmap::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csName = _T("Bitmap");
	return csName;
}

CCadBitmap CCadBitmap::operator=(CCadBitmap &v)
{
	//***************************************************
	// operator=
	//		Provides the Methodality when one object
	// value is assigned to another
	// parameters:
	//	v......reference to object to get value(s) from
	//
	// return value:this
	//--------------------------------------------------
	m_pBM = new CMyBitmap();
	m_P1 = v.m_P1;
	m_P2 = v.m_P2;
	m_csBMFileName = v.GetBitMapFileName();
	GetAttributes().CopyFrom(v.GetPtrToAttributes());
	m_pBM->LoadImageBitmap(m_csBMFileName);
	GetAttributes().CopyFrom(v.GetPtrToAttributes());
	return *this;
}

CCadObject * CCadBitmap::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadBitmap *pBM = new CCadBitmap;
	*pBM = *this;
	pBM->CopyAttributesFrom(&m_LastAttributes);
	return pBM ;
}

CDoublePoint& CCadBitmap::GetCenter(CDoublePoint& Center)
{
	//***************************************************
	// GetCenter
	//	Get the point at the "center" of the object.
	// parameters:
	//
	// return value:the center point
	//--------------------------------------------------
	CDoubleRect rect = CDoubleRect(m_P1, m_P2);
	Center = rect.GetCenter(Center);
	return Center;
}

void CCadBitmap::ChangeCenter(CDoubleSize sz)
{
	//***************************************************
	// ChangeCenter
	//	Change the center position of the object
	// parameters:
	//	p......amount to change center by
	//
	// return value:
	//--------------------------------------------------
	m_P1 -= sz;
	m_P2 -= sz;
}

CDoubleSize& CCadBitmap::GetSize(CDoubleSize& size)
{
	//***************************************************
	// GetSize
	//	Get the size of the object.  Reutrns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	size = CDoubleRect(m_P1, m_P2).GetSize(size);
	return size;
}

void CCadBitmap::ChangeSize(CDoubleSize Sz)
{
	//***************************************************
	// ChangeSize
	//	Change the size of the object
	// parameters:
	//	sz.....size to change object by (not change to)
	// return value:
	//--------------------------------------------------
	m_P2.dX += Sz.dCX;
	m_P2.dY += Sz.dCY;
}

DocFileParseToken CCadBitmap::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
{
	//***************************************************
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

void CCadBitmap::CopyAttributesTo(SBitmapAttributes*pAttrib)
{
	/***************************************************
	*	GetAttributes
	*		This Method is used to copy the
	*	attributes from this object into one pointed to 
	*	by the parameter 
	*
	* Parameters:
	*	pAttrb.....pointer to attributes structure to copy
	***************************************************/
	GetAttributes().CopyTo(pAttrib);
}

void CCadBitmap::CopyAttributesFrom(SBitmapAttributes*pAttrib)
{
	/***************************************************
	*	CopyAttributesFrom
	*		This Method is used to copy the
	*	attributes pointed to by the parameter into
	*	this object
	*
	* Parameters:
	*	pAttrb.....pointer to attributes structure to copy
	***************************************************/
	GetAttributes().CopyFrom(pAttrib);
	ClearNeedsAttributes();
}

BOOL CCadBitmap::NeedsAttributes()
{
	return (m_AttributesGood == FALSE);
}

void CCadBitmap::ClearNeedsAttributes()
{
	m_AttributesGood = TRUE;
}

ObjectDrawState CCadBitmap::ProcessDrawMode(ObjectDrawState DrawState)
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
	CDoublePoint MousePos = GETVIEW()->GetCurrentMousePosition();

	switch (DrawState)
	{
	case ObjectDrawState::START_DRAWING:
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		break;
	case ObjectDrawState::END_DRAWING:
		Id = GETVIEW()->MessageBoxW(_T("Do you want to keep\nThe current\nAttributes?"), _T("Keep Or Toss"), MB_YESNO);
		if (IDYES == Id)
		{
			m_CurrentAttributes.CopyTo(&m_LastAttributes);
		}
		break;
	case ObjectDrawState::SET_ATTRIBUTES:
		Id = EditProperties();
		if (IDOK == Id)
		{
			CopyAttributesTo(&m_CurrentAttributes);
		}
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		GETVIEW()->EnableAutoScroll(TRUE);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		DrawState = ObjectDrawState::ROTATE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Arrow:Place Rotation Point"));
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		DrawState = ObjectDrawState::PLACE_LBUTTON_UP;
		break;
	case ObjectDrawState::PLACE_LBUTTON_UP:
		GETVIEW()->EnableAutoScroll(FALSE);
		GETVIEW()->AddObjectAtFrontIntoDoc(this);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Arrow:Locate Arrow Tip Point"));
		GETVIEW()->Invalidate();
		break;
	}
	return DrawState;
}

ObjectDrawState CCadBitmap::MouseMove(ObjectDrawState DrawState)
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
	CDoublePoint MousePos;

	switch (DrawState)
	{
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		m_P2 = MousePos;
		break;
	}
	GETVIEW()->Invalidate();
	return DrawState;
}

int CCadBitmap::EditProperties()
{
	return 0;
}


void CCadBitmap::LoadBitmapImage(CString csPath)
{
	if (m_pBM) delete m_pBM;
	m_pBM = new CMyBitmap;
	m_pBM->LoadImageBitmap(csPath);
	m_csBMFileName = csPath;
	BITMAP bm;
	m_pBM->GetBitmap(&bm);
	GetAttributes().m_BitmapSize = CDoubleSize(int(bm.bmWidth), int(bm.bmHeight));
}

void CCadBitmap::RestoreAspectRatio()
{
	CPoint P1, P2;
	double AspectRatioBM;
	AspectRatioBM = GetAttributes().m_BitmapSize.dCY / GetAttributes().m_BitmapSize.dCX;
	m_P2.dY = AspectRatioBM * abs(m_P2.dX - m_P1.dX) + m_P1.dY;
	GetAttributes().m_MaintainAspectRatio = TRUE;
}
