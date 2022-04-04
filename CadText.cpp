#include "pch.h"

CCadText::CCadText():CCadObject()
{
	m_pFont = 0;
	m_pSelPen = 0;
	m_LastScaleX = 0.0;
	if (!m_AttributesGood)
	{
		m_AttributesGood = TRUE;
		m_LastAttributes.CopyFrom(GETAPP.GetTextAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}

CCadText::CCadText(CCadText& v):CCadObject()
{

	m_pFont = 0;
	m_pSelPen = 0;
	m_P1 = v.m_P1;
	GetAttributes().CopyFrom(v.GetPtrToAttributes());
	m_rectSelect = v.m_rectSelect;
	SetType(ObjectType::TEXT);
}

CCadText::~CCadText()
{
	if (m_pFont) delete m_pFont;
}

void CCadText::Move(CDoubleSize Diff)
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
	CDoubleRect rect = GetTextRectangle(rect);
	Rotate(GetAngle(), rect);
}

void CCadText::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
{
	//***************************************************
	// Save
	//		This Method save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
	char* pTempString = new char[256];

	fprintf_s(pO, "%s%s(\"%s\",",
		GETAPP.MkIndentString(pTempString, Indent),
		CLexer::TokenToString(DocFileParseToken::TEXT),
		GETAPP.ConvertCStringToChar(pTempString, m_csText)
	);
	m_P1.Save(pO, DocFileParseToken::POINT, Indent + 1, flags);
	GETAPP.SaveString(pO, Indent + 1, m_csText, DocFileParseToken::TEXT);
	GetAttributes().Save(pO, Indent + 1, flags);

	delete[]pTempString;
}

void CCadText::SetVertex(int v, CPoint p)
{
	//***************************************************
	// SetVertex
	//	This Method is used to change the position of
	// a vertex.
	//
	// parameters:
	// v......index of the vertex
	// p......Amnount to change the vertex by
	//
	// return value: none
	//--------------------------------------------------
}


int CCadText::GrabPoint(CDoublePoint p)
{
	//***************************************************
	// GrabPoint
	//	This Method checks for a vertex at point p
	//
	// parameters:
	//	p.....point to check for presence of a vertex
	//
	// return value:
	//	returns index of vertex if succesful
	//	returns -1 on fail
	//--------------------------------------------------
	return 0;
}


void CCadText::Draw(CDC * pDC, MODE mode, CSize Offset, CScale Scale)
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
	CFont *pOldFont;
	CPen *pOldPen;
	COLORREF OldColor, OldBk;
	int FontHeight, FontWidth;
	CPoint P1;
	int OldMode;

	if (CCadText::m_RenderEnable)
	{
		P1 = m_P1.ToPixelPoint(Offset, Scale);
		FontHeight = GETAPP.RoundDoubleToInt(Scale.GetScaleX() * GetFontHeight());
		FontWidth = GETAPP.RoundDoubleToInt(Scale.GetScaleX() * GetFontWidth());
		if (IsLastModeSame(mode) || IsDirty() || (m_LastScaleX != Scale.GetScaleX()))
		{
			CDoubleRect rect = GetTextRectangle(pDC,rect,  Scale);
			Rotate(GetAngle(), rect);
			switch (mode.DrawMode)
			{
			case ObjectDrawMode::SELECTED:
				if (m_pSelPen == 0)
				{
					m_pSelPen = new CPen;
					m_pSelPen->CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
				}
			case ObjectDrawMode::SKETCH:
			case ObjectDrawMode::FINAL:
				if (this->m_pFont) delete m_pFont;
				m_pFont = new CFont;
				if (Scale.GetScaleY() > 0.0)
					m_pFont->CreateFontW(
						GETAPP.RoundDoubleToInt(GetFontHeight() * Scale.GetScaleY()),
						GETAPP.RoundDoubleToInt(GetFontWidth() * Scale.GetScaleX()),
						GETAPP.RoundDoubleToInt(GetAngle() * 10.0),
						GETAPP.RoundDoubleToInt(GetAngle() * 10.0),
						GetFontWeight(),
						0, 0, 0,
						DEFAULT_CHARSET, 
						OUT_CHARACTER_PRECIS,
						CLIP_CHARACTER_PRECIS, 
						PROOF_QUALITY, 
						DEFAULT_PITCH,
						GetFontName()
					);
				else
					m_pFont->CreateFont(
						GETAPP.RoundDoubleToInt(GetFontHeight() * Scale.GetScaleY()),
						GETAPP.RoundDoubleToInt(GetFontWidth() * Scale.GetScaleX()),
						GETAPP.RoundDoubleToInt(-GetAngle() * 10.0),
						GETAPP.RoundDoubleToInt(-GetAngle() * 10.0),
						GetFontWeight(),
						0, 0, 0, 
						DEFAULT_CHARSET, 
						OUT_CHARACTER_PRECIS,
						CLIP_CHARACTER_PRECIS,
						PROOF_QUALITY, 
						DEFAULT_PITCH,
						GetFontName()
					);

				break;
			}
			SetDirty(0);
			SetLastMode( mode);
			m_LastScaleX = Scale.GetScaleX();
		}
		OldColor = pDC->SetTextColor(GetTextColor());
		OldBk = pDC->SetBkColor(GetBackgroundColor());
		pOldFont = pDC->SelectObject(m_pFont);
		if (GetTransparent())
			OldMode = pDC->SetBkMode(TRANSPARENT);
		else
			OldMode = pDC->SetBkMode(OPAQUE);
		pDC->TextOutW(P1.x, P1.y, GetText());
		pDC->SetBkMode(OldMode);
		if (mode.DrawMode == ObjectDrawMode::SELECTED)
		{
			pOldPen = pDC->SelectObject(m_pSelPen);
			m_rectSelect.Draw(pDC, mode, Offset, Scale);
			pDC->SelectObject(pOldPen);
		}
		pDC->SetBkColor(OldBk);
		pDC->SetTextColor(OldColor);
		pDC->SelectObject(pOldFont);
	}
}

