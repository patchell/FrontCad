/****************************************************************
* CCadObject
*		This is the base class for all of the drawing objects
* They are all the same for this simple reason that any of
* them may exist in any document.  The idea is that they behave
* the same so as to make things just a bit easier on me while
* writing the program.  This class was derived from an earlier
* work that I called FrontCad.  This version is supposedely 
* much more object oriented.
****************************************************************/
#include "pch.h"

CCadObject::CCadObject()
{
	//***************************************************
	// CCadObject
	//	This is the default constructor
	// parameters:
	//	none
	// return value:
	//	none
	//--------------------------------------------------
	m_Type = ObjectType::BASE;
	m_Dirty = 0;
	m_Selected = 0;
	m_pNext = 0;
	m_pPrev = 0;
	m_pNextSel = 0;
	m_pPrevSel = 0;
}



CCadObject::~CCadObject()
{
	//***************************************************
	// ~CCadObject
	//		This is the base object destructor
	//
	// parameters:
	//	none
	// return value:
	//	none
	//--------------------------------------------------
}

void CCadObject::Move(CDoubleSize Diff)
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
}

void CCadObject::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
{
	//***************************************************
	// Save
	//		This Method save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//	Indent..Number of spaces to indent from left margin
	//	flags...Various flags to control output
	//
	// return value:none
	//--------------------------------------------------
}

void CCadObject::SetVertex(int v, CDoubleSize sz)
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


int CCadObject::GrabPoint(CDoublePoint p)
{
	//***************************************************
	// GrabPoint
	//	This Method checks for a vertex at point p
	//
	// parameters:
	//	p.....point to check for presence of a vertex
	//	scale....scale factor
	//
	// return value:
	//	returns index of vertex if succesful
	//	returns -1 on fail
	//--------------------------------------------------
	return -1;
}


void CCadObject::Draw(CDC* pDC, MODE mode, CSize Offset, CScale Scale)
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
}

int CCadObject::IsDirty(void)
{
	//***************************************************
	// IsDirty
	//	returns the status of the Dirty State of the
	// object
	// parameters:
	//
	// return value:
	//--------------------------------------------------
	return m_Dirty;
}

void CCadObject::SetDirty(int d)
{
	//***************************************************
	// SetDirty
	// Changes the state of the dirty flag
	// parameters:
	//	d.......new state of dirty flag.
	//
	// return value:
	//--------------------------------------------------
	m_Dirty = d;
}

int CCadObject::IsSelected()
{
	//***************************************************
	// IsSelected
	//	returns the state of the selected flag
	// parameters:
	//
	// return value:State of the selected flag
	//--------------------------------------------------
	return m_Selected;
}

void CCadObject::SetSelected(int Flag)
{
	//***************************************************
	// SetSelected
	//	Changes the state of the selcted flag.
	// parameters:
	// Flag......New state of the selected flag
	//
	// return value:
	//--------------------------------------------------
	m_Selected = Flag;
}

void CCadObject::AdjustReference(CDoubleSize Ref)
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

}


CString& CCadObject::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csTypeName = CString(_T("CCadOject Base Class"));
	return csTypeName;
}

CCadObject CCadObject::operator=(CCadObject &v)
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
	return CCadObject();
}

CCadObject * CCadObject::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	return new CCadObject;
}

void CCadObject::SetRect(CDoubleRect & rect, CDoublePoint P1, CDoublePoint P2, CDoubleSize Lw)
{
	//--------------------------------------------------
	// SetRect
	//	Just what is the method supposed to do?
	// parameters:
	//	rect.........reference to destination rect
	//	P1
	//	P2
	// Lw........... Size of Rectangle
	// return value:
	//--------------------------------------------------
	if (((P1.dX > P2.dX) && (P1.dY > P2.dY)))
	{
		CDoublePoint temp;
		temp = P2;
		P2 = P1;
		P1 = temp;
	}
	else if (((P1.dX > P2.dX) && (P1.dY < P2.dY)))
	{
		CDoublePoint t1, t2;
		t1 = P1;
		t2 = P2;
		P1.dX = t2.dX;
		P2.dX = t1.dX;
	}
	else if ((P1.dX < P2.dX) && (P1.dY > P2.dY))
	{
		CDoublePoint t1, t2;
		t1 = P1;
		t2 = P2;
		P1.dY = t2.dY;
		P2.dY = t1.dY;
	}
	rect = CDoubleRect(
		P1 + Lw, 
		P2 - Lw + CDoubleSize(1.0, 1.0)
	);
}

ObjectDrawState CCadObject::MouseMove(ObjectDrawState DrawState)
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
	return DrawState;
}

int CCadObject::EditProperties(void)
{
	return 0;
}
