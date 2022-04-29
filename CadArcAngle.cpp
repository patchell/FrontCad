#include "pch.h"

CCadArcAngle::CCadArcAngle(): CCadObject()
{
	m_Radius = 0;
	m_StartAngle = 0.0;
	m_EndAngle = 0.0;
	GetName().Format(_T("ARCANGLE_%d"), ++m_ArcCount);
	if (!m_AttributesGood)
	{
		m_AttributesGood = TRUE;
		m_LastAttributes.CopyFrom(GETAPP.GetArcAngleAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
//	CopyAttributesFrom(&m_CurrentAttributes);
}

CCadArcAngle::~CCadArcAngle()
{
}

BOOL CCadArcAngle::NeedsAttributes()
{
	return (m_AttributesGood == FALSE);
}

void CCadArcAngle::ClearNeedsAttributes()
{
	m_AttributesGood = TRUE;
}

ObjectDrawState CCadArcAngle::ProcessDrawMode(ObjectDrawState DrawState)
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
		break;
	case ObjectDrawState::END_DRAWING:
		if (m_AttributesDirty)
		{
			Id = GETVIEW()->MessageBoxW(_T("Do you want to keep\nThe current\nAttributes?"), _T("Keep Or Toss"), MB_YESNO);
			if (IDYES == Id)
			{
				m_CurrentAttributes.CopyTo(&m_LastAttributes);
			}
			m_AttributesDirty = FALSE;
		}
		break;
	case ObjectDrawState::SET_ATTRIBUTES:
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		GETVIEW()->EnableAutoScroll(TRUE);
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		break;
	case ObjectDrawState::PLACE_LBUTTON_UP:
		break;
	case ObjectDrawState::ARCSTART_LBUTTON_DOWN:
		break;
	case ObjectDrawState::ARCSTART_LBUTTON_UP:
		break;
	case ObjectDrawState::ARCEND_LBUTTON_DOWN:
		break;
	case ObjectDrawState::ARCEND_LBUTTON_UP:
		GETVIEW()->EnableAutoScroll(FALSE);
		break;
	}
	return DrawState;
}