BOOL CCadText::PointInObjectAndSelect(
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
		if (m_rectSelect.PointInRectangle (p))
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

CDoublePoint CCadText::GetReference()
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

void CCadText::AdjustReference(CDoubleSize Ref)
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
}

CDoubleRect& CCadText::GetRect(CDoubleRect& rect)
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

	return GetTextRectangle(rect);
}

CString& CCadText::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csTypeName = CString(_T("Text"));
	return csTypeName;
}

CCadText CCadText::operator=(CCadText &v)
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
	m_pFont = 0;
	m_pSelPen = 0;
	m_P1 = v.m_P1;
	CopyAttributesFrom(&v.GetAttributes());
	m_rectSelect = v.m_rectSelect;
	return *this;
}

CCadObject * CCadText::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadText *pCT = new CCadText;
	*pCT = *this;
	return pCT;
}

void CCadText::SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------

}

void CCadText::RenderEnable(int e)
{
	//***************************************************
	// RenderEnable
	//	chhanges the state of the render enable flag.
	// The base class does not contain this flag.
	// The render enable flag is a static member of
	// the derived class.
	// parameters:
	//	e......new state of enable flag
	//
	// return value:
	//--------------------------------------------------
	CCadText::m_RenderEnable = e;
}

CDoublePoint& CCadText::GetCenter(CDoublePoint& Center)
{
	//***************************************************
	// GetCenter
	//	Get the point at the "center" of the object.
	// parameters:
	//
	// return value:the center point
	//--------------------------------------------------
	CDoubleRect rect;
	rect = GetRect(rect);
	Center = rect.GetCenter(Center);
	return Center;
}

void CCadText::ChangeCenter(CDoubleSize sz)
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
}

CDoubleSize& CCadText::GetSize(CDoubleSize& size)
{
	//***************************************************
	// GetSize
	//	Get the size of the object.  Reutrns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	CDoubleRect rect;

	rect = GetRect(rect);
	size = rect.GetSize(size);
	return size;
}

void CCadText::ChangeSize(CSize Sz)
{
	//***************************************************
	// ChangeSize
	//	Change the size of the object
	// parameters:
	//	sz.....size to change object by (not change to)
	// return value:None
	//--------------------------------------------------
}

DocFileParseToken CCadText::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
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
	Token = pLex->Accept(Token, DocFileParseToken::TEXT);
	Token = pLex->Accept(Token, DocFileParseToken('('));
	int loop = 1;
	CString csError,csTemp;

	while (loop)
	{
		switch (Token)
		{
		case DocFileParseToken::STRING:
			csTemp = CString(pLex->GetLexBuff());
			SetText(csTemp);
			Token = pLex->Accept(Token, DocFileParseToken::STRING);
			break;
		case DocFileParseToken::POINT:
			Token = pLex->Point(DocFileParseToken::POINT, m_P1, Token);
			break;
		case DocFileParseToken::COLOR:
			Token = pLex->Color(DocFileParseToken::COLOR, GetAttributes().m_colorText, Token);
			break;
		case DocFileParseToken::BKCOLOR:
			Token = pLex->Color(DocFileParseToken::BKCOLOR, GetAttributes().m_colorBK, Token);
			break;
		case DocFileParseToken::FONT:
			Token = pLex->csString(DocFileParseToken::FONT, GetAttributes().m_csFontName, Token);
			break;
		case DocFileParseToken::WEIGHT:
			Token = pLex->ReadUINTDecimalValue(
				DocFileParseToken::WEIGHT, 
				GetAttributes().m_fontWeight, 
				Token
			);
			break;
		case DocFileParseToken::HEIGHT:
			Token = pLex->ReadDoubleValue(
				DocFileParseToken::HEIGHT, 
				GetAttributes().m_fontHeight, 
				Token
			);
			break;
		case DocFileParseToken::WIDTH:
			Token = pLex->ReadDoubleValue(
				DocFileParseToken::WIDTH, 
				GetAttributes().m_fontWidth, 
				Token
			);
			break;
		case DocFileParseToken::ANGLE:
			Token = pLex->ReadDoubleValue(DocFileParseToken::ANGLE, GetAttributes().m_Angle, Token);
			break;
		case DocFileParseToken::TRANSPARENTToken:
			Token = pLex->ReadUINTDecimalValue(
				DocFileParseToken::TRANSPARENTToken, 
				GetAttributes().m_Transparent, 
				Token
			);
			break;
		case DocFileParseToken(','):
			Token = pLex->Accept(Token, DocFileParseToken(','));
			break;
		case DocFileParseToken(')'):
			loop = 0;
			Token = pLex->Accept(Token, DocFileParseToken(')'));
			break;
		default:
			loop = 0;
			csError.Format(_T("Unexpected Token %lS"), CString(pLex->TokenToString(Token)).GetBuffer());
			pLex->Error(csError);
			break;
		}
	}
	return Token;
}

void CCadText::CopyAttributesTo(STextAttributes *pAttrib)
{
	/***************************************************
	*	CopyAttributesTo
	*		This Method is used to copy the
	*	attributes this object into an outside
	*	desitination
	*
	* Parameters:
	*	pAttrib.....pointer to attributes structure to copy to
	***************************************************/
	GetAttributes().CopyTo(pAttrib);
}

void CCadText::CopyAttributesFrom(STextAttributes *pAttrib)
{
	/***************************************************
	*	CopyAttributesFrom
	*		This Method is used to copy the
	*	attributes pointed to by the parameter into
	*	this object
	*
	* Parameters:
	*	pAttrib.....pointer to attributes structure to copy
	***************************************************/
	GetAttributes().CopyFrom(pAttrib);
	ClearNeedsAttributes();
}

ObjectDrawState CCadText::ProcessDrawMode(ObjectDrawState DrawState)
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
		GETVIEW()->EnableAutoScroll(TRUE);
		DrawState = ObjectDrawState::SET_ATTRIBUTES;
		GETAPP.UpdateStatusBar(_T("TEXT:Enter Text Parameters"));
		break;
	case ObjectDrawState::END_DRAWING:
		Id = GETVIEW()->MessageBoxW(_T("Do you want to keep\nThe current\nAttributes?"), _T("Keep Or Toss"), MB_YESNO);
		if (IDYES == Id)
		{
			m_CurrentAttributes.CopyTo(&m_LastAttributes);
		}
		GETVIEW()->EnableAutoScroll(FALSE);
		GETAPP.UpdateStatusBar(_T(""));
		break;
	case ObjectDrawState::SET_ATTRIBUTES:
		Id = EditProperties();
		if (IDOK == Id)
		{
			CopyAttributesTo(&m_CurrentAttributes);
		}
		GETAPP.UpdateStatusBar(_T("TEXT:Place Text on Document"));
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		m_P1 = MousePos;
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		m_P1 = MousePos;
		GETVIEW()->AddObjectAtFrontIntoDoc(this);
		GETVIEW()->SetObjectTypes(new CCadText);
		DrawState = ObjectDrawState::SET_ATTRIBUTES;
		GETAPP.UpdateStatusBar(_T("TEXT:Enter Text Parameters"));
		GETVIEW()->Invalidate();
		break;
	}
	return DrawState;
}


ObjectDrawState CCadText::MouseMove(ObjectDrawState DrawState)
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
	CDoublePoint MousePos = GETVIEW()->GetCurrentMousePosition();

	switch (DrawState)
	{
		case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
			m_P1 = MousePos;
			GETVIEW()->Invalidate();
			break;
	}
	return DrawState;
}

void CCadText::Rotate(double Angle, CDoubleRect& rect)
{
	///----------------------------------------------
	/// Rotate
	///		Thhis Method rotates the selection
	/// rectangle.
	///
	///		parameters:
	///			Angle....Angle to rotate by in degrees
	///			rect.....Reference to an Arbitray Rect that
	///						The rotated rectangle is put in
	///----------------------------------------------
	rect.Rotate(
		rect.GetPoint(RectPoint::UPPERLEFT), 
		Angle, 
		RectPoint::UPPERLEFT
	);
}

CDoubleRect& CCadText::GetTextRectangle(CDoubleRect& rect)
{
	///------------------------------------------
	/// GetTextRectangle
	///		This Method is used to get the
	/// rectangle that surrounds the text displayed
	/// bu the text object.
	///------------------------------------------
	CDC* pDC = GETVIEW()->GetDC();
	CScale InchesPerPixel = GETVIEW()->GetGrid().GetInchesPerPixel();
	rect = GetTextRectangle(pDC, rect, InchesPerPixel);
	GETVIEW()->ReleaseDC(pDC);
	return rect;
}

CDoubleRect& CCadText::GetTextRectangle(CDC *pDC, CDoubleRect& rect, CScale Scale)
{
	///------------------------------------------
	/// GetTextRectangle
	///		This Method is used to get the
	/// rectangle that surrounds the text displayed
	/// bu the text object.
	///
	///	parameters:
	///		pDC.....pointer to the device context
	///		Scale...Scale factor for display- PixelsPerInch
	/// returns:
	///		CdoubleRect that encloses the text t
	///------------------------------------------
	CFont font, *oldfont;
	font.CreateFontW(
		GETAPP.RoundDoubleToInt(GetFontHeight() * Scale.GetScaleX()),
		GETAPP.RoundDoubleToInt(GetFontWidth() * Scale.GetScaleX()), 
		0, 0,
		GetFontWeight(), 
		0, 0, 0, 
		DEFAULT_CHARSET, 
		OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS, 
		PROOF_QUALITY, DEFAULT_PITCH,
		GetFontName()
	);
	oldfont = pDC->SelectObject(&font);
	CDoubleSize cz = CDoubleSize(pDC->GetTextExtent(GetText()));
	pDC->SelectObject(oldfont);
	rect.SetPoints(m_P1, m_P1 + cz);
	return rect;
}

BOOL CCadText::NeedsAttributes()
{
	return (m_AttributesGood == FALSE);
}

void CCadText::ClearNeedsAttributes()
{
	m_AttributesGood = TRUE;
}

CDoublePoint CCadText::CalcTextShiftonRotation(
	CDoublePoint Pivot, 
	double Radius, 
	double angle
)
{
	///--------------------------------------------
	///	CalcTextShiftonRotation
	///		This Method is just a little complicated.
	/// What this does is it calculates the point to
	/// draw text at for a rotated text object.  This
	/// makes it so the text apears to be centered
	/// at point p1 at the given angle from the
	/// center.
	///
	/// parameters:
	///		Pivot......Point to rotate text about
	///		Radius.....Distance to the Center of Text from Pivot
	///		Angle......Angle in degree to rotate
	///
	/// returns:
	///		point at wich to paint text.
	///--------------------------------------------
	CDoubleSize rP;	// resulting point
	CDoubleRect TextRect = GetTextRectangle(TextRect);;
	CDoublePoint Center = TextRect.GetCenter(Center);

	TextRect.Rotate(Pivot, angle,RectPoint::UPPERLEFT);
	rP.dCX = Radius * cos(angle - 90.0);
	rP.dCY = Radius * sin(angle - 90.0);
	return m_P1 - rP;
}

void CCadText::UpdateText(void)
{
	CDoubleRect rect;

	rect = GetTextRectangle(rect);
	Rotate(GetAngle(), rect);
}

int CCadText::EditProperties(void)
{
	int ID;
	CDlgTextProperties Dlg;
	Dlg.SetTextObject(this);
	ID = Dlg.DoModal();
	return ID;
}
